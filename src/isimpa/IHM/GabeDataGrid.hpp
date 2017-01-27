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
#include <wx/object.h>
#include <wx/dynarray.h>
#include "PropGrid.h"
#include <vector>

/*! \file GabeDataGrid.h
    \brief  Tableur permettant de charger, de modifier et de sauvegarder les fichiers au format Gabe
*/
#ifndef __GABEDATAGRID__
#define __GABEDATAGRID__




// ------ wxGridDoubleArray
// A 2-dimensional array of strings for data values
//
WX_DECLARE_OBJARRAY(double, ArrayOfDouble);
WX_DECLARE_OBJARRAY(ArrayOfDouble, wxGridDoubleArray);


/**
 * Grille de données pouvant contenir à la fois des chaines de caractères ainsi que des nombres
 */

class wxGridMixedTable : public wxGridStringTable
{
    wxDECLARE_DYNAMIC_CLASS(wxGridMixedTable);
public:
    wxGridMixedTable();
    wxGridMixedTable( int numRows, int numCols );
    virtual ~wxGridMixedTable();

    // overridden functions
    void Clear();
    bool InsertRows( size_t pos = 0, size_t numRows = 1 );
    bool AppendRows( size_t numRows = 1 );
    bool DeleteRows( size_t pos = 0, size_t numRows = 1 );
    bool InsertCols( size_t pos = 0, size_t numCols = 1 );
    bool AppendCols( size_t numCols = 1 );
    bool DeleteCols( size_t pos = 0, size_t numCols = 1 );
	void SetValueAsDouble(int row, int col, double value);
	bool CanGetValueAs(int row, int col, const wxString& typeName);
	double GetValueAsDouble(int row, int col);
private:
	wxGridDoubleArray m_datadouble;
};




/**
 *   \brief  Tableur permettant de charger, de modifier et de sauvegarder les fichiers au format Gabe
 *   Contrairement à GabeElPropGrid cette grille contient exactement les données du fichier.
 */

class GabeDataGrid: public PropGrid
{
protected:
	
enum GABE_EVENT_MENU
{
	ID_CREATE_DIAGRAM=PropGrid::EVENT_MENU_LAST+1,
	ID_CREATE_DOCUMENT,
	ID_INSERT_NEW_ROW_BEFORE,
	ID_INSERT_NEW_ROW_AFTER,
	ID_INSERT_NEW_COL_BEFORE,
	ID_INSERT_NEW_COL_AFTER,
	ID_RENAME_COL_LABEL,
	ID_RENAME_ROW_LABEL,
	ID_DELETE_COL,
	ID_DELETE_ROW,
	ID_SAVE,
	ID_SAVE_AS
};
	wxString filePath;
	bool modified;
	virtual void DoFillMenu(wxGridEvent& ev,wxMenu* mainMenu);
	void OnBuildDiagram(wxCommandEvent& event);
	void OnCreateDocument(wxCommandEvent& event);
	void OnInsertRowOrCol(wxCommandEvent& event);
	void OnEditRowOrCol(wxCommandEvent& event);
	void OnSaveDocument(wxCommandEvent& event);

	
	void OnDeleteRow(wxCommandEvent& event);
	void OnDeleteCol(wxCommandEvent& event);

	void SendRefreshFolderEvent();
	void OnCellValueChanged(wxGridEvent& ev);
	bool AskUserWhereToSaveData(wxString& out_filePath);
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
	GabeDataGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr);
	/**
	 * Destructeur
	 */
	~GabeDataGrid();
	/**
	 * Charge les données dans la grille
	 * @param gabeFilePath Chemin du fichier de données
	 */
	void LoadData(wxString gabeFilePath);
	/**
	 * Empeche la modification des données par l'utilisateur
	 */
	void LockUserUpdate(bool readOnly=true);
	/**
	 * Sauvegarde les données de la grille dans un fichier gabe
	 * @param gabeFilePath Chemin du fichier de données
	 */
	void SaveData(wxString gabeFilePath);
	/**
	 * Sauvegarde les données sous forme de fichier CSV (Comma Separated Values)
	 * @param csvFilePath Chemin du fichier de sortie
	 **/
	void SaveDataCSV(wxString csvFilePath);
	/**
	 * Charge les données dans la grille
	 * @param lblCols Libellé des colonnes
	 * @param lblRows Libellé des lignes
	 * @param cells Valeurs texte des cellules
	 * @param cellsValue Valeurs des nombres des cellules
	 */
	void LoadData(std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<wxString>& cells,std::vector<wxFloat32>& cellsValue,std::vector<wxGridCellAttr*>* colsAttr=NULL);

private:	
	void ClearColsAndRows();
	void CopyUserSelectionToArrays(std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<wxString>& cells,std::vector<wxFloat32>& cellsValue, bool initStringCells=true);
    DECLARE_EVENT_TABLE()
};
#endif