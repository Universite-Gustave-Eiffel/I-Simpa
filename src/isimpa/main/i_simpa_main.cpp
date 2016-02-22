/* ----------------------------------------------------------------------
* I-SIMPA (http://i-simpa.ifsttar.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) 200 7-2014 - IFSTTAR - Judicael Picaut, Nicolas Fortin
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

#include "i_simpa_main.h"
#include "IHM/customdialog/customdlg.h"
#include <wx/aboutdlg.h>
#include "IHM/languageSelection.hpp"
#include <wx/ffile.h>
#include <wx/evtloop.h>
#include "last_cpp_include.hpp"

BEGIN_EVENT_TABLE(MainUiFrame, wxFrame)
	EVT_MENU(ID_nouveau_projet, MainUiFrame::NewProject)
	EVT_MENU(ID_fermer, MainUiFrame::Quit)
	EVT_CLOSE(MainUiFrame::ExitProgram)
	EVT_SHOW(MainUiFrame::OnShow)
	EVT_MENU(ID_importer, MainUiFrame::OnOpenFile)
	EVT_MENU(ID_exporter, MainUiFrame::OnSaveFile)
	EVT_MENU(ID_generer, MainUiFrame::OnCreateModel)
	EVT_MENU(ID_enregistrer, MainUiFrame::OnSaveProject)
	EVT_MENU(ID_enregistrer_sous, MainUiFrame::OnSaveToProject)
	EVT_MENU(ID_enregistrer_copie, MainUiFrame::OnSaveProjectCopy)
	EVT_MENU(ID_ouvrir, MainUiFrame::OnOpenProject)

	EVT_MENU(ID_preferences_lang, MainUiFrame::OnChangeLanguage)
	EVT_MENU(ID_preferences_options, MainUiFrame::OnShowPreferenceTree)

	EVT_MENU(ID_Dock3Dwindow, MainUiFrame::OnSetDockStatus)
	EVT_MENU(ID_Float3Dwindow, MainUiFrame::OnSetDockStatus)
	EVT_MENU(ID_clearwindowmanager, MainUiFrame::OnClearWindowManager)
	EVT_MENU(ID_Console_SaveToFile, MainUiFrame::OnSaveConsoleToFile)
	EVT_MENU(ID_Console_ClearLog, MainUiFrame::OnClearConsole)
	EVT_MENU(ID_start_simulation, MainUiFrame::OnStartSimulation)
	EVT_MENU(ID_pause_simulation, MainUiFrame::OnPauseSimulation)
	EVT_MENU(ID_stop_simulation, MainUiFrame::OnStopSimulation)
	EVT_MENU(ID_previous_step_simulation, MainUiFrame::OnPreviousStepSimulation)
	EVT_MENU(ID_next_step_simulation, MainUiFrame::OnNextStepSimulation)
	EVT_MENU(ID_delete_simulation, MainUiFrame::OnEmptySimulation)

	EVT_MENU( ID_tool_wireframe_and_flat,MainUiFrame::OnModeToolWireFrameAndFlat)
	EVT_MENU( ID_tool_flat_inside,MainUiFrame::OnModeToolFlat)
	EVT_MENU( ID_tool_wireframe_all,MainUiFrame::OnModeToolWireFrame)

	EVT_MENU(ID_flat, MainUiFrame::OnModeFlat)
	EVT_MENU(ID_flat_inside, MainUiFrame::OnModeFlat)
	EVT_MENU(ID_flat_none, MainUiFrame::OnModeFlat)

	EVT_MENU(ID_ModelMaterial, MainUiFrame::OnModeMaterial)
	EVT_MENU(ID_SoundMaterial, MainUiFrame::OnModeMaterial)

	EVT_MENU(ID_wireframe_all, MainUiFrame::OnModeWireFrame)
	EVT_MENU(ID_wireframe_shape, MainUiFrame::OnModeWireFrame)
	EVT_MENU(ID_wireframe_none, MainUiFrame::OnModeWireFrame)

	EVT_MENU(ID_simulation_particle, MainUiFrame::LinkedMenuEvent)
	EVT_MENU(ID_simulation_recepteurss, MainUiFrame::LinkedMenuEvent)
	EVT_MENU(ID_grid_xy, MainUiFrame::LinkedMenuEvent)
	EVT_MENU(ID_grid_xz, MainUiFrame::LinkedMenuEvent)
	EVT_MENU(ID_grid_yz, MainUiFrame::LinkedMenuEvent)
	EVT_MENU(ID_copygl_tofile, MainUiFrame::CopyGlToFile)
	EVT_MENU(ID_copygl_tofile_withdim, MainUiFrame::CopyGlToFileWithDim)

	EVT_MENU(ID_camera_firstperson, MainUiFrame::OnChangeCameraMode)
	EVT_MENU(ID_camera_rotate, MainUiFrame::OnChangeCameraMode)

	EVT_MENU(ID_toolGetdBLevelMode, MainUiFrame::OnGetdbLevelMode)
	EVT_MENU(ID_SwitchSelectionMode, MainUiFrame::OnSwitchSelectionMode)
	EVT_MENU(ID_toolCameraMode, MainUiFrame::OnCameraToolMode)

	EVT_MENU(ID_Mailler, MainUiFrame::OnMaillage)
	EVT_MENU(ID_ShowHideMailler, MainUiFrame::OnShowHideMaillage)
	EVT_MENU(ID_Tool_ShowHideMailler, MainUiFrame::OnShowHideMaillage)
	EVT_SCROLL_THUMBRELEASE(MainUiFrame::OnScrollChanged)
	EVT_SCROLL(MainUiFrame::OnScrollChanged)
	EVT_RADIOBUTTON(ID_xPlan,MainUiFrame::OnRadioX)
	EVT_RADIOBUTTON(ID_yPlan,MainUiFrame::OnRadioY)
	EVT_RADIOBUTTON(ID_zPlan,MainUiFrame::OnRadioZ)
	EVT_MENU(ID_ChgModeCamera, MainUiFrame::OnChangeCameraMode)


	EVT_MENU(ID_InitCamera, MainUiFrame::InitCamera)
	EVT_MENU(ID_outil_compute_shape, MainUiFrame::OnComputeShape)
	EVT_MENU(ID_outil_resultpostprocess_substract_recepteursurf,  MainUiFrame::OnSubstractRecepteurS)
	//EVT_MENU(ID_outil_options, MainUiFrame::OnShowPreferenceTree)

	EVT_MENU_RANGE(ID_History, ID_History+MAX_HISTORY_SHOW-1, MainUiFrame::OnClickHistoryFile)
	EVT_COMMAND(-1,wxEVT_PSPS_MAIN_EVENT,MainUiFrame::OnMainEvent)

	EVT_MENU(ID_annuler, MainUiFrame::OnUndo)
	EVT_MENU(ID_retablir, MainUiFrame::OnRedo)
	EVT_MENU(ID_Help_About, MainUiFrame::OnShowAboutDialog)
	//EVT_MENU(ID_Help_Forum, MainUiFrame::OnLinkWebForum)
	EVT_MENU(ID_Help_Web, MainUiFrame::OnLinkWebIsimpa)
	EVT_MENU(ID_Help_Doc, MainUiFrame::OnLinkWebDoc)
	EVT_MENU(ID_Help_Licence, MainUiFrame::OnFileLicence)
	EVT_MENU(ID_Help_Doc_Isimpa_Pdf, MainUiFrame::OnFileIsimpaDoc)
	EVT_MENU(ID_Help_Doc_Spps_Pdf, MainUiFrame::OnFileSppsDoc)
	//VT_MENU(ID_changeLanguage, MainUiFrame::OnChangeLanguage)
END_EVENT_TABLE()


void OnSelectVertex PARAM_BOUND_ON_SELECT_FACES
{
	projetCourant->OnSelectVertex(facelst,dragging);
}

void OnSelectPosition(const vec3 position)
{
	projetCourant->OnSelectPosition(position);
}


void OnSceneElementEvent(wxCommandEvent& eventElement)
{
	projetCourant->ElementEvent(eventElement,ProjectManager::EVENT_CTRL_SCENE);
}
void OnReportElementEvent(wxCommandEvent& eventElement)
{
	projetCourant->ElementEvent(eventElement,ProjectManager::EVENT_CTRL_REPORT);
}
void OnCoreElementEvent(wxCommandEvent& eventElement)
{
	projetCourant->ElementEvent(eventElement,ProjectManager::EVENT_CTRL_CORE);
}
void OnUserConfigElementEvent(wxCommandEvent& eventElement)
{
	projetCourant->ElementEvent(eventElement,ProjectManager::EVENT_CTRL_USERCONFIG);
}


wxString GetLicenseText()
{
	wxFFile licenseFile("license.txt");
	wxString fileContent;
	licenseFile.ReadAll(&fileContent);
	return fileContent;
}

MainUiFrame::MainUiFrame(wxLocale &lang) : wxFrame(NULL, -1, _("Interface ")+APPLICATION_NAME,
									wxDefaultPosition, wxSize(1024,768),
									wxDEFAULT_FRAME_STYLE),m_locale(lang)
{
	saveManagerConfig=true;
	CustomLog logClass;
	// Liaison du manager aui à la fenêtre
	//m_mgr.SetFrame(this);
	m_mgr.SetManagedWindow(this);
	wxIcon ico_ui;
	if(ico_ui.LoadFile("./isimpa.ico",wxBITMAP_TYPE_ICO))
		SetIcon(ico_ui);
	else
		wxLogError(_("Can not read the I-SIMPA icon"));
	//Creation des menus


	wxMenuBar* mb = new wxMenuBar;

	wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_nouveau_projet, _("New project\tCtrl+N"));
	file_menu->Append(ID_ouvrir, _("Open project\tCtrl+O"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_importer, _(wxT("Import new scene")));
	file_menu->Append(ID_exporter, _("Export scene"));
	file_menu->Append(ID_generer, _("New scene"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_enregistrer, _("Save project\tCtrl+S"));
	file_menu->Append(ID_enregistrer_sous, _("Save project as ..."));
	file_menu->Append(ID_enregistrer_copie, _("Save copy of project"));
	file_menu->AppendSeparator();
	wxMenu* recent_projet_menu = new wxMenu;
	file_menu->Append(ID_projets_recent, _("Recent projects"),recent_projet_menu);

	//Utilisation de la classe projets récent
	fileHistory = new wxFileHistory(MAX_HISTORY_SHOW,ID_History);
	fileHistory->UseMenu(recent_projet_menu);
	fileHistory->Load(*ApplicationConfiguration::GetFileConfig());
	file_menu->AppendSeparator();

	wxMenu* file_preferences_menu = new wxMenu;
	file_preferences_menu->Append(ID_preferences_options, _("Options"));
	file_preferences_menu->Append(ID_preferences_lang, _("Language"));
	file_menu->Append(ID_preferences, _("Settings"),file_preferences_menu);

	file_menu->AppendSeparator();
	file_menu->Append(ID_fermer, _("Exit"));

	edit_menu = new wxMenu;
	edit_menu->Append(ID_annuler, _("Undo\tCtrl+Z"))->Enable(false);
	edit_menu->Append(ID_retablir, _("Redo\tCtrl+Y"))->Enable(false);

	wxMenu* simulation_menu = new wxMenu;
	simulation_menu->Append(ID_start_simulation, _("Play"));
	simulation_menu->Append(ID_pause_simulation, _("Pause"));
	simulation_menu->Append(ID_stop_simulation, _("Stop"));
	simulation_menu->AppendSeparator();
	simulation_menu->Append(ID_Mailler, _("Meshing"));

	wxMenu* outils_menu = new wxMenu;
	wxMenu* outils_menu_postprocess = new wxMenu;
	outils_menu_postprocess->Append(ID_outil_resultpostprocess_substract_recepteursurf, _("Compare surface receivers"));
	outils_menu->Append(ID_outil_resultpostprocess,_("File results process"),outils_menu_postprocess);

	//outils_menu->AppendSeparator();
	//outils_menu->Append(ID_outil_options, _("Options..."));


	affichage_menu = new wxMenu;

	affichage_face_menu = new wxMenu;
	affichage_face_menu->Append(ID_flat, _("Outside"),"Outside",wxITEM_RADIO);
	affichage_face_menu->Append(ID_flat_inside, _("Inside"),"Inside",wxITEM_RADIO);
	affichage_face_menu->Append(ID_flat_none, _("None"),"None",wxITEM_RADIO);
	affichage_face_menu->Check(ID_flat_inside,true);
	affichage_menu->Append(ID_fold_face, _("Faces"),affichage_face_menu);


	wxMenu* affichage_material_menu = new wxMenu;
	affichage_material_menu->Append(ID_ModelMaterial, _("Original"),"Original",wxITEM_RADIO);
	affichage_material_menu->Append(ID_SoundMaterial, _("Material"),"Material",wxITEM_RADIO);
	affichage_menu->Append(ID_fold_material, _("Material color"),affichage_material_menu);
	affichage_material_menu->Check(ID_ModelMaterial,true);

	affichage_ligne_menu = new wxMenu;
	affichage_ligne_menu->Append(ID_outil_compute_shape, _("Contour lines calculation"));
	affichage_ligne_menu->Append(ID_wireframe_all, _("All"),"All",wxITEM_RADIO);
	affichage_ligne_menu->Append(ID_wireframe_shape, _("Contour"),"Contour",wxITEM_RADIO);
	affichage_ligne_menu->Append(ID_wireframe_none, _("None"),"None",wxITEM_RADIO);
	affichage_ligne_menu->Check(ID_wireframe_none,true);
	affichage_menu->Append(ID_fold_wireframe, _("Lines"),affichage_ligne_menu);



/*
	affichage_menu->AppendSeparator();

	wxMenu* affichage_simulation_menu = new wxMenu;

	LinkMenuItemWithElement(affichage_simulation_menu->Append(ID_simulation_particle, _("Rendu des particules"),"",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_PARTICULES,
		"showparticle");
	LinkMenuItemWithElement(affichage_simulation_menu->Append(ID_simulation_recepteurss, _("Rendu des récepteurs surfaciques"),"",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_PARTICULES,
		"showrecepteurss");
	affichage_menu->Append(ID_fold_simulation, _("Simulation"),affichage_simulation_menu);
*/


	affichage_menu->AppendSeparator();

	affichage_menu->Append(ID_ShowHideMailler, _("Hide meshing"),"Hide meshing",wxITEM_CHECK);

	affichage_menu->AppendSeparator();

	affichage_menu->Append(ID_InitCamera, _("Reinitialize camera"));
	affichage_camera_menu = new wxMenu;
	affichage_camera_menu->Append(ID_camera_firstperson, _("First person"),"First person",wxITEM_RADIO);
	affichage_camera_menu->Append(ID_camera_rotate, _("Rotation / Zoom"),"Rotation / Zoom",wxITEM_RADIO);
	affichage_camera_menu->Check(ID_camera_rotate,true);
	affichage_menu->Append(ID_fold_camera, _("Camera"),affichage_camera_menu);

	affichage_menu->AppendSeparator();

	//Ces éléments du menu interagissent avec l'etat d'élément du projet
	LinkMenuItemWithElement(affichage_menu->Append(ID_grid_xy, _("XY Grid"),"XY Grid",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,
		"showgxy");
	LinkMenuItemWithElement(affichage_menu->Append(ID_grid_xz, _("XZ Grid"),"XZ Grid",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,
		"showgxz");
	LinkMenuItemWithElement(affichage_menu->Append(ID_grid_yz, _("YZ Grid"),"YZ Grid",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,
		"showgyz");

	affichage_menu->AppendSeparator();

	// Copie de la fenêtre openGl dans un fichier ou dans le presse papier
	wxMenu* affichage_copygl = new wxMenu;
	//affichage_copygl->Append(ID_copygl_toclipboard, _("Vers le presse papier"));
	affichage_copygl->Append(ID_copygl_tofile, _("To file"));
	affichage_copygl->Append(ID_copygl_tofile_withdim, _("To a file with a specified resolution"));
	affichage_menu->Append(ID_fold_copygl, _("Copy 3D View"),affichage_copygl);
	//affichage_menu->AppendSeparator();
	//affichage_menu->Append(ID_changeLanguage,_("Changer la langue du logiciel"));



	wxMenu* fenetre_menu = new wxMenu;
	fenetre_menu->Append(ID_clearwindowmanager, _("Reinitialize interface"));
	wxMenu* window3D_menu_console = new wxMenu;
	window3D_menu_console->Append(ID_Dock3Dwindow, _("Set 3D View as main window"));
	window3D_menu_console->Append(ID_Float3Dwindow, _("Floating 3D View"));
	fenetre_menu->AppendSubMenu(window3D_menu_console,_("3D View"));
	wxMenu* fenetre_menu_console = new wxMenu;
	fenetre_menu_console->Append(ID_Console_SaveToFile, _("Export to file"));
	fenetre_menu_console->AppendSeparator();
	fenetre_menu_console->Append(ID_Console_ClearLog, _("Delete windows"));
	fenetre_menu->AppendSeparator();
	fenetre_menu->AppendSubMenu(fenetre_menu_console,_("Console"));


	wxMenu* aide_menu = new wxMenu;
	//aide_menu->Append(ID_Help, _("Aide"));
	aide_menu->Append(ID_Help_Web, _("Website"));
	//aide_menu->Append(ID_Help_Forum, _("Online forums"));
	aide_menu->Append(ID_Help_Doc, _("Online documentation"));
	aide_menu->AppendSeparator();
	aide_menu->Append(ID_Help_Doc_Isimpa_Pdf, _("I-Simpa documentation (PDF in French)"));
	aide_menu->Append(ID_Help_Doc_Spps_Pdf, _("SPPS documentation (PDF in French)"));
	aide_menu->AppendSeparator();
	aide_menu->Append(ID_Help_Licence, _("License"));
	aide_menu->Append(ID_Help_About, _("About ")+APPLICATION_NAME);



	mb->Append(file_menu, _("File"));
	mb->Append(edit_menu, _("Edition"));
	mb->Append(simulation_menu, _("Simulation"));
	mb->Append(outils_menu, _("Tools"));
	mb->Append(affichage_menu, _("View"));
	mb->Append(fenetre_menu, _("Window"));
	mb->Append(aide_menu, _("Help"));

    SetMenuBar(mb);

	wxSize client_size = GetClientSize();
	// Creation du contrôle texte d'historique des opérations
	shellNotebook= new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );
	shellNotebook->SetName("console");

	logWindow = new wxTextCtrl(this, -1, "",
							wxDefaultPosition, wxSize(200,150),
							wxTE_RICH | wxNO_BORDER | wxTE_MULTILINE | wxTE_READONLY);

    shellNotebook->AddPage(logWindow, _("Messages") , true);
	#ifdef USE_PYTHON
		PyConsole* consolePython=new PyConsole(this);
		shellNotebook->AddPage(consolePython, _("Python(TM)") , false);
	#else
		PyConsole* consolePython=NULL;
	#endif


	//Creation du panneau qui contient 3 arbres

	ctrlNotebook = new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );


	// Creation des 3 arbres de gestion du projet
	this->treeCore = new uiTreeCtrl(ctrlNotebook.get(), -1, wxPoint(0,0), wxSize(-1,250),wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS  | wxTR_HIDE_ROOT); //wxTR_MULTIPLE |
	this->treeResult = new uiTreeCtrl(ctrlNotebook.get(), -1, wxPoint(0,0), wxSize(-1,250), wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT );
	this->treeScene = new uiTreeCtrl(ctrlNotebook.get(), -1, wxPoint(0,0), wxSize(-1,250), wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT ); //
	this->treeUserPref = new uiTreeCtrl(this, -1, wxPoint(0,0), wxSize(-1,250), wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT ); //

	treeCore->BindElementEvent(&OnCoreElementEvent);
	treeResult->BindElementEvent(&OnReportElementEvent);
	treeScene->BindElementEvent(&OnSceneElementEvent);
	treeUserPref->BindElementEvent(&OnUserConfigElementEvent);


    ctrlNotebook->AddPage(this->treeScene, _("Scene") , false);
    ctrlNotebook->AddPage(this->treeCore, _("Calculation") , false);
    ctrlNotebook->AddPage(this->treeResult, _("Results") , false);



	// Ajout des panneaux de l'interface
	m_mgr.AddPane(ctrlNotebook.get(), wxAuiPaneInfo().
					Name("projet").Caption(_("Project")).
					Left().Position(0).CloseButton(false));
	m_mgr.AddPane(shellNotebook.get(), wxAuiPaneInfo().
					Name("console").Caption(_("Console")).
					Bottom().Position(0).CloseButton(false));

	m_mgr.AddPane(treeUserPref.get(), wxAuiPaneInfo().
				Name("userpref").Caption(_("Interface options")).Hide()
				);
	int gl_attrib[20] = { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
	WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
	WX_GL_LEVEL , 1};

	GlFrame = new OpenGlViewer(this, -1, wxPoint(0,25), wxSize(-1,-1),
												0, _("Main window") ); //, gl_attrib

	toolbarGl = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);

	slPlan = new wxSlider(toolbarGl, ID_slPlan, 0, 0, 1000,wxDefaultPosition,wxSize(100,-1));
	xPlan = new wxRadioButton(toolbarGl,ID_xPlan,"x",wxDefaultPosition,wxDefaultSize,wxRB_GROUP );
	yPlan = new wxRadioButton(toolbarGl,ID_yPlan,"y");
	zPlan = new wxRadioButton(toolbarGl,ID_zPlan,"z");

	wxToolBar* simulation= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	wxToolBar* visualisation= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	wxToolBar* tbProjet= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER );
	mousetool= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);

	mousetool->SetToolBitmapSize(wxSize(16,20));
	toolbarGl->SetToolBitmapSize(wxSize(16,20));
	simulation->SetToolBitmapSize(wxSize(16,20));
	visualisation->SetToolBitmapSize(wxSize(16,20));
	tbProjet->SetToolBitmapSize(wxSize(16,20));

	wxString ressourceFolder=ApplicationConfiguration::CONST_RESOURCE_FOLDER;

	tbProjet->AddTool(ID_nouveau_projet, _("New project"), wxImage(ressourceFolder+"/Bitmaps/toolbar_newproject.png", wxBITMAP_TYPE_PNG), _("New project"));
	tbProjet->AddTool(ID_ouvrir, _("Open project"), wxImage(ressourceFolder+"/Bitmaps/toolbar_openproject.png", wxBITMAP_TYPE_PNG), _("Open project"));
	tbProjet->AddSeparator();
	tbProjet->AddTool(ID_enregistrer, _("Save project"), wxImage(ressourceFolder+"/Bitmaps/toolbar_saveproject.png", wxBITMAP_TYPE_PNG), _("Save project"));
	tbProjet->AddTool(ID_enregistrer_sous, _("Save project as ..."), wxImage(ressourceFolder+"/Bitmaps/toolbar_saveprojectas.png", wxBITMAP_TYPE_PNG), _("Save project as ..."));

	simulation->AddTool(ID_previous_step_simulation, _("Previous time step"), wxImage(ressourceFolder+"/Bitmaps/toolbar_animation_previousstep.png", wxBITMAP_TYPE_PNG), _("Previous time step"));
	simulation->AddTool(ID_start_simulation, _("Play"), wxImage(ressourceFolder+"/Bitmaps/toolbar_animation_start.png", wxBITMAP_TYPE_PNG), _("Play"));
	simulation->AddTool(ID_pause_simulation, _("Pause"), wxImage(ressourceFolder+"/Bitmaps/toolbar_animation_pause.png", wxBITMAP_TYPE_PNG), _("Pause"));
	simulation->AddTool(ID_next_step_simulation, _("Next time step"), wxImage(ressourceFolder+"/Bitmaps/toolbar_animation_nextstep.png", wxBITMAP_TYPE_PNG), _("Next time step"));
	simulation->AddTool(ID_stop_simulation, _("Stop"), wxImage(ressourceFolder+"/Bitmaps/toolbar_animation_stop.png", wxBITMAP_TYPE_PNG), _("Stop"));
	simulation->AddTool(ID_delete_simulation, _("Delete the simulation"), wxImage(ressourceFolder+"/Bitmaps/toolbar_hideallanimations.png", wxBITMAP_TYPE_PNG), _("Delete the simulation"));

	visualisation->AddTool(ID_tool_flat_inside, _("Face view"), wxImage(ressourceFolder+"/Bitmaps/toolbar_showfaceonly.png", wxBITMAP_TYPE_PNG), _("Face view"));
	visualisation->AddTool(ID_tool_wireframe_all, _("Wireframe view"), wxImage(ressourceFolder+"/Bitmaps/toolbar_showsidesonly.png", wxBITMAP_TYPE_PNG), _("Wireframe view"));
	visualisation->AddTool(ID_tool_wireframe_and_flat, _("Combined view"), wxImage(ressourceFolder+"/Bitmaps/toolbar_showfacewithsides.png", wxBITMAP_TYPE_PNG), _("Combined view"));


	mousetool->AddRadioTool(ID_toolCameraMode, _("Camera mode"), wxImage(ressourceFolder+"/Bitmaps/toolbar_tool_camera.png", wxBITMAP_TYPE_PNG), wxNullBitmap, _("Camera mode"));
	mousetool->AddRadioTool(ID_SwitchSelectionMode, _("Surface selection mode"), wxImage(ressourceFolder+"/Bitmaps/toolbar_tool_faceselection.png", wxBITMAP_TYPE_PNG), wxNullBitmap, _("Surface selection mode"));
	mousetool->AddRadioTool(ID_toolGetdBLevelMode, _("Extract sound level from 3D view"), wxImage(ressourceFolder+"/Bitmaps/toolbar_tool_extractlvl.png", wxBITMAP_TYPE_PNG), wxNullBitmap, _("Extract sound level from 3D view"));

	visualisation->AddSeparator();
	visualisation->AddTool(ID_InitCamera, _("Reinitialize camera"), wxImage((ressourceFolder+"/Bitmaps/toolbar_camera_movetoorigin.png"), wxBITMAP_TYPE_PNG), _("Reinitialize camera"));
	visualisation->AddTool(ID_camera_rotate, _("Orbit camera"), wxImage((ressourceFolder+"/Bitmaps/toolbar_camera_rotation.png"), wxBITMAP_TYPE_PNG), _("Orbit camera"));
	visualisation->AddTool(ID_camera_firstperson, _("First person camera"), wxImage((ressourceFolder+"/Bitmaps/toolbar_camera_firstperson.png"), wxBITMAP_TYPE_PNG), _("First person camera"));

	toolbarGl->AddTool(ID_Mailler, _("Meshing"), wxImage((ressourceFolder+"/Bitmaps/toolbar_run_tetmesh.png"), wxBITMAP_TYPE_PNG), _("Meshing"));
	toolbarGl->AddTool(ID_Tool_ShowHideMailler, _("Show / Hide meshing"), wxImage((ressourceFolder+"/Bitmaps/toolbar_show_hide_tetmesh.png"), wxBITMAP_TYPE_PNG), _("Show / Hide meshing"));
	toolbarGl->AddSeparator();
	toolbarGl->AddControl(xPlan);
	toolbarGl->AddControl(yPlan);
	toolbarGl->AddControl(zPlan);
	toolbarGl->AddControl(slPlan);

	toolbarGl->Realize();
	simulation->Realize();
	visualisation->Realize();
	tbProjet->Realize();
	mousetool->Realize();
/*
	wxAuiNotebook* glFrameNotebook = new wxAuiNotebook(this, wxID_ANY,
									wxDefaultPosition,
									wxDefaultSize,
                                    wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );
	glFrameNotebook->AddPage(&(*(GlFrame)), _("Vue 3D") , true);
*/

	m_mgr.AddPane(&(*(GlFrame)), wxAuiPaneInfo().Name("3Dview").Caption(_("Main window")).
					CenterPane().Show());
    m_mgr.AddPane(tbProjet, wxAuiPaneInfo().
                  Name(wxT("projetTb")).Caption(_("Project toolbar")).
                  ToolbarPane().Position(0).Top().CloseButton(false));



    m_mgr.AddPane(visualisation, wxAuiPaneInfo().
                  Name(wxT("visualisationTb")).Caption(_("Camera toolbar")).
                  ToolbarPane().Top().Position(1).CloseButton(false));


    m_mgr.AddPane(toolbarGl, wxAuiPaneInfo().
                  Name(wxT("maillageTb")).Caption(_("Meshing toolbar")).
                  ToolbarPane().Top().Position(2).CloseButton(false));

    m_mgr.AddPane(simulation, wxAuiPaneInfo().
                  Name(wxT("simulationTb")).Caption(_("Animation toolbar")).
                  ToolbarPane().Position(3).Top().CloseButton(false));

    m_mgr.AddPane(mousetool, wxAuiPaneInfo().
                  Name(wxT("toolTb")).Caption(_("Pointer toolbar")).
                  ToolbarPane().Position(4).Top().CloseButton(false));
	//Feuille de propriétés

	propertiesGrid = new MainPropGrid(this,-1,wxDefaultPosition,wxSize(200,300),262144,_("Properties"));


	m_mgr.AddPane(&(*(propertiesGrid)), wxAuiPaneInfo().
					Name(_("Properties")).Caption(_("Properties")).
					Bottom().Position(-1).BestSize(350,400).CloseButton(false));


	//Charge l'arrangement des fenêtres en fonction de l'utilisateur
	wxString strConf;

	originalPerspective=m_mgr.SavePerspective();
	if ( ApplicationConfiguration::GetFileConfig()->Read("interface/generalPerspective", &strConf) )
	{
		if(strConf!="")
			m_mgr.LoadPerspective(strConf);
	}

	// tell the manager to "commit" all the changes just made
	m_mgr.Update();


	// Attache la méthode à l'événement de sélection d'un vertex dans la fenêtre 3D
	GlFrame->bindSelectionEvent(&OnSelectVertex);
	GlFrame->bindSelectionPositionEvent(&OnSelectPosition);

	//Lie les principaux contrôles au gestionnaire de projet
	projetCourant->SetControlPointer(logWindow,this->treeCore,this->treeResult,this->treeScene,this->treeUserPref.get(),toolbarGl,this,GlFrame.get(),propertiesGrid.get(),&m_mgr,consolePython);

	//Lie les éléments du menu avec les éléments du projet en cours
	AttachElementsWithMenuItems();
}

void MainUiFrame::OnClearConsole(wxCommandEvent& event)
{
	projetCourant->OnClearConsole();
}

void MainUiFrame::OnSaveConsoleToFile(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog( this, _("Save console"), "Text file (*.txt)|*.txt", "Text file (*.txt)|*.txt", _("Text file (*.txt)|*.txt"),
														wxFD_SAVE , wxDefaultPosition);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxString filename(wxString(saveFileDialog.GetPath()));
		projetCourant->OnSaveConsoleToFile(filename);
	}
}
void MainUiFrame::OnClickHistoryFile(wxCommandEvent& event)
{
	wxString f(fileHistory->GetHistoryFile(event.GetId() - ID_History));
    if (!f.empty() && AskUserForSaveModification())
	{
			projetCourant->Open(f);
			AttachElementsWithMenuItems();
	}

}

void MainUiFrame::LinkMenuItemWithElement(wxMenuItem* menItem,Element::ELEMENT_TYPE eType,wxString propName)
{
	linked_Menu linkRow;
	linkRow.menuItem=menItem;
	linkRow.pereElType=eType;
	linkRow.propCategory=EL_CATERORY_CHECK;
	linkRow.propertyName=propName;
	linkRow.rootType=Element::ELEMENT_TYPE_SCENE_ROOT;

	lstLinkedMenu.push_back(linkRow);
}
void MainUiFrame::AttachElementsWithMenuItems()
{
	Element* rootcfg=ApplicationConfiguration::GetRootScene();
	if(rootcfg)
	{
		for(int i=0;i<lstLinkedMenu.size();i++)
		{
			Element* elementFound=rootcfg->GetElementByType(lstLinkedMenu[i].pereElType);
			if(elementFound)
			{
				elementFound->LinkMenuWithPropBool(lstLinkedMenu[i].propertyName,lstLinkedMenu[i].menuItem);
			}
		}
	}
}

void MainUiFrame::LinkedMenuEvent(wxCommandEvent& event) //Evenements de click sur un menu lié a un élément
{
	Element* rtScene=ApplicationConfiguration::GetRootScene();
	if(!rtScene)
		return;
	int idEvent=event.GetId();
	for(int i=0;i<lstLinkedMenu.size();i++)
	{
		if(idEvent==lstLinkedMenu[i].menuItem->GetId())
		{

			Element* pereItem=rtScene->GetElementByType(lstLinkedMenu[i].pereElType);
			if(pereItem!=NULL)
			{
				pereItem->UpdateBoolConfig(lstLinkedMenu[i].propertyName,lstLinkedMenu[i].menuItem->IsChecked());
			}
			propertiesGrid->RefreshValues(); //Raffraichie les valeurs de la grille de propriétés
			break;
		}
	}
}
void MainUiFrame::ShowFrame(wxWindow* window)
{
	wxAuiPaneInfo paneToShow=this->m_mgr.GetPane(window);
	if(paneToShow.IsOk())
		paneToShow.Show();
}

void MainUiFrame::InitCamera(wxCommandEvent& event)
{
	this->GlFrame->ResetCamera();
}

void MainUiFrame::OnSetDockStatus(wxCommandEvent& event)
{
	wxAuiPaneInfo& paneInfo=this->m_mgr.GetPane(GlFrame.get());
	if(paneInfo.IsOk())
	{
		if(event.GetId()==ID_Dock3Dwindow)
		{
			paneInfo.CentrePane();
		}else if(event.GetId()==ID_Float3Dwindow)
		{
			paneInfo.Float();
			paneInfo.MaximizeButton();
			paneInfo.Dockable();
			paneInfo.CaptionVisible();
			paneInfo.Floatable( );
			paneInfo.MinSize(wxSize(320,240));
		}
		this->m_mgr.Update();
	}
}
void MainUiFrame::OnClearWindowManager(wxCommandEvent& event)
{
	saveManagerConfig=false;
	m_mgr.LoadPerspective(originalPerspective);
}
void MainUiFrame::CopyGlToFileWithDim(wxCommandEvent& event)
{
	projetCourant->CopyGlToFileWithDim();
}
void MainUiFrame::CopyGlToFile(wxCommandEvent& event)
{
	projetCourant->CopyGlToFile();
}

void MainUiFrame::NewProject(wxCommandEvent& event)
{
	if(AskUserForSaveModification())
	{
		projetCourant->NewProject(); //chargement d'un nouveau projet
		//Lie les éléments du menu avec les éléments du projet en cours
		AttachElementsWithMenuItems();	 // mise à jour des contrôles
	}
}




void MainUiFrame::OnRadioX(wxCommandEvent& event)
{
	if(event.GetEventType()==wxEVT_COMMAND_RADIOBUTTON_SELECTED )
	{
		projetCourant->OnChangePlan(0);
	}
}

void MainUiFrame::OnRadioY(wxCommandEvent& event)
{
	if(event.GetEventType()==wxEVT_COMMAND_RADIOBUTTON_SELECTED )
	{
		projetCourant->OnChangePlan(1);
	}
}

void MainUiFrame::OnMainEvent(wxCommandEvent& event)
{
	ApplicationConfiguration::MAIN_EVENT evtType=(ApplicationConfiguration::MAIN_EVENT)event.GetInt();
	if(evtType==ApplicationConfiguration::MAIN_EVENT_UNDO_REDO_UPDATE)
	{
		if(projetCourant)
		{
			edit_menu->Enable(ID_annuler,projetCourant->CanUndo());
			edit_menu->Enable(ID_retablir,projetCourant->CanRedo());
		}
	}else if(evtType==ApplicationConfiguration::MAIN_EVENT_SWITCH_TO_CAMERA_TOOL){
		//wxPostEvent(this,wxCommandEvent(wxEVT_COMMAND_RADIOBOX_SELECTED,ID_toolCameraMode));
		//mousetool->EnableTool(ID_toolCameraMode,true);
		mousetool->ToggleTool(ID_toolCameraMode,true);
        wxCommandEvent nevt(wxEVT_COMMAND_RADIOBOX_SELECTED,ID_toolCameraMode);
		this->OnCameraToolMode(nevt);
	}else{
		projetCourant->OnMainEvent(evtType,event);
	}
}
void MainUiFrame::OnSubstractRecepteurS(wxCommandEvent& event)
{
	projetCourant->OnMenuSubstractRecepteurSurf();
}
void MainUiFrame::OnComputeShape(wxCommandEvent& event)
{
	projetCourant->DoShapeComputation(true);
}
void MainUiFrame::OnRadioZ(wxCommandEvent& event)
{
	if(event.GetEventType()==wxEVT_COMMAND_RADIOBUTTON_SELECTED )
	{
		projetCourant->OnChangePlan(2);
	}
}

void MainUiFrame::OnChangeCameraMode(wxCommandEvent& event)
{
	if(event.GetId()==ID_camera_firstperson)
		projetCourant->OnChangeCameraMode(event,OpenGlViewer::CAMERA_FIRST_PERSON);
	else if(event.GetId()==ID_camera_rotate)
		projetCourant->OnChangeCameraMode(event,OpenGlViewer::CAMERA_ROTATION);
	else
		projetCourant->OnChangeCameraMode(event,GlFrame->NextCameraMode());
}

void MainUiFrame::OnModeToolWireFrameAndFlat(wxCommandEvent& event)
{
	affichage_face_menu->Check(ID_flat_inside,true);
	affichage_ligne_menu->Check(ID_wireframe_shape,true);
	projetCourant->OnModeFlatInside(event);
	projetCourant->OnModeWireFrameShape(event);
}

void MainUiFrame::OnModeToolFlat(wxCommandEvent& event)
{
	affichage_face_menu->Check(ID_flat_inside,true);
	affichage_ligne_menu->Check(ID_wireframe_none,true);
	projetCourant->OnModeFlatInside(event);
	projetCourant->OnModeWireFrameNone(event);
}
void MainUiFrame::OnModeToolWireFrame(wxCommandEvent& event)
{
	affichage_face_menu->Check(ID_flat_none,true);
	affichage_ligne_menu->Check(ID_wireframe_shape,true);
	projetCourant->OnModeFlatNone(event);
	projetCourant->OnModeWireFrameShape(event);
}

void MainUiFrame::OnModeMaterial(wxCommandEvent& event)
{
	if(event.GetId()==ID_ModelMaterial)
		projetCourant->OnModeOriginalPaint(event);
	else if(event.GetId()==ID_SoundMaterial)
		projetCourant->OnModeMaterialPaint(event);
}

void MainUiFrame::OnModeFlat(wxCommandEvent& event)
{
	if(event.GetId()==ID_flat)
		projetCourant->OnModeFlatOutside(event);
	else if(event.GetId()==ID_flat_inside)
		projetCourant->OnModeFlatInside(event);
	else if(event.GetId()==ID_flat_none)
		projetCourant->OnModeFlatNone(event);
}

void MainUiFrame::OnModeWireFrame(wxCommandEvent& event)
{
	if(event.GetId()==ID_wireframe_all)
		projetCourant->OnModeWireFrameFull(event);
	else if(event.GetId()==ID_wireframe_shape)
		projetCourant->OnModeWireFrameShape(event);
	else if(event.GetId()==ID_wireframe_none)
		projetCourant->OnModeWireFrameNone(event);
}

void MainUiFrame::OnMaillage(wxCommandEvent& event)
{
	projetCourant->OnMaillage(event);
}

void MainUiFrame::OnShowHideMaillage(wxCommandEvent& event)
{
	projetCourant->OnShowHideMaillage(event);
	if(event.GetId()==ID_Tool_ShowHideMailler)
		affichage_menu->Check(ID_ShowHideMailler,!affichage_menu->IsChecked(ID_ShowHideMailler));
}

void MainUiFrame::OnCameraToolMode(wxCommandEvent& event)
{
	projetCourant->OnCameraTool(event);
}

void MainUiFrame::OnUndo(wxCommandEvent& event)
{
	projetCourant->OnMenuUndo();
	edit_menu->Enable(ID_annuler,projetCourant->CanUndo());
	edit_menu->Enable(ID_retablir,projetCourant->CanRedo());
}
void MainUiFrame::OnRedo(wxCommandEvent& event)
{
	projetCourant->OnMenuRedo();
	edit_menu->Enable(ID_annuler,projetCourant->CanUndo());
	edit_menu->Enable(ID_retablir,projetCourant->CanRedo());
}
void MainUiFrame::OnGetdbLevelMode(wxCommandEvent& event)
{
	projetCourant->OnGetdbLevelMode(event);
}
void MainUiFrame::OnSwitchSelectionMode(wxCommandEvent& event)
{
	projetCourant->OnSwitchSelectionMode(event);
}


void MainUiFrame::OnNextStepSimulation(wxCommandEvent& event)
{
	projetCourant->OnNextStepSimulation(event);
}
void MainUiFrame::OnPreviousStepSimulation(wxCommandEvent& event)
{
	projetCourant->OnPreviousStepSimulation(event);
}

void MainUiFrame::OnStartSimulation(wxCommandEvent& event)
{
	projetCourant->OnStartSimulation(event);
}

void MainUiFrame::OnScrollChanged(wxScrollEvent& event)
{
	projetCourant->OnScrollChanged(this->slPlan->GetValue(),this->slPlan->GetMax());
}

void MainUiFrame::OnPauseSimulation(wxCommandEvent& event)
{
	projetCourant->OnPauseSimulation(event);
}

void MainUiFrame::OnStopSimulation(wxCommandEvent& event)
{
	projetCourant->OnStopSimulation(event);
}

void MainUiFrame::OnEmptySimulation(wxCommandEvent& event)
{
	projetCourant->OnEmptySimulation();
}

void MainUiFrame::OnChangeLanguage(wxCommandEvent& event)
{
	MainUiFrame::AskApplicationLanguage(wxLocale::GetSystemLanguage());
	this->saveManagerConfig=false;
	wxLogInfo(_("Language will be changed after restarting I-Simpa"));
}

void MainUiFrame::OnShowPreferenceTree(wxCommandEvent& event)
{
	wxAuiPaneInfo& optionPane=m_mgr.GetPane("userpref");
	if(optionPane.IsOk())
	{
		optionPane.Show().Float();
		m_mgr.Update();
	}
}
void MainUiFrame::OnShowAboutDialog(wxCommandEvent& event)
{
	wxAboutDialogInfo aboutDlg;
	aboutDlg.SetVersion(wxString::Format("2014 [%i.%i.%i]",ApplicationConfiguration::SPPS_UI_VERSION_MAJOR,ApplicationConfiguration::SPPS_UI_VERSION_MINOR,ApplicationConfiguration::SPPS_UI_VERSION_REVISION));
	aboutDlg.SetName(APPLICATION_NAME);
	aboutDlg.SetWebSite(wxT("http://i-simpa.ifsttar.fr"));
	//aboutDlg.SetLicence(GetLicenseText());
	aboutDlg.SetLicence("I-Simpa is an open source software (GPL v3).");
	aboutDlg.AddDeveloper("Nicolas FORTIN (Ifsttar)");
	aboutDlg.AddDeveloper("Judicaël PICAUT (Ifsttar)");
	aboutDlg.SetCopyright("(c) 2014 - Ifsttar <i-simpa@ifsttar.fr>");

	//wxArraySting devs;
	//devs.
	_("License >>");
	_("Developers >>");
	//aboutDlg.SetDescription(_("Outil de prévision acoustique pour l'habitat et le milieu urbain."));
	wxAboutBox(aboutDlg);
}

void MainUiFrame::OnLinkWebForum(wxCommandEvent& event)
{
	//ShellExecute( NULL,TEXT("open"), TEXT("http://i-simpa.ifsttar.fr/community/forum/"), NULL, NULL, SW_SHOWNORMAL);
	wxLaunchDefaultBrowser("http://i-simpa.ifsttar.fr/community/forum/");
}

void MainUiFrame::OnLinkWebIsimpa(wxCommandEvent& event)
{
	//ShellExecute( NULL,TEXT("open"), TEXT("http://i-simpa.ifsttar.fr/"), NULL, NULL, SW_SHOWNORMAL);
	wxLaunchDefaultBrowser("http://i-simpa.ifsttar.fr/");
}
void MainUiFrame::OnLinkWebDoc(wxCommandEvent& event)
{
	//ShellExecute( NULL,TEXT("open"), TEXT("http://i-simpa.ifsttar.fr/wiki/"), NULL, NULL, SW_SHOWNORMAL);
	wxLaunchDefaultBrowser("https://github.com/Ifsttar/I-Simpa/wiki");
}

void MainUiFrame::OnFileLicence(wxCommandEvent& event)
{
	wxLaunchDefaultApplication("licence.rtf");
}

void MainUiFrame::OnFileIsimpaDoc(wxCommandEvent& event)
{
	std::string docpath = ApplicationConfiguration::CONST_RESOURCE_FOLDER+wxString("doc")+wxFileName::GetPathSeparator()+wxString("documentation")+wxFileName::GetPathSeparator()+"manuel_I_Simpa.pdf";
	wxLaunchDefaultApplication(docpath);
}
void MainUiFrame::OnFileSppsDoc(wxCommandEvent& event)
{
	std::string docpath = ApplicationConfiguration::CONST_RESOURCE_FOLDER+wxString("doc")+wxFileName::GetPathSeparator()+wxString("documentation")+wxFileName::GetPathSeparator()+"SPPS_manuel.pdf";
	wxLaunchDefaultApplication(docpath);
}

void MainUiFrame::OnOpenFile (wxCommandEvent & event)
{
		smart_ptr<wxFileDialog> openFileDialog = new wxFileDialog ( this, _("Import new scene"), "", "", _("All compatible files|*.3ds;*.ply;*.bin;*.poly;*.stl|3DS file (*.3ds)|*.3ds|POLY file (*.poly)|*.bin|PLY file (*.ply)|*.ply|Binary file 1.00 (*.bin)|*.bin|Stererolithograpic file (*.stl)|*.stl"),
														wxFD_OPEN, wxDefaultPosition);

	if (openFileDialog->ShowModal() == wxID_OK)
	{
		wxString FileName(openFileDialog->GetPath());
		projetCourant->ChangeModel3d(FileName);
	}
}

void MainUiFrame::OnCreateModel(wxCommandEvent & event)
{
	float modelHeight=0;
	float modelWidth=0;
	float modelLength=0;


	bool heightOk=false;
	bool lengthOk=false;
	bool widthOk=false;
	while(!lengthOk || !heightOk || !widthOk)
	{
		wxCustomEntryDialog textDialog(this,_("Please enter scene dimensions"),_("Creating scene"));
		textDialog.AddTextControl(_("Width (x)"),"5.0");
		textDialog.AddTextControl(_("Length (y)"),"5.0");
		textDialog.AddTextControl(_("Height (z)"),"5.0");
		if (textDialog.ShowModal() == wxID_OK)
		{
			std::vector<wxString> valeursChamps;
			textDialog.GetValues(valeursChamps);
			 valeursChamps[0];

			modelWidth=Convertor::ToFloat(valeursChamps[0]);
			if(modelWidth!=0)
				widthOk=true;
			modelLength=Convertor::ToFloat(valeursChamps[1]);
			if(modelLength!=0)
				lengthOk=true;
			modelHeight=Convertor::ToFloat(valeursChamps[2]);
			if(modelHeight!=0)
				heightOk=true;
		}else{
			return;
		}

	}

	vec3 debCuboide(0,0,0);
	vec3 finCuboide(modelWidth,modelHeight,-modelLength);
	projetCourant->BuildModel3d(debCuboide,finCuboide);
}

void MainUiFrame::OnSaveFile (wxCommandEvent & event)
{
	wxFileDialog saveFileDialog( this, _("Export model"), "", "", _("Stanford University (*.ply)|*.ply|Stanford University with project groups (*.mat.ply)|*.mat.ply|Poly file (*.poly)|*.poly|Mesh file (*.mesh)|*.mesh|NFF file (*.nff)|*.nff|Binary file (*.bin)|*.bin|Binary file Calcul SPPS (*.cbin)|*.cbin|Ascii file (*.asc)|*.asc"),
														wxFD_SAVE, wxDefaultPosition);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxString FileName(wxString(saveFileDialog.GetPath()));
		projetCourant->OnExportModel(FileName);
	}
}


void MainUiFrame::OnOpenProject(wxCommandEvent& event)
{
	smart_ptr<wxFileDialog> openFileDialog = new wxFileDialog ( this, _("Open project"), "", "", _("Project (*.proj)|*.proj"),
														wxFD_OPEN, wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK)
	{
		wxString FileName(openFileDialog->GetPath());
		if(AskUserForSaveModification())
		{
			if(projetCourant->Open(FileName))
			{
				fileHistory->AddFileToHistory(FileName);
				//Lie les éléments du menu avec les éléments du projet en cours
				AttachElementsWithMenuItems();
			}
		}
	}
}

void MainUiFrame::OnSaveToProject(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog( this, _("Save project"), "", "", "Projet (*.proj)|*.proj",
														wxFD_SAVE, wxDefaultPosition);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxString FileName(wxString(saveFileDialog.GetPath()));
		fileHistory->AddFileToHistory(FileName);
		projetCourant->SaveTo(FileName);
	}
}
int MainUiFrame::AskApplicationLanguage(int defaultLanguage)
{
	int choosenLanguage=defaultLanguage;
//	LanguageSelector langSelection(NULL,_("Please choose language:"),_("Language"),ApplicationConfiguration::CONST_RESOURCE_FOLDER,ApplicationConfiguration::CONST_RESOURCE_FOLDER+ApplicationConfiguration::CONST_RESOURCE_DATA_FOLDER+wxString("flags")+wxFileName::GetPathSeparator());
	LanguageSelector langSelection(NULL,_("Please choose language:"),_("Language"),ApplicationConfiguration::CONST_RESOURCE_FOLDER+wxString("locale")+wxFileName::GetPathSeparator(),ApplicationConfiguration::CONST_RESOURCE_FOLDER+ApplicationConfiguration::CONST_RESOURCE_DATA_FOLDER+wxString("flags")+wxFileName::GetPathSeparator());
	wxInt32 choice=langSelection.ShowModal();
	if(choice==wxID_OK)
	{
		int sel=langSelection.GetSelectedLanguage();
		choosenLanguage=(wxLanguage)sel;
		ApplicationConfiguration::GetFileConfig()->Write("interface/language",Convertor::ToString(sel));
		ApplicationConfiguration::GetFileConfig()->Write("interface/generalPerspective",""); //Nettoyage du positionnement de l'interface
	}
	return choosenLanguage;
}
void MainUiFrame::SetLabel(const wxString& label)
{
	wxFrame::SetLabel(label);
}
bool MainUiFrame::AskUserForSaveModification()
{
	if(projetCourant->IsProjectNeedSave())
	{
		wxMessageDialog dialog( NULL, _("Project has been modified.\nSave project with modifications?"),
			wxString(_("Interface "))+APPLICATION_NAME, wxYES_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);

		wxCommandEvent cmdEvt;
		switch ( dialog.ShowModal() )
		{
			case wxID_YES:
				this->OnSaveProject(cmdEvt);
				return true;
				break;

			case wxID_NO:
				return true;
				break;

			case wxID_CANCEL:
				return false;
				break;

			default:
				wxLogError(_("Unexpected Dialog response return !"));
				return false;
		}
	}else{
		return true;
	}
}

void MainUiFrame::OnSaveProjectCopy(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog( this, _("Save copy of project"), "", "", "Projet (*.proj)|*.proj",
														wxFD_SAVE, wxDefaultPosition);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxString FileName(wxString(saveFileDialog.GetPath()));
		projetCourant->SaveCopy(FileName);
	}
}

void MainUiFrame::OnSaveProject(wxCommandEvent& event)
{
	if(!projetCourant->Save())
	{
		this->OnSaveToProject(event);
	}
}

void MainUiFrame::ChargerVertex()
{
	//this->tree->ChargerVertex(m_Object);
}

void MainUiFrame::Quit(wxCommandEvent& event)
{
	Close(false);
}
void MainUiFrame::OnShow(wxShowEvent& event)
{

}
void MainUiFrame::ExitProgram(wxCloseEvent& event)
{

	if(!this->AskUserForSaveModification())
	{
		if(event.CanVeto())
		{
			event.Veto();
		}else{
			projetCourant->SaveChanges(); //Si fermeture par le système on n'ecrase pas la sauvegarde mais on sauvegarde les modifications dans le projet ouvert.
		}
	}else{
		fileHistory->Save(*ApplicationConfiguration::GetFileConfig()); //Sauvegarde de l'historique des fichiers


		m_mgr.RestoreMaximizedPane();

		wxString cfgMngGen;
		if(saveManagerConfig)
			cfgMngGen=m_mgr.SavePerspective();

		ApplicationConfiguration::GetFileConfig()->Write("interface/generalPerspective",cfgMngGen); //Sauvegarde du positionnement de l'interface

		ApplicationConfiguration::GetFileConfig()->Flush(); //Ecrit sur le disque la configuration avant la fermeture du programme afin d'éviter le bug wxWidgets (http://trac.wxwidgets.org/ticket/11094)
		//this->OnCloseWindow(event);
		//wxPostEvent(this,event);
	}

	if(!event.GetVeto())
	{
		projetCourant->CloseApp();
		this->OnCloseWindow(event);
	}
}

MainUiFrame::~MainUiFrame()
{

	//Decharge le projet avant les contrôles de l'application
	//Reiinit the wxLogOutput

	delete wxLog::SetActiveTarget(new wxLogStderr(NULL));
	// deinitialize the frame manager
	m_mgr.UnInit();

}



BEGIN_EVENT_TABLE(ISimpaApp, wxApp)
EVT_KEY_DOWN(ISimpaApp::OnKeyDown)
EVT_KEY_UP(ISimpaApp::OnKeyUp)
EVT_DROP_FILES(ISimpaApp::OnFileDrop)
END_EVENT_TABLE()

wxDECLARE_APP(ISimpaApp);
wxIMPLEMENT_APP(ISimpaApp);

