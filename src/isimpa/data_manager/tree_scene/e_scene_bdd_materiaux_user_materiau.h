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

#include "data_manager/generic_element/e_materiau_user.h"
#include "data_manager/tree_scene/e_scene_bdd_materiaux_propmateriau.h"
#include "data_manager/tree_scene/e_scene_bdd_materiaux_rendermateriau.h"


#ifndef __USERMAT__
#define __USERMAT__


/** \file e_scene_bdd_materiaux_user_materiau.h
   \brief Element représentant un matériau, les surfaces vont tirer leurs propriétés de réflexion de ce type de matériau
*/

/**
   \brief Element représentant un matériau, les surfaces vont tirer leurs propriétés de réflexion de ce type de matériau
*/

class E_Scene_Bdd_Materiaux_User_Materiau: public Element
{
private:
	bool ignoreModification;
public:
	E_Scene_Bdd_Materiaux_User_Materiau( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"New material",Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_MATERIAU,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_MATERIAL_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_MATERIAL_CLOSE);
		this->elementInfo.userDestroyable=true;
		ignoreModification=false;
		wxGetTranslation("New material");
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
					if(typeEle==Element::ELEMENT_TYPE_MATERIAU_USER || typeEle==Element::ELEMENT_TYPE_MATERIAU_APP)
						this->AppendFils(new E_Materiau_User(currentChild,this));
					else if(typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_PROPMATERIAU)
						this->AppendFils(new E_Scene_Bdd_Materiaux_PropertyMaterial(currentChild,this));
					else if(typeEle==Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_MATERIAU_RENDER)
						this->AppendFils(new E_Scene_Bdd_Materiaux_RenderMaterial(currentChild,this));
				}
				currentChild = currentChild->GetNext();
			}
		}
	}
	
	E_Scene_Bdd_Materiaux_User_Materiau( Element* parent, wxString name=wxTRANSLATE("New material"))
		:Element(parent,name,Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_MATERIAU)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_MATERIAL_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_MATERIAL_CLOSE);
		this->elementInfo.userDestroyable=true;
		ignoreModification=false;
		this->AppendFils( new E_Materiau_User(this));
		this->AppendFils( new E_Scene_Bdd_Materiaux_PropertyMaterial(this));
		this->AppendFils( new E_Scene_Bdd_Materiaux_RenderMaterial(this));
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		//Traitement spécial car seulement les matériaux utilisés sont exportés vers le code de calcul
		//On utilise le noeud XML créé par le spectre comme noeud pour les éléments fils (autre que le spectre)
		Element* spectrumMaterial=this->GetElementByType(ELEMENT_TYPE_MATERIAU_USER);
		if(spectrumMaterial)
		{
			std::size_t spectrumid(spectrumMaterial->GetXmlId());
			wxXmlNode* spectrumNode=spectrumMaterial->SaveXMLCoreDoc(NoeudParent);
			if(spectrumNode!=NoeudParent) //Si le matériau est utilisé par le projet
			{
				for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
				{
					if((*itfils)->GetXmlId()!=spectrumid)
						(*itfils)->SaveXMLCoreDoc(spectrumNode);
				}
			}
		}
		return NoeudParent;
	}
	void BeginDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree)
	{
		treeEvent.Allow();
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
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{	
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent); //ne sauvegarde pas les propriétés
		thisNode->SetName("usermat"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
	//Bloque tout les evenement de propagation d'événement de modification
	void LockModificationEvent(bool unLock=false)
	{
    	ignoreModification=!unLock;
	}

private:
	/**
	 * Cette méthode met à jour une valeur du spectre du matériau
	 * @param[in] freq Frequence à mettre à jour
	 * @param[in] col Nom de la colonne à mettre à jour
	 * @param[in] newVal Nouvelle valeur du champ
	 * @return Vrai si element mis à jour avec succès
	 */
	bool UpdateGenValue(int freq,wxString col,float newVal)
	{
		Element* spectre=this->GetElementByType(ELEMENT_TYPE_MATERIAU_USER);
		if(spectre)
		{
			Element* ligne=spectre->GetElementByLibelle(Convertor::ToString(freq));
			if(ligne)
			{
				return ligne->UpdateDecimalConfig(col,newVal);
			}else{
				return false;
			}
		}else{
			return false;
		}
	}
	/**
	 * Cette méthode met à jour une propriété du matériau
	 * @param[in] prop Nom de la propriété
	 * @param[in] newVal Nouvelle valeur du champ
	 * @return Vrai si element mis à jour avec succès
	 */
	bool UpdateGenPropValue(wxString prop,wxString newVal)
	{
		Element* propEl=this->GetElementByType(ELEMENT_TYPE_SCENE_BDD_MATERIAUX_PROPMATERIAU);
		if(propEl)
		{
			return propEl->UpdateStringConfig(prop,newVal);
		}else{
			return false;
		}
	}
public:
	/**
	 * Mise à jour de l'absorption pour une bande de frequence 
	 * @param[in] freq Frequence à mettre à jour
	 * @param[in] newVal Nouvelle valeur du champ
	 * @return Vrai si element mis à jour avec succès
	 */
	bool UpdateAbsorptionValue(int freq,float newVal)
	{
		return UpdateGenValue(freq,"absorb",newVal);
	}
	/**
	 * Mise à jour de diffusion pour une bande de frequence
	 * @param[in] freq Frequence à mettre à jour
	 * @param[in] newVal Nouvelle valeur du champ
	 * @return Vrai si element mis à jour avec succès
	 */
	bool UpdateDiffusionValue(int freq,float newVal)
	{
		return UpdateGenValue(freq,"diffusion",newVal);
	}
	/**
	 * Mise à jour de propriétés
	 * @param[in] newVal Nouvelle valeur du champ
	 * @return Vrai si element mis à jour avec succès
	 */
	bool UpdateDescriptionValue(wxString newVal)
	{
		return UpdateGenPropValue("description",newVal);
	}
	/**
	 * Mise à jour de propriétés
	 * @param[in] newVal Nouvelle valeur du champ
	 * @return Vrai si element mis à jour avec succès
	 */
	bool UpdateReferenceValue(wxString newVal)
	{
		return UpdateGenPropValue("reference",newVal);
	}



	void Modified(Element* eModif)
	{
		if(!ignoreModification)
			Element::Modified(eModif); //Element de base modifié
	}
	
};

#endif
