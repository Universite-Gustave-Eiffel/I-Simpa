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

/** \file e_scene_projet_environnement.h
   \brief Element dossier contenant la configuration de l'environnement du projet
*/

/**
   \brief Element dossier contenant la configuration de l'environnement du projet
   
	Les données concernant la température, l'humidité relative et la pression atmosphérique sont renseignées et stocké ici.
*/
class E_Scene_Projet_Environnement: public Element
{
private:
	enum EFFET_METEO
	{
		EFFET_METEO_TRES_FAVORABLE,
		EFFET_METEO_FAVORABLE,
		EFFET_METEO_HOMOGENE,
		EFFET_METEO_DEFAVORABLE,
		EFFET_METEO_TRES_DEFAVORABLE
	};
	enum TYPE_SOL
	{
		TYPE_SOL_EAU_LIBRE,
		TYPE_SOL_NU,
		TYPE_SOL_GAZON_RAS,
		TYPE_SOL_GAZON_DENSE,
		TYPE_SOL_BLE,
		TYPE_SOL_HABITAT_EPARS,
		TYPE_SOL_PERIURBAIN_DENSE,
		TYPE_SOL_PERIURBAIN_PEU_DENSE,
		TYPE_SOL_URBAIN_DENSE
	};
	void GetParamsMeteoEffects(EFFET_METEO meteoStatus, float* aLog,float* bLin)
	{
		switch(meteoStatus)
		{
			case EFFET_METEO_TRES_FAVORABLE:
				*aLog=1.f;
				*bLin=.12f;
				break;
			case EFFET_METEO_FAVORABLE:
				*aLog=.4f;
				*bLin=.04f;
				break;
			case EFFET_METEO_HOMOGENE:
				*aLog=0.f;
				*bLin=0.f;
				break;
			case EFFET_METEO_DEFAVORABLE:
				*aLog=-.4f;
				*bLin=-.04f;
				break;
			case EFFET_METEO_TRES_DEFAVORABLE:
				*aLog=-1.f;
				*bLin=-0.12f;
				break;
		}
	}
	float GetZ0(TYPE_SOL groundType)
	{
		switch(groundType)
		{
			case TYPE_SOL_EAU_LIBRE:
				return .006f;
				break;
			case TYPE_SOL_NU:
				return .02f;
				break;
			case TYPE_SOL_GAZON_RAS:
				return .001f;
				break;
			case TYPE_SOL_GAZON_DENSE:
				return .02f;
				break;
			case TYPE_SOL_BLE:
				return .16f;
				break;
			case TYPE_SOL_HABITAT_EPARS:
				return .6f;
				break;
			case TYPE_SOL_PERIURBAIN_PEU_DENSE:
				return 1.2f;
				break;
			case TYPE_SOL_PERIURBAIN_DENSE:
				return 1.8f;
				break;
			case TYPE_SOL_URBAIN_DENSE:
				return 10.f;
				break;
		}
		return 0.f;
	}
	void InitProperties() 
	{
		this->AppendPropertyDecimal("z0","Rugosité - z0 (m)",0.02f,false,4,false,true,0,0.0001f,true);
		_("Roughness - z0 (m)");
		this->AppendPropertyDecimal("alog","Effets météo - Gradient de célérité alog",0,false,4,false,false,0,0,true);
		_("Meteorological effect - Celerity gradient a_log");
		this->AppendPropertyDecimal("blin","Effets météo - Gradient de célérité blin",0,false,4,false,false,0,0,true);
		_("Meteorological effect - Celerity gradient b_lin");
		this->AppendPropertyDecimal("temperature","Temperature (°C)",20,false,2,false,false,0,0,true);
		_("Temperature (°C)");
		this->AppendPropertyDecimal("humidite","Humidité relative (%)",50,false,0,true,true,100,0,true);
		_("Relative humidity (%)");
		this->AppendPropertyDecimal("pression","Pression atmosphérique (Pa)",101325,false,1,false,false,0,0,true);
		_("Atmospheric pressure (Pa)");
		std::vector<wxString> effetsMeteo;
		std::vector<int> effetsMeteoIndex;
		effetsMeteo.push_back("Très favorable");
		effetsMeteoIndex.push_back(EFFET_METEO_TRES_FAVORABLE);
		effetsMeteo.push_back("Favorable");
		effetsMeteoIndex.push_back(EFFET_METEO_FAVORABLE);
		effetsMeteo.push_back("Homogène");
		effetsMeteoIndex.push_back(EFFET_METEO_HOMOGENE);
		effetsMeteo.push_back("Défavorable");
		effetsMeteoIndex.push_back(EFFET_METEO_DEFAVORABLE);
		effetsMeteo.push_back("Très défavorable");
		effetsMeteoIndex.push_back(EFFET_METEO_TRES_DEFAVORABLE);
		_("Meteorological effect");
		_("Very favorable");
		_("Favorable");
		_("Homogeneous");
		_("Unfavorable");
		_("Very unfavorable");
		this->AppendPropertyList("lst_meteoeffect","Effets météo - Profiles",effetsMeteo,EFFET_METEO_HOMOGENE,false,1,effetsMeteoIndex);
		_("Meteorology effects - Profiles");
		std::vector<wxString> solTypes;
		std::vector<int> solTypesIndex;
		solTypes.push_back("Eau libre");
		solTypesIndex.push_back(TYPE_SOL_EAU_LIBRE);
		solTypes.push_back("Sol nu");
		solTypesIndex.push_back(TYPE_SOL_NU);
		solTypes.push_back("Gazon ras");
		solTypesIndex.push_back(TYPE_SOL_GAZON_RAS);
		solTypes.push_back("Gazon dense");
		solTypesIndex.push_back(TYPE_SOL_GAZON_DENSE);
		solTypes.push_back("Blé (1m de haut)");
		solTypesIndex.push_back(TYPE_SOL_BLE);
		solTypes.push_back("Habitat épars (fermes, villages, arbres, haies)");
		solTypesIndex.push_back(TYPE_SOL_HABITAT_EPARS);
		solTypes.push_back("Périurbain peu dense (zone résidentielles, jardins)");
		solTypesIndex.push_back(TYPE_SOL_PERIURBAIN_PEU_DENSE);
		solTypes.push_back("Périurbain dense");
		solTypesIndex.push_back(TYPE_SOL_PERIURBAIN_DENSE);
		solTypes.push_back("Urbain dense");
		solTypesIndex.push_back(TYPE_SOL_URBAIN_DENSE);
		_("Water");
		_("Ground");
		_("Short Lawn");
		_("Dense Lawn");
		_("Wheat (1m height)");
		_("Sparse habitat (farm, trees, hedges)");
		_("Low concentration Suburb (residential areas, gardens)");
		_("Dense Suburb");
		_("Dense urban");
		this->AppendPropertyList("lst_soltype","Rugosité - Types de sol",solTypes,TYPE_SOL_NU,false,1,solTypesIndex,true);
		_("Rugosity - Ground type");
		InitCoeffConstraint();
	}
	void InitCoeffConstraint()
	{
		this->AppendPropertyBool("disable_absatmo_computation","Absorption atmosphérique imposé",false,true);
		_("Set user defined atmospheric absorption");
		this->AppendPropertyDecimal("absatmo","Absorption atmosphérique",0,true,5,false,true,0,0,true);
		_("Atmospheric absorption");
	}
public:
	E_Scene_Projet_Environnement( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Environnement",Element::ELEMENT_TYPE_SCENE_PROJET_ENVIRONNEMENTCONF,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_ENVIRONMENT);
		_("Environment");
		if(!this->IsPropertyExist("absatmo"))
		{
			InitCoeffConstraint();
		}
	}

	E_Scene_Projet_Environnement( Element* parent)
		:Element(parent,"Environnement",Element::ELEMENT_TYPE_SCENE_PROJET_ENVIRONNEMENTCONF)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_ENVIRONMENT);
		InitProperties();
	}

	
	void Modified(Element* eModif)
	{
		t_elementInfo filsInfo=eModif->GetElementInfos();
		if(filsInfo.libelleElement=="lst_meteoeffect")
		{
			float aLog,bLin;
			GetParamsMeteoEffects((EFFET_METEO)this->GetListConfig("lst_meteoeffect"),&aLog,&bLin);
			this->UpdateDecimalConfig("alog",aLog);
			this->UpdateDecimalConfig("blin",bLin);
		} else if(filsInfo.libelleElement=="lst_soltype")
		{
			this->UpdateDecimalConfig("z0",GetZ0((TYPE_SOL)this->GetListConfig("lst_soltype")));
		} else if(filsInfo.libelleElement=="disable_absatmo_computation")
		{
			bool state=this->GetBoolConfig("disable_absatmo_computation");
			this->SetReadOnlyConfig("absatmo",!state);
		}
		Element::Modified(eModif);
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* NoeudCourant=new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"condition_atmospherique");
		return Element::SaveXMLCoreDoc(NoeudCourant);
	}
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("atmoconfig"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
