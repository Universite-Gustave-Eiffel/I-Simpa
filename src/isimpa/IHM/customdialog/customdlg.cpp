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

/////////////////////////////////////////////////////////////////////////////
// Name:        customdlg.cpp
// Purpose:     Custom Entry Dialog class
// Author:      Nicolas Fortin
// Modified by:
// Created:     09.01.08
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

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

    #include <wx/utils.h>
    #include <wx/dialog.h>
    #include <wx/button.h>
    #include <wx/stattext.h>
    #include <wx/intl.h>
    #include <wx/sizer.h>
#endif

#if wxUSE_STATLINE
  #include <wx/statline.h>
#endif


#include "customdlg.h"
#include "IHM/PropGrid.h"
#include "last_cpp_include.hpp"


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxCustomEntryDialog
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxCustomEntryDialog, wxDialog)
    EVT_BUTTON(wxID_OK, wxCustomEntryDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, wxCustomEntryDialog::OnCancel)
END_EVENT_TABLE()

IMPLEMENT_CLASS(wxCustomEntryDialog, wxDialog)

wxCustomEntryDialog::wxCustomEntryDialog(wxWindow *parent,
                                         const wxString& message,
                                         const wxString& caption,
                                         const wxPoint& pos)
                   : wxDialog(parent, wxID_ANY, caption,
				   pos, wxDefaultSize,wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxRESIZE_BORDER)
{

	this->SetMinSize(FromDIP(wxSize(320,320)));
    wxBeginBusyCursor();

	wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
#if wxUSE_STATTEXT
    // 1) text message
    topsizer->Add( CreateTextSizer( message ), 0, wxALL, 10 );
#endif

    // 2) prompt and text ctrl
	wxBoxSizer* vinputsizer = new wxBoxSizer(wxHORIZONTAL);

	//vinputsizer->SetVGap(5);
	//vinputsizer->SetHGap(5);

	topsizer->Add( vinputsizer, 1, wxEXPAND | wxTOP|wxBOTTOM, 5 );

    // 3) buttons if any
    wxSizer *buttonSizer = CreateSeparatedButtonSizer(wxOK | wxCANCEL);
    if ( buttonSizer )
    {
        topsizer->Add(buttonSizer, wxSizerFlags().Expand().DoubleBorder());
    }

	PropGrid* newGrid=new PropGrid(this,wxCustomEntryDialog::CUSTOM_DIALOG_CTRLS_GRID);
	vinputsizer->Add( newGrid,1,wxEXPAND);
	//newGrid->SetMinSize(FromDIP(wxSize(300,200)));
	//newGrid->AppendCols();
	newGrid->SetColLabelValue(0,wxGetTranslation("Value"));
	newGrid->SetColLabelAlignment(wxVERTICAL,wxALIGN_LEFT);

    SetSizer( topsizer );
    SetAutoLayout( true );

    topsizer->SetSizeHints( this );
    topsizer->Fit( this );

    Centre( wxBOTH );



    wxEndBusyCursor();
}

void wxCustomEntryDialog::AddListBox( const wxString& label,	const wxString& defaultValue,const wxArrayString& values)
{

	PropGrid* dialogGrid=wxDynamicCast(this->FindWindowById(wxCustomEntryDialog::CUSTOM_DIALOG_CTRLS_GRID),PropGrid);
	if(dialogGrid)
	{
		dialogGrid->AppendRows(1);
		std::size_t idrow(dialogGrid->GetNumberRows()-1);
		dialogGrid->SetRowLabelValue(idrow,label);
		dialogGrid->SetCellEditor(idrow,0,new wxGridCellChoiceEditor(values));
		dialogGrid->SetCellValue(idrow,0,defaultValue);
		dialogGrid->AutoSizeColumns(false);
		dialogGrid->SetRowLabelSize(wxGRID_AUTOSIZE);
		dialogGrid->SetColLabelSize(wxGRID_AUTOSIZE);
	}
	GetSizer()->Fit(this);
}
void wxCustomEntryDialog::AddTextControl( const wxString& label, const wxString& defaultvalue	)
{
	PropGrid* dialogGrid=wxDynamicCast(this->FindWindowById(wxCustomEntryDialog::CUSTOM_DIALOG_CTRLS_GRID),PropGrid);
	if(dialogGrid)
	{
		dialogGrid->AppendRows(1);
		std::size_t idrow(dialogGrid->GetNumberRows()-1);
		dialogGrid->SetRowLabelValue(idrow,label);
		dialogGrid->SetCellValue(idrow,0,defaultvalue);
		dialogGrid->AutoSizeColumns(false);
		dialogGrid->SetRowLabelSize(wxGRID_AUTOSIZE);
		dialogGrid->SetColLabelSize(wxGRID_AUTOSIZE);
	}
	GetSizer()->Fit(this);
}

void wxCustomEntryDialog::GetValues(std::vector<wxString> &values)
{
	PropGrid* dialogGrid=wxDynamicCast(this->FindWindowById(wxCustomEntryDialog::CUSTOM_DIALOG_CTRLS_GRID),PropGrid);
	if(dialogGrid)
	{
		values.reserve(dialogGrid->GetNumberRows());
		for(int i=0;i<dialogGrid->GetNumberRows();i++)
			values.push_back(dialogGrid->GetCellValue(i,0));
	}
}

void wxCustomEntryDialog::OnOK(wxCommandEvent& event)
{


    EndModal(wxID_OK);
}

void wxCustomEntryDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}


