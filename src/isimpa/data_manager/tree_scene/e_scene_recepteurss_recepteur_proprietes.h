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

/** \file e_scene_recepteurss_recepteur_proprietes.h
   \brief Element correspondant à la configuration d'un récepteur de surface
*/

/**
   \brief Element correspondant à la configuration d'un récepteur de surface
   @see E_Scene_Recepteurss_Recepteur
*/
class E_Scene_Recepteurss_Recepteur_Proprietes: public Element
{
private:

    void AddActiveProp()
    {
        this->AppendPropertyBool("enabled", wxTRANSLATE("Enable"), true, false);
    }
	void InitProperties()
	{
		this->AppendPropertyText("description",wxTRANSLATE("Description"),"");
        AddActiveProp();
	}
public:
	E_Scene_Recepteurss_Recepteur_Proprietes( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Properties",Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_PROPRIETES,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		_("Properties");
        if(!IsPropertyExist("enabled")) {
            AddActiveProp();
        }
		// Change property label about #148
		Element::initPropLabel(this, "enabled", wxTRANSLATE("Enable"));
	}

	E_Scene_Recepteurss_Recepteur_Proprietes( Element* parent)
		:Element(parent,"Properties",Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_PROPRIETES)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		InitProperties();
	}

	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("prop"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
};
