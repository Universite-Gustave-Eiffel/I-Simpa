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

#include "e_directivity.h"
#include "data_manager/e_data_file.h"
#include "main/i_simpa-global.h"
#include <wx/filename.h>
#include "last_cpp_include.hpp"

E_Directivity::E_Directivity(Element* parent, wxString Nom, ELEMENT_TYPE _type, wxXmlNode* nodeElement)
	:Element(parent, Nom, _type, nodeElement)
{
	SetIcon(GRAPH_STATE_ALL, GRAPH_ITEM);

	// TODO : Gestion des directivités par appconfig

	if (nodeElement != NULL) // && nodeElement->GetAttribute("wxid",&propVal)
	{
		wxString propValue;

		wxXmlNode* currentChild;
		currentChild = nodeElement->GetChildren();
		wxFileName storageFolder(projetCourant->GetCurrentFolder());
		storageFolder.AppendDir("loudspeaker");
		while (currentChild != NULL)
		{
			if (currentChild->GetAttribute("eid", &propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				if (typeEle == Element::ELEMENT_TYPE_FILE)
				{
					this->AppendFils(new E_Data_File(currentChild, this, storageFolder.GetPath()));
				}
			}
			currentChild = currentChild->GetNext();
		}
	}else{
		Modified(this);
	}
}

void E_Directivity::InitProperties()
{
	wxFileName storageFolder (projetCourant->GetCurrentFolder());
	storageFolder.AppendDir("loudspeaker");
	if (!storageFolder.DirExists())
	{
		storageFolder.Mkdir();
	}
	this->AppendPropertyFile("file", "loudspeaker", storageFolder.GetPath());
}

void E_Directivity::InitProp()
{

}

wxXmlNode* E_Directivity::SaveXMLCoreDoc(wxXmlNode* NoeudParent)
{
	return Element::SaveXMLCoreDoc(NoeudParent);
}

wxXmlNode* E_Directivity::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
	thisNode->SetName("directivities"); // Nom de la balise xml ( pas d'espace autorise )
	return thisNode;
}

void E_Directivity::Modified(Element* eModif)
{
	Element* pereEModif = eModif->GetElementParent();
	if (pereEModif != NULL)
	{
		Element::Modified(eModif); //Element de base modifié
	}
}

