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



/*! \file projet.h
    \brief Classe de gestion de projet

	Permet d'enregistrer, de charger un projet à partir d'un fichier.
	Gestion des événements générés par Element .

	Il permet d'acceder a tout les éléments composant le projet

	C'est le coeur du programme
*/


#include <wx/dynarray.h>
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/aui/aui.h>
#include <wx/toolbar.h>
#include <wx/progdlg.h>

#include "IHM/uiTreeCtrl.h"
#include "IHM/MainPropGrid.h"
#include "IHM/pyConsole.hpp"
#include "3dengine/OpenGlViewer.h"
#include "3dengine/Core/Objet3D.h"
#include "3dengine/Core/Particules.h"
#include "3dengine/Core/Recepteurs_surfacique.h"
#include "3dengine/Core/Recepteurs_ponctuel_intensity.hpp"

#include "manager/modelRetriever.hpp"
#include "data_manager/appconfig.h"

#ifndef __PROJECTMANAGER_H__
#define __PROJECTMANAGER_H__

WX_DECLARE_OBJARRAY(wxXmlDocument, ArrayOfXmlTree);
WX_DECLARE_STRING_HASH_MAP( wxString, StringHash );

namespace uictrl
{
	class application;
}
class PythonShell;
/**
 *  \brief Classe de gestion de projet
 *
 *  Cette classe représente un projet, elle a une vue globale sur tous les éléments composants le programme
 *	On peut l'assimiler comme le chef d'orchestre	Permet d'enregistrer, de charger un projet à partir d'un fichier.
 *
 *	Gestion des événements générés par Element .
 *
 *	Il permet d'acceder a tout les éléments composant le projet
 */

class ProjectManager{
private:
	/**
	 * @brief Structure de données définissant les paramètres de maillage
	 */
	struct param_TetGenMaillage
	{
		bool debugMode;					/*!< Mode débuggage -d */
		bool isMaxVolume;				/*!< Contrainte de volume */
		bool isAreaConstraint;			/*!< Contrainte d'aire sur les récepteurs de surface */
		bool doMeshRepair;			/*!<  Activer la correction du maillage*/
		float maxVolume;				/*!< Valeur de la contrainte de volume */
		float quality;					/*!< Rapport rayon arrete */
		wxString cmd_append;			/*!< Ajout de paramètres à l'execution du maillage */
		wxString user_defined_params;	/*!< Remplacement des paramètres standart par les paramètres utilisateurs */
		float maxAreaOnRecepteurss;		/*!< Aire maximal des faces des tetrahèdre coplainaires au faces composants les recepteurs surfaciques. */
	};

	/**
	 * @brief Données de liens entre les faces et les groupes
	 * Cette structure de données permet de ne pas perdre les groupes de surfaces lors de l'import d'une scène remplacant l'actuelle où seul certaines faces ont étés rajoutés
	 */
	struct t_retrieves_groups
	{
		ModelRetriever retrieverTool;
		std::vector<Element*> containers; //Indice de modelRetriever correspond à l'indice de cette table contenant les liens vers les groupes
	};




public:

	/** @defgroup genprojectmanager Classe ProjectManager
	 *	\brief Gestionnaire d'un projet
	 *  @{
	 */
	/**
	 * Constructeur, chargement du dernier projet utilisé, si aucun chargement d'un nouveau projet
	 */
	ProjectManager();						// chargement d'un nouveau projet
	/**
	 * Destructeur, décharge les éléments
	 */
	~ProjectManager();

	/**
	 * Enumeration des controles contenant les éléments.
	 */
	enum eventCtrl {
		EVENT_CTRL_SCENE,
		EVENT_CTRL_REPORT,
		EVENT_CTRL_CORE,
		EVENT_CTRL_USERCONFIG
	};

	/**
	 * @brief Données de paramètres pour l'importation d'un modèle 3D
	 * Cette structure de données permet de spécifier les préférence quand aux traitements à appliquer lors de l'import d'un modèle
	 */
	struct t_param_load_model
	{
		t_param_load_model():paramTetgen("-pq5.00 -T.00001 -E"),epsilonLinkingFaceGroup(0.f),glueSurfaces(true) { }
		wxString pathModel;
		bool keepexistingfacegroup;
		bool docorrection;
		bool domeshsurface;
		bool glueSurfaces;
		bool launchRemeshWizard;
		wxString paramTetgen;
		float epsilonLinkingFaceGroup; //Distance maximum de lien face / ancien groupe
	};


	/**
	 * Initialise les contrôles et charge le projet courant
	 * @warning Appeler uniquement aprés SetControlPointer
	 */
	void ControlPointerInitialisation();
	/**
	 * Appelé par la fênetre principale, attache les contrôles à cette classe.
	 * @param _logControl Fenêtre de log ( messages)
	 * @param _treeCore Fenêtre d'élément hiérarchique des éléments de coeur de calcul.
	 * @param _treeResult Fenêtre d'élément hiérarchique des éléments de résultat
	 * @param _treeScene Fenêtre d'élément hiérarchique des éléments de scène
	 * @param _glToolBar Barre d'outils 3D
	 * @param _mainFrame Fenêtre principale du logiciel
	 * @param _GlFrame Fenêtre 3D
	 * @param _propFrame Grille de propriétés
	 * @param _auiManager Gestionnaire de fenêtres
	 */
	void SetControlPointer(	wxTextCtrl* _logControl,uiTreeCtrl* _treeCore, uiTreeCtrl* _treeResult ,
		uiTreeCtrl* _treeScene, uiTreeCtrl* _treeUserPref,wxToolBar* _glToolBar,wxFrame* _mainFrame,OpenGlViewer* _GlFrame,MainPropGrid * _propFrame,wxAuiManager* _auiManager,PyConsole* pyCtrl );

	/** @defgroup osprojprojectmanager Chargement,Sauvegarde du projet
	 *  Ces méthodes sont appelées pour sauvegarder ou charger un projet.
	 *  @{
	 */
	/**
	 * Cette méthode créé un nouveau projet et le charge comme projet courant
	 */
	void NewProject();
	/**
	 * Cette méthode charge le projet dans le dossier courant.
	 * @param reloadXmlFile A faux le document chargé dans l'objet projetConfig sera ouvert. Sinon le fichier du projet sera lu et ouvert.
	 */
	void LoadCurrentProject(bool reloadXmlFile=true);
	/**
	 * Cette méthode charge un fichier de projet
	 * @param filename Chemin du fichier
	 * @return Vrai si le chargement a réussi
	 */
	bool Open(const wxString&filename);
	/**
	 * Enregistre le projet dans un autre dossier
	 * @param filename Chemin du fichier
	 * @param updateConfig A vrai le chemin de sauvegarde du projet est mis à jour ( la sauvegarde standard ce fera dans ce dossier à présent)
	 * @return Vrai si la sauvegarde a réussi
	 */
	bool SaveTo( const wxString& filename, bool updateConfig=true );
	/**
	 * Sauvegarde le projet sur l'emplacement spécifié dans le fichier de configuration E_Scene_Configuration
	 * @return Vrai si la sauvegarde a réussi
	 */
	bool Save( );
	/**
	 * Enregistre une copie du projet dans un autre dossier
	 * @param filename Chemin du fichier
	 * @return Vrai si la sauvegarde a réussi
	 */
	bool SaveCopy(const wxString&filename );
	/**
	 * Transfert les modifications effectuées sur les éléments dans l'objet XML en mémoire
	 * @return Vrai si l'opération a réussi
	 */
	bool SaveChanges( );
	/**
	 * Ferme le projet courant
	 */
	void CloseApp();
	/**
	 * Efface les fichiers et sous dossiers du dossier en paramètre
	 * @param folderToEmpty Chemin absolue ou relatif au dossier temporaire
	 */
	static bool ClearFolder( wxString folderToEmpty );
	/**
	 * Copie les fichiers et sous dossiers du dossier en paramètre
	 * @param folderToEmpty Chemin absolue ou relatif au dossier temporaire
	 */
	bool CopyFolder( wxString folderToCopy, wxString toFolder );

	/**
	 * Retourne le nom des fichiers et dossiers du dossier en paramètre
	 * @param rootFolder Chemin absolue ou relatif au dossier à lire
	 * @param folderDir Liste de chaines de caractères
	 */
	static void GetAllFolderItems(wxString rootFolder,wxArrayString& folderDir, bool seekSubDirs=true);
	/** @} */
	/**
	 * Renomme l'entete de la fenêtre principale
	 * @param Libelle Nouveau libellé
	 * @param Updated A vrai ajoute une étoile a coté du libellé pour indiquer que le projet a été modifié.
	 */

	void SetMainFrameName(wxString Libelle,bool Updated);
	/**
	 * Gestion de l'événement d'un élément
	 * Généralement invoqué par le menu popup des éléments dans l'arbre.
	 * @param eventElement Objet événement lié
	 * @param fromCtrl Indice du contrôle lié à l'événement
	 */
	void ElementEvent(wxCommandEvent& eventElement,eventCtrl fromCtrl,Element* eltevt=NULL);
	//Commandes de la fenêtre principale

	/** @defgroup cmdprojectmanager Commandes de la fenêtre principale
	 *  Ces méthodes sont appelées suite au clic dans le menu de la fenêtre principale
	 *  @{
	 */
	/**
	 * Barre d'outils->Switch camera mode
	 * L'utilisateur souhaite changer de mode de caméra
	 * @param event Objet lié à l'événement
	 * @param cameraMode Nouveau mode de caméra souhaité
	 */
	void OnChangeCameraMode(wxCommandEvent& event,OpenGlViewer::CAMERA_MODE cameraMode);
	/**
	 * L'utilisateur active le rendu des faces extérieures
	 * @param event Objet lié à l'événement
	 */
	void OnModeFlatOutside(wxCommandEvent& event);
	/**
	 * Evenement provenant d'une fenetre du projet
	 */
	void OnMainEvent( ApplicationConfiguration::MAIN_EVENT eventId,wxCommandEvent& event);
	/**
	 * L'utilisateur active le rendu des faces intérieures
	 * @param event Objet lié à l'événement
	 */
	void OnModeFlatInside(wxCommandEvent& event);
	/**
	 * A partir de la normale des face calcul le contour des surfaces de la scène afin de rendre une visualisation épurée du modèle en mode filaire.
	 */
	void DoShapeComputation(const bool userEvent=false,const bool resetFaceOrientation=false);
	/**
	 * L'utilisateur désactive le rendu des faces
	 * @param event Objet lié à l'événement
	 */
	void OnModeFlatNone(wxCommandEvent& event);
	/**
	 * L'utilisateur active le rendu des contour des faces
	 * @param event Objet lié à l'événement
	 */
	void OnModeWireFrameFull(wxCommandEvent& event);
	/**
	 * L'utilisateur active le rendu des contour des faces de forme
	 * @param event Objet lié à l'événement
	 */
	void OnModeWireFrameShape(wxCommandEvent& event);
	/**
	 * L'utilisateur désire voir la coloration des faces de la scène en fonction des couleurs spécifié dans le fichier de modèle
	 * @param event Objet lié à l'événement
	 */
	void OnModeOriginalPaint(wxCommandEvent& event);
	/**
	 * L'utilisateur désire voir la coloration des faces de la scène en fonction des couleurs des matériaux associés aux groupes de faces.
	 * @param event Objet lié à l'événement
	 */
	void OnModeMaterialPaint(wxCommandEvent& event);

	/**
	 * L'utilisateur décide de créer un ou plusieurs rendu de récepteur de surface à partir de la soustraction de deux ou plus fichiers de résult
	 */
	void OnMenuSubstractRecepteurSurf();

	/**
	 * L'utilisateur souhaite importer un modèle externe en effectuant une méthode de remaillage du modèle afin de le rendre compatible avec tout les codes de calculs.
	 */
	void OnStartRemeshWizard();
	/**
	 * L'utilisateur souhaite annuler une modification du projet
	 */
	void OnMenuUndo();
	/**
	 * L'utilisateur souhaite revenir à l'état initial
	 */
	void OnMenuRedo();
	/**
	 * L'utilisateur désactive le rendu en fil de fer
	 * @param event Objet lié à l'événement
	 */

	void OnModeWireFrameNone(wxCommandEvent& event);
	/**
	 * L'utilisateur active le maillage du modèle ( pour prévisualisation)
	 * @param event Objet lié à l'événement
	 */
	void OnMaillage(wxCommandEvent& event);
	/**
	 * L'utilisateur active/désactive le rendu du maillage
	 * @param event Objet lié à l'événement
	 */
	void OnShowHideMaillage(wxCommandEvent& event);
	/**
	 * L'utilisateur active l'outil sélection des faces
	 * @param event Objet lié à l'événement
	 */
	void OnSwitchSelectionMode(wxCommandEvent& event);
	/**
	 * L'utilisateur active l'outil de récuperation des niveau de pression à un point des récepteurs de surface
	 * @param event Objet lié à l'événement
	 */
	void OnGetdbLevelMode(wxCommandEvent& event);
	/**
	 * L'utilisateur active l'outil caméra
	 * @param event Objet lié à l'événement
	 */
	void OnCameraTool(wxCommandEvent& event);
	/**
	 * L'utilisateur a cliqué sur le bouton pas de temps suivant
	 * @param event Objet lié à l'événement
	 */
	void OnNextStepSimulation(wxCommandEvent& event);
	/**
	 * L'utilisateur a cliqué sur le bouton pas de temps précédent
	 * @param event Objet lié à l'événement
	 */
	void OnPreviousStepSimulation(wxCommandEvent& event);
	/**
	 * L'utilisateur active la simulation des particules
	 * @param event Objet lié à l'événement
	 */
	void OnStartSimulation(wxCommandEvent& event);
	/**
	 * L'utilisateur fait glisser la barre de sélection du plan de coupe
	 * @param value Valeur du curseur
	 * @param max valeur maximale de la barre
	 */
	void OnScrollChanged(float value,float max);
	/**
	 * L'utilisateur met en pause la simulation des particules
	 * @param event Objet lié à l'événement
	 */
	void OnPauseSimulation(wxCommandEvent& event);
	/**
	 * L'utilisateur arrête la simulation des particules
	 * @param event Objet lié à l'événement
	 */
	void OnStopSimulation(wxCommandEvent& event);
	/**
	 * L'utilisateur désire supprimer la simulation
	 * @param event Objet lié à l'événement
	 */
	void OnEmptySimulation();
	/**
	 * L'utilisateur désire changer le modèle de la scène
	 * @param FileName Chemin du nouveau modèle
	 */
	void ChangeModel3d(const wxString& FileName);
	/**
	 * Remplacement de la scène
	 * @param paramLoading Paramètre de chargement
	 */
	bool LoadScene(const t_param_load_model& paramLoading);
	/**
	 * L'utilisateur désire créer le modèle de la scène
	 * @param debCuboide Coordonnées de début de la scène
	 * @param finCuboide Coordonnées de fin de la scène
	 */
	void BuildModel3d(vec3 debCuboide,vec3 finCuboide);
	/**
	 * L'utilisateur a sélectionné une face de la scène
	 * @param group Indice du groupe de la face ( indice du groupe de la scène, ce n'est PAS E_Scene_Groupesurfaces_Groupe)
	 * @param vertex Indice de la face
	 */
	void OnSelectVertex PARAM_BOUND_ON_SELECT_FACES;
	/**
	 * L'utilisateur a sélectionné une position dans la vue 3D
	 * @param posValue Coordonnées x,y,z de la position
	 */
	void OnSelectPosition(vec3 posValue);
	/**
	 * L'utilisateur a utilisé l'outil permettant de définir une coordonnées selon la vue 3D
	 */
	void OnAskSelectPosition();
	/**
	 * L'utilisateur désire exporter le modèle de la scène
	 * @param FileName Chemin du fichier
	 */
	void OnExportModel(wxString& FileName);
	/**
	 * L'utilisateur désire modifier le plan de coupe de la scène
	 * @param axe Nouvel axe du plan de coupe ( x:0 y:1 z:2)
	 */
	void OnChangePlan(int axe);
	/**
	 * L'utilisateur désire exporter le rendu 3D de la scène
	 * @param fileName Chemin du fichier
	 */
	void CopyGlToFile( );
	/**
	 * L'utilisateur désire exporter le rendu 3D de la scène
	 * @param fileName Chemin du fichier
	 */
	void CopyGlToFileWithDim();
	/**
	 * L'utilisateur désire exporter l'historique des messages
	 * @param fileName Chemin du fichier
	 */
	void OnSaveConsoleToFile(wxString fileName);
	/**
	 * L'utilisateur désire exporter l'historique de la console python
	 * @param fileName Chemin du fichier
	 */
	void OnSaveShellToFile(wxString fileName);
	/**
	 * L'utilisateur désire effacer l'historique des messages
	 */
	void OnClearConsole();
	void OnClearShell();
	/** @} */
	/**
	 * Lors-ce qu'un element a été modifié la propagation de l'événement va de cette élément jusqu'a la racine des éléments puis vers cette méthode.
	 * C'est ici que l'on peut mettre à jour les contrôles afin de rendre compte de ce changement d'état.
	 * @param elUpdate Pointeur vers l'élément qui a changé d'état.
	 */
	void OnUpdateEvent(Element* elUpdate);
	/**
	 * Met à jour les paramètres correspondant à l'animation 3D
	 * @param animationPropertyElement Noeud ayant les propriétées
	 */
	void UpdateSimulationConfig(Element* animationPropertyElement);

	/** @defgroup keybprojectmanager Etat du clavier
	 *  @{
	 */
	//Etat du clavier
	bool shiftDown;   /*!< Bouton shift pressé */
	bool ctrlDown;	  /*!< Bouton control pressé */
	/** @} */

	//Methode des contrôle
	/**
	* Ajouter un message à l'historique des messages
	* @param message Message a inserrer
	*/
	void AddLogMessage(const wxString& message,const wxColour* msgColor=(wxColour*)NULL);
	/**
	 * Actualise l'arbre de résultat.
	 */
	void RefreshReportFolder();
	/**
	* Charger les propriétés d'un élément dans la grille de propriétés
	* @param elementToOpen Element à ouvrir
	*/
	void OpenPropertyElement(Element* elementToOpen);

	/**
	 * Cette procédure va tenter de réparer le modèle actuellement chargé et optionnelement d'effectuer un maillage de surface
	 * @param paramRepair Paramètre
	 */
	void RepairCurrentMesh(const t_param_load_model& paramRepair );
	/** Etat du projet
	* @return Vrai si le projet a été modifié depuis la dernière sauvegarde
	*/
	bool IsProjectNeedSave();

	/**
	 * Indique si il est possible de revenir en arrière dans les modifications
	 */
	bool CanUndo();
	/**
	 * Indique si il est possible de revenir en avant dans les modifications
	 */
	bool CanRedo();
	/**
	 * Supprimer l'historique des modifications
	 */
	void ClearUndoRedoHistory();
	/** @} */
	/** @defgroup pythonrelatedmethod Contrôle du module python sur le logiciel
	 *  @{
	 */

	/**
	 * Initialisation du moteur Python
	 */
	void InitPythonEngine();
	/**
	 * Execution d'une commande utilisateur
	 * @param cmd Contient une ou des lignes de codes python
	 */
	void RunPythonCmd(const wxString& cmd);

	/**
	 * Supprime les objets python
	 */
	void UnloadPyShell();
	/**
	 * Execution d'une commande python par l'utilisateur
	 * @param eventCmdPython Contient une ligne python
	 */
	void OnPushPythonCommand(wxCommandEvent& eventCmdPython);
	/**
	 * Lors de l'execution d'un evenement il est possible d'ajouter des paramètres
	 */
	void AppendBatchParameters(const wxString& name,const wxString& value);
	/**
	 * Efface les paramètres stockées
	 */
	void ClearBatchParameters();
	/**
	 * Cherche le paramètre dans les arguments définies par le code python
	 * @param[out] value Valeur du paramètre
	 * @param[in] name Nom du paramètre
	 * @return Vrai si le paramètre existe
	 */
	bool GetParameter(const wxString& name,wxString& value);


	void SendPythonEvent(const int& eventid,const int& elementid);

	PythonShell* GetPythonShell();
	/**
	 * Au cas où du code python est associé à un groupe d'élément, le programme donne la main à ces scripts afin de modifier le menu popup
	 */
	bool GetPythonManagedMenu(const int& elementType,const int& elementId, wxMenu& menu);
	/**
	 *	Lors du chargement d'un projet cette méthode est appelé afin d'executer les scripts python devant être éxécuté à cette occasion
	 */
	void PyProjectLoadingEvt();

	/** @} */
private:
	friend class uictrl::application;
	/**
	 * Initialisation des contrôles selon les données du projet courant.
	 * \callgraph
	 */
	void Init( );
	Element* rootCore;
	Element* rootResult;
	Element* rootScene;
	smart_ptr<Element> rootUserConfig;
	/**
	 * Enregistre les modification afin de pouvoir annuler par la suite
	 */
	bool PushModificationToHistory(bool force=false);
	bool RunTetGenBoundaryMesh( wxString cmd, wxString cacheFolder, wxString sceneName, wxString sceneNameExt);
	bool RunRemeshProcess(wxString fileToRemesh);
	void OpenNewDataWindow(Element* linkedElement);
	/**
	 * Ouvre un programme externe avec ce fichier en paramètre
	 */
	void OpenOsExternalApplication(Element* linkedElement);
	bool UpdateZip(wxString folderfrom,wxString zipfilename);
	bool ZipFolder(const wxString&folderfrom,const wxString&zipfilename);
	bool UnZipFolder(const wxString&zipfilename,const wxString&folderTo);

	/**
	 * Créé l'arborescence des paramètres utilisateurs (propre à la session utilisateur et non à la session
	 */
	void CreateUserPreferenceTree();
	/**
	 * Sauvegarde les paramètres utilisateur dans le dossier de session de l'utilisateur
	 */
	void SaveUserPreferenceTree();
	/**
	 * Charge le fichier xml de préférence de l'utilisateur
	 */
	bool LoadUserPreferenceTree();
	/**
	 * Ajout des nouvelles préférences disponibles depuis cette version
	 * @param oldVersion Au format MAJOR.MINOR.REVISION
	 */
	void PatchUserPreferenceTree(const wxString& oldVersion);

	void LoadFacesFromModel(wxProgressDialog* progDiag=NULL,t_retrieves_groups* oldFacesDistribution=NULL,float epsilonDist=0.f );
	void UpdateXmlFile( wxString toFolder="",bool saveToFile=true);
	/**
	 * A partir du projet courant, alimente la structure de données en paramètre.
	 */
	void BuildFaceGroupAssociation(t_retrieves_groups& oldFacesDistribution);
	//Methodes d'événements de Element
	void OnMenuNewRecepteurPonctuel(Element* elrpgroup);
	void OnMenuNewRecepteurSurfacique(Element* elrsgroup,const Element::IDEVENT& evtype);
	void OnMenuNewSurfaceGroup(Element* elrsgroup);
	void OnMenuNewRecepteurPGroup(Element* elrpgroup);

	void OnMenuCreateFGroupFromSelection();
	void OnMenuClearSurfaceGroup(uiTreeCtrl* fromCtrl,Element* eRoot,Element* surfgroup);
	void OnMenuNewSource(Element* source);
	void OnMenuNewSourceGroup(Element* elementSelected);
	void OnMenuNewUserFreq(Element* elementSelected);
	Element* OnMenuNewEncombrement(Element* elementSelected,Element::ELEMENT_TYPE typeEl);

	void OnMenuNewVolume(Element* elementSelected);
	void OnMenuNewUserMat(Element* matgroup);
	void OnMenuNewMatGroup(Element* matgroup);
	void OnMenuImportMaterial();
	void OnMenuLoadParticleSimulation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elPartRecord,Element::IDEVENT loadingMethod);
	void OnMenuReloadFolder(uiTreeCtrl* fromCtrl,Element* eRoot,Element* folderel);
	void OnMenuDeleteFolder(uiTreeCtrl* fromCtrl,Element* eRoot,Element* folderel);
	void OnMenuLoadRecepteurssSimulation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elRsRecord,Element::IDEVENT loadingMethod);
	void OnMenuLoadRecepteurspSimulation(Element* elRpRecord,Element::IDEVENT loadingMethod);

	void OnOpenFolder(Element* folder);
	void OnDrawableElementDelete();


	void OnFindSubVolumes(Element* volumes_el);

	void OnConvertSubVolumeToFitting(Element* volume_el);
	void OnMenuDoAcousticParametersComputation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* rpres); //Calcul des paramètres acoustique d'un récepteur ponctuel

	/**
	 * Calcul des paramètres acoustique d'un récepteur ponctuel LF, LFC et G
	 *
	 * $LF_{t_{1}}^{t_{2}}=\frac{\int_{t_{1}}^{t_{2}}\left[g^{2}\left(t\right).cos^{2}\theta\right]dt}{\int_{0}^{t_{2}}g\left(t\right)^{2}dt}$
	 * $LFC_{t_{1}}^{t_{2}}=\frac{\int_{t_{1}}^{t_{2}}g^{2}\left(t\right)\mid cos\theta\mid dt}{\int_{0}^{t_{2}}g^{2}\left(t\right)dt}$
	 * $LG_{t_{1}}^{t_{2}}=10log_{10}\frac{\int_{t_{1}}^{t_{2}}g^{2}\left(t\right)cos^{2}\theta dt}{\sum\frac{wi}{4\pi*100}}$
	 * $G_{t_{1}}^{t_{2}}=10log_{10}\frac{\int_{t_{1}}^{t_{2}}g^{2}\left(t\right)dt}{\sum\frac{wi}{4\pi*100}}$
	 */
	void OnMenuDoAdvancedAcousticParametersComputation(Element* selectedElement); //
	void OnMenuRecepteurSurfDoAcousticParametersComputation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* rpadvres,const wxString& parametertype); //Calcul des paramètres acoustique d'un récepteur de surface

	/**
	 * Supprimer l'element séléctionné par l'utilisateur
	 */
	void OnMenuDeleteElement(uiTreeCtrl* fromCtrl,Element* eRoot,bool askConfirmation=false,wxString confirmationLabel="");
	void OnMenuRenameElement(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementToRename);
	void OnMenuGetProperties(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementWithProp);

	void OnMenuCopy(uiTreeCtrl* fromCtrl,Element* eRoot);
	void OnMenuPaste(uiTreeCtrl* fromCtrl,Element* ePaste);
	void OnMenuSetPreselection(Element* bfreqEl, Element::IDEVENT preselectionMode );
	void OnSelectElement( uiTreeCtrl* fromCtrl,Element* eRoot);

	void OnMenuFlipFaces();


	bool ClearCurrentFolder( );
	bool BuildEmptyProject(wxXmlNode* noeudRacine);

	//Méthodes de calcul
	void RunCoreCalculation(Element* coreCalculation);
	bool RunCoreMaillage(Element* selectedCore);
	bool RunTetGenMaillage(param_TetGenMaillage& paramMaillage);


	//Algorithme d'importation
	void ImportMaterial(wxString fromFile);
	void ImportMaterialCatt(wxString fromFile);
	void ImportMaterialOdeon(wxString fromFile);

	//Composants du projets
	wxXmlDocument projetConfig;
	wxXmlDocument userPreferenceDocument;
	ArrayOfXmlTree configHistory;					/*!<  Historique des modification */
	wxDateTime lastHistoryUpdate;					/*!<  Date de la dernière sauvegarde des modifications */
	wxUint16 currentHistoryNavigation;				/*!< Indice de la navigation dans l'historique 0:Version courante 1:Dernier enregistrement dans configHistory etc..*/
	ParticulesControler particulesContainer;
	RecepteursSControler recepteursSContainer;
	RecepteursPControler recepteursPContainer;
	PythonShell* pyShell;

	//Contrôles
	wxTextCtrl* logControl;
	PyConsole* shellControl;
	uiTreeCtrl* treeCore;
	uiTreeCtrl* treeResult;
	uiTreeCtrl* treeScene;
	uiTreeCtrl* treeUserPref;
	wxToolBar* glToolBar;
	wxFrame* mainFrame;
	OpenGlViewer* GlFrame;
	MainPropGrid* propFrame;
	wxAuiManager* auiManager;

	//Données du projet
	CObjet3D sceneMesh;

	// Chemins des fichiers
	wxString dossierCourant;
	wxString FichierConfig;
	wxString FichierConfigDefaut;
	wxString PathCores;
	wxString LastComputationFolder; /*<! Dernier dossier temporaire de calcul */
	wxString UserPreferenceXmlFilePath;

	//Variables et objets
	bool ProcessUpdateEvent; /*<! Prend en compte ou non l'execution de OnUpdateEvent */
	bool needSave;
	bool pyeventmode;		/*!< Lors-ce que python contrôle l'application pyeventmode est à vrai */
	bool renderOriginalMaterial;
	StringHash pyenventparams; /*!< Ce tableau de chaîne correspond a ce que taperai l'utilisateur lors d'un evenement */
};

#endif
