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

/**  \file e_scene_projet_userconfiguration.h
 *   \brief Propriétés du projet n'ayant aucune importance pour les méthodes du calcul
 * 
 *   Il s'agit d'information relatives au projet tel que le nom du projet, son auteur etc..	
 */

/**
 *   \brief Propriétés du projet n'ayant aucune importance pour les méthodes du calcul
 * 
 *   Il s'agit d'information relatives au projet tel que le nom du projet, son auteur etc..	
 * 
 */
class E_Scene_UserConfiguration: public Element
{
private:

	void InitProperties() 
	{
		this->AppendPropertyText("projectname",wxTRANSLATE("Project name"),wxGetTranslation("New project"));
		this->AppendPropertyText("projectdesc",wxTRANSLATE("Project description"),"");
		this->AppendPropertyText("projectuser",wxTRANSLATE("Author"),"");
		wxDateTime maintenant=wxDateTime::UNow();
		wxString folderDated=maintenant.Format("%d-%m-%Y %Hh%Mm");
		this->AppendPropertyText("projectdate",wxTRANSLATE("Date"),folderDated);
	}
public:
	E_Scene_UserConfiguration( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Configuration",Element::ELEMENT_TYPE_SCENE_PROJET_USERCONFIGURATION,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_PROJECT_AUTHOR);
		wxGetTranslation("Configuration");
	}

	E_Scene_UserConfiguration( Element* parent)
		:Element(parent,"Configuration",Element::ELEMENT_TYPE_SCENE_PROJET_USERCONFIGURATION)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_PROJECT_AUTHOR);
		InitProperties();
	}

	

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("Configuration"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
