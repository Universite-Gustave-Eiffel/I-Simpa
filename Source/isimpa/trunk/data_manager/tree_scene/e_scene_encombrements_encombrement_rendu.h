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

/** \file e_scene_encombrements_encombrement_rendu.h
   \brief Propriétés de rendu d'un encombrement
*/
#ifndef __ELEMENT_FITTING_FITTING_RENDERING_H__
#define __ELEMENT_FITTING_FITTING_RENDERING_H__
/**
   \brief Propriétés de rendu d'un encombrement
	
   Propriétés de rendu en rapport avec l'encombrement
*/
class E_Scene_Encombrements_Encombrement_Rendu: public Element
{
private:
	void InitLineRenderingProp()
	{
		std::vector<wxString> modes;
		modes.push_back("Volume");
		modes.push_back("Bordures");
		this->AppendPropertyList("rendermode","Mode de rendu",modes,0);
		_("Volume");
		_("Borders");
		_("Render mode");
	}
	void InitProperties()
	{
		this->AppendPropertyColor("couleur","Couleur",GetRandValue()*255,GetRandValue()*255,GetRandValue()*255);
		_("Color"); //Pour generer le fichier de langues automatiquement
		this->AppendPropertyBool("showlabel","Afficher le libellé",false);
		_("Show name");
		if(this->pere && this->pere->GetElementInfos().typeElement!=Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT)
		{
			this->AppendPropertyDecimal("obj_transp","Opacité",.5f,false,2,true,true,1,0);
			_("Opacity");
			InitLineRenderingProp();
		}
	}
public:
	E_Scene_Encombrements_Encombrement_Rendu( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_3D_DISPLAY);
		_("Display");
		if(parent && parent->GetElementInfos().typeElement!=Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT && !this->IsPropertyExist("rendermode"))
			InitLineRenderingProp();
	}

	E_Scene_Encombrements_Encombrement_Rendu( Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_3D_DISPLAY);
		InitProperties();
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		return NoeudParent;
	}
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("rendu"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
};
#endif
