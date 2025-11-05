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

#include "wxGridCellFontEditor.h"
#include <wx/fontdlg.h>
#include "last_cpp_include.hpp"
// ----------------------------------------------------------------------------
// wxGridCellFontEditor
// ----------------------------------------------------------------------------
void wxGridCellFontEditor::BeginEdit(int row, int col, wxGrid* grid)
{
    wxASSERT_MSG(m_control, wxT("The wxGridCellEditor must be created first!"));

	wxFont currentValue = grid->GetCellFont(row,col);
	wxFontData valeurPolice;
	valeurPolice.SetInitialFont(currentValue);
	wxFontDialog ctrlChoixPolice(grid,valeurPolice);

	if(ctrlChoixPolice.ShowModal()==wxID_OK)
	{
		valeurPolice=ctrlChoixPolice.GetFontData();
		wxFont choosenFont=valeurPolice.GetChosenFont();
		grid->SetCellFont(row,col,choosenFont);
		grid->SetCellValue(row,col,choosenFont.GetNativeFontInfoUserDesc());
		wxGridCellTextEditor::BeginEdit(row,col,grid);
		//this->Show(true,this->GetCellAttr());
	}
}

void wxGridCellFontEditor::Show(bool show, wxGridCellAttr *attr)
{
	wxGridCellTextEditor::Show(show,attr);
	m_control->Hide();
}



/*void wxGridCellFontEditor::StartingKey(wxKeyEvent& event)
{
	//event.Skip(true);
}*/

bool wxGridCellFontEditor::EndEdit(int row, int col, const wxGrid *grid,
                         const wxString& oldval, wxString *newval)
{
    return true;
}