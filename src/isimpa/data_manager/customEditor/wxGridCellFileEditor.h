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
#include <wx/textctrl.h>

#ifndef __FILE_EDITOR__
#define __FILE_EDITOR__

/*! \file wxGridFileColorEditor.h
*  @brief Editeur de cellule de type fichier
*  @see E_Data_File
*/

/**
*  @brief Editeur de cellule de type fichier
*
*  Lors ce que l'utilisateur d�sire �diter un champ fichier l'instance de cette classe associ� est appel� via wxGridCellFileEditor::BeginEdit
*  Lors ce que la grille MainPropGrid affiche ce type de champ elle appelle wxGridCellFileEditor::Show
*  A la fin de l'�dition du champ la m�thode wxGridCellFileEditor::EndEdit est appel� afin de mettre � jour la cellule texte de base
*  @see E_Data_File
*/

class wxGridCellFileEditor : public wxGridCellTextEditor
{
private:
	wxString dialogTitle;
	wxString fileExtension;
public:
	wxGridCellFileEditor(wxString dialogTitle, wxString fileExtension);

	/**
	* D�but d'�dition du champ
	* @param row Ligne
	* @param col Colonne
	* @param grid Pointeur vers le contr�le
	*/
	virtual void BeginEdit(int row, int col, wxGrid* grid);

	/**
	* Affichage du champ
	* @param show "use the specified attributes to set colours/fonts for it."
	* @param attr Attributs de la cellule associ�e
	*/
	virtual void Show(bool show, wxGridCellAttr *attr);

	/**
	* Fin d'�dition du champ
	* @param row Ligne
	* @param col Colonne
	* @param grid Pointeur vers le contr�le
	*/
	virtual bool EndEdit(int row, int col, const wxGrid *grid,
		const wxString& oldval, wxString *newval);
};

#endif