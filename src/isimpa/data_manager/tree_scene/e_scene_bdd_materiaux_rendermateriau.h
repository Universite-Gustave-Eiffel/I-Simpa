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

#ifndef __ELEMENT_RENDER_MATERIAL__
#define __ELEMENT_RENDER_MATERIAL__

/** \file e_scene_bdd_materiaux_rendermateriau.h
   \brief Element représentant le rendu d'un matériau
*/

/**
   \brief Element représentant le rendu d'un matériau
*/
class E_Scene_Bdd_Materiaux_RenderMaterial: public Element
{
public:
	E_Scene_Bdd_Materiaux_RenderMaterial( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"3D display",Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_MATERIAU_RENDER,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_3D_DISPLAY);
		wxGetTranslation("3D display");
		this->SetDrawable();
	}
	E_Scene_Bdd_Materiaux_RenderMaterial( Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_MATERIAU_RENDER)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_3D_DISPLAY);
		this->AppendPropertyColor("mat_color","Color",GetRandValue()*255,GetRandValue()*255,GetRandValue()*255);
		wxGetTranslation("Color");
		this->SetDrawable();
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("matcolor"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}


};

#endif
