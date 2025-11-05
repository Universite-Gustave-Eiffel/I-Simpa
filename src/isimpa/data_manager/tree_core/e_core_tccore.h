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

#include "data_manager/tree_core/e_core_core.h"

#ifndef __E_CORE_TC__
#define __E_CORE_TC__

/*! \file e_core_tccore.h
    \brief Classical theroy computation core declaration
*/


/**
    \brief Classical theroy computation core declaration
*/
class E_Core_Tc: public E_Core_Core
{
private:
	void InitExportRs(Element* confCore)
	{
		confCore->AppendPropertyBool("output_recs_byfreq","Export surface receivers for each frequency band",true,true);
		wxGetTranslation("Export surface receivers for each frequency band");
	}
public:

	void AddConfTc()
	{
		E_Core_Core_Configuration* confCore=new E_Core_Core_Configuration(this);
		this->AppendFils(confCore);
		confCore->AppendPropertyBool("abs_atmo_calc","Active calculation of atmospheric absorption",true,true);
		InitExportRs(confCore);
		wxGetTranslation("Atmospheric absorption calculation");
	}
	E_Core_Tc( Element* parent, wxXmlNode* noeudCourant)
		:E_Core_Core(parent, wxTRANSLATE("Classical theory of reverberation (TCR)"),ELEMENT_TYPE_CORE_TC,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_STANDARTCORE_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_STANDARTCORE_CLOSE);
		Element* coreConf=this->GetElementByType(Element::ELEMENT_TYPE_CORE_CORE_CONFIG);
		if(!coreConf) //Maj version < 05/11/2008
		{
			AddConfTc();
		}else{
			InitExportRs(coreConf);
		}
        if (GetStringConfig("exeName").EndsWith(".exe")) {
            UpdateStringConfig("exeName", "classicalTheory");
        }
		if (GetStringConfig("corePath").StartsWith("theorie_classique")) {
			UpdateStringConfig("corePath", wxString("classical_theory") + wxFileName::GetPathSeparator());
		}
	}

	E_Core_Tc( Element* parent)
		:E_Core_Core(parent, wxTRANSLATE("Classical theory of reverberation"),ELEMENT_TYPE_CORE_TC)
	{
		this->elementInfo.expanded = true;
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_STANDARTCORE_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_STANDARTCORE_CLOSE);

		this->AppendFilsByType(ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE);
		AddConfTc();
		Element* newElement=new E_Core_Core_Bfreqselection(this);
		this->AppendFils(newElement)->Hide();
		this->AppendPropertyText("direct_recepteurSOutputName","",wxTRANSLATE("Direct field")+ wxString(wxFileName::GetPathSeparator()),true,true)->Hide();
		this->AppendPropertyText("sabine_recepteurSOutputName","", wxTRANSLATE("Total field (Sabine)")+ wxString(wxFileName::GetPathSeparator()),true,true)->Hide();
		this->AppendPropertyText("eyring_recepteurSOutputName","", wxTRANSLATE("Total field (Eyring)")+ wxString(wxFileName::GetPathSeparator()),true,true)->Hide();

		this->AppendPropertyText("modelName","","mesh.cbin",true,true)->Hide();
		this->AppendPropertyText("exeName","","classicalTheory")->Hide();
		this->AppendPropertyText("corePath","",wxString("classical_theory")+wxFileName::GetPathSeparator())->Hide();
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
