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
#include "data_manager/generic_element/e_gammefrequence_user.h"
/** \file e_scene_bdd_spectrums_user.h
   \brief Element représentant un dossier contenant un ou plusieurs spectres et/ou un ou plusieurs groupes de spectres
*/

/**
   \brief Element représentant un dossier contenant un ou plusieurs spectres et/ou un ou plusieurs groupes de spectres
*/
class E_Scene_Bdd_Spectrums_User: public Element
{
public:
	E_Scene_Bdd_Spectrums_User( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"User",Element::ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_USER,noeudCourant)
	{
		insertPasteMenu=true;
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_USER_SPECTRUMS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_USER_SPECTRUMS_CLOSE);
		_("User");
		if(noeudCourant!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va créer les fils de notre noeudCourant
			InsertChildrens(currentChild);
		}
	}

	E_Scene_Bdd_Spectrums_User( Element* parent)
		:Element(parent,"User",Element::ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_USER)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_USER_SPECTRUMS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_USER_SPECTRUMS_CLOSE);
		insertPasteMenu=true;
	}

	
	void OnRightClic(wxMenu* leMenu)
	{
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_USERFREQ, _("New spectrum"),"./Bitmaps/popup_new.png"));
		Element::OnRightClic(leMenu);
	}

	bool InsertChildrens(wxXmlNode* currentChild)
	{
		bool somethingAdded(false);
		wxString propValue;
		while(currentChild!=NULL)
		{
			if(currentChild->GetPropVal("eid",&propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				if(typeEle==Element::ELEMENT_TYPE_GAMMEFREQ_USER || typeEle==Element::ELEMENT_TYPE_GAMMEFREQ_APP)
				{
					this->AppendFils(new E_GammeFrequence_User(currentChild,this));
					somethingAdded=true;
				}
			}
			currentChild = currentChild->GetNext();
		}
		return somethingAdded;
	}

	void OnPaste(wxXmlNode* nodeElement)
	{
		//On ajoute les éléments compatibles
		wxXmlNode* firstChild=nodeElement->GetChildren();
		if(!InsertChildrens(firstChild))
		{
			Element::OnPaste(nodeElement);
		}
		else
		{
			Element::FillWxTree(); //ajout dans l'arbre des données
			//Indique la modification de cette branche
			this->ForceBottomModified();
			this->Modified(this);
		}

	}
	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle="")
	{
		
		if(etypefils==Element::ELEMENT_TYPE_GAMMEFREQ_USER)
			return this->AppendFils(new E_GammeFrequence_User(this));
		return Element::AppendFilsByType(etypefils,libelle);
	}
	
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("userspectre"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}


};
