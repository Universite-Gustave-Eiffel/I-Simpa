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

#include "RecepteurSOperationDialog.h"

#include <wx/frame.h>
#include <wx/sizer.h>
#include "ComboTreeCtrl.h"
#include <wx/stattext.h>
#include <wx/log.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include "last_cpp_include.hpp"

RecepteurSelectionWizard::RecepteurSelectionWizard(wxFrame *frame, E_Report_File* _rootFolder)
        : wxWizard(frame,wxID_ANY,wxGetTranslation("Compare surface receivers"),wxBitmap("./Bitmaps/wiz3.png", wxBITMAP_TYPE_PNG)),
		rootFolder(_rootFolder)
{ 
	wxGetTranslation("&Cancel");
	wxGetTranslation("< &Back");
    m_page1 = new PageReferenceSelection(this);
	PageOtherRecepeurSurfSelection* otherPage=new PageOtherRecepeurSurfSelection(this);
	PageReferenceSelection::Chain(m_page1,otherPage);


    GetPageAreaSizer()->Add(m_page1);
    GetPageAreaSizer()->Add(otherPage);
 
}


RecepteurSelectionWizard::t_UserSelection RecepteurSelectionWizard::GetUserPreferences()
{
	return userSelection;
}

void RecepteurSelectionWizard::RefreshNextButtonState()
{
	wxButton* btNext=wxDynamicCast(wxWindow::FindWindowById(wxID_FORWARD,this),wxButton);
	btNext->SetLabel( HasNextPage(this->GetCurrentPage()) ? wxGetTranslation("&Next >") : wxGetTranslation("&Finish"));
}
BEGIN_EVENT_TABLE(PanelReferenceSelection, wxPanel)
	EVT_COMBOBOX(WIZARDCTRL_REFERENCE_SELECTION,  PanelReferenceSelection::OnRecepteurSurfSelected)
END_EVENT_TABLE()

PanelReferenceSelection::PanelReferenceSelection(wxWindow* parent,RecepteurSelectionWizard *wizManager, wxWindowID id)
		: wxPanel(parent, id)
	{

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

		wxGenericComboCtrl* NewControl = new wxGenericComboCtrl(this, WIZARDCTRL_REFERENCE_SELECTION, wxEmptyString,
                               wxDefaultPosition, wxDefaultSize,
                               wxCB_READONLY);

		ComboTreeCtrl* popupCtrlTree=new ComboTreeCtrl();
		popupCtrlTree->AddActiveFilter(Element::ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_STANDART);
		popupCtrlTree->AddActiveFilter(Element::ELEMENT_TYPE_REPORT_FOLDER);
		NewControl->SetPopupControl(popupCtrlTree);

		wizManager->GetRootFolder()->FillWxTree(popupCtrlTree,popupCtrlTree->AddRoot("root"));

        wxBoxSizer *hboxSizer = new wxBoxSizer(wxHORIZONTAL);
		
        hboxSizer->Add(
            NewControl,
            1, //Stretching
            wxALL,
            5 // Border
        );

		 mainSizer->Add(
		hboxSizer,
            0,
           wxGROW| wxALL,
            5
        );
        SetSizer(mainSizer);
        mainSizer->Fit(this);

	}

void PanelReferenceSelection::OnRecepteurSurfSelected(wxCommandEvent& event)
{
	auto* treeBox = dynamic_cast<wxGenericComboCtrl *>(FindWindowById(WIZARDCTRL_REFERENCE_SELECTION,this));
	if(treeBox)
	{
		// Check the returned control type is the expected one
		const ComboTreeCtrl *popupCtrlTree = dynamic_cast<ComboTreeCtrl *>(treeBox->GetPopupControl());
		if(popupCtrlTree)
		{
			Element* comboElement=popupCtrlTree->GetElementSelection();
			if(comboElement && !comboElement->IsFittingWithThisType(Element::ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION))
			{
				wxLogError(wxGetTranslation("This file or folder doesn't correspond to a compatible surface receivers"));
			}
			
		}
	}
}

E_Report_File* PanelReferenceSelection::GetSelectedReportFile()
{
	auto* treeBox = dynamic_cast<wxGenericComboCtrl *>(FindWindowById(WIZARDCTRL_REFERENCE_SELECTION,this));
	if(treeBox)
	{
		// Check the returned control type is the expected one
		const ComboTreeCtrl *popupCtrlTree = dynamic_cast<ComboTreeCtrl *>(treeBox->GetPopupControl());
		if(popupCtrlTree)
		{
			Element* selectedElement=popupCtrlTree->GetElementSelection();
			if(selectedElement)
			{
					return dynamic_cast<E_Report_File*>(selectedElement);
			}
		}
	}
	return nullptr;
}

BEGIN_EVENT_TABLE(PageReferenceSelection, wxWizardPageSimple)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY,  PageReferenceSelection::OnPageChanging)
END_EVENT_TABLE()
PageReferenceSelection::PageReferenceSelection(RecepteurSelectionWizard *parent)
: wxWizardPageSimple(parent) ,
  wiz_parent(parent)
    { 
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        mainSizer->Add(
			new wxStaticText(this, wxID_ANY,wxGetTranslation("Please, select surface receivers of reference:")),
            0,
            wxALL,
            5
        );

		panelCtrl=new PanelReferenceSelection(this,parent);
		
		mainSizer->Add(
            panelCtrl,
            0,
            wxGROW| wxALL,
            0 
			  );

        SetSizer(mainSizer);
        mainSizer->Fit(this);
    }

void PageReferenceSelection::OnPageChanging(wxWizardEvent& wizEvent)
{
		E_Report_File* selectedFile=panelCtrl->GetSelectedReportFile();
		if(!selectedFile || selectedFile->GetElementInfos().typeElement!=Element::ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION)
		{
			wxLogError(wxGetTranslation("You must select a surface receiver data file to continue"));
			wizEvent.Veto();
			return;
		}
		selectedFile->BuildFullPath(wiz_parent->userSelection.referenceFilePath);
}


BEGIN_EVENT_TABLE(PageOtherRecepeurSurfSelection, wxWizardPageSimple)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY,  PageOtherRecepeurSurfSelection::OnPageChanging)
	EVT_WIZARD_PAGE_CHANGED(wxID_ANY,  PageOtherRecepeurSurfSelection::OnPageChanged)
	EVT_CHECKBOX(WIZARDRSCTRL_CHECKBOX_ANOTHER_RS_ENTRY, PageOtherRecepeurSurfSelection::OnCheck )
END_EVENT_TABLE()
PageOtherRecepeurSurfSelection::PageOtherRecepeurSurfSelection(RecepteurSelectionWizard *parent)
: wxWizardPageSimple(parent) ,
  wiz_parent(parent),
  id_rsarray_entry(-1)
    { 
		nextRSchooser=NULL;
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        mainSizer->Add(
			new wxStaticText(this, wxID_ANY,wxGetTranslation("Surface receiver to be compared to the reference one:")),
            0,
            wxALL,
            5
        );

		panelCtrl=new PanelReferenceSelection(this,parent);
		
		mainSizer->Add(
            panelCtrl,
            0,
			wxGROW| wxALL,
            0 
			  );

        mainSizer->Add(
			new wxStaticText(this, wxID_ANY,wxGetTranslation("Result filename")),
            0,
            wxALL,
            5
        );
        mainSizer->Add(
			new wxTextCtrl(this,WIZARDRSCTRL_TEXTBOX_RESULT_FILENAME,wxGetTranslation("Power gain")),
            0,
            wxALL,
            5
        );

		wxCheckBox* checkAdd=new wxCheckBox(this,WIZARDRSCTRL_CHECKBOX_ANOTHER_RS_ENTRY,wxGetTranslation("Add new data file"));
		checkAdd->SetToolTip(wxGetTranslation("Add next data file"));
        mainSizer->Add(
			checkAdd,
            0,
            wxALL,
            5
        );
        SetSizer(mainSizer);
        mainSizer->Fit(this);
    }

void PageOtherRecepeurSurfSelection::OnPageChanging(wxWizardEvent& wizEvent)
{
		if(wizEvent.GetDirection()) //Si on va en avant
		{
			wxTextCtrl* libelCtrl=wxDynamicCast(wxWindow::FindWindowById(WIZARDRSCTRL_TEXTBOX_RESULT_FILENAME,this),wxTextCtrl);
			E_Report_File* selectedFile=panelCtrl->GetSelectedReportFile();
			//On test si l'utilisateur a bien choisi un fichier compatible
			if(!selectedFile || selectedFile->GetElementInfos().typeElement!=Element::ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION)
			{
				wxLogError(wxGetTranslation("You must select a surface receiver data file to continue"));
				wizEvent.Veto();
				return;
			}
			//Chargement du nom et chemin du fichier
			wxString selectedFileName;
			wxString outputFileName=libelCtrl->GetValue();
			selectedFile->BuildFullPath(selectedFileName);
			//On test si l'utilisateur n'a pas déjà choisi ce fichier dans le traitement, de même pour nom du fichier de sortie
			if(outputFileName.IsEmpty())
			{
				wxLogError(wxGetTranslation("Please, give filename for the output results"));
				wizEvent.Veto();
				return;
			}
			if(selectedFileName==wiz_parent->userSelection.referenceFilePath)
			{
				wxLogError(wxGetTranslation("You have select the same file as the reference one.  Please select another file"));
				wizEvent.Veto();
				return;
			}
			for(wxInt32 identry=0;identry<wiz_parent->userSelection.comparaisonFilesPath.size();identry++)
			{
				if(identry!=id_rsarray_entry)
				{
					if(wiz_parent->userSelection.comparaisonFilesPath[identry]==selectedFileName)
					{
						wxLogError(wxGetTranslation("You have already select this file. Please select another file"));
						wizEvent.Veto();
						return;
					}
					if(outputFileName==wiz_parent->userSelection.comparaisonFilesOutputFileNames[identry])
					{
						wxLogError(wxGetTranslation("You have already used this label as an output data file.  Please select another label"));
						wizEvent.Veto();
						return;
					}
				}
			}
			if(id_rsarray_entry==-1)
			{
				wiz_parent->userSelection.comparaisonFilesPath.push_back(selectedFileName);
				wiz_parent->userSelection.comparaisonFilesOutputFileNames.push_back(outputFileName);
				id_rsarray_entry=wiz_parent->userSelection.comparaisonFilesPath.size()-1;
			}else{
				wiz_parent->userSelection.comparaisonFilesPath[id_rsarray_entry]=selectedFileName;
				wiz_parent->userSelection.comparaisonFilesOutputFileNames[id_rsarray_entry]=outputFileName;
				if(!this->GetNext())
				{
					//Si l'utilisateur est revenu en arrière et a déchoché le choix de poursuivre l'insertion de fichier supplémentaire, il faut supprimer les enregistrements qui suivent l'enregistrement courant
					this->wiz_parent->userSelection.comparaisonFilesOutputFileNames.erase(this->wiz_parent->userSelection.comparaisonFilesOutputFileNames.begin()+id_rsarray_entry+1,this->wiz_parent->userSelection.comparaisonFilesOutputFileNames.end());
					this->wiz_parent->userSelection.comparaisonFilesPath.erase(this->wiz_parent->userSelection.comparaisonFilesPath.begin()+id_rsarray_entry+1,this->wiz_parent->userSelection.comparaisonFilesPath.end());
				}
			}
		}
}
		
void PageOtherRecepeurSurfSelection::OnCheck(wxCommandEvent& ctrlEvent)
{
	wiz_parent->RefreshNextButtonState();
}

void PageOtherRecepeurSurfSelection::OnPageChanged(wxWizardEvent& wizEvent)
{
		if(wizEvent.GetDirection() && !nextRSchooser) //Si on arrive de la page précédente pour la première fois 
		{
			nextRSchooser=new PageOtherRecepeurSurfSelection(wiz_parent);
			nextRSchooser->SetPrev(this);
		}
}


wxWizardPage* PageOtherRecepeurSurfSelection::GetNext() const
{
	wxCheckBox* checkBoxCtrl=wxDynamicCast(wxWindow::FindWindowById(WIZARDRSCTRL_CHECKBOX_ANOTHER_RS_ENTRY,this),wxCheckBox);
	if(checkBoxCtrl)
	{
		if(checkBoxCtrl->GetValue())
		{
			return nextRSchooser;
		}else{
			return NULL;
		}
	}
	return NULL;
}
