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

#include "data_manager/drawable_element.h"

/**  \file e_scene_projet_demo.h
 *   \brief Description courte de la classe
 * 
 *   Informations détaillées de la classe
 */

/**
 *   \brief Description courte de la classe
 * 
 *   Informations détaillées de la classe
 */
class E_Scene_Projet_Demo: public E_Drawable 
{
public:
	E_Scene_Projet_Demo( wxXmlNode* noeudCourant ,  Element* parent) 
		:E_Drawable(parent,"Demo",Element::ELEMENT_TYPE_SCENE_PROJET_DEMO,Element::GRAPH_ITEM,noeudCourant)
	{
		wxGetTranslation("Demo");
	}

	E_Scene_Projet_Demo( Element* parent)
		:E_Drawable(parent,"Demo",Element::ELEMENT_TYPE_SCENE_PROJET_DEMO,Element::GRAPH_ITEM)
	{
		this->AppendPropertyText("demoname","Nom de la démo","Sans nom !");
		wxGetTranslation("Nom de la démo");
		this->AppendPropertyColor("democolor","Couleur de démonstration",0,120,0);
		wxGetTranslation("Couleur de démonstration");
	}

	void DrawItem( vec4 unitizeValue )
	{
		vec3 couleurTexteDemo=this->GetColorConfig("democolor")/255;	// Lecture de la couleur
		wxString texteDemo=this->GetStringConfig("demoname");			// Lecture du libellé
		vec3 PositionDemo(0,0,0);										// Position du texte à l'origine
		labelInfo.clear();
		AddText(texteDemo,PositionDemo,couleurTexteDemo);
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("demo"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};