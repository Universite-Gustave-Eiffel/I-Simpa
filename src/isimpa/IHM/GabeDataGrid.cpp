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

#include "GabeDataGrid.hpp"

#include <wx/wupdlock.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/aui/auibook.h>
#include <wx/log.h>
#include <wx/textdlg.h>
#include <wx/textfile.h>
#include <wx/filename.h>

#include <input_output/gabe/gabe.h>
#include "DialogDiagramCreator.h"
#include <wx/arrimpl.cpp>
#include "data_manager/appconfig.h"
#include "customRenderer/gridcellgabefloatrenderer.hpp"
#include "last_cpp_include.hpp"

WX_DEFINE_OBJARRAY(ArrayOfDouble)
WX_DEFINE_OBJARRAY(wxGridDoubleArray)

wxIMPLEMENT_DYNAMIC_CLASS( wxGridMixedTable, wxGridStringTable )

wxGridMixedTable::wxGridMixedTable()
	:wxGridStringTable()
{

}
wxGridMixedTable::wxGridMixedTable( int numRows, int numCols ):
wxGridStringTable(numRows,numCols)
{
    m_datadouble.Alloc( numRows );

    ArrayOfDouble sa;
    sa.Alloc( numCols );
    sa.Add( 0, numCols );

    m_datadouble.Add( sa, numRows );

}
wxGridMixedTable::~wxGridMixedTable()
{

}

// overridden functions
void wxGridMixedTable::Clear()
{	
	wxGridStringTable::Clear();
	
    int row, col;
    int numRows, numCols;

    numRows = m_datadouble.GetCount();
    if ( numRows > 0 )
    {
        numCols = m_datadouble[0].GetCount();

        for ( row = 0; row < numRows; row++ )
        {
            for ( col = 0; col < numCols; col++ )
            {
                m_datadouble[row][col] = 0;
            }
        }
    }
}
bool wxGridMixedTable::InsertRows( size_t pos , size_t numRows  )
{	
    size_t curNumRows = m_datadouble.GetCount();
    size_t curNumCols = ( curNumRows > 0 ? m_datadouble[0].GetCount() :
                          ( GetView() ? GetView()->GetNumberCols() : 0 ) );

	wxArrayString oldLabelVal;
    if ( pos < curNumRows )
    {
		ArrayOfDouble sa;
		sa.Alloc( curNumCols );
		sa.Add( 0, curNumCols );
		m_datadouble.Insert( sa, pos, numRows );
		for(int idrow=pos;idrow<curNumRows;idrow++)
			oldLabelVal.push_back(GetRowLabelValue(idrow));
    }

	/*
	if ( !m_rowLabels.IsEmpty() )
    {
        m_rowLabels.Insert( wxEmptyString, pos, numRows );

        size_t i;
        for ( i = pos; i < pos + numRows; i++ )
            m_rowLabels[i] = wxGridTableBase::GetRowLabelValue( i );
    }
	*/
	bool res=wxGridStringTable::InsertRows(pos,numRows);
	//<debug wxWidgets pour la mise à jour des libellés
	if(pos < curNumRows )
	{
		curNumRows = m_datadouble.GetCount();
		for(int idrow=pos+1;idrow<curNumRows;idrow++)
			SetRowLabelValue(idrow,oldLabelVal[idrow-pos-1]);
	}
	return res;
}
bool wxGridMixedTable::AppendRows( size_t numRows  )
{	
    size_t curNumRows = m_datadouble.GetCount();
    size_t curNumCols = ( curNumRows > 0
                         ? m_datadouble[0].GetCount()
                         : ( GetView() ? GetView()->GetNumberCols() : 0 ) );

    ArrayOfDouble sa;
    if ( curNumCols > 0 )
    {
        sa.Alloc( curNumCols );
        sa.Add( 0, curNumCols );
    }

    m_datadouble.Add( sa, numRows );
	return wxGridStringTable::AppendRows(numRows);
}
bool wxGridMixedTable::DeleteRows( size_t pos , size_t numRows  )
{	
    size_t curNumRows = m_datadouble.GetCount();

    if ( pos >= curNumRows )
    {
        return wxGridStringTable::DeleteRows(pos,numRows);
    }

    if ( numRows > curNumRows - pos )
    {
        numRows = curNumRows - pos;
    }

    if ( numRows >= curNumRows )
    {
        m_datadouble.Clear();
    }
    else
    {
        m_datadouble.RemoveAt( pos, numRows );
    }
	return wxGridStringTable::DeleteRows(pos,numRows);
}
bool wxGridMixedTable::InsertCols( size_t pos , size_t numCols  )
{	
    size_t row, col;

    size_t curNumRows = m_datadouble.GetCount();
    size_t curNumCols = ( curNumRows > 0
                         ? m_datadouble[0].GetCount()
                         : ( GetView() ? GetView()->GetNumberCols() : 0 ) );

    if ( pos >= curNumCols )
    {
        return wxGridStringTable::InsertCols(pos,numCols);
    }

	for ( row = 0; row < curNumRows; row++ )
    {
        for ( col = pos; col < pos + numCols; col++ )
        {
            m_datadouble[row].Insert( 0., col );
        }
    }
	return wxGridStringTable::InsertCols(pos,numCols);
}
bool wxGridMixedTable::AppendCols( size_t numCols  )
{	
    size_t row;

    size_t curNumRows = m_datadouble.GetCount();

    for ( row = 0; row < curNumRows; row++ )
    {
        m_datadouble[row].Add( 0, numCols );
    }
	return wxGridStringTable::AppendCols(numCols);
}

double wxGridMixedTable::GetValueAsDouble(int row, int col)
{
	return m_datadouble[row][col];
}
void wxGridMixedTable::SetValueAsDouble(int row, int col, double value)
{
    wxCHECK_RET( (row < GetNumberRows()) && (col < GetNumberCols()),
                 _T("invalid row or column index in wxGridStringTable") );

    m_datadouble[row][col] = value;
	wxGridStringTable::SetValue(row,col,wxString::Format("%e",value));
}

bool wxGridMixedTable::CanGetValueAs(int row, int col, const wxString& typeName)
{
	if(typeName==wxGRID_VALUE_FLOAT)
	{
		if(!(m_datadouble[row][col]==0 && GetValue(row,col)!="0")) //Si la donné n'a pas été initialisé les données en chaine de caractère sera utilisé
			return true;
		else
			return false;
	}else{
		return wxGridStringTable::CanGetValueAs(row,col,typeName);
	}
}
bool wxGridMixedTable::DeleteCols( size_t pos , size_t numCols  )
{	
    size_t row;

    size_t curNumRows = m_datadouble.GetCount();
    size_t curNumCols = ( curNumRows > 0 ? m_datadouble[0].GetCount() :
                          ( GetView() ? GetView()->GetNumberCols() : 0 ) );

    if ( pos >= curNumCols )
    {
		return wxGridStringTable::DeleteCols(pos,numCols);
    }

    int colID;
    if ( GetView() )
        colID = GetView()->GetColAt( pos );
    else
        colID = pos;

    if ( numCols > curNumCols - colID )
    {
        numCols = curNumCols - colID;
    }

	for ( row = 0; row < curNumRows; row++ )
    {
        if ( numCols >= curNumCols )
        {
            m_datadouble[row].Clear();
        }
        else
        {
            m_datadouble[row].RemoveAt( colID, numCols );
        }
    }
	return wxGridStringTable::DeleteCols(pos,numCols);
}




BEGIN_EVENT_TABLE( GabeDataGrid, PropGrid )
	EVT_MENU(ID_CREATE_DIAGRAM, GabeDataGrid::OnBuildDiagram)
	EVT_MENU(ID_CREATE_DOCUMENT, GabeDataGrid::OnCreateDocument)
	EVT_MENU(ID_INSERT_NEW_ROW_BEFORE, GabeDataGrid::OnInsertRowOrCol)
	EVT_MENU(ID_INSERT_NEW_ROW_AFTER, GabeDataGrid::OnInsertRowOrCol)
	EVT_MENU(ID_INSERT_NEW_COL_BEFORE, GabeDataGrid::OnInsertRowOrCol)
	EVT_MENU(ID_INSERT_NEW_COL_AFTER, GabeDataGrid::OnInsertRowOrCol)
	EVT_MENU(ID_RENAME_COL_LABEL, GabeDataGrid::OnEditRowOrCol)
	EVT_MENU(ID_RENAME_ROW_LABEL, GabeDataGrid::OnEditRowOrCol)
	EVT_MENU(ID_DELETE_COL, GabeDataGrid::OnDeleteCol)
	EVT_MENU(ID_DELETE_ROW, GabeDataGrid::OnDeleteRow)
	EVT_MENU(ID_SAVE, GabeDataGrid::OnSaveDocument)
	EVT_MENU(ID_SAVE_AS, GabeDataGrid::OnSaveDocument)
	EVT_GRID_CELL_CHANGED( GabeDataGrid::OnCellValueChanged )
END_EVENT_TABLE()

GabeDataGrid::GabeDataGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
:PropGrid(parent,id,pos,size,style,name)
{
	this->SetTable(new wxGridMixedTable(1,1),true);
	allowPaste=true;
	this->SetRowLabelAlignment(wxVERTICAL,wxALIGN_LEFT);
}


GabeDataGrid::~GabeDataGrid()
{
	if(modified && this->IsEditable())
	{
		//On propose à l'utilisateur de sauvegarder les modifications
		wxMessageDialog dialog( NULL, wxGetTranslation("Project has been modified. Save modifications?"),
					wxGetTranslation("Spreadsheet"), wxYES_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		wxCommandEvent cmdEvt;
		switch ( dialog.ShowModal() )
		{
			case wxID_YES:
				if(this->filePath.IsEmpty())	//On demande à l'utilisateur de renseigner le dossier où enregistrer le tableau
				{
					wxString FileName;
					if (AskUserWhereToSaveData(FileName))
					{
						this->SaveData(FileName);
						SendRefreshFolderEvent();
					}
				}else{
					this->SaveData(this->filePath);
				}
				break;
		}
	}
}

void GabeDataGrid::SendRefreshFolderEvent()
{
	wxCommandEvent eventUpdate( wxEVT_PSPS_MAIN_EVENT, GetId() );
	eventUpdate.SetInt(ApplicationConfiguration::MAIN_EVENT_REFRESH_REPORTFOLDER);
	wxWindow* ptDataWindow=this->GetGrandParent();
	if(ptDataWindow && ptDataWindow->GetParent())
		wxPostEvent(ptDataWindow->GetParent(), eventUpdate);
	else
		wxLogMessage(wxGetTranslation("Please, refresh root folder of the data tree")); //en attendant de pouvoir propager correctement l'évenement de réactualisation de l'arbre report

}
bool GabeDataGrid::AskUserWhereToSaveData(wxString& out_filePath)
{

	wxString DefautDir=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_REPORT_FOLDER_PATH;
	wxFileDialog saveFileDialog( this, wxGetTranslation("Save document"), DefautDir, "Data sheet (*.gabe)|*.gabe|CSV  text file (*.csv)|*.csv", wxGetTranslation("Data sheet (*.gabe)|*.gabe|CSV  text file (*.csv)|*.csv"),
														wxFD_SAVE, wxDefaultPosition);
	
	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		out_filePath=saveFileDialog.GetPath();
		return true;
	}else{
		return false;
	}
}
void GabeDataGrid::OnCellValueChanged(wxGridEvent& ev)
{
	ev.Allow();
	modified=true;
	double newVal;
	if(this->GetCellValue(ev.GetRow(),ev.GetCol()).ToDouble(&newVal))
		this->GetTable()->SetValueAsDouble(ev.GetRow(),ev.GetCol(),newVal);
}

void GabeDataGrid::OnDeleteRow(wxCommandEvent& event)
{
	bool success;
	success=this->DeleteRows(this->currentSelection.TopRow);
	if(success)
		modified=true;
}
void GabeDataGrid::OnDeleteCol(wxCommandEvent& event)
{
	bool success;
	success=this->DeleteCols(this->currentSelection.LeftCol);
	if(success)
		modified=true;
}
void GabeDataGrid::OnInsertRowOrCol(wxCommandEvent& event)
{
	bool success;
	if(event.GetId()==ID_INSERT_NEW_ROW_BEFORE)
		success=this->InsertRows(this->currentSelection.BottomRow); 
	else if(event.GetId()==ID_INSERT_NEW_COL_BEFORE)
		success=this->InsertCols(this->currentSelection.LeftCol);
	else if(event.GetId()==ID_INSERT_NEW_ROW_AFTER)
		success=this->InsertRows(this->currentSelection.BottomRow+1); 
	else if(event.GetId()==ID_INSERT_NEW_COL_AFTER)
		success=this->InsertCols(this->currentSelection.LeftCol+1);
	if(success)
		modified=true;
}
void GabeDataGrid::OnEditRowOrCol(wxCommandEvent& event)
{
	wxString messInfo;
	wxString curLabelVal;
	if(event.GetId()==ID_RENAME_ROW_LABEL) //Editer la colonne
	{
		messInfo=wxGetTranslation("Please, give the name of line");
		curLabelVal=this->GetRowLabelValue(this->currentSelection.BottomRow);
	}else{																//Editer la ligne
		messInfo=wxGetTranslation("Please, give the name of column");
		curLabelVal=this->GetColLabelValue(this->currentSelection.LeftCol);
	}
	wxTextEntryDialog fenetreSaisieMessage(this,messInfo,wxGetTranslation(APPLICATION_NAME),curLabelVal);
	if(fenetreSaisieMessage.ShowModal()==wxID_OK)
	{
		curLabelVal=fenetreSaisieMessage.GetValue();
		if(event.GetId()==ID_RENAME_ROW_LABEL) //Editer la colonne
		{
			this->SetRowLabelValue(this->currentSelection.BottomRow,curLabelVal);
		}else{																//Editer la ligne
			this->SetColLabelValue(this->currentSelection.LeftCol,curLabelVal);
		}
		modified=true;
	}
}
void GabeDataGrid::OnSaveDocument(wxCommandEvent& event)
{
	if(event.GetId()==ID_SAVE && !this->filePath.IsEmpty())
	{
		this->SaveData(this->filePath);
	}else{
		if(AskUserWhereToSaveData(this->filePath))
		{
			wxFileName savePath(this->filePath);
			if(savePath.GetExt()=="gabe")
				this->SaveData(this->filePath);
			else if(savePath.GetExt()=="csv")
				this->SaveDataCSV(this->filePath);
			SendRefreshFolderEvent();
		}
	}
}

void GabeDataGrid::OnCreateDocument(wxCommandEvent& event)
{
	wxAuiNotebook* noteBookWin=wxDynamicCast(this->m_parent,wxAuiNotebook);
	std::vector<wxString> lblCols;
	std::vector<wxString> lblRows;
	std::vector<float> cellsValue;
	std::vector<wxString> cells;
	std::vector<wxGridCellAttr*> cellsAttr;

	CopyUserSelectionToArrays(lblCols,lblRows,cells,cellsValue,true);
	GabeDataGrid* newSubWindow= new GabeDataGrid(this,-1);
	newSubWindow->LoadData(lblCols,lblRows,cells,cellsValue,&cellsAttr);
	noteBookWin->AddPage(newSubWindow,wxGetTranslation("New project"));
	noteBookWin->SetSelection(noteBookWin->GetPageCount()-1);
	
}

void GabeDataGrid::CopyUserSelectionToArrays(std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<wxString>& cells,std::vector<wxFloat32>& cellsValue, bool initStringCells)
{

	lblCols=std::vector<wxString>(currentSelection.RightCol-currentSelection.LeftCol+1);
	lblRows=std::vector<wxString>(currentSelection.BottomRow-currentSelection.TopRow+1);
	cellsValue=std::vector<float>(lblCols.size()*lblRows.size());
	if(initStringCells)
		cells=std::vector<wxString>(lblCols.size()*lblRows.size());

	wxInt32 zeroBaseRow=0;
	for ( int row = currentSelection.TopRow; row <= currentSelection.BottomRow; row++ )
	{
		lblRows[zeroBaseRow]=this->GetRowLabelValue(row);
		wxInt32 zeroBaseCol=0;
		for ( int col = currentSelection.LeftCol; col <= currentSelection.RightCol; col++ )
		{
			if(row==currentSelection.TopRow)
				lblCols[zeroBaseCol]=this->GetColLabelValue(col);
			double valCell;
			if(this->GetTable()->CanGetValueAs(row,col,wxGRID_VALUE_FLOAT))
			{
				valCell=this->GetTable()->GetValueAsDouble(row,col);
			}else if(this->GetTable()->CanGetValueAs(row,col,wxGRID_VALUE_NUMBER))
			{
				valCell=(double)this->GetTable()->GetValueAsLong(row,col);
			}else if(!this->GetCellValue(row,col).ToDouble(&valCell))
			{
				//Impossible de convertir
				wxLogError(wxGetTranslation("Value %s can't be converted to decimal number"),this->GetCellValue(row,col));
			}			
			cellsValue[(zeroBaseCol*lblRows.size())+zeroBaseRow]=valCell;
			if(initStringCells)
				cells[(zeroBaseCol*lblRows.size())+zeroBaseRow]=this->GetCellValue(row,col);
			zeroBaseCol++;
		}
		zeroBaseRow++;
	}
}
void GabeDataGrid::OnBuildDiagram(wxCommandEvent& event)
{
	//Ouverture de l'interface de création de graphique
	//Création du dialogue
	BuildGraphSheetDialog* sgCreatorDialog=new BuildGraphSheetDialog(this,this->m_parent,wxID_ANY,wxGetTranslation("New diagram"));

	wxFileName folderExtractor(filePath);
	sgCreatorDialog->SetDataFolder(folderExtractor.GetPath());

	//Ajout des données séléctionnées
	std::vector<wxString> lblCols;
	std::vector<wxString> lblRows;
	std::vector<float> cellsValue;
	std::vector<wxString> cells;

	CopyUserSelectionToArrays(lblCols,lblRows,cells,cellsValue,false);
	sgCreatorDialog->LoadData(lblCols,lblRows,cellsValue);

	sgCreatorDialog->ShowModal();
	wxAuiNotebook* noteBookWin=wxDynamicCast(this->m_parent,wxAuiNotebook);
	if(noteBookWin)
		noteBookWin->SetSelection(noteBookWin->GetPageCount()-1);
}
void GabeDataGrid::DoFillMenu(wxGridEvent& ev,wxMenu* mainMenu)
{
	PropGrid::DoFillMenu(ev,mainMenu);
	//Si la séléction est au moins de 2*1 ou de 1*2 on permet d'afficher un graphique
	wxInt32 RowsSelected=currentSelection.BottomRow-currentSelection.TopRow+1;
	wxInt32 ColsSelected=currentSelection.RightCol-currentSelection.LeftCol+1;
	if(RowsSelected>1 || ColsSelected>1)
	{
		mainMenu->AppendSeparator();
		mainMenu->Append(ID_CREATE_DIAGRAM,wxGetTranslation("New diagram"));
		mainMenu->Append(ID_CREATE_DOCUMENT,wxGetTranslation("Export to an editing spreadsheet"));
	}
	if(this->IsEditable() && (RowsSelected==1 || ColsSelected==1))
	{
		//Une seule cellule selectionné
		mainMenu->AppendSeparator();
		if(RowsSelected==1)
		{
			wxMenu* addRowMenu=new wxMenu();
			addRowMenu->Append(ID_INSERT_NEW_ROW_BEFORE,wxGetTranslation("Before"));
			addRowMenu->Append(ID_INSERT_NEW_ROW_AFTER,wxGetTranslation("After"));
			mainMenu->AppendSubMenu(addRowMenu,wxGetTranslation("Insert new line"));
			mainMenu->Append(ID_DELETE_ROW,wxGetTranslation("Delete line"));
			mainMenu->Append(ID_RENAME_ROW_LABEL,wxGetTranslation("Edit line properties"));
		}
		if(ColsSelected==1 && RowsSelected==1)			
			mainMenu->AppendSeparator();
		if(ColsSelected==1)
		{
			wxMenu* addColMenu=new wxMenu();
			addColMenu->Append(ID_INSERT_NEW_COL_BEFORE,wxGetTranslation("Before"));
			addColMenu->Append(ID_INSERT_NEW_COL_AFTER,wxGetTranslation("After"));
			mainMenu->AppendSubMenu(addColMenu,wxGetTranslation("Insert new column"));
			mainMenu->Append(ID_DELETE_COL,wxGetTranslation("Delete column"));
			mainMenu->Append(ID_RENAME_COL_LABEL,wxGetTranslation("Edit column properties"));
		}
	}
	mainMenu->AppendSeparator();
	if(this->IsEditable())
		mainMenu->Append(ID_SAVE,wxGetTranslation("Save data"));
	mainMenu->Append(ID_SAVE_AS,wxGetTranslation("Save data as..."));
}
void GabeDataGrid::LockUserUpdate(bool readOnly)
{
	this->EnableEditing(!readOnly);
	allowPaste=!readOnly;
}
void GabeDataGrid::SaveData(wxString gabeFilePath)
{
	modified=false;
	using namespace formatGABE;
	GABE binExport(this->GetNumberCols()+1);
	//Création de la colonne des libellés
	GABE_Data_ShortString* rowLabels= new GABE_Data_ShortString(this->GetNumberRows());
	for(int idrow=0;idrow<this->GetNumberRows();idrow++)
	{
		rowLabels->SetString(idrow,this->GetRowLabelValue(idrow));
	}
	binExport.SetCol(0,rowLabels);
	//Création des colonnes du tableau
	for (int idcol = 0; idcol < this->GetNumberCols(); idcol++)
	{
		GABE_Object* newCol;
		wxString cellDataType=this->GetTable()->GetTypeName(0,idcol);
		if(this->GetTable()->CanGetValueAs(0,idcol,wxGRID_VALUE_FLOAT))
		{
			GABE_Data_Float* newColFloat=new GABE_Data_Float(this->GetNumberRows());
			newCol=newColFloat;
			double convertedValue;
			for(int idrow=0;idrow<this->GetNumberRows();idrow++)
			{
				if(this->GetCellValue(idrow,idcol).ToDouble(&convertedValue))
					newColFloat->Set(idrow,(Floatb)convertedValue);
			}

			wxGridCellGabeFloatRenderer* floatRenderer=dynamic_cast<wxGridCellGabeFloatRenderer*>(this->GetCellRenderer(0,idcol));
			if(floatRenderer)
				newColFloat->headerData.numOfDigits=floatRenderer->GetPrecision();
		}else if(this->GetTable()->CanGetValueAs(0,idcol,wxGRID_VALUE_NUMBER))
		{
			GABE_Data_Integer* newColInt=new GABE_Data_Integer(this->GetNumberRows());
			newCol=newColInt;
			long convertedValue;
			for(int idrow=0;idrow<this->GetNumberRows();idrow++)
			{
				if(this->GetCellValue(idrow,idcol).ToLong(&convertedValue))
					newColInt->Set(idrow,(Intb)convertedValue);
			}
		}else{
			GABE_Data_ShortString* newColString=new GABE_Data_ShortString(this->GetNumberRows());
			newCol=newColString;
			for(int idrow=0;idrow<this->GetNumberRows();idrow++)
			{
				newColString->SetString(idrow,this->GetCellValue(idrow,idcol));
			}
		}
		newCol->SetLabel(this->GetColLabelValue(idcol));
		binExport.SetCol(idcol+1,newCol);
	}
	binExport.Save(std::string(gabeFilePath.utf8_str()));
}
void GabeDataGrid::SaveDataCSV(wxString csvFilePath)
{
	wxTextFile outfile(csvFilePath);
	outfile.Create();
	if(!wxFileExists(csvFilePath))
	{
		wxLogError(wxGetTranslation("Unable to write this file with this destination folder!"));
		return;
	}
	wxString lineOfText;
	for (int idcol = 0; idcol < this->GetNumberCols(); idcol++)
	{
		lineOfText+=";";
		lineOfText+=this->GetColLabelValue(idcol);
	}
	outfile.AddLine(lineOfText);
	for (int idrow = 0; idrow < this->GetNumberRows(); idrow++)
	{
		lineOfText.clear();
		lineOfText+=this->GetRowLabelValue(idrow);
		//Création des colonnes du tableau		
		for (int idcol = 0; idcol < this->GetNumberCols(); idcol++)
		{
			lineOfText+=";";
			if(this->GetTable()->CanGetValueAs(0,idcol,wxGRID_VALUE_FLOAT))
			{
				double cellvalue=this->GetTable()->GetValueAsDouble(idrow,idcol);
				lineOfText+=wxString::Format("%e",cellvalue);
			}else{
				lineOfText+=this->GetCellValue(idrow,idcol);
			}
		}
		outfile.AddLine(lineOfText);
		outfile.Write();
	}
	outfile.Close();
}
void GabeDataGrid::LoadData(wxString gabeFilePath)
{
	modified=false;
	wxWindowUpdateLocker noUpdates(this); //Bloque la mise à jour de l'affichage
	//Lecture du fichier
	ClearColsAndRows();
	filePath=gabeFilePath;
	using namespace formatGABE;
	GABE binImport(gabeFilePath.utf8_str());
	if(binImport.GetCols()>0)	
	{
		LockUserUpdate(binImport.IsReadOnly());
		int firstgabecol=1;
		//Lecture de la première colonne et affectation dans les libellés des lignes
		formatGABE::GABE_Data_ShortString* gabeData=NULL;
		binImport.GetCol(0,&gabeData);
		if(gabeData)
		{
			this->InsertRows(0,gabeData->GetSize());
			for(int idrow=0;idrow<gabeData->GetSize();idrow++)
			{
				this->SetRowLabelValue(idrow, wxGetTranslation(gabeData->GetStringEquiv(idrow)));
				AutoSizeLibelle(idrow);
			}
		}else{
			firstgabecol=0;
		}
		this->InsertCols(0,binImport.GetCols()-firstgabecol);
		//Lecture des autres colonnes et affectation du type de données
		
		for(int idcol=firstgabecol;idcol<binImport.GetCols();idcol++)
		{
			GABE_Object* newCol=binImport[idcol];
			if(newCol)
			{
				int rows=newCol->GetSize();	
				if(rows>this->GetNumberRows())
					this->AppendRows(rows-this->GetNumberRows());
				
				wxString labcol(newCol->GetLabel());
				if(labcol.Len()>0)
					SetColLabelValue(idcol-firstgabecol,labcol);
				if(newCol->GetObjectType()==formatGABE::GABE_OBJECTTYPE_FLOAT)
				{
					//Si cette colonne de données peut être interprété en nombre décimal
					GABE_Data_Float* floatCol;
					if(binImport.GetCol(idcol,&floatCol))
					{
						for(int idrow=0;idrow<rows;idrow++)
						{	
							this->GetTable()->SetValueAsDouble(idrow,idcol-firstgabecol,*(*floatCol)[idrow]);
						}
						wxGridCellAttr* attrCol=GetOrCreateCellAttr(0, idcol - firstgabecol);
						attrCol->SetEditor(new wxGridCellFloatEditor(-1, floatCol->headerData.numOfDigits));
						attrCol->SetRenderer(new wxGridCellGabeFloatRenderer(floatCol->headerData.numOfDigits));
						SetColAttr(idcol-firstgabecol,attrCol);
					}
				}else if(newCol->GetObjectType()==formatGABE::GABE_OBJECTTYPE_INT)
				{
					for(int idrow=0;idrow<rows;idrow++)
					{	
						SetCellValue(idrow,idcol-firstgabecol,newCol->GetStringEquiv(idrow));
					}
					//Si cette colonne de données peut être interprété en nombre entier
					GABE_Data_Integer* intCol;
					if(binImport.GetCol(idcol,&intCol))
					{
						wxGridCellAttr* attrCol=GetOrCreateCellAttr(0, idcol - firstgabecol);
						attrCol->SetEditor(new wxGridCellNumberEditor());
						attrCol->SetRenderer(new wxGridCellNumberRenderer());
						SetColAttr(idcol-firstgabecol,attrCol);
					}
				}else{
					for(int idrow=0;idrow<rows;idrow++)
					{	
						SetCellValue(idrow,idcol-firstgabecol,newCol->GetStringEquiv(idrow));
					}
				}
			}
		}
	}else{
		wxLogError(wxGetTranslation("Corrupted file or file doesn't exist"));
	}
	//Redimensionnement
	AutoSizeColumns(false);	
	//AutoSizeRows(false);
	this->SetRowLabelSize(wxGRID_AUTOSIZE);
	this->SetColLabelSize(wxGRID_AUTOSIZE);
}

void GabeDataGrid::ClearColsAndRows()
{
	if(this->GetNumberRows()>0)
		this->DeleteRows(0,this->GetNumberRows());
	if(this->GetNumberCols()>0)
		this->DeleteCols(0,this->GetNumberCols());

}


void GabeDataGrid::LoadData(std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<wxString>& cells,std::vector<wxFloat32>& cellsValue,std::vector<wxGridCellAttr*>* colsAttr)
{
	modified=true;
	ClearColsAndRows();
	wxWindowUpdateLocker noUpdates(this); //Bloque la mise à jour de l'affichage
	int nbrowel=lblRows.size();
	int nbcolel=lblCols.size();
 
	///////////////////
	// Redimensionnement des colonnes
	if(this->GetNumberRows()>nbrowel)
		this->DeleteRows(0,nbrowel-this->GetNumberRows());
	else if(this->GetNumberRows()<nbrowel)
		this->AppendRows(nbrowel-this->GetNumberRows());
	if(this->GetNumberCols()>nbcolel)
		this->DeleteCols(0,nbcolel-this->GetNumberCols()-1);
	else if(this->GetNumberCols()<nbcolel)
		this->AppendCols(nbcolel-this->GetNumberCols());

	//Affectation des libellés des colonnes
	for(int idcol=0;idcol<nbcolel;idcol++)
	{
		if((colsAttr==NULL || colsAttr->size()<idcol) && (cellsValue.size() == nbrowel*nbcolel && cells.empty()))
		{
			wxGridCellAttr* attrCol=GetOrCreateCellAttr(0, idcol);
			attrCol->SetEditor(new wxGridCellFloatEditor());
			attrCol->SetRenderer(new wxGridCellGabeFloatRenderer(4));
			SetColAttr(idcol,attrCol);
		}else{
			if(colsAttr)
			{
				if(colsAttr->size()>idcol && colsAttr->at(idcol))
					SetColAttr(idcol,colsAttr->at(idcol));
			}
		}
		this->SetColLabelValue(idcol,lblCols[idcol]);
	}
	//Affectation des libellés des lignes
	for(int idrow=0;idrow<nbrowel;idrow++)
	{
		this->SetRowLabelValue(idrow,lblRows[idrow]);
		AutoSizeLibelle( idrow );
	}
	
	//Affectation des valeurs des cellules

	if(cells.size()>=cellsValue.size())
	{
		if(cells.size()==nbrowel*nbcolel)
		{
			for(int idrow=0;idrow<nbrowel;idrow++)
			{
				for(int idcol=0;idcol<nbcolel;idcol++)
				{
					this->SetCellValue(idrow,idcol,cells[(idcol*nbrowel)+idrow]);
				}
			}
		}
	}else{
		if(cellsValue.size()==nbrowel*nbcolel)
		{
			for(int idrow=0;idrow<nbrowel;idrow++)
			{
				for(int idcol=0;idcol<nbcolel;idcol++)
				{
					this->GetTable()->SetValueAsDouble(idrow,idcol,cellsValue[(idcol*nbrowel)+idrow]);
				}
			}
		}
	}
	//Redimensionnement
	AutoSizeColumns(false);	
	//AutoSizeRows(false);
	this->SetRowLabelSize(wxGRID_AUTOSIZE);
	this->SetColLabelSize(wxGRID_AUTOSIZE);
}
