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

#include "e_report_gabe.h"
#include <input_output/gabe/gabe.h>
#include "data_manager/e_data_row.h"
#include <wx/log.h>
#include "last_cpp_include.hpp"
E_Report_Gabe::E_Report_Gabe(Element* parent,wxString Nom,wxString Path,ELEMENT_TYPE etype)
	:E_Report_File(parent,Nom,Path,etype)
{
	SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_GABE);
}

E_Report_Gabe::E_Report_Gabe(Element* parent,wxXmlNode* nodeElement,ELEMENT_TYPE etype)
	:E_Report_File(parent,"Unnamed Array File","",etype,nodeElement)
{
	SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_GABE);
}

wxXmlNode* E_Report_Gabe::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = E_Report_File::SaveXMLDoc(NoeudParent);
	thisNode->SetName("arrayfile"); // Nom de la balise xml ( pas d'espace autorise )
	return thisNode;
}

bool E_Report_Gabe::IsFittingWithThisType( const ELEMENT_TYPE& typeTest )
{
	if(typeTest==ELEMENT_TYPE_REPORT_GABE)
		return true;
	else
		return E_Report_File::IsFittingWithThisType(typeTest);
}
bool E_Report_Gabe::GetArrayData(wxWindow* auiBookWin,wxString& arrayTitle,std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<wxString>& cells,std::vector<float>& cellsValue)
{
	blockUpdateNotification=true;
	wxString fullPath;
	this->BuildFullPath(fullPath);
	//////////////////////////////
	// Initialisation
	cells.clear();
	lblCols.clear();
	lblRows.clear();
	//////////////////////////////
	//Ouverture du fichier binaire

	using namespace formatGABE;
	GABE binImport(fullPath.utf8_str());
	int cols=binImport.GetCols();
	if(cols==0)
		return false;
	//Lecture du libelle des lignes
	GABE_Data_ShortString* dataLbl;
	binImport.GetCol(0,&dataLbl);
	
	int nbCellsRow=0;
	for(int idcol=0;idcol<cols;idcol++)
		if(binImport[idcol])
			if(binImport[idcol]->GetSize()>nbCellsRow)
				nbCellsRow=binImport[idcol]->GetSize();

	lblRows=std::vector<wxString>(nbCellsRow);
	int firstDataColIndex;
	if(dataLbl)
	{
		firstDataColIndex=1;
		for(int idrow=0;idrow<nbCellsRow;idrow++)
			lblRows[idrow]=(*dataLbl)[idrow]->strData;
		arrayTitle=wxString(wxGetTranslation(this->elementInfo.libelleElement))+", "+wxString((*dataLbl).GetLabel());
	}else{
		firstDataColIndex=0;
		for(int idrow=0;idrow<nbCellsRow;idrow++)
			lblRows[idrow]=Convertor::ToString(idrow+1);
		arrayTitle=wxGetTranslation(this->elementInfo.libelleElement);
	}

	lblCols=std::vector<wxString>(cols-firstDataColIndex);
	int nbCellsCol=lblCols.size();
	cells=std::vector<wxString>(nbCellsCol*nbCellsRow);
	cellsValue=std::vector<float>(nbCellsCol*nbCellsRow);

	for(int idcol=firstDataColIndex;idcol<cols;idcol++)
	{
		GABE_Object* newCol=binImport[idcol];
		if(newCol)
		{
			int rows=newCol->GetSize();			 
			lblCols[idcol-firstDataColIndex]=newCol->GetLabel();
			bool feedValue = false;
			//Si cette colonne de données peut être interprété en nombre décimal
			GABE_Data_Float* floatCol;
			if(binImport.GetCol(idcol,&floatCol))
			{
				feedValue = true;
				for(int idrow=0;idrow<rows;idrow++)
				{	
					cellsValue[((idcol-firstDataColIndex)*rows)+idrow]=*(*floatCol)[idrow];
				}
			} else {
				//Si cette colonne de données peut être interprété en nombre entier
				GABE_Data_Integer* intCol;
				if(binImport.GetCol(idcol,&intCol))
				{
					for(int idrow=0;idrow<rows;idrow++)
					{	
						cellsValue[((idcol-firstDataColIndex)*rows)+idrow]=(float)*(*intCol)[idrow];
					}
				}
			}
			// Numeric value cannot be extracted then extract as a string cells
			if(!feedValue) {
				for (int idrow = 0; idrow<rows; idrow++)
				{
					cells[((idcol - firstDataColIndex)*rows) + idrow] = (*newCol).GetStringEquiv(idrow);
				}			
			}
		}else{
			return false;
		}
	}
	return true;
}


bool E_Report_Gabe::OpenFileInGrid()
{
	return true;
}