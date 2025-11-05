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

#include "data_manager/element.h"

#include "e_scene_groupesurfaces.h"
#include "e_scene_recepteurss.h"
#include "e_scene_recepteursp.h"
#include "e_scene_sources.h"
#include "e_scene_encombrements.h"
#include "e_scene_volumes.h"

/** \file e_scene_donnees.h
   \brief Element dossier contenant les données du projet
*/

/**
   \brief Element dossier contenant les données du projet
   
	@see E_Scene_Groupesurfaces
	@see E_Scene_Recepteursp
	@see E_Scene_Recepteurss
	@see E_Scene_Sources
	@see E_Scene_Encombrements
*/
class E_Scene_Donnees: public Element
{
public:
	E_Scene_Donnees( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,wxTRANSLATE("Data"),Element::ELEMENT_TYPE_SCENE_DONNEES,noeudCourant)

	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_DATA_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_DATA_CLOSE);
		bool volume_has_been_loaded=false;
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
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES)
					{
						this->AppendFils(new E_Scene_Groupesurfaces(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP)
					{
						this->AppendFils(new E_Scene_Recepteursp(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS)
					{
						this->AppendFils(new E_Scene_Recepteurss(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_SOURCES)
					{
						this->AppendFils(new E_Scene_Sources(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS)
					{
						this->AppendFils(new E_Scene_Encombrements(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_VOLUMES)
					{
						this->AppendFils(new E_Scene_Volumes(currentChild,this));
						volume_has_been_loaded=true;
					}
				}
				currentChild = currentChild->GetNext();
			}
		}
		if(!volume_has_been_loaded)
			this->AppendFils(new E_Scene_Volumes(this));

	}

	E_Scene_Donnees( Element* parent)
		:Element(parent,wxTRANSLATE("Data"),Element::ELEMENT_TYPE_SCENE_DONNEES)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_DATA_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_DATA_CLOSE);
		this->AppendFils(new E_Scene_Groupesurfaces(this));
		this->AppendFils(new E_Scene_Recepteursp(this));
		this->AppendFils(new E_Scene_Recepteurss(this));
		this->AppendFils(new E_Scene_Sources(this));	
		this->AppendFils(new E_Scene_Encombrements(this));
		this->AppendFils(new E_Scene_Volumes(this));
		this->elementInfo.expanded=true;
	}

	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("donnees"); // Nom de la balise xml ( pas d'espace ni accent )

		return thisNode;
	}


};
