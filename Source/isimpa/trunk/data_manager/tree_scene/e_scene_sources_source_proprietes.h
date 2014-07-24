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

/**  \file e_scene_sources_source_proprietes.h
 *   \brief Propriétés d'une source sonore ponctuelle de la scène
 */

/**
 *   \brief Propriétés d'une source sonore ponctuelle de la scène
 * 
 */
class E_Scene_Sources_Source_Proprietes: public Element
{
private:
	//v1.01
	void InitNewProperties()
	{
		this->AppendPropertyBool("enable","Source active",true);
		_("Active source");
	}
	void InitProperties()
	{
		this->AppendPropertyText("description","Description","");
		_("Description");
		std::vector<wxString> vDirectivite;
		std::vector<int> iDirectivite;
		vDirectivite.push_back("Omnidirectionnel");
		vDirectivite.push_back("Unidirectionnel");
		vDirectivite.push_back("Plan XY");
		vDirectivite.push_back("Plan YZ");
		vDirectivite.push_back("Plan XZ");
		_("Omnidirectional");
		_("Unidirectional");
		_("XY plan");
		_("YZ plan");
		_("XZ plan");
		this->AppendPropertyList("directivite","Directivité",vDirectivite,DIRECTIVITE_SOURCE_OMINIDIRECTIONNEL,false,1,iDirectivite,true);
		_("Directivity");
		this->AppendPropertyDecimal("u","Direction x",1,true,2,false,false,0,0,true);
		this->AppendPropertyDecimal("v","Direction y",1,true,2,false,false,0,0,true);
		this->AppendPropertyDecimal("w","Direction z",1,true,2,false,false,0,0,true);
		_("Direction X");
		_("Direction Y");
		_("Direction Z");
		this->AppendPropertyDecimal("delay","Délai de déclenchement",0.f,false,4,false,true,0,0,true);
		_("Time delay (s)");
		InitNewProperties();
	}
public:
	enum DIRECTIVITE_SOURCE
	{
		DIRECTIVITE_SOURCE_OMINIDIRECTIONNEL,
		DIRECTIVITE_SOURCE_UNIDIRECTIONNEL,
		DIRECTIVITE_SOURCE_XY,
		DIRECTIVITE_SOURCE_YZ,
		DIRECTIVITE_SOURCE_XZ
	};
	E_Scene_Sources_Source_Proprietes( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Propriétés",Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		ignore_count_change=true;
		if(!this->IsPropertyExist("enable"))
			InitNewProperties();
		if(this->GetBoolConfig("enable"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives++;
		ignore_count_change=false;
		_("Properties");
	}

	E_Scene_Sources_Source_Proprietes( Element* parent)
		:Element(parent,"Propriétés",Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		ignore_count_change=true;
		InitProperties();
		if(this->GetBoolConfig("enable"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives++;
		ignore_count_change=false;
	}
	~E_Scene_Sources_Source_Proprietes()
	{
		if(this->GetBoolConfig("enable"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives--;
	}
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("prop"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
	void Modified(Element* eModif)
	{
		t_elementInfo filsInfo=eModif->GetElementInfos();
		if(filsInfo.libelleElement=="directivite")
		{
			bool newStateCoord=false;
			if(this->GetListConfig("directivite")!=DIRECTIVITE_SOURCE_UNIDIRECTIONNEL)
			{
				newStateCoord=true;
			}else{
				newStateCoord=false;
			}
			this->SetReadOnlyConfig("u",newStateCoord);
			this->SetReadOnlyConfig("v",newStateCoord);
			this->SetReadOnlyConfig("w",newStateCoord);
		}else if(!ignore_count_change && filsInfo.libelleElement=="enable")
		{
			if(this->GetBoolConfig("enable"))
				ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives++;
			else
				ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives--;
		}
		Element::Modified(eModif);
	}
protected:
	bool ignore_count_change;
};
