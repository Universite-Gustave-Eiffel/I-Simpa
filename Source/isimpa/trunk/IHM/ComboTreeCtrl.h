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

#include "first_header_include.hpp"

#include <wx/treectrl.h>
#include <wx/combo.h>
#include "data_manager/element.h"
#include <list>
#ifndef __COMBOTREECTRL_H__
#define __COMBOTREECTRL_H__


/** 
 * @file ComboTreeCtrl.h
 * @brief Vue en arbre dans une liste déroulante. Utilisé pour afficher les matériaux disponibles.
 */

/**
 * @brief Vue en arbre dans une liste déroulante. Utilisé pour afficher les matériaux disponibles.
 */
class ComboTreeCtrl : public wxTreeCtrl, public wxComboPopup
{
	public :
	

    virtual void Init();

    virtual bool Create( wxWindow* parent );

    virtual void OnShow();

    virtual wxSize GetAdjustedSize( int minWidth,
                                    int WXUNUSED(prefHeight),
                                    int maxHeight );

    virtual wxWindow *GetControl();

    // Associe un numero d'élément avec un pointeur d'élément
	// Cet élément devient alors sélectionnable par l'utilisateur
    wxTreeItemId Associate(const wxTreeItemId& newElId,Element* newEl);

    virtual void SetStringValue( const wxString& s );
	virtual void SetCurrent( Element* selected );

    virtual wxString GetStringValue() const;
	virtual Element* GetElementSelection() const;

	//virtual void GetActiveFilter(std::vector<Element::ELEMENT_TYPE>& filter);
	virtual bool ElementFitWithFilter(Element* el);
	virtual void AddActiveFilter(Element::ELEMENT_TYPE newFilter);
    //
    // Popup event handlers
    //

    // Mouse hot-tracking
    void OnMouseMove(wxMouseEvent& event);

    // On mouse left, set the value and close the popup
    void OnMouseClick(wxMouseEvent& event);

	virtual void OnPopup();
protected:
	struct cont_El
	{
		cont_El(Element* _element,wxTreeItemId _elementId):elementLink(_element),elementId(_elementId){}
		Element* elementLink;
		wxTreeItemId elementId;
	};
	std::vector<cont_El> loadedElements;
	std::list<Element::ELEMENT_TYPE> currentFilter;
    wxTreeItemId        m_value; // current item index
    wxTreeItemId        m_itemHere; // hot item in popup
private:
    DECLARE_EVENT_TABLE()
};

#endif