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

#include "e_scene_groupesurfaces_groupe.h"
#include "data_manager/drawable_element.h"
#include "e_scene_recepteurss_recepteur_rendu.h"
#include "e_scene_recepteurss_recepteur_proprietes.h"

/** \file e_scene_recepteurss_recepteur.h
   \brief Element correspondant à un récépteur de surface

   Cet élément hérite de E_Drawable afin d'être représenté dans la vue 3D.
*/

/**
   \brief Element correspondant à un récépteur de surface

   Cet élément hérite de E_Drawable afin d'être représenté dans la vue 3D.
*/
class E_Scene_Recepteurss_Recepteur: public E_Drawable
{
public:
	E_Scene_Recepteurss_Recepteur( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Drawable(parent,wxTRANSLATE("Scene receiver"),Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACE_RECEIVER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACE_RECEIVER_CLOSE);
		this->AllowMultipleSelection();
		this->elementInfo.userDestroyable=true;
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurS++;
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
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_PROPRIETES)
					{
						this->AppendFils(new E_Scene_Recepteurss_Recepteur_Proprietes(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_RENDU)
					{
						this->AppendFils(new E_Scene_Recepteurss_Recepteur_Rendu(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE)
					{
						// Update element name issue #148
						if(currentChild->GetAttribute("label","") != "Surface") {
							currentChild->DeleteAttribute("label");
							currentChild->AddAttribute("label", wxTRANSLATE("Surfaces"));
						}
						this->AppendFils(new E_Scene_Groupesurfaces_Groupe(currentChild,this));
					}
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	/**
	 * Constructeur de création, appelé lors de l'ajout d'un nouveau récepteur.
	 * @param parent Element parent
	 * @param nom Libellé du recepteur
	 */
	E_Scene_Recepteurss_Recepteur( Element* parent,wxString nom="Receiver")
		:E_Drawable(parent,nom,Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACE_RECEIVER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACE_RECEIVER_CLOSE);
		this->AllowMultipleSelection();
		this->elementInfo.userDestroyable=true;
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurS++;
		this->AppendFils(new E_Scene_Recepteurss_Recepteur_Proprietes(this));
		this->AppendFils(new E_Scene_Groupesurfaces_Groupe(this,wxTRANSLATE("Surfaces"),true));
	}
	~E_Scene_Recepteurss_Recepteur()
	{
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurS--;
	}

	void DrawItem( vec4 unitizeValue )
	{

	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"recepteur_surfacique");
		thisNode->AddAttribute("id",Convertor::ToString(elementInfo.xmlIdElement));
		
		thisNode->AddAttribute("name",elementInfo.libelleElement);
		return thisNode;
	}

	void OnBeginLabelEdit(wxTreeEvent& treeEvent)
	{
		//Overload this function to allow editing
	}
	 
	void OnEndLabelEdit(wxTreeEvent& treeEvent)
	{
		if(treeEvent.GetLabel()!="")
		{
			this->elementInfo.libelleElement=treeEvent.GetLabel();
			this->Modified(this); //Indique le l'element a été modifié
		}
	}


	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("recepteurs"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
