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

#ifndef __ELEMENT_PROPERTY_MATERIAL__
#define __ELEMENT_PROPERTY_MATERIAL__

/** \file e_scene_bdd_materiaux_propmateriau.h
   \brief Element représentant les propriétés d'un matériau
*/

/**
   \brief Element représentant les propriétés d'un matériau
*/
class E_Scene_Bdd_Materiaux_PropertyMaterial: public Element
{
public:
	
	void AddSideProperty()
	{
		std::vector<wxString> choices;
		choices.push_back(_("Unilatéral"));
		choices.push_back(_("Bilatéral"));
		this->AppendPropertyList("side_material",_("Sens de prise en compte"),choices,1,false,1,std::vector<int>(),true);
	}

	E_Scene_Bdd_Materiaux_PropertyMaterial( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Description du matériau",Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_PROPMATERIAU,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		_("Description du matériau");
		if(!this->IsPropertyExist("side_material"))
		{
			AddSideProperty();
		}
	}
	E_Scene_Bdd_Materiaux_PropertyMaterial( Element* parent)
		:Element(parent,"Description du matériau",Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_PROPMATERIAU)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		this->AppendPropertyDecimal("masse_volumique","Masse volumique (Kg/m3)",0,false,4,false,false,0,0,true);
		this->AppendPropertyDecimal("resistivite","Résistivité (cgs)",100000,false,4,false,false,0,0,true);
		this->AppendPropertyText("description","Description","");
		this->AppendPropertyText("reference","Référence","");
		AddSideProperty();
		_("Référence");
		_("Description");
		_("Masse volumique (Kg/m3)");
		_("Résistivité (cgs)");
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("property"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}


};

#endif