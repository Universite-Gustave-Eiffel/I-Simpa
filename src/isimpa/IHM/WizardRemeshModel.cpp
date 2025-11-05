/* ----------------------------------------------------------------------
* I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
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

#include "WizardRemeshModel.hpp"
#include "data_manager/appconfig.h" //Permet d'obtenir les dossier temporaires
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/log.h>
//#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include "last_cpp_include.hpp"

BEGIN_EVENT_TABLE(ModelReconstructionUI::ModelReconstructionWizard,wxWizard)
	EVT_WIZARD_FINISHED(wxID_ANY,  ModelReconstructionUI::ModelReconstructionWizard::OnFinish)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ModelReconstructionUI::PageOneQualityParameters,wxWizardPageSimple)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY,  ModelReconstructionUI::PageOneQualityParameters::OnPageChanging)
	EVT_WIZARD_PAGE_CHANGED(wxID_ANY,  ModelReconstructionUI::PageOneQualityParameters::OnPageChanged)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ModelReconstructionUI::PageTwoVolumeSelection,wxWizardPageSimple)
	EVT_LISTBOX(ModelReconstructionUI::PageTwoVolumeSelection::WIZARD_CTRL_VOLUMES_LIST, ModelReconstructionUI::PageTwoVolumeSelection::OnVolumeSelectionChange)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY,  ModelReconstructionUI::PageTwoVolumeSelection::OnPageChanging)
	EVT_WIZARD_PAGE_CHANGED(wxID_ANY,  ModelReconstructionUI::PageTwoVolumeSelection::OnPageChanged)
END_EVENT_TABLE()
namespace ModelReconstructionUI
{
	
	ModelReconstructionWizard::ModelReconstructionWizard(wxFrame *frame,ModelReconstructionManagerBase* _modelReconstructionManager)
		: wxWizard(frame,wxID_ANY,wxGetTranslation("Mesh correction computation"),wxNullBitmap,wxDefaultPosition,wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxRESIZE_BORDER),
			modelReconstructionManager(_modelReconstructionManager),
			m_page1(NULL)
	{ 
		wxGetTranslation("&Cancel");
		wxGetTranslation("< &Back");
		m_page1 = new PageOneQualityParameters(this);
		PageTwoVolumeSelection* volumeSelection=new PageTwoVolumeSelection(this);
		PageOneQualityParameters::Chain(m_page1,volumeSelection);
		GetPageAreaSizer()->Add(m_page1);
		GetPageAreaSizer()->Add(volumeSelection);
	}
	
	void ModelReconstructionWizard::OnFinish(wxWizardEvent& wizEvent)
	{
		this->modelReconstructionManager->FinalizeModelReconstruction();
	}
	wxControl* PageOneQualityParameters::AddCtrl(wxControl* newCtrl,const wxString& label,wxSizer* mainSizer)
	{
		
		//Boite secondaire pour redimensionner les contrôles horizontalement
		 wxBoxSizer *hboxSizer = new wxBoxSizer(wxHORIZONTAL);

		 wxStaticText* ctrlText=new wxStaticText(this,wxID_ANY,label);


        hboxSizer->Add(
            newCtrl,
            1, //Stretching
            wxALL,
            5 // Border
        );
		//Ajout du 
		 mainSizer->Add(
		ctrlText,
            0,
          wxTOP,
            5
        );

		 mainSizer->Add(
		hboxSizer,
            0,
           wxGROW| wxALL,
            1
        );
		return newCtrl;
	}
	PageOneQualityParameters::PageOneQualityParameters(ModelReconstructionWizard *parent)
		: wxWizardPageSimple(parent)
	{
		ReconstructionUserPreferences_t defaultParameters;
		//Boite principale pour aligner les contrôles à gauche
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

		
	    //Ajout du "slider"
		AddCtrl(new wxSlider(this,WIZARD_CTRL_SLIDER_DEPTH,defaultParameters.depth,5,10,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS),wxGetTranslation("Model solving"),mainSizer);
		//Ajout du choix du mode de triangularisation du modèle
		wxArrayString triangulationMethods;
		triangulationMethods.push_back(wxGetTranslation("Marching cube")); //TRIANGULATION_METHOD_MARCHING_CUBE
		wxComboBox* triangulationMethod= new wxComboBox(this,WIZARD_CTRL_COMBO_TRIANGULATION_MODE,triangulationMethods[defaultParameters.triangulationMethod],wxDefaultPosition,wxDefaultSize,triangulationMethods);
		triangulationMethod->SetSelection(defaultParameters.triangulationMethod);
		AddCtrl(triangulationMethod,wxGetTranslation("Triangulation method"),mainSizer);
		//Ajout de la liste déroulante du choix de la méthode adaptative
		wxArrayString adaptativeMethods;
		adaptativeMethods.push_back(wxGetTranslation("None"));					//ADAPTATIVE_MESH_METHOD_NONE
		adaptativeMethods.push_back(wxGetTranslation("Vertices translation"));	//ADAPTATIVE_MESH_METHOD_VERTICE_TRANSLATION
		wxComboBox* adaptativeMethod=new wxComboBox(this,WIZARD_CTRL_COMBO_ADAPTATIVE_REMESH_MODE,adaptativeMethods[defaultParameters.adaptativeMeshMethod],wxDefaultPosition,wxDefaultSize,adaptativeMethods);
		adaptativeMethod->SetSelection(defaultParameters.adaptativeMeshMethod);
		AddCtrl(adaptativeMethod,wxGetTranslation("Remesh adaptation"),mainSizer);
		//Ajout de la liste déroulante du choix de la méthode de simplification
		wxArrayString simplificationMethods;
		simplificationMethods.push_back(wxGetTranslation("Edge collapse"));		//MESH_SIMPLIFICATION_METHOD_EDGE_COLLAPSE
		wxComboBox* simplificationMethod=new wxComboBox(this,WIZARD_CTRL_COMBO_SIMPLIFICATION_MODE,simplificationMethods[defaultParameters.meshSimplificationMethod],wxDefaultPosition,wxDefaultSize,simplificationMethods);
		simplificationMethod->SetSelection(defaultParameters.meshSimplificationMethod);
		AddCtrl(simplificationMethod,wxGetTranslation("Reduction of the number of surfaces"),mainSizer);
		//Ajout du choix de la qualité min des triangles
		AddCtrl(new wxSlider(this,WIZARD_CTRL_SLIDER_TRIANGLE_QUALITY,(int)(defaultParameters.minTriangleQuality*100),0,62,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS),wxGetTranslation("Quality constraint"),mainSizer);
		//Ajout du choix de l'approximation de la simplification
		AddCtrl(new wxSlider(this,WIZARD_CTRL_SLIDER_EPSILON_MERGE,(int)(defaultParameters.mergeEpsilon*1000),900,1000,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS),wxGetTranslation("Approximation constraint"),mainSizer);
		//Finalisation de l'initialisation des contrôles
        SetSizer(mainSizer);
        mainSizer->Fit(this);
	}

	void PageOneQualityParameters::OnPageChanging(wxWizardEvent& wizEvent)
	{
		//Alimentation de la prochaine page
		ReconstructionUserPreferences_t reconstructionPreferences;
		//Lecture des champs et mise à jour de la structure
		////////////
		// Champ depth
		wxSlider* depthSlider=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_SLIDER_DEPTH,this),wxSlider);
		if(depthSlider)
			reconstructionPreferences.depth=depthSlider->GetValue();
		////////////
		// Champ Triangulation
		wxComboBox* comboTriangulation=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_COMBO_TRIANGULATION_MODE,this),wxComboBox);
		if(comboTriangulation)
			reconstructionPreferences.triangulationMethod=(ReconstructionUserPreferences_t::TRIANGULATION_METHOD)comboTriangulation->GetCurrentSelection();
		////////////
		// Champ AdaptativeMethod
		wxComboBox* comboAdaptative=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_COMBO_ADAPTATIVE_REMESH_MODE,this),wxComboBox);
		if(comboAdaptative)
			reconstructionPreferences.adaptativeMeshMethod=(ReconstructionUserPreferences_t::ADAPTATIVE_MESH_METHOD)comboAdaptative->GetCurrentSelection();
		////////////
		// Champ Simplification
		wxComboBox* comboSimplification=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_COMBO_SIMPLIFICATION_MODE,this),wxComboBox);
		if(comboSimplification)
			reconstructionPreferences.meshSimplificationMethod=(ReconstructionUserPreferences_t::MESH_SIMPLIFICATION_METHOD)comboSimplification->GetCurrentSelection();
		////////////
		// Champ qualitée triangle
		wxSlider* triangleQualitySlider=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_SLIDER_TRIANGLE_QUALITY,this),wxSlider);
		if(triangleQualitySlider)
			reconstructionPreferences.minTriangleQuality=triangleQualitySlider->GetValue()/100.f;
		////////////
		// Champ epsilon fusion triangles
		wxSlider* normalEpsilonFusionSlider=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_SLIDER_EPSILON_MERGE,this),wxSlider);
		if(normalEpsilonFusionSlider)
			reconstructionPreferences.mergeEpsilon=normalEpsilonFusionSlider->GetValue()/1000.f;
		
		PageTwoVolumeSelection* volumeSelWindow=wxDynamicCast(this->GetNext(),PageTwoVolumeSelection);
		if(volumeSelWindow)
		{
			volumeSelWindow->ComputeVolumeListFromParameter(reconstructionPreferences);
		}
	}
	void PageOneQualityParameters::OnPageChanged(wxWizardEvent& wizEvent)
	{
		//Chargement de la page
	}
		

	PageTwoVolumeSelection::PageTwoVolumeSelection(ModelReconstructionWizard *parent)
		: wxWizardPageSimple(parent),
		modelReconstructionManager(parent->GetModelReconstructionManager())
	{
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		 wxBoxSizer *hboxSizer = new wxBoxSizer(wxHORIZONTAL);

		 wxStaticText* ctrlText=new wxStaticText(this,wxID_ANY,wxGetTranslation("Please select the volume to extract"));


    	 wxListBox* volumesLst=new wxListBox(this,WIZARD_CTRL_VOLUMES_LIST,wxDefaultPosition,wxDefaultSize,wxArrayString());


        hboxSizer->Add(
            volumesLst,
            1, //Stretching
           wxGROW| wxALL,
            5 // Border
        );
		//Ajout du 
		 mainSizer->Add(
		ctrlText,
            0,
          wxTOP,
            5
        );

		 mainSizer->Add(
		hboxSizer,
            1, //Stretching
           wxGROW| wxALL,
            1
        );
		//Finalisation de l'initialisation des contrôles
        SetSizer(mainSizer);
        mainSizer->Fit(this);
	}


		void PageTwoVolumeSelection::ComputeVolumeListFromParameter(const ReconstructionUserPreferences_t& reconstructionPreferences)
		{
			localParameters=reconstructionPreferences;
			volumesIds.clear();
			volumesValues.clear();
			//Appel de la fonction de lecture des volumes
			this->modelReconstructionManager->GetAvaibleVolumes(reconstructionPreferences,volumesIds,volumesValues);
			//wxLogMessage(wxString::Format("%i volumes",volumesIds.size()));
			wxListBox* volumesLst=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_VOLUMES_LIST,this),wxListBox);
			if(volumesLst)
			{
				volumesLst->Clear();
				wxArrayString UserSelection;
				UserSelection.reserve(volumesIds.size());
				VolumeIdList_t::const_iterator itvolId=volumesIds.begin();
				VolumeValueList_t::const_iterator itvolValue=volumesValues.begin();
				while(itvolId!=volumesIds.end() && itvolValue!=volumesValues.end())
				{
					UserSelection.push_back(wxString::Format(wxGetTranslation("Vol id:%i  %.0f m3"),*itvolId,*itvolValue));
					itvolId++;
					itvolValue++;
				}
				volumesLst->InsertItems(UserSelection,0);
				if(!volumesIds.empty())
				{
					volumesLst->SetSelection(0,true);
					this->localParameters.volumeIdToExport.push_back(volumesIds.front());
					this->modelReconstructionManager->SetParameters(this->localParameters);
				}
			}
		}

	wxWizardPage* PageTwoVolumeSelection::GetNext() const
	{
		return NULL;
	}
	void PageTwoVolumeSelection::OnPageChanging(wxWizardEvent& wizEvent)
	{

	}
	void PageTwoVolumeSelection::OnPageChanged(wxWizardEvent& wizEvent)
	{


	}
	void PageTwoVolumeSelection::OnVolumeSelectionChange(wxCommandEvent& wizEvent)
	{
		this->localParameters.volumeIdToExport.clear();

		wxListBox* volumesLst=wxDynamicCast(wxWindow::FindWindowById(WIZARD_CTRL_VOLUMES_LIST,this),wxListBox);
		if(volumesLst)
		{
			std::size_t selectionRowNum=volumesLst->GetSelection();
			if(selectionRowNum<volumesIds.size())
			{
				std::vector<VolumeId_t> volVec(volumesIds.begin(),volumesIds.end());
				this->localParameters.volumeIdToExport.push_back(volVec[selectionRowNum]);
			}
		}
		this->modelReconstructionManager->SetParameters(this->localParameters);
	}
};
