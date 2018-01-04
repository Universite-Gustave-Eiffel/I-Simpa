/* ----------------------------------------------------------------------
* I-SIMPA (http://i-simpa.ifsttar.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) 2007-2014 - IFSTTAR - Judicael Picaut, Nicolas Fortin
*
* I-SIMPA is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
* 
* I-SIMPA is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA or 
* see <http://ww.gnu.org/licenses/>
*
* For more information, please consult: <http://i-simpa.ifsttar.fr> or 
* send an email to i-simpa@ifsttar.fr
*
* To contact Ifsttar, write to Ifsttar, 14-20 Boulevard Newton
* Cite Descartes, Champs sur Marne F-77447 Marne la Vallee Cedex 2 FRANCE
* or write to scientific.computing@ifsttar.fr
* ----------------------------------------------------------------------*/

#include "first_header_include.hpp"

#include "generic_element/e_gammefrequence.h"
#include "generic_element/e_materiau.h"
#include "generic_element/e_directivity.h"
#include <wx/fileconf.h>
#include "manager/smart_ptr.h"
#include <wx/hashmap.h>

#ifndef __APPCFG__
#define __APPCFG__


#define APPLICATION_NAME wxString("I-SIMPA")
#define UNDO_HISTORY_SIZE 5 //Nombre de retour en arrière possible
#define UNDO_HISTORY_TIMESTEP 2000 // Interval de temps minimum entre chaque retour en arrière (ms)

/*! \file appconfig.h
 *  \brief Classe de configuration du projet.
 *
 *	Ce fichier contient la classe ApplicationConfiguration
 */


WX_DECLARE_HASH_MAP( int, Element*, wxIntegerHash, wxIntegerEqual, t_HashElement );

/**
 *  Evenement à générer afin de raffraichir l'arbre report
 */
wxDECLARE_EVENT(wxEVT_PSPS_MAIN_EVENT, wxCommandEvent);

/*! \class ApplicationConfiguration
	\brief Classe de configuration du projet.

	Cette classe statique permet d'accéder à la configuration :
		- Du projet
		- De la session utilisateur (système)
		- De l'application (constantes)
	Elle lit le fichier de configuration xml du logiciel (le même fichier quelque soit le projet)
	Elle définit les bandes de fréquence utilisé avec leurs valeurs de pondération
	Elle définit les lois de référence
	Elle pointe vers les éléments du projet du type spectre,matériau
	Elle permet d'accéder à la racine du projet courant.

	En utilisant cette classe vous êtes en mesure d'acceder aux informations majeures du projet
*/
class ApplicationConfiguration
{
public:
	/**
	 * Evenement globaux interprété par projet, l'algorithme principal (mainloop)
	 *
	 */
	enum MAIN_EVENT
	{
		MAIN_EVENT_ENUMSTART=399,
		MAIN_EVENT_REFRESH_REPORTFOLDER,
		MAIN_EVENT_UNDO_REDO_UPDATE,
		MAIN_EVENT_POST_PYTHON_COMMAND,
		MAIN_EVENT_SWITCH_TO_CAMERA_TOOL,
		MAIN_EVENT_ENUMEND
	};
	/**
	 * Type globaux référencé par la classe ApplicationConfiguration
	 */
	enum ELEMENT_REF_TYPE
	{
		ELEMENT_REF_TYPE_DRAWABLE,
		ELEMENT_REF_TYPE_FACEGROUP,
		ELEMENT_REF_TYPE_OTHER,
		ELEMENT_REF_TYPE_SIZE /*!< Nombre de type d'élément référencé */
	};

	typedef int t_freq;				/*!< Redéfinition du type frequence (Hz) */
	typedef float t_ponderation;	/*!< Redéfinition du type ponderation (dBa) */
	 /** \brief Enumeration des vecteurs (3 flottants)
     *  Définir un nouveau vecteur ici afin de partager des informations de positions spatiales entre tout les modules faisant appel à la classe de configuration.
	 *	Ces données ne sont pas sauvegardées à la fermeture du programme.
	 *	@see GetVecParam()
	 *	@see SetVecParam()
     */
	enum VECPARAM_ELEMENT
	{
		PARAM_MODEL_MINBOX,			/*!< x,y,z minimal du domaine (cube enveloppant les surfaces) */
		PARAM_MODEL_MAXBOX			/*!< x,y,z maximal du domaine (cube enveloppant les surfaces) */
	};
	/**
	 * @brief Structure de donnant exprimant le lien entre les faces et les éléments du projet à destination du coeur de calcul
	 */
	struct t_PropFace
	{
		int idMaterial;				/*!< Indice du materiau */
		int idRecepteurSurfacique;	/*!< Indice du récepteur surfacique */
		int idEncombrement;			/*!< Indice de l'encombrement */
		t_PropFace(){idMaterial=0;idRecepteurSurfacique=-1;idEncombrement=-1;};
	};
private:
	struct tab_HashElement
	{
		t_HashElement refElementLst[ELEMENT_REF_TYPE_SIZE];
	};
	static tab_HashElement tab_refElementLst;
	static wxXmlDocument appConfig;						/*!< Document XML de l'application. Contient la base de données de référence. */
	static std::vector<E_GammeFrequence*> allSpectrum;	/*!< Tableau contenant un pointeur vers les élément de la base de données de spectre (référence et utilisateur confondu) */
	static std::vector<E_Materiau*> allMaterial;		/*!< Tableau contenant un pointeur vers les élément de la base de données de matériaux (référence et utilisateur confondu) */
	static std::vector<E_Directivity*> allDirectivity;	/*!< Tableau contenant un pointeur vers les élément de la base de données de directivités (référence et utilisateur confondu) */
	static smart_ptr<wxFileConfig> projectConfig;		/*!< wxFileConfig est une classe de wxWidgets permettant d'enregistrer des informations sur la session utilisateur (unix,windows) */
	static std::vector<t_freq> tabFreq;					/*!< Tableau contenant toutes les bandes de fréquences utilisées dans le logiciel. */
	static std::vector<t_ponderation> tabPonderation;	/*!< Tableau contenant toutes les pondérations dB->dBa utilisées dans le logiciel. */
	static std::vector<wxString> tabLoisReflexion;		/*!< Tableau contenant toutes les lois de réflexion utilisées dans le logiciel. */
	/*!
	*	\brief Enregistrement correspondant à un vecteur en mémoire.
	*	@see GetVecParam()
	*	@see SetVecParam()
	*/
	struct t_vec_param{
	  int idParam;
	  vec3 value;
	};
	static std::vector<t_vec_param> vec_param;			/*!< Tableau qui contient tous les vecteurs */
	 /** \brief Indice d'un vecteur dans le tableau pour un Element donnée.
	 *	Permet de connaître à quel indice du tableau l'on peut acceder à l'élément VECPARAM_ELEMENT.
	 *	@see GetVecParam()
	 *	@see SetVecParam()
	 *	@see vec_param
     *  @return L'identifiant correspondant du tableau vec_param.
     */
	static int GetIdVecParam(VECPARAM_ELEMENT param);
	static void AddParam(VECPARAM_ELEMENT param,vec3 Value);					/*!< Ajoute un vecteur dans le tableau */
	static bool orderSpectrums(E_GammeFrequence* lSp,E_GammeFrequence* rSp);	/*!< Trie les spectres au sein d'un tableau */
	static bool orderMateriaux(E_Materiau* lSp,E_Materiau* rSp);				/*!< Trie les matériaux au sein d'un tableau */
	static bool orderDirectivity(E_Directivity* lSp, E_Directivity* rSp);		/*!< Trie les matériaux au sein d'un tableau */
	static wxString nameNodeSpectre;											/*!< Nom du noeud xml contenant les spectre de références */
	static wxString nameNodeMateriaux;											/*!< Nom du noeud xml contenant les matériaux de références */
	static wxString nameNodeDirectivity;										/*!< Nom du noeud xml contenant les directivités de références */
	static Element* rootScene;													/*!< Pointeur vers l'élément source de la scène du projet */
	static smart_ptr<Element> rootUserConfig;									/*!< Element racine des préférences de l'utilisateur */
public:



	/** @defgroup appconfiggroup Classe ApplicationConfiguration
	 *	Cette classe statique permet d'acceder à la configuration
	 *		- Du projet
	 *		- De la session utilisateur (système)
	 *		- De l'application (constantes)
	 *  @{
	 */

	/** @defgroup varstatic Constantes de l'application
	 *  Constantes de l'application. Ce sont des informations qui ne changeront pas quelque soit le projet.
	 *  @{
	 */
	static const wxString CONST_REPORT_FOLDER_PATH;
	static const wxString CONST_REPORT_PARTICLE_FOLDER_PATH;
	static const wxString CONST_REPORT_PARTICLE_FILENAME;
	static const wxString CONST_REPORT_RECEPTEURSS_FOLDER_PATH;
	static const wxString CONST_REPORT_RECEPTEURSS_FILENAME;
	static const wxString CONST_REPORT_RECEPTEURSS_CUT_FILENAME;
	static const wxString CONST_REPORT_RECEPTEURSS_FILENAME_TR;
	static const wxString CONST_REPORT_RECEPTEURSS_FILENAME_EDT;
	static const wxString CONST_REPORT_DIRECTIVITIES_FOLDER_PATH;
	static const wxString CONST_MODEL_SCENE_FILENAME;
	static const wxString CONST_TETGEN_EXE_FILENAME;
	static const wxString CONST_TETGEN_EXE_PATH;
	static const wxString CONST_TETROOT_PATH;
	static const wxString CONST_PREPROCESS_EXE_FILENAME;
	static const wxString CONST_PREPROCESS_EXE_PATH;
	static const wxString CONST_GRAPH_CONFIG_PATH;
	static const wxString CONST_RESOURCE_BITMAP_FOLDER; /*<! Dossier racine où se trouve les données du programme*/
	static const wxString CONST_RESOURCE_ISO_FOLDER; /*<! Dossier racine où se trouve les données du programme*/
	static const wxString CONST_RESOURCE_DIRECTIVITY_FOLDER;  /*<! Dossier racine où se trouve les directivités de référence du programme*/
	//static const wxString CONST_RESOURCE_LNG_FOLDER; /*<! Dossier racine où se trouve les dossiers de langue*/
	static const wxString CONST_CORE_PATH; /*<! Dossier racine où se trouve les données du programme*/
	static const wxString CONST_STATIC_XML_FILE; /*<! fichier xml de configuration d'application (materiaux et spectres application)*/
	//static const unsigned long CONST_WORKINGLIMIT;//timestamp unix de fin de fonctionnement (0 aucune limite)
	static const int SPPS_UI_VERSION_MAJOR;
	static const int SPPS_UI_VERSION_MINOR;
	static const int SPPS_UI_VERSION_REVISION;
	static const int SPPS_VERSION_MAJOR;
	static const int SPPS_VERSION_MINOR;
	static const int SPPS_VERSION_REVISION;
	static const int CTR_VERSION_MAJOR;
	static const int CTR_VERSION_MINOR;
	static const int CTR_VERSION_REVISION;
	static const wxString BUILD_DATE;
	static const wxString CONST_USER_PREFERENCE_FILE_NAME;

	/** @} */

	/** @defgroup varglobal Variables globales de l'application
	 *  Ce sont des informations qui ne peuvent être stockés dans le fichier XML et qui sont propre au processus courant de l'application
	 *  @{
	 */
	struct t_App_Info
	{
		int quant_Faces;
		int quant_Encombrement;
		int quant_Encombrement_Actifs;
		int quant_RecepteurP;
		int quant_RecepteurS;
		int quant_Sources;
		int quant_Sources_Actives;
		int quant_SurfGroup;
		int quant_Volumes;
		float volScene;
		float surfScene;
	} static GLOBAL_CURRENT_APPLICATION_INFORMATIONS;

	struct t_GLOBAL_VAR
	{
        wxString appDataFolderPath;
		wxString cacheFolderPath;
		wxString workingFolderPath;
		bool drawMaterialColors;
	} static GLOBAL_VAR;
	/** @} */

	/**
     * Chargement du fichier XML de l'application
     * @param propFile Chemin et nom du fichier XML
     */
	static void LoadConfiguration(wxString propFile);

	/** @defgroup Constantes Données constantes du logiciel
	 *  Ce sont les informations qui ne vont pas évoluer au cours du projet ni entre les projets
	 *	Ces informations peuvent seulement évoluer lors d'une nouvelle version de SPPS (Mettre a jour les constantes de version du logiciel)
	 *  @{
	 */
	static const std::vector<t_freq>& GetAllFrequencies( );							/*!< Retourne la liste des toutes les frequences par bande de l'application */
	static const std::vector<t_ponderation>& GetAllPonderation( );					/*!< Retourne la liste des ponderations */
	static const std::vector<wxString>& GetLstLoisReff( );							/*!< Retourne la liste des lois de réflexion */
	static t_ponderation GetPonderationVal(const wxString& sfreqValue);				/*!< Retourne la pondération en dB*/
	/** @} */

	/**
     * Retourne le fichier de session utilisateur de l'application
     */
	static wxFileConfig* GetFileConfig();
	/**
	 * Retourne le document XML de préférence de l'utilisateur
	 */
	static smart_ptr<Element> GetUserPreferenceRoot();
	static void OnApplicationClose();

	/** @defgroup tabvecteurs Vecteurs disponibles globalement dans le projet
     *  Les vecteurs ici servent à partager des informations de positions spatiales entre tout les modules faisant appel à la classe de configuration
	 *	Ces données ne sont pas sauvegardées à la fermeture du programme
	 *  @{
	 */
	static vec3 GetVecParam(VECPARAM_ELEMENT param); 				/*!< Retourne le vecteur VECPARAM_ELEMENT*/
	static void SetVecParam(VECPARAM_ELEMENT param,vec3 newVal);	/*!< Affecte la nouvelle valeur du vecteur VECPARAM_ELEMENT */
	/** @} */


	static void SetRootScene(Element* _rootScene);					/**< Met à jour le pointeur de racine du projet */
	static void SetUserPreferenceElement(smart_ptr<Element> userPreference);	/**< Met à jour le pointeur de racine des preferences de l'utilisateur */
	static Element* GetRootScene();									/**< Retourne l'élément racine du projet (NULL si aucun) */

	/**
	 * Cette méthode retourne le tableau liant les faces avec les éléments du projet. Elle permet d'obtenir les matériaux associés aux faces du modèle.
	 *
	 */
	static void GetFaceLink( std::vector<std::vector<t_PropFace> > &facesAssociations);

	/** @defgroup appconfigspectre Méthodes liées aux spectres
	 *  @{
	 */

	/**
	*	\brief Enregistrement correspondant à un spectre du projet (Référence ou Utilisateur)
	*	@see AddSpectrum()
	*	@see DeleteSpectrum()
	*	@see GetSpectrum()
	*	@see GetFreeSpectrumId()
	*	@see IsIdSpectrumExist()
	*	@see GetLstSpectre()
	*	@see E_GammeFrequence
	*/
	struct t_lstSpec{
	  int idSpectre;
	  Element::ELEMENT_TYPE typeSpectre;
	  wxString nom;
	};

	/**
	 * Ajoute un spectre dans la liste des spectres
	 * @param elToAdd Adresse du spectre à ajouter
	 * @see DeleteSpectrum()
	 * @see E_GammeFrequence
	 */
	static void AddSpectrum(E_GammeFrequence* elToAdd);

	/**
	 * Supprime un spectre de la liste des spectres
	 * @param xmlId Identifiant du spectre à supprimer
	 * @see AddSpectrum()
	 * @see Element::t_elementInfo::xmlIdElement
	 */
	static void DeleteSpectrum(int xmlId);
	/**
	 * Retourne le spectre ayant le numéro de spectre idSpectre
	 * @param idSpectre Un entier désignant le n° du spectre (ce n'est PAS xmlIdElement)
	 * @param typeSpectre Le type de spectre, APP ou USER
	 * @see E_GammeFrequence::GetIdSpectre()
	 * @return Le spectre si existant, NULL si inexistant
	 */
	static E_GammeFrequence* GetSpectrum(int idSpectre,Element::ELEMENT_TYPE typeSpectre);

	/**
	 * Appelé uniquement par le constructeur de E_GammeFrequence, cette méthode permet d'obtenir un indice unique de spectre
	 * @see E_GammeFrequence
	 * @return Un numéro de spectre non utilisé
	 */
	static int GetFreeSpectrumId();
	/**
	 * Méthode appelé par le constructeur de E_GammeFrequence lors de l'importation XML afin de s'assurer de l'unicité de son indice
	 * @param idSpectre Indice du spectre (ce n'est PAS xmlIdElement)
	 * @see E_GammeFrequence::GetIdSpectre()
	 * @see E_GammeFrequence
	 * @return Vrai si cet indice correspond a un spectre
	 */
	static bool IsIdSpectrumExist(int idSpectre);
	/**
	 * Utilisé dans les listes de spectres afin d'obtenir les noms des spectres et leurs indices
	 * @see AddSpectrum()
	 * @see t_lstSpec
	 * @return Une liste de t_lstSpec
	 */
	static std::vector<t_lstSpec> GetLstSpectre();

	/**
	 * Cette méthode retourne le noeud XML correspondant à la BDD de référence de spectres du projet
	 * @see E_Scene_Bdd_Spectrums_Application()
	 * @return wxXmlNode Noeud Xml
	 */
	static wxXmlNode* GetAppSpectreNode();
	/** @} */

	/** @defgroup appconfigmaterials Méthodes liées aux matériaux
	 *  @{
	 */
	/*!
	*	@brief Enregistrement correspondant à un matériau du projet (Référence ou Utilisateur)
	*	@see AddMateriau()
	*	@see DeleteMateriau()
	*	@see GetMateriau()
	*	@see GetFreeMateriauId()
	*	@see IsIdMateriauExist()
	*	@see GetAppMateriauNode()
	*	@see E_Materiau
	*/
	struct t_lstMat{
	  int idMateriau;
	  Element::ELEMENT_TYPE typeMateriau;
	  wxString nom;
	};

	/**
	 * Ajoute un matériau dans la liste des matériaux
	 * @param elToAdd Adresse du matériau à ajouter
	 * @see DeleteMateriau()
	 * @see E_Materiau
	 */
	static void AddMateriau(E_Materiau*);
	/**
	 * Supprime un matériau dans la liste des matériaux
	 * @param xmlId Identifiant du matériau à supprimer
	 * @see AddMateriau()
	 * @see Element::t_elementInfo::xmlIdElement
	 */
	static void DeleteMateriau(int xmlId);
	/**
	 * Supprime toutes les références aux matériaux dans la liste des matériaux
	 * @see AddMateriau()
	 * @see Element::t_elementInfo::xmlIdElement
	 */
	static void DeleteAllMateriaux();
	/**
	 * Retourne le matériau ayant le numéro de matériau idMateriau
	 * @param idMateriau Un entier désignant le n° du matériau (ce n'est PAS xmlIdElement)
	 * @param typeMateriau Le type de matériau, APP ou USER
	 * @see E_Materiau::GetIdMateriau()
	 * @return Le matériau si existant, NULL si inexistant
	 */
	static E_Materiau* GetMateriau(int idMateriau);
	/**
	 * Retrouve l'indice du matériau en indiquant l'adresse d'un élément contenant un matériau
	 * @param pereEle Element ayant un matériau dans ses fils
	 * @return L'indice du matériau, -1 si non trouvé.
	 */
	static int GetMateriauId(Element* pereEle);
	/**
	 * Appelé uniquement par le constructeur de E_Materiau, cette méthode permet d'obtenir un indice unique de matériau
	 * @see E_Materiau()
	 * @return Un numéro de matériau non utilisé
	 */
	static int GetFreeMateriauId();
	/**
	 * Méthode appelé par le constructeur de E_Materiau lors de l'importation XML afin de s'assurer de l'unicité de son indice
	 * @param idMateriau Indice du matériau (ce n'est PAS xmlIdElement)
	 * @see E_Materiau::GetIdSpectre()
	 * @see E_Materiau
	 * @return Vrai si cet indice correspond a un matériau
	 */
	static bool IsIdMateriauExist(int idMateriau);
	/**
	 * Cette méthode retourne le noeud XML correspondant à la BDD de référence de matériaux du projet
	 * @see E_Scene_Bdd_Materiaux_Application()
	 * @return wxXmlNode Noeud Xml
	 */
	static wxXmlNode* GetAppMateriauNode();
	/**
	 * Utilisé dans les listes de matériaux afin d'obtenir les noms des matériaux et leurs indices
	 * @see AddMateriau()
	 * @see t_lstMat
	 * @return Une liste de t_lstMat
	 */
	static std::vector<t_lstMat> GetLstMateriaux();
	/** @} */
	/** @defgroup appconfigdirectiv Méthodes liées aux directivités
	*  @{
	*/

	/**
	*	\brief Enregistrement correspondant à une directivité du projet (Référence ou Utilisateur)
	*	@see AddDirectivity()
	*	@see DeleteDirectivity()
	*	@see GetDirectivity()
	*	@see GetFreeDirectivityId()
	*	@see IsIdDirectivityExist()
	*	@see GetLstDirectivity()
	*	@see E_Directivity
	*/
	struct t_lstDirectiv {
		int idDirectivity;
		Element::ELEMENT_TYPE typeDirectivity;
		wxString nom;
	};

	/**
	* Ajoute une directivité dans la liste des spectres
	* @param elToAdd Adresse de la directivité à ajouter
	* @see DeleteDirectivity()
	* @see E_Directivity
	*/
	static void AddDirectivity(E_Directivity* elToAdd);

	/**
	* Supprime une directivité de la liste des directivités
	* @param xmlId Identifiant de la directivité à supprimer
	* @see AddDirectivity()
	* @see Element::t_elementInfo::xmlIdElement
	*/
	static void DeleteDirectivity(int xmlId);
	/**
	* Retourne la directivité ayant le numéro de directivité idDirectivity
	* @param idDirectivity Un entier désignant le n° de directivité (ce n'est PAS xmlIdElement)
	* @see E_Directivity::GetIdDirectivity()
	* @return La directivité si existant, NULL si inexistant
	*/
	static E_Directivity* GetDirectivity(int idDirectivity);
	/**
	* Retrouve l'indice de la directivité en indiquant l'adresse d'un élément contenant une directivité
	* @param pereEle Element ayant une directivité dans ses fils
	* @return L'indice de la directivité, -1 si non trouvé.
	*/
	static int GetDirectivityId(Element* pereEle);
	/**
	* Appelé uniquement par le constructeur de E_Directivity, cette méthode permet d'obtenir un indice unique de directivité
	* @see E_Directivity
	* @return Un numéro de directivité non utilisé
	*/
	static int GetFreeDirectivityId();
	/**
	* Méthode appelé par le constructeur de E_Directivity lors de l'importation XML afin de s'assurer de l'unicité de son indice
	* @param idDirectivity Indice de la directivité (ce n'est PAS xmlIdElement)
	* @see E_Directivity::GetIdDirectivity()
	* @see E_Directivity
	* @return Vrai si cet indice correspond a une directivité
	*/
	static bool IsIdDirectivityExist(int idDirectivity);

	/**
	 * @return Application ressources path
	 */
	static wxString getResourcesFolder();
	/**
	* Utilisé dans les listes de directivités afin d'obtenir les noms des directivités et leurs indices
	* @see AddDirectivity()
	* @see t_lstDirectiv
	* @return Une liste de t_lstDirectiv
	*/
	static std::vector<t_lstDirectiv> GetLstDirectivity();

	/**
	* Cette méthode retourne le noeud XML correspondant à la BDD de référence de directivités du projet
	* @see E_Scene_Bdd_Directivity_Application()
	* @return wxXmlNode Noeud Xml
	*/
	static wxXmlNode* GetAppDirectivityNode();
	/** @} */
		/** @defgroup appconfigrefelement Méthodes de gestion des données
	 *  @{
	 */
	/**
	 * Ajout d'un élément dans le tableau global
	 * Grâce à cette méthode des éléments ayant un groupe commun peuvent être rassemblés
	 */

	static void AppendRefElement(Element* newRefEl,ELEMENT_REF_TYPE hashIndex);
	/**
	 * Suppression d'un élément dans le tableau global
	 * Grâce à cette méthode des éléments ayant un groupe commun peuvent être rassemblés
	 */
	static void DeleteRefElement(Element* refElToBroke,ELEMENT_REF_TYPE hashIndex);
	static t_HashElement::iterator BeginRef(ELEMENT_REF_TYPE hashIndex);
	static t_HashElement::iterator EndRef(ELEMENT_REF_TYPE hashIndex);
	static t_HashElement::size_type SizeRef(ELEMENT_REF_TYPE hashIndex);
	/** @} */
	/** @} */

};

#endif
