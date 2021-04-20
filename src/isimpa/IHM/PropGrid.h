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

#include <wx/grid.h>
#include <wx/menu.h>

/*! \file PropGrid.h
    \brief Classe d'affichage des propriétés

	Affiche sous forme d'une grille des données
*/
#ifndef __PROPGRID__
#define __PROPGRID__

/**
 * 
 *  \brief Classe d'affichage des propriétés
 *
 *	Affiche sous forme d'une grille des données
 */

class PropGrid: public wxGrid
{
protected:
	void setCellValueWithEvt(int row,int col,const wxString& val);
	enum EVENT_MENU
	{
		ID_COPIER,
		ID_COLLER,
		ID_COPY_ON_ROW,
		ID_COPY_ON_COLUMN,
		EVENT_MENU_LAST
	};
	struct t_selection
	{
		t_selection():TopRow(0),LeftCol(0),BottomRow(0),RightCol(0){};
		int TopRow;
		int LeftCol;
		int BottomRow;
		int RightCol;
	};
	t_selection currentSelection;
	void OnRightClic(wxGridEvent& ev);
	virtual void DoFillMenu(wxGridEvent& ev,wxMenu* mainMenu);

	void Copy(wxCommandEvent& event);
	void Paste(wxCommandEvent& event);

	virtual void CopyOnRow(wxCommandEvent& event);
	virtual void CopyOnColumn(wxCommandEvent& event);

	void OnRangeSelected( wxGridRangeSelectEvent& ev );
	void OnCellSelected( wxGridEvent& ev );
	void OnLostFocus( wxFocusEvent& ev);
    DECLARE_EVENT_TABLE()
	
	bool allowPaste;

	//Functions for automatic resizing of last column
	void OnGridWindowSize(wxSizeEvent& event);
	void OnColHeaderSize(wxGridSizeEvent& event);
	void AutoSizeLastCol();
public:
	/**
	 * Constructeur
	 * @param parent Fenêtre parente
	 * @param id Indice de la fenêtre
	 * @param pos Position de la fenêtre 
	 * @param size Taille de la fenêtre
	 * @param style Style de la fenêtre
	 * @param name Libellé de la fenêtre
	 */
	PropGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr);
	/**
	 * Redimensionne les colonne selon le libellé de la ligne row
	 */
	void AutoSizeLibelle( int row );
};
#endif