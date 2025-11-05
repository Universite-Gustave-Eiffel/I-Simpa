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

#include "e_materiau.h"

#ifndef __SPEC__USERMAT__
#define __SPEC__USERMAT__


/** \file e_materiau_user.h
   \brief Spécialisation de E_Materiau

   Ce spectre peut être modifié par l'utilisateur
*/

/**
   \brief Spécialisation de E_Materiau

	Ce spectre peut être modifié par l'utilisateur
	@see E_Scene_Bdd_Materiaux_User_Materiau
*/

class E_Materiau_User: public E_Materiau
{
public:
	E_Materiau_User( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Materiau(parent,"",Element::ELEMENT_TYPE_MATERIAU_USER,noeudCourant)
	{
	}

	E_Materiau_User( Element* parent)
		:E_Materiau(parent,"",Element::ELEMENT_TYPE_MATERIAU_USER)
	{
		//Ajoute l l'etat d'accès des champs
		InitProperties();
	}
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Materiau::SaveXMLDoc(NoeudParent);
		thisNode->SetName("materiau"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};

#endif