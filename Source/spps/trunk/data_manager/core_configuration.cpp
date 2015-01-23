#include "core_configuration.h"
#include <iostream>


Core_Configuration::Core_Configuration( CoreString xmlFilePath )
{
	CXml fichierXml(xmlFilePath);
	LoadCfgFile( fichierXml );

	#ifdef _PROFILE_
		SetConfigInformation(SPROP_CORE_WORKING_DIRECTORY,"tmp\\");
	#endif
	#ifdef _DEBUG
		//SetConfigInformation(SPROP_CORE_WORKING_DIRECTORY,"tmp\\");
	#endif
	CXmlNode* root=fichierXml.GetRoot();
	if(root)
	{

		

		CXmlNode* simuNode=root->GetChild("simulation");
		if(simuNode)
		{
			SetConfigInformation(SPROP_PARTICULE_FILE_PATH,simuNode->GetProperty("particules_filename"));
			SetConfigInformation(SPROP_PARTICULE_FOLDER_PATH,simuNode->GetProperty("particules_directory"));
			SetConfigInformation(SPROP_STATS_FILE_PATH,simuNode->GetProperty("stats_filename"));
			int nbpart=simuNode->GetProperty("nbparticules").ToInt();
			if(nbpart<1)
				nbpart=1;
			int nbpartrender=simuNode->GetProperty("nbparticules_rendu").ToInt();
			if(nbpartrender<0)
				nbpartrender=0;

			SetConfigInformation(IPROP_DO_MULTITHREAD,1);

			SetConfigInformation(IPROP_QUANT_PARTICLE_CALCULATION,nbpart);
			SetConfigInformation(IPROP_QUANT_PARTICLE_OUTPUT,nbpartrender);
			//Calcul de la norme des vecteurs vitesse des particules
			SetConfigInformation(FPROP_NORM_VEC_PART,(*FastGetConfigValue(FPROP_CELERITE))*(*FastGetConfigValue(FPROP_TIME_STEP)));
			SetConfigInformation(IPROP_DO_CALC_ABS_ATMO,simuNode->GetProperty("abs_atmo_calc").ToInt());
			int outputBySource = 0;
			if(simuNode->IsPropertyExist("output_recp_bysource")) {
				outputBySource = simuNode->GetProperty("output_recp_bysource").ToInt();
			}
			SetConfigInformation(I_PROP_OUTPUT_RECEIVER_BY_SOURCE, outputBySource);
			uentier_long seed = 0;
			if(simuNode->IsPropertyExist("random_seed")) {
				seed = simuNode->GetProperty("random_seed").ToInt();
			}
			if(seed != 0) {
				// User define a random seed, multi-thread have to be deactivated in order to do the same computation
				SetConfigInformation(IPROP_DO_MULTITHREAD,0);
				std::cout<<"Random seed has been set; then multi-thread has been desactivated."<<std::endl;
			}
			SetConfigInformation(I_PROP_RANDOM_SEED,seed);
			SetConfigInformation(IPROP_DO_CALC_CHAMP_DIRECT,simuNode->GetProperty("direct_calc").ToInt());
			SetConfigInformation(IPROP_DO_CALC_ENCOMBREMENT,simuNode->GetProperty("enc_calc").ToInt());
			SetConfigInformation(IPROP_ENERGY_CALCULATION_METHOD,simuNode->GetProperty("computation_method").ToInt());
			SetConfigInformation(FPROP_RAYON_RECEPTEURP,simuNode->GetProperty("rayon_recepteurp").ToFloat());
			SetConfigInformation(FPROP_EPSILON_TRANSMISSION,simuNode->GetProperty("trans_epsilon").ToFloat());
			SetConfigInformation(IPROP_DO_CALC_TRANSMISSION,simuNode->GetProperty("trans_calc").ToInt());
			SetConfigInformation(IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ,simuNode->GetProperty("output_recs_byfreq").ToInt());
			SetConfigInformation(I_PROP_SURFACE_RECEIVER_MODE,simuNode->GetProperty("surf_receiv_method").ToInt());
		}
	}
}

decimal Core_Configuration::GetNormVecPart( const vec3& position, t_Tetra* currentTetra )
{
	if(currentTetra->z!=-1)
	{
		float zSol=position.z-currentTetra->g.z+currentTetra->z;
		//retourne (c0+alog*logf(1+zSol/z0)+blin*zSol)*deltaT
		return (*FastGetConfigValue(Base_Core_Configuration::FPROP_CELERITE)					//c0
				+(*FastGetConfigValue(Base_Core_Configuration::FPROP_ALOG))		//+ alog
				*log(1+(zSol/(*FastGetConfigValue(Base_Core_Configuration::FPROP_Z0))))			//* log(1+(zSol/z0))
				+(*FastGetConfigValue(Base_Core_Configuration::FPROP_BLIN)*zSol)	//
				)*(*FastGetConfigValue(FPROP_TIME_STEP));									// *deltaT
	}else{
		return (*FastGetConfigValue(Base_Core_Configuration::FPROP_CELERITE))*(*FastGetConfigValue(FPROP_TIME_STEP));
	}
}

Core_Configuration::~Core_Configuration( )
{
}


void Core_Configuration::SetConfigInformation(FPROP propertyIndex,decimal valeur)
{
	Base_Core_Configuration::SetConfigInformation((BASE_FPROP)propertyIndex,valeur);
}
void Core_Configuration::SetConfigInformation(SPROP propertyIndex,CoreString valeur)
{
	Base_Core_Configuration::SetConfigInformation((BASE_SPROP)propertyIndex,valeur);
}
void Core_Configuration::SetConfigInformation(IPROP propertyIndex,entier valeur)
{
	Base_Core_Configuration::SetConfigInformation((BASE_IPROP)propertyIndex,valeur);
}


void Core_Configuration::SetConfigInformation(BASE_FPROP propertyIndex,decimal valeur)
{
	Base_Core_Configuration::SetConfigInformation(propertyIndex,valeur);
}
void Core_Configuration::SetConfigInformation(BASE_SPROP propertyIndex,CoreString valeur)
{
	Base_Core_Configuration::SetConfigInformation(propertyIndex,valeur);
}
void Core_Configuration::SetConfigInformation(BASE_IPROP propertyIndex,entier valeur)
{
	Base_Core_Configuration::SetConfigInformation(propertyIndex,valeur);
}

t_Material* Core_Configuration::GetMaterialByOutsideIndex( uentier outsideIndex )
{
	uentier sizeOfMaterials=materialList.size();
	for(uentier i=0;i<sizeOfMaterials;i++)
		if((*materialList[i]).outsideMaterialIndice==outsideIndex)
			return materialList[i];
	return NULL;
}
