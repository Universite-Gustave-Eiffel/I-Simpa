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
#include "data_manager/e_data_tree.h"
#include "data_manager/e_data_list.h"

/**  \file e_scene_sources_source_proprietes.h
 *   \brief Propriétés d'une source sonore ponctuelle de la scène
 */

/**
 *   \brief Propriétés d'une source sonore ponctuelle de la scène
 * 
 */
class E_Scene_Sources_Source_Properties: public Element
{
private:
	//v1.01
	void InitNewProperties()
	{
		this->AppendPropertyBool("enable",wxTRANSLATE("Active source"),true);
	}

	void InitDirectivityProperties() {
		this->AppendPropertyInteger("iddirectivity", "iddirectivity", 0, false)->Hide();
        dynamic_cast<E_Data_List*>(this->GetElementByLibelle("directivite"))->AppendItem(wxTRANSLATE("Directivity balloon"), 5);
	}
	void InitProperties()
	{
		this->AppendPropertyText("description","Description","");
		_("Description");
		std::vector<wxString> vDirectivite;
		std::vector<int> iDirectivite;
		vDirectivite.push_back(wxTRANSLATE("Omnidirectional"));
		vDirectivite.push_back(wxTRANSLATE("Unidirectional"));
		vDirectivite.push_back(wxTRANSLATE("XY plan"));
		vDirectivite.push_back(wxTRANSLATE("XY plan"));
		vDirectivite.push_back(wxTRANSLATE("XZ plan"));
		vDirectivite.push_back(wxTRANSLATE("Directivity balloon"));
		this->AppendPropertyList("directivite",wxTRANSLATE("Directivity"),vDirectivite,DIRECTIVITE_SOURCE_OMINIDIRECTIONNEL,false,1,iDirectivite,true);

		this->AppendPropertyDecimal("u", wxTRANSLATE("Direction X"),1,true,2,false,false,0,0,true);
		this->AppendPropertyDecimal("v", wxTRANSLATE("Direction Y"),1,true,2,false,false,0,0,true);
		this->AppendPropertyDecimal("w", wxTRANSLATE("Direction Z"),1,true,2,false,false,0,0,true);
		this->AppendPropertyDecimal("delay", wxTRANSLATE("Time delay (s)"),0.f,false,4,false,true,0,0,true);

		InitNewProperties();
		InitDirectivityProperties();
	}

	void InitProp() {
		if (this->pere != NULL)
		{
			if (this->pere->GetElementInfos().typeElement == ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
			{
				this->DeleteAllElementByType(ELEMENT_TYPE_TREE_LIST);
				Element* rootDirectivities = ApplicationConfiguration::GetRootScene()->GetElementByType(Element::ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES);
				if (rootDirectivities)
				{
					Element* defaultEle = ApplicationConfiguration::GetDirectivity(this->GetIntegerConfig("iddirectivity"));
					std::list<Element::ELEMENT_TYPE> filterTree;
					filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES);
					filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_APP);
					filterTree.push_back(ELEMENT_TYPE_DIRECTIVITIES_APP);
					filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_USER);
					filterTree.push_back(ELEMENT_TYPE_DIRECTIVITIES_USER);
					this->AppendFils(new E_Data_Tree(this, "directivity-balloon", "Directivity Balloon", rootDirectivities, filterTree, defaultEle, false, 1));
					if (this->GetListConfig("directivite") != DIRECTIVITE_SOURCE_DIRECTIONNEL)
					{
						this->SetReadOnlyConfig("directivity-balloon");
					}
				}
			}
		}
	}
public:
	enum DIRECTIVITE_SOURCE
	{
		DIRECTIVITE_SOURCE_OMINIDIRECTIONNEL,
		DIRECTIVITE_SOURCE_UNIDIRECTIONNEL,
		DIRECTIVITE_SOURCE_XY,
		DIRECTIVITE_SOURCE_YZ,
		DIRECTIVITE_SOURCE_XZ,
		DIRECTIVITE_SOURCE_DIRECTIONNEL
	};
	E_Scene_Sources_Source_Properties( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Properties",Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		ignore_count_change=true;
		if(!this->IsPropertyExist("enable")) {
            InitNewProperties();
        }
		if(!this->IsPropertyExist("iddirectivity")) {
			InitDirectivityProperties();
		}
		if(this->GetBoolConfig("enable"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives++;
		ignore_count_change=false;
		_("Properties");
	}

	E_Scene_Sources_Source_Properties( Element* parent)
		:Element(parent,"Properties",Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		ignore_count_change=true;
		InitProperties();
		if(this->GetBoolConfig("enable"))
			ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives++;
		ignore_count_change=false;
	}
	~E_Scene_Sources_Source_Properties()
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
		// si l'on a changé de ballon de directivité
		if (filsInfo.typeElement == ELEMENT_TYPE_TREE_LIST && filsInfo.libelleElement == "directivity-balloon")
		{
			E_Data_Tree* ElementTree = dynamic_cast<E_Data_Tree*>(eModif);
			if (ElementTree)
			{
				this->UpdateEntierConfig("iddirectivity", ApplicationConfiguration::GetDirectivityId(ElementTree->GetChoice()));
			}
		}
		// si l'on a changé de type de directivité
		if(filsInfo.libelleElement=="directivite")
		{
			bool newStateCoord=false;
			if(this->GetListConfig("directivite") == DIRECTIVITE_SOURCE_UNIDIRECTIONNEL 
				|| this->GetListConfig("directivite") == DIRECTIVITE_SOURCE_DIRECTIONNEL)
			{
				newStateCoord=false;
			}else{
				newStateCoord=true;
			}
			this->SetReadOnlyConfig("u",newStateCoord);
			this->SetReadOnlyConfig("v",newStateCoord);
			this->SetReadOnlyConfig("w",newStateCoord);
			
			bool newStateBalloon = (this->GetListConfig("directivite") == DIRECTIVITE_SOURCE_DIRECTIONNEL) ? false : true;
			this->SetReadOnlyConfig("directivity-balloon", newStateBalloon);
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
