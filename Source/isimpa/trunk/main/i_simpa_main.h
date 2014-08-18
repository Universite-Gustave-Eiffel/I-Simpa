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

#include "data_manager/projet.h"
#include <wx/listctrl.h>
#include <wx/laywin.h>
#include <wx/slider.h>
#include "manager/smart_ptr.h"
#include <wx/filedlg.h>
#include <wx/docview.h> //pour l'historique des fichiers ouvert
#include <wx/stdpaths.h>
#include <wx/snglinst.h>
#include <wx/evtloop.h>

ProjectManager *projetCourant;
#define MAX_HISTORY_SHOW 5


/*! \file i_simpa_main.h
    \brief Ce fichier est le fichier racine du projet, la déclaration de l'application générale ce fait ici .
*/


/**
 *	\brief Fenêtre principale du logiciel
 *	La création des sous-fenêtres et l'insertion de tout les controles sont fait au sein de cette classe.
*/
class MainUiFrame : public wxFrame
{
	private:

		wxAuiManager m_mgr;
		smart_ptr<wxSashLayoutWindow> Prop;
		smart_ptr<OpenGlViewer> GlFrame;
		smart_ptr<wxAuiNotebook> ctrlNotebook;
		smart_ptr<wxAuiNotebook> shellNotebook;
		smart_ptr<MainPropGrid> propertiesGrid;
		smart_ptr<wxFileHistory> fileHistory;
		smart_ptr<uiTreeCtrl> treeUserPref;

		wxMenu* affichage_menu;
		wxMenu* affichage_camera_menu;
		wxMenu* affichage_face_menu;
		wxMenu* affichage_ligne_menu;
		wxMenu* edit_menu;
		wxSlider* slPlan;
		wxToolBar* toolbarGl;
		wxRadioButton* xPlan;
		wxRadioButton* yPlan;
		wxRadioButton* zPlan;
		wxToolBar* mousetool;
		uiTreeCtrl* treeCore;
		uiTreeCtrl* treeResult;
		uiTreeCtrl* treeScene;

		bool saveManagerConfig;
		enum
		{
			ID_History,
			ID_SizeEvent = ID_History+MAX_HISTORY_SHOW , //ID_History contient MAX_HISTORY_SHOW enregistrements
			ID_nouveau_projet,
			ID_ouvrir,
			ID_importer,
			ID_exporter,
			ID_generer,
			ID_enregistrer,
			ID_enregistrer_sous,
			ID_enregistrer_copie,
			ID_enregistrer_tous,
			ID_projets_recent,
			ID_preferences,
				ID_preferences_options,
				ID_preferences_lang,
			ID_fermer,

			ID_annuler,
			ID_retablir,
			ID_copier,
			ID_couper,
			ID_coller,

			ID_surface,
			ID_Encombrement,
			ID_Maillage,
			ID_affichage,

			ID_sources,
			ID_recepteurs,
			ID_recepteurs_plan,
			ID_conditions,
			ID_preference,

			ID_parametres_calcul,
			ID_parametres_traitement,

			ID_valeurs,
			ID_cartographie,
			ID_result_recepteurs,
			ID_particules,

			ID_start_simulation,
			ID_next_step_simulation,
			ID_previous_step_simulation,
			ID_stop_simulation,
			ID_delete_simulation,
			ID_pause_simulation,
			ID_tool_flat_inside,
			ID_tool_wireframe_all,
			ID_tool_wireframe_and_flat,
			ID_fold_face,
				ID_flat,
				ID_flat_inside,
				ID_flat_none,
			ID_fold_material,
				ID_ModelMaterial,
				ID_SoundMaterial,
			ID_fold_wireframe,
				ID_wireframe_all,
				ID_wireframe_shape,
				ID_wireframe_none,
			ID_fold_simulation,
				ID_simulation_particle,
				ID_simulation_recepteurss,
			ID_fold_camera,
				ID_camera_firstperson,
				ID_camera_rotate,
			ID_grid_xy,
			ID_grid_xz,
			ID_grid_yz,
			ID_fold_copygl,
				ID_copygl_toclipboard,
				ID_copygl_tofile,
				ID_copygl_tofile_withdim,

			ID_SwitchSelectionMode,
			ID_toolCameraMode,
			ID_toolGetdBLevelMode,

			ID_Mailler,
			ID_ShowHideMailler,
			ID_Tool_ShowHideMailler,
			ID_slPlan,
			ID_xPlan,
			ID_yPlan,
			ID_zPlan,
			ID_ChgModeCamera,
			ID_InitCamera,

			ID_outil_sources,
			ID_outil_recepteurs,
			ID_materiau,

			ID_fenetre,
			ID_clearwindowmanager,
			ID_Dock3Dwindow,
			ID_Float3Dwindow,
			ID_Console,
			ID_Console_SaveToFile,
			ID_Console_ClearLog,
			ID_cascade,
			ID_vertical,
			ID_horizontal,
			ID_minimiser,
			ID_maximiser,
			ID_tout_fermer,
			ID_supprimer,
			ID_proprietees,
			ID_ajouter,
			//ID_changeLanguage,

			ID_Model3d,

			ID_outil_compute_shape,
			ID_outil_resultpostprocess,
			ID_outil_resultpostprocess_substract_recepteursurf,
			//ID_outil_options,

			ID_Help,
			ID_Help_About,
			ID_Help_Licence,
			//ID_Help_Forum,
			ID_Help_Doc,
			ID_Help_Doc_Isimpa_Pdf,
			ID_Help_Doc_Spps_Pdf,
			ID_Help_Web
		};

		enum EL_CATERORY
		{
			EL_CATERORY_NORMAL,
			EL_CATERORY_RADIO,
			EL_CATERORY_CHECK
		};

		struct linked_Menu
		{
			wxMenuItem* menuItem;
			wxString propertyName;
			Element::ELEMENT_TYPE rootType;
			Element::ELEMENT_TYPE pereElType;
			EL_CATERORY propCategory;
		};

		std::vector<linked_Menu> lstLinkedMenu;
		wxString originalPerspective; // Built-in perspective configuration

	public :
		/**
		 * Constructeur de la fenêtre principale
		 * @param lang Objet de contrôle de traduction afin de modifier les paramètres d'internationalisation locaux.
		 */
		MainUiFrame(wxLocale &lang);
		/** Destructeur
		*/
		~MainUiFrame();
		/**
		 *  Affiche la fenêtre qui a été déjà chargé mais fermé par l'utilisateur.
		 * @param window Pointeur vers la fenêtre à ouvrir de nouveau.
		 */
		void ShowFrame(wxWindow* window);
		/**
		 * Gestionnaire de traduction
		 */
		wxLocale &m_locale;
		/**
		 * Si le projet a été modifié demande à l'utilisateur s'il désire enregistrer son projet avant qu'il soit fermé
		 * @return Faux si l'utilisateur veut empecher la fermeture de son projet, vrai si le projet peut être fermé.
		 */
		bool AskUserForSaveModification();
		wxTextCtrl* logWindow;
		virtual void SetLabel(const wxString& label);


		static int AskApplicationLanguage(int defaultLanguage);
	private:
		void OnSaveProject(wxCommandEvent& event);
		void OnSaveConsoleToFile(wxCommandEvent& event);
		void OnClearConsole(wxCommandEvent& event);
		void OnSetDockStatus(wxCommandEvent& event);
		void OnClearWindowManager(wxCommandEvent& event);
		void CopyGlToFile(wxCommandEvent& event);
		void CopyGlToFileWithDim(wxCommandEvent& event);
		void OnModeToolWireFrameAndFlat(wxCommandEvent& event);
		void OnModeToolFlat(wxCommandEvent& event);
		void OnModeToolWireFrame(wxCommandEvent& event);
		void LinkedMenuEvent(wxCommandEvent& event);
		void AttachElementsWithMenuItems();
		void LinkMenuItemWithElement(wxMenuItem* menItem,Element::ELEMENT_TYPE eType,wxString propName);
		void OnClickHistoryFile(wxCommandEvent& event);
		void ExitProgram(wxCloseEvent& event);
		void OnShow(wxShowEvent& event);
		void OnOpenProject(wxCommandEvent& event);
		void OnSaveToProject(wxCommandEvent& event);
		void OnSaveProjectCopy(wxCommandEvent& event);
		void OnModeMaterial(wxCommandEvent& event);
		void NewProject(wxCommandEvent& event);
		void InitCamera(wxCommandEvent& event);
		void OnChangeCameraMode(wxCommandEvent& event);
		void OnMaillage(wxCommandEvent& event);
		void OnShowHideMaillage(wxCommandEvent& event);
		void OnModeFlat(wxCommandEvent& event);
		void OnModeWireFrame(wxCommandEvent& event);
		void OnStartSimulation(wxCommandEvent& event);
		void OnNextStepSimulation(wxCommandEvent& event);
		void OnPreviousStepSimulation(wxCommandEvent& event);
		void OnStopSimulation(wxCommandEvent& event);
		void OnEmptySimulation(wxCommandEvent& event);
		void OnPauseSimulation(wxCommandEvent& event);
		void OnSwitchSelectionMode(wxCommandEvent& event);
		void OnGetdbLevelMode(wxCommandEvent& event);
		void OnCameraToolMode(wxCommandEvent& event);
		void Quit(wxCommandEvent& event);
		void OnOpenFile(wxCommandEvent & event);
		void OnCreateModel(wxCommandEvent & event);
		void OnSaveFile(wxCommandEvent & event);
		void OnScrollChanged(wxScrollEvent& event);
		void OnRadioX(wxCommandEvent& event);
		void OnRadioY(wxCommandEvent& event);
		void OnRadioZ(wxCommandEvent& event);
		void OnComputeShape(wxCommandEvent& event);
		void OnSubstractRecepteurS(wxCommandEvent& event);
		void OnMainEvent(wxCommandEvent& event);
		void OnShowAboutDialog(wxCommandEvent& event);
		void OnLinkWebForum(wxCommandEvent& event);
		void OnLinkWebIsimpa(wxCommandEvent& event);
		void OnLinkWebDoc(wxCommandEvent& event);
		void OnFileLicence(wxCommandEvent& event);
		void OnFileIsimpaDoc(wxCommandEvent& event);
		void OnFileSppsDoc(wxCommandEvent& event);
		void OnChangeLanguage(wxCommandEvent& event);
		void OnShowPreferenceTree(wxCommandEvent& event);
		/**
		 * Menu annuler
		 */
		void OnUndo(wxCommandEvent& event);
		/**
		 * Menu refaire
		 */
		void OnRedo(wxCommandEvent& event);
		//void OnSelectVertex(const long group, const long vertex);
		void ChargerVertex();



		DECLARE_EVENT_TABLE();
};


/**
 *	@brief Surcharge de la classe standard wxLogStderr
 *
 *	Effectue la redirection des messages d'erreur vers le contrôle de log de l'interface
 */
class CustomLog : public wxLog
{
public:
	/**
	 *	@brief A l'événement de génération d'un message, le message est redirigé
	 *
	 *	Pour faire appel à cette méthode n'importe où dans le logiciel, utiliser : wxLogInfo()
	 *	Se référer au document développeur de wxWidgets
	*/
	void DoLog(wxLogLevel level, const wxChar *szString, time_t t)
	{
		wxString message(szString);


		switch ( level ) {
			case wxLOG_FatalError:
				DoLogString(wxString(_("Fatal error: ")) + szString, t,wxRED);
				DoLogString(_("Program aborted"), t,wxRED);
				Flush();
		#ifdef __WXWINCE__
				ExitThread(3);
		#else
				abort();
		#endif
				break;

			case wxLOG_Error:
				DoLogString(wxString(_("Error:")) + szString, t,wxRED);
				break;

			case wxLOG_Warning:
				DoLogString(wxString(_("Warning: ")) + szString, t,wxRED);
				break;

			case wxLOG_Message:
				DoLogString(szString, t,wxBLUE);
				break;
			case wxLOG_Info:
			case wxLOG_Status:
			default:    // log unknown log levels too
				DoLogString(szString, t,wxBLACK);
				break;

			case wxLOG_Trace:
			case wxLOG_Debug:
		#ifdef __WXDEBUG__
				{
					wxString msg = level == wxLOG_Trace ? wxT("Trace: ")
														: wxT("Debug: ");
					msg << szString;
					DoLogString(msg, t,wxGREEN);
				}
		#endif // Debug
				break;
		}

	}
	void DoLogString(const wxChar *szString, time_t WXUNUSED(t),const wxColour* msgColor)
	{
		wxString str;
		TimeStamp(&str);
		projetCourant->AddLogMessage(str + wxString(szString) +   _T("\n"),msgColor);
	}
};


/**
 * @brief Analogue à la méthode main() en C
 * Classe de l'application générale, charge la classe MainUiFrame ( AUI)
 */
class ISimpaApp : public wxApp
{
	private:
		MainUiFrame* frame;
	public:
		/**
		 * Première méthode appelé à l'execution du logiciel
		 *	Dans l'ordre, initialise :
		 *		- Le gestionnaire de langue
		 *		- Le gestionnaire de projet
		 *		- La fenêtre générale de l'application
		 *		- Le gestionnaire de sortie d'erreur ( wxLog)
		 *
		 * @return true Si l'execution du programme c'est terminé normallement
		 */
		bool OnInit()
		{

			/*
			if(ApplicationConfiguration::CONST_WORKINGLIMIT!=0 && ApplicationConfiguration::CONST_WORKINGLIMIT<wxGetLocalTime())
			{
				//on génére une erreur car le logiciel ne doit plus être utilisé
				wxMessageBox(_("Date d'utilisation d'I-SIMPA dépassé :"));
				return false;
			}
			*/
			//Applique le dossier de l'exécutable comme dossier courant
			wxStandardPaths stPath;
			wxFileName fPath=stPath.GetExecutablePath();
			wxString WorkingDir=fPath.GetPath();
			wxSetWorkingDirectory(WorkingDir);

			//Crée le dossier de préférence de l'utilisateur dans le dossier de session
			wxStandardPaths stdpathreader;
			if(!wxDirExists(stdpathreader.GetUserDataDir()))
				wxMkdir(stdpathreader.GetUserDataDir());

			//Charge les gestionnaires de formats d'images
			wxImage::AddHandler(new wxJPEGHandler); //ajoute le support du format jpeg
			wxImage::AddHandler(new wxPNGHandler); //ajoute le support du format png
			wxImage::AddHandler(new wxICOHandler); //ajoute le support du format ico

			//Charge le gestionnaire de language
			wxLocale::AddCatalogLookupPathPrefix(_T("locale")); //Rechercher les fichiers de langue dans le répertoire "locale"
			wxLanguage choosenLanguage=wxLANGUAGE_DEFAULT;
			wxString strConf;
			if(ApplicationConfiguration::GetFileConfig()->Read("interface/language",&strConf))
			{
				choosenLanguage=(wxLanguage)Convertor::ToInt(strConf);
			}else{
				//Il faut demander à l'utilisateur de choisir sa langue
				choosenLanguage=(wxLanguage)MainUiFrame::AskApplicationLanguage(choosenLanguage);
			}
			lang.Init(choosenLanguage, wxLOCALE_LOAD_DEFAULT);
			lang.AddCatalog("internat",wxLANGUAGE_DEFAULT,"");

			// Réserve ou réutilise le cache du projet
			ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath=stdpathreader.GetUserDataDir()+wxFileName::GetPathSeparator()+ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+wxFileName::GetPathSeparator();
			m_checker = new wxSingleInstanceChecker(APPLICATION_NAME);
			wxInt32 instance_count=1;
			if(!wxDir::Exists(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath))
				wxMkdir(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath);
			wxString instancefold=wxString("instance%i")+wxFileName::GetPathSeparator();
			if ( m_checker->IsAnotherRunning() )
			{
				while(wxDir::Exists(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+wxString::Format(instancefold,instance_count)))
					instance_count++;
				ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+wxString::Format(instancefold,instance_count);
			}else{
				bool cleartmpfolder=true;
				wxString instanceFolder=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+wxString::Format(instancefold,instance_count);
				wxArrayString folderDirs;
				ProjectManager::GetAllFolderItems(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath,folderDirs,false);
				if(folderDirs.size()>1)
				{

					wxMessageDialog dialog( NULL, _("Many old sessions of the user interface are available.\nDo you want to retrieve an old session?"),
					wxString(_("Interface "))+APPLICATION_NAME, wxYES_DEFAULT|wxYES_NO|wxICON_INFORMATION);

					wxCommandEvent cmdEvt;
					wxInt32 userResponse=dialog.ShowModal();
					if(userResponse==wxID_YES)
					{
						wxSingleChoiceDialog dialchoice(NULL,_("Please choose the application instance"),wxString(_("Interface "))+APPLICATION_NAME,folderDirs);
						if(dialchoice.ShowModal()==wxID_OK)
						{
							cleartmpfolder=false;
							instanceFolder=folderDirs[dialchoice.GetSelection()]+wxFileName::GetPathSeparator();
						}else{
							cleartmpfolder=true;
						}
					}else if(wxID_NO)
					{
						cleartmpfolder=true;
					}

				}else{
					cleartmpfolder=false;
				}
				//On nettoie le dossier temporaire
				if(cleartmpfolder)
				{
					ProjectManager::ClearFolder(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath);
					wxMkdir(instanceFolder);
				}
				ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath=instanceFolder;
			}
			//Charge le gestionnaire de projet
			projetCourant = new ProjectManager();

			/*
			if(ApplicationConfiguration::CONST_WORKINGLIMIT!=0 && ApplicationConfiguration::CONST_WORKINGLIMIT<wxGetLocalTime())
				projetCourant=NULL;
				*/
			//Charge la feuille principale
			frame = new MainUiFrame(lang);
			SetTopWindow(frame);

			//Surcharge la classe gestionnaire de log
			delete wxLog::SetActiveTarget(new CustomLog());
			/*
			wxLogTextCtrl *logWindow = new wxLogTextCtrl(&(*(frame->logWindow)));
			logWindow->SetVerbose(true);
			delete wxLog::SetActiveTarget(logWindow);
			*/

			//Active la reception des fichiers par drag&drop
			//DragAcceptFiles((HWND)frame->GetHWND(),true); //msw only
			frame->DragAcceptFiles(true);


			//Affiche la feuille principale
			frame->Show();

			doInit=true;

			//Nom de fichier de projet passé en paramètre
			if(this->argc>1)
			{
				wxString filename=this->argv[1];
				projetCourant->Open(filename);
			}


			return true;
		}

		virtual void OnEventLoopEnter( wxEventLoopBase *  loop)
		{
			if(loop && loop->IsMain() && doInit)
			{
				projetCourant->ControlPointerInitialisation();
				doInit=false;
			}
		}
		/**
		 * Dans n'importe quel partie de l'application lors de la pression sur un bouton du clavier
		 * @param kEvent Objet lié à l'événement
		 */
		void OnKeyDown(wxKeyEvent& kEvent)
		{
			if(kEvent.GetKeyCode()==WXK_SHIFT)
			{
				projetCourant->shiftDown=true;
			}else if(kEvent.GetKeyCode()==WXK_CONTROL)
			{
				projetCourant->ctrlDown=true;
			}
			kEvent.Skip(true);
		}

		/**
		 * Dans n'importe quel partie de l'application lors du retrait de la pression sur un bouton du clavier
		 * @param kEvent Objet lié à l'événement
		 */
		void OnKeyUp(wxKeyEvent& kEvent)
		{
			if(kEvent.GetKeyCode()==WXK_SHIFT)
			{
				projetCourant->shiftDown=false;
			}else if(kEvent.GetKeyCode()==WXK_CONTROL)
			{
				projetCourant->ctrlDown=false;
			}
			kEvent.Skip(true);
		}
		/**
		 * Appelé lors du glisser déposé d'un fichier de l'explorateur ( ou équivalent UNIX) vers l'application.
		 * Ouvre le fichier si celui-ci est un fichier de projet
		 */
		void OnFileDrop(wxDropFilesEvent& dropEvent)
		{
			int nbFiles=dropEvent.GetNumberOfFiles();
			if(nbFiles>=1)
			{
				wxString fichier=*dropEvent.GetFiles();
				if(frame->AskUserForSaveModification())
					projetCourant->Open(fichier);
			}
		}
		/**
		 * Destructeur de l'application
		 * Détruit les gestionnaire déclarés via la méthode new
		 */
		~ISimpaApp()
		{
			delete projetCourant;
			this->Dispatch();
			delete m_checker;
		}
		ISimpaApp()
			:frame(NULL),doInit(false)
		{

		}
protected:
	/** Fichier d'internationalisation
	 */
	wxLocale lang;
	wxSingleInstanceChecker* m_checker;
	bool doInit;//True si l'interface peut être initialisé pour son premier lancement
	/**
	 * Déclaration de la table de liaison événements, méthodes
	 */
	DECLARE_EVENT_TABLE();
};

