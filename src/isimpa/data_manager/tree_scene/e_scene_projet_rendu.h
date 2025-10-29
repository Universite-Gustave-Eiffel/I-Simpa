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

#include "data_manager/element.h"
#include "e_scene_projet_rendu_origine.h"

/** \file e_scene_projet_rendu.h
 *   \brief Element dossier contenant les élément de configuration du rendu 3D
 */

/**
 *   \brief Element dossier contenant les élément de configuration du rendu 3D
 */
class E_Scene_Rendu: public Element
{
public:
	E_Scene_Rendu( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_PROJET_RENDU,noeudCourant)
	{
		
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_RENDERING_FOLDER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_RENDERING_FOLDER_CLOSE);
		wxGetTranslation("Display");
		if(noeudCourant!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va créer les fils de notre noeudCourant
			wxString propValue;
			while(currentChild!=NULL)
			{
				if(currentChild->GetAttribute("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE)
						this->AppendFils(new E_Scene_Rendu_Origine(currentChild,this));
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Rendu( Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_PROJET_RENDU)
	{
		wxGetTranslation("Display");
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_RENDERING_FOLDER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_RENDERING_FOLDER_CLOSE);
		this->AppendFils(new E_Scene_Rendu_Origine(this));	
	}

	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("rendu"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}


};
