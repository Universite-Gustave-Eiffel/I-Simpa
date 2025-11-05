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
#include "data_manager/generic_element/e_gammefrequence_app.h"
#include "data_manager/appconfig.h"

/** \file e_scene_bdd_spectrums_app.h
   \brief Element représentant un dossier contenant un ou plusieurs spectres et/ou un ou plusieurs groupes de spectres
*/

/**
   \brief Element représentant un dossier contenant un ou plusieurs spectres et/ou un ou plusieurs groupes de spectres
*/

class E_Scene_Bdd_Spectrums_Application: public Element
{
public:
	E_Scene_Bdd_Spectrums_Application( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Reference",Element::ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_APP,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_APPLICATION_SPECTRUMS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_APPLICATION_SPECTRUMS_CLOSE);
		wxGetTranslation("Reference");

		wxXmlNode* noeudCfg=ApplicationConfiguration::GetAppSpectreNode();

		if(noeudCfg!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCfg->GetChildren();
			// On va créer les fils de notre noeudCourant
			wxString propValue;
			while(currentChild!=NULL)
			{
				if(currentChild->GetAttribute("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					if(typeEle==Element::ELEMENT_TYPE_GAMMEFREQ_USER || typeEle==Element::ELEMENT_TYPE_GAMMEFREQ_APP)
						this->AppendFils(new E_GammeFrequence_App(currentChild,this));
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Bdd_Spectrums_Application( Element* parent)
		:Element(parent,"Reference",Element::ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_APP)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_APPLICATION_SPECTRUMS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_APPLICATION_SPECTRUMS_CLOSE);
	}


	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDocWithoutChildren(NoeudParent);
		thisNode->SetName("appspectre"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}

};
