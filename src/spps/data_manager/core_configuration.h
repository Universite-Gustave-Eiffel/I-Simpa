#include "data_manager/base_core_configuration.h"

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
		SPROP_PARTICULE_FILE_PATH,					/*!< Nom du fichier de sortiedes particules (par bande) */
		SPROP_PARTICULE_FOLDER_PATH,					/*!< Nom de dossier de sortie des particules (toute bande) */
		SPROP_STATS_FILE_PATH						/*!< Nom du fichier de statistiques des particules */
	};
	/**
	 * Enumeration des propriétés de type float
	 */
	enum FPROP
	{
		FPROP_NORM_VEC_PART,							/*!< Norme vecteur déplacement (m/pas de temps) */
		FPROP_RAYON_RECEPTEURP,
		FPROP_EPSILON_TRANSMISSION						/*!< Fraction d'énérgie pour laquelle il n'y a plus de division d'énérgie */
	};
	/**
	 * Enumeration des propriétés de type entier
	 */
	enum IPROP
	{
		IPROP_QUANT_PARTICLE_CALCULATION,			/*!< Nombre total de particule dans la simulation par source*/
		IPROP_QUANT_PARTICLE_OUTPUT,				/*!< Nombre total de particule à enregistrer par source */
		IPROP_DO_CALC_ABS_ATMO,						/*!< Calcul de l'absorption atmosphérique */
		IPROP_DO_CALC_CHAMP_DIRECT,					/*!< 100% Absorption des surfaces */
		IPROP_DO_CALC_ENCOMBREMENT,					/*!< Prendre en compte les encombrements dans le calcul */
		IPROP_DO_MULTITHREAD,						/*!< A 0 calcul de une seul bande de fréquence à la fois */
		IPROP_ENERGY_CALCULATION_METHOD,			/*!< (faux)Calcul des absorptions de manière aléatoire (vrai) méthode énérgetique*/
		IPROP_OUTPUT_RECEPTEURS_SURF_BY_FREQ,		/*!< Sortie des récepteurs de surface par bande de fréquence */
		IPROP_DO_CALC_TRANSMISSION,
		I_PROP_SURFACE_RECEIVER_MODE,				/*!< 0: Mode intensité avec pondération selon l'angle 2: Mode SPL */
		I_PROP_OUTPUT_RECEIVER_BY_SOURCE,			/*!< 0 Output soundprec.recp, 1 output soudprec.recp by source */
		I_PROP_RANDOM_SEED,							/*!< Random SEED */
        I_PROP_SAVE_SURFACE_INTERSECTION,           /*!< Save particle and surface intersection events*/
        I_PROP_SAVE_RECEIVER_INTERSECTION           /*!< Save particle and receiver intersection events*/
	};

	/**
	 * Initialisation des paramètres du coeur de calcul à partir d'un fichier XML
	 * @param xmlFilePath Chemin du fichier XML
	 */
	Core_Configuration( CoreString xmlFilePath, bool verbose_mode = false);
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

	/**
	 * Retourne l'adresse du matériau correspondant à l'indice Xml du matériau
	 * @param outsideIndex Indice Xml
	 * @return Pointeur vers un matériau si existant. NULL si inéxistant.
	 */
	t_Material* GetMaterialByOutsideIndex( uentier outsideIndex );

	decimal GetNormVecPart(  const vec3& position, t_Tetra* currentTetra );
private:
	
	void SetConfigInformation(FPROP propertyIndex,decimal valeur);
	void SetConfigInformation(SPROP propertyIndex,CoreString valeur);
	void SetConfigInformation(IPROP propertyIndex,entier valeur);
	void SetConfigInformation(BASE_FPROP propertyIndex,decimal valeur);
	void SetConfigInformation(BASE_SPROP propertyIndex,CoreString valeur);
	void SetConfigInformation(BASE_IPROP propertyIndex,entier valeur);
	

};

#endif