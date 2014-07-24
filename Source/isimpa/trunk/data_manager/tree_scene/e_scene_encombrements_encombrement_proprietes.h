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
#include "data_manager/appconfig.h"
#ifndef __ENCOMBREMENT_PROP__
#define __ENCOMBREMENT_PROP__


/** \file e_scene_encombrements_encombrement_proprietes.h
   \brief Propriétés d'un encombrement
*/

/**
   \brief Propriétés d'un encombrement

   Propriétés en rapport avec l'encombrement
*/

class E_Scene_Encombrements_Encombrement_Proprietes: public Element
{
private:

	void InitProperties()
	{
		this->AppendPropertyText(wxT("description"),wxT("Description"),wxT(""));
		_("Description");
		this->AppendPropertyBool(wxT("useforcalculation"),wxT("Encombrement actif"),true);
		_("Active fitting zone");
	}
public:
	E_Scene_Encombrements_Encombrement_Proprietes( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Propriétés",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		ignore_count_change=false;
		_("Properties");
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement++;
		if(this->GetBoolConfig("useforcalculation"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement_Actifs++;
	}

	E_Scene_Encombrements_Encombrement_Proprietes( Element* parent)
		:Element(parent,"Propriétés",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		ignore_count_change=true;
		InitProperties();
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement++;
		if(this->GetBoolConfig("useforcalculation"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement_Actifs++;
		ignore_count_change=false;
	}
	~E_Scene_Encombrements_Encombrement_Proprietes()
	{
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement--;
		if(this->GetBoolConfig("useforcalculation"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement_Actifs--;
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("prop"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}


	void Modified(Element* eModif)
	{
		if(!ignore_count_change && eModif->GetElementInfos().libelleElement=="useforcalculation")
		{
			if(this->GetBoolConfig("useforcalculation"))
				ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement_Actifs++;
			else
				ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement_Actifs--;
		}
		Element::Modified(eModif);
	}
protected:
	bool ignore_count_change;
};
#endif
