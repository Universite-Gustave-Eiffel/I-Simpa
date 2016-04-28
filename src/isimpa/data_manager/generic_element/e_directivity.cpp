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
#include "data_manager/e_data_row.h"
#include "last_cpp_include.hpp"

E_Directivity::E_Directivity(Element* parent, wxString Nom, ELEMENT_TYPE _type, wxXmlNode* nodeElement)
	:Element(parent, Nom, _type, nodeElement)
{
	SetIcon(GRAPH_STATE_ALL, GRAPH_ITEM);

	if (nodeElement != NULL) // && nodeElement->GetAttribute("wxid",&propVal)
	{
		//Element initialisé AVEC Xml
	}else{
		//Element initialisé SANS Xml
	}
}

void E_Directivity::InitProperties()
{
	E_Data_Row* newGridLine = new E_Data_Row(this, "filename", "filename");
	newGridLine->AppendPropertyText("filename", "value", "loudspeaker_file.fls", true, true);
	this->AppendFils(newGridLine);
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
