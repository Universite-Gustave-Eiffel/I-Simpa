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

#include <wx/panel.h>
#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>
#include <vector>
#include "simpleGraphEnumerators.h"

#ifndef __DIALOG_DIAGRAM_CREATOR__
#define __DIALOG_DIAGRAM_CREATOR__

/**
 * @brief Fenetre de configuration d'ajout de graphique
 */
class BuildGraphSheetDialog : public wxPropertySheetDialog
{
	DECLARE_CLASS(BuildGraphSheetDialog)
public:
	BuildGraphSheetDialog(wxWindow* parent,wxWindow* auiNotebookParent, wxWindowID id,
                   const wxString& title,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& sz = wxDefaultSize);
	void LoadData(std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<float>& cellsValue);
	/**
	 * Optionnel, Dossier où sont stocké les données du graphique.
	 */
	void SetDataFolder(const wxString& grid_folder) { dataFolder=grid_folder;}
protected:
	void OnOK(wxCommandEvent& event);

	std::vector<wxString> cols;
	std::vector<wxString> rows;
	std::vector<float> curveValues;

	wxPanel* CreateMainSettingsPage(wxWindow* parent);
	sgSpace::StyleArray drawingMethodStyles;
	wxString dataFolder;
private:
	wxWindow* auiNotebook;
	DECLARE_EVENT_TABLE()
};

#endif
