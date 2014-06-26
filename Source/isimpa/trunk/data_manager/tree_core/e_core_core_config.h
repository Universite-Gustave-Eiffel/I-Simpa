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


#ifndef _CORE_CONF_
#define _CORE_CONF_




/*! \file e_core_core_config.h
    \brief Element de configuration générique des moteurs de calculs
*/
/**
	@brief Element de configuration générique des moteurs de calculs
*/
class E_Core_Core_Configuration: public Element
{
public:
	E_Core_Core_Configuration( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Propriétés",Element::ELEMENT_TYPE_CORE_CORE_CONFIG,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		_("Propriétés");
	}

	E_Core_Core_Configuration( Element* parent)
		:Element(parent,"Propriétés",Element::ELEMENT_TYPE_CORE_CORE_CONFIG)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("configuration"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
	/**
		Initialisation des propriétés communes à tout les coeurs de calcul (celles lues par lib_interface
	*/
	void InitProperties()
	{
		
		AppendPropertyDecimal("duree_simulation","Durée de la simulation (s)",2,false,8,false,true,0,0.0001f,true);
		AppendPropertyDecimal("pasdetemps","Pas de temps (s)",0.01f,false,8,false,false,0,0,true);
		_("Durée de la simulation (s)");
		_("Pas de temps (s)");
	}
	
};

#endif