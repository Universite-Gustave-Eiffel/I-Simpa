#include "data_manager\base_core_configuration.h"
/**
 * @file core_configuration.h
 * @brief Gestion des données de configuration
 *
 * Cette classe permet d'acceder aux informations de la scène de facon optimisé et simple
 */

#ifndef __CORE_CONFIGURATION__
#define __CORE_CONFIGURATION__

/**
 * @brief Gestion des données de configuration
 *
 * Cette classe permet d'acceder aux informations de la scène de facon optimisé et simple
 */
class Core_Configuration : public Base_Core_Configuration
{
public:
	/**
	 * Enumeration des propriétés de type chaine
	 */
	enum SPROP
	{
		SPROP_DIRECT_RECEPTEUR_S_FILE_PATH,
		SPROP_SABINE_RECEPTEUR_S_FILE_PATH,
		SPROP_EYRING_RECEPTEUR_S_FILE_PATH,
		SPROP_OTHER_CORE_OUTPUT_PATH
	};
	/**
	 * Enumeration des propriétés de type float
	 */
	enum FPROP
	{
		FPROP_NONE							/*!<  */
	};
	/**
	 * Enumeration des propriétés de type entier
	 */
	enum IPROP
	{
		IPROP_DO_CALC_ABS_ATMO,						/*!< Calcul de l'absorption atmosphérique */
		IPROP_CALC_TO_EXT_CORE,						/*!< Calcul à destination d'un autre code de calcul et non pour l'utilisateur */
		IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ,	/*!< Sortie des récepteurs de surface par bande de fréquence */
		IPROP_DO_SURFACE_ANGULAR_WEIGHTING    /*!< Pondération de l'énergie selon l'angle d'incidence sur les récepteurs de surfaces*/
	};
	/**
	 * Initialisation des paramètres du coeur de calcul à partir d'un fichier XML
	 * @param xmlFilePath Chemin du fichier XML
	 */
	Core_Configuration( CoreString xmlFilePath );
	/**
	 * Destructeur
	 */
	~Core_Configuration( );
	/////////////////////////////////////////////////////////
	//	Trés rapide
	/////////////////////////////////////////////////////////
	inline decimal* FastGetConfigValue(FPROP propertyIndex){return (tabFloatProp+propertyIndex); }
	inline CoreString* FastGetConfigValue(SPROP propertyIndex){return (tabStringProp+propertyIndex); }
	inline entier* FastGetConfigValue(IPROP propertyIndex){return (tabIntProp+propertyIndex); }

	inline decimal* FastGetConfigValue(BASE_FPROP propertyIndex){return (tabFloatProp+propertyIndex); }
	inline CoreString* FastGetConfigValue(BASE_SPROP propertyIndex){return (tabStringProp+propertyIndex); }
	inline entier* FastGetConfigValue(BASE_IPROP propertyIndex){return (tabIntProp+propertyIndex); }

	bool CalculationForOutsideCore(){ return *FastGetConfigValue(IPROP_CALC_TO_EXT_CORE); }
private:

	void SetConfigInformation(FPROP propertyIndex,decimal valeur);
	void SetConfigInformation(SPROP propertyIndex,CoreString valeur);
	void SetConfigInformation(IPROP propertyIndex,entier valeur);
	void SetConfigInformation(BASE_FPROP propertyIndex,decimal valeur);
	void SetConfigInformation(BASE_SPROP propertyIndex,CoreString valeur);
	void SetConfigInformation(BASE_IPROP propertyIndex,entier valeur);


};

#endif
