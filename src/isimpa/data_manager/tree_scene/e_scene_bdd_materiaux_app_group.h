/* ----------------------------------------------------------------------
* I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
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
* For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or
* send an email to contact@noise-planet.org
*
* ----------------------------------------------------------------------*/

#include "first_header_include.hpp"

#include "data_manager/tree_scene/e_scene_bdd_materiaux_app_materiau.h"

/** \file e_scene_bdd_materiaux_app_group.h
   \brief Element représentant un dossier de matériaux, ce dossier peut contenir un dossier de meme type ou un matériau
*/

/**
   \brief Element représentant un dossier de matériaux, ce dossier peut contenir un dossier de meme type ou un matériau
*/
class E_Scene_Bdd_Materiaux_App_Group: public Element
{
public:
	E_Scene_Bdd_Materiaux_App_Group( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Group",Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_GROUP,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_APPLICATION_MATERIALS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_APPLICATION_MATERIALS_CLOSE);
		wxGetTranslation("Group");
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
					if(typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_GROUP || typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_GROUP)
						this->AppendFils(new E_Scene_Bdd_Materiaux_App_Group(currentChild,this));
					if(typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_MATERIAU || typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_MATERIAU)
						this->AppendFils(new E_Scene_Bdd_Materiaux_App_Materiau(currentChild,this));
				}
				currentChild = currentChild->GetNext();
			}
		}
	}
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("appgroup"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}

	

};
