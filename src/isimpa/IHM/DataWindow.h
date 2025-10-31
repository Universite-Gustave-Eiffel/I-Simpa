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

#include <wx/aui/auibook.h>
#include "data_manager/element.h"
/**
 * @file DataWindow.h
 * @brief Fenêtre contenant un graphique ainsi que plusieurs grilles de données
 *
 */


#ifndef __DATAWINDOW__
#define __DATAWINDOW__
/**
 * @brief Fenêtre contenant un graphique ainsi que plusieurs grilles de données
 *
 */

class DataWindow : public wxAuiNotebook
{
protected:
	/**
	 * Evenement Drag&Drop d'onglet. Permet de refuser ou d'accepter l'import d'un onglet provenant d'une autre fenêtre
	 */
	void OnAllowNotebookDnD(wxAuiNotebookEvent& evt);
	/**
	 * Evenement De fermeture d'un onglet. Le graphique lié à cette fenêtre doit être mis à jour en conséquence.
	 */
	void OnPageClosed(wxAuiNotebookEvent& evt);
	/**
	 * Evenement De déplacement d'un onglet vers un autre contrôle. Le graphique lié à cette fenêtre doit être mis à jour en conséquence.
	 */
	void OnPageDragAway(wxAuiNotebookEvent& evt);
	/**
	 * Lors-ce que la source de données a été modifié (ajout/suppression d'onglet de données)
	 */
	void OnDataSrcChange();
	DECLARE_EVENT_TABLE();
public:
    DataWindow(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxAUI_NB_DEFAULT_STYLE);
	
	/**
	 * Ajout d'une feuille de données dans le contrôle.
	 */
	void PushElement(Element* newElement);

	
};

#endif