#include "core_configuration.h"


Core_Configuration::Core_Configuration( CoreString xmlFilePath )
{
	CXml fichierXml(xmlFilePath);
	
	CXmlNode* root=fichierXml.GetRoot();
	if(root)
	{
		CXmlNode* simuNode=root->GetChild("simulation");
		if(simuNode)
		{
			//Ajout des informations de pas de temps ( non renseigné dans ce mode de calcul )
			simuNode->AddProperty("pasdetemps","1.00");
			simuNode->AddProperty("duree_simulation","1.00");	
			
			if(simuNode->IsPropertyExist("direct_recepteurSOutputName"))
			{
				//Code de calcul démarré par I-SIMPA
				SetConfigInformation(IPROP_CALC_TO_EXT_CORE,0);
				SetConfigInformation(SPROP_DIRECT_RECEPTEUR_S_FILE_PATH,simuNode->GetProperty("direct_recepteurSOutputName"));
				SetConfigInformation(SPROP_SABINE_RECEPTEUR_S_FILE_PATH,simuNode->GetProperty("sabine_recepteurSOutputName"));
				SetConfigInformation(SPROP_EYRING_RECEPTEUR_S_FILE_PATH,simuNode->GetProperty("eyring_recepteurSOutputName"));
				SetConfigInformation(IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ,simuNode->GetProperty("output_recs_byfreq").ToInt());
				SetConfigInformation(IPROP_DO_CALC_ABS_ATMO,simuNode->GetProperty("abs_atmo_calc").ToInt());
				SetConfigInformation(IPROP_DO_SURFACE_ANGULAR_WEIGHTING,1);
			}else{
				//Code de calcul démarré par autre code de calcul
				SetConfigInformation(IPROP_DO_CALC_ABS_ATMO,1);
				SetConfigInformation(IPROP_CALC_TO_EXT_CORE,1);
				SetConfigInformation(SPROP_OTHER_CORE_OUTPUT_PATH,simuNode->GetProperty("output_folder"));
				SetConfigInformation(IPROP_DO_SURFACE_ANGULAR_WEIGHTING,simuNode->GetProperty("do_angular_weighting").ToInt());
			}
		}
	}
	LoadCfgFile( fichierXml );
	#ifdef _DEBUG
	//SetConfigInformation(SPROP_CORE_WORKING_DIRECTORY,"tmp\\");
	#endif
	
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

