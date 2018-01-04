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
#include "e_scene_bdd_directivities_app.h"
#include "e_scene_bdd_directivities_user.h"

/** \file e_scene_bdd_directivities.h
\brief This element is the directivity folder, it holds application and user defined directivities profiles.
*/

/**
\brief This element is the directivity folder, it holds application and user defined directivities profiles.
*/
class E_Scene_Bdd_Directivities : public Element
{
public:
	E_Scene_Bdd_Directivities(wxXmlNode* noeudCourant, Element* parent)
		:Element(parent, wxTRANSLATE("Directivities"), Element::ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES, noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED, GRAPH_USER_DIRECTIVITY_OPEN);
		SetIcon(GRAPH_STATE_NORMAL, GRAPH_USER_DIRECTIVITY_CLOSE);

		if (noeudCourant != NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			wxString propValue;
			while (currentChild != NULL)
			{
				if (currentChild->GetAttribute("eid", &propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);

					if (typeEle == Element::ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_APP)
						this->AppendFils(new E_Scene_Bdd_Directivities_Application(currentChild, this));
					else if (typeEle == Element::ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_USER)
						this->AppendFils(new E_Scene_Bdd_Directivities_User(currentChild, this));
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Bdd_Directivities(Element* parent)
		:Element(parent, wxTRANSLATE("Directivities"), Element::ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES)
	{
		SetIcon(GRAPH_STATE_EXPANDED, GRAPH_ITEM);
		SetIcon(GRAPH_STATE_NORMAL, GRAPH_ITEM);
		this->AppendFils(new E_Scene_Bdd_Directivities_Application(this));
		this->AppendFils(new E_Scene_Bdd_Directivities_User(this));
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		return NoeudParent;
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("directivities"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
