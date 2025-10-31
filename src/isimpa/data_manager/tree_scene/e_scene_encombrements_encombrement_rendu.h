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
		modes.push_back("Borders");
		this->AppendPropertyList("rendermode","Render mode",modes,0);
		wxGetTranslation("Volume");
		wxGetTranslation("Borders");
		wxGetTranslation("Render mode");
	}
	void InitProperties()
	{
		this->AppendPropertyColor("couleur","Color",GetRandValue()*255,GetRandValue()*255,GetRandValue()*255);
		wxGetTranslation("Color"); //Pour generer le fichier de langues automatiquement
		this->AppendPropertyBool("showlabel","Show name",false);
		wxGetTranslation("Show name");
		if(this->pere && this->pere->GetElementInfos().typeElement!=Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT)
		{
			this->AppendPropertyDecimal("obj_transp","Opacity",.5f,false,2,true,true,1,0);
			wxGetTranslation("Opacity");
			InitLineRenderingProp();
		}
	}
public:
	E_Scene_Encombrements_Encombrement_Rendu( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_3D_DISPLAY);
		wxGetTranslation("Display");
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
