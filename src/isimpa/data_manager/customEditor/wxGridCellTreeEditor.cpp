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

#include "wxGridCellTreeEditor.h"
#include "last_cpp_include.hpp"






wxGridCellTreeEditor::wxGridCellTreeEditor(Element* _rootItem,Element* _defaultSelection,const std::list<Element::ELEMENT_TYPE>& elementFilter)
:wxGridCellEditor(),rootItem(_rootItem),currentSelection(_defaultSelection),currentFilters(elementFilter)
{
	
}

void wxGridCellTreeEditor::BeginEdit(int row, int col, wxGrid* grid)
{
    wxASSERT_MSG(m_control,
                 wxT("The wxGridCellEditor must be created first!"));

    wxEvtHandler* evtHandler = NULL;
    if (m_control)
        evtHandler = m_control->GetEventHandler();

    m_startValue = grid->GetTable()->GetValue(row, col);

   // find the right position, or default to the first if not found
    Combo()->SetValue(m_startValue);

    Combo()->SetInsertionPointEnd();
    Combo()->SetFocus();

	// Save currently edited cell and bind combo closeup with edit end handler
	edited_row = row;
	edited_col = col;
	currentGrid = grid;
	Combo()->Bind(wxEVT_COMBOBOX_CLOSEUP, &wxGridCellTreeEditor::onEndEdit, this);
}

void wxGridCellTreeEditor::onEndEdit(wxCommandEvent& event) {
    wxString value = wxString();
	this->EndEdit(edited_row, edited_col, currentGrid, "", &value);								// Apply changes 'locally'
	wxPostEvent(Combo(), wxGridEvent(0, wxEVT_GRID_CELL_CHANGED, currentGrid, edited_row, edited_col)); //Post event so that e_data_tree action is triggered
	event.Skip();
}

bool wxGridCellTreeEditor::EndEdit(int row, int col, const wxGrid* grid, const wxString& oldval, wxString *newval)
{
	ComboTreeCtrl* popupCtrlTree=dynamic_cast<ComboTreeCtrl*>( Combo()->GetPopupControl());
	if(popupCtrlTree)
	{
		Element* newSelection=popupCtrlTree->GetElementSelection();
		wxString value = Combo()->GetValue();
		*newval=value;
		if ( currentSelection == newSelection )
			return false;

		currentSelection=newSelection;
		grid->GetTable()->SetValue(row, col, value);

		Combo()->Unbind(wxEVT_COMBOBOX_CLOSEUP, &wxGridCellTreeEditor::onEndEdit, this); //Not sure if it is needed(?)
		return true;
	}else{
		Combo()->Unbind(wxEVT_COMBOBOX_CLOSEUP, &wxGridCellTreeEditor::onEndEdit, this);
		return false;
	}
}

Element* wxGridCellTreeEditor::GetSelection()
{
	return currentSelection;
}

/*
    bool wxGridCellTreeEditor::EndEdit(int row, int col, const wxGrid *grid,
                         const wxString& oldval, wxString *newval)
	{
		if(oldval!=GetValue())
		{
			*newval=GetValue();
			return true;
		}else{
			return false;
		}
	}
	*/

    void wxGridCellTreeEditor::ApplyEdit(int row, int col, wxGrid* grid)
	{
		grid->SetCellValue(row,col,GetValue());
	}


wxGridCellEditor *wxGridCellTreeEditor::Clone() const
{
	return new wxGridCellTreeEditor(rootItem,currentSelection,currentFilters);
}

void wxGridCellTreeEditor::Create(wxWindow *parent,wxWindowID id,wxEvtHandler *evtHandler)
{
    wxGenericComboCtrl* NewControl = new wxGenericComboCtrl(parent, id, wxEmptyString,
                               wxDefaultPosition, wxDefaultSize,
                               wxCB_READONLY);

	ComboTreeCtrl* popupCtrlTree=new ComboTreeCtrl();

	for(std::list<Element::ELEMENT_TYPE>::iterator itfilter=currentFilters.begin();itfilter!=currentFilters.end();itfilter++)
		popupCtrlTree->AddActiveFilter(*itfilter);
	NewControl->SetPopupControl(popupCtrlTree);

	rootItem->FillWxTree(popupCtrlTree,popupCtrlTree->AddRoot("root"));
	if(currentSelection)
		popupCtrlTree->SetCurrent(currentSelection);
	
	m_control=NewControl;
    wxGridCellEditor::Create(parent, id, evtHandler);
}

ComboTreeCtrl* wxGridCellTreeEditor::GetTree()
{
	return (ComboTreeCtrl*)Combo()->GetPopupControl();
}
void wxGridCellTreeEditor::Reset(void)
{

    Combo()->SetValue(m_startValue);
    Combo()->SetInsertionPointEnd();
}


wxString wxGridCellTreeEditor::GetValue() const
{
	return Combo()->GetValue();
}
