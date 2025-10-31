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

#include <vector>
#include "PropGrid.h"
#include "data_manager/e_data.h"

/*! \file MainPropGrid.h
    \brief Classe d'affichage des propriétés principale (unique instance)

	Affiche sous forme d'une grille les propriétés d'un élément.
	Ces propriétés peuvent être modifiées.
*/
#ifndef __MAINPROPGRID__
#define __MAINPROPGRID__

/**
 * 
 *  \brief Classe d'affichage des propriétés principale (unique instance)
 *
 *	Affiche sous forme d'une grille les propriétés d'un élément.
 *	Ces propriétés peuvent être modifié.
 * 
 */

class MainPropGrid: public PropGrid
{
private:
	enum MAINPROP_EVENT_MENU
	{
		ID_CREATE_DIAGRAM=PropGrid::EVENT_MENU_LAST+1
	};
	struct rowInfo
	{
		E_Data* elementItem;
		long rowNumber;
	};
	std::vector<rowInfo> dataItems;
	wxTreeItemId idCurrentElement;
	Element* CurrentElement; 
	int XmlIdElement;
	void OnCellValueChanged(wxGridEvent& ev);
	void KeyEvt(wxKeyEvent& ev);
    DECLARE_EVENT_TABLE()
	bool reloadElement;
	void OnSelectionChange(wxGridEvent& ev);
	void MouseEvt(wxMouseEvent& ev);
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
	MainPropGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr);
	~MainPropGrid();
	/**
	 * Charge les propriétés d'un élément
	 * @param elementToShowProp Adresse de l'élément à éditer
	 */
	void LoadElement(Element* elementToShowProp);
	/**
	 * Ferme l'élément ouvert
	 */
	void CloseElement();
	/**
	 * Actualise les valeurs des propriétés de l'élément ouvert.
	 */
	void RefreshValues();
	/**
	 * Recupère l'identifiant de l'élément ouvert.
	 * @return Indice de l'Element. Indice invalide si aucun élément ouvert, ou élément non présent dans la fenêtre hierarchique.
	 */
	wxTreeItemId GetTreeIdElementOpen();	
	/**
	 * Recupère l'élément ouvert.
	 * @return Indice xml de l'élément ouvert
	 */
	int GetXmlIdElementOpen();
	virtual void DoFillMenu(wxGridEvent& ev,wxMenu* mainMenu);
	void OnBuildDiagram(wxCommandEvent& event);
	/**
	 * If the grid has been destroyed, alive is false
	 */
	smart_ptr<bool> alive;
};
#endif