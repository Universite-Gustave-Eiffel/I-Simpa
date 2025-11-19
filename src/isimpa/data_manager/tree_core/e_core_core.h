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
#include "data_manager/tree_core/e_core_core_tetconf.h"
#include "data_manager/tree_core/e_core_core_config.h"
#include "data_manager/tree_core/e_core_core_bfreqselection.h"
#ifndef __E_CORE_CORE__
#define __E_CORE_CORE__


/*! \file e_core_core.h
    \brief Element virtuel correspondant à un moteur de calcul générique
*/
/**
	\brief Element virtuel correspondant à un moteur de calcul générique
*/
class E_Core_Core: public Element
{

public:


	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle="")
	{
		if(etypefils==ELEMENT_TYPE_CORE_CORE_CONFIG)
		{
			E_Core_Core_Configuration* econf=new E_Core_Core_Configuration(this);
			this->AppendFils(econf);
			econf->InitProperties();
			return econf;
		}else if(etypefils==ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
			return this->AppendFils(new E_Core_Core_Conf_Mailleur(this));
		else if(etypefils==ELEMENT_TYPE_CORE_CORE_BFREQSELECTION)
			return this->AppendFils(new E_Core_Core_Bfreqselection(this));
		else
			return Element::AppendFilsByType(etypefils,libelle);
	}
	E_Core_Core( Element* parent,wxString nomEl,ELEMENT_TYPE typeEle , wxXmlNode* noeudCourant=NULL)
		:Element(parent,nomEl,typeEle,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_STANDARTCORE_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_STANDARTCORE_CLOSE);
		if(noeudCourant==NULL)
		{
			/**
			 * Raz des paramètres du coeur de calcul
			 * coreData
			 */
		}else{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va créer les fils de notre noeudCourant
			wxString propValue;
			while(currentChild!=NULL)
			{
				if(currentChild->GetAttribute("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					if(typeEle==Element::ELEMENT_TYPE_CORE_CORE_CONFIG)
						this->AppendFils(new E_Core_Core_Configuration(currentChild,this));
					else if(typeEle==Element::ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
						this->AppendFils(new E_Core_Core_Conf_Mailleur(currentChild,this));
					else if(typeEle==Element::ELEMENT_TYPE_CORE_CORE_BFREQSELECTION)
						this->AppendFils(new E_Core_Core_Bfreqselection(currentChild,this));
				}
				currentChild=currentChild->GetNext();
			}
		}
	}		
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* NoeudCourant=new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"simulation");

		NoeudCourant->AddAttribute("recepteurss_directory",ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FOLDER_PATH);
		NoeudCourant->AddAttribute("recepteurss_filename",ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FILENAME);
		NoeudCourant->AddAttribute("recepteurss_cut_filename",ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_CUT_FILENAME);

		NoeudCourant->AddAttribute("receiversp_directory",wxTRANSLATE("Punctual receivers"));
		NoeudCourant->AddAttribute("receiversp_filename",wxTRANSLATE("Sound level")+wxString(".recp"));
		NoeudCourant->AddAttribute("receiversp_filename_adv",wxTRANSLATE("Advanced sound level")+wxString(".gap"));
		NoeudCourant->AddAttribute("cumul_filename",wxTRANSLATE("Total energy")+wxString(".recp"));

		return Element::SaveXMLCoreDoc(NoeudCourant);
	}
	void OnRightClic(wxMenu* leMenu)
	{
		const wxString resourceFolder=ApplicationConfiguration::getResourcesFolder();
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_RUN_CALCULATION, wxGetTranslation("Run calculation"),resourceFolder + "/Bitmaps/popup_launch_calculation.png"));
		Element::OnRightClic(leMenu);
	}
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		return thisNode;
	}
};

#endif
