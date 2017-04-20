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

/*
	Projet.cpp
*/
#include "projet.h"

#include "python_interface/pythonstdioredirect.hpp"
#include "e_data_color.h"
#include "e_data_text.h"
#include "e_position.h"
#include <wx/wfstream.h>
#include <wx/sstream.h>
#include <wx/zipstrm.h>
#include <wx/ptr_scpd.h>
#include <wx/app.h>
#include "data_manager/appconfig.h"
#include "manager/processManager.h"
#include "manager/stringParser.h"
#include "manager/bigfilereader.h"
#include <wx/clipbrd.h>
#include <wx/regex.h>
#include <wx/stdpaths.h>
#include "tree_rapport/e_report_recepteurssvisualisation.h"
#include "tree_rapport/e_report_unknown_file.h"
#include "IHM/DataWindow.h"
#include <wx/wupdlock.h>
#include "IHM/RecepteurSOperationDialog.h"
#include "3dengine/tools/recepteursurf_difference.hpp"
#include "IHM/customdialog/customdlg.h"
#include "data_manager/python_interface/instanceManager.hpp"
#include "IHM/loadingSceneDialog.hpp"
#include "data_manager/tree_userpref/e_userprefnode.hpp"




#include "data_manager/tree_scene/e_scene.h"
#include "data_manager/tree_scene/e_scene_groupesurfaces.h"
#include "data_manager/tree_core/e_core.h"
#include "data_manager/tree_rapport/e_report.h"
#include "data_manager/tree_scene/e_scene_bdd_materiaux.h"
#include "manager/boollocker.h"
#include "last_cpp_include.hpp"

wxDEFINE_SCOPED_PTR_TYPE(wxZipEntry);

extern ProjectManager *projetCourant;



void OnElementRootUpdate( Element* updEl)
{
	projetCourant->OnUpdateEvent(updEl);
}


/**
 * Cette classe est à attacher à l'élément qui permet à l'utilisateur de modifier les paramètres de rendu 3D
 */
struct SceneUpdater : public t_baseEvtCatcher
{
	SceneUpdater(Element* _render3DPrefItem,CObjet3D& _objToUpdate,OpenGlViewer* _GlFrame)
		: objToUpdate(_objToUpdate),render3DPrefItem(_render3DPrefItem),GlFrame(_GlFrame)
	{
		objToUpdate.SetColorSelection(render3DPrefItem->GetColorConfig("colorselection")/255);
		objToUpdate.SetColorModelLines(render3DPrefItem->GetColorConfig("colormodellines")/255);
		objToUpdate.SetColorModelBackground(render3DPrefItem->GetColorConfig("colorbackground")/255);
		objToUpdate.SetDefaultModelColor(render3DPrefItem->GetColorConfig("colordefaultmodel")/255);
		GlFrame->Refresh();
	}
	virtual void OnModified(const int& elid)
	{
		Element* updatedEl=render3DPrefItem->GetElementByXMLId(elid);
		const Element::t_elementInfo& elementInfo=updatedEl->GetElementInfos();
		if(elementInfo.libelleElement=="colorselection")
		{
			objToUpdate.SetColorSelection(render3DPrefItem->GetColorConfig("colorselection")/255);
		}else if(elementInfo.libelleElement=="colormodellines")
		{
			objToUpdate.SetColorModelLines(render3DPrefItem->GetColorConfig("colormodellines")/255);
		}else if(elementInfo.libelleElement=="colorbackground")
		{
			objToUpdate.SetColorModelBackground(render3DPrefItem->GetColorConfig("colorbackground")/255);
		}else if(elementInfo.libelleElement=="colordefaultmodel")
		{
			objToUpdate.SetDefaultModelColor(render3DPrefItem->GetColorConfig("colordefaultmodel")/255);
		}
		GlFrame->Refresh();
	}
	CObjet3D& objToUpdate;
	Element* render3DPrefItem;
	OpenGlViewer* GlFrame;
};
/**
 * Cette classe est à attacher à l'élément qui permet à l'utilisateur de modifier l'activation des animations
 */
struct AnimationUpdater : public t_baseEvtCatcher
{
	AnimationUpdater(Element* _animationPrefItem)
		: animationPrefItem(_animationPrefItem)
	{
		projetCourant->UpdateSimulationConfig(animationPrefItem);
	}
	virtual void OnModified(const int& elid)
	{
		projetCourant->UpdateSimulationConfig(animationPrefItem);
	}
	Element* animationPrefItem;
};

/**
 * Cette classe est à attacher à l'élément qui permet à l'utilisateur de modifier les palettes de couleur pour les réceupteurs de surfaces
 */
struct IsoPaletteUpdater : public t_baseEvtCatcher
{
	IsoPaletteUpdater(Element* _IsoSelectionItem,RecepteursSControler* _rsAnimator)
		: IsoSelectionItem(_IsoSelectionItem),rsAnimator(_rsAnimator)
	{
		UpdateIsoFile();
		UpdateIsoLineColor();
	}
	void UpdateIsoFile()
	{
		rsAnimator->LoadGplFile(IsoSelectionItem->GetStringConfig("path_isofile"));
	}

	void UpdateIsoLineColor()
	{
		vec3 nvcouleur(IsoSelectionItem->GetColorConfig("iso_line_color"));
		rsAnimator->SetIsoLineColor(ivec3((long)nvcouleur.r,(long)nvcouleur.g,(long)nvcouleur.b));
		rsAnimator->Refresh(NULL,true);
	}
	virtual void OnModified(const int& elid)
	{
		Element* updatedEl=IsoSelectionItem->GetElementByXMLId(elid);
		if(!updatedEl)
			return;
		const Element::t_elementInfo& elementInfo=updatedEl->GetElementInfos();
		if(elementInfo.libelleElement=="path_isofile")
		{
			UpdateIsoFile();
		}else if(elementInfo.libelleElement=="iso_line_color")
		{
			UpdateIsoLineColor();
		}
	}
	Element* IsoSelectionItem;
	RecepteursSControler* rsAnimator;
};

/**
 * Cette classe est à attacher à l'élément qui permet à l'utilisateur de modifier les préférence d'un animateur
 */
struct AnimatorUserPreferenceUpdater : public t_baseEvtCatcher
{
	AnimatorUserPreferenceUpdater(Element* _userpreferenceItem,Animator* _linkedAnimator)
		: userpreferenceItem(_userpreferenceItem),linkedAnimator(_linkedAnimator)
	{
		UpdateTextForeColor();
		UpdateTextBackColor();
		UpdateTextFont();
	}
	void UpdateTextForeColor()
	{
		vec3 nvcouleur(userpreferenceItem->GetColorConfig("legend_text_foreground_color"));
		linkedAnimator->SetTextForegroundColor(ivec4((long)nvcouleur.r,(long)nvcouleur.g,(long)nvcouleur.b,0));
		linkedAnimator->RedrawLegend();
	}
	void UpdateTextBackColor()
	{
		long alpha=255;
		if(userpreferenceItem->GetBoolConfig("legend_text_background_invisible"))
			alpha=0;
		vec3 nvcouleur(userpreferenceItem->GetColorConfig("legend_text_background_color"));
		linkedAnimator->SetTextBackgroundColor(ivec4((long)nvcouleur.r,(long)nvcouleur.g,(long)nvcouleur.b,alpha));
		linkedAnimator->RedrawLegend();
	}
	void UpdateTextFont()
	{
		linkedAnimator->SetTextFont(userpreferenceItem->GetFontConfig("legend_font"));
		linkedAnimator->RedrawLegend();
	}

	virtual void OnModified(const int& elid)
	{
		Element* updatedEl=userpreferenceItem->GetElementByXMLId(elid);
		if(!updatedEl)
			return;
		const Element::t_elementInfo& elementInfo=updatedEl->GetElementInfos();
		if(elementInfo.libelleElement=="legend_text_foreground_color")
		{
			UpdateTextForeColor();
		}else if(elementInfo.libelleElement=="legend_text_background_color" || elementInfo.libelleElement=="legend_text_background_invisible")
		{
			UpdateTextBackColor();
		}else if(elementInfo.libelleElement=="legend_font")
		{
			UpdateTextFont();
		}
	}
	Element* userpreferenceItem;
	Animator* linkedAnimator;
};


/**
 * Cette classe est à attacher à l'élément qui permet à l'utilisateur de modifier les propriétés de rendu des particules
 */
struct ParticleAnimatorUpdater : public t_baseEvtCatcher
{
	ParticleAnimatorUpdater(Element* _ParticleUserPreferenceItem,ParticulesControler* _particleAnimator)
		: particleUserPreferenceItem(_ParticleUserPreferenceItem),particleAnimator(_particleAnimator)
	{
		UpdateParticleColor();
	}
	void UpdateParticleColor()
	{
		vec3 nvcouleur(particleUserPreferenceItem->GetColorConfig("particle_color"));
		particleAnimator->SetParticleColor(ivec3((long)nvcouleur.r,(long)nvcouleur.g,(long)nvcouleur.b));
		particleAnimator->RefreshAnimationRendering();
	}
	virtual void OnModified(const int& elid)
	{
		Element* updatedEl=particleUserPreferenceItem->GetElementByXMLId(elid);
		if(!updatedEl)
			return;
		const Element::t_elementInfo& elementInfo=updatedEl->GetElementInfos();
		if(elementInfo.libelleElement=="particle_color")
		{
			UpdateParticleColor();
		}
	}
	Element* particleUserPreferenceItem;
	ParticulesControler* particleAnimator;
};
/**
 * Retourne la version raccourci de strToFit si nécessaire, en fonction de la taille du progDialog
 * @param progDialog Dialogue de progression
 * @param strToFit Chaine de caractère à raccourcir
 * @return La version courte de la chaine de caractère
 */

wxString GetFittedString(const wxProgressDialog& progDialog,const wxString& strToFit)
{
	wxString outName=strToFit;
	int w,h;
	int win_w,win_h;
	progDialog.GetTextExtent(outName,&w,&h);
	progDialog.GetClientSize(&win_w,&win_h);
	wxSizer* windowChilds=progDialog.GetSizer();
	if(!windowChilds)
		return strToFit;
	wxSizerItem* labelSizerItem=windowChilds->GetItem((size_t)0);
	if(!labelSizerItem)
		return strToFit;
	win_w-=labelSizerItem->GetBorder();
	if(w>win_w && (win_w!=0 && win_h!=0))
	{
		wxString label(outName);
		int throwed_char=outName.size()-floor(outName.size()*(win_w/(float)w))+2;
		do
		{
			outName=".."+label.Mid(throwed_char);
			progDialog.GetTextExtent(outName,&w,&h);
			throwed_char++;
		}while(w>win_w && throwed_char<label.size()-1);
	}
	return outName;
}


ProjectManager::ProjectManager()
:lastHistoryUpdate(wxDateTime::UNow()),
currentHistoryNavigation(0)
,renderOriginalMaterial(true)
{
	InstanceManager::AppendReference(OBJECT_REF_TYPE_PROJECT,0,this);
	//On utilise le projet présent dans /current/

	this->dossierCourant = ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath;
	this->FichierConfig = "projet_config.xml";	//fichier de config du projet
	this->FichierConfigDefaut =ApplicationConfiguration::CONST_STATIC_XML_FILE; //fichier de configuration statique
	this->PathCores=ApplicationConfiguration::CONST_CORE_PATH;

	ctrlDown=false;
	shiftDown=false;
	ProcessUpdateEvent=true;
	needSave=false;
	pyeventmode=false;

	this->rootCore = NULL;
	this->rootResult = NULL;
	this->rootScene = NULL;
	this->propFrame = NULL;
	this->auiManager=NULL;
	this->mainFrame=NULL;
	this->GlFrame=NULL;
	this->pyShell=NULL;

	//Gestion fichier xml de configuration par défaut
	ApplicationConfiguration::LoadConfiguration(this->FichierConfigDefaut);

	//Gestion repertoire de projet
	wxFileName fname;
	if (wxDirExists(this->dossierCourant))
	{
		fname.AssignDir( this->dossierCourant );
	}else{
		//Le dossier /current/ n'existe pas, on le créé
		if (!fname.Mkdir( this->dossierCourant))
		{
			//On ne peut créer le dossier alors qu'il n'existe pas
			return;
		}
	}

}

void ProjectManager::AddLogMessage(const wxString& message,const wxColour* msgColor)
{
	if(msgColor)
		this->logControl->SetDefaultStyle(*msgColor);
	else
		this->logControl->SetDefaultStyle(*wxBLACK);
	this->logControl->AppendText(message);
	this->logControl->ShowPosition(this->logControl->GetLastPosition());

}

void ProjectManager::RefreshReportFolder()
{
	if(this->rootResult)
		dynamic_cast<E_Report*>(this->rootResult)->RefreshReportFolder();
}

void ProjectManager::CopyGlToFile()
{
	wxImage glImage;
	GlFrame->GetImage(glImage);
	if(glImage.IsOk())
	{
		wxFileDialog saveFileDialog( mainFrame, _("Export view 3D"), "PNG file (*.png)|*.png|JPG file (*.jpg)|*.jpg|Bitmap file (*.bmp)|*.bmp", "PNG file (*.png)|*.png|JPG file (*.jpg)|*.jpg|Bitmap file (*.bmp)|*.bmp", _("PNG file (*.png)|*.png|JPG file (*.jpg)|*.jpg|Bitmap file (*.bmp)|*.bmp"),
															wxFD_SAVE, wxDefaultPosition);

		if (saveFileDialog.ShowModal() == wxID_OK)
		{
			wxString filename(wxString(saveFileDialog.GetPath()));
			glImage.SaveFile(filename);
			wxLogMessage(_("Image saved successfully"));

		}
	}
	else
		wxLogError(_("Non-allowed image. Exportation cancelled"));
}

void ProjectManager::CopyGlToFileWithDim()
{
	wxImage glImage;
	wxSize sizeCapture;
	wxCustomEntryDialog entrydiag(mainFrame,_("Please give the size of the exported picture."),_("Export 3D View"));
	entrydiag.AddTextControl(_("Width"),"800");
	entrydiag.AddTextControl(_("Height"),"600");
	if(entrydiag.ShowModal()==wxID_OK)
	{
		std::vector<wxString> values;
		entrydiag.GetValues(values);
		sizeCapture.Set(Convertor::ToInt(values[0]),Convertor::ToInt(values[1]));
		GlFrame->GetImage(glImage,sizeCapture.GetWidth(),sizeCapture.GetHeight());
		if(glImage.IsOk())
		{
			wxFileDialog saveFileDialog( mainFrame, _("Export view 3D"), "PNG file (*.png)|*.png|JPG file (*.jpg)|*.jpg|Bitmap file (*.bmp)|*.bmp", "PNG file (*.png)|*.png|JPG file (*.jpg)|*.jpg|Bitmap file (*.bmp)|*.bmp", _("PNG file (*.png)|*.png|JPG file (*.jpg)|*.jpg|Bitmap file (*.bmp)|*.bmp"),
																wxFD_SAVE, wxDefaultPosition);

			if (saveFileDialog.ShowModal() == wxID_OK)
			{
				wxString filename(wxString(saveFileDialog.GetPath()));
				glImage.SaveFile(filename);
				wxLogMessage(_("Image saved successfully"));

			}
		}
		else
			wxLogError(_("Non-allowed image. Exportation cancelled"));
	}
}
void ProjectManager::ElementEvent(wxCommandEvent& eventElement,eventCtrl fromCtrl,Element* elementSelected)
{

	uiTreeCtrl* pCtrl;
	Element* eRoot;
	//On initialise les pointeurs vers les controles et élements concernées par l'événement
	switch(fromCtrl)
	{
		case EVENT_CTRL_CORE:
			pCtrl=this->treeCore;
			eRoot=this->rootCore;
			break;
		case EVENT_CTRL_SCENE:
			pCtrl=this->treeScene;
			eRoot=this->rootScene;
			break;
		case EVENT_CTRL_REPORT:
			pCtrl=this->treeResult;
			eRoot=this->rootResult;
			break;
		case EVENT_CTRL_USERCONFIG:
			pCtrl=this->treeUserPref;
			eRoot=this->rootUserConfig.get();
			break;
	}
	if(!elementSelected)
	{
		wxTreeItemId idElementToRename=pCtrl->GetSelection();
		if(idElementToRename.IsOk())
			elementSelected=pCtrl->GetElement(idElementToRename);
		else return;
	}

	//La validation de tout les menus de tous les popups des 3 arbres hierarchiques arrivent ici
	if(eventElement.GetId()>=Element::IDEVENT_LAST_FIXED)
	{
		this->SendPythonEvent(eventElement.GetId()-Element::IDEVENT_LAST_FIXED,elementSelected->GetXmlId());
	}else
	{
		Element::IDEVENT idEvenementElement=(Element::IDEVENT)eventElement.GetId();

		bool PassiveEvent=true;

		//EVENEMENT QUI N'IMPLIQUENT PAS UNE MODIFICATION DU PROJET
		switch(idEvenementElement)
		{
		case Element::IDEVENT_SELECT_TREE_ITEM:
			this->OnSelectElement(pCtrl,eRoot);
			break;
		case Element::IDEVENT_GETPROPERTIES:
			this->OnMenuGetProperties(pCtrl,eRoot,elementSelected);
			break;
		case Element::IDEVENT_SELECT_POSITION:
			this->OnAskSelectPosition();
			break;
		case Element::IDEVENT_COPIER:
			this->OnMenuCopy(pCtrl,eRoot);
			break;
		case Element::IDEVENT_LOAD_PARTICLE_SIMULATION_PATH:
		case Element::IDEVENT_LOAD_PARTICLE_SIMULATION:
			OnMenuLoadParticleSimulation(pCtrl,eRoot,elementSelected,idEvenementElement);
			break;
		case Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP:
		case Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP_SUM:
		case Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM:
			OnMenuLoadRecepteurssSimulation(pCtrl,eRoot,elementSelected,idEvenementElement);
			break;
		case Element::IDEVENT_LOAD_RECEPTEURSP_SIMULATION:
			OnMenuLoadRecepteurspSimulation(elementSelected,idEvenementElement);

		case Element::IDEVENT_OPEN_FOLDER:
			OnOpenFolder(elementSelected);
			break;
		default:
			PassiveEvent=false;
		};
		if(!PassiveEvent)
		{

			PushModificationToHistory();
			//EVENEMENT QUI IMPLIQUENT UNE MODIFICATION DU PROJET
			switch(idEvenementElement)
			{
			case  Element::IDEVENT_DELETE_ELEMENT:
					if(!pCtrl->GetEditControl()) //Si l'utilisateur n'est pas en train de modifier le libellé de l'élément
					{
						OnMenuDeleteElement(pCtrl,eRoot);
					}
					break;
			case Element::IDEVENT_RENAME_ELEMENT:
				this->OnMenuRenameElement(pCtrl,eRoot,elementSelected);
				break;
			case Element::IDEVENT_INVERT_FACE_ORIENTATION:
				OnMenuFlipFaces();
				break;
			case Element::IDEVENT_NEW_SURFACE_GROUP:
				this->OnMenuNewSurfaceGroup(elementSelected);
				break;
			case Element::IDEVENT_NEW_RECEPTEUR_P:
				this->OnMenuNewRecepteurPonctuel(elementSelected);
				break;
			case Element::IDEVENT_NEW_SOURCE:
				this->OnMenuNewSource(elementSelected);
				break;
			case Element::IDEVENT_NEW_SOURCE_GROUP:
				this->OnMenuNewSourceGroup(elementSelected);
				break;
			case Element::IDEVENT_NEW_RECEPTEUR_S_COUPE:
			case Element::IDEVENT_NEW_RECEPTEUR_S:
				this->OnMenuNewRecepteurSurfacique(elementSelected,idEvenementElement);
				break;
			case Element::IDEVENT_NEW_USERFREQ:
				this->OnMenuNewUserFreq(elementSelected);
				break;
			case Element::IDEVENT_NEW_ENCOMBREMENT:
				this->OnMenuNewEncombrement(elementSelected,Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT);
				break;
			case Element::IDEVENT_NEW_VOLUME:
				this->OnMenuNewVolume(elementSelected);
				break;
			case Element::IDEVENT_BUILD_VOLUMES_FROM_TRIMESH:
				this->OnFindSubVolumes(elementSelected);
				break;
			case Element::IDEVENT_NEW_ENCOMBREMENT_CUBOIDE:
				this->OnMenuNewEncombrement(elementSelected,Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE);
				break;
			case Element::IDEVENT_NEW_USERMAT:
				this->OnMenuNewUserMat(elementSelected);
				break;
			case Element::IDEVENT_RUN_CALCULATION:
				this->RunCoreCalculation(elementSelected);
				break;
			case Element::IDEVENT_NEW_MATERIAL_GROUP:
				this->OnMenuNewMatGroup(elementSelected);
				break;
			case Element::IDEVENT_COLLER:
				this->OnMenuPaste(pCtrl,elementSelected);
				break;
			case Element::IDEVENT_IMPORT_MATERIAL:
				OnMenuImportMaterial();
				break;
			case Element::IDEVENT_RELOAD_FOLDER:
				OnMenuReloadFolder(pCtrl,eRoot,elementSelected);
				break;
			case Element::IDEVENT_DELETE_FOLDER:
				OnMenuDeleteFolder(pCtrl,eRoot,elementSelected);
				break;
			case Element::IDEVENT_BFREQ_PRESELECTION_NONE:
			case Element::IDEVENT_BFREQ_PRESELECTION_THIRD_BAND:
			case Element::IDEVENT_BFREQ_PRESELECTION_BAND:
			case Element::IDEVENT_BFREQ_PRESELECTION_BUILDING_THIRD_BAND:
			case Element::IDEVENT_BFREQ_PRESELECTION_BUILDING_BAND:
			case Element::IDEVENT_BFREQ_PRESELECTION_ALL:
				OnMenuSetPreselection(elementSelected,idEvenementElement );
				break;
			case Element::IDEVENT_EMPTY_POINTER_VERTEX_GROUP:
				OnMenuClearSurfaceGroup(pCtrl,eRoot,elementSelected);
				break;
			case Element::IDEVENT_RECP_COMPUTE_ACOUSTIC_PARAMETERS:
				OnMenuDoAcousticParametersComputation(pCtrl,eRoot,elementSelected);
				break;
			case Element::IDEVENT_RECP_COMPUTE_ADVANCED_ACOUSTIC_PARAMETERS:
				OnMenuDoAdvancedAcousticParametersComputation(elementSelected);
				break;
			case Element::IDEVENT_RECEPTEURS_COMPUTE_EDT:
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"EDT");
				break;
			case Element::IDEVENT_RECEPTEURS_COMPUTE_TR:
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"TR15");
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"TR30");
				break;
			case Element::IDEVENT_RECEPTEURS_COMPUTE_CLARITY:
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"C50");
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"C80");
				break;
			case Element::IDEVENT_RECEPTEURS_COMPUTE_DEFINITION:
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"D50");
				break;
			case Element::IDEVENT_RECEPTEURS_COMPUTE_TS:
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"TS");
				break;
			case Element::IDEVENT_RECEPTEURS_COMPUTE_ST:
				this->OnMenuRecepteurSurfDoAcousticParametersComputation(pCtrl,eRoot,elementSelected,"ST");
				break;
			case Element::IDEVENT_NEW_RECEPTEURP_GROUP:
				this->OnMenuNewRecepteurPGroup(elementSelected);
				break;
			case Element::IDEVENT_NEW_SURFACE_GROUP_FROM_SELECTION:
				this->OnMenuCreateFGroupFromSelection();
				break;
			case Element::IDEVENT_CONVERT_VOL_TO_FITTING:
				this->OnConvertSubVolumeToFitting(elementSelected);
				break;
			case Element::IDEVENT_NEW_USERDIRECTIV:
				this->OnMenuNewUserDirectiv(elementSelected);
				break;
			default:
				wxLogDebug("Event not generated %i ",idEvenementElement);
			};
			this->GlFrame->RefreshElementDraw();
		}
	}
}

wxString ProjectManager::GetCurrentFolder() {
	return this->dossierCourant;
}

void ProjectManager::CloseApp()
{

	if(this->GlFrame)
	{
		this->GlFrame->StopSimulation();
		this->GlFrame->legendDrawer.Clear();
	}

	this->OnEmptySimulation();
	wxWindowUpdateLocker lockTreeCoreRefresh(this->treeCore);
	wxWindowUpdateLocker lockTreeSceneRefresh(this->treeScene);
	wxWindowUpdateLocker lockTreeResultRefresh(this->treeResult);
	//Supprime les liens pour un gain de temps en fermeture
	ApplicationConfiguration::DeleteAllMateriaux();


	delete this->rootCore;
	delete this->rootResult;
	delete this->rootScene;


	this->rootCore = NULL;
	this->rootResult = NULL;
	this->rootScene = NULL;



	ApplicationConfiguration::SetRootScene(NULL);
	Element::SetElementCount(InstanceManager::GetElementCount()+1);


	ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS=ApplicationConfiguration::t_App_Info();
	ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.volScene=-1;
	ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.surfScene=-1;

	propFrame->CloseElement();
}


ProjectManager::~ProjectManager()
{
	this->SaveUserPreferenceTree();
	this->rootUserConfig=(Element*)NULL;
	ApplicationConfiguration::OnApplicationClose();
	UnloadPyShell();
}
void ProjectManager::RunCoreCalculation(Element* coreCalculation)
{
	///////////////////////////////////////////
	///	Verification des paramètres de calcul
	///////////////////////////////////////////

	if(ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources==0)
	{
		wxLogError(_("Calculation requires, at least, one active sound source"));
		return;
	}

	if(this->sceneMesh.GetNumFaces()==0)
	{
		wxLogError(_("Your model doesn't have any face. Please create or import a model."));
		return;
	}

	//On active l'onglet des messages
	wxWindow* consoleWindow=mainFrame->FindWindowByName("console");
	if(consoleWindow)
	{
		wxAuiNotebook* consoleNoteBook=wxStaticCast(consoleWindow,wxAuiNotebook);
		consoleNoteBook->SetSelection(0);
	}

	Element* elWithFreq=coreCalculation->GetElementByType(Element::ELEMENT_TYPE_CORE_CORE_BFREQSELECTION);
	if(elWithFreq)
	{
		//On cherche un élément booléen à vrai
		std::vector<Element*> lstFreq;
		elWithFreq->GetAllElementByType(Element::ELEMENT_TYPE_BOOL_BFREQ,lstFreq);
		bool zeroFreq=true;
		for(int idfreq=0;idfreq<lstFreq.size();idfreq++)
		{
			E_Data_Bool_Bfreq* elFreq=dynamic_cast<E_Data_Bool_Bfreq*>(lstFreq[idfreq]);
			if(elFreq)
			{
				if(elFreq->GetValue())
				{
					zeroFreq=false;
					break;
				}
			}
		}
		if(zeroFreq)
		{
			wxLogError(_("Calculation requires do define a valid frequency band"));
			return;
		}
	}

	this->GlFrame->PauseSimulation(); //Pour limiter les ressources occupées par l'interface on arrête de raffraichir le rendu 3D
	///////////////////////////////////////////
	///	Initialisation des paramètres
	///////////////////////////////////////////
	wxDateTime timeDebOperation=wxDateTime::UNow();
	wxString modelFileName=coreCalculation->GetStringConfig("modelName");
	wxString corePath=coreCalculation->GetStringConfig("corePath");
	wxString reportFolderName=dossierCourant+ApplicationConfiguration::CONST_REPORT_FOLDER_PATH;
	wxString xmlCoreFileName="config.xml";
	wxString exeName=coreCalculation->GetStringConfig("exeName");
	wxString tetraFileName=coreCalculation->GetStringConfig("tetrameshFileName");
	wxString rootCorePath=this->PathCores+corePath;

	///////////////////////////////////////////
	//Ajout du dossier daté de résultat
	///////////////////////////////////////////
	wxDateTime maintenant=wxDateTime::UNow();
	wxString folderDated=maintenant.Format("%Y-%m-%d_%Hh%Mm%Ss");
	if(!wxDirExists(reportFolderName))
		wxMkdir(reportFolderName);
	reportFolderName+=coreCalculation->GetTreeLabel();
	if(!wxDirExists(reportFolderName))
		wxMkdir(reportFolderName);
	reportFolderName+=wxFileName::GetPathSeparator()+folderDated+wxFileName::GetPathSeparator();
	if(!wxFileExists(reportFolderName))
		wxMkdir(reportFolderName);
	wxString workingDir=reportFolderName;//this->PathCores+corePath+tempFolder;
	ApplicationConfiguration::GLOBAL_VAR.workingFolderPath = workingDir;
	///////////////////////////////////////////
	///	Verifications de l'existance du coeur de calcul
	///////////////////////////////////////////

    wxFileName corefilename(exeName);
    wxString ext = corefilename.GetExt();

    #ifdef _WIN32
    if(ext=="") {
        exeName += ".exe";
        corefilename = wxFileName(exeName);
        ext = "exe";
    }
    #endif
	if(!wxFileExists(this->PathCores+corePath+exeName))
	{
		wxLogError(_("Calculation program file not found."));
		return;
	}
	///////////////////////////////////////////
	//  Maillage de la scène
	///////////////////////////////////////////
	if(!this->RunCoreMaillage(coreCalculation))
	{
		wxLogError(_("Calculation requires a scene meshing"));
		return;
	}



	///////////////////////////////////////////
	///	On exporte les fichiers utilisés par le code de calcul
	///////////////////////////////////////////
	if(!wxDirExists(workingDir))
		wxMkdir(workingDir);
	if(!this->sceneMesh.Save((workingDir+modelFileName).ToStdString())) {
		wxLogError(_("Error while saving the 3d model"));
		return;
	}
	if(!tetraFileName.empty())
		this->sceneMesh.SaveMaillage((workingDir+tetraFileName).ToStdString(),true);
	wxXmlDocument xmlCoreDocument;
	wxXmlNode* rootConfig = new wxXmlNode(wxXML_ELEMENT_NODE,"configuration");

	LastComputationFolder=workingDir;
	rootConfig->AddAttribute("workingdirectory",workingDir);


	xmlCoreDocument.SetRoot(rootConfig);
	rootScene->SaveXMLCoreDoc(rootConfig);
	coreCalculation->SaveXMLCoreDoc(rootConfig);
	xmlCoreDocument.SetFileEncoding("iso-8859-1"); //tant que libinterface ne gère par l'utf-8 on utilise ca
	xmlCoreDocument.Save(workingDir+xmlCoreFileName);

	///////////////////////////////////////////
	///	Pour ne pas perdre les modifications en cas de crash, on sauvegarde le projet (sans écraser le fichier de projet)
	///////////////////////////////////////////
	this->UpdateXmlFile();

	///////////////////////////////////////////
	///	On execute le logiciel de calcul
	///////////////////////////////////////////

	//Declaration du processus
	wxString cmd;
	//On attend que l'execution soit terminée
	bool hasOutput=true;
	wxProgressDialog progDialog(_("Calculation execution"),_("Calculation in progress..."),10000,mainFrame,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );

	wxString labelOutput=exeName+" :";

	wxDateTime timeDebCalculation=wxDateTime::UNow();

	if(ext=="py" || ext=="pyc") {

	#ifdef _WIN32
		cmd = ApplicationConfiguration::getResourcesFolder() + "python.exe -u \"" + rootCorePath + exeName + "\" \"" + workingDir + xmlCoreFileName + "\"";
	#else
		cmd = "python -u \"" + rootCorePath + exeName + "\" \"" + workingDir + xmlCoreFileName + "\"";
	#endif // _WIN32

    }
    else {
        cmd = rootCorePath + exeName + " \"" + workingDir + xmlCoreFileName + "\"";
    }

    bool result = uiRunExe(mainFrame, cmd, labelOutput, &progDialog);
	wxLongLong durationCalculation=wxDateTime::UNow().GetValue()-timeDebCalculation.GetValue();

	wxLogMessage(_("Calculation time: %lld ms"),durationCalculation.GetValue());

	///////////////////////////////////////////
	// Copie du fichier de projet XML dans le dossier de rapport de calcul
	///////////////////////////////////////////
	wxLogMessage(_("Copy of calculation parameters"));
	this->UpdateXmlFile(reportFolderName+wxFileName::GetPathSeparator());


	///////////////////////////////////////////
	///	On ajoute un enregistrement des résultats de calcul dans les éléments
	///////////////////////////////////////////
	wxLogMessage(_("Refreshing onglet 'Report'"));

    bool resetHistoryBackup = false;
    // Refresh of report folder is not a user action and should not trigger tree backup
    if(rootUserConfig->GetElementByLibelle("mainpref")->GetElementByLibelle("history")->GetBoolConfig("keep_modification_history")) {
        rootUserConfig->GetElementByLibelle("mainpref")->GetElementByLibelle("history")->UpdateBoolConfig("keep_modification_history", false);
        resetHistoryBackup = true;
    }

	RefreshReportFolder();

    if(resetHistoryBackup) {
        rootUserConfig->GetElementByLibelle("mainpref")->GetElementByLibelle("history")->UpdateBoolConfig("keep_modification_history", true);
    }

	wxLongLong durationOperation=wxDateTime::UNow().GetValue()-timeDebOperation.GetValue();
	wxLogMessage(_("Total time calculation: %lld ms"),durationOperation.GetValue());


}


void ProjectManager::OnMenuNewSurfaceGroup(Element* searchResult)
{
	Element* newGroupe=searchResult->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE);
	newGroupe->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newGroupe->GetElementInfos().idElement);
		this->OnMenuRenameElement(treeScene,this->rootScene,newGroupe);
	}
}


void ProjectManager::OnMenuNewSourceGroup(Element* elementSelected)
{
	Element* sourcesfolder=elementSelected->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_SOURCES);
	sourcesfolder->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(sourcesfolder->GetElementInfos().idElement);
		this->OnMenuRenameElement(this->treeScene,rootScene,sourcesfolder);
	}
}

void ProjectManager::OnMenuNewSource(Element* searchResult)
{
	Element* newSource=searchResult->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE);
	newSource->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->Expand(searchResult->GetElementInfos().idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newSource->GetElementInfos().idElement);
		this->OnMenuRenameElement(this->treeScene,rootScene,newSource);
	}
}

void ProjectManager::OnMenuNewVolume(Element* elementSelected)
{
	Element* newel=elementSelected->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME);
	newel->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->Expand(elementSelected->GetElementInfos().idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newel->GetElementInfos().idElement);
		this->OnMenuRenameElement(this->treeScene,this->rootScene,newel);
	}
}
Element* ProjectManager::OnMenuNewEncombrement(Element* elementSelected,Element::ELEMENT_TYPE typeEl)
{
	Element* newSon=elementSelected->AppendFilsByType(typeEl);
	if(newSon!=NULL)
	{
		newSon->FillWxTree(this->treeScene);
		if(!pyeventmode)
		{
			this->treeScene->Expand(elementSelected->GetElementInfos().idElement);
			this->treeScene->UnselectAll();
			this->treeScene->SelectItem(newSon->GetElementInfos().idElement);
			this->OnMenuRenameElement(this->treeScene,this->rootScene,newSon);
		}
	}
	return newSon;
}

void ProjectManager::OnMenuImportMaterial()
{
	wxString FileName;
	if(!pyeventmode)
	{
		wxFileDialog openFileDialog( mainFrame, _("Import material from file"), "CATT-Acoustic file (*.txt)|*.txt|li8 Odeon file (*.li8)|*.li8", "CATT-Acoustic file (*.txt)|*.txt|li8 Odeon file (*.li8)|*.li8", _("CATT-Acoustic file (*.txt)|*.txt|li8 Odeon file (*.li8)|*.li8"),
															wxFD_OPEN, wxDefaultPosition);

		if (openFileDialog.ShowModal() == wxID_OK)
		{
			FileName=openFileDialog.GetPath();
			//On appel la méthode
		}else{
			return;
		}
	}else{
		GetParameter("path",FileName);
	}
	ImportMaterial(FileName);
}

void ProjectManager::ImportMaterial(wxString fromFile)
{
	wxFileName nomDeFichier(fromFile);
	if(nomDeFichier.FileExists())
	{
		wxString ext=nomDeFichier.GetExt().Lower();


		if(ext=="li8")
		{
			ImportMaterialOdeon(fromFile);
		}else if(ext=="txt")
		{
			ImportMaterialCatt(fromFile);
		}else{
			wxLogMessage(_("Unknown file format"));
		}
	}else{
		wxLogError(_("File does not exist"));
	}
}

void ProjectManager::ImportMaterialCatt(wxString fromFile)
{
	wxFileName nomDeFichier(fromFile);
	wxLogMessage(_("Import from CATT-Acoustic file"));

	//Ajout du groupe dans l'arbre du projet
	Element* searchResult=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER);
	if(searchResult==NULL)
		return;
	E_Scene_Bdd_Materiaux_User* elfolder=dynamic_cast<E_Scene_Bdd_Materiaux_User*>(searchResult);
	E_Scene_Bdd_Materiaux_User_Group* nvGroup=elfolder->AppendUserGroup(nomDeFichier.GetFullName());

	//Ouverture du fichier
	BigFileReader lecteur(fromFile,10000);

	//Attention utilisations d'expressions régulières ( mal de tête en perspective ), mot clé: regex

	//Fonctionne quelque soit le nombre de bande de frequence
	wxRegEx generalMatHeader(wxT("(.*) = <(([0-9]* ?)*)>( L )?(<(([0-9]* ?)*)>)?"));

	wxRegEx matInfos(wxT("(.*):(.*)"));

	wxString ligne=lecteur.GetLine();
	while(lecteur.IsOk())
	{
		//Nouveau matériau

		ligne.Trim(false); //enleve les espaces au debut

		if(ligne[0]!=59 && !ligne.IsEmpty()) //Si c'est un commentaire, ou ligne vide on passe
		{
			//La construction doit être
			std::vector<float> valAbs;
			std::vector<float> valDiffu;

			//Si la ligne correspond à l'expression régulière
			if(generalMatHeader.Matches(ligne))
			{
				E_Scene_Bdd_Materiaux_User_Group* nouvGroup=nvGroup;
				wxString absName= generalMatHeader.GetMatch(ligne,1);
				wxString chaineAbsorption=generalMatHeader.GetMatch(ligne,2);
				wxString chaineDiffusion=generalMatHeader.GetMatch(ligne,6);

				wxArrayString tabAbs;
				wxArrayString tabDiff;
				wxString categoryMat;
				wxString descriptionMat;
				wxString referenceMat;
				SplitString(chaineAbsorption," ",tabAbs);
				SplitString(chaineDiffusion," ",tabDiff);
				//Lecture des propriétés du matériau
				ligne=lecteur.GetLine();
				while(ligne.IsEmpty() && lecteur.IsOk())
					ligne=lecteur.GetLine();
				while(!ligne.IsEmpty() && lecteur.IsOk())
				{
					ligne.Trim(false); //enleve les espaces au debut
					if(matInfos.Matches(ligne))
					{
						wxString label=matInfos.GetMatch(ligne,1);
						wxString value=matInfos.GetMatch(ligne,2);
						if(label=="C")
							categoryMat=value;
						else if(label=="D")
							descriptionMat=value;
						else if(label=="R")
						{
							referenceMat=value;
							break;
						}
						else break;
						ligne=lecteur.GetLine();
						while(ligne.IsEmpty() && lecteur.IsOk())
							ligne=lecteur.GetLine();
					}else{
						break;
					}
				}
				//On recherche si le matériau a une catégorie
				if(categoryMat.Trim()!="")
				{
					//Si oui on recherche si cette catégorie a déjà été créé
					Element* tmpGroup=nvGroup->GetElementByLibelle(categoryMat);
					if(tmpGroup && tmpGroup->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_GROUP)
					{
						nouvGroup=dynamic_cast<E_Scene_Bdd_Materiaux_User_Group*>(tmpGroup);
					}else{
						nouvGroup=nvGroup->AppendUserGroup(categoryMat);
					}
				}
				//Ajout du matériau dans le groupe
				E_Scene_Bdd_Materiaux_User_Materiau* importedMaterial=nouvGroup->AppendUserMateriau(absName);

				//Maj des propriétés du matériau
				importedMaterial->UpdateDescriptionValue(descriptionMat);
				importedMaterial->UpdateReferenceValue(referenceMat);

				if(tabAbs.size()>=6)
				{
					//*************************************************
					//		Valeurs d'absorption
					// 125 Hz
					float absValue=Convertor::ToFloat(tabAbs[0])/100;
					importedMaterial->UpdateAbsorptionValue(50,absValue);
					importedMaterial->UpdateAbsorptionValue(63,absValue);
					importedMaterial->UpdateAbsorptionValue(80,absValue);
					importedMaterial->UpdateAbsorptionValue(100,absValue);
					importedMaterial->UpdateAbsorptionValue(125,absValue);
					importedMaterial->UpdateAbsorptionValue(160,absValue);
					// 250 Hz
					absValue=Convertor::ToFloat(tabAbs[1])/100;
					importedMaterial->UpdateAbsorptionValue(200,absValue);
					importedMaterial->UpdateAbsorptionValue(250,absValue);
					importedMaterial->UpdateAbsorptionValue(315,absValue);
					// 500 Hz
					absValue=Convertor::ToFloat(tabAbs[2])/100;
					importedMaterial->UpdateAbsorptionValue(400,absValue);
					importedMaterial->UpdateAbsorptionValue(500,absValue);
					importedMaterial->UpdateAbsorptionValue(630,absValue);
					// 1000 Hz
					absValue=Convertor::ToFloat(tabAbs[3])/100;
					importedMaterial->UpdateAbsorptionValue(800,absValue);
					importedMaterial->UpdateAbsorptionValue(1000,absValue);
					importedMaterial->UpdateAbsorptionValue(1250,absValue);
					// 2000 Hz
					absValue=Convertor::ToFloat(tabAbs[4])/100;
					importedMaterial->UpdateAbsorptionValue(1600,absValue);
					importedMaterial->UpdateAbsorptionValue(2000,absValue);
					importedMaterial->UpdateAbsorptionValue(2500,absValue);
					// 4000 Hz
					absValue=Convertor::ToFloat(tabAbs[5])/100;
					importedMaterial->UpdateAbsorptionValue(3150,absValue);
					importedMaterial->UpdateAbsorptionValue(4000,absValue);
					importedMaterial->UpdateAbsorptionValue(5000,absValue);
					// 8000 Hz
					if(tabAbs.size()==8)
						absValue=Convertor::ToFloat(tabAbs[6])/100;
					importedMaterial->UpdateAbsorptionValue(6300,absValue);
					importedMaterial->UpdateAbsorptionValue(8000,absValue);
					importedMaterial->UpdateAbsorptionValue(10000,absValue);
					// 16000 Hz
					if(tabAbs.size()==8)
						absValue=Convertor::ToFloat(tabAbs[7])/100;
					importedMaterial->UpdateAbsorptionValue(12500,absValue);
					importedMaterial->UpdateAbsorptionValue(16000,absValue);
					importedMaterial->UpdateAbsorptionValue(20000,absValue);

				}
				if(tabDiff.size()>=6)
				{
					// 125 Hz
					float diffValue=Convertor::ToFloat(tabDiff[0])/100;
					importedMaterial->UpdateDiffusionValue(50,diffValue);
					importedMaterial->UpdateDiffusionValue(63,diffValue);
					importedMaterial->UpdateDiffusionValue(80,diffValue);
					importedMaterial->UpdateDiffusionValue(100,diffValue);
					importedMaterial->UpdateDiffusionValue(125,diffValue);
					importedMaterial->UpdateDiffusionValue(160,diffValue);
					// 250 Hz
					diffValue=Convertor::ToFloat(tabDiff[1])/100;
					importedMaterial->UpdateDiffusionValue(200,diffValue);
					importedMaterial->UpdateDiffusionValue(250,diffValue);
					importedMaterial->UpdateDiffusionValue(315,diffValue);
					// 500 Hz
					diffValue=Convertor::ToFloat(tabDiff[2])/100;
					importedMaterial->UpdateDiffusionValue(400,diffValue);
					importedMaterial->UpdateDiffusionValue(500,diffValue);
					importedMaterial->UpdateDiffusionValue(630,diffValue);
					// 1000 Hz
					diffValue=Convertor::ToFloat(tabDiff[3])/100;
					importedMaterial->UpdateDiffusionValue(800,diffValue);
					importedMaterial->UpdateDiffusionValue(1000,diffValue);
					importedMaterial->UpdateDiffusionValue(1250,diffValue);
					// 2000 Hz
					diffValue=Convertor::ToFloat(tabDiff[4])/100;
					importedMaterial->UpdateDiffusionValue(1600,diffValue);
					importedMaterial->UpdateDiffusionValue(2000,diffValue);
					importedMaterial->UpdateDiffusionValue(2500,diffValue);
					// 4000 Hz
					diffValue=Convertor::ToFloat(tabDiff[5])/100;
					importedMaterial->UpdateDiffusionValue(3150,diffValue);
					importedMaterial->UpdateDiffusionValue(4000,diffValue);
					importedMaterial->UpdateDiffusionValue(5000,diffValue);
					// 8000 Hz
					if(tabDiff.size()==8)
						diffValue=Convertor::ToFloat(tabDiff[6])/100;
					importedMaterial->UpdateDiffusionValue(6300,diffValue);
					importedMaterial->UpdateDiffusionValue(8000,diffValue);
					importedMaterial->UpdateDiffusionValue(10000,diffValue);
					// 16000 Hz
					if(tabDiff.size()==8)
						diffValue=Convertor::ToFloat(tabDiff[7])/100;
					importedMaterial->UpdateDiffusionValue(12500,diffValue);
					importedMaterial->UpdateDiffusionValue(16000,diffValue);
					importedMaterial->UpdateDiffusionValue(20000,diffValue);
				}
				wxLogMessage(_("Importation of material %s"),absName);
				mainFrame->Update();
				ligne=lecteur.GetLine();
			}else{
				ligne=lecteur.GetLine();
			}
		}else{
			ligne=lecteur.GetLine();
		}
	}

	elfolder->FillWxTree(treeScene,wxTreeItemId());


	//Informations déstinné au traducteur
	_("Chairs");
	_("Audiences");
	_("Walls");
	_("Window");
	_("Floors");



}

void ProjectManager::ImportMaterialOdeon(wxString fromFile)
{
	wxLogMessage(_("Import from Odeon file"));
	wxFileName nomDeFichier(fromFile);

	//Ajout du groupe dans l'arbre du projet
	Element* searchResult=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER);
	if(searchResult==NULL)
		return;
	E_Scene_Bdd_Materiaux_User* elfolder=dynamic_cast<E_Scene_Bdd_Materiaux_User*>(searchResult);
	E_Scene_Bdd_Materiaux_User_Group* nvGroup=elfolder->AppendUserGroup(nomDeFichier.GetFullName());

	//Ouverture du fichier
	BigFileReader lecteur(fromFile,10000);
	wxString ligne=lecteur.GetLine();
	stringParser lineParser;
	while(lecteur.IsOk())
	{
		if(ligne!="")
		{
			lineParser.Parse(ligne);
			int idMat=lineParser.GetNextInteger();
			wxString absName=Convertor::ToString(idMat);
			wxString descriptionMat=lineParser.GetNextStringLine();
			descriptionMat.Trim(false);
			descriptionMat.Trim();

			//Recuperation des bandes de frequences des absorptions
			ligne=lecteur.GetLine();
			while(ligne.IsEmpty() && lecteur.IsOk())
				ligne=lecteur.GetLine();
			lineParser.Parse(ligne);
			std::vector<float> tabAbs;
			while(!lineParser.IsEmpty())
				tabAbs.push_back(lineParser.GetNextFloat());
			if(tabAbs.size()==8)
			{
				wxLogMessage(_("Importation of material %s"),absName);
				//Ajout du matériau dans le groupe
				E_Scene_Bdd_Materiaux_User_Materiau* importedMaterial=nvGroup->AppendUserMateriau(absName);
				//*************************************************
				//		Propriétés
				importedMaterial->UpdateDescriptionValue(descriptionMat);
				//*************************************************
				//		Valeurs d'absorption
				// 63 Hz
				float absValue=tabAbs[0];
				importedMaterial->UpdateAbsorptionValue(50,absValue);
				importedMaterial->UpdateAbsorptionValue(63,absValue);
				importedMaterial->UpdateAbsorptionValue(80,absValue);
				// 125 Hz
				absValue=tabAbs[1];
				importedMaterial->UpdateAbsorptionValue(100,absValue);
				importedMaterial->UpdateAbsorptionValue(125,absValue);
				importedMaterial->UpdateAbsorptionValue(160,absValue);
				// 250 Hz
				absValue=tabAbs[2];
				importedMaterial->UpdateAbsorptionValue(200,absValue);
				importedMaterial->UpdateAbsorptionValue(250,absValue);
				importedMaterial->UpdateAbsorptionValue(315,absValue);
				// 500 Hz
				absValue=tabAbs[3];
				importedMaterial->UpdateAbsorptionValue(400,absValue);
				importedMaterial->UpdateAbsorptionValue(500,absValue);
				importedMaterial->UpdateAbsorptionValue(630,absValue);
				// 1000 Hz
				absValue=tabAbs[4];
				importedMaterial->UpdateAbsorptionValue(800,absValue);
				importedMaterial->UpdateAbsorptionValue(1000,absValue);
				importedMaterial->UpdateAbsorptionValue(1250,absValue);
				// 2000 Hz
				absValue=tabAbs[5];
				importedMaterial->UpdateAbsorptionValue(1600,absValue);
				importedMaterial->UpdateAbsorptionValue(2000,absValue);
				importedMaterial->UpdateAbsorptionValue(2500,absValue);
				// 4000 Hz
				absValue=tabAbs[6];
				importedMaterial->UpdateAbsorptionValue(3150,absValue);
				importedMaterial->UpdateAbsorptionValue(4000,absValue);
				importedMaterial->UpdateAbsorptionValue(5000,absValue);
				// 8000 Hz
				absValue=tabAbs[7];
				importedMaterial->UpdateAbsorptionValue(6300,absValue);
				importedMaterial->UpdateAbsorptionValue(8000,absValue);
				importedMaterial->UpdateAbsorptionValue(10000,absValue);
				// 8000 Hz -> 20000 Hz
				importedMaterial->UpdateAbsorptionValue(12500,absValue);
				importedMaterial->UpdateAbsorptionValue(16000,absValue);
				importedMaterial->UpdateAbsorptionValue(20000,absValue);
				mainFrame->Update();
			}
		}
		ligne=lecteur.GetLine();
	}
	elfolder->FillWxTree(treeScene,wxTreeItemId());

}
void ProjectManager::OnMenuNewUserFreq(Element* searchResult)
{
	Element* newSon=searchResult->AppendFilsByType(Element::ELEMENT_TYPE_GAMMEFREQ_USER);
	newSon->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->Expand(searchResult->GetElementInfos().idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newSon->GetElementInfos().idElement);
		this->OnMenuRenameElement(treeScene,this->rootScene,newSon);
	}
}

void ProjectManager::OnMenuNewUserDirectiv(Element* searchResult)
{
	Element* newSon = searchResult->AppendFilsByType(Element::ELEMENT_TYPE_DIRECTIVITIES_USER);
	newSon->FillWxTree(this->treeScene);
	if (!pyeventmode)
	{
		this->treeScene->Expand(searchResult->GetElementInfos().idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newSon->GetElementInfos().idElement);
		this->OnMenuRenameElement(treeScene, this->rootScene, newSon);
	}
}

void ProjectManager::OnMenuNewMatGroup(Element* searchResult)
{
	Element::t_elementInfo searchResultInfo=searchResult->GetElementInfos();
	E_Scene_Bdd_Materiaux_User_Group* newSon;
	if(searchResultInfo.typeElement==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER)
	{
		E_Scene_Bdd_Materiaux_User* elfolder=dynamic_cast<E_Scene_Bdd_Materiaux_User*>(searchResult);
		newSon=elfolder->AppendUserGroup();
	}else if(searchResultInfo.typeElement==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_GROUP){
		E_Scene_Bdd_Materiaux_User_Group* elfolder=dynamic_cast<E_Scene_Bdd_Materiaux_User_Group*>(searchResult);
		newSon=elfolder->AppendUserGroup();
	}
	searchResult->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->Expand(searchResultInfo.idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newSon->GetElementInfos().idElement);
		this->OnMenuRenameElement(treeScene,this->rootScene,newSon);
	}
}
void ProjectManager::OnMenuNewUserMat(Element* searchResult)
{
	Element::t_elementInfo searchResultInfo=searchResult->GetElementInfos();
	E_Scene_Bdd_Materiaux_User_Materiau* newSon;
	if(searchResultInfo.typeElement==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER)
	{
		E_Scene_Bdd_Materiaux_User* elfolder=dynamic_cast<E_Scene_Bdd_Materiaux_User*>(searchResult);
		newSon=elfolder->AppendUserMateriau();
	}else if(searchResultInfo.typeElement==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_GROUP){
		E_Scene_Bdd_Materiaux_User_Group* elfolder=dynamic_cast<E_Scene_Bdd_Materiaux_User_Group*>(searchResult);
		newSon=elfolder->AppendUserMateriau();
	}
	searchResult->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->Expand(searchResultInfo.idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newSon->GetElementInfos().idElement);
		this->OnMenuRenameElement(treeScene,this->rootScene,newSon);
	}
}


void ProjectManager::OnMenuCopy(uiTreeCtrl* fromCtrl,Element* eRoot)
{
	wxXmlDocument tmpDocXml;
	wxXmlNode* xmlRoot=new wxXmlNode(wxXML_ELEMENT_NODE,"root");
	tmpDocXml.SetRoot(xmlRoot);

	//Recuperation du wxTreeItemId
	wxArrayTreeItemIds multiselection;
	fromCtrl->GetSelections(multiselection);
	for(int idsel=0;idsel<multiselection.size();idsel++)
	{
		wxTreeItemId idElement=multiselection[idsel];
		if(idElement.IsOk())
		{
			//Recuperation de l'élément
			Element* elementTarget=fromCtrl->GetElement(idElement);
			if(elementTarget!=NULL)
			{
				elementTarget->ExportToXMLDoc(xmlRoot);
			}
		}
	}
	if(tmpDocXml.IsOk())
	{
		wxStringOutputStream tmpStr;
		tmpDocXml.Save(tmpStr);
		wxTextDataObject* data=new wxTextDataObject;
		data->SetText(tmpStr.GetString());
        wxTheClipboard->Open();
		wxTheClipboard->SetData(data);
		wxTheClipboard->Flush();
		wxTheClipboard->Close();
	}
}
void ProjectManager::OnMenuPaste(uiTreeCtrl* fromCtrl,Element* elementTarget)
{
	wxTextDataObject data;
	wxTheClipboard->GetData(data);
	wxStringInputStream tmpStr(data.GetText());
	wxXmlDocument tmpDocXml(tmpStr);
	if(tmpDocXml.IsOk())
	{
		//On initialise les XMLid des éléments à importer
		wxXmlNode* nodeElement=tmpDocXml.GetRoot();
		//Appel du comportement local de l'élément
		elementTarget->OnPaste(nodeElement);
	}
	fromCtrl->Expand(elementTarget->GetElementInfos().idElement);
}

void ProjectManager::OnMenuNewRecepteurSurfacique(Element* searchResult,const Element::IDEVENT& evtype)
{
	Element::ELEMENT_TYPE typetoadd=Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR;
	if(evtype==Element::IDEVENT_NEW_RECEPTEUR_S_COUPE)
		typetoadd=Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE;
	if(searchResult->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS)
	{
		Element* newRecepteur=searchResult->AppendFilsByType(typetoadd);
		if(!newRecepteur)
			return;
		newRecepteur->FillWxTree(this->treeScene);
		if(!pyeventmode)
		{
			this->treeScene->Expand(searchResult->GetElementInfos().idElement);
			this->treeScene->UnselectAll();
			this->treeScene->SelectItem(newRecepteur->GetElementInfos().idElement);
			this->OnMenuRenameElement(treeScene,this->rootScene,newRecepteur);
		}
	}
}



void ProjectManager::OnMenuNewRecepteurPGroup(Element* searchResult)
{
	Element* newRecepteurpgroup=searchResult->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_RECEPTEURSP);
	newRecepteurpgroup->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->Expand(searchResult->GetElementInfos().idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newRecepteurpgroup->GetElementInfos().idElement);
		this->OnMenuRenameElement(treeScene,this->rootScene,newRecepteurpgroup);
	}
}

void ProjectManager::OnMenuNewRecepteurPonctuel(Element* searchResult)
{
	Element* newRecepteur=searchResult->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR);
	newRecepteur->FillWxTree(this->treeScene);
	if(!pyeventmode)
	{
		this->treeScene->Expand(searchResult->GetElementInfos().idElement);
		this->treeScene->UnselectAll();
		this->treeScene->SelectItem(newRecepteur->GetElementInfos().idElement);
		this->OnMenuRenameElement(treeScene,this->rootScene,newRecepteur);
	}
}

void ProjectManager::OnSaveConsoleToFile(wxString fileName)
{
	if(logControl!=NULL)
		this->logControl->SaveFile(fileName,wxFILE_KIND_DISK);
}

void ProjectManager::OnSaveShellToFile(wxString fileName)
{
	if(shellControl!=NULL)
		this->shellControl->SaveFile(fileName,wxFILE_KIND_DISK);
}
void ProjectManager::OnClearConsole()
{
	if(logControl!=NULL)
		this->logControl->Clear();
}
void ProjectManager::OnMenuDeleteElement(uiTreeCtrl* fromCtrl,Element* eRoot,bool askConfirmation,wxString confirmationLabel)
{
	wxArrayTreeItemIds multiselection;
	fromCtrl->GetSelections(multiselection);
	for(int idsel=0;idsel<multiselection.size();idsel++)
	{
		wxTreeItemId idElement=multiselection[idsel];
		Element* elementEvent=fromCtrl->GetElement(idElement);
		if(elementEvent)
		{
			// Si cet élément peut être détruit par l'utilisateur
			if(elementEvent->GetElementInfos().userDestroyable)
			{

				if(askConfirmation)
				{
					wxMessageDialog dialog( mainFrame, confirmationLabel,
						this->mainFrame->GetLabel(), wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
					if( dialog.ShowModal()==wxID_NO)
							return;
				}
				eRoot->DeleteElementByTreeId(elementEvent->GetElementInfos().idElement);
			}
		}
	}
}

void ProjectManager::OnMenuRenameElement(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementToRename)
{
	//Recuperation du wxTreeItemId
	if(!pyeventmode)
	{
		fromCtrl->EditLabel(elementToRename->GetElementInfos().idElement);
	}else{
		wxString newlabel;
		wxTreeEvent tevt;
		if(this->GetParameter("name",newlabel))
		{
			fromCtrl->SetItemText(elementToRename->GetElementInfos().idElement,newlabel);
			tevt.SetLabel(newlabel);
			elementToRename->OnEndLabelEdit(tevt);
		}
	}
}

void ProjectManager::OpenPropertyElement(Element* elementToOpen)
{

	if(elementToOpen->IsContainsProperties())
	{
		this->propFrame->LoadElement(elementToOpen);
		wxString parentLabel="";
		Element* parent=elementToOpen->GetElementParent();
		if(parent)
		{
			parentLabel=wxString::Format(" (%s)",_(parent->GetTreeLabel()));
		}
		this->auiManager->GetPane(this->propFrame).Caption(_("Properties - ")+wxString(_(elementToOpen->GetTreeLabel()))+parentLabel);
		this->auiManager->Update(); //maj des modifications
	}else if(elementToOpen->IsFittingWithThisType(Element::ELEMENT_TYPE_REPORT_GABE))
	{
		this->OpenNewDataWindow(elementToOpen);
	}else if(elementToOpen->IsFittingWithThisType(Element::ELEMENT_TYPE_REPORT_UNKNOWN))
	{
		this->OpenOsExternalApplication(elementToOpen);
	}
}

void ProjectManager::OnOpenFolder(Element* folder)
{
	E_Report_Folder* elfolder=dynamic_cast<E_Report_Folder*>(folder);
	if(elfolder)
	{
		elfolder->OpenFolder();
	}
}
void ProjectManager::OnMenuLoadRecepteurspSimulation(Element* elRpRecord,Element::IDEVENT loadingMethod)
{

	//Arret de la simulation
	this->GlFrame->StopSimulation();
	//Recuperation du wxTreeItemId
	//Conversion en élément de rapport de simulation
	E_Report_File* reportEle=dynamic_cast<E_Report_File*>(elRpRecord);
	if(reportEle)
	{
		wxString fileToLoad;
		reportEle->BuildFullPath(fileToLoad);

		//On verifie que le fichier(s) existe
		if(wxFileExists(fileToLoad))
		{
			recepteursPContainer.LoadRecepteursPFile(fileToLoad,sceneMesh.UnitizeVar);
			recepteursPContainer.EnableRendering();
		}else{
			wxLogError(_("File does not exist"));
			return;
		}
	}
	//Demmarage de la simulation
	this->GlFrame->RunSimulation();
}
void ProjectManager::OnMenuLoadRecepteurssSimulation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementWithProp,Element::IDEVENT loadingMethod)
{
	wxDateTime AnimatorLoading=wxDateTime::UNow();
	//Arret de la simulation
	this->GlFrame->StopSimulation();
	//Recuperation du wxTreeItemId
	//Conversion en élément de rapport de simulation
	E_Report_RecepteurSurfaciqueVisualisation* reportEle=dynamic_cast<E_Report_RecepteurSurfaciqueVisualisation*>(elementWithProp);
	if(reportEle)
	{
		wxString fileToLoad;
		reportEle->BuildFullPath(fileToLoad);
		wxArrayString pathRssFile;
		pathRssFile.push_back(fileToLoad);
		//On verifie que le fichier(s) existe
		if(wxFileExists(fileToLoad))
		{
			recepteursSContainer.LoadRecepteursSFile(pathRssFile,sceneMesh.UnitizeVar,reportEle,loadingMethod);
			recepteursSContainer.UpdateRecepteurSSConfig();
		}else{
			wxLogError(_("Surface receivers file does not exist for the corresponding frequency"));
		}
	}
	//Demmarage de la simulation
	Element* simulationEl=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_PARTICULES);
	this->GlFrame->RunSimulation();
	//#ifdef _DEBUG
	//wxLogDebug("Total Chargement en %i ms",wxDateTime::UNow().GetValue()-AnimatorLoading.GetValue());
	//#endif
}
void ProjectManager::OnMenuLoadParticleSimulation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementWithProp,Element::IDEVENT loadingMethod)
{
	//Arret de la simulation
	this->GlFrame->StopSimulation();
	//Recuperation du wxTreeItemId

	//Conversion en élément de rapport de simulation
	E_Report_File* reportEle=dynamic_cast<E_Report_File*>(elementWithProp);
	if(reportEle)
	{
		wxString pathPartFile;
		reportEle->BuildFullPath(pathPartFile);
		if(pathPartFile!="")
		{
			if(wxFileExists(pathPartFile))
			{
				particulesContainer.LoadParticleFile(pathPartFile,sceneMesh.UnitizeVar,reportEle,loadingMethod);

			}else{
				wxLogError(_("Particles file does not exist for the corresponding frequency"));
			}
		}
	}

	//Demmarage de la simulation
	this->rootUserConfig->GetElementByLibelle("3drendering")->GetElementByLibelle("animations")->UpdateBoolConfig("showparticle",true);
	this->GlFrame->RunSimulation();
}
void ProjectManager::OnMenuCreateFGroupFromSelection()
{
	wxWindowUpdateLocker freezeTree(this->treeScene);
	Element* searchResult=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES);
	if(searchResult!=NULL)
	{
		Element* newGroupe=searchResult->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE);
		newGroupe->FillWxTree(this->treeScene);
		wxTreeItemId groupid=newGroupe->GetElementInfos().idElement;
		wxTreeEvent begindrag(wxEVT_COMMAND_TREE_BEGIN_DRAG,this->treeScene);
		wxPostEvent(this->treeScene,begindrag);
		wxTreeEvent enddrag(wxEVT_COMMAND_TREE_END_DRAG,this->treeScene,groupid);
		wxPostEvent(this->treeScene,enddrag);
	}
}
void ProjectManager::OnMenuClearSurfaceGroup(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementWithProp)
{
	elementWithProp->DeleteAllElementByTypeR(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX);
}

void ProjectManager::OnMenuDeleteFolder(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementWithProp)
{
	wxMessageDialog dialog( mainFrame, _("Do you really want to delete this folder?"),
	this->mainFrame->GetLabel(), wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
	if( dialog.ShowModal()==wxID_NO)
			return;
	//Conversion en élément de rapport de simulation
	E_Report_File* reportEle=dynamic_cast<E_Report_File*>(elementWithProp);
	if(reportEle)
	{
		reportEle->DeleteFolder();
	}
}

void ProjectManager::OnMenuSetPreselection(Element* bfreqEl, Element::IDEVENT preselectionMode )
{
	E_Core_Core_Bfreqselection* bfreqObj=dynamic_cast<E_Core_Core_Bfreqselection*>(bfreqEl);
	if(bfreqObj)
	{
		bfreqObj->SetPreselection(preselectionMode);
		if(propFrame)
		{
			propFrame->RefreshValues();
		}
	}
}


void ProjectManager::OnMenuReloadFolder(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementWithProp)
{
	//Conversion en élément de rapport de simulation
	E_Report_File* reportEle=dynamic_cast<E_Report_File*>(elementWithProp);
	if(reportEle)
	{
		reportEle->RefreshFolderContents();
		reportEle->FillWxTree(fromCtrl);
	}
}
void ProjectManager::OnMenuGetProperties(uiTreeCtrl* fromCtrl,Element* eRoot,Element* elementWithProp)
{
	OpenPropertyElement(elementWithProp);
}

void ProjectManager::OnMenuFlipFaces()
{
	sceneMesh.FlipSelectedFaces();
	this->sceneMesh.Save((ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME).ToStdString());
	GlFrame->Refresh();
}

void ProjectManager::OnSelectElement( uiTreeCtrl* fromCtrl,Element* eRoot)
{
	//Séléction d'un ou plusieurs element dans l'arbre. On désélectionne les éléments autre que ceux
	// pouvant faire l'objet d'une sélection multiple


	//Recuperation du wxTreeItemId
	wxArrayTreeItemIds multiselection;
	fromCtrl->GetSelections(multiselection);
	bool deselection_faces=true;
	bool deselection=false;
	int last_elementtypeselected=-1;
	for(int idsel=0;idsel<multiselection.size();idsel++)
	{
		wxTreeItemId idElement=multiselection[idsel];
		Element* elementTarget=fromCtrl->GetElement(idElement);
		if(elementTarget!=NULL)
		{
			Element::t_elementInfo eInfos=elementTarget->GetElementInfos();
			if(eInfos.typeElement!=last_elementtypeselected && last_elementtypeselected!=-1)
				deselection=true;
			last_elementtypeselected=eInfos.typeElement;
			if(eInfos.typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
			{
				//SELECTION D'UNE FACE
				if(deselection_faces)
					this->sceneMesh.UnselectVertex();
				deselection_faces=false;
				E_Scene_Groupesurfaces_Groupe_Vertex* faceElement=dynamic_cast<E_Scene_Groupesurfaces_Groupe_Vertex*>(elementTarget);
				this->sceneMesh.SelectVertex(faceElement->GetFace(),faceElement->GetGroup());
				//Toutes les faces sont sélectionnées
				if(multiselection.size()==1) //Si seul les faces du groupe sont sélectionnées alors on met a jour la propriété d'aire si nécéssaire
				{
					float volFace=elementTarget->GetDecimalConfig("aire");
					if(volFace==0)
					{
						//On met à jour le volume du groupe
						volFace=sceneMesh.GetSelectionArea();
						if(volFace!=0)
						{
							elementTarget->UpdateDecimalConfig("aire",volFace);
						}
					}
				}
			}else if(eInfos.typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE)
			{
				//SELECTION D'UN GROUPE
				if(deselection_faces)
					this->sceneMesh.UnselectVertex();
				deselection_faces=false;
				E_Scene_Groupesurfaces_Groupe* faceElement=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(elementTarget);
				std::vector<t_faceIndex> lstFace;
				faceElement->GetFaces(lstFace);
				for(int idface=0;idface<lstFace.size();idface++)
				{
					this->sceneMesh.SelectVertex(lstFace[idface].face,lstFace[idface].group);
				}
				//Toutes les faces sont sélectionnées
				if(multiselection.size()==1) //Si seul les faces du groupe sont sélectionnées alors on met a jour la propriété d'aire si nécéssaire
				{
					float volGroupe=elementTarget->GetDecimalConfig("aire");
					if(volGroupe==0)
					{
						//On met à jour le volume du groupe
						volGroupe=sceneMesh.GetSelectionArea();
						if(volGroupe!=0)
						{
							elementTarget->UpdateDecimalConfig("aire",volGroupe);
						}
					}
				}
			}else{
				if(!elementTarget->IsAllowMultipleSelection())
				{
					//Désélectionne les éléments qui ne peuvent être sélectionné simulaténément
					if(idsel>0)
						fromCtrl->UnselectItem(idElement);

				}else{
					if(deselection) //les éléments de type différend ne peuvent être séléctionné ensemble
						fromCtrl->UnselectItem(idElement);
				}

				if(deselection_faces && multiselection.size()==1)
				{
					this->sceneMesh.UnselectVertex();
					deselection_faces=false;
				}
			}
		}


	}

	if(!deselection_faces)
		this->GlFrame->Refresh();
}
void ProjectManager::OnAskSelectPosition()
{
	//Switch to camera mode
	wxCommandEvent eventUpdate( wxEVT_PSPS_MAIN_EVENT, mainFrame->GetId() );
	eventUpdate.SetInt(ApplicationConfiguration::MAIN_EVENT_SWITCH_TO_CAMERA_TOOL);
	wxPostEvent(mainFrame, eventUpdate);
	GlFrame->modeSelectionPoint=true;
	wxLogMessage(_("Please click on the 3D view to update the coordinates")+wxString(" :"));
}


void ProjectManager::OnSelectPosition(vec3 posValue)
{
	if(GlFrame->GetCurrentTool()==OpenGlViewer::TOOL_MODE_RECEPTEURS_SOUNDLEVEL_EXTRACT)
	{
		recepteursSContainer.UserWantToKnowAcousticLevelAtPosition(posValue);
	}else{
		GlFrame->modeSelectionPoint=false;
		Element* elementTarget=treeScene->GetElement(treeScene->GetSelection());
		if(elementTarget && elementTarget->GetElementInfos().typeElement==Element::ELEMENT_TYPE_POSITION)
		{
				E_Position* elementPosition=dynamic_cast<E_Position*>(elementTarget);
				elementPosition->SetPosValue(posValue);
				this->OnMenuGetProperties(treeScene,rootScene,elementPosition);
		}
	}
}

void ProjectManager::NewProject()
{

	ClearUndoRedoHistory();
	this->ClearCurrentFolder(); //retourne faux si un fichier impossible à supprimer
	this->sceneMesh.Load("");
	if(this->propFrame!=NULL)
		this->propFrame->CloseElement();
	wxXmlDocument tmpDocXml;
	wxXmlNode* xmlRoot=new wxXmlNode(wxXML_ELEMENT_NODE,"projet");
	tmpDocXml.SetRoot(xmlRoot);
	this->BuildEmptyProject(xmlRoot);
	if(tmpDocXml.IsOk())
		tmpDocXml.Save(this->dossierCourant+this->FichierConfig);
	this->LoadCurrentProject();
	this->Init();
}

void ProjectManager::LoadCurrentProject(bool reloadXmlFile)
{
	wxProgressDialog progInfo(_("Loading project"),_("Loading XML file"),100,mainFrame,wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );

	this->CloseApp(); //ferme le projet courant


	int prog=0; //compteur information progression
	if(reloadXmlFile)
		this->projetConfig.Load(this->dossierCourant+this->FichierConfig);
	wxString fEncoding=this->projetConfig.GetFileEncoding();
	//this->projetConfig.SetEncoding("utf-8");
	// Initialisation des données par rapport au fichier XML

	wxXmlNode* rootNode = this->projetConfig.GetRoot();

	wxString propVal;
	if(rootNode!=NULL)
	{
		wxXmlNode* currentNode = rootNode->GetChildren();
		while( currentNode!=NULL)
		{
			if(currentNode->GetAttribute("eid",&propVal))
			{
				long typeEle;
				propVal.ToLong(&typeEle);
				if(typeEle==Element::ELEMENT_TYPE_CORE_ROOT)
				{
					this->rootCore = new E_Core(currentNode); //le chargement du fichier xml va se propager dans les éléments
					dynamic_cast<E_Core*>(this->rootCore)->BindUpdateEvent(OnElementRootUpdate);
					progInfo.Update(prog+8);
				}else if(typeEle==Element::ELEMENT_TYPE_RESULT_ROOT){
					this->rootResult = new E_Report(currentNode);
					dynamic_cast<E_Report*>(this->rootResult)->BindUpdateEvent(OnElementRootUpdate);
					progInfo.Update(prog+8);
				}else if(typeEle==Element::ELEMENT_TYPE_SCENE_ROOT){
					this->rootScene = new E_Scene(currentNode);
					ApplicationConfiguration::SetRootScene(this->rootScene);
					progInfo.Update(prog+8);
				}

			}
			currentNode=currentNode->GetNext();
		}
		if(this->rootCore==NULL || this->rootScene==NULL || this->rootResult==NULL)
		{
			wxLogError(_("XML file corrupted. File can't be loaded"));
			return;
		}
	}else{
		return; //Fichier vide
	}
	progInfo.Update(25,_("Loading 3D scene ..."));
	// Initialisation de la scène 3D
	Element* elementCible=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION);
	if(elementCible!=NULL)
	{
		wxString urlScene=this->dossierCourant+elementCible->GetStringConfig("urlmodel");
		if(urlScene!=this->dossierCourant && reloadXmlFile)
		{
			if(wxFileExists(urlScene))
			{
				sceneMesh.Load(urlScene.ToStdString());
			}else{
				wxLogError(_("Scene does not exist!"));
			}
		}
		dynamic_cast<E_Scene*>(this->rootScene)->BindUpdateEvent(OnElementRootUpdate);
	}else{
		return;
	}

}

bool ProjectManager::BuildEmptyProject(wxXmlNode* noeudRacine)
{
	if(noeudRacine!=NULL)
	{
		Element::SetElementCount(InstanceManager::GetElementCount()+1);

		//On va créer les trois racines du projet Scene,Core,Report
		E_Core elementCore;
		E_Scene elementScene;
		E_Report elementReport;

		//On leur demande de renseigner le document XML
		elementCore.SaveXMLDoc(noeudRacine);
		elementScene.SaveXMLDoc(noeudRacine);
		elementReport.SaveXMLDoc(noeudRacine);
		return true;
	}else{
		return false;
	}
}


void ProjectManager::Init( )
{
	if(this->rootCore==NULL || this->rootScene==NULL || this->rootResult==NULL)
		return;
    
	treeCore->Init(this->rootCore);
	treeScene->Init(this->rootScene);
	treeResult->Init(this->rootResult);

	this->GlFrame->OpenModel(&sceneMesh);

	this->GlFrame->ClearAnimatorLst();
	this->GlFrame->AddAnimator(&particulesContainer);
	this->GlFrame->AddAnimator(&recepteursSContainer);
	this->GlFrame->AddAnimator(&recepteursPContainer);
	particulesContainer.Init(); //Reinitialise l'encapsuleur des positions de partcicules
	recepteursSContainer.Init();
	GlFrame->RefreshElementDraw();
	//On affiche les propriétés de la configuration
	Element* conf=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_USERCONFIGURATION);
	if(conf)
	{
		this->OnMenuGetProperties(treeScene,rootScene,conf);
	}

	Element* elementCible=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION);
	if(elementCible!=NULL)
	{
		wxFileName urlProjet(elementCible->GetStringConfig("urlsave"));
		SetMainFrameName(urlProjet.GetName(),false);
	}
	sceneMesh.backgroundChange=true;
	this->sceneMesh.modelChange=true;
	GlFrame->Refresh();
	//On execute les scripts python __project_loading__.py
	BoolLocker locevt(&ProcessUpdateEvent,false);
	this->PyProjectLoadingEvt();
}

void ProjectManager::ControlPointerInitialisation()
{
	treeUserPref->Init(this->rootUserConfig.get());
	if(!wxFileExists(this->dossierCourant+FichierConfig))
	{
		this->NewProject();
	}else{
		// Un projet se trouve déjà dans le dossier current (dernier projet ouvert par l'utilisateur)
		// On charge ce projet au démarrage

		ClearUndoRedoHistory();
		this->LoadCurrentProject();
		this->Init();
	}
}
void ProjectManager::SetControlPointer(	wxTextCtrl* _logControl,uiTreeCtrl* _treeCore, uiTreeCtrl* _treeResult,
	uiTreeCtrl* _treeScene, uiTreeCtrl* _treeUserPref, wxToolBar* _glToolBar,wxFrame* _mainFrame,OpenGlViewer* _GlFrame,MainPropGrid* _propFrame ,wxAuiManager* _auiManager,PyConsole* pyCtrl)
{
	logControl=_logControl;
	shellControl=pyCtrl;
	treeCore=_treeCore;
	treeResult=_treeResult;
	treeScene=_treeScene;
	treeUserPref=_treeUserPref;
	glToolBar=_glToolBar;
	mainFrame=_mainFrame;
	GlFrame=_GlFrame;
	propFrame=_propFrame;
	auiManager=_auiManager;

	InitPythonEngine(); //Initialisation de python

	//Chargement de l'arbre de préférence (peut contenir des élément implémenté en python
	UserPreferenceXmlFilePath= wxStandardPaths::Get().GetUserDataDir()+wxFileName::GetPathSeparator()+ApplicationConfiguration::CONST_USER_PREFERENCE_FILE_NAME;
	if(!wxFileExists(UserPreferenceXmlFilePath))
	{
		this->CreateUserPreferenceTree();
	}else{
		this->LoadUserPreferenceTree();
	}
	ApplicationConfiguration::SetUserPreferenceElement(this->rootUserConfig);


	Element* render3d_node=this->rootUserConfig->GetElementByLibelle("3drendering");
	if(render3d_node)
	{

		render3d_node->SetIcon(Element::GRAPH_STATE_EXPANDED,Element::GRAPH_USER_PREF_ROOT_OPEN);
		render3d_node->SetIcon(Element::GRAPH_STATE_NORMAL,Element::GRAPH_USER_PREF_ROOT_CLOSE);
		//Lie l'événement de mis à jour des propriétés à l'objet de la scène 3D
		Element* cfgScene3D=render3d_node->GetElementByLibelle("general");
		if(cfgScene3D)
		{
			cfgScene3D->AppendEventCatcher(EventCatcher(new SceneUpdater(cfgScene3D,sceneMesh,GlFrame)));
			cfgScene3D->SetIcon(Element::GRAPH_STATE_ALL,Element::GRAPH_PREF_GENERAL);
		}
		Element* cfgAnimation=render3d_node->GetElementByLibelle("animations");
		if(cfgAnimation)
		{
			cfgAnimation->AppendEventCatcher(EventCatcher(new AnimationUpdater(cfgAnimation)));
			cfgAnimation->SetIcon(Element::GRAPH_STATE_ALL,Element::GRAPH_PREF_ANIMATION);
		}
		Element* cfgIsoMap=render3d_node->GetElementByLibelle("soundmap");
		if(cfgIsoMap)
		{
			cfgIsoMap->AppendEventCatcher(EventCatcher(new IsoPaletteUpdater(cfgIsoMap,&recepteursSContainer)));
			cfgIsoMap->SetIcon(Element::GRAPH_STATE_ALL,Element::GRAPH_PREF_NOISE_MAP);
		}
		Element* cfgLegend=render3d_node->GetElementByLibelle("legend");
		if(cfgLegend)
		{
			cfgLegend->SetIcon(Element::GRAPH_STATE_ALL,Element::GRAPH_PREF_LEGEND);
			cfgLegend->AppendEventCatcher(EventCatcher(new AnimatorUserPreferenceUpdater(cfgLegend,&recepteursSContainer)));
			cfgLegend->AppendEventCatcher(EventCatcher(new AnimatorUserPreferenceUpdater(cfgLegend,&recepteursPContainer)));
			cfgLegend->AppendEventCatcher(EventCatcher(new AnimatorUserPreferenceUpdater(cfgLegend,&particulesContainer)));
		}
		Element* cfgParticle=render3d_node->GetElementByLibelle("particle");
		if(cfgParticle)
		{
			cfgParticle->SetIcon(Element::GRAPH_STATE_ALL,Element::GRAPH_PREF_PARTICLES);
			cfgParticle->AppendEventCatcher(EventCatcher(new ParticleAnimatorUpdater(cfgParticle,&particulesContainer)));
		}
	}
	Element* mainpref_node=this->rootUserConfig->GetElementByLibelle("mainpref");
	if(mainpref_node)
	{
		mainpref_node->SetIcon(Element::GRAPH_STATE_EXPANDED,Element::GRAPH_USER_PREF_MAINCONFIGURATION_OPEN);
		mainpref_node->SetIcon(Element::GRAPH_STATE_NORMAL,Element::GRAPH_USER_PREF_MAINCONFIGURATION_CLOSE);
		Element* cfgHistory=mainpref_node->GetElementByLibelle("history");
		if(cfgHistory)
		{
			cfgHistory->SetIcon(Element::GRAPH_STATE_ALL,Element::GRAPH_USER_PREF_HISTORY);
		}
	}
}
bool ProjectManager::ClearFolder( wxString folderToEmpty )
{
	wxArrayString tabFichiers;
	GetAllFolderItems(folderToEmpty,tabFichiers);
	long nbfichier=tabFichiers.Count();
	if(nbfichier>0)
	{
		//Suppression des fichiers
		for(long i=0;i<nbfichier;i++)
		{
			wxString fichierdel = tabFichiers[i];
			if(wxFileExists(fichierdel))
			{
				wxRemoveFile(fichierdel);
			}
		}
		//Suppression des repertoires
		for(long i=nbfichier-1;i>=0;i--)
		{
			wxString fichierdel = tabFichiers[i];
			if(wxDirExists(fichierdel))
			{
				wxRmdir(fichierdel);
			}
		}
	}
	tabFichiers.clear();
	GetAllFolderItems(folderToEmpty,tabFichiers);
	if(tabFichiers.Count()==0)
		return true;
	else
		return false;

}


bool ProjectManager::CopyFolder( wxString folderToCopy, wxString toFolder )
{
	wxArrayString tabFichiers;
	this->GetAllFolderItems(folderToCopy,tabFichiers);
	long nbfichier=tabFichiers.Count();
	if(nbfichier>0)
	{
		//Copie des repertoires
		for(long i=0;i<nbfichier;i++)
		{
			wxString fichiercpy = tabFichiers[i];
			wxFileName folderTransf(fichiercpy);
			folderTransf.MakeRelativeTo(folderToCopy);
			wxString toCpy=toFolder+folderTransf.GetFullPath();
			if(wxDirExists(fichiercpy))
			{
				wxMkdir(toCpy);//Dossier
			}else if(wxFileExists(fichiercpy))
			{
				wxCopyFile(fichiercpy,toCpy);
			}
		}
	}
	return true;
}

bool ProjectManager::ClearCurrentFolder( )
{
	return ClearFolder(this->dossierCourant);
}


bool ProjectManager::UpdateZip(wxString folderfrom,wxString zipfilename)
{
	wxFileName relPath(folderfrom);
	relPath.RemoveLastDir();
	wxString relativeFolder=relPath.GetFullPath();
	wxArrayString tabFichiers;
	this->GetAllFolderItems(folderfrom,tabFichiers);
	long nbfichier=tabFichiers.Count();
	wxString tmpFileName=zipfilename+".tmp";
	using namespace std;
	auto_ptr<wxFFileInputStream> in(new wxFFileInputStream(zipfilename));
    wxTempFileOutputStream out(tmpFileName);

    wxZipInputStream inzip(*in);
	wxProgressDialog progInfo(_("Saving project"),_("Updating project backup"),100,mainFrame,wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );

    wxZipOutputStream outzip(out);

    wxZipEntryPtr entry;

    // transfer any meta-data for the archive as a whole (the zip comment
    // in the case of zip)
    outzip.CopyArchiveMetaData(inzip);

    // call CopyEntry for each entry except those matching the pattern
	//////////////////////////////////////////////////////////////////
	// Mise à jour des fichiers déjà présent dans l'archive
	//////////////////////////////////////////////////////////////////
	int nbUpdated=0;
	int nbToUpdate=inzip.GetTotalEntries();
    while (entry.reset(inzip.GetNextEntry()), entry.get() != NULL)
	{
		nbUpdated++;
		//Si le fichier existe toujours
		wxString elementName=entry->GetName();			//"instancex/file01.txt"
		wxFileName inputFile(elementName);
		inputFile.RemoveDir(0);							//"file01.txt"
		elementName=folderfrom+inputFile.GetFullPath();	//"current/instancex/file01.txt"
		inputFile=wxFileName(elementName);
		wxFileName toolPath(elementName);
		toolPath.MakeRelativeTo(relativeFolder);
		wxString entryName=toolPath.GetFullPath();		//"instancey/file01.txt"
		int indexEl;
		if(inputFile.IsDir())
			indexEl=tabFichiers.Index(elementName.Left(elementName.size()-1));
		else
			indexEl=tabFichiers.Index(elementName);
		if(indexEl!=wxNOT_FOUND)
		{
			tabFichiers.RemoveAt(indexEl);
		}
		if (wxFileExists(elementName) || wxDirExists(elementName))
		{
			// Si le fichier n'a pas été modifié alors on copie les données directement
			wxDateTime zipFileDate=entry->GetDateTime();
			zipFileDate.SetSecond(0);
			zipFileDate.SetMillisecond(0);
			wxDateTime realFileDate=inputFile.GetModificationTime();
			realFileDate.SetSecond(0);
			realFileDate.SetMillisecond(0);
			wxFileName showPath(inputFile);
			showPath.MakeRelativeTo(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath);
			if(zipFileDate.IsEqualTo(realFileDate) || inputFile.IsDir())
			{
				if(!inputFile.IsDir())
				{
					progInfo.Update((int)((nbUpdated / (float)nbToUpdate) * 100), wxString::Format(_("Copy of the file\n%s"), GetFittedString(progInfo, inputFile.GetFullPath())));
				}
				//progInfo.Fit();
				//progInfo.Update();
				entry->SetName(entryName);
				if (!outzip.CopyEntry(entry.release(), inzip))
					break;
			}else{
				//Le fichier a été modifié on compacte ce nouveau fichier dans l'archive
				if(inputFile.IsFileReadable())
				{
					progInfo.Update((int)((nbUpdated/(float)nbToUpdate)*100), wxString::Format(_("Compression of file\n%s"),GetFittedString(progInfo,showPath.GetFullPath())));
					wxFileStream fstr(elementName);
					outzip.PutNextEntry(entryName,realFileDate);
					outzip<<fstr;
				}
			}
		}
	}

    // Ferme le flux afin de pouvoir continuer à transferer des données
    in.reset();


	//////////////////////////////////////////////////////////////////
	// Ajout des nouveaux fichiers non présent dans l'archive
	//////////////////////////////////////////////////////////////////
	nbfichier=tabFichiers.size();
	if(nbfichier>0)
	{
		//Compression des fichiers
		for(long i=0;i<nbfichier;i++)
		{
			wxFileName toolPath(tabFichiers[i]);
			toolPath.MakeRelativeTo(relativeFolder);
			wxString entryName=toolPath.GetFullPath();
			if(wxFileExists(tabFichiers[i]))
			{
				wxFileName nouvFich(tabFichiers[i]);
				progInfo.Update((int)((i / (float)nbfichier) * 100), wxString::Format(_("Compression of file\n%s"), nouvFich.GetFullName()));
				wxDateTime fileTime=nouvFich.GetModificationTime();
				outzip.PutNextEntry(entryName,fileTime);
				wxFFileInputStream fstr(tabFichiers[i]);
				outzip<<fstr;
			}else if(wxDirExists(tabFichiers[i]))
			{
				outzip.PutNextDirEntry(entryName);
			}
		}
	}



    // Verifie le bon déroulement de l'enregistrement du fichier
    bool success = inzip.Eof() && outzip.Close() && out.Commit();

	if(success)
	{
		wxRemoveFile(zipfilename);
		wxRenameFile(tmpFileName,zipfilename);
	}

	return success;
}

bool ProjectManager::ZipFolder(const wxString&folderfrom,const wxString&zipfilename)
{
	wxFileName relPath(folderfrom);
	relPath.RemoveLastDir();
	wxString relativeFolder=relPath.GetFullPath();

	//Verification de l'espace disque disponible
	wxDiskspaceSize_t sizeFolder=wxDir::GetTotalSize(folderfrom).ToULong();
	wxDiskspaceSize_t sizeFree;
	wxFileName destinationZip(zipfilename);
	destinationZip.MakeAbsolute();
	if(wxGetDiskSpace(destinationZip.GetPath(), NULL, &sizeFree))
	{
		if(sizeFree<sizeFolder)
		{
			wxLogError(_("Enough memory. %li Mo required, %li Mo free on the corresponding disk"),(long)(sizeFolder.ToLong()/1000000),(long)(sizeFree.ToLong()/1000000));
			return false;
		}
	}

	//Si le fichier zip existe déjà on met à le jour le fichier déjà enregistré
	if(wxFileExists(zipfilename))
		return UpdateZip(folderfrom,zipfilename);


	wxProgressDialog progInfo(_("Project compression"),_("Project files compression in progress ..."),100,mainFrame,wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
	wxArrayString tabFichiers;
	this->GetAllFolderItems(folderfrom,tabFichiers);
	wxFFileOutputStream out(zipfilename);

    wxZipOutputStream zip(out);

	long nbfichier=tabFichiers.Count();
	if(nbfichier>0)
	{
		//Compression des fichiers
		for(long i=0;i<nbfichier;i++)
		{
			wxFileName toolPath(tabFichiers[i]);
			toolPath.MakeRelativeTo(relativeFolder);
			wxString entryName=toolPath.GetFullPath();
			if(wxFileExists(tabFichiers[i]))
			{
				wxFileName nouvFich(tabFichiers[i]);
				progInfo.Update((int)((i / (float)nbfichier) * 100), wxString::Format(_("Compression of file\n%s"), GetFittedString(progInfo, nouvFich.GetFullPath())));
				zip.PutNextEntry(entryName,nouvFich.GetModificationTime());
				wxFFileInputStream fstr(tabFichiers[i]);
				zip<<fstr;
			}else if(wxDirExists(tabFichiers[i]))
			{
				zip.PutNextDirEntry(entryName);
			}
		}
	}
	return true;
}

void ProjectManager::CreateUserPreferenceTree()
{
	userPreferenceDocument.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE,"configuration"));
	//Noeud Racine
	rootUserConfig=new E_UserPreferenceNode(NULL,"userprefroot");
	/////////////////////////////////////////////////////////////
	//Dossier Rendu 3D
	Element* render3d_node=rootUserConfig->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_NODE,"3drendering");

	_("3drendering");
	/////////////////////////////////////////////////////////////
	//Dossier Rendu 3D / Général
	Element* render3d_general_node=render3d_node->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM,"general");
	_("general");
	render3d_general_node->AppendPropertyColor("colormodellines","Scene lines",0,0,255);
	render3d_general_node->AppendPropertyColor("colorselection","Selection",128,0,255);
	render3d_general_node->AppendPropertyColor("colorbackground","Scene background",255,255,255);
	render3d_general_node->AppendPropertyColor("colordefaultmodel","Default color",125,125,125);
	_("Scene lines");
	_("Selection");
	_("Scene background");
	_("Default color");
	/////////////////////////////////////////////////////////////
	//Dossier Rendu 3D / Animations
	Element* render3d_animation_node=render3d_node->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM,"animations");
	_("animations");
	render3d_animation_node->AppendPropertyDecimal("animation_rate","Render frequency (Hz)",60,false,4,true,true,100,1);
	render3d_animation_node->AppendPropertyBool("showparticle","Particles display",true);
	render3d_animation_node->AppendPropertyBool("showrecepteurss","Surface receivers display",true);
	render3d_animation_node->AppendPropertyColor("legendtextcolour","Label color",0,0,0);
	_("Render frequency (Hz)");
	_("Label color");
	//_("Display time step (s)");
	_("Particles display");
	_("Surface receivers display");
	/////////////////////////////////////////////////////////////
	//Dossier Rendu 3D / Légendes
	Element* render3d_legend_node=render3d_node->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM,"legend");
	_("legend");
	render3d_legend_node->AppendPropertyFont("legend_font","Police font");
	render3d_legend_node->AppendPropertyColor("legend_text_foreground_color","Text color",0,0,0);
	render3d_legend_node->AppendPropertyColor("legend_text_background_color","Text background color",255,255,255);
	render3d_legend_node->AppendPropertyBool("legend_text_background_invisible","Transparent text background",true);
	_("Police font");
	_("Text color");
	_("Text background color");
	_("Transparent text background");

	/////////////////////////////////////////////////////////////
	//Dossier Rendu 3D / Carte de bruits
	Element* render3d_soundmap_node=render3d_node->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM_ISOTEMPLATE,"soundmap");
	render3d_soundmap_node->AppendPropertyColor("iso_line_color","Iso-lines color",0,0,0);
	_("Iso-lines color");
	_("soundmap");
	/////////////////////////////////////////////////////////////
	//Dossier Rendu 3D / Particules
	Element* render3d_particle_node=render3d_node->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM,"particle");
	render3d_particle_node->AppendPropertyColor("particle_color","Color of particles",15,15,128);
	_("Color of particles");
	_("particle");

	/////////////////////////////////////////////////////////////
	//Dossier préférence générale
	Element* main_node=rootUserConfig->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_NODE,"mainpref");
	_("mainpref");
	/////////////////////////////////////////////////////////////
	//Dossier Général / Historique
	Element* main_history_node=main_node->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM,"history");
	_("history");
	main_history_node->AppendPropertyBool("keep_modification_history","Keep modification history",true);
	_("Keep modification history");
}
void ProjectManager::SaveUserPreferenceTree()
{
	//userPreferenceDocument.SetVersion(wxString::Format("%i.%i.%i",ApplicationConfiguration::SPPS_UI_VERSION_MAJOR,ApplicationConfiguration::SPPS_UI_VERSION_MINOR,ApplicationConfiguration::SPPS_UI_VERSION_REVISION));
	userPreferenceDocument.GetRoot()->DeleteAttribute("app_version");
	userPreferenceDocument.GetRoot()->AddAttribute("app_version",wxString::Format("%i.%i.%i",ApplicationConfiguration::SPPS_UI_VERSION_MAJOR,ApplicationConfiguration::SPPS_UI_VERSION_MINOR,ApplicationConfiguration::SPPS_UI_VERSION_REVISION));
	rootUserConfig->SaveXMLDoc(userPreferenceDocument.GetRoot());
	userPreferenceDocument.SetFileEncoding("utf-8");
	//userPreferenceDocument.SetEncoding("utf-8");
	userPreferenceDocument.Save(UserPreferenceXmlFilePath);
}
bool ProjectManager::LoadUserPreferenceTree()
{
	userPreferenceDocument.Load(UserPreferenceXmlFilePath);
	wxXmlNode* rootNode =userPreferenceDocument.GetRoot();
	if(rootNode!=NULL)
	{
		wxXmlNode* currentNode = rootNode->GetChildren();
		if( currentNode!=NULL)
		{
			rootUserConfig=new E_UserPreferenceNode(NULL,"root",currentNode);
			wxString appver;
			userPreferenceDocument.GetRoot()->GetAttribute("app_version",&appver);
			PatchUserPreferenceTree(appver);
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}
void ProjectManager::PatchUserPreferenceTree(const wxString& oldVersion)
{
	int major,minor,revision;
	wxSscanf(oldVersion,"%i.%i.%i",&major,&minor,&revision);
	int old_flat=major*10000+minor*100+revision;
	int current_flat=ApplicationConfiguration::SPPS_UI_VERSION_MAJOR*10000+ApplicationConfiguration::SPPS_UI_VERSION_MINOR*100+ApplicationConfiguration::SPPS_UI_VERSION_REVISION;
	if(major==ApplicationConfiguration::SPPS_UI_VERSION_MAJOR && minor==ApplicationConfiguration::SPPS_UI_VERSION_MINOR && revision==ApplicationConfiguration::SPPS_UI_VERSION_REVISION)
		return;

	if(old_flat<10109)
	{
		/////////////////////////////////////////////////////////////
		//Dossier préférence générale
		Element* main_node=rootUserConfig->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_NODE,"mainpref");
		_("mainpref");
		/////////////////////////////////////////////////////////////
		//Dossier Général / Historique
		Element* main_history_node=main_node->AppendFilsByType(Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM,"history");
		_("history");
		main_history_node->AppendPropertyBool("keep_modification_history","Keep modification history",true);
		_("Keep modification history");
	}
}
bool ProjectManager::UnZipFolder(const wxString&zipfilename,const wxString&folderTo)
{
	wxProgressDialog progInfo(_("Project decompression"),_("Project files decompression in progress ..."),100,mainFrame,wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
	wxString fto(folderTo);
    wxZipEntryPtr entry;
    wxFFileInputStream in(zipfilename);
    wxZipInputStream zip(in);

	int nbfichier=zip.GetTotalEntries();
	int i=0;
    while (entry.reset(zip.GetNextEntry()), entry.get() != NULL)
    {
        // access meta-data
        wxString name = entry->GetName();
		wxFileName fichToCreate(name);
		fichToCreate.RemoveDir(0);
		fichToCreate.SetPath(fto+fichToCreate.GetPath());
		wxString path=fichToCreate.GetPath();
		name=fichToCreate.GetFullPath();
		if(!wxDirExists(path))
			wxMkdir(path);
		if(!fichToCreate.IsDir())
		{
			i++;
			progInfo.Update((int)((i/(float)nbfichier)*100),wxString::Format(_("Dezip of file\n%s"),GetFittedString(progInfo,name)));
			//progInfo.Fit();
			//progInfo.Update();
			wxFFileOutputStream out(name);

			wxString extension=fichToCreate.GetExt().Lower();
			// Empeche l'importation d'un virus provenant d'un autre projet
			if(!(extension=="pyd" || extension=="py" || extension=="vbs" || extension=="exe" || extension=="com" || extension=="bat" || extension=="pif" || extension=="scr" || extension=="reg" ))
			{
				// read 'zip' to access the entry's data
				out<<zip;
				out.Close();
				wxDateTime entryTime = entry->GetDateTime();
				fichToCreate.SetTimes(NULL,&entryTime,&entryTime);
			}
		}
    }
	return true;
}


bool ProjectManager::Open(const wxString&filename)
{
	wxFileName fProjet(filename);
	if(fProjet.GetExt()=="proj" && fProjet.FileExists())
	{
		this->ClearCurrentFolder();
		//this->NewProject();
		try
		{
			this->UnZipFolder(filename,this->dossierCourant);
		}catch( ... ) {
			return false;
		}
		this->LoadCurrentProject(true);
		this->Init();
		//Sauvegarde du nom dans l'element de config
		Element* elementCible=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION);
		if(elementCible!=NULL)
		{
			elementCible->UpdateStringConfig("urlsave",filename);
		}
		//logControl->Clear();
		wxLogMessage(_("Loading project: %s"), fProjet.GetFullName());
		return true;
	}else{
		return false;
	}

}
void ProjectManager::SetMainFrameName(wxString Libelle,bool Updated)
{
	if(this->mainFrame)
	{
		wxString addModif;
		if(Updated)
		{
			addModif=" *";
			needSave=true;
		}else{
			needSave=false;
		}
		addModif<<" - "<<APPLICATION_NAME;
		this->mainFrame->SetLabel(Libelle+addModif);
	}
}

bool ProjectManager::IsProjectNeedSave()
{
	return needSave;
}

bool ProjectManager::Save( )
{
	wxString nomdefichier;
	if(!this->rootScene)
		return false;
	Element* elementCible=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION);
	if(elementCible!=NULL)
	{
		nomdefichier=elementCible->GetStringConfig("urlsave");
		if(!wxFileExists(nomdefichier))
			return false;
	}else{
		return false;
	}
	if(this->needSave)
		return this->SaveTo(nomdefichier);
	else
		return true;
}

bool ProjectManager::SaveCopy(const wxString& filename )
{
	return this->SaveTo(filename,false);
}

bool ProjectManager::SaveTo( const wxString& filename , bool updateConfig)
{
	wxFileName pathSauvegarde(filename);

	if(!this->rootScene)
	{
		return false;
	}else{
		wxProgressDialog progInfo(_("Saving project"),_("Updating XML file"),100,mainFrame,wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
		//Sauvegarde du nom dans l'element de config
		Element* elementCible=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION);
		if(elementCible!=NULL && updateConfig)
		{
			elementCible->UpdateStringConfig("urlsave",filename);
		}
		SaveChanges();
	}

	if(updateConfig)
	{
		SetMainFrameName(pathSauvegarde.GetName(),false);
	}
	bool res=ZipFolder(this->dossierCourant,filename);
	wxLogMessage(_("Save finished"));
	return res;
}

bool ProjectManager::SaveChanges( )
{
	this->UpdateXmlFile();
	return true;
}


void ProjectManager::GetAllFolderItems(wxString rootFolder,wxArrayString& folderDir, bool seekSubDirs)
{
	if(wxDirExists(rootFolder))
	{
		wxDir dir(rootFolder);
		wxString filename;
		bool cont = dir.GetFirst(&filename);
		while ( cont )
		{
			folderDir.Add(rootFolder+filename);
			if(wxDirExists(rootFolder+filename))
				if(seekSubDirs)
					GetAllFolderItems(rootFolder+filename+wxFileName::GetPathSeparator(),folderDir);
			cont = dir.GetNext(&filename);
		}
	}
}

void ProjectManager::OnChangeCameraMode(wxCommandEvent& event,OpenGlViewer::CAMERA_MODE cameraMode)
{
	this->GlFrame->SetCameraMode(cameraMode);
}

void ProjectManager::OnMainEvent( ApplicationConfiguration::MAIN_EVENT eventId,wxCommandEvent& event)
{
	switch(eventId)
	{
		case ApplicationConfiguration::MAIN_EVENT_REFRESH_REPORTFOLDER:
			RefreshReportFolder();
			break;
		case ApplicationConfiguration::MAIN_EVENT_POST_PYTHON_COMMAND:
			this->OnPushPythonCommand(event);
			break;
	}
}


void ProjectManager::DoShapeComputation(const bool userEvent,const bool resetFaceOrientation)
{
	this->sceneMesh.ComputeFacesBorders(resetFaceOrientation);

	if(userEvent)
		this->sceneMesh.Save((ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME).ToStdString());

	this->GlFrame->Refresh();
}

void ProjectManager::OnModeFlatOutside(wxCommandEvent& event)
{
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelFaces,renderOriginalMaterial);
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelMaterialFaces,!renderOriginalMaterial);
	ApplicationConfiguration::GLOBAL_VAR.drawMaterialColors=!renderOriginalMaterial;
	this->sceneMesh.modelChange=true;
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelInside,false);
	this->GlFrame->RefreshElementDraw();
}

void ProjectManager::OnModeFlatInside(wxCommandEvent& event)
{

	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelFaces,renderOriginalMaterial);
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelMaterialFaces,!renderOriginalMaterial);
	ApplicationConfiguration::GLOBAL_VAR.drawMaterialColors=!renderOriginalMaterial;
	this->sceneMesh.modelChange=true;
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelInside,true);
	this->GlFrame->Refresh();
	this->GlFrame->RefreshElementDraw();
}
void ProjectManager::OnModeFlatNone(wxCommandEvent& event)
{
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelMaterialFaces,false);
	ApplicationConfiguration::GLOBAL_VAR.drawMaterialColors=false;
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelFaces,false);
	this->sceneMesh.modelChange=true;
	this->GlFrame->Refresh();
	this->GlFrame->RefreshElementDraw();
}

void ProjectManager::OnModeWireFrameFull(wxCommandEvent& event)
{
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelLinesAndConstruction,true);
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelLines,true);
}

void ProjectManager::OnModeWireFrameShape(wxCommandEvent& event)
{
	DoShapeComputation();
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelLinesAndConstruction,false);
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelLines,true);
}
void ProjectManager::OnModeWireFrameNone(wxCommandEvent& event)
{
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelLinesAndConstruction,false);
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelLines,false);
}

void ProjectManager::OnModeOriginalPaint(wxCommandEvent& event)
{
	renderOriginalMaterial=true;
	ApplicationConfiguration::GLOBAL_VAR.drawMaterialColors=false;
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelMaterialFaces,false);
 	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelFaces,true);
	this->GlFrame->RefreshElementDraw();
}
void ProjectManager::OnModeMaterialPaint(wxCommandEvent& event)
{
	renderOriginalMaterial=false;
	ApplicationConfiguration::GLOBAL_VAR.drawMaterialColors=true;
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelMaterialFaces,true);
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderModelFaces,false);
	this->GlFrame->RefreshElementDraw();
}

void ProjectManager::OnMaillage(wxCommandEvent& event)
{
	if(!this->rootCore)
		return;
	//On demande à l'utilisateur de choisir le mailleur associé au moteur de calcul
	std::list<Element*> eFilsCore;
	this->rootCore->GetChildElement(eFilsCore);
	smart_ptr_ar<wxString> lstCore=new wxString[eFilsCore.size()];
	int i=0;
	for(std::list<Element*>::iterator itfils=eFilsCore.begin();itfils!=eFilsCore.end();itfils++)
	{
		lstCore[i]=(*itfils)->GetElementInfos().libelleElement;
		i++;
	}
	wxSingleChoiceDialog choiceDialog(mainFrame,_("Please chose a calculation code"),_("Display mesh"),eFilsCore.size(),&(*lstCore));
	if(choiceDialog.ShowModal()==wxID_OK)
	{
		std::list<Element*>::iterator itfils=eFilsCore.begin();
		int i_sel=choiceDialog.GetSelection();
		for(int itcounter=0;itcounter<i_sel;itcounter++)
			itfils++;
		RunCoreMaillage((*itfils));
		this->GlFrame->ChangeRenderMode(OpenGLApp::renderMaillageFaces,true);
		this->GlFrame->SetCutPlane(true,-1,-1,.5);
	}
}

void ProjectManager::OnShowHideMaillage(wxCommandEvent& event)
{
	this->GlFrame->ChangeRenderMode(OpenGLApp::renderMaillageFaces,!this->GlFrame->GetCurrentRenderMode(OpenGLApp::renderMaillageFaces));
}

void ProjectManager::OnCameraTool(wxCommandEvent& event)
{
	this->GlFrame->SetCurrentTool(OpenGlViewer::TOOL_MODE_CAMERA);
}

void ProjectManager::OnSwitchSelectionMode(wxCommandEvent& event)
{
	this->GlFrame->SetCurrentTool(OpenGlViewer::TOOL_MODE_SELECTION);
	wxLogMessage(_("Select face: Left Click on the 3D view 3D to select a face; Double left Click+Ctrl to select a face entirely"));
}



void ProjectManager::OnGetdbLevelMode(wxCommandEvent& event)
{
	this->GlFrame->SetCurrentTool(OpenGlViewer::TOOL_MODE_RECEPTEURS_SOUNDLEVEL_EXTRACT);
}
void ProjectManager::OnStartSimulation(wxCommandEvent& event)
{
	this->GlFrame->RunSimulation();
}

void ProjectManager::OnNextStepSimulation(wxCommandEvent& event)
{
	this->GlFrame->NextStepSimulation();
}

void ProjectManager::OnPreviousStepSimulation(wxCommandEvent& event)
{
	this->GlFrame->PreviousStepSimulation();
}


void ProjectManager::OnScrollChanged(float value,float max)
{
	float Percentage = value/max;
	this->GlFrame->SetCutPlane(true,-1,-1,Percentage);
}

void ProjectManager::OnPauseSimulation(wxCommandEvent& event)
{
	this->GlFrame->PauseSimulation();
}

void ProjectManager::OnStopSimulation(wxCommandEvent& event)
{
	this->GlFrame->StopSimulation();
}

void ProjectManager::OnEmptySimulation( )
{
	if(GlFrame)
	{
		GlFrame->InitAnimatorLst();
		GlFrame->Refresh();
	}
}

void ProjectManager::OnSelectVertex PARAM_BOUND_ON_SELECT_FACES
{
	wxWindowUpdateLocker updLock(this->treeScene);
	//On affiche sur la vue 3D
	if(!ctrlDown)
	{
		this->sceneMesh.UnselectVertex();
		for(int idface=0;idface<facelst.size();idface++)
			this->sceneMesh.SelectVertex(facelst[idface].f,facelst[idface].g);
	}else{
		for(int idface=0;idface<facelst.size();idface++)
		{
			if(!dragging && this->sceneMesh.IsSelectedFace(facelst[idface].f,facelst[idface].g))
			{
				this->sceneMesh.UnselectVertex(facelst[idface]);
			}else{
				this->sceneMesh.SelectVertex(facelst[idface]);
			}
		}
	}
	//On sélectionne l'élément dans l'arbre
	Element* targetElement=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES);
	if(targetElement != NULL)
	{
		E_Scene_Groupesurfaces* groupeSurfaces=dynamic_cast<E_Scene_Groupesurfaces*>(targetElement);
		if(!ctrlDown)
			this->treeScene->UnselectAll();
		this->treeScene->LockElementEvent();
		for(int idface=0;idface<facelst.size();idface++)
		{
			wxTreeItemId idFace=groupeSurfaces->GetIdElementByFaceAndGroup(facelst[idface].f,facelst[idface].g);

			if(idFace.IsOk())
			{
				bool selectitem=true;
				if(!dragging && ctrlDown)
				{
					if(this->treeScene->IsSelected(idFace))
						selectitem=false;
				}
				if(facelst.size()==1)
				{
					this->treeScene->ScrollTo(idFace);
				}
				this->treeScene->SelectItem(idFace,selectitem);
			}
		}
		this->treeScene->LockElementEvent(false);
	}
}

void ProjectManager::OnExportModel(wxString& FileName)
{
	sceneMesh.Save(FileName.ToStdString());
}


void ProjectManager::OnChangePlan(int axe)
{
	this->GlFrame->SetCutPlane(true,axe,-1,-1);
}

void ProjectManager::OnUpdateEvent(Element* elUpdate)
{
	if(ProcessUpdateEvent)
	{
		PushModificationToHistory();

		if(elUpdate->IsDrawable())
			this->GlFrame->RefreshElementDraw();

		//Notification de changement du projet
		if(needSave==false)
		{
			Element* elementCible=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION);
			if(elementCible!=NULL)
			{
				wxFileName urlProjet(elementCible->GetStringConfig("urlsave"));
				SetMainFrameName(urlProjet.GetName(),true);
			}
		}
		this->GlFrame->Refresh(false);
	}
}
bool ProjectManager::LoadScene(const t_param_load_model& paramLoading)
{
	wxProgressDialog progInfo(_("Loading 3D scene ..."),_("Loading 3D scene ..."),100,mainFrame,wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
	t_retrieves_groups oldFacesDistribution;
	if(paramLoading.keepexistingfacegroup)
		BuildFaceGroupAssociation(oldFacesDistribution);
	bool loadSuccess=sceneMesh.Load(paramLoading.pathModel.ToStdString());
	if(loadSuccess)
	{
		this->RepairCurrentMesh(paramLoading);
			if(paramLoading.glueSurfaces)
				DoShapeComputation(false,true); //Calcul des contours des surfaces et correction de l'orientation
		this->GlFrame->OpenModel(&sceneMesh);
		this->GlFrame->AddAnimator(&particulesContainer);
		this->GlFrame->AddAnimator(&recepteursSContainer);
		this->GlFrame->AddAnimator(&recepteursPContainer);

		progInfo.Update(25,_("Loading faces of 3D scene"));
		this->AddLogMessage(wxString::Format(_("Loading 3D scene: %s\n"), paramLoading.pathModel.AfterLast(wxFileName::GetPathSeparator())));
		if(paramLoading.keepexistingfacegroup)
			this->LoadFacesFromModel(&progInfo,&oldFacesDistribution,paramLoading.epsilonLinkingFaceGroup);
		else
			this->LoadFacesFromModel(&progInfo);
		//On sauvegarde la nouvelle scène3D
		progInfo.Update(50,_("Saving 3D scene"));
		this->sceneMesh.Save((this->dossierCourant+ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME).ToStdString());
		//Chargement de la surface du modèle
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.surfScene=this->sceneMesh.GetAireScene();
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.volScene=0;
		//Sauvegarde du nom dans l'element de config
		Element* elementCible=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION);
		if(elementCible!=NULL)
		{
			elementCible->UpdateStringConfig("urlmodel",ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME);
		}
		this->GlFrame->RefreshElementDraw();
		return true;
	}else{
		wxLogError(_("Unable to load the 3D model"));
		return false;
	}
}

void ProjectManager::ChangeModel3d(const wxString& FileName)
{
	t_param_load_model paramLoading;
	paramLoading.pathModel=FileName;
	sceneLoadOptionDialog optDialog(mainFrame,_("Please, select 3D scene importation options"),_("Loading 3D scene"),paramLoading.paramTetgen);

	int res=optDialog.ShowModal();
	if(res==wxID_OK)
	{
		paramLoading.keepexistingfacegroup=optDialog.IsKeepExistingFaceLinks();
		paramLoading.domeshsurface=optDialog.IsMeshSurface();
		paramLoading.docorrection=optDialog.IsMeshRepair();
		paramLoading.paramTetgen=optDialog.GetMeshParameters();
		paramLoading.launchRemeshWizard=optDialog.IsRemeshModel();
		paramLoading.epsilonLinkingFaceGroup=optDialog.GetEpsilonLinkingFaceGroup();
		LoadScene(paramLoading);
		//supprime les groupes vides
		Element* elgrp=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES);
		if(elgrp)
		{
			E_Scene_Groupesurfaces* groupefolder=dynamic_cast<E_Scene_Groupesurfaces*>(elgrp);
			if(groupefolder)
				groupefolder->ClearEmptyGroup();
		}
	}
}


void ProjectManager::BuildModel3d(vec3 debCuboide,vec3 finCuboide)
{
	bool loadSuccess=this->sceneMesh.BuildModel(debCuboide,finCuboide);
	if(loadSuccess)
	{
		this->AddLogMessage(_("Build of a 3D scene.\n"));
		//On sauvegarde la nouvelle scène3D
		if(!this->sceneMesh.Save((this->dossierCourant+ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME).ToStdString())) {
			wxLogError(_("Error while saving the 3d model"));
		}
		this->ChangeModel3d(this->dossierCourant+ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME);
	}
}

void ProjectManager::BuildFaceGroupAssociation(t_retrieves_groups& oldFacesDistribution)
{
	ivec3 isommets;
	vec3 sommets[3];
	//Il faut obtenir des groupes du projet la liste des faces qu'ils contiennent
	std::vector<Element*> grpList;
	this->rootScene->GetAllElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE,grpList);
	for(wxInt32 i=0;i<grpList.size();i++)
	{
		E_Scene_Groupesurfaces_Groupe* curGrp=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(grpList[i]);
		if(curGrp)
		{
			std::vector<t_faceIndex> grpFaceList;
			curGrp->GetFaces(grpFaceList);
			for(wxInt32 iface=0;iface<grpFaceList.size();iface++)
			{
				isommets=this->sceneMesh.GetFace(grpFaceList[iface]).Vertices;
				this->sceneMesh.GetRealVertice(isommets.a,&(sommets[0]));
				this->sceneMesh.GetRealVertice(isommets.b,&(sommets[1]));
				this->sceneMesh.GetRealVertice(isommets.c,&(sommets[2]));
				oldFacesDistribution.retrieverTool.AddFace(sommets[0],sommets[1],sommets[2]);
			}
			oldFacesDistribution.retrieverTool.CloseGroup();
			oldFacesDistribution.containers.push_back(curGrp);
		}
	}
}
void ProjectManager::LoadFacesFromModel(wxProgressDialog* progDiag,t_retrieves_groups* oldFacesDistribution ,float epsilonDist)
{
	ProcessUpdateEvent=false;
	//On recupere l'element générateur de groupe
	Element* searchResult=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES);
	if(searchResult!=NULL)
	{
		//On vide les groupes existants
		rootScene->DeleteAllElementByTypeR(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX);
		//On ajoute les groupes selon le modèle
		if(!oldFacesDistribution) //Si on ne tente pas de retrouver les anciennes définitions de surfaces
		{
			int nbgroup=this->sceneMesh.GetNumGroups();
			int nbmodelfaces=this->sceneMesh.GetNumFaces();
			int totfacesinserted=0;
			int ansprog=0;
			for(int i=0;i<nbgroup;i++)
			{
				int nbface=this->sceneMesh.GetNumFaces(i);
				if(nbface>0)
				{
					E_Scene_Groupesurfaces_Groupe* newGroupe=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(searchResult->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE,this->sceneMesh.GetNameGroup(i)));

					std::list<t_faceIndex> faceList;
					for(long j=0;j<nbface;j++)
					{
						faceList.push_back(t_faceIndex(j,i));
						if(progDiag!=NULL)
						{
							totfacesinserted++;
							int prog=((float)totfacesinserted/nbmodelfaces)*100;
							if(prog>0 && prog<100 && prog!=ansprog)
							{
								progDiag->Update(prog*.75+25);
								ansprog=prog;
							}
						}
					}
					newGroupe->AddFaces(faceList);
				}
			}
		}else{
			/*Les groupes ont été vidés et nous allons distribuer les surfaces en fonctions des attributions
			 * L'opération est la suivante pour chaque face :
			 * 1:Retrouver la liste des groupes de la nouvelle face à partir du centre
			 */
			vec3 faceCenter;
			long nbGroup=sceneMesh.GetNumGroups();
			for(long idgroup=0;idgroup<nbGroup;idgroup++)
			{
				E_Scene_Groupesurfaces_Groupe* newGroupe=NULL;
				long nbFace=sceneMesh.GetNumFaces(idgroup);
				for(long idface=0;idface<nbFace;idface++)
				{
					t_faceIndex currentFace(idface,idgroup);
					sceneMesh.GetRealCenterFace(currentFace,&faceCenter);
					std::vector<octreeTool::elementSize> groupIndices;
					std::vector<float> groupDist;
					//Test de collision afin de retourner la liste des indices des anciens groupes selon
					//les coordonnées du centre de la nouvelle face
					oldFacesDistribution->retrieverTool.GetGroupByDotCoord(faceCenter,groupIndices,groupDist,epsilonDist);
					bool onlyPointerGroup=true;
					if(groupIndices.size()>0) //Si un ancien groupe contient cette surface
					{
						float bestSurfaceDist=2*epsilonDist;
						int bestSurfaceIdResult(-1);
						for(octreeTool::elementSize idresult=0;idresult<groupIndices.size();idresult++)
						{
							E_Scene_Groupesurfaces_Groupe* group=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(oldFacesDistribution->containers[groupIndices[idresult]]);
							if(!group->IsPointer())
							{
								if(groupDist[idresult]<bestSurfaceDist)
								{
									bestSurfaceIdResult=idresult;
									bestSurfaceDist=groupDist[idresult];
								}
								onlyPointerGroup=false;
							}else{
								group->AddFace(idface,idgroup);
							}
						}
						if(bestSurfaceIdResult!=-1)
						{
							E_Scene_Groupesurfaces_Groupe* group=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(oldFacesDistribution->containers[groupIndices[(std::size_t)bestSurfaceIdResult]]);
							group->AddFace(idface,idgroup);
						}
					}
					if(groupIndices.size()==0 || onlyPointerGroup)
					{
						//Aucun ancien groupe ne contenait cette surface
						//Nous devons donc l'allouer à un nouveau groupe
						if(!newGroupe)
							newGroupe=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(searchResult->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE,this->sceneMesh.GetNameGroup(idgroup)));
						newGroupe->AddFace(idface,idgroup);
					}
				}
			}
		}
		this->rootScene->ForceBottomModified();
		searchResult->FillWxTree(this->treeScene);
	}
	ProcessUpdateEvent=true;
}


void ProjectManager::UpdateXmlFile(wxString toFolder,bool saveToFile)
{
	wxXmlNode* noeudRacine=this->projetConfig.GetRoot();
	if(!noeudRacine)
		return;
	noeudRacine->DeleteAttribute("appversion");

	wxString Appversion;
	Appversion<<ApplicationConfiguration::SPPS_UI_VERSION_MAJOR<<"."<<ApplicationConfiguration::SPPS_UI_VERSION_MINOR<<"."<<ApplicationConfiguration::SPPS_UI_VERSION_REVISION;
	noeudRacine->AddAttribute("appversion",Appversion);
	Element* eleInfos=this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_PROJET_INFORMATION);
	if(eleInfos)
		eleInfos->InitProp();
	this->rootCore->SaveXMLDoc(noeudRacine);
	this->rootScene->SaveXMLDoc(noeudRacine);
	this->rootResult->SaveXMLDoc(noeudRacine);
	//this->projetConfig.SetFileEncoding("utf-8");
	//this->projetConfig.SetEncoding("utf-8");
	if(saveToFile)
	{
		if(toFolder=="")
			this->projetConfig.Save(this->dossierCourant+FichierConfig); //,wxXML_NO_INDENTATION
		else
			this->projetConfig.Save(toFolder+FichierConfig);
	}
}

void ProjectManager::OpenOsExternalApplication(Element* linkedElement)
{
	E_Report_Unknown* reportEle=dynamic_cast<E_Report_Unknown*>(linkedElement);
	if(reportEle)
	{
		reportEle->OpenFile();
	}
}

void ProjectManager::OpenNewDataWindow(Element* linkedElement)
{
	if(mainFrame)
	{
		E_Report_File* reportFile=dynamic_cast<E_Report_File*>(linkedElement);
		if(reportFile)
		{
			wxString fileFullPath;
			reportFile->BuildFullPath(fileFullPath);
			DataWindow* newWindow=new DataWindow(mainFrame,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_TAB_EXTERNAL_MOVE );
			wxWindowUpdateLocker noUpdates(newWindow); //Bloque la mise à jour de l'affichage

			wxFileName fileOperator(fileFullPath);
			fileOperator.MakeRelativeTo(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_REPORT_FOLDER_PATH);

			wxString shortName(fileOperator.GetFullPath());
			if(shortName.size()>71)
				shortName=".."+fileFullPath.Mid(fileFullPath.size()-71);

			auiManager->AddPane(newWindow, wxAuiPaneInfo().
				Caption(_("Spreadsheet")+wxString(" - ")+shortName).FloatingSize(380,200)
					  .Float().CloseButton(true).MaximizeButton(true).DestroyOnClose(true));

			auiManager->Update();


			newWindow->PushElement(linkedElement);
		}
	}
}


	void ProjectManager::RepairCurrentMesh(const t_param_load_model& paramRepair)
	{
		bool meshModified(false);

		wxString cacheFolder=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+"temp"+wxFileName::GetPathSeparator();
		if(!wxDirExists(cacheFolder))
			wxMkdir(cacheFolder);
		this->ClearFolder(cacheFolder);
		wxString meshName=cacheFolder+"mesh_to_repair.poly";
		//Sauvegarder le modèle ouvert en fichier .poly
		//sceneMesh.Save(meshName);
		//Etape de reconstruction approximative du modèle
		if(paramRepair.launchRemeshWizard)
		{
			meshModified=true;
			OnStartRemeshWizard();
		}
		//2eme étape éxecuter le logiciel de correction de modèle
		if(paramRepair.docorrection)
		{
			meshModified=true;
			if(!wxFileExists(meshName))
			{
				if(paramRepair.domeshsurface)
				{
					sceneMesh._SavePOLY(meshName.ToStdString(),false,false,true,NULL,true);
				}else{
					sceneMesh._SavePOLY(meshName.ToStdString());
				}
			}
			RunRemeshProcess(meshName);
		}
		// Si pas d'amélioration du contour du modèle
		if(paramRepair.domeshsurface)
		{
			if(!wxFileExists(meshName))
				sceneMesh._SavePOLY(meshName.ToStdString(),false,false,true,NULL,true);
			//Save by materials
			
			//Maillage de la bordure
			RunTetGenBoundaryMesh(paramRepair.paramTetgen,cacheFolder,"mesh_to_repair","poly");
			meshModified=true;
		}
		if(paramRepair.docorrection && !paramRepair.domeshsurface)
		{
			//3eme étape, charger le fichier .POLY modifié en gardant les groupes existants et les identifiant de matériaux(couleur d'origine par face) (consistant grâce aux marqueurs par face des fichiers POLY)
			sceneMesh.LoadPolyWithoutLostCurrentModelGroupAndMaterials(meshName.ToStdString());
		}
		//Charger les nouvelles faces du modèle
		if(meshModified)
			LoadFacesFromModel(NULL);
	}




	void ProjectManager::OnMenuSubstractRecepteurSurf( )
	{
		Element* firstFolder=rootResult->GetElementByType(Element::ELEMENT_TYPE_REPORT_FOLDER);
		if(!firstFolder)
			return;
		E_Report_File* reportFolder=dynamic_cast<E_Report_File*>(firstFolder);
		if(!reportFolder)
			return;
		RecepteurSelectionWizard wizardSelectionRs(mainFrame,reportFolder);

		if(wizardSelectionRs.RunWizard(wizardSelectionRs.GetFirstPage()))
		{

			RecepteurSelectionWizard::t_UserSelection userFilesSelection=wizardSelectionRs.GetUserPreferences();

			RecepteurSurfAttenuationComputation rsAttenuation(userFilesSelection.referenceFilePath);

			for(int idfile=0;idfile<userFilesSelection.comparaisonFilesPath.size();idfile++)
			{
				wxFileName outputPath(userFilesSelection.comparaisonFilesPath[idfile]);
				outputPath.SetName(userFilesSelection.comparaisonFilesOutputFileNames[idfile]);
				rsAttenuation.SaveAttenuationRecepteurSurfFile(userFilesSelection.comparaisonFilesPath[idfile],outputPath.GetFullPath());
			}
			RefreshReportFolder();
		}
	}

	void ProjectManager::UpdateSimulationConfig(Element* animationPropertyElement)
	{
		GlFrame->SetSimulationRefreshRate((1/animationPropertyElement->GetDecimalConfig("animation_rate"))*1000);
		particulesContainer.EnableRendering(animationPropertyElement->GetBoolConfig("showparticle"));
		recepteursSContainer.EnableRendering(animationPropertyElement->GetBoolConfig("showrecepteurss"));
	}
