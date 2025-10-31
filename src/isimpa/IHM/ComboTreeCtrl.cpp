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

#include "ComboTreeCtrl.h"
#include "last_cpp_include.hpp"


BEGIN_EVENT_TABLE(ComboTreeCtrl, wxTreeCtrl)
    EVT_MOTION(ComboTreeCtrl::OnMouseMove)
    // NOTE: Left down event is used instead of left up right now
    //       since MSW wxTreeCtrl doesn't seem to emit left ups
    //       consistently.
    EVT_LEFT_DOWN(ComboTreeCtrl::OnMouseClick)
END_EVENT_TABLE()

void ComboTreeCtrl::Init()
{
}

bool ComboTreeCtrl::Create( wxWindow* parent )
    {
        return wxTreeCtrl::Create(parent,1,
                                  wxPoint(0,0),wxDefaultSize,
                                  wxTR_HIDE_ROOT|wxTR_HAS_BUTTONS|
                                  wxTR_SINGLE|wxTR_LINES_AT_ROOT|
                                  wxSIMPLE_BORDER);
    }

void ComboTreeCtrl::OnShow()
    {
        // make sure selected item is visible
        if ( m_value.IsOk() )
            EnsureVisible(m_value);
    }

wxSize ComboTreeCtrl::GetAdjustedSize( int minWidth,
                                    int WXUNUSED(prefHeight),
                                    int maxHeight )
    {
        return wxSize(FromDIP(wxMax(200,minWidth)),FromDIP(wxMin(150,maxHeight)));
    }

wxWindow *ComboTreeCtrl::GetControl() { return this; }

Element* ComboTreeCtrl::GetElementSelection() const
{
	for(int idEl=0;idEl<loadedElements.size();idEl++)
	{
		if(loadedElements[idEl].elementId==m_value)
			return loadedElements[idEl].elementLink;
	}
	return NULL;	
}


wxTreeItemId ComboTreeCtrl::Associate(const wxTreeItemId& newElId,Element* newEl)
{
	loadedElements.push_back(cont_El(newEl,newElId));
	return newElId;
}

void ComboTreeCtrl::SetCurrent( Element* selected )
{
	wxTreeItemId found;

	for(int idEl=0;idEl<loadedElements.size() && !found.IsOk();idEl++)
	{
		if(loadedElements[idEl].elementLink==selected)
			found=loadedElements[idEl].elementId;
	}
	if(found.IsOk() )
	{
        m_value = m_itemHere = found;
		wxTreeCtrl::SelectItem(found,true);
		this->m_combo->SetValue( this->GetStringValue() );
	}
}
void ComboTreeCtrl::SetStringValue( const wxString& s )
{
	this->m_combo->SetValue( this->GetStringValue() );
}

bool ComboTreeCtrl::ElementFitWithFilter(Element* el)
{
	for(std::list<Element::ELEMENT_TYPE>::iterator it=currentFilter.begin();it!=currentFilter.end();it++)
		if(el->IsFittingWithThisType(*it))
			return true;
	return false;
}
void ComboTreeCtrl::AddActiveFilter(Element::ELEMENT_TYPE newFilter)
{
	currentFilter.push_back(newFilter);
}
void ComboTreeCtrl::OnPopup()
{
    if ( m_value.IsOk() )
        return wxTreeCtrl::SelectItem(m_value,true);
}
wxString ComboTreeCtrl::GetStringValue() const
{
    if ( m_value.IsOk() )
        return wxTreeCtrl::GetItemText(m_value);
    return wxEmptyString;
}

//
// Popup event handlers
//

// Mouse hot-tracking
void ComboTreeCtrl::OnMouseMove(wxMouseEvent& event)
{
    int resFlags;
    wxTreeItemId itemHere = HitTest(event.GetPosition(),resFlags);
    if ( itemHere.IsOk() && (resFlags & wxTREE_HITTEST_ONITEMLABEL) )
    {
        wxTreeCtrl::SelectItem(itemHere,true);
        m_itemHere = itemHere;
    }
    event.Skip();
}

// On mouse left, set the value and close the popup
void ComboTreeCtrl::OnMouseClick(wxMouseEvent& event)
{
    int resFlags;
    wxTreeItemId itemHere = HitTest(event.GetPosition(),resFlags);
    if ( itemHere.IsOk() && (resFlags & wxTREE_HITTEST_ONITEMLABEL) )
    {
        m_itemHere = itemHere;
        m_value = itemHere;
        Dismiss();
		wxPostEvent(this,wxCommandEvent(wxEVT_COMMAND_COMBOBOX_SELECTED ,m_combo->GetId()));
    }
    event.Skip();
}