
#ifdef _DEBUG
#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif
#endif
#include <coreInitialisation.h>
#include "TC_CalculationCore.h"
/**
 * @brief Structure contenant tout les outils voués au calcul de propagation
 */
struct t_ToolBox
{
	ReportManager* outputTool;
	Core_Configuration* configurationTool;
	t_Mesh* sceneMesh;
	t_TetraMesh* tetraMesh;
	progressionInfo* progManager;
};


int MainProcess(int argc, char* argv[])
{

	using namespace std;
	//**************************************************
	//Initialisation
	t_ToolBox applicationToolBox;
	progressionInfo progOutputManager(3); //gestionnaire d'affichage de progression
	applicationToolBox.progManager=&progOutputManager;
	formatCoreBIN::ioModel modelEntree;
	t_Mesh sceneMesh;
	t_TetraMesh sceneTetraMesh;
	applicationToolBox.sceneMesh=&sceneMesh;
	applicationToolBox.tetraMesh=&sceneTetraMesh;

	//**************************************************
	//Verification des arguments
	string pathFichier;
	if(argc>1)
	{
		pathFichier.append(argv[1]);
		for(int idarg=2;idarg<argc;idarg++)
		{
			pathFichier.append(" ");
			pathFichier.append(argv[idarg]);
		}
	}else{
		cout<<_("The path of the XML configuration file must be specified!")<<endl;
		return 1;
	}


	//**************************************************
	// 1: Lire le fichier XML
	cout<<_("XML configuration file is currently loading...")<<endl;
	cout<<pathFichier<<endl;
	Core_Configuration configManager(pathFichier);
	applicationToolBox.configurationTool=&configManager;
	cout<<"XML configuration file has been loaded."<<endl;


	//**************************************************
	// 2: Initialisation des variables
	CoreString workingDir=*configManager.FastGetConfigValue(Core_Configuration::SPROP_CORE_WORKING_DIRECTORY);
	CoreString sceneMeshPath=*configManager.FastGetConfigValue(Core_Configuration::SPROP_MODEL_FILE_PATH);
	sceneMeshPath=workingDir+sceneMeshPath;


	//**************************************************
	// 3: Chargement du modèle
	if(!initMesh(sceneMesh,workingDir,sceneMeshPath,configManager))
		return 1;

	//**************************************************
	// 4: Chargement du maillage
	if(!initTetraMesh(workingDir+*configManager.FastGetConfigValue(Core_Configuration::SPROP_TETRAHEDRALIZATION_FILE_PATH),sceneMesh,configManager.freqList.size(),sceneTetraMesh,configManager))
		return 1;

	//**************************************************
	// 5: Création des données paramètre du coeur de calcul

	ReportManager::t_ParamReport reportParameter;
	reportParameter._recepteur_surf_Path=workingDir;
	reportParameter.nbTimeStep=1;
	reportParameter.freqIndex=0;
	reportParameter.tetraModel=&sceneTetraMesh;
	reportParameter.sceneModel=&sceneMesh;
	//Reserve l'espace mémoire pour les récepteurs surfacique pour toutes les bandes de fréquence
	for(std::size_t idFreq=0;idFreq<configManager.freqList.size();idFreq++)
		InitRecepteurSBfreq(configManager.recepteur_s_List,idFreq,*configManager.FastGetConfigValue(Core_Configuration::IPROP_QUANT_TIMESTEP));
	//**************************************************
	// 6: Executer les calculs

	CalculationCore::CONF_CALCULATION confCalc;
	confCalc.reportParameter=reportParameter;
	CalculationCore calcCore(sceneMesh,sceneTetraMesh,confCalc,configManager);

	calcCore.Run();
	//**************************************************
	// 7: Une fois tout les threads de calculs fermés on compile les fichiers de resultats


	if(!configManager.CalculationForOutsideCore())
	{
		std::vector<CoreString> reportFreqLbl;
		for(std::size_t idfreq=0;idfreq<configManager.freqList.size();idfreq++)
			reportFreqLbl.push_back(CoreString::FromInt(configManager.freqList[idfreq]->freqValue)+" Hz");

		char ret[2]={13,0};
		/////////////
		// Colonnes des valeurs globales
		std::vector<CoreString> globalColsLbl;
		for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
		{
			CoreString lblTh;
			if(idTheorie==MODE_CALCUL_SABINE)
				lblTh="Sabine";
			else
				lblTh="Eyring";
			globalColsLbl.push_back("A_"+lblTh+ret+"m²");
			globalColsLbl.push_back("TR_"+lblTh+ret+"s");
			globalColsLbl.push_back("L_"+lblTh+ret+"dB");

		}
		/////////////
		// Colonnes des récepteurs ponctuels
		std::vector<CoreString> colsLbl;
		colsLbl.push_back("Direct"+CoreString(ret)+"dB SPL");
		colsLbl.push_back("Total (Sabine)"+CoreString(ret)+"dB SPL");
		colsLbl.push_back("Total (Eyring)"+CoreString(ret)+"dB SPL");


		CoreString fileGlobalName=*configManager.FastGetConfigValue(Core_Configuration::SPROP_CORE_WORKING_DIRECTORY);
		fileGlobalName+=_("main_calculation");
		CoreString rpsFolder=*configManager.FastGetConfigValue(Core_Configuration::SPROP_CORE_WORKING_DIRECTORY);
		if(calcCore.recepteurPList.size()>0)
		{
			rpsFolder+=_("Punctual receivers\\");
			st_mkdir(rpsFolder.c_str());
			ReportManager::SauveTCRecepteursPonctuels(rpsFolder,reportFreqLbl,colsLbl,&calcCore.recepteurPList);
		}
		ReportManager::SauveTCGlobalsValues(fileGlobalName,reportFreqLbl,globalColsLbl,calcCore.mainData);
	}else{
		//Sauvegarde des données pour les autres code de calculs
		CoreString exportcoredir=*configManager.FastGetConfigValue(Core_Configuration::SPROP_CORE_WORKING_DIRECTORY);
		exportcoredir+=*configManager.FastGetConfigValue(Core_Configuration::SPROP_OTHER_CORE_OUTPUT_PATH)+"\\";
		st_mkdir(exportcoredir.c_str());
		//Export du fichier des récepteurs ponctuels
		ReportManager::SauveFusionTCRecepteursPonctuels(exportcoredir,calcCore.recepteurPList);
		//Export des fichiers des récepteurs surfaciques
		ReportManager::SauveFusionRecepteursSurfaciques(exportcoredir,configManager.recepteur_s_List);
	}
	return 0;
}

int main(int argc, char* argv[])
{

	int res=MainProcess(argc,argv);


	#ifdef _DEBUG
	#ifdef _WIN32
		_CrtDumpMemoryLeaks();
    #endif
	#endif

	return res;
}
