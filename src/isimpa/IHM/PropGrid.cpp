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

#include "PropGrid.h"
#include <wx/dcclient.h>
#include <wx/clipbrd.h>
#include <wx/tokenzr.h>
#include <wx/log.h>
#include <wx/dataobj.h>
#include "IHM/customItem/uimenuitem.hpp"

#ifdef __WXDEBUG__
	#include <wx/log.h>
#include "last_cpp_include.hpp"
#endif

BEGIN_EVENT_TABLE( PropGrid, wxGrid )
	EVT_GRID_CELL_RIGHT_CLICK(PropGrid::OnRightClic)
	EVT_MENU(ID_COPIER, PropGrid::Copy)
	EVT_MENU(ID_COLLER, PropGrid::Paste)
	EVT_MENU(ID_COPY_ON_ROW, PropGrid::CopyOnRow)
	EVT_MENU(ID_COPY_ON_COLUMN, PropGrid::CopyOnColumn)
	EVT_GRID_RANGE_SELECT( PropGrid::OnRangeSelected )
	EVT_GRID_SELECT_CELL( PropGrid::OnCellSelected )
	EVT_KILL_FOCUS( PropGrid::OnLostFocus)
END_EVENT_TABLE()

void PropGridSplitString(wxString src,wxString delimiter,wxArrayString& dst)
{
	wxStringTokenizer tkz(src, delimiter);
	while ( tkz.HasMoreTokens() )
	{
		dst.Add(tkz.GetNextToken());
	}
}

PropGrid::PropGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
:wxGrid(parent,id,pos,size,style,name)
{
	allowPaste=true;
	this->CreateGrid(0,1,wxGridSelectCells);
	//Déclaration des raccourcis pour la grille de données ( propriété d'un élément et les feuilles de données)
	wxAcceleratorEntry entries[2];
	entries[0].Set(wxACCEL_CTRL, (int) 'C',     ID_COPIER);
	entries[1].Set(wxACCEL_CTRL,  (int) 'V',     ID_COLLER);
	wxAcceleratorTable accel(2, entries);
	this->SetAcceleratorTable(accel);
	SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
}

void PropGrid::OnLostFocus( wxFocusEvent& ev)
{
	/*
	wxLogWarning("debug lost focus !");
	if(!this->IsMouseInWindow()) // windowGetFocus && this->FindWindowById(windowGetFocus->GetId())==NULL
	{
		this->SaveEditControlValue();
	}
	*/
}

void PropGrid::OnCellSelected( wxGridEvent& ev )
{
	currentSelection.TopRow=ev.GetRow();
	currentSelection.LeftCol=ev.GetCol();
	currentSelection.BottomRow=ev.GetRow();
	currentSelection.RightCol=ev.GetCol();
    ev.Skip();
}
void PropGrid::OnRangeSelected( wxGridRangeSelectEvent& ev )
{

	if(ev.Selecting())
	{
		bool reselect=false;
		//Fusion avec la précédente séléction
		if((currentSelection.TopRow==ev.GetTopRow() && currentSelection.BottomRow==ev.GetBottomRow()))
		{
			currentSelection.LeftCol=wxMin(currentSelection.LeftCol,ev.GetLeftCol());
			currentSelection.RightCol=wxMax(ev.GetRightCol(),currentSelection.RightCol);
		}
		else if(currentSelection.LeftCol==ev.GetLeftCol() && currentSelection.RightCol==ev.GetRightCol())
		{
			currentSelection.TopRow=wxMin(ev.GetTopRow(),currentSelection.TopRow);
			currentSelection.BottomRow=wxMax(ev.GetBottomRow(),currentSelection.BottomRow);
		}else{
			currentSelection.TopRow=ev.GetTopRow();
			currentSelection.LeftCol=ev.GetLeftCol();
			currentSelection.BottomRow=ev.GetBottomRow();
			currentSelection.RightCol=ev.GetRightCol();
		}
		//if(oldSelection.TopRow!=currentSelection.TopRow || oldSelection.LeftCol!=currentSelection.LeftCol || oldSelection.BottomRow!=currentSelection.BottomRow || oldSelection.RightCol!=currentSelection.RightCol)
		if(reselect)
			SelectBlock(currentSelection.TopRow, currentSelection.LeftCol, currentSelection.BottomRow, currentSelection.RightCol);
	}else{
		currentSelection=t_selection();
	}
	#ifdef __WXDEBUG__
	//wxLogDebug("orig:[%i;%i] dest:[%i;%i] sel[%i]",currentSelection.TopRow,currentSelection.LeftCol,currentSelection.BottomRow,currentSelection.RightCol,ev.Selecting());
	#endif

    ev.Skip();
}

void PropGrid::CopyOnRow(wxCommandEvent& event)
{
	wxString cellValue=this->GetCellValue(currentSelection.TopRow,currentSelection.LeftCol);
	for(int col = 0;col < this->GetNumberCols();col++)
	{
		if(col!=currentSelection.LeftCol)
		{
			for(int row = currentSelection.TopRow;row <= currentSelection.BottomRow;row++)
			{
					if(!this->IsReadOnly(row,col))
					{
						wxString cellValue=this->GetCellValue(row,currentSelection.LeftCol);
						setCellValueWithEvt(row,col,cellValue);
					}
			}
		}
	}
}
void PropGrid::CopyOnColumn(wxCommandEvent& event)
{
	for(int row = 0;row < this->GetNumberRows();row++)
	{
		if(row!=currentSelection.TopRow)
		{
			for(int col = currentSelection.LeftCol;col <= currentSelection.RightCol;col++)
			{
				if(!this->IsReadOnly(row,col))
				{
					wxString cellValue=this->GetCellValue(currentSelection.TopRow,col);
					setCellValueWithEvt(row,col,cellValue);
				}
			}
		}
	}
}

void PropGrid::Copy(wxCommandEvent& event)
{
	wxString toutleschamps;
    for ( int row = currentSelection.TopRow; row <= currentSelection.BottomRow; row++ )
    {
		if(row>currentSelection.TopRow)
			toutleschamps<<"\r\n";
		for ( int col = currentSelection.LeftCol; col <= currentSelection.RightCol; col++ )
		{
			if(col>currentSelection.LeftCol)
				toutleschamps<<char(9);
			toutleschamps<<this->GetCellValue(row,col);
		}
	}
    wxTextDataObject* data=new wxTextDataObject;
	data->SetText(toutleschamps);

	wxTheClipboard->SetData(data);
	wxTheClipboard->Flush();
	wxTheClipboard->Close();

}

void PropGrid::setCellValueWithEvt(int row,int col,const wxString& val)
{
	wxDouble dummy;
	long dummyL;
	wxString cellDataType=this->GetTable()->GetTypeName(row,col);
	if(cellDataType==wxGRID_VALUE_STRING
		|| (val.ToLong(&dummyL) && cellDataType==wxGRID_VALUE_NUMBER)
		|| (val.ToDouble(&dummy) && cellDataType==wxGRID_VALUE_FLOAT)
		)
	{
		this->SetCellValue(row,col,val);
		wxGridEvent customGreedEvent(1,wxEVT_GRID_CELL_CHANGED,this,row,col);
		ProcessWindowEvent( customGreedEvent );
	}
}
void PropGrid::Paste(wxCommandEvent& event)
{
	if(!allowPaste)
		return;
    wxTextDataObject data;
	wxTheClipboard->GetData(data);
	wxString dataStr=data.GetText();
	int srcRow,srcCol,nbRowSel,nbColSel;
	srcRow=0;
	srcCol=0;
	nbRowSel=this->GetNumberRows()-currentSelection.TopRow;
	nbColSel=this->GetNumberCols()-currentSelection.LeftCol;
	wxArrayString lignes;
	PropGridSplitString(dataStr,"\r\n",lignes);
	while(srcRow<lignes.Count() && srcRow<nbRowSel) //Pour chaque ligne
	{
		wxArrayString colonnes;
		PropGridSplitString(lignes[srcRow],char(9),colonnes);
		srcCol=0;
		while(srcCol<colonnes.Count() && srcCol<nbColSel) //Pour chaque colonne
		{
			int col=currentSelection.LeftCol+srcCol;
			int row=currentSelection.TopRow+srcRow;
			if(row >= 0 && row < this->GetNumberRows() && col>=0 && col<this->GetNumberCols())
			{
				if(!this->IsReadOnly(row,col))
				{
					setCellValueWithEvt(row,col,colonnes[srcCol]);
				}
			}
			srcCol++;
		}
		srcRow++;
	}


}

void PropGrid::DoFillMenu(wxGridEvent& ev,wxMenu* mainMenu)
{
	mainMenu->Append(ID_COPIER, _("Copy"));
	if(allowPaste)
	{
		mainMenu->Append(ID_COLLER, _("Paste"));
		wxMenu* subMenu=new wxMenu;
		subMenu->Append(ID_COPY_ON_COLUMN,_("to the column"));
		subMenu->Append(ID_COPY_ON_ROW,_("to the row"));
		mainMenu->Append(new wxUiMenuItem(mainMenu, -1,_("Set the same value"),"Set the same value",wxITEM_NORMAL,subMenu));
	}
}
void PropGrid::OnRightClic(wxGridEvent& ev)
{
	wxMenu menuClicDroit;
	this->DoFillMenu(ev,&menuClicDroit);
	if(!this->GetCapture()) //si la souris n'a pas été capturé par une autre fenêtre
		this->PopupMenu(&menuClicDroit);
}

void PropGrid::AutoSizeLibelle( int row )
{
	/*
    wxArrayString lines;
    long w, h;

    // Hide the edit control, so it
    // won't interfere with drag-shrinking.
    if ( IsCellEditControlShown() )
    {
        HideCellEditControl();
        SaveEditControlValue();
    }

    // autosize row height depending on label text
    StringToLines( GetRowLabelValue( row ), lines );
    wxClientDC dc( (wxWindow *)m_rowLabelWin );
    GetTextBoxSize( dc, lines, &w, &h );
	if ( w > GetRowLabelSize())
	{
		SetRowLabelSize(w);
	}
    ForceRefresh();
	*/
	//AutoSizeRowLabelSize(row);
}
