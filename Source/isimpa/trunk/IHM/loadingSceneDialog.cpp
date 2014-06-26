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
	SL_CTRLS_ID_COMBO_EPSILON
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


	cb_RemeshModel = new wxCheckBox(this,SL_CTRLS_ID_CHECKBOX_REMESH_MODEL,_("Opération de remaillage approximatif du modèle"));
	cb_RemeshModel->SetToolTip(_("Si cette option est activée un guide vous permettra de remodeliser la scène importée automatiquement, cette scène sera une approximation du modèle originale mais sera compatible avec tout les codes de calculs."));
	cb_RemeshModel->SetValue(false);
	topsizer->Add(cb_RemeshModel,0,wxLEFT | wxTOP, 5);
	//Champ choix de la correction du modèle
	cb_TryToRepairMesh = new wxCheckBox(this,wxID_ANY,_("Tenter de réparer le modèle"));
	cb_TryToRepairMesh->SetToolTip(_("Si cette option est activée alors le modèle importé subira une correction afin d'assurer une meilleure compatibilité avec le mailleur tétraèdrique."));
	cb_TryToRepairMesh->SetValue(true);
	topsizer->Add(cb_TryToRepairMesh,0,wxLEFT | wxTOP,5);
	//Champ choix maillage de surface
	cb_TryToMeshSurface = new wxCheckBox(this,SL_CTRLS_ID_CHECKBOX_SURFACE_MESH,_("Effectuer un maillage de surface"));
	cb_TryToMeshSurface->SetToolTip(_("Les surfaces du modèle importées seront maillées afin d'obtenir une discrétisation en triangles de formes régulières. Cette option est utile pour un meilleur rendu des récepteurs de surface."));
	topsizer->Add(cb_TryToMeshSurface,0,wxLEFT | wxTOP,5);
	// Champ de paramètrage de tetgen
	vinputsizer->Add( new wxStaticText( this, wxID_ANY, _("Paramètres de tetgen") ), 0);
	txt_ParamMesh = new wxTextCtrl(this,wxID_ANY,defaultMeshParams);
	txt_ParamMesh->Disable();
	cb_TryToMeshSurface->SetValue(false);
	vinputsizer->Add( txt_ParamMesh,0,wxGROW| wxALL,1);
	topsizer->Add( vinputsizer, 0, wxEXPAND | wxLEFT, 20);
	//Champ choix de conservation des liens entre les groupes et les faces
	cb_KeepExistingFaceLinks = new wxCheckBox(this,SL_CTRLS_ID_CHECKBOX_RESTOREGROUPS,_("Garder les groupes existants"));
	cb_KeepExistingFaceLinks->SetValue(true);
	cb_KeepExistingFaceLinks->SetToolTip(_("Les surfaces importées ayant la même position que les surfaces du modèle avant l'importation sont associées aux matériaux, récepteur de surfaces et encombrements déjà définies."));
	topsizer->Add(cb_KeepExistingFaceLinks,0,wxLEFT | wxTOP,5);
	//Champ de choix de l'epsilon
	vslidersizer->Add( new wxStaticText( this, wxID_ANY, _("Distance maximale d'association (m)") ), 0);
	//slider_EpsilonLinkingFaceGroupe = new wxSlider(this,wxID_ANY,1,0,500,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
	//vslidersizer->Add(slider_EpsilonLinkingFaceGroupe,0,wxGROW| wxALL,1);
	wxTextCtrl* sliderCtrl=new wxTextCtrl(this, SL_CTRLS_ID_COMBO_EPSILON, epsilonValue, wxDefaultPosition, wxDefaultSize, 0,wxTextValidator(wxFILTER_NUMERIC, &epsilonValue));
	sliderCtrl->SetHelpText(_("Distance maximale d'association en metre "));
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
		wxLogError(_("La valeur epsilon doit être supérieur à 0 m"));
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
	wxLogError(_("Impossible de lire la valeur epsilon"));
	return 0.01f;
}