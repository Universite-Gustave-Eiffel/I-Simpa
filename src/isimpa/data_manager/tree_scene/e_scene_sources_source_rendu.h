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

/**  \file e_scene_sources_source_rendu.h
 *   \brief Propriétés de rendu 3D d'une source sonore ponctuelle de la scène
 */

/**
 *   \brief Propriétés de rendu 3D d'une source sonore ponctuelle de la scène
 * 
 */
class E_Scene_Sources_Source_Rendu: public Element
{
private:

	void InitProperties()
	{
		this->AppendPropertyColor("couleur","Color",255,0,0);
		wxGetTranslation("Color"); //Pour generer le fichier de langues automatiquement
		this->AppendPropertyBool("showlabel","Show name",true);
		wxGetTranslation("Show name");
	}
public:
	E_Scene_Sources_Source_Rendu( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_RENDU,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_3D_DISPLAY);
		wxGetTranslation("Display");
		InitProperties();
	}

	E_Scene_Sources_Source_Rendu( Element* parent)
		:Element(parent,"Display",Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_RENDU)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_3D_DISPLAY);
		InitProperties();
	}

	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("rendu"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
};
