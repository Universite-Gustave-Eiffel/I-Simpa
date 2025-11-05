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
#include "data_manager/e_data_bool_bfreqchoice.h"
#include "data_manager/appconfig.h"

#ifndef _BFREQ_SELECTION_
#define _BFREQ_SELECTION_

/*! \file e_core_core_bfreqselection.h
    \brief Element permettant de limiter le spectre de fréquence à calculer.
*/

/**
 \brief Element permettant de limiter le spectre de fréquence à calculer.
*/
class E_Core_Core_Bfreqselection: public Element
{
public:
	E_Core_Core_Bfreqselection( wxXmlNode* nodeElement ,  Element* parent)
		:Element(parent,wxTRANSLATE("Frequency bands"),Element::ELEMENT_TYPE_CORE_CORE_BFREQSELECTION,nodeElement)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_SPECTRUM);
		wxString propVal;
		//Affecte la valeur de l'element

		if(nodeElement!=NULL && nodeElement->GetAttribute("wxid",&propVal))
		{
			wxString propValue;

			//Element initialisé AVEC Xml
			wxXmlNode* currentChild;
			currentChild = nodeElement->GetChildren();
			while(currentChild!=NULL)
			{
				if(currentChild->GetAttribute("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					if(typeEle==Element::ELEMENT_TYPE_BOOL_BFREQ)
					{
						this->AppendFils(new E_Data_Bool_Bfreq(currentChild,this));
					}			
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Core_Core_Bfreqselection( Element* parent)
		:Element(parent,wxTRANSLATE("Frequency bands"),Element::ELEMENT_TYPE_CORE_CORE_BFREQSELECTION)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_SPECTRUM);
		InitProperties();
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* NoeudCourant=new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"freq_enum");
		return Element::SaveXMLCoreDoc(NoeudCourant);
	}

	void SetPreselection( Element::IDEVENT preselectionMode)
	{
		short cur_count=0;
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			E_Data_Bool* elData=dynamic_cast<E_Data_Bool*>((*itfils));
			if(elData)
			{
				ApplicationConfiguration::t_freq freq=Convertor::ToInt(elData->GetElementInfos().libelleElement);
				bool checkItem=false;
				bool isOctave=(cur_count==1);
				switch(preselectionMode)
				{
					case Element::IDEVENT_BFREQ_PRESELECTION_ALL:
					case Element::IDEVENT_BFREQ_PRESELECTION_THIRD_BAND:
						checkItem=true;
						break;
					case Element::IDEVENT_BFREQ_PRESELECTION_BUILDING_THIRD_BAND:
						if(freq>=100 && freq<=5000)
							checkItem=true;
						break;
					case Element::IDEVENT_BFREQ_PRESELECTION_BAND:
						checkItem=isOctave;
						break;
					case Element::IDEVENT_BFREQ_PRESELECTION_BUILDING_BAND:
						if(isOctave && freq>=125 && freq<=4000)
							checkItem=true;
						break;					
				};
				elData->SetValue(checkItem);
				cur_count++;
				if(cur_count==3)
					cur_count=0;
			};
		}
	}
	void OnRightClic(wxMenu* leMenu)
	{
		wxMenu* submenupreselect=new wxMenu();
		submenupreselect->Append(Element::GetMenuItem(submenupreselect,Element::IDEVENT_BFREQ_PRESELECTION_NONE, wxGetTranslation("Unselect all")));
		submenupreselect->Append(Element::GetMenuItem(submenupreselect,Element::IDEVENT_BFREQ_PRESELECTION_ALL, wxGetTranslation("Select all")));
		wxMenu* submenupreselect_octave=new wxMenu();
		submenupreselect_octave->Append(Element::GetMenuItem(submenupreselect_octave,Element::IDEVENT_BFREQ_PRESELECTION_BAND, wxGetTranslation("All [63-16000] Hz")));
		submenupreselect_octave->Append(Element::GetMenuItem(submenupreselect_octave,Element::IDEVENT_BFREQ_PRESELECTION_BUILDING_BAND, wxGetTranslation("Building/Road [125-4000] Hz")));
		wxMenu* submenupreselect_thirdoctave=new wxMenu();
		submenupreselect_thirdoctave->Append(Element::GetMenuItem(submenupreselect_thirdoctave,Element::IDEVENT_BFREQ_PRESELECTION_THIRD_BAND, wxGetTranslation("All [50-20000] Hz")));
		submenupreselect_thirdoctave->Append(Element::GetMenuItem(submenupreselect_thirdoctave,Element::IDEVENT_BFREQ_PRESELECTION_BUILDING_THIRD_BAND, wxGetTranslation("Building/Road [100-5000] Hz")));
		submenupreselect->AppendSubMenu(submenupreselect_octave,wxGetTranslation("Octave"));
		submenupreselect->AppendSubMenu(submenupreselect_thirdoctave,wxGetTranslation("Third octave"));
		leMenu->AppendSubMenu(submenupreselect,wxGetTranslation("Automatic selection"));
		Element::OnRightClic(leMenu);
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("core_conf_bfreq"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
protected:
	/**
	 Initialisation des propriétés
	*/
	void InitProperties()
	{
		std::vector<ApplicationConfiguration::t_freq> freqTab(ApplicationConfiguration::GetAllFrequencies());

		for(int i=0;i<freqTab.size();i++)
		{
			this->AppendFils(new E_Data_Bool_Bfreq(this,Convertor::ToString(freqTab[i]),Convertor::ToString(freqTab[i])+" Hz",true));
		}
	}
	
};

#endif
