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

#include "e_gammefrequence.h"


/** \file e_gammefrequence_user.h
   \brief Classe spécialisant e_gammefrequence

    Représente le spectre personalisé utilisateur (modifiable)
*/

/**
   \brief Classe spécialisant e_gammefrequence 

    Représente le spectre personalisé utilisateur (modifiable)
*/

class E_GammeFrequence_User  : public E_GammeFrequence
{
public:
	E_GammeFrequence_User( wxXmlNode* noeudCourant ,  Element* parent)
		:E_GammeFrequence(parent,"User spectrum",Element::ELEMENT_TYPE_GAMMEFREQ_USER,noeudCourant)
	{
		
		this->elementInfo.userDestroyable=true;
		wxGetTranslation("User spectrum");
	}

	E_GammeFrequence_User( Element* parent)
		:E_GammeFrequence(parent,"User spectrum",Element::ELEMENT_TYPE_GAMMEFREQ_USER)
	{
		this->elementInfo.userDestroyable=true;
		InitProperties();
	}



	void InitProp()
	{
		ignoreNextUpdate=true;
		this->SetReadOnlyAllConfig(false,-1);
		ignoreNextUpdate=false;
	}

	void OnBeginLabelEdit(wxTreeEvent& treeEvent)
	{
		//On autorise l'edition en surchargeant l'événement
	}
	 
	void OnEndLabelEdit(wxTreeEvent& treeEvent)
	{
		if(treeEvent.GetLabel()!="")
		{
			this->elementInfo.libelleElement=treeEvent.GetLabel();
			this->Modified(this); //Indique le l'element a été modifié
		}
	}
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_GammeFrequence::SaveXMLDoc(NoeudParent);
		thisNode->SetName("frequences"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
