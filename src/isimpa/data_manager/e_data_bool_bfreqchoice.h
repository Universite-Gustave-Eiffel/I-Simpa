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

#include "data_manager/e_data_bool.h"

#ifndef __ELEMENT_BOOLEAN__BFREQ__
#define __ELEMENT_BOOLEAN__BFREQ__
/** \file e_data_bool_bfreqchoice.h
   \brief Classe spécialisant E_Data_Bool afin de représenter une valeur booléene de choix de bande de fréquence
*/

/**
   \brief Classe spécialisant E_Data_Bool afin de représenter une valeur booléene de choix de bande de fréquence
	@see MainPropGrid
*/
class E_Data_Bool_Bfreq: public E_Data_Bool
{
public:
	E_Data_Bool_Bfreq( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Data_Bool( noeudCourant ,  parent,ELEMENT_TYPE_BOOL_BFREQ)
	{
	}

	E_Data_Bool_Bfreq( Element* parent, wxString dataName,wxString dataLabel, bool defaultDataValue)
		:E_Data_Bool( parent, dataName,dataLabel,defaultDataValue,ELEMENT_TYPE_BOOL_BFREQ)
	{
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* NoeudCourant=new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"bfreq");
		NoeudCourant->AddAttribute("freq",this->elementInfo.libelleElement);
		
		if(elementValue)
			NoeudCourant->AddAttribute("docalc","1");
		else
			NoeudCourant->AddAttribute("docalc","0");

		return NoeudParent;
	}

};

#endif