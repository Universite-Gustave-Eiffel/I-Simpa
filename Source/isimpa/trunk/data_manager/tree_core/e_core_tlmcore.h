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

#ifndef __E_CORE_TLM__
#define __E_CORE_TLM__

/*! \file e_core_tlmcore.h
    \brief Element correspondant au moteur de calcul "TLM"
*/


/**
    \brief Element correspondant au moteur de calcul "TLM"
*/
class E_Core_Tlm: public E_Core_Core
{
public:

	E_Core_Tlm( Element* parent, wxXmlNode* noeudCourant)
		:E_Core_Core(parent,"TLM",ELEMENT_TYPE_CORE_TC,noeudCourant)
	{
		_("TLM");
	}

	E_Core_Tlm( Element* parent)
		:E_Core_Core(parent,"TLM",ELEMENT_TYPE_CORE_TC)
	{
		
		this->AppendFilsByType(ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE);
		Element* newElement=new E_Core_Core_Bfreqselection(this);
		this->AppendFils(newElement)->Hide();


		
		
		this->AppendPropertyText("modelName","","mesh.cbin",true,true)->Hide();
		this->AppendPropertyText("exeName","","TLM.exe")->Hide();
		this->AppendPropertyText("corePath","","tlm\\")->Hide();
		this->AppendPropertyText("tetrameshFileName","","tetramesh.mbin",true,true)->Hide();
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Core_Core::SaveXMLDoc(NoeudParent);
		thisNode->SetName("tlm");
		return thisNode;
	}
};

#endif
