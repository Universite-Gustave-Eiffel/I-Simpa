/* ----------------------------------------------------------------------
* I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
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
* For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or
* send an email to contact@noise-planet.org
*
* ----------------------------------------------------------------------*/

#include "i_simpa_main.h"
#include "IHM/customdialog/customdlg.h"
#include <wx/aboutdlg.h>
#include "IHM/languageSelection.hpp"
#include <wx/ffile.h>
#include <wx/evtloop.h>
#include <IHM/AboutDialog.hpp>
#include "last_cpp_include.hpp"
#include "data_manager/python_interface/pythonshell.hpp"


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
    EVT_MENU(ID_preferences_appdata, MainUiFrame::OnChangeAppData)
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
	EVT_MENU(ID_Help_Offline_Doc, MainUiFrame::OnLinkOfflineDoc)
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

MainUiFrame::MainUiFrame(wxLocale &lang) : wxFrame(NULL, -1, wxGetTranslation("Interface ")+APPLICATION_NAME,
									wxDefaultPosition, wxDefaultSize,
									wxDEFAULT_FRAME_STYLE),m_locale(lang)
{

	const wxString resourceFolder=ApplicationConfiguration::getResourcesFolder();
	saveManagerConfig=true;
	CustomLog logClass;
	// Liaison du manager aui à la fenêtre
	//m_mgr.SetFrame(this);
	m_mgr.SetManagedWindow(this);
	wxIcon ico_ui;
    #if not defined(__APPLE__)
		ico_ui.LoadFile(resourceFolder + "/isimpa.ico",wxBITMAP_TYPE_ICO)
	#else
		ico_ui.LoadFile(resourceFolder + "/isimpa.icns",wxBITMAP_TYPE_ICON);
	#endif

	if(ico_ui.IsOk())
		SetIcon(ico_ui);
	else
		wxLogError(wxGetTranslation("Can not read the I-SIMPA icon"));
	//Creation des menus


	wxMenuBar* mb = new wxMenuBar;

	wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_nouveau_projet, wxGetTranslation("New project\tCtrl+N"));
	file_menu->Append(ID_ouvrir, wxGetTranslation("Open project\tCtrl+O"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_importer, wxGetTranslation(wxT("Import new scene")));
	file_menu->Append(ID_exporter, wxGetTranslation("Export scene"));
	file_menu->Append(ID_generer, wxGetTranslation("New scene"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_enregistrer, wxGetTranslation("Save project\tCtrl+S"));
	file_menu->Append(ID_enregistrer_sous, wxGetTranslation("Save project as..."));
	file_menu->Append(ID_enregistrer_copie, wxGetTranslation("Save copy of project"));
	file_menu->AppendSeparator();
	wxMenu* recent_projet_menu = new wxMenu;
	file_menu->Append(ID_projets_recent, wxGetTranslation("Recent projects"),recent_projet_menu);

	//Utilisation de la classe projets récent
	fileHistory = new wxFileHistory(MAX_HISTORY_SHOW,ID_History);
	fileHistory->UseMenu(recent_projet_menu);
	fileHistory->Load(*ApplicationConfiguration::GetFileConfig());
	file_menu->AppendSeparator();

	wxMenu* file_preferences_menu = new wxMenu;
	file_preferences_menu->Append(ID_preferences_options, wxGetTranslation("Options"));
	file_preferences_menu->Append(ID_preferences_lang, wxGetTranslation("Language"));
    file_preferences_menu->Append(ID_preferences_appdata, wxGetTranslation("Change application data folder"));
	file_menu->Append(ID_preferences, wxGetTranslation("Settings"),file_preferences_menu);

	file_menu->AppendSeparator();
	file_menu->Append(ID_fermer, wxGetTranslation("Exit"));

	edit_menu = new wxMenu;
	edit_menu->Append(ID_annuler, wxGetTranslation("Undo\tCtrl+Z"))->Enable(false);
	edit_menu->Append(ID_retablir, wxGetTranslation("Redo\tCtrl+Y"))->Enable(false);

	wxMenu* simulation_menu = new wxMenu;
	simulation_menu->Append(ID_start_simulation, wxGetTranslation("Play"));
	simulation_menu->Append(ID_pause_simulation, wxGetTranslation("Pause"));
	simulation_menu->Append(ID_stop_simulation, wxGetTranslation("Stop"));
	simulation_menu->AppendSeparator();
	simulation_menu->Append(ID_Mailler, wxGetTranslation("Meshing"));

	wxMenu* outils_menu = new wxMenu;
	wxMenu* outils_menu_postprocess = new wxMenu;
	outils_menu_postprocess->Append(ID_outil_resultpostprocess_substract_recepteursurf, wxGetTranslation("Compare surface receivers"));
	outils_menu->Append(ID_outil_resultpostprocess,wxGetTranslation("File results process"),outils_menu_postprocess);

	//outils_menu->AppendSeparator();
	//outils_menu->Append(ID_outil_options, wxGetTranslation("Options..."));


	view_menu = new wxMenu;

	view_menu->Append(ID_clearwindowmanager, wxGetTranslation("Reinitialize interface"));
	view_menu->AppendSeparator();
	view_face_menu = new wxMenu;
	view_face_menu->Append(ID_flat, wxGetTranslation("Outside"),"Outside",wxITEM_RADIO);
	view_face_menu->Append(ID_flat_inside, wxGetTranslation("Inside"),"Inside",wxITEM_RADIO);
	view_face_menu->Append(ID_flat_none, wxGetTranslation("None"),"None",wxITEM_RADIO);
	view_face_menu->Check(ID_flat_inside,true);
	view_menu->Append(ID_fold_face, wxGetTranslation("Faces"),view_face_menu);


	wxMenu* affichage_material_menu = new wxMenu;
	affichage_material_menu->Append(ID_ModelMaterial, wxGetTranslation("Original"),"Original",wxITEM_RADIO);
	affichage_material_menu->Append(ID_SoundMaterial, wxGetTranslation("Material"),"Material",wxITEM_RADIO);
	view_menu->Append(ID_fold_material, wxGetTranslation("Material color"),affichage_material_menu);
	affichage_material_menu->Check(ID_ModelMaterial,true);

	view_line_menu = new wxMenu;
	view_line_menu->Append(ID_wireframe_all, wxGetTranslation("All"),"All",wxITEM_RADIO);
	view_line_menu->Append(ID_wireframe_shape, wxGetTranslation("Contour"),"Contour",wxITEM_RADIO);
	view_line_menu->Append(ID_wireframe_none, wxGetTranslation("None"),"None",wxITEM_RADIO);
	view_line_menu->Check(ID_wireframe_none,true);
	view_menu->Append(ID_fold_wireframe, wxGetTranslation("Lines"),view_line_menu);

	view_menu->AppendSeparator();

	view_menu->Append(ID_ShowHideMailler, wxGetTranslation("Hide meshing"),"Hide meshing",wxITEM_CHECK);

	view_menu->AppendSeparator();

	view_menu->Append(ID_InitCamera, wxGetTranslation("Reinitialize camera"));
	view_camera_menu = new wxMenu;
	view_camera_menu->Append(ID_camera_firstperson, wxGetTranslation("First person"),"First person",wxITEM_RADIO);
	view_camera_menu->Append(ID_camera_rotate, wxGetTranslation("Rotation / Zoom"),"Rotation / Zoom",wxITEM_RADIO);
	view_camera_menu->Check(ID_camera_rotate,true);
	view_menu->Append(ID_fold_camera, wxGetTranslation("Camera"),view_camera_menu);

	view_menu->AppendSeparator();

	//Ces éléments du menu interagissent avec l'etat d'élément du projet
	LinkMenuItemWithElement(view_menu->Append(ID_grid_xy, wxGetTranslation("XY Grid"),"XY Grid",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,
		"showgxy");
	LinkMenuItemWithElement(view_menu->Append(ID_grid_xz, wxGetTranslation("XZ Grid"),"XZ Grid",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,
		"showgxz");
	LinkMenuItemWithElement(view_menu->Append(ID_grid_yz, wxGetTranslation("YZ Grid"),"YZ Grid",wxITEM_CHECK),
		Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,
		"showgyz");

	view_menu->AppendSeparator();

	wxMenu* window3D_menu_console = new wxMenu;
	window3D_menu_console->Append(ID_Dock3Dwindow, wxGetTranslation("Set 3D View as main window"));
	window3D_menu_console->Append(ID_Float3Dwindow, wxGetTranslation("Floating 3D View"));
	window3D_menu_console->Append(ID_copygl_tofile, wxGetTranslation("Copy 3D View"));
	view_menu->AppendSubMenu(window3D_menu_console,wxGetTranslation("3D View"));
	wxMenu* fenetre_menu_console = new wxMenu;
	fenetre_menu_console->Append(ID_Console_SaveToFile, wxGetTranslation("Export to file"));
	fenetre_menu_console->AppendSeparator();
	fenetre_menu_console->Append(ID_Console_ClearLog, wxGetTranslation("Delete windows"));
	view_menu->AppendSeparator();
	view_menu->AppendSubMenu(fenetre_menu_console,wxGetTranslation("Console"));


	wxMenu* aide_menu = new wxMenu;
	aide_menu->Append(ID_Help_Web, wxGetTranslation("Website"));
	aide_menu->Append(ID_Help_Doc, wxGetTranslation("Online documentation"));
	aide_menu->Append(ID_Help_Offline_Doc, wxGetTranslation("Offline documentation"));
	aide_menu->AppendSeparator();
	aide_menu->Append(ID_Help_About, wxGetTranslation("About ")+APPLICATION_NAME);



	mb->Append(file_menu, wxGetTranslation("File"));
	mb->Append(edit_menu, wxGetTranslation("Edition"));
	mb->Append(simulation_menu, wxGetTranslation("Simulation"));
	mb->Append(outils_menu, wxGetTranslation("Tools"));
	mb->Append(view_menu, wxGetTranslation("View"));
	mb->Append(aide_menu, wxGetTranslation("Help"));

    SetMenuBar(mb);

	wxSize client_size = GetClientSize();
	// Creating the Operation History Text Control
	shellNotebook= new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    FromDIP(wxSize(430,200)),
                                    wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );
	shellNotebook->SetName("console");

	logWindow = new wxTextCtrl(this, -1, "",
							wxDefaultPosition, FromDIP(wxSize(200,150)),
							wxTE_RICH | wxNO_BORDER | wxTE_MULTILINE | wxTE_READONLY);

    shellNotebook->AddPage(logWindow, wxGetTranslation("Messages") , true);
	#ifdef USE_PYTHON
		PyConsole* consolePython=new PyConsole(this);
		shellNotebook->AddPage(consolePython, wxGetTranslation("Python(TM)") , false);
	#else
		PyConsole* consolePython=NULL;
	#endif


	// Creation of the panel which contains 3 trees

	ctrlNotebook = new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    FromDIP(wxSize(430,200)),
                                    wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );


	// Creation of the 3 project management trees
	this->treeCore = new uiTreeCtrl(ctrlNotebook.get(), -1, wxPoint(0,0), FromDIP(wxSize(-1,250)),wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS  | wxTR_HIDE_ROOT, true); //wxTR_MULTIPLE |
	this->treeResult = new uiTreeCtrl(ctrlNotebook.get(), -1, wxPoint(0,0), FromDIP(wxSize(-1,250)), wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT, false );
	this->treeScene = new uiTreeCtrl(ctrlNotebook.get(), -1, wxPoint(0,0), FromDIP(wxSize(-1,250)), wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT, true ); //
	this->treeUserPref = new uiTreeCtrl(this, -1, wxPoint(0,0), FromDIP(wxSize(-1,250)), wxTR_EDIT_LABELS | wxNO_BORDER | wxTR_MULTIPLE | wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT, true ); //

	treeCore->BindElementEvent(&OnCoreElementEvent);
	treeResult->BindElementEvent(&OnReportElementEvent);
	treeScene->BindElementEvent(&OnSceneElementEvent);
	treeUserPref->BindElementEvent(&OnUserConfigElementEvent);


    ctrlNotebook->AddPage(this->treeScene, wxGetTranslation("Scene") , false);
    ctrlNotebook->AddPage(this->treeCore, wxGetTranslation("Calculation") , false);
    ctrlNotebook->AddPage(this->treeResult, wxGetTranslation("Results") , false);



	// Add interface panels
	m_mgr.AddPane(ctrlNotebook.get(), wxAuiPaneInfo().
					Name("projet").Caption(wxGetTranslation("Project")).
					Left().Position(0).CloseButton(false));
	m_mgr.AddPane(shellNotebook.get(), wxAuiPaneInfo().
					Name("console").Caption(wxGetTranslation("Console")).
					Bottom().Position(0).CloseButton(false));

	m_mgr.AddPane(treeUserPref.get(), wxAuiPaneInfo().
				Name("userpref").Caption(wxGetTranslation("Interface options")).Hide()
				);
#ifdef __WXMSW__
    int *gl_attrib = NULL;
#else
    int gl_attrib[20] =
        { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
        WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
        WX_GL_DOUBLEBUFFER,

#  if defined(__WXMAC__)  || defined(__WXQT__) || defined(__linux__)
        GL_NONE };
#  else
        None };
#  endif
#endif
	GlFrame = new OpenGlViewer(this, -1, wxPoint(0,25), wxSize(-1,-1),
												0, wxGetTranslation("Main window"), gl_attrib );

	toolbarGl = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);

	slPlan = new wxSlider(toolbarGl, ID_slPlan, 0, 0, 1000,wxDefaultPosition,FromDIP(wxSize(100,16)));
	xPlan = new wxRadioButton(toolbarGl,ID_xPlan,"x",wxDefaultPosition,wxDefaultSize,wxRB_GROUP );
	yPlan = new wxRadioButton(toolbarGl,ID_yPlan,"y");
	zPlan = new wxRadioButton(toolbarGl,ID_zPlan,"z");

	wxToolBar* simulation= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	wxToolBar* visualisation= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	wxToolBar* tbProjet= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER );
	mousetool= new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);

    mousetool->SetToolBitmapSize(FromDIP(wxSize(16,16)));
    toolbarGl->SetToolBitmapSize(FromDIP(wxSize(16,16)));
	simulation->SetToolBitmapSize(FromDIP(wxSize(16,16)));
	visualisation->SetToolBitmapSize(FromDIP(wxSize(16,16)));
	tbProjet->SetToolBitmapSize(FromDIP(wxSize(16,16)));

	tbProjet->AddTool(ID_nouveau_projet, wxGetTranslation("New project"), wxImage(resourceFolder+"/Bitmaps/toolbar_newproject.png", wxBITMAP_TYPE_PNG), wxGetTranslation("New project"));
	tbProjet->AddTool(ID_ouvrir, wxGetTranslation("Open project"), wxImage(resourceFolder+"/Bitmaps/toolbar_openproject.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Open project"));
	tbProjet->AddSeparator();
	tbProjet->AddTool(ID_enregistrer, wxGetTranslation("Save project"), wxImage(resourceFolder+"/Bitmaps/toolbar_saveproject.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Save project"));
	tbProjet->AddTool(ID_enregistrer_sous, wxGetTranslation("Save project as..."), wxImage(resourceFolder+"/Bitmaps/toolbar_saveprojectas.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Save project as..."));

	simulation->AddTool(ID_previous_step_simulation, wxGetTranslation("Previous time step"), wxImage(resourceFolder+"/Bitmaps/toolbar_animation_previousstep.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Previous time step"));
	simulation->AddTool(ID_start_simulation, wxGetTranslation("Play"), wxImage(resourceFolder+"/Bitmaps/toolbar_animation_start.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Play"));
	simulation->AddTool(ID_pause_simulation, wxGetTranslation("Pause"), wxImage(resourceFolder+"/Bitmaps/toolbar_animation_pause.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Pause"));
	simulation->AddTool(ID_next_step_simulation, wxGetTranslation("Next time step"), wxImage(resourceFolder+"/Bitmaps/toolbar_animation_nextstep.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Next time step"));
	simulation->AddTool(ID_stop_simulation, wxGetTranslation("Stop"), wxImage(resourceFolder+"/Bitmaps/toolbar_animation_stop.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Stop"));
	simulation->AddTool(ID_delete_simulation, wxGetTranslation("Delete the simulation"), wxImage(resourceFolder+"/Bitmaps/toolbar_hideallanimations.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Delete the simulation"));

	visualisation->AddTool(ID_tool_flat_inside, wxGetTranslation("Face view"), wxImage(resourceFolder+"/Bitmaps/toolbar_showfaceonly.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Face view"));
	visualisation->AddTool(ID_tool_wireframe_all, wxGetTranslation("Wireframe view"), wxImage(resourceFolder+"/Bitmaps/toolbar_showsidesonly.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Wireframe view"));
	visualisation->AddTool(ID_tool_wireframe_and_flat, wxGetTranslation("Combined view"), wxImage(resourceFolder+"/Bitmaps/toolbar_showfacewithsides.png", wxBITMAP_TYPE_PNG), wxGetTranslation("Combined view"));


	mousetool->AddRadioTool(ID_toolCameraMode, wxGetTranslation("Camera mode"), wxImage(resourceFolder+"/Bitmaps/toolbar_tool_camera.png", wxBITMAP_TYPE_PNG), wxNullBitmap, wxGetTranslation("Camera mode"));
	mousetool->AddRadioTool(ID_SwitchSelectionMode, wxGetTranslation("Surface selection mode"), wxImage(resourceFolder+"/Bitmaps/toolbar_tool_faceselection.png", wxBITMAP_TYPE_PNG), wxNullBitmap, wxGetTranslation("Surface selection mode"));
	mousetool->AddRadioTool(ID_toolGetdBLevelMode, wxGetTranslation("Extract sound level from 3D view"), wxImage(resourceFolder+"/Bitmaps/toolbar_tool_extractlvl.png", wxBITMAP_TYPE_PNG), wxNullBitmap, wxGetTranslation("Extract sound level from 3D view"));

	visualisation->AddSeparator();
	visualisation->AddTool(ID_InitCamera, wxGetTranslation("Reinitialize camera"), wxImage((resourceFolder+"/Bitmaps/toolbar_camera_movetoorigin.png"), wxBITMAP_TYPE_PNG), wxGetTranslation("Reinitialize camera"));
	visualisation->AddTool(ID_camera_rotate, wxGetTranslation("Orbit camera"), wxImage((resourceFolder+"/Bitmaps/toolbar_camera_rotation.png"), wxBITMAP_TYPE_PNG), wxGetTranslation("Orbit camera"));
	visualisation->AddTool(ID_camera_firstperson, wxGetTranslation("First person camera"), wxImage((resourceFolder+"/Bitmaps/toolbar_camera_firstperson.png"), wxBITMAP_TYPE_PNG), wxGetTranslation("First person camera"));

	toolbarGl->AddTool(ID_Mailler, wxGetTranslation("Meshing"), wxImage((resourceFolder+"/Bitmaps/toolbar_run_tetmesh.png"), wxBITMAP_TYPE_PNG), wxGetTranslation("Meshing"));
	toolbarGl->AddTool(ID_Tool_ShowHideMailler, wxGetTranslation("Show / Hide meshing"), wxImage((resourceFolder+"/Bitmaps/toolbar_show_hide_tetmesh.png"), wxBITMAP_TYPE_PNG), wxGetTranslation("Show / Hide meshing"));
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
	m_mgr.AddPane(GlFrame.get(), wxAuiPaneInfo().Name("3Dview").Caption(wxGetTranslation("Main window")).
					CenterPane());

    m_mgr.AddPane(tbProjet, wxAuiPaneInfo().
                  Name(wxT("projetTb")).Caption(wxGetTranslation("Project toolbar")).
                  ToolbarPane().Position(0).Top().CloseButton(false));



    m_mgr.AddPane(visualisation, wxAuiPaneInfo().
                  Name(wxT("visualisationTb")).Caption(wxGetTranslation("Camera toolbar")).
                  ToolbarPane().Top().Position(1).CloseButton(false));


    m_mgr.AddPane(toolbarGl, wxAuiPaneInfo().
                  Name(wxT("maillageTb")).Caption(wxGetTranslation("Meshing toolbar")).
                  ToolbarPane().Top().Position(2).CloseButton(false));

    m_mgr.AddPane(simulation, wxAuiPaneInfo().
                  Name(wxT("simulationTb")).Caption(wxGetTranslation("Animation toolbar")).
                  ToolbarPane().Position(3).Top().CloseButton(false));

    m_mgr.AddPane(mousetool, wxAuiPaneInfo().
                  Name(wxT("toolTb")).Caption(wxGetTranslation("Pointer toolbar")).
                  ToolbarPane().Position(4).Top().CloseButton(false));
	// Property Sheet

	propertiesGrid = new MainPropGrid(this,-1,wxDefaultPosition,FromDIP(wxSize(200,300)),262144,wxGetTranslation("Properties"));


	m_mgr.AddPane(&(*(propertiesGrid)), wxAuiPaneInfo().
					Name(wxGetTranslation("Properties")).Caption(wxGetTranslation("Properties")).
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

	// Render 3D view
	m_mgr.GetPane("3Dview").Show();

	// Resize window
	this->SetSize(FromDIP(wxSize(1024, 768)));
}
void MainUiFrame::OnWindowLoaded() {

}
void MainUiFrame::OnClearConsole(wxCommandEvent& event)
{
	projetCourant->OnClearConsole();
}

void MainUiFrame::OnSaveConsoleToFile(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog( this, wxGetTranslation("Save console"), "Text file (*.txt)|*.txt", "Text file (*.txt)|*.txt", wxGetTranslation("Text file (*.txt)|*.txt"),
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
			paneInfo.MinSize(FromDIP(wxSize(320,240)));
		}
		this->m_mgr.Update();
	}
}
void MainUiFrame::OnClearWindowManager(wxCommandEvent& event)
{
	saveManagerConfig=false;
	m_mgr.LoadPerspective(originalPerspective);
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
	view_face_menu->Check(ID_flat_inside,true);
	view_line_menu->Check(ID_wireframe_shape,true);
	projetCourant->OnModeFlatInside(event);
	projetCourant->OnModeWireFrameShape(event);
}

void MainUiFrame::OnModeToolFlat(wxCommandEvent& event)
{
	view_face_menu->Check(ID_flat_inside,true);
	view_line_menu->Check(ID_wireframe_none,true);
	projetCourant->OnModeFlatInside(event);
	projetCourant->OnModeWireFrameNone(event);
}
void MainUiFrame::OnModeToolWireFrame(wxCommandEvent& event)
{
	view_face_menu->Check(ID_flat_none,true);
	view_line_menu->Check(ID_wireframe_shape,true);
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
		view_menu->Check(ID_ShowHideMailler,!view_menu->IsChecked(ID_ShowHideMailler));
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
	wxLogMessage(wxGetTranslation("Language will be changed after restarting I-Simpa"));
}

void MainUiFrame::OnChangeAppData(wxCommandEvent& event)
{
    MainUiFrame::AskApplicationDataDir(ApplicationConfiguration::GLOBAL_VAR.appDataFolderPath);
    ApplicationConfiguration::GetFileConfig()->Write("interface/appdata", ApplicationConfiguration::GLOBAL_VAR.appDataFolderPath);
    wxLogMessage(wxGetTranslation("Application data folder will be changed after restarting I-Simpa"));
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
    AboutDialog aboutDialog;
    aboutDialog.CreateAboutDialog(this);
	aboutDialog.ShowModal();
}

void MainUiFrame::OnLinkWebIsimpa(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser("https://i-simpa.univ-gustave-eiffel.fr/");
}

void MainUiFrame::OnLinkWebDoc(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser("http://i-simpa-wiki.readthedocs.io");
}


void MainUiFrame::OnLinkOfflineDoc(wxCommandEvent& event)
{
	wxString docpath = wxString("file://") + ApplicationConfiguration::getResourcesFolder() + wxString("doc") + wxFileName::GetPathSeparator() + wxString("documentation") + wxFileName::GetPathSeparator() + wxString("Offline_documentation") + wxFileName::GetPathSeparator() + wxString("html") + wxFileName::GetPathSeparator() + wxString("index.html");
	wxLaunchDefaultBrowser(docpath);
}

void MainUiFrame::OnOpenFile (wxCommandEvent & event)
{
		smart_ptr<wxFileDialog> openFileDialog = new wxFileDialog ( this, wxGetTranslation("Import new scene"), "", "", wxGetTranslation("All compatible files|*.3ds;*.ply;*.bin;*.poly;*.stl|3DS file (*.3ds)|*.3ds|POLY file (*.poly)|*.bin|PLY file (*.ply)|*.ply|Binary file 1.00 (*.bin)|*.bin|Stererolithograpic file (*.stl)|*.stl"),
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
		wxCustomEntryDialog textDialog(this,wxGetTranslation("Please enter scene dimensions (m)"),wxGetTranslation("Creating scene"));
		textDialog.AddTextControl(wxGetTranslation("Width (x)"),"5.0");
		textDialog.AddTextControl(wxGetTranslation("Length (y)"),"5.0");
		textDialog.AddTextControl(wxGetTranslation("Height (z)"),"5.0");
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
	wxFileDialog saveFileDialog( this, wxGetTranslation("Export model"), "", "", wxGetTranslation("Stanford University (*.ply)|*.ply|Stanford University with project groups (*.mat.ply)|*.mat.ply|Poly file (*.poly)|*.poly|Mesh file (*.mesh)|*.mesh|NFF file (*.nff)|*.nff|Binary file (*.bin)|*.bin|Binary file Calcul SPPS (*.cbin)|*.cbin|Ascii file (*.asc)|*.asc"),
														wxFD_SAVE, wxDefaultPosition);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxString FileName(wxString(saveFileDialog.GetPath()));
		projetCourant->OnExportModel(FileName);
	}
}


void MainUiFrame::OnOpenProject(wxCommandEvent& event)
{
	smart_ptr<wxFileDialog> openFileDialog = new wxFileDialog ( this, wxGetTranslation("Open project"), "", "", wxGetTranslation("Project (*.proj)|*.proj"),
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
	wxFileDialog saveFileDialog( this, wxGetTranslation("Save project"), "", "", "Projet (*.proj)|*.proj",
														wxFD_SAVE, wxDefaultPosition);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxString FileName(wxString(saveFileDialog.GetPath()));
		fileHistory->AddFileToHistory(FileName);
		projetCourant->SaveTo(FileName);
	}
}
wxString MainUiFrame::AskApplicationDataDir(wxString defaultApplicationDirectory) {


    wxMessageDialog dialog( NULL, wxString::Format(
                                    wxGetTranslation("Do you accept to write the projects into this directory:\n%s"), defaultApplicationDirectory),
                            wxGetTranslation("Application data directory"), wxYES_DEFAULT|wxYES_NO|wxICON_INFORMATION);

	if(dialog.ShowModal()== wxID_YES) {
		return defaultApplicationDirectory;
	} else {
		// User want to select a folder
		wxDirDialog folderChooser(NULL, wxGetTranslation("Select an empty folder that will contain your projects"), defaultApplicationDirectory,  wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		if(folderChooser.ShowModal() == wxID_OK) {
			return folderChooser.GetPath();
		} else {
			return defaultApplicationDirectory;
		}
	}
}

int MainUiFrame::AskApplicationLanguage(int defaultLanguage)
{
	int choosenLanguage=defaultLanguage;
	LanguageSelector langSelection(NULL, wxGetTranslation("Please choose language:"),wxGetTranslation("Language"));
	wxInt32 choice=langSelection.ShowModal();
	if(choice==wxID_OK)
	{
		int sel=langSelection.GetSelectedLanguage();
		choosenLanguage=(wxLanguage)sel;
		ApplicationConfiguration::GetFileConfig()->Write("interface/language",wxString(Convertor::ToString(sel)));
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
		wxMessageDialog dialog( NULL, wxGetTranslation("Project has been modified.\nSave project with modifications?"),
			wxString(wxGetTranslation("Interface "))+APPLICATION_NAME, wxYES_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);

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
				wxLogError(wxGetTranslation("Unexpected Dialog response return !"));
				return false;
		}
	}else{
		return true;
	}
}

void MainUiFrame::OnSaveProjectCopy(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog( this, wxGetTranslation("Save copy of project"), "", "", "Projet (*.proj)|*.proj",
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
        m_mgr.UnInit();
		this->OnCloseWindow(event);
	}
}

MainUiFrame::~MainUiFrame()
{

	//Decharge le projet avant les contrôles de l'application
	//Reiinit the wxLogOutput

	delete wxLog::SetActiveTarget(new wxLogStderr(NULL));

}



BEGIN_EVENT_TABLE(ISimpaApp, wxApp)
EVT_KEY_DOWN(ISimpaApp::OnKeyDown)
EVT_KEY_UP(ISimpaApp::OnKeyUp)
EVT_DROP_FILES(ISimpaApp::OnFileDrop)
END_EVENT_TABLE()

wxDECLARE_APP(ISimpaApp);
wxIMPLEMENT_APP(ISimpaApp);
