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

#include "data_manager/tree_core/e_core_core.h"

#ifndef __E_CORE_TC__
#define __E_CORE_TC__

/*! \file e_core_tccore.h
    \brief Element correspondant au moteur de calcul "Théorie classique"
*/


/**
    \brief Element correspondant au moteur de calcul "Théorie classique"
*/
class E_Core_Tc: public E_Core_Core
{
private:
	void InitExportRs(Element* confCore)
	{
		confCore->AppendPropertyBool("output_recs_byfreq","Export surface receivers for each frequency band",true,true);
		_("Export surface receivers for each frequency band");
	}
public:

	void AddConfTc()
	{
		E_Core_Core_Configuration* confCore=new E_Core_Core_Configuration(this);
		this->AppendFils(confCore);
		confCore->AppendPropertyBool("abs_atmo_calc","Atmospheric absorption calculation",true,true);
		InitExportRs(confCore);
		_("Atmospheric absorption calculation");
	}
	E_Core_Tc( Element* parent, wxXmlNode* noeudCourant)
		:E_Core_Core(parent,"Classical theory of reverberation",ELEMENT_TYPE_CORE_TC,noeudCourant)
	{

		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_STANDARTCORE_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_STANDARTCORE_CLOSE);
		_("Classical theory of reverberation");
		Element* coreConf=this->GetElementByType(Element::ELEMENT_TYPE_CORE_CORE_CONFIG);
		if(!coreConf) //Maj version < 05/11/2008
		{
			AddConfTc();
		}else{
			InitExportRs(coreConf);
		}
        if (GetStringConfig("exeName").EndsWith(".exe")) {
            UpdateStringConfig("exeName", "TheorieClassique");
        }
	}

	E_Core_Tc( Element* parent)
		:E_Core_Core(parent,"Classical theory of reverberation",ELEMENT_TYPE_CORE_TC)
	{
		_("Classical theory of reverberation");
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_STANDARTCORE_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_STANDARTCORE_CLOSE);

		this->AppendFilsByType(ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE);
		AddConfTc();
		Element* newElement=new E_Core_Core_Bfreqselection(this);
		this->AppendFils(newElement)->Hide();
		this->AppendPropertyText("direct_recepteurSOutputName","",wxString("Direct field")+wxFileName::GetPathSeparator(),true,true)->Hide();
		this->AppendPropertyText("sabine_recepteurSOutputName","",wxString("Total field (Sabine)")+wxFileName::GetPathSeparator(),true,true)->Hide();
		this->AppendPropertyText("eyring_recepteurSOutputName","",wxString("Total field (Eyring)")+wxFileName::GetPathSeparator(),true,true)->Hide();
		_("Direct field");
		_("Total field (Sabine)");
		_("Total field (Eyring)");
		//_("Main calculation");



		this->AppendPropertyText("modelName","","mesh.cbin",true,true)->Hide();
		this->AppendPropertyText("exeName","","TheorieClassique")->Hide();
		this->AppendPropertyText("corePath","",wxString("theorie_classique")+wxFileName::GetPathSeparator())->Hide();
		this->AppendPropertyText("tetrameshFileName","","tetramesh.mbin",true,true)->Hide();
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Core_Core::SaveXMLDoc(NoeudParent);
		thisNode->SetName("tc");
		return thisNode;
	}
};

#endif
