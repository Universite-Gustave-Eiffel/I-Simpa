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

#include "first_header_include.hpp"

#include <wx/grid.h>
#include "IHM/ComboTreeCtrl.h"
#include <list>
/*! \file wxGridCellTreeEditor.h
 *  @brief Editeur de cellule de type arbre
 *  @see E_Data_Tree
 */
/**
 *  @brief Editeur de cellule de type arbre
 *
 *  Lors ce que l'utilisateur désire éditer un champ arbre l'instance de cette classe associé est appelé via wxGridCellTreeEditor::BeginEdit
 *  Lors ce que la grille MainPropGrid affiche ce type de champ elle appelle wxGridCellTreeEditor::Show
 *  A la fin de l'édition du champ la méthode wxGridCellTreeEditor::EndEdit est appelé afin de mettre à jour la cellule texte de base
 *  @see E_Data_Tree
 */
#define wxUSE_TREECOMBOBOX 1
#if wxUSE_TREECOMBOBOX

// the editor for string data allowing to choose from the list of strings
class wxGridCellTreeEditor : public wxGridCellEditor
{
public:
    // if !allowOthers, user can't type a string not in choices array
	wxGridCellTreeEditor(Element* _rootItem,Element* _defaultSelection,const std::list<Element::ELEMENT_TYPE>& elementFilter);

    virtual void Create(wxWindow* parent,
                        wxWindowID id,
                        wxEvtHandler* evtHandler);
	
    virtual bool EndEdit(int row, int col, const wxGrid *grid,
                         const wxString& oldval, wxString *newval);

    virtual void ApplyEdit(int row, int col, wxGrid* grid);

	/**
	 * Début d'édition du champ
	 * @param row Ligne
	 * @param col Colonne
	 * @param grid Pointeur vers le contrôle
	 */
    virtual void BeginEdit(int row, int col, wxGrid* grid);

	/**
	 * Event handler for cell edit end. Called when combobox is closed to automatically apply changes.
	 */
	void onEndEdit(wxCommandEvent& event);

	/**
	 * Fin d'édition du champ
	 * @param row Ligne
	 * @param col Colonne
	 * @param grid Pointeur vers le contrôle
	 */
    //wxwidgets <2.9 virtual bool EndEdit(int row, int col, wxGrid* grid);

    virtual void Reset();

    virtual wxGridCellEditor *Clone() const;

    // added GetValue so we can get the value which is in the control
    virtual wxString GetValue() const;
	Element* GetSelection();

	ComboTreeCtrl* GetTree();
protected:
    wxGenericComboCtrl *Combo() const { return (wxGenericComboCtrl *)m_control; }
	int edited_row, edited_col;
	wxGrid* currentGrid;
	Element *rootItem;
	Element *currentSelection;
	std::list<Element::ELEMENT_TYPE> currentFilters;
    wxString        m_startValue;

    DECLARE_NO_COPY_CLASS(wxGridCellTreeEditor)
};

#endif // wxUSE_COMBOBOX