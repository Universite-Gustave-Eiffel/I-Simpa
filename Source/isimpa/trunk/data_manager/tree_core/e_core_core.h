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

	/** 
	 * Initialisation des propriétés du coeur de calcul. Seulement lors de la création d'un nouveau projet
	 * @param[in] coreData Données du coeur de calcul
	 */
	/*
	void InitHiddenConfig()
	{
		this->AppendPropertyText("modelName","",coreData.modelName,true,true)->Hide();
		this->AppendPropertyText("exeName","",coreData.exeName)->Hide();
		this->AppendPropertyText("corePath","",coreData.corePath)->Hide();
		this->AppendPropertyText("tetrameshFileName","",coreData.tetrahedraMeshFileName,true,true)->Hide();
	}
	*/

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
				if(currentChild->GetPropVal("eid",&propValue))
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

		NoeudCourant->AddProperty("recepteurss_directory",ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FOLDER_PATH);
		NoeudCourant->AddProperty("recepteurss_filename",ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FILENAME);
		NoeudCourant->AddProperty("recepteurss_cut_filename",ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_CUT_FILENAME);

		NoeudCourant->AddProperty("receiversp_directory",wxString("Punctual_receivers"));
		NoeudCourant->AddProperty("receiversp_filename","soundpressure.recp");
		NoeudCourant->AddProperty("receiversp_filename_adv","advParams.gap");
		NoeudCourant->AddProperty("cumul_filename","Cumul_energetique.recp");

		return Element::SaveXMLCoreDoc(NoeudCourant);
	}
	void OnRightClic(wxMenu* leMenu)
	{
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_RUN_CALCULATION, _("Run calculation"),"./Bitmaps/popup_launch_calculation.png"));
		Element::OnRightClic(leMenu);
	}
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		return thisNode;
	}
};

#endif
