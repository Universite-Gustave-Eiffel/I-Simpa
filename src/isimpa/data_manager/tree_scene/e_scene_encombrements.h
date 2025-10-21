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
#include "e_scene_encombrements_encombrement_model.h"
#include "e_scene_encombrements_encombrement_cuboide.h"
/** \file e_scene_encombrements.h
   \brief Element représentant un dossier contenant un ou plusieurs encombrements
*/

/**
   \brief Element représentant un dossier contenant un ou plusieurs encombrements
*/
class E_Scene_Encombrements: public Element
{
public:
	E_Scene_Encombrements( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Fitting zone",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS,noeudCourant)
	{
		
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_FITTINGS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_FITTINGS_CLOSE);
		wxGetTranslation("Fitting zone");
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
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT)
					{
						this->AppendFils(new E_Scene_Encombrements_Encombrement_Model(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE)
					{
						this->AppendFils(new E_Scene_Encombrements_Encombrement_Cuboide(currentChild,this));
					}

				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Encombrements( Element* parent)
		:Element(parent,"Fitting zone",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_FITTINGS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_FITTINGS_CLOSE);

	}
	
	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle="")
	{
		if(etypefils==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT)
		{
			if(libelle=="")
				return this->AppendFils(new E_Scene_Encombrements_Encombrement_Model(this));
			else
				return this->AppendFils(new E_Scene_Encombrements_Encombrement_Model(this,libelle));
		}else if(etypefils==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE)
		{
			if(libelle=="")
				return this->AppendFils(new E_Scene_Encombrements_Encombrement_Cuboide(this));
			else
				return this->AppendFils(new E_Scene_Encombrements_Encombrement_Cuboide(this,libelle));
		}
		return Element::AppendFilsByType(etypefils,libelle);
	}
	

	void OnRightClic(wxMenu* leMenu)
	{
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_ENCOMBREMENT, wxGetTranslation("Define scene fitted zone "),"./Bitmaps/popup_new.png"));
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_ENCOMBREMENT_CUBOIDE, wxGetTranslation("Define rectangular fitted zone"),"./Bitmaps/popup_new.png"));
		Element::OnRightClic(leMenu);
	}

	// Sauvegarde des informations à destination des moteurs de calculs

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"encombrement_enum");
		Element::SaveXMLCoreDoc(thisNode);
		return thisNode;
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("encombrements"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
