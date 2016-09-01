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
/** \file e_scene_projet_informations.h
 *  \brief Element représentant les informations du projet
 */

/**
 *  \brief Element représentant les informations du projet
 */
class E_Scene_Projet_Information: public Element
{
public:
	E_Scene_Projet_Information( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Information",Element::ELEMENT_TYPE_SCENE_PROJET_INFORMATION,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_INFORMATION);
		_("Information");
		if(!this->IsPropertyExist("nbencombrementenable"))
		{
			AppendNewProperties();
			this->SetReadOnlyAllConfig();
		}
	}

	E_Scene_Projet_Information( Element* parent)
		:Element(parent,"Informations",Element::ELEMENT_TYPE_SCENE_PROJET_INFORMATION)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_INFORMATION);
		InitProperties();
	}
	/**
	 * Nouveaux champs à la version 1.02 du logiciel
	 */
	void AppendNewProperties()
	{

		this->AppendPropertyInteger("nbencombrementenable","Number of active fitting zones",0);
		this->AppendPropertyInteger("nbsourcesenable","Number of active sound sources",0);
		_("Number of active fitting zones");
		_("Number of active sound sources");
	}
	/*
	 * Ajoute les éléments de configuration tel que l'emplacement du fichier de modèle et le chemin de sauvegarde par défaut du projet
	 */
	void InitProperties() 
	{
		this->AppendPropertyInteger("nbfacemodel","Model face count",0);
		this->AppendPropertyInteger("nbencombrement","Number of fitting zones",0);
		this->AppendPropertyInteger("nbrecepteurp","Number of punctual receivers",0);
		this->AppendPropertyInteger("nbrecepteurs","Number of surfaces receivers",0);
		this->AppendPropertyInteger("nbsources","Number of sound sources",0);
		this->AppendPropertyInteger("nbsurfacegroup","Number of surface groups",0);
		this->AppendPropertyDecimal("volmodel","Scene volume (m3)",0.f,true,1);
		this->AppendPropertyDecimal("surfscene","Scene surface (m2)",0.f,true,1);
		AppendNewProperties();
		this->SetReadOnlyAllConfig();
		_("Model face count");
		_("Number of fitting zones");
		_("Number of punctual receivers");
		_("Number of surfaces receivers");
		_("Number of sound sources");
		_("Number of surface groups");
		_("Scene volume (m3)");
		_("Scene surface (m2)");
	}
		
	void InitProp()
	{
		this->UpdateEntierConfig("nbfacemodel",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Faces);
		this->UpdateEntierConfig("nbencombrement",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement);
		this->UpdateEntierConfig("nbencombrementenable",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement_Actifs);
		this->UpdateEntierConfig("nbrecepteurp",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurP);
		this->UpdateEntierConfig("nbrecepteurs",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurS);
		this->UpdateEntierConfig("nbsources",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources);
		this->UpdateEntierConfig("nbsourcesenable",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources_Actives);
		this->UpdateEntierConfig("nbsurfacegroup",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_SurfGroup);
		if(ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.volScene!=-1)
			this->UpdateDecimalConfig("volmodel",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.volScene);
		if(ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.surfScene!=-1)
			this->UpdateDecimalConfig("surfscene",ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.surfScene);
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("info"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
