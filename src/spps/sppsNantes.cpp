// sppsNantes.cpp : définit le point d'entrée pour l'application console.
//
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#ifdef _WIN32
        #include <crtdbg.h>
    #endif
	#include <input_output/gabe/gabe.h>
	#define __USE_MULTITHREAD__ 0
#else
	#define __USE_MULTITHREAD__ 1
#endif

#ifdef _PROFILE_
	#undef __USE_MULTITHREAD__
	#define __USE_MULTITHREAD__ 0
#endif

#include <coreinitialisation.h>
#include "CalculationCore.h"
#include "tools/dotdistribution.h"
#include "sppsInitialisation.h"
#include "sppsVersion.h"

#if __USE_MULTITHREAD__
	#include <boost/thread/thread.hpp>
	#include <boost/bind.hpp>

	boost::mutex sppsmutex; /*!< Variable permettant la synchronisation des processus */
#endif
    // Print warning if particle lost is superior than this ratio
const double PARTICLE_LOST_WARNING_RATIO = 0.05;
bool verbose_mode = false;

/**
 * @brief Structure contenant tout les outils voués au calcul de propagation
 */
struct t_ToolBox
{
	CalculationCore* calculationTool;
	ReportManager* outputTool;
	Core_Configuration* configurationTool;
	t_Mesh* sceneMesh;
	t_TetraMesh* tetraMesh;
	progressionInfo* mainProgressionOutput;
	CalculationCore::CONF_CALCULATION confCalc;
};

/**
 * Execute le calcul pour une source donné
 * @param applicationTools Panel d'objets permettant le calcul de propagation
 * @param sourceInfo Informations sur la source à utiliser pour l'émission
 * @param confPartFrame Informations de bases pour toutes les particules
 */
void runSourceCalculation( progressOperation* parentOperation, t_ToolBox& applicationTools, t_Source& sourceInfo, CONF_PARTICULE& confPartFrame, unsigned int freq)
{
	if(!sourceInfo.currentVolume)
	{
	    #if __USE_MULTITHREAD__
		boost::mutex::scoped_lock lock(sppsmutex);
		#endif
		std::cerr<<"Unable to find the source position!";
		return;
	}
	uentier quandparticules=*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::IPROP_QUANT_PARTICLE_CALCULATION);
	decimal rapportPartOutput=1;
	//Calcul du rapport particules à enregistrer sur le nombre de particules à calculer
	if(quandparticules>0)
		rapportPartOutput=(decimal)(*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::IPROP_QUANT_PARTICLE_OUTPUT))/(decimal)quandparticules;
	if(!(rapportPartOutput>=0 && rapportPartOutput<=1))
		rapportPartOutput=0;
	confPartFrame.energie=sourceInfo.bandeFreqSource[confPartFrame.frequenceIndex].w_j/quandparticules;
	//Calcul de l'energie epsilon pour les particules
	confPartFrame.energie_epsilon=confPartFrame.energie*pow(10.,-(l_decimal)(*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::FPROP_EPSILON_TRANSMISSION)));
	confPartFrame.currentTetra=sourceInfo.currentVolume;
	confPartFrame.sourceid=sourceInfo.idsource;
	decimal nomVecVitesse=applicationTools.configurationTool->GetNormVecPart(sourceInfo.Position,confPartFrame.currentTetra);
	decimal currentRapport=0;

	//Si la directivité de la source est unidirectionnelle
	if(sourceInfo.type==SOURCE_TYPE_UNIDIRECTION)
		confPartFrame.direction=sourceInfo.Direction*nomVecVitesse;
	if (sourceInfo.type == SOURCE_TYPE_DIRECTION && sourceInfo.directivity->asDataForFrequency(freq) == false)
	{
		if (verbose_mode) { std::cout << "No directivity data for frequency: " << freq << " => skipping" << std::endl; }
		return;
	}
	progressOperation thisSrcOperation(parentOperation,quandparticules);
	//Prise en compte du délai de la source
	confPartFrame.pasCourant=(uentier_court)ceil(sourceInfo.sourceDelay/(*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP)));
	confPartFrame.stateParticule=PARTICULE_STATE_ALIVE;
	if((*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::IPROP_QUANT_TIMESTEP))>confPartFrame.pasCourant)
	{
		int lastmill=-1;

		for(uentier idpart=1;idpart<=quandparticules;idpart++)
		{
			/*
			if(int(idpart/1000.)!=lastmill)
			{
				lastmill=int(idpart/1000.);
				std::cout<<"Solve "<<lastmill<<"xxx # particles."<<std::endl;
			}
			*/
			CONF_PARTICULE confPart=confPartFrame;
			if(sourceInfo.type==SOURCE_TYPE_OMNIDIRECTION)
				ParticleDistribution::GenSphereDistribution(confPart,nomVecVitesse);
			else if(sourceInfo.type==SOURCE_TYPE_XY)
				ParticleDistribution::GenXYDistribution(confPart,nomVecVitesse);
			else if(sourceInfo.type==SOURCE_TYPE_XZ)
				ParticleDistribution::GenXZDistribution(confPart,nomVecVitesse);
			else if(sourceInfo.type==SOURCE_TYPE_YZ)
				ParticleDistribution::GenYZDistribution(confPart,nomVecVitesse);
			else if (sourceInfo.type == SOURCE_TYPE_DIRECTION) 
			{
				ParticleDistribution::GenSphereDistribution(confPart, nomVecVitesse);
				// aténuation en fonction de la direction
				std::tuple<double, double> coord_sph = t_DirectivityBalloon::loudspeaker_coordinate(sourceInfo.Direction, confPart.direction);
				double phi = RadToDeg(std::get<0>(coord_sph));
				double theta = RadToDeg(std::get<1>(coord_sph));
				if (sourceInfo.directivity->asInterpolatedValue(freq, phi, theta))
				{
					double spl = sourceInfo.directivity->getInterpolatedValue(freq, phi, theta);
					confPart.energie *= pow(10, spl / 10);
				}
			}

			float lenPart=confPart.direction.length();
			confPart.position=sourceInfo.Position;
			currentRapport+=rapportPartOutput;
			if(currentRapport>=1)
			{
				confPart.outputToParticleFile=true;
				currentRapport=0;
			}else{
				confPart.outputToParticleFile=false;
			}

			applicationTools.outputTool->NewParticule(confPart);
			applicationTools.calculationTool->Run(confPart);

			applicationTools.outputTool->SaveParticule();


			//Calcul des particules issues de confPart
			//std::cout<<"Solve "<<applicationTools.confCalc.duplicatedParticles.size()<<" duplicated particles."<<std::endl;
			while(!applicationTools.confCalc.duplicatedParticles.empty())
			{
				confPart=applicationTools.confCalc.duplicatedParticles.front();
				applicationTools.confCalc.duplicatedParticles.pop_front();
				applicationTools.outputTool->NewParticule(confPart);
				applicationTools.calculationTool->Run(confPart);
				applicationTools.outputTool->SaveParticule();
			}


			#if __USE_MULTITHREAD__
				boost::mutex::scoped_lock lock(sppsmutex);
			#endif
			applicationTools.mainProgressionOutput->OutputCurrentProgression();
			//progressOperation thisPartOperation(&thisSrcOperation);
			thisSrcOperation.Next();
		}
	}
}


/**
 * Execute le calcul pour une frequence donné
 * @param applicationTools Panel d'objets permettant le calcul de propagation
 * @param freqInfo Informations sur la fréquence à utiliser pour l'émission
 * @param confPartFrame Informations de bases pour toutes les particules
 */
void runFrequenceCalculation(  progressOperation* parentOperation, ReportManager::t_ParamReport reportParameter, t_ToolBox applicationTools, t_sppsThreadParam* threadData, CONF_PARTICULE confPartFrame)
{
    using namespace std;
	//Reserve l'espace mémoire pour cette bande de fréquence
	InitRecepteurSBfreq(applicationTools.configurationTool->recepteur_s_List,threadData->freqInfos->freqIndex,*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::IPROP_QUANT_TIMESTEP));
	//Initialisation du gestionnaire de sortie de données
	reportParameter.freqIndex=threadData->freqInfos->freqIndex;
	reportParameter.freqValue=threadData->freqInfos->freqValue;
	if((applicationTools.configurationTool->recepteur_s_List.size()>0 || applicationTools.configurationTool->recepteur_scut_List.size()>0 ) && *applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ))
	{
		//Création du dossier pour le récepteur surfacique à cette fréquence
		reportParameter._recepteur_surf_Path+=stringClass::FromInt(reportParameter.freqValue)+stringClass(" Hz/");
		reportParameter._recepteur_surf_cut_Path=reportParameter._recepteur_surf_Path;
		//Création du dossier de fréquence
		st_mkdir(reportParameter._recepteur_surf_Path);
		//Ajout du nom du fichier à la fin
		reportParameter._recepteur_surf_Path+=*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_PATH);
		reportParameter._recepteur_surf_cut_Path+=*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH);
	}

	ReportManager outputTool(reportParameter);
	applicationTools.outputTool=&outputTool;
	CalculationCore calculationTool(*applicationTools.sceneMesh,*applicationTools.tetraMesh,applicationTools.confCalc,*applicationTools.configurationTool,&outputTool);
	applicationTools.calculationTool=&calculationTool;

	confPartFrame.frequenceIndex=threadData->freqInfos->freqIndex;
	//Pour chaque source
	progressOperation thisFreqOperation(parentOperation,applicationTools.configurationTool->srcList.size());

	for(std::size_t idsrc=0;idsrc<applicationTools.configurationTool->srcList.size();idsrc++)
	{
		runSourceCalculation(&thisFreqOperation,applicationTools,*applicationTools.configurationTool->srcList[idsrc],confPartFrame, threadData->freqInfos->freqValue);
	}
	//A partir d'ici les threads s'arretent puis continuent un par un.
	//Première ligne de code du processus en cours
	#if __USE_MULTITHREAD__
		if(true) //Cette branche conditionnelle existe afin que l'objet lock soit détruit à la fin du traitement de la condition
		{
			boost::mutex::scoped_lock lock(sppsmutex);
	#endif
		//Post traitement des récepteurs de surface
		ReportManager::SetPostProcessSurfaceReceiver(*applicationTools.configurationTool,threadData->freqInfos->freqIndex,applicationTools.configurationTool->recepteur_s_List,*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
		ReportManager::SetPostProcessCutSurfaceReceiver(*applicationTools.configurationTool,threadData->freqInfos->freqIndex,applicationTools.configurationTool->recepteur_scut_List,*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
		//Sauvegarde des récepteurs surfacique pour cette bande de fréquence
		if(applicationTools.configurationTool->recepteur_s_List.size()>0 &&  *applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ))
#ifdef UTILISER_MAILLAGE_OPTIMISATION
			ReportManager::SauveRecepteursSurfaciques(reportParameter._recepteur_surf_Path,threadData->freqInfos->freqIndex,applicationTools.configurationTool->recepteur_s_List,*applicationTools.tetraMesh,*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
#else
			ReportManager::SauveRecepteursSurfaciques(reportParameter._recepteur_surf_Path,threadData->freqInfos->freqIndex,applicationTools.configurationTool->recepteur_s_List,*applicationTools.sceneMesh,*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
#endif
		if(applicationTools.configurationTool->recepteur_scut_List.size()>0 &&  *applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ))
			ReportManager::SauveRecepteursSurfaciquesCoupe(reportParameter._recepteur_surf_cut_Path,applicationTools.configurationTool->recepteur_scut_List,*applicationTools.configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),true,false,threadData->freqInfos->freqIndex);
		outputTool.SaveAndCloseParticleFile();				//Finalisation du fichier de particule
		threadData->GabeColData=outputTool.GetColStats();	//Recupere les données des etats de particules
		threadData->GabeSumEnergyFreq=outputTool.GetSumEnergy();//Recupere les données du niveau sonore global
        threadData->particleStats = outputTool.statReport;  // Copy particle statistics
		outputTool.FillWithLefData(*threadData); //Recupere les données du lef (utilisé pour le calcul du LF et LFC)
		if (verbose_mode) { cout << "End of calculation at " << threadData->freqInfos->freqValue << " Hz." << endl; }


	#if __USE_MULTITHREAD__
		}
	#endif
}



int MainProcess(int argc, char* argv[])
{

	using namespace std;

	cout<<"SPPS version "<<SPPS_VERSION_MAJOR<<"."<<SPPS_VERSION_MINOR<<"."<<SPPS_VERSION_REVISION<<endl;
	//**************************************************
	//Initialisation
	t_ToolBox applicationToolBox;
	applicationToolBox.mainProgressionOutput=NULL;
	formatCoreBIN::ioModel modelEntree;
	t_Mesh sceneMesh;
	t_TetraMesh sceneTetraMesh;
	applicationToolBox.sceneMesh=&sceneMesh;
	applicationToolBox.tetraMesh=&sceneTetraMesh;

	//**************************************************
	//Verification des arguments
	string pathFichier;
	bool asParam = false;
	bool asFilePath = false;

	if(argc>1)
	{	
		for(int idarg=1; idarg<argc; idarg++)
		{
			string arg = argv[idarg];

			if (arg == "-v" && !asFilePath){ // params need to be before the filepath
				verbose_mode = true;
				asParam = true;
			}
			else if (pathFichier.empty()) {
				pathFichier.append(arg);
				asFilePath = true;
			}
			else {
				pathFichier.append(" ");
				pathFichier.append(arg);
			}
		}
	}

	if (!asFilePath)
	{
		cout << "The path of the XML configuration file must be specified!" << endl;
		return 1;
	}

	//**************************************************
	// 1: Lire le fichier XML
	if (verbose_mode) { cout << "XML configuration file is currently loading..." << endl; }
	Core_Configuration configManager(pathFichier, verbose_mode);
	applicationToolBox.configurationTool=&configManager;
	if (verbose_mode) { cout << "XML configuration file has been loaded." << endl; }

	//**************************************************
	// 2: Initialisation des variables
	CoreString workingDir=*configManager.FastGetConfigValue(Core_Configuration::SPROP_CORE_WORKING_DIRECTORY);
	CoreString sceneMeshPath=*configManager.FastGetConfigValue(Core_Configuration::SPROP_MODEL_FILE_PATH);
	sceneMeshPath=workingDir+sceneMeshPath;
	// Chargement du Random SEED
	entier seedValue = *configManager.FastGetConfigValue(Core_Configuration::I_PROP_RANDOM_SEED);
	if(seedValue!=0) {
		SetRandSeed((uint32_t)seedValue);
	}

	//**************************************************
	// 3: Chargement du modèle
	if(!initMesh(sceneMesh,workingDir,sceneMeshPath,configManager, verbose_mode))
		return 1;

	//**************************************************
	// 4: Chargement du maillage
	if(!initTetraMesh(workingDir+*configManager.FastGetConfigValue(Core_Configuration::SPROP_TETRAHEDRALIZATION_FILE_PATH),sceneMesh,configManager.freqList.size(),sceneTetraMesh,configManager, verbose_mode))
		return 1;

	// Attach the neighboring tetrahedron with all punctual receivers depending of punctual receiver radius.
    ExpandPunctualReceiverTetrahedronLocalisation(&sceneTetraMesh, &configManager.recepteur_p_List, configManager);
	TranslateSourceAtTetrahedronVertex(configManager.srcList,&sceneTetraMesh);
	if(!CheckSourcePosition(configManager.srcList, &sceneMesh)) {
		std::cerr << _("A sound source position is intersecting with the 3D model. Move the sound source inside the 3D model") << std::endl;
		return 1;
	}
	//**************************************************
	// 5: Instancier paramètre gestionnaire de sortie de données
	ReportManager::t_ParamReport reportParameter;
	reportParameter._particleFileName=*configManager.FastGetConfigValue(Core_Configuration::SPROP_PARTICULE_FILE_PATH);
	reportParameter._particlePath=workingDir+*configManager.FastGetConfigValue(Core_Configuration::SPROP_PARTICULE_FOLDER_PATH);
	reportParameter._recepteur_surf_Path=workingDir+*configManager.FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH);
	reportParameter.nbParticles=*configManager.FastGetConfigValue(Core_Configuration::IPROP_QUANT_PARTICLE_OUTPUT)*configManager.srcList.size();
	reportParameter.nbTimeStep=*configManager.FastGetConfigValue(Core_Configuration::IPROP_QUANT_TIMESTEP); //(int)((*configManager.FastGetConfigValue(Core_Configuration::FPROP_SIMULATION_TIME))/(*configManager.FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP)));
	reportParameter.timeStep=*configManager.FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP);
	reportParameter.tetraModel=&sceneTetraMesh;
	reportParameter.configManager=&configManager;
	reportParameter.sceneModel=&sceneMesh;
	reportParameter.working_Path=workingDir;

	//Création du dossier contenant les recepteurs surfaciques
	if(configManager.recepteur_s_List.size()>0 || configManager.recepteur_scut_List.size()>0 )
		st_mkdir(reportParameter._recepteur_surf_Path);


	//**************************************************
	// 6: Instancer les threads

	#if __USE_MULTITHREAD__
		boost::thread_group threads;
	#endif

	//**************************************************
	// 7: Executer les threads de calculs

	CalculationCore::CONF_CALCULATION confCalc;
	confCalc.nbPasTemps=reportParameter.nbTimeStep;
	confCalc.pasTemps=(*configManager.FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
	applicationToolBox.calculationTool=NULL;
	applicationToolBox.confCalc=confCalc;

	CONF_PARTICULE particuleFrame;

	progressionInfo progOutputManager(*configManager.FastGetConfigValue(Core_Configuration::IPROP_QUANT_BFREQ_TO_CALCULATE)); //gestionnaire d'affichage de progression
	applicationToolBox.mainProgressionOutput=&progOutputManager;

	std::vector<t_sppsThreadParam> threadsData(configManager.freqList.size());


	for(std::size_t idfreq=0;idfreq<configManager.freqList.size();idfreq++)
	{
		if(configManager.freqList[idfreq]->doCalculation)
		{
			threadsData.at(idfreq).freqInfos=configManager.freqList[idfreq];

			#if __USE_MULTITHREAD__
			if(*configManager.FastGetConfigValue(Core_Configuration::IPROP_DO_MULTITHREAD))
				threads.add_thread(new boost::thread(boost::bind(&runFrequenceCalculation,progOutputManager.GetMainOperation(),reportParameter,applicationToolBox,&threadsData.at(idfreq),particuleFrame)));
			else
			#endif
			runFrequenceCalculation(progOutputManager.GetMainOperation(),reportParameter,applicationToolBox,&threadsData.at(idfreq),particuleFrame);
		}
	}
	#if __USE_MULTITHREAD__
		uentier workingThreads=threads.size();
		if(*configManager.FastGetConfigValue(Core_Configuration::IPROP_DO_MULTITHREAD))
			threads.join_all();
	#endif

	cout<<"End of calculation."<<endl;

	//**************************************************
	// 8: Une fois tout les threads de calculs fermés on compile les fichiers de resultats
	reportCompilation(configManager,workingDir);

	ReportManager::SaveThreadsStats(workingDir+*configManager.FastGetConfigValue(Core_Configuration::SPROP_STATS_FILE_PATH),workingDir+*configManager.FastGetConfigValue(Core_Configuration::SPROP_CUMUL_FILE_PATH),threadsData,reportParameter);

	if (verbose_mode) { cout << "Saving Ponctual Receiver Advanced Parameters..." << endl; }
	ReportManager::SaveRecpAcousticParamsAdvance(*configManager.FastGetConfigValue(Core_Configuration::SPROP_ADV_PONCTUAL_RECEIVER_FILE_PATH),threadsData,reportParameter);
	if (verbose_mode) { cout << "End of save of Ponctual Receiver Advanced Parameters." << endl; }

	if (verbose_mode) { cout << "Saving Ponctual Receiver Intensity..." << endl; }
	ReportManager::SaveRecpIntensity("Punctual receiver intensity.gabe",threadsData,reportParameter);
	if (verbose_mode) { cout << "End of save of Ponctual Receiver intensity." << endl; }

	if (verbose_mode) { cout << "Saving sound level for each Ponctual Receiver per source..." << endl; }
	ReportManager::SaveSoundLevelBySource("Sound level per source.recps",threadsData,reportParameter);
	if (verbose_mode) { cout << "End of save sound level for each Ponctual Receiver per source." << endl; }
    st_mkdir(workingDir + *configManager.FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH));
    stringClass globalRecSurfPath=workingDir+*configManager.FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH)+"Global"+ st_path_separator();
	//Création du dossier Global
	st_mkdir(globalRecSurfPath);
    stringClass globalSurfCutPath=globalRecSurfPath+*configManager.FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH);
	globalRecSurfPath+=*configManager.FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_PATH);
	if (verbose_mode) { cout << "Saving Global Surface Receiver Data..." << endl; }
	#ifndef _PROFILE_
		#ifdef UTILISER_MAILLAGE_OPTIMISATION
			ReportManager::SauveGlobalRecepteursSurfaciques(globalRecSurfPath,configManager.recepteur_s_List,sceneTetraMesh,*configManager.FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
		#else
			ReportManager::SauveGlobalRecepteursSurfaciques(globalRecSurfPath,configManager.recepteur_s_List,sceneMesh,*configManager.FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
		#endif
		ReportManager::SauveRecepteursSurfaciquesCoupe(globalSurfCutPath,configManager.recepteur_scut_List,*configManager.FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP));
	#endif
		if (verbose_mode) { cout << "End of save Global Surface Receiver Data." << endl; }

    // Check lost particle in error and show a warning if there is too many lost particles
    long totalLost = 0l;
    long total = 0l;
    for (std::size_t idfreq = 0; idfreq<configManager.freqList.size(); idfreq++)
    {
        if (configManager.freqList[idfreq]->doCalculation)
        {
            const t_sppsThreadParam& tdata = threadsData.at(idfreq);
            totalLost += tdata.particleStats.partLost + tdata.particleStats.partLoop;
            total += tdata.particleStats.partTotal;
        }
    }
    if((double)totalLost / (double)total > PARTICLE_LOST_WARNING_RATIO) {
        fprintf(stderr, _("Warning %li particles has been in error on %li particles. The computation result may be wrong, please check the particles statitics file for more details."), totalLost, total);
    }    
	//**************************************************
	// 9: Libère l'espace mémoire
	for(std::size_t idfreq=0;idfreq<threadsData.size();idfreq++)
		threadsData.at(idfreq).clearMem();


	return 0;
}

int main(int argc, char* argv[])
{
	MainProcess(argc,argv);

    #ifdef _WIN32
	#ifdef _DEBUG
		_CrtDumpMemoryLeaks(); //Affiche les fuites mémoires
	#endif
	#endif

	return 0;
}

