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

#ifndef _CONF_MAILLEUR_
#define _CONF_MAILLEUR_

/*! \file e_core_core_tetconf.h
    \brief Element permettant de paramétrer le moteur de maillage de la scène
*/

/**
 \brief Element permettant de paramétrer le moteur de maillage de la scène
*/
class E_Core_Core_Conf_Mailleur: public Element
{
public:
	E_Core_Core_Conf_Mailleur( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Maillage",Element::ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_TETMESH_PARAMETERS);
		//Add debug mode parameter v<1.21
		if(!this->IsPropertyExist("debugmode"))
		{
			this->AppendPropertyBool("debugmode","Test mesh topology",false);
		}
	}

	E_Core_Core_Conf_Mailleur( Element* parent)
		:Element(parent,"Maillage",Element::ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_TETMESH_PARAMETERS);
		InitProperties();
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		return NoeudParent;
	}
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("mesh_conf"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}

protected:

	void Modified(Element* eModif)
	{
		t_elementInfo eInfo=eModif->GetElementInfos();
		if(eInfo.libelleElement=="ismaxvol")
		{
			this->SetReadOnlyConfig("maxvol",!this->GetBoolConfig("ismaxvol"));
		}else if(eInfo.libelleElement=="isareaconstraint")
		{
			this->SetReadOnlyConfig("constraintrecepteurss",!this->GetBoolConfig("isareaconstraint"));
		}
		Element::Modified(eModif);
	}
	/**
	 Initialisation des propriétés communes à tout les mailleurs ( fait avant l'initialisation des mailleurs spécialisés )
	*/
	void InitProperties()
	{
		this->AppendPropertyDecimal("maxvol","Contrainte de volume (m3)",100);
		this->AppendPropertyDecimal("minratio","Rapport Rayon/arête",2,false,2,false,true,0,1);
		this->AppendPropertyText("appendparams","Paramétres supplémentaires","");
		this->AppendPropertyText("userdefineparams","Paramétres utilisateur","");
		this->AppendPropertyDecimal("constraintrecepteurss","Contrainte d'aire des récepteurs surfaciques (m²)",5.f,true,3,false,true,0,0.001f);
		this->AppendPropertyBool("isareaconstraint","Contrainte d'aire des récepteurs surfaciques",false);
		this->AppendPropertyBool("ismaxvol","Contrainte de volume",false);
		this->AppendPropertyBool("preprocess","Correction du modèle avant maillage",true);
		this->AppendPropertyBool("debugmode","Test mesh topology",false);
		_("Contrainte de volume (m3)");
		_("Rapport Rayon/arête");
		_("Paramétres supplémentaires");
		_("Paramétres utilisateur");
		_("Contrainte d'aire des récepteurs surfaciques (m²)");
		_("Contrainte d'aire des récepteurs surfaciques");
		_("Contrainte de volume");
		_("Correction du modèle avant maillage");
		_("Test mesh topology");
		_("Maillage");
	}
	
};

#endif