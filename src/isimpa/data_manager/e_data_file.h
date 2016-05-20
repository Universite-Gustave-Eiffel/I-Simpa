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
#include <wx/filename.h>
#include <wx/filefn.h> 
#include <wx/string.h>

/** \file e_data_file.h
\brief Classe spécialisant E_Data afin de représenter un chemin de fichier
*/

/**
\brief Classe spécialisant E_Data afin de représenter un chemin de fichier
@see MainPropGrid
*/

class E_Data_File : public E_Data
{
protected:
	wxFileName file;
	wxFileName storageFolder;
	bool copyToFolder;
public:
	E_Data_File(wxXmlNode* noeudCourant, Element* parent, wxString storageFolder)
		:E_Data(parent, "Unnamedprop", "", Element::ELEMENT_TYPE_FILE, noeudCourant)
	{
		// création depuis le xml
		wxString propFile;
		if (noeudCourant->GetAttribute("value", &propFile))
		{
			this->file.Assign(storageFolder, propFile);
			this->storageFolder.AssignDir(storageFolder);
			copyToFolder = true;
		}
	}

	E_Data_File(Element* parent, wxString dataName, wxString dataLabel, wxString storageFolder)
		:E_Data(parent, dataName, dataLabel, Element::ELEMENT_TYPE_FILE)
	{
		this->file.Assign("Choose a file");
		this->storageFolder.AssignDir(storageFolder);
		copyToFolder = true;
	}

	E_Data_File(Element* parent, wxString dataName, wxString dataLabel)
		:E_Data(parent, dataName, dataLabel, Element::ELEMENT_TYPE_FILE)
	{
		copyToFolder = false;
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Data::SaveXMLDoc(NoeudParent);
		thisNode->DeleteAttribute("value");
		thisNode->AddAttribute("value", file.GetFullName());
		return thisNode;
	}

	virtual void FillWxGrid(wxGrid* gridToFeed, int col = 0)
	{
		E_Data::FillWxGrid(gridToFeed, col);
		int row = gridToFeed->GetNumberRows() - 1;
		gridToFeed->SetCellEditor(row, col, new wxGridCellFileEditor());
		gridToFeed->SetCellValue(row, col, file.GetFullName());
	}

	void SetValue(const wxString& newFilePath)
	{
		wxFileName newFile(newFilePath);

		// if the choosen file is "ok", we can paste it in our storage folder and the field become readOnly
		if (newFile != file)
		{
			if (copyToFolder)
			{
				file.Assign(storageFolder.GetPath() + wxFileName::GetPathSeparator() + newFile.GetFullName());
				wxCopyFile(newFile.GetFullPath(), file.GetFullPath());
			} else 
			{
				file.Assign(newFile.GetFullPath());
			}
			readOnly = true;
			Modified(this);
		}
	}

	virtual void OnValueChanged(wxGridEvent& ev, wxGrid* gridCtrl)
	{
		int row = ev.GetRow();
		int col = ev.GetCol();
		SetValue(gridCtrl->GetCellValue(row, col));
		gridCtrl->SetCellValue(row, col, file.GetFullName());
	}

	wxFileName GetFile() {
		return file;
	}

};

#endif
