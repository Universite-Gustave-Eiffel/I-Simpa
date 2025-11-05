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

#include "data_manager/generic_element/e_materiau_app.h"
#include "data_manager/tree_scene/e_scene_bdd_materiaux_propmateriau.h"
#include "data_manager/tree_scene/e_scene_bdd_materiaux_rendermateriau.h"


#ifndef __APPMAT__
#define __APPMAT__

/** \file e_scene_bdd_materiaux_app_materiau.h
   \brief Element représentant un matériau, les surfaces vont tirer leurs propriétés de réflexion de ce type de matériau
*/

/**
   \brief Element représentant un matériau, les surfaces vont tirer leurs propriétés de réflexion de ce type de matériau
*/

class E_Scene_Bdd_Materiaux_App_Materiau: public Element
{
public:
	E_Scene_Bdd_Materiaux_App_Materiau( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Application",Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_MATERIAU,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_MATERIAL_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_MATERIAL_CLOSE);
		wxGetTranslation("Application");
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
					if(typeEle==Element::ELEMENT_TYPE_MATERIAU_USER || typeEle==Element::ELEMENT_TYPE_MATERIAU_APP)
						this->AppendFils(new E_Materiau_App(currentChild,this));
					else if(typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_PROPMATERIAU)
						this->AppendFils(new E_Scene_Bdd_Materiaux_PropertyMaterial(currentChild,this));
					else if(typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_MATERIAU_RENDER)
						this->AppendFils(new E_Scene_Bdd_Materiaux_RenderMaterial(currentChild,this));
				}
				currentChild = currentChild->GetNext();
			}
		}
	}
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("appmateriau"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}



	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		//Traitement spécial car seulement les matériaux utilisés sont exportés vers le code de calcul
		//On utilise le noeud XML créé par le spectre comme noeud pour les éléments fils (autre que le spectre)
		Element* spectrumMaterial=this->GetElementByType(ELEMENT_TYPE_MATERIAU_APP);
		if(spectrumMaterial)
		{
			std::size_t spectrumid(spectrumMaterial->GetXmlId());
			wxXmlNode* spectrumNode=spectrumMaterial->SaveXMLCoreDoc(NoeudParent);
			if(spectrumNode!=NoeudParent) //Si le matériau est utilisé par le projet
			{
				for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
				{
					if((*itfils)->GetXmlId()!=spectrumid)
						(*itfils)->SaveXMLCoreDoc(spectrumNode);
				}
			}
		}
		return NoeudParent;
	}
};

#endif
