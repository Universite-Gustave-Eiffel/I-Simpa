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

#include "data_manager/e_data.h"
#ifndef __ELEMENT_FILE__
#define __ELEMENT_FILE__
#include "data_manager/customEditor/wxGridCellFileEditor.h"

/** \file e_data_file.h
\brief Classe spécialisant E_Data afin de représenter un chemin de fichier
*/

/**
\brief Classe spécialisant E_Data afin de représenter un chemin de fichier
@see MainPropGrid
*/

class E_Data_File : public E_Data
{
public:
	E_Data_File(wxXmlNode* noeudCourant, Element* parent)
		:E_Data(parent, "Unnamedprop", "", Element::ELEMENT_TYPE_FILE, noeudCourant)
	{
		// création depuis le xml
	}

	E_Data_File(Element* parent, wxString dataName, wxString dataLabel)
		:E_Data(parent, dataName, dataLabel, Element::ELEMENT_TYPE_FILE)
	{

	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Data::SaveXMLDoc(NoeudParent);
		return thisNode;
	}

	virtual void FillWxGrid(wxGrid* gridToFeed, int col = 0)
	{
		E_Data::FillWxGrid(gridToFeed, col);
		int row = gridToFeed->GetNumberRows() - 1;
		gridToFeed->SetCellEditor(row, col, new wxGridCellFileEditor());
	}

};
#endif
