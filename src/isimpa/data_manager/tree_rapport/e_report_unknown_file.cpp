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

#include "e_report_unknown_file.h"
#include <wx/mimetype.h> //Permet d'associer un fichier inconnu par psps avec tout les fichiers connu par le système d'exploitation
#include <wx/icon.h>
#include <wx/imaglist.h>
#include "last_cpp_include.hpp"

E_Report_Unknown::E_Report_Unknown(Element* parent,wxString Nom,wxString Path)
		:E_Report_File(parent,Nom,Path,ELEMENT_TYPE_REPORT_UNKNOWN)
{


}

E_Report_Unknown::E_Report_Unknown(Element* parent,wxXmlNode* nodeElement)
	:E_Report_File(parent,"Unnamed file","",ELEMENT_TYPE_REPORT_UNKNOWN,nodeElement)
{


}


wxXmlNode* E_Report_Unknown::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = E_Report_File::SaveXMLDoc(NoeudParent);
	thisNode->SetName("unk"); // Nom de la balise xml ( pas d'espace autorise )
	return thisNode;
}

bool E_Report_Unknown::ExtKnownByCurrentOs(const wxString& fileName)
{
	wxFileName fileConstr(fileName);
	if(fileConstr.GetExt().empty())
		return false;
	wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(fileConstr.GetExt());
    if ( !ft )
    {
        return false;
	}else{
		wxString cmd;
		bool ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(fileName));
		delete ft;
		return ok;
	}
}

void E_Report_Unknown::FillWxTree(wxTreeCtrl* treeToFeed,const wxTreeItemId& parentId)
{
	if(!this->elementInfo.idElement.IsOk() && treeToFeed)
	{
		wxFileName fileConstr(this->filePath);
		wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(fileConstr.GetExt());
		if ( ft )
		{
			wxIconLocation iconLocation;
			if(ft->GetIcon(&iconLocation))
			{
				wxIcon appIcon(iconLocation);
				if(wxFileExists(iconLocation.GetFileName()) && appIcon.IsOk())
					this->elementInfo.graphElement=treeToFeed->GetImageList()->Add(appIcon);	
			}
			delete ft;
		}
	}
	E_Report_File::FillWxTree(treeToFeed,parentId);
}
void E_Report_Unknown::OpenFile()
{
	wxFileName fileConstr(this->filePath);
	wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(fileConstr.GetExt());
	if ( ft )
	{
		wxString fullPath;
		this->BuildFullPath(fullPath);
		wxFileName longPath(fullPath);
		longPath.MakeAbsolute();
		wxString cmd;
		bool ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(longPath.GetFullPath()));
		delete ft;
		wxExecute(cmd);
	}
	
}