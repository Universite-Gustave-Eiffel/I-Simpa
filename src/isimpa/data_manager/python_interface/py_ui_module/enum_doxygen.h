
//Généré par generate_enum_def.py le datestamp
/**
 * @file enum_doxygen.h
 * \~french Déclaration des énumérations
 * \~english Enum declarations
 */
namespace uictrl
{

 /**
     *  \~french Enumeration des fonctions "built-in" disponibles pour les éléments
     *  \~english Avaible built-in events
     */
    enum idevent
	{
		IDEVENT_DELETE_ELEMENT,/*!< \~french Supprime un element \~english Element delete */
		IDEVENT_RENAME_ELEMENT,/*!< @param  {"name": "new label"} \~french Renommer un élément \~english Element rename  */
		IDEVENT_COPIER,/*!< \~french Evenement \~english built-in event */
		IDEVENT_COLLER,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_SURFACE_GROUP,/*!< \~french Evenement \~english built-in event */
		IDEVENT_GETPROPERTIES,/*!< \~french Evenement \~english built-in event */
		IDEVENT_SELECT_TREE_ITEM,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_RECEPTEUR_P,/*!< \~french Evenement \~english built-in event */
		IDEVENT_SELECT_POSITION,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_SOURCE,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_RECEPTEUR_S,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_USERFREQ,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_ENCOMBREMENT,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_ENCOMBREMENT_CUBOIDE,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_USERMAT,/*!< \~french Evenement \~english built-in event */
		IDEVENT_NEW_MATERIAL_GROUP,/*!< \~french Evenement \~english built-in event */
		IDEVENT_RUN_CALCULATION,/*!< \~french Evenement \~english built-in event */
		IDEVENT_IMPORT_MATERIAL,/*!< @param {"path": "material file path"} \~french Importer un fichier de matériaux catt ou odeon \~english Import material file from odeon or Catt */
		IDEVENT_LOAD_PARTICLE_SIMULATION,/*!< \~french Evenement \~english built-in event */
		IDEVENT_LOAD_PARTICLE_SIMULATION_PATH,/*!< \~french Evenement \~english built-in event */
		IDEVENT_RELOAD_FOLDER,/*!< \~french Evenement \~english built-in event */
		IDEVENT_DELETE_FOLDER,/*!< \~french Evenement \~english built-in event */
		IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP,/*!< \~french Evenement \~english built-in event */
		IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP_SUM,/*!< \~french Evenement \~english built-in event */
		IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM,/*!< \~french Evenement \~english built-in event */
		IDEVENT_INVERT_FACE_ORIENTATION,/*!< \~english built-in event \~french Evenement inverser l'orientation des faces sélectionnées */
		IDEVENT_EMPTY_POINTER_VERTEX_GROUP,/*!< \~english built-in event \~french Evenement vider un pointeur de groupe de surface*/
		IDEVENT_RECP_COMPUTE_ACOUSTIC_PARAMETERS,/*!< \~english built-in event \~french Evenement calculer les paramètre acoustique d'un recepteur ponctuel @param {"C": "Clarté (ms)","D":"Définition (ms)","TR":"Décroissance pour le calcul du TR"} */
		IDEVENT_RECEPTEURS_COMPUTE_TR,/*!< \~english built-in event \~french Evenement calcul de la cartographie du tr */
		IDEVENT_RECEPTEURS_COMPUTE_EDT,/*!< \~english built-in event \~french Evenement calcul de la cartographie de l'edt */
		IDEVENT_RECEPTEURS_COMPUTE_CLARITY,/*!< \~english built-in event \~french Evenement calcul de la cartographie */
		IDEVENT_RECEPTEURS_COMPUTE_DEFINITION,/*!< \~english built-in event \~french Evenement calcul de la cartographie */
		IDEVENT_RECEPTEURS_COMPUTE_TS,/*!< \~english built-in event \~french Evenement calcul de la cartographie */
		IDEVENT_RECEPTEURS_COMPUTE_ST,/*!< \~english built-in event \~french Evenement calcul de la cartographie */
		IDEVENT_REPORT_PARTICULES_MAKE_GABE,/*!< \~english built-in event \~french Evenement création du graph niveau sonore en fonction du temps*/
		IDEVENT_BFREQ_PRESELECTION_NONE,/*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
		IDEVENT_BFREQ_PRESELECTION_THIRD_BAND,/*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
		IDEVENT_BFREQ_PRESELECTION_BAND,/*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
		IDEVENT_BFREQ_PRESELECTION_BUILDING_THIRD_BAND,/*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
		IDEVENT_BFREQ_PRESELECTION_BUILDING_BAND,/*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
		IDEVENT_RECP_COMPUTE_ADVANCED_ACOUSTIC_PARAMETERS,/*!< \~english built-in event \~french Evenement calculer les paramètre acoustique d'un recepteur ponctuel @param  {"LF": "Fraction d'énergie latérale précoce LF (ms)","LFC":"Fraction d'énergie latérale précoce LFC (ms)","LG":"Tenue acoustique latérale LG(ms)","G":"Tenue acoustique G (ms)"} */
		IDEVENT_NEW_RECEPTEURP_GROUP,/*!< \~english built-in event \~french Nouveau groupe de récepteur */
		IDEVENT_NEW_SOURCE_GROUP,/*!< \~english built-in event \~french Nouveau groupe de sources */
		IDEVENT_NEW_SURFACE_GROUP_FROM_SELECTION,/*!< \~english built-in event \~french Nouveau groupe de surface à partir de la sélection */
		IDEVENT_LOAD_RECEPTEURSP_SIMULATION,/*!< \~english built-in event \~french Chargement de l'animation des récepteurs ponctuels */
		IDEVENT_BUILD_VOLUMES_FROM_TRIMESH,/*!< \~english built-in event \~french Déclaration des volumes à partir du maillage */
		IDEVENT_NEW_VOLUME,/*!< \~english built-in event \~french Ajout d'un volume*/
		IDEVENT_OPEN_FOLDER,/*!< \~english built-in event \~french Ouverture d'un dossier */
		IDEVENT_CONVERT_VOL_TO_FITTING,/*!< \~english built-in event \~french Converti le volume en encombrement scène */
		IDEVENT_NEW_RECEPTEUR_S_COUPE,/*!< \~english Add a cutting plan receiver event \~french Ajouter une récepteur en coupe */
		IDEVENT_BFREQ_PRESELECTION_ALL,/*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
		IDEVENT_LAST_FIXED,

	};

 /**
     *  \~french Enumeration de tout les types d'élément "built-in" existant
     *  \~english Avaible built-in element types
     */
    enum element_type
	{
		ELEMENT_TYPE_CORE_ROOT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_RESULT_ROOT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_ROOT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_GROUPESURFACES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR_PROPRIETES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSS,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_PROPRIETES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_SOURCES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_SOURCES_SOURCE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PUISSANCE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_SOURCES_SOURCE_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_TEXT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_COLOR,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_POSITION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_LIST,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_INTEGER,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_FLOAT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_BOOL,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_RENDU_MODEL,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_SPECTRUMS,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_USER,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_APP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_GROUP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_MATERIAU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_GROUP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_MATERIAU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_PROPMATERIAU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_CATMATERIAL,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_MATERIAU_APP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_MATERIAU_USER,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_GAMMEFREQ_APP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_GAMMEFREQ_USER,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_PROPERTY_FREQ,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_ROW,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_ROW_BFREQ,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_ROW_MATERIAU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_ENCOMBREMENTS,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_GAMMEABSORPTION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_CORE_SPPS,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_CORE_CORE_CONFIG,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_USERCONFIGURATION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_RENDU_PARTICULES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_DONNEES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_ENVIRONNEMENTCONF,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_DRAWABLE,/*!< \~french Element pouvant être représenté dans la vue 3D \~english 3D object element */
		ELEMENT_TYPE_CORE_CORE_BFREQSELECTION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_BOOL_BFREQ,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_FOLDER,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_PARTVISUALISATION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_GABE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_GABE_RECP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_TREE_LIST,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_CORE_TC,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_PROJET_INFORMATION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_BDD_MATERIAUX_MATERIAU_RENDER,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_FONT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_CORE_TLM,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_GABE_GAP,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_UNKNOWN,/*!< Fichier inconnu par PSPS mais connu par le système d'exploitation */
		ELEMENT_TYPE_CORE_SPPS_OCTREE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_RPI,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_VOLUMES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_VOLUMES_VOLUME,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_PROPRIETES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_PYTHON_EXTENSION,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_ELEMENT,/*!< Lors de la déclaration d'un élément utilisateur, ce type permet d'exprimer le fait que l'élément hérite directement de l'élément de base*/
		ELEMENT_TYPE_CORE_CORE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_FILE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_STANDART,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_GAIN,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_TR,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_EDT,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_PROPRIETES,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
		ELEMENT_TYPE_USER_PREFERENCE_NODE,/*!< \~french élément de noeud des préférences utilisateurs \~english User preference node, in the user prefrence tree*/
		ELEMENT_TYPE_USER_PREFERENCE_ITEM,/*!< \~french élément des préférences utilisateurs \~english User preference item, in the user prefrence tree*/
		ELEMENT_TYPE_USER_PREFERENCE_ITEM_ISOTEMPLATE,/*!< \~french élément des préférences utilisateurs qui liste les palettes de couleurs iso \~english User preference item, in the user prefrence tree where user can choose iso palette.*/
		ELEMENT_TYPE_ROW_EXTBFREQ,/*!< \~french élément ligne de frequence contenant également une attenuation \~english Extanded freq row property */

	};

 /**
     *  \~french Enumeration des icones déjà chargées
     *  \~english Application tree icons
     */
    enum graph
	{
		GRAPH_FOLDER,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_ITEM,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_FOLDER_OPEN,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_FITTINGS_OPEN,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_FITTINGS_CLOSE,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_FITTING_OPEN,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_FITTING_CLOSE,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_PUNCTUAL_RECEIVERS_OPEN,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_PUNCTUAL_RECEIVERS_CLOSE,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_SURFACE_RECEIVERS_OPEN,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_SURFACE_RECEIVERS_CLOSE,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_SOUND_SOURCES_OPEN,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_SOUND_SOURCES_CLOSE,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_SURFACES_OPEN,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_SURFACES_CLOSE,/*!< \~french Indice d'une image \~english Built-in picture declaration */
		GRAPH_VOLUMES_CLOSE,
		GRAPH_VOLUMES_OPEN,
		GRAPH_PROJECT_OPEN,
		GRAPH_PROJECT_CLOSE,
		GRAPH_DATA_CLOSE,
		GRAPH_DATA_OPEN,
		GRAPH_USER_MATERIALS_CLOSE,
		GRAPH_USER_MATERIALS_OPEN,
		GRAPH_APPLICATION_MATERIALS_CLOSE,
		GRAPH_APPLICATION_MATERIALS_OPEN,
		GRAPH_DATABASE_CLOSE,
		GRAPH_DATABASE_OPEN,
		GRAPH_USER_SPECTRUMS_CLOSE,
		GRAPH_USER_SPECTRUMS_OPEN,
		GRAPH_APPLICATION_SPECTRUMS_CLOSE,
		GRAPH_APPLICATION_SPECTRUMS_OPEN,
		GRAPH_MATERIAL_CLOSE,
		GRAPH_MATERIAL_OPEN,
		GRAPH_STANDARTCORE_CLOSE,
		GRAPH_STANDARTCORE_OPEN,
		GRAPH_CORES_CLOSE,
		GRAPH_CORES_OPEN,
		GRAPH_SPPSCORE_CLOSE,
		GRAPH_SPPSCORE_OPEN,
		GRAPH_DISK_FOLDER_OPEN,
		GRAPH_DISK_FOLDER_CLOSE,
		GRAPH_ENVIRONMENT,
		GRAPH_PROJECT_AUTHOR,
		GRAPH_INFORMATION,
		GRAPH_DISK_DEFAULT_FILE,
		GRAPH_DISK_GABE,
		GRAPH_DISK_RS,
		GRAPH_DISK_PARTICLE,
		GRAPH_EL_CONFIGURATION,
		GRAPH_EL_3D_DISPLAY,
		GRAPH_EL_POSITION,
		GRAPH_EL_TRIANGLE,
		GRAPH_SPECTRUM,
		GRAPH_ORIGIN,
		GRAPH_TETMESH_PARAMETERS,
		GRAPH_RENDERING_FOLDER_CLOSE,
		GRAPH_RENDERING_FOLDER_OPEN,
		GRAPH_ROOT_MATERIALS_OPEN,
		GRAPH_ROOT_MATERIALS_CLOSE,
		GRAPH_ROOT_SPECTRUMS_OPEN,
		GRAPH_ROOT_SPECTRUMS_CLOSE,
		GRAPH_PUNCTUAL_RECEIVER_OPEN,
		GRAPH_PUNCTUAL_RECEIVER_CLOSE,
		GRAPH_SURFACE_RECEIVER_OPEN,
		GRAPH_SURFACE_RECEIVER_CLOSE,
		GRAPH_SOUND_SOURCE_OPEN,
		GRAPH_SOUND_SOURCE_CLOSE,
		GRAPH_VOLUME_OPEN,
		GRAPH_VOLUME_CLOSE,
		GRAPH_PREF_ANIMATION,
		GRAPH_PREF_GENERAL,
		GRAPH_PREF_LEGEND,
		GRAPH_PREF_NOISE_MAP,
		GRAPH_PREF_PARTICLES,
		GRAPH_USER_PREF_ROOT_CLOSE,
		GRAPH_USER_PREF_ROOT_OPEN,
		GRAPH_USER_PREF_MAINCONFIGURATION_OPEN,
		GRAPH_USER_PREF_MAINCONFIGURATION_CLOSE,
		GRAPH_USER_PREF_HISTORY,
		GRAPH_LAST_STATIC_GRAPH,/*!< \~french Dernier icône \~english Last graph id */

	};

    /**
     * @brief Alias
     */
    struct Element
    {
        typedef element_type ELEMENT_TYPE;
        typedef idevent IDEVENT;
        typedef graph GRAPH;
    };
}

