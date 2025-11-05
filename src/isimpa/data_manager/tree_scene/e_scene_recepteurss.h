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
#include "e_scene_recepteurss_recepteur.h"
#include "e_scene_recepteurss_recepteurcoupe.h"

/** \file e_scene_recepteurss.h
   \brief Element dossier contenant la liste des récepteurs surfaciques de la scène.
*/

/**
   \brief Element dossier contenant la liste des récepteurs surfaciques de la scène.
*/

class E_Scene_Recepteurss: public Element
{
public:
	E_Scene_Recepteurss( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,wxTRANSLATE("Surface receivers"),Element::ELEMENT_TYPE_SCENE_RECEPTEURSS,noeudCourant)
	{
		insertPasteMenu=true;
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACE_RECEIVERS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACE_RECEIVERS_CLOSE);
		if(noeudCourant!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va créer les fils de notre noeudCourant
			wxString propValue;
			while(currentChild!=NULL)
			{
				if(currentChild->GetAttribute("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR)
						this->AppendFils(new E_Scene_Recepteurss_Recepteur(currentChild,this));
					else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE)
						this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe(currentChild,this));
				}
				currentChild = currentChild->GetNext();
			}
		}
	}
	E_Scene_Recepteurss( Element* parent)
		:Element(parent,wxTRANSLATE("Surface receivers"),Element::ELEMENT_TYPE_SCENE_RECEPTEURSS)
	{
		insertPasteMenu=true;
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACE_RECEIVERS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACE_RECEIVERS_CLOSE);

	}
	bool InsertChildrens( wxXmlNode* currentChild )
	{
		bool somethingAdded=false;
		// On va créer les fils de notre noeudCourant
		wxString propValue;
		while(currentChild!=NULL)
		{
			if(currentChild->GetAttribute("eid",&propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				
				if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR)
				{
					this->AppendFils(new E_Scene_Recepteurss_Recepteur(currentChild,this));
					somethingAdded=true;
				}else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE){
					this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe(currentChild,this));
					somethingAdded=true;
				}
			}
			currentChild = currentChild->GetNext();
		}
		return somethingAdded;
	}
	
	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle="")
	{
		if(etypefils==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR)
		{
			if(libelle=="")
				return this->AppendFils(new E_Scene_Recepteurss_Recepteur(this));
			else
				return this->AppendFils(new E_Scene_Recepteurss_Recepteur(this,libelle));
		}else if(etypefils==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE)
		{
			if(libelle=="")
				return this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe(this));
			else
				return this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe(this,libelle));
		}
		return NULL;
	}


	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"recepteurss");
		Element::SaveXMLCoreDoc(thisNode);
		return thisNode;
	}

	void OnRightClic(wxMenu* leMenu)
	{	
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_RECEPTEUR_S, wxGetTranslation("New scene receiver"),"./Bitmaps/popup_new.png"));
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_RECEPTEUR_S_COUPE, wxGetTranslation("New plane receiver"),"./Bitmaps/popup_new.png"));
		Element::OnRightClic(leMenu);
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
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("recepteurss"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}


};
