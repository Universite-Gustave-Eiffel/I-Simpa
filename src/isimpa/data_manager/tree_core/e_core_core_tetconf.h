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
		:Element(parent,wxTRANSLATE("Meshing"),Element::ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_TETMESH_PARAMETERS);
		//Add debug mode parameter v<1.21
		if(!this->IsPropertyExist("debugmode"))
		{
			this->AppendPropertyBool("debugmode","Test mesh topology",false);
		}
	}

	E_Core_Core_Conf_Mailleur( Element* parent)
		:Element(parent,wxTRANSLATE("Meshing"),Element::ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
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
            if (this->GetBoolConfig("isareaconstraint")) {
                this->UpdateStringConfig("appendparams", "");
            }
            else {
                this->UpdateStringConfig("appendparams", "-Y");
            }
        }
		Element::Modified(eModif);
	}
	/**
	 Initialisation des propriétés communes à tout les mailleurs ( fait avant l'initialisation des mailleurs spécialisés )
	*/
	void InitProperties()
	{
        this->AppendPropertyDecimal("maxvol", wxTRANSLATE("Volume constraint (m3)"),100);
		this->AppendPropertyDecimal("minratio", wxTRANSLATE("Radius/Edge ratio"),5,false,2,false,true,0,1);
        // -Y parameter does not create additional point on the limits of the domain
        // reduce the number of tetrahedrons is good for computation speed
		this->AppendPropertyText("appendparams", wxTRANSLATE("Additional parameters"),"-Y");
		this->AppendPropertyText("userdefineparams", wxTRANSLATE("User-defined parameters"),"");
		this->AppendPropertyDecimal("constraintrecepteurss",wxTRANSLATE("Surface receivers constraint (m²)"),5.f,true,3,false,true,0,0.001f);
		this->AppendPropertyBool("isareaconstraint", wxTRANSLATE("Surface receivers constraint"),false);
		this->AppendPropertyBool("ismaxvol", wxTRANSLATE("Volume constraint"),false);
		this->AppendPropertyBool("preprocess", wxTRANSLATE("Scene correction before meshing"),true);
		this->AppendPropertyBool("debugmode", wxTRANSLATE("Test mesh topology"),false);

	}
	
};

#endif
