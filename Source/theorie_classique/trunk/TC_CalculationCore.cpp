#include "TC_CalculationCore.h"
#include <tools/collision.h>
#include <iostream>
#include <tools/octree44_triangleElement.hpp>
#include <tools/octree44_segmentElement.hpp>

#define _REF_P_0 20e-6
#define _REF_I_0 1e-12

bool isTransparent(const t_cFace& faceInfo)
{
	if(faceInfo.faceEncombrement==NULL || faceInfo.faceMaterial->matSpectrumProperty[0].tau==1)
		return false;
	else
		return true;
}

CalculationCore::CalculationCore(t_Mesh& _sceneMesh,t_TetraMesh& _sceneTetraMesh,CONF_CALCULATION &_confEnv, Core_Configuration &_configurationTool)
: mainData(_configurationTool.freqList.size()),
  recepteurPList(_configurationTool.recepteur_p_List.size()),
  progressOutput(3),
  modelOctree(NULL)
{
	using namespace octreeTool;
	sceneMesh=&_sceneMesh;
	confEnv=_confEnv;
	configurationTool=&_configurationTool;
	sceneTetraMesh=&_sceneTetraMesh;
	lstFaces.reserve(sceneMesh->pface_size);
	for(std::size_t idface=0;idface<sceneMesh->pface_size;idface++)
	{
		lstFaces.push_back(new triangleElement(sceneMesh->pvertices[sceneMesh->pfaces[idface].sommets.a],sceneMesh->pvertices[sceneMesh->pfaces[idface].sommets.b],sceneMesh->pvertices[sceneMesh->pfaces[idface].sommets.c]));
	}
	modelOctree = new octreeTool::Octree44(lstFaces,4);
}
CalculationCore::~CalculationCore()
{
	using namespace octreeTool;
	for(elementSize idface=0;idface<lstFaces.size();idface++)
	{
		delete lstFaces[idface];
	}
	delete modelOctree;
}
bool CalculationCore::Run()
{
	///////////////////////////////////////////
	// Calcul du volume de la scène
	mainData.sceneVolume=Get_Volume_Scene();
	///////////////////////////////////////////
	// Calcul globaux pour chaque théories de calcul
	std::cout<<"Step 1/3 : Main calculation."<<std::endl;
	if(NB_THEORIES>0)
		progressOperation stepOneOp(progressOutput.GetMainOperation());

	for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
	{
		RunStep1((MODE_CALCUL)idTheorie);
	}
	progressOutput.OutputCurrentProgression();
	///////////////////////////////////////////
	// Calcul pour chaque récepteurs ponctuels
	if(configurationTool->recepteur_p_List.size()>0)
	{
		std::cout<<"Step 2/3 : Punctual receivers calculation."<<std::endl;
		RunStep2(progressOutput.GetMainOperation());
	}else{
		std::cout<<"Step 2/3 : No Punctual receivers."<<std::endl;
	}

	///////////////////////////////////////////
	// Calcul pour chaque recepteurs surfaciques et sauvegarde de ceux-ci
	if(configurationTool->recepteur_s_List.size()>0 || configurationTool->recepteur_scut_List.size()>0)
	{
		std::cout<<"Step 3/3 : Surface receivers calculation."<<std::endl;
		RunStep3(progressOutput.GetMainOperation());
	}else{
		std::cout<<"Step 3/3 : No Surface receivers."<<std::endl;
	}

	return true;
}


decimal CalculationCore::Get_A_Sabine ( entier idFreq )
{
	decimal A_Sabine=0.f;
	//Pour chaque surface composant la scène
	for(uentier idface=0;idface<this->sceneMesh->pface_size;idface++)
	{
		//Si la surface n'est pas une surface d'un encombrement
		if(!isTransparent(this->sceneMesh->pfaces[idface]))
		{
			ivec3 sommets=this->sceneMesh->pfaces[idface].sommets;
			decimal aireSurface=GetAireTriangle(this->sceneMesh->pvertices[sommets.a],this->sceneMesh->pvertices[sommets.b],this->sceneMesh->pvertices[sommets.c]);
			A_Sabine+=aireSurface*this->sceneMesh->pfaces[idface].faceMaterial->matSpectrumProperty[idFreq].absorption;
		}
	}
	return A_Sabine;
}

decimal CalculationCore::Get_A_Eyring ( entier idFreq )
{
	decimal A_Eyring=0.f;
	decimal A_Moyen=0.f;
	//Pour chaque surface composant la scène

	double aireTot=0.;
	for(uentier idface=0;idface<this->sceneMesh->pface_size;idface++)
	{
		//Si la surface n'est pas une surface d'un encombrement
		if(!isTransparent(this->sceneMesh->pfaces[idface]))
		{
			ivec3 sommets=this->sceneMesh->pfaces[idface].sommets;
			aireTot+=GetAireTriangle(this->sceneMesh->pvertices[sommets.a],this->sceneMesh->pvertices[sommets.b],this->sceneMesh->pvertices[sommets.c]);
		}
	}

	for(uentier idface=0;idface<this->sceneMesh->pface_size;idface++)
	{
		//Si la surface n'est pas une surface d'un encombrement
		if(!isTransparent(this->sceneMesh->pfaces[idface]))
		{
			ivec3 sommets=this->sceneMesh->pfaces[idface].sommets;
			decimal aireSurface=GetAireTriangle(this->sceneMesh->pvertices[sommets.a],this->sceneMesh->pvertices[sommets.b],this->sceneMesh->pvertices[sommets.c]);

				A_Moyen+=(aireSurface*this->sceneMesh->pfaces[idface].faceMaterial->matSpectrumProperty[idFreq].absorption)/aireTot;
		}
	}
	return -aireTot*log(1-A_Moyen);
}

decimal CalculationCore::Get_TR_( decimal A, entier idFreq )
{
	if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_ABS_ATMO))
		return 0.163 * this->mainData.sceneVolume / (4*this->configurationTool->freqList[idFreq]->absorption_atmospherique*this->mainData.sceneVolume + A);
	else
		return 0.163 * this->mainData.sceneVolume / (A);
}
decimal CalculationCore::Get_L_lin( decimal A, entier idFreq )
{
	decimal somme_energie=0.f;
	for(std::size_t idsource=0;idsource<this->configurationTool->srcList.size();idsource++)
	{

		somme_energie+=this->configurationTool->srcList[idsource]->bandeFreqSource[idFreq].w_j;
	}
	if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_ABS_ATMO))
		return 4*somme_energie/( (4*this->configurationTool->freqList[idFreq]->absorption_atmospherique*this->mainData.sceneVolume + A) );
	else
		return 4*somme_energie/( (A) );
}

decimal CalculationCore::Get_L( decimal L_lin )
{
	return 10*log10f( *this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_RHO) * (*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_CELERITE)) * L_lin / pow(_REF_P_0,2));
}
decimal CalculationCore::Get_Champ_Direct_Lin( entier idFreq, vec3 positionRecepteur , bool angleAttenuation,vec3 dotNormal)
{
	decimal Champ_Direct_Lin=0.F;
	float receiveEnergie=0.f;
	for(std::size_t idsource=0;idsource<this->configurationTool->srcList.size();idsource++)
	{
		if(!IsCollisionWithScene(positionRecepteur,this->configurationTool->srcList[idsource]->Position))
		{
			vec3 vecSrcToReceiver=positionRecepteur-this->configurationTool->srcList[idsource]->Position;
			receiveEnergie=this->configurationTool->srcList[idsource]->bandeFreqSource[idFreq].w_j/ ( 4 * M_PI * pow((vecSrcToReceiver).length(),2));
			if(angleAttenuation)
			{
				if(vecSrcToReceiver.dot(dotNormal)<0) //si la face du recepteur est orienté dans l'autre direction on inverse la normal
					dotNormal*=-1;
				receiveEnergie*=cosf(dotNormal.angle(vecSrcToReceiver));
			}
			Champ_Direct_Lin+=receiveEnergie;
		}
	}
	return Champ_Direct_Lin;
}
decimal CalculationCore::Get_Recepteur_P_Direct( decimal  Champ_Direct_Lin )
{
	return 10*log10f( *this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_RHO) * (*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_CELERITE)) * Champ_Direct_Lin / pow(_REF_P_0,2));
}
decimal CalculationCore::Get_Recepteur_P_Champ_Total ( decimal  Champ_Direct_Lin, decimal L_lin )
{
	return 10*log10f( *this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_RHO) * (*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_CELERITE)) * (Champ_Direct_Lin + L_lin) / pow(_REF_P_0,2));
}
decimal CalculationCore::Get_Recepteur_S_Direct( decimal  Champ_Direct_Lin )
{
	return Champ_Direct_Lin; //10*log10f( Champ_Direct_Lin / _REF_I_0 );
}

decimal CalculationCore::Get_Recepteur_S_Champ_Total ( decimal  Champ_Direct_Lin, decimal L_lin )
{
	return Champ_Direct_Lin + L_lin; //10*log10f( (Champ_Direct_Lin + L_lin ) / _REF_I_0  );
}

decimal CalculationCore::Get_Volume_Scene( )
{
	decimal vol_scene=0.f;
	//Pour chaque tetrah_dre
	for(uentier idtetra=0;idtetra<this->sceneTetraMesh->tetraedresSize;idtetra++)
	{
		ivec4 sommetsTetra=this->sceneTetraMesh->tetraedres[idtetra].sommets;
		vol_scene+=CalcTetraVolume( this->sceneTetraMesh->nodes[sommetsTetra.a],this->sceneTetraMesh->nodes[sommetsTetra.b],this->sceneTetraMesh->nodes[sommetsTetra.c],this->sceneTetraMesh->nodes[sommetsTetra.d]);
	}
	return vol_scene;
}

/*
 * Calcul des valeurs globales par bandes
 */
void CalculationCore::RunStep1( MODE_CALCUL modeCalcul )
{

	///////////////////////////////////////////
	// Calcul pour chaque bande de fréquence
	for(std::size_t idFreq=0;idFreq<this->configurationTool->freqList.size();idFreq++)
	{
		///////////////////////////////////////////
		// Calcul de l'aire d'absorption équivalente (m²)
		float AireAbsorptionEquivalente;
		if(modeCalcul==MODE_CALCUL_SABINE)
			AireAbsorptionEquivalente=Get_A_Sabine(idFreq);
		else
			AireAbsorptionEquivalente=Get_A_Eyring(idFreq);
		mainData.frequencyDependentValues[idFreq].modeCalcul[modeCalcul].AireAbsorptionEquivalente=AireAbsorptionEquivalente;

		///////////////////////////////////////////
		// Calcul du temps de réverberation (s)
		mainData.frequencyDependentValues[idFreq].modeCalcul[modeCalcul].TR=Get_TR_(AireAbsorptionEquivalente,idFreq);

		///////////////////////////////////////////
		// Calcul du niveau sonore du champ reverbéré linéaire
		mainData.frequencyDependentValues[idFreq].modeCalcul[modeCalcul].NiveauSonoreChampReverbereLineaire=Get_L_lin(AireAbsorptionEquivalente,idFreq);

		///////////////////////////////////////////
		// Calcul du niveau sonore du champ reverbéré
		mainData.frequencyDependentValues[idFreq].modeCalcul[modeCalcul].NiveauSonoreChampReverbere=Get_L(mainData.frequencyDependentValues[idFreq].modeCalcul[modeCalcul].NiveauSonoreChampReverbereLineaire);
	}
}
/*
 * Calcul des valeurs par récepteurs ponctuels
 */
void CalculationCore::RunStep2( progressOperation* parentProgressOp )
{
	///////////////////////////////////////////
	// Initialisation des données des récepteurs ponctuels
	progressOperation Step2op(parentProgressOp,recepteurPList.size());
	for(std::size_t idRp=0;idRp<recepteurPList.size();idRp++)
	{
		t_TC_RecepteurPonctuel* currentRecepteurP=&recepteurPList[idRp];

		progressOperation RecPop(&Step2op,currentRecepteurP->nbFreq);
		currentRecepteurP->init(this->configurationTool->freqList.size());
		currentRecepteurP->linkedRecepteurP=this->configurationTool->recepteur_p_List[idRp];
		///////////////////////////////////////////
		// Calcul pour chaque bande de fréquence

		for(int idFreq=0;idFreq<currentRecepteurP->nbFreq;idFreq++)
		{
			progressOperation freqOp(&RecPop);

			decimal ChampDirectLineaire=Get_Champ_Direct_Lin(idFreq,currentRecepteurP->linkedRecepteurP->position);
			currentRecepteurP->tabDataByFreq[idFreq].ChampDirectLineaire=*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_RHO) * (*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_CELERITE)) * ChampDirectLineaire;
			currentRecepteurP->tabDataByFreq[idFreq].ChampDirect=Get_Recepteur_P_Direct(ChampDirectLineaire);

			///////////////////////////////////////////
			// Calcul pour chaque théorie
			for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
			{
				///////////////////////////////////////////
				// Calcul du Champ Total
				currentRecepteurP->tabDataByFreq[idFreq].tabDataByTheorie[(MODE_CALCUL)idTheorie].ChampTotal=Get_Recepteur_P_Champ_Total(ChampDirectLineaire,mainData.frequencyDependentValues[idFreq].modeCalcul[(MODE_CALCUL)idTheorie].NiveauSonoreChampReverbereLineaire);
			}
			progressOutput.OutputCurrentProgression();
		}
	}
}
/*
 * Calcul des valeurs par récepteurs de surface
 */
void CalculationCore::RunStep3( progressOperation* parentProgressOp )
{


	///////////////////////////////
	// Création des dossiers pour chaque méthode de calcul
	CoreString rootFolder=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_CORE_WORKING_DIRECTORY);
	/////////////////////////////////////////////////////
	// Pour chaque mode de calcul, création des dossiers
	for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
	{
		CoreString modeCalculFolder=rootFolder;
		if((MODE_CALCUL)idTheorie==MODE_CALCUL_SABINE)
			modeCalculFolder+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_SABINE_RECEPTEUR_S_FILE_PATH);
		else
			modeCalculFolder+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_EYRING_RECEPTEUR_S_FILE_PATH);
		if(!configurationTool->CalculationForOutsideCore())
			st_mkdir(modeCalculFolder.c_str());
		modeCalculFolder+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH);
		if(!configurationTool->CalculationForOutsideCore())
			st_mkdir(modeCalculFolder.c_str());
	}
	/////////////////////////////////////////////////////
	// Création des dossiers pour le mode de calcul direct
	CoreString modeCalculFolderDirect=rootFolder;
	modeCalculFolderDirect+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_DIRECT_RECEPTEUR_S_FILE_PATH);
	if(!configurationTool->CalculationForOutsideCore())
			st_mkdir(modeCalculFolderDirect.c_str());
	modeCalculFolderDirect+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH);
	if(!configurationTool->CalculationForOutsideCore())
			st_mkdir(modeCalculFolderDirect.c_str());



	CoreString directFolder=rootFolder;
	CoreString SabineFolder=rootFolder;
	CoreString EyringFolder=rootFolder;
	directFolder+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_DIRECT_RECEPTEUR_S_FILE_PATH)+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH);
	SabineFolder+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_SABINE_RECEPTEUR_S_FILE_PATH)+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH);
	EyringFolder+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_EYRING_RECEPTEUR_S_FILE_PATH)+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH);

	progressOperation thisOperation(parentProgressOp,this->configurationTool->freqList.size());

	///////////////////////////////////////////////////////////////////////////
	// Création de NB_THEORIES copies des récepteurs de surface
	std::vector<r_Surf*> recepteurSLin[NB_THEORIES];
	std::vector<r_SurfCut*> recepteurSCutLin[NB_THEORIES];

	if(!configurationTool->CalculationForOutsideCore())
	{
		for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
		{
			ReportManager::CopyRecepteurSurf(this->configurationTool->recepteur_s_List,recepteurSLin[idTheorie]);
			ReportManager::CopyRecepteurCut(this->configurationTool->recepteur_scut_List,recepteurSCutLin[idTheorie]);
		}
	}
	///////////////////////////////////////////
	// Calcul pour chaque bande de fréquence
	for(std::size_t idFreq=0;idFreq<this->configurationTool->freqList.size();idFreq++)
	{
		if(this->configurationTool->freqList[idFreq]->doCalculation)
		{
			progressOperation thisFreq(&thisOperation,NB_THEORIES);

			this->confEnv.reportParameter.freqIndex=idFreq;
			this->confEnv.reportParameter.freqValue=this->configurationTool->freqList[idFreq]->freqValue;
			CoreString pathFreq;
			pathFreq=stringClass::FromInt(this->confEnv.reportParameter.freqValue)+stringClass(" Hz\\");
			////////////////////////
			// Création des dossier pour la fréquence en cours
			if(!configurationTool->CalculationForOutsideCore() && this->configurationTool->FastGetConfigValue(Core_Configuration::IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ))
			{
				st_mkdir((directFolder+pathFreq).c_str());
				st_mkdir((SabineFolder+pathFreq).c_str());
				st_mkdir((EyringFolder+pathFreq).c_str());
			}

			///////////////////////////////////////////
			// paramètre gestionnaire de sortie de données
			ReportManager::t_ParamReport thCfgDir=this->confEnv.reportParameter;
			thCfgDir._recepteur_surf_Path=directFolder+pathFreq;
			//thCfgDir._recepteur_surf_Path+=*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_DIRECT_RECEPTEUR_S_FILE_PATH);

			SetEnergyDirect( idFreq, configurationTool->recepteur_s_List );
			SetEnergyDirectCut(idFreq, configurationTool->recepteur_scut_List );

			///////////////////////////////////////////
			// Calcul pour chaque théorie
			ReportManager::t_ParamReport thCfg=this->confEnv.reportParameter;
			if(!configurationTool->CalculationForOutsideCore())
			{
				for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
				{
					progressOperation thisTheorie(&thisFreq);
					if((MODE_CALCUL)idTheorie==MODE_CALCUL_SABINE)
						thCfg._recepteur_surf_Path=SabineFolder+pathFreq;
					else
						thCfg._recepteur_surf_Path=EyringFolder+pathFreq;

					SetEnergyLin( (MODE_CALCUL)idTheorie,idFreq,configurationTool->recepteur_s_List, recepteurSLin[idTheorie] );
					SetEnergyLinCut( (MODE_CALCUL)idTheorie,idFreq,configurationTool->recepteur_scut_List, recepteurSCutLin[idTheorie] );
					///////////////////////////////////////////
					// Enregistrement du fichier

					if(this->configurationTool->FastGetConfigValue(Core_Configuration::IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ))
					{
						ReportManager::SauveRecepteursSurfaciques(thCfg._recepteur_surf_Path+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_PATH),idFreq,recepteurSLin[idTheorie],*sceneTetraMesh,*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false);
						ReportManager::SauveRecepteursSurfaciquesCoupe(thCfg._recepteur_surf_Path+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH),recepteurSCutLin[idTheorie],*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false,false,idFreq);
					}
					progressOutput.OutputCurrentProgression();
				}
			}
			///////////////////////////////////////////
			// Enregistrement du fichier direct
			if(this->configurationTool->FastGetConfigValue(Core_Configuration::IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ))
			{
				ReportManager::SauveRecepteursSurfaciques(thCfgDir._recepteur_surf_Path+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_PATH),idFreq,configurationTool->recepteur_s_List,*sceneTetraMesh,*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false);
				ReportManager::SauveRecepteursSurfaciquesCoupe(thCfgDir._recepteur_surf_Path+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH),configurationTool->recepteur_scut_List,*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false,false,idFreq);
			}
		}
	}
	//Enregistrement toute bandes
	if(!configurationTool->CalculationForOutsideCore())
	{
		for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
		{
			stringClass filePath;
			if((MODE_CALCUL)idTheorie==MODE_CALCUL_SABINE)
				filePath=SabineFolder+"Global\\";
			else
				filePath=EyringFolder+"Global\\";
			st_mkdir(filePath.c_str());
			ReportManager::SauveGlobalRecepteursSurfaciques(filePath+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_PATH),recepteurSLin[idTheorie],*sceneTetraMesh,*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false);
			ReportManager::SauveRecepteursSurfaciquesCoupe(filePath+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH),configurationTool->recepteur_scut_List,*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false);
		}
		st_mkdir((directFolder+"Global\\").c_str());
		ReportManager::SauveGlobalRecepteursSurfaciques(directFolder+"Global\\"+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_PATH),configurationTool->recepteur_s_List,*sceneTetraMesh,*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false);
		ReportManager::SauveRecepteursSurfaciquesCoupe(directFolder+"Global\\"+*this->configurationTool->FastGetConfigValue(Core_Configuration::SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH),configurationTool->recepteur_scut_List,*this->configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP),false);
	}
	///////////////////////////////////////////
	// Libération de l'espace mémoire
	for(int idTheorie=0;idTheorie<NB_THEORIES;idTheorie++)
	{
		for(std::size_t idRs=0;idRs<recepteurSLin[idTheorie].size();idRs++)
		{
			delete recepteurSLin[idTheorie][idRs];
		}
		for(std::size_t idRsCut=0;idRsCut<recepteurSCutLin[idTheorie].size();idRsCut++)
		{
			delete recepteurSCutLin[idTheorie][idRsCut];
		}
	}
}
void CalculationCore::SetEnergyDirectCut( entier idFreq, std::vector<r_SurfCut*>& recepteurS )
{
	for(std::vector<r_SurfCut*>::iterator Rs=recepteurS.begin();Rs!=recepteurS.end();Rs++)
	{
		vec3 vCellVec=((*Rs)->Avert-(*Rs)->Bvert)/(float)(*Rs)->NbCellV; //BA
		vec3 uCellVec=((*Rs)->Cvert-(*Rs)->Bvert)/(float)(*Rs)->NbCellU; //BC
		vec3 Goffset=vCellVec/2.f+uCellVec/2.f;
		for(unsigned int idrow=0;idrow<(*Rs)->nbrows;idrow++) //U
		{
			for(unsigned int idcol=0;idcol<(*Rs)->nbcols;idcol++) //V
			{
				vec3 Gcell=(*Rs)->Bvert+(vCellVec*(float)idcol)+(uCellVec*(float)idrow)+Goffset;
				(*Rs)->data[idFreq][idrow][idcol][0]=Get_Champ_Direct_Lin(idFreq,Gcell,*this->configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_SURFACE_ANGULAR_WEIGHTING),(*Rs)->planeNormal);
			}
		}
	}
}
void CalculationCore::SetEnergyDirect( entier idFreq, std::vector<r_Surf*>& recepteurS )
{
	for(std::size_t idRs=0;idRs<recepteurS.size();idRs++)
	{
		for(std::size_t idFace=0;idFace<recepteurS[idRs]->nbFaces;idFace++)
		{
			r_Surf_Face* currentFace=&recepteurS[idRs]->faces[idFace];
			//Calcul du point G centre de gravité des sommets du triangle du récepteur surfacique
			vec3 G=GetGTriangle(currentFace->sommets[0],currentFace->sommets[1],currentFace->sommets[2]);
			//Calcul de la normale de la face
			vec3 v_faceNormal=FaceNormal(currentFace->sommets[0],currentFace->sommets[1],currentFace->sommets[2]);
			//Translation du Point G d'une distance d'EPSILON le long de la normal de la face afin de ne pas être occulté par la face elle même lors du test de collision.
			G+=v_faceNormal*BARELY_EPSILON;
			currentFace->energieRecu[idFreq][0]=Get_Champ_Direct_Lin(idFreq,G,*this->configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_SURFACE_ANGULAR_WEIGHTING),v_faceNormal);
		}
	}
}

void CalculationCore::SetEnergyLinCut( MODE_CALCUL modeCalcul, entier idFreq , std::vector<r_SurfCut*>& recepteurSDirect, std::vector<r_SurfCut*>& recepteurSLin )
{
	unsigned int idrs=0;
	for(std::vector<r_SurfCut*>::iterator Rs=recepteurSLin.begin();Rs!=recepteurSLin.end();Rs++)
	{
		vec3 vCellVec=((*Rs)->Avert-(*Rs)->Bvert)/(float)(*Rs)->NbCellV; //BA
		vec3 uCellVec=((*Rs)->Cvert-(*Rs)->Bvert)/(float)(*Rs)->NbCellU; //BC
		vec3 Goffset=vCellVec/2.f+uCellVec/2.f;
		for(unsigned int idrow=0;idrow<(*Rs)->nbrows;idrow++) //U
		{
			for(unsigned int idcol=0;idcol<(*Rs)->nbcols;idcol++) //V
			{
				vec3 Gcell=(*Rs)->Bvert+(uCellVec*(float)idcol)+(vCellVec*(float)idrow)+Goffset;
				(*Rs)->data[idFreq][idrow][idcol][0]=Get_Recepteur_S_Champ_Total(recepteurSDirect[idrs]->data[idFreq][idrow][idcol][0],mainData.frequencyDependentValues[idFreq].modeCalcul[modeCalcul].NiveauSonoreChampReverbereLineaire);
			}
		}
		idrs++;
	}
}

void CalculationCore::SetEnergyLin( MODE_CALCUL modeCalcul, entier idFreq , std::vector<r_Surf*>& recepteurSDirect, std::vector<r_Surf*>& recepteurSLin )
{
	for(std::size_t idRs=0;idRs<recepteurSLin.size();idRs++)
	{
		for(std::size_t idFace=0;idFace<recepteurSLin[idRs]->nbFaces;idFace++)
		{
			r_Surf_Face* currentFaceDirect=&recepteurSDirect[idRs]->faces[idFace];
			recepteurSLin[idRs]->faces[idFace].energieRecu[idFreq][0]=Get_Recepteur_S_Champ_Total(currentFaceDirect->energieRecu[idFreq][0],mainData.frequencyDependentValues[idFreq].modeCalcul[modeCalcul].NiveauSonoreChampReverbereLineaire);
		}
	}
}



bool CalculationCore::IsCollisionWithScene(vec3 from,vec3 to)
{
	using namespace octreeTool;
	segmentElement vectest(from,to);
	std::vector<elementSize> candidates;
	//for(std::size_t idface=0;idface<sceneMesh->pface_size;idface++)
	//	candidates.push_back(idface);
	modelOctree->GetAllCollisionCandidates(vectest,candidates);
	vec3 dir=to-from;

	for(std::vector<elementSize>::iterator it=candidates.begin();it!=candidates.end();it++)
	{
		uentier cface=(*it);
		t_cFace& facetest=sceneMesh->pfaces[cface];
		if(!isTransparent(facetest)) //les encombrements sont ignorés
		{
			float t,u,v;

			vec3& vert0=sceneMesh->pvertices[facetest.sommets.a];
			vec3& vert1=sceneMesh->pvertices[facetest.sommets.b];
			vec3& vert2=sceneMesh->pvertices[facetest.sommets.c];


			if(collision_manager::intersect_triangle(from,dir,
				vert0,
				vert1,
				vert2,
				&t,&u,&v)==1)
			{
				if(t>BARELY_EPSILON && t<=1)
				{
					return true;
					break;
				}

			}
		}
	}
	return false;
}
