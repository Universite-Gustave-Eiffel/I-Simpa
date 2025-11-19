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
#include "data_manager/generic_element/e_directivity_user.h"

/** \file e_scene_bdd_spectrums_app.h
\brief Element repr�sentant un dossier contenant une ou plusieures directivit�s et/ou un ou plusieurs groupes de directivit�s
*/

/**
\brief Element repr�sentant un dossier contenant une ou plusieures directivit�s et/ou un ou plusieurs groupes de directivit�s
*/

class E_Scene_Bdd_Directivities_User : public Element
{
public:
	E_Scene_Bdd_Directivities_User(wxXmlNode* noeudCourant, Element* parent)
		:Element(parent, "User", Element::ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_USER, noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED, GRAPH_USER_DIRECTIVITY_OPEN);
		SetIcon(GRAPH_STATE_NORMAL, GRAPH_USER_DIRECTIVITY_CLOSE);
		insertPasteMenu = true;

		if (noeudCourant != NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va cr�er les fils de notre noeudCourant
			InsertChildrens(currentChild);
		}
	}

	E_Scene_Bdd_Directivities_User(Element* parent)
		:Element(parent, "User", Element::ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_USER)
	{
		SetIcon(GRAPH_STATE_EXPANDED, GRAPH_ITEM);
		SetIcon(GRAPH_STATE_NORMAL, GRAPH_ITEM);
		insertPasteMenu = true;
	}

	bool InsertChildrens(wxXmlNode* currentChild)
	{
		bool childAdded {false};
		wxString propValue;
		while (currentChild != NULL)
		{
			if (currentChild->GetAttribute("eid", &propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				if (typeEle == Element::ELEMENT_TYPE_DIRECTIVITIES_USER)
				{
					this->AppendFils(new E_Directivity_User(currentChild, this));
					childAdded = true;
				}
			}
			currentChild = currentChild->GetNext();
		}
		return childAdded;
	}

	static void RemoveDirectivityId(wxXmlNode* nodeElement)
	{
		if (nodeElement != NULL)
		{
			nodeElement->DeleteAttribute("iddirectivity");
			wxXmlNode* childElement = nodeElement->GetChildren();
			while (childElement != NULL)
			{
				RemoveDirectivityId(childElement);
				childElement = childElement->GetNext();
			}
		}
	}

	void OnPaste(wxXmlNode* nodeElement) {
	
		RemoveDirectivityId(nodeElement);
		wxXmlNode* firstChild = nodeElement->GetChildren();
		if (!InsertChildrens(firstChild))
		{
			Element::OnPaste(nodeElement);
		}
		else
		{
			Element::FillWxTree(); //ajout dans l'arbre des donn�es
								   //Indique la modification de cette branche
			this->ForceBottomModified();
			this->Modified(this);
		}
	}

	void OnRightClic(wxMenu* leMenu)
	{
		const wxString resourceFolder=ApplicationConfiguration::getResourcesFolder();
		leMenu->Append(GetMenuItem(leMenu, Element::IDEVENT_NEW_USERDIRECTIV, wxGetTranslation("New directivity"), resourceFolder + "/Bitmaps/popup_new.png"));
		Element::OnRightClic(leMenu);
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("userdirectivities"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}

	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils, const wxString& libelle = "")
	{
		if (etypefils == Element::ELEMENT_TYPE_DIRECTIVITIES_USER)
			return this->AppendFils(new E_Directivity_User(this));
		return Element::AppendFilsByType(etypefils, libelle);
	}
};
