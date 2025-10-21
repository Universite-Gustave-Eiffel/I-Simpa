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

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <stdio.h>
	#include <wx/textctrl.h> 
	#include <wx/valtext.h>
    #include <wx/utils.h>
    #include <wx/dialog.h>
    #include <wx/button.h>
    #include <wx/stattext.h>
    #include <wx/textctrl.h>
    #include <wx/checkbox.h>
    #include <wx/intl.h>
    #include <wx/sizer.h>
	#include <wx/slider.h> 
	#include <wx/log.h> 
#endif

#if wxUSE_STATLINE
  #include <wx/statline.h>
#endif

#include "loadingSceneDialog.hpp"
#include "last_cpp_include.hpp"
	
enum SL_CTRLS_ID
{
	SL_CTRLS_ID_CHECKBOX_REMESH_MODEL,
	SL_CTRLS_ID_CHECKBOX_SURFACE_MESH,
	SL_CTRLS_ID_CHECKBOX_RESTOREGROUPS,
	SL_CTRLS_ID_COMBO_EPSILON,
	SL_CTRLS_ID_LIST_UNIT
};

BEGIN_EVENT_TABLE(sceneLoadOptionDialog, wxDialog)
    EVT_BUTTON(wxID_OK, sceneLoadOptionDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, sceneLoadOptionDialog::OnCancel)
	EVT_CHECKBOX( SL_CTRLS_ID_CHECKBOX_SURFACE_MESH , sceneLoadOptionDialog::OnCheckMeshModel )
	EVT_CHECKBOX( SL_CTRLS_ID_CHECKBOX_REMESH_MODEL , sceneLoadOptionDialog::OnLauchRemeshWizard)
	EVT_CHECKBOX( SL_CTRLS_ID_CHECKBOX_RESTOREGROUPS, sceneLoadOptionDialog::OnCheckRestoreGroups)
END_EVENT_TABLE()


IMPLEMENT_CLASS(sceneLoadOptionDialog, wxDialog)




sceneLoadOptionDialog::sceneLoadOptionDialog(wxWindow *parent,
                        const wxString& message,
                        const wxString& caption,
                        const wxString& defaultMeshParams,
                        const wxPoint& pos)
						: wxDialog(parent, wxID_ANY, caption,
                              pos, wxDefaultSize)
							  ,
							  epsilonValue(wxString::Format("%g",0.01))
{

    wxBeginBusyCursor();

	wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );

    // 1) text message
    topsizer->Add( CreateTextSizer( message ), 0, wxALL, 5 );

    // 2) prompt and text ctrl
    wxBoxSizer* vinputsizer = new wxBoxSizer(wxVERTICAL);
    //wxFlexGridSizer* vslidersizer = new wxFlexGridSizer(2,1,0,0);
	wxBoxSizer* vslidersizer=new wxBoxSizer(wxVERTICAL);

	//vinputsizer->SetVGap(5);
	//vinputsizer->SetHGap(5);

	// Import unit choice
	wxArrayString choices;
	choices.push_back(wxGetTranslation("Metric (meters)"));
	choices.push_back(wxGetTranslation("Metric (centimeters)"));
	choices.push_back(wxGetTranslation("Metric (millimeters)"));
	choices.push_back(wxGetTranslation("Imperial (feet)"));
	choices.push_back(wxGetTranslation("Imperial (inches)"));
	choice_ModelImportUnit = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
	choice_ModelImportUnit->SetToolTip(wxGetTranslation("Unit used in this file, I-Simpa will convert it to meters"));
	choice_ModelImportUnit->SetSelection(0);
	topsizer->Add(choice_ModelImportUnit, 0, wxLEFT | wxTOP, 5);

	cb_RemeshModel = new wxCheckBox(this,SL_CTRLS_ID_CHECKBOX_REMESH_MODEL,wxGetTranslation("Average model remesh"));
	cb_RemeshModel->SetToolTip(wxGetTranslation("With this option you are able to remesh the input model to make it compatible with all calculation core. Use this option as a last resort"));
	cb_RemeshModel->SetValue(false);
	topsizer->Add(cb_RemeshModel,0,wxLEFT | wxTOP, 5);
	//Choice of model correction field
	cb_TryToRepairMesh = new wxCheckBox(this,wxID_ANY,wxGetTranslation("Repair model"));
	cb_TryToRepairMesh->SetToolTip(wxGetTranslation("If this option is activated, the scene will be imported with a correction process to improve compatibility with the mesh generator"));
	cb_TryToRepairMesh->SetValue(true);
	topsizer->Add(cb_TryToRepairMesh,0,wxLEFT | wxTOP,5);
	//surface mesh field choice 
	cb_TryToMeshSurface = new wxCheckBox(this,SL_CTRLS_ID_CHECKBOX_SURFACE_MESH,wxGetTranslation("Surface meshing"));
	cb_TryToMeshSurface->SetToolTip(wxGetTranslation("Faces of the scene will be meshed in order to enhance the resolution of the surface receivers display"));
	topsizer->Add(cb_TryToMeshSurface,0,wxLEFT | wxTOP,5);
	// Tetgen settings field
	vinputsizer->Add( new wxStaticText( this, wxID_ANY, wxGetTranslation("TetGen parameters") ), 0);
	txt_ParamMesh = new wxTextCtrl(this,wxID_ANY,defaultMeshParams);
	txt_ParamMesh->Disable();
	cb_TryToMeshSurface->SetValue(false);
	vinputsizer->Add( txt_ParamMesh,0,wxGROW| wxALL,1);
	topsizer->Add( vinputsizer, 0, wxEXPAND | wxLEFT, 20);
	// Scope of choice for the conservation of face allocation to groups
	cb_KeepExistingFaceLinks = new wxCheckBox(this,SL_CTRLS_ID_CHECKBOX_RESTOREGROUPS,wxGetTranslation("Keep existing groups"));
	cb_KeepExistingFaceLinks->SetValue(true);
	cb_KeepExistingFaceLinks->SetToolTip(wxGetTranslation("New faces with the same position than old faces keep materials and surface receivers"));
	topsizer->Add(cb_KeepExistingFaceLinks,0,wxLEFT | wxTOP,5);
	// Epsilon field (merge of vertices)
	vslidersizer->Add( new wxStaticText( this, wxID_ANY, wxGetTranslation("Association maximum distance (m)") ), 0);
	wxTextCtrl* sliderCtrl=new wxTextCtrl(this, SL_CTRLS_ID_COMBO_EPSILON, epsilonValue, wxDefaultPosition, wxDefaultSize, 0,wxTextValidator(wxFILTER_NUMERIC, &epsilonValue));
	sliderCtrl->SetHelpText(wxGetTranslation("Association maximum distance (m)"));
	vslidersizer->Add(sliderCtrl,0,wxGROW| wxALL,1);
	topsizer->Add( vslidersizer, 0, wxEXPAND | wxLEFT, 20);
    // 3) buttons if any
    wxSizer *buttonSizer = CreateSeparatedButtonSizer(wxOK | wxCANCEL);
    if ( buttonSizer )
    {
        topsizer->Add(buttonSizer, wxSizerFlags().Expand().DoubleBorder());
    }

    SetSizer( topsizer );
    SetAutoLayout( true );

    topsizer->SetSizeHints( this );
    topsizer->Fit( this );

    Centre( wxBOTH );

    wxEndBusyCursor();
}


bool sceneLoadOptionDialog::IsRemeshModel()
{
	return cb_RemeshModel->GetValue();
}
void sceneLoadOptionDialog::OnOK(wxCommandEvent& event)
{
	if(GetEpsilonLinkingFaceGroup()<=0.f)
		wxLogError(wxGetTranslation("Epsilon must be higher than 0 meter"));
    else
		EndModal(wxID_OK);
}

void sceneLoadOptionDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
void sceneLoadOptionDialog::OnCheckRestoreGroups(wxCommandEvent& evt)
{
	wxWindow* winTxt=this->FindWindowById(SL_CTRLS_ID_COMBO_EPSILON,this);
	if(winTxt)
	{
		wxTextCtrl* epsilonCtrl=wxDynamicCast(winTxt,wxTextCtrl);
		epsilonCtrl->Enable(IsKeepExistingFaceLinks());
	}
}
void sceneLoadOptionDialog::OnLauchRemeshWizard(wxCommandEvent& evt)
{
	cb_TryToRepairMesh->SetValue(!IsRemeshModel());
}
void sceneLoadOptionDialog::OnCheckMeshModel(wxCommandEvent& evt)
{
	txt_ParamMesh->Enable(IsMeshSurface());
}

wxString sceneLoadOptionDialog::GetMeshParameters()
{
	return txt_ParamMesh->GetValue();
}
bool sceneLoadOptionDialog::IsMeshRepair()
{
	return cb_TryToRepairMesh->GetValue();
}
bool sceneLoadOptionDialog::IsMeshSurface()
{
	return cb_TryToMeshSurface->GetValue();
}

float sceneLoadOptionDialog::GetModelScale() {
	int selection = choice_ModelImportUnit->GetSelection();
	switch (selection)
	{
	case 1:
		//model in centimeter
		return 0.01f;
	case 2:
		// model in millimeter
		return 0.001f;
	case 3:
		// model in feet
		return 0.3048f;
	case 4:
		// model in inch
		return 0.0254f;
	default:
		return 1.0f;
		break;
	}
}

bool sceneLoadOptionDialog::IsKeepExistingFaceLinks()
{
	return cb_KeepExistingFaceLinks->GetValue();
}
float sceneLoadOptionDialog::GetEpsilonLinkingFaceGroup()
{
	//return slider_EpsilonLinkingFaceGroupe->GetValue()/100.f;
	wxDouble value;
	wxWindow* winTxt=this->FindWindowById(SL_CTRLS_ID_COMBO_EPSILON,this);
	if(winTxt)
	{
		wxTextCtrl* epsilonCtrl=wxDynamicCast(winTxt,wxTextCtrl);
		if(epsilonCtrl->GetValue().ToDouble(&value))
			return value;
	}
	wxLogError(wxGetTranslation("Unable to read the epsilon value"));
	return 0.01f;
}
