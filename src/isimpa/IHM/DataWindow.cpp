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

#include "DataWindow.h"
#include "data_manager/tree_rapport/e_report_gabe.h"
#include "IHM/GabeDataGrid.hpp"
#include "last_cpp_include.hpp"

/*
Evenements disponibles

	wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE
	wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED
	wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING
	wxEVT_COMMAND_AUINOTEBOOK_BUTTON
	wxEVT_COMMAND_AUINOTEBOOK_BEGIN_DRAG
	wxEVT_COMMAND_AUINOTEBOOK_END_DRAG
	wxEVT_COMMAND_AUINOTEBOOK_DRAG_MOTION
	wxEVT_COMMAND_AUINOTEBOOK_ALLOW_DND
	wxEVT_COMMAND_AUINOTEBOOK_TAB_MIDDLE_DOWN
	wxEVT_COMMAND_AUINOTEBOOK_TAB_MIDDLE_UP
	wxEVT_COMMAND_AUINOTEBOOK_TAB_RIGHT_DOWN
	wxEVT_COMMAND_AUINOTEBOOK_TAB_RIGHT_UP
	wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSED
	wxEVT_COMMAND_AUINOTEBOOK_DRAG_DONE
	wxEVT_COMMAND_AUINOTEBOOK_BG_DCLICK
*/

BEGIN_EVENT_TABLE(DataWindow, wxAuiNotebook)
	EVT_AUINOTEBOOK_ALLOW_DND(wxID_ANY, DataWindow::OnAllowNotebookDnD)
	//EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, DataWindow::OnPageClosed)
	//EVT_AUINOTEBOOK_DRAG_DONE(wxID_ANY, DataWindow::OnPageDragAway)
END_EVENT_TABLE()


DataWindow::DataWindow(wxWindow* parent,
                  wxWindowID id,
                  const wxPoint& pos,
                  const wxSize& size,
                  long style)
				  :wxAuiNotebook(parent,id,pos,size,style)
{
}

void DataWindow::PushElement(Element* newElement)
{
	E_Report_Gabe* gabeElement=dynamic_cast<E_Report_Gabe*>(newElement);
	if(gabeElement)
	{
		if(!gabeElement->OpenFileInGrid()) //Si les données doivent être fourni par l'élément 
		{
			std::vector<wxString> lblCols;
			std::vector<wxString> lblRows;
			std::vector<wxString> cells;
			std::vector<float> cellsValue;
			wxString titleArray;
			if(gabeElement->GetArrayData(this,titleArray,lblCols,lblRows,cells,cellsValue))
			{
				GabeDataGrid* newSubWindow= new GabeDataGrid(this,-1);
				newSubWindow->LoadData(lblCols,lblRows,cells,cellsValue);
				newSubWindow->LockUserUpdate();
				this->AddPage(newSubWindow,titleArray,true);
			}
		}else{
			wxString pathGabe;
			gabeElement->BuildFullPath(pathGabe);
			GabeDataGrid* newSubWindow= new GabeDataGrid(this,-1);
			newSubWindow->LoadData(pathGabe);
			this->AddPage(newSubWindow,_(newElement->GetElementInfos().libelleElement),true);
		}
	}
}

void DataWindow::OnAllowNotebookDnD(wxAuiNotebookEvent& evt)
{
	evt.Allow();
}


void DataWindow::OnPageClosed(wxAuiNotebookEvent& evt)
{
	OnDataSrcChange();
	evt.Skip();
}

void DataWindow::OnDataSrcChange()
{
	if(this->GetPageCount()==0)
	{
		wxAuiManager* currentManager=wxAuiManager::GetManager(this);
		if(currentManager)
		{
			currentManager->ClosePane(currentManager->GetPane(this));
			currentManager->Update();
		}
	}
}

void DataWindow::OnPageDragAway(wxAuiNotebookEvent& evt)
{
	evt.Allow();
	OnDataSrcChange();
	evt.Skip();
}