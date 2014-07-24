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

#include "e_userprefitem.hpp"
#include <wx/hashmap.h>
#include <wx/dir.h>
#include <wx/filename.h>

#ifndef __E_USERPREFITEMISO__
#define __E_USERPREFITEMISO__


/*! \file e_userprefitemisotemplate.hpp
    \brief Correspond à l'élément porteur de propriétés iso
*/
/**
	\brief Correspond à l'élément porteur de propriétés iso
	Cette classe permet de placer des propriétés
*/
class E_UserPreferenceItemIsoTemplate: public E_UserPreferenceItem
{
protected:
	bool folder_parsed;
	std::list<wxString> fullPathIso;
public:
	E_UserPreferenceItemIsoTemplate(Element* parent,wxString Nom, wxXmlNode* nodeElement=NULL)
		:E_UserPreferenceItem(parent,Nom,ELEMENT_TYPE_USER_PREFERENCE_ITEM_ISOTEMPLATE,nodeElement)
		,folder_parsed(false)
	{
		if(!nodeElement)
			this->AppendPropertyText("path_isofile","path_isofile",ApplicationConfiguration::CONST_RESOURCE_DATA_FOLDER+ApplicationConfiguration::CONST_RESOURCE_ISO_FOLDER+"jet.gpl")->Hide();
		
	}

	virtual bool IsContainsProperties()
	{
		return true;
	}
	virtual void InitProp()
	{
		if(!folder_parsed)
		{
			wxArrayString files_found;
			wxString folderiso(ApplicationConfiguration::CONST_RESOURCE_FOLDER+ApplicationConfiguration::CONST_RESOURCE_DATA_FOLDER+ApplicationConfiguration::CONST_RESOURCE_ISO_FOLDER);
			int defaultIso(0);
			Element* isoprop(NULL);
			wxStringHash strHash;
			if(this->IsPropertyExist("isofilename",&isoprop))
			{
				defaultIso=this->GetListConfig("isofilename");
				this->DeleteElementByXmlId(isoprop->GetXmlId());
			}else{
				defaultIso=strHash("jet");
			}

			std::vector<wxString> gplFileName;
			std::vector<int> gplFileId;
			if(wxDirExists(folderiso))
			{
				if(wxDir::GetAllFiles(folderiso,&files_found,"*.gpl"))
				{
					gplFileName.reserve(files_found.size());
					gplFileId.reserve(files_found.size());
					for(wxArrayString::iterator itfile=files_found.begin();itfile!=files_found.end();itfile++)
					{
						fullPathIso.push_back((*itfile));
						wxFileName fichGPL((*itfile));
						int idFichier=(int)strHash(fichGPL.GetName());
						gplFileId.push_back(idFichier);
						gplFileName.push_back(fichGPL.GetName());
					}
				}
			}
			this->AppendPropertyList("isofilename","Palette des niveaux",gplFileName,defaultIso,false,1,gplFileId,false);
			_("Iso-levels color");
			folder_parsed=true;
		}
	}
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("prefitemiso"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
	virtual void Modified(Element* eModif)
	{
		if(eModif->GetElementInfos().libelleElement=="isofilename")
		{
			int selectedEl=this->GetListConfig("isofilename");
			wxStringHash strHash;
			for(std::list<wxString>::iterator itpal=this->fullPathIso.begin();itpal!=this->fullPathIso.end();itpal++)
			{
				wxFileName fileiso(*itpal);
				if((int)strHash(fileiso.GetName())==selectedEl)
				{
					this->UpdateStringConfig("path_isofile",(*itpal));
					break;
				}
			}
		}
		E_UserPreferenceItem::Modified(eModif);
	}
};

#endif
