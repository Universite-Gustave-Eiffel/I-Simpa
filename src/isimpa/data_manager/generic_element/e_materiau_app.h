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

#include "e_gammefrequence.h"


#ifndef __SPEC__APPMAT__
#define __SPEC__APPMAT__


/** \file e_materiau_app.h
   \brief Spécialisation de E_Materiau

   Ce spectre ne peut pas être modifié par l'utilisateur
*/

/**
   \brief Spécialisation de E_Materiau

   Ce spectre ne peut pas être modifié par l'utilisateur
	@see E_Scene_Bdd_Materiaux_App_Materiau
*/
class E_Materiau_App: public E_Materiau
{
public:
	E_Materiau_App( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Materiau(parent,"Reference material",Element::ELEMENT_TYPE_MATERIAU_APP,noeudCourant)
	{
		wxGetTranslation("Reference material");
		const std::vector<ApplicationConfiguration::t_freq> tabfreq=ApplicationConfiguration::GetAllFrequencies();
		//Les champs de saisies sont en lectures seules pour les matériaux d'application
		ignoreModification = true;
		for(int i=0;i<tabfreq.size();i++)
		{
			this->SetReadOnlyConfig(wxString::Format("%i",tabfreq[i]),true,-1); //-1 toute la ligne
		}
		ignoreModification = false;
	}
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Materiau::SaveXMLDoc(NoeudParent);
		thisNode->SetName("materiau"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};

#endif
