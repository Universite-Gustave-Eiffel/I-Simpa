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
#include "e_scene_recepteursp_recepteur.h"
/** \file e_scene_recepteursp.h
   \brief Element dossier contenant la liste des récepteurs ponctuels de la scène.
*/

/**
   \brief Element dossier contenant la liste des récepteurs ponctuels de la scène.
*/
class E_Scene_Recepteursp: public Element
{
public:
	E_Scene_Recepteursp( wxXmlNode* noeudCourant ,  Element* parent,const wxString& name="Punctual receivers")
		:Element(parent,name,Element::ELEMENT_TYPE_SCENE_RECEPTEURSP,noeudCourant)
	{

		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_PUNCTUAL_RECEIVERS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_PUNCTUAL_RECEIVERS_CLOSE);
		insertPasteMenu=true;
		_("Punctual receivers");
		if(noeudCourant!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va créer les fils de notre noeudCourant
			wxString propValue;
			while(currentChild!=NULL)
			{
				if(currentChild->GetPropVal("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);

					if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
					{
						this->AppendFils(new E_Scene_Recepteursp_Recepteur(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP)
					{
						E_Scene_Recepteursp* newgroup=new E_Scene_Recepteursp(currentChild,this);
						newgroup->SetUserGroup();
						this->AppendFils(newgroup);
					}

				}
				currentChild = currentChild->GetNext();
			}
		}
	}
	bool InsertChildrens( wxXmlNode* currentChild )
	{
		bool somethingAdded=false;

		// On va créer les fils de notre noeudCourant
		wxString propValue;
		while(currentChild!=NULL)
		{
			if(currentChild->GetPropVal("eid",&propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
				{
					wxString recpName;
					currentChild->GetPropVal("name",&recpName);
					currentChild->DeleteProperty("name");
					currentChild->AddProperty("name",wxString::Format(_("Copy of %s"),recpName));
					this->AppendFils(new E_Scene_Recepteursp_Recepteur(currentChild,this))->ForceBottomModified();
					somethingAdded=true;
				}else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP)
				{
					wxString recpName;
					currentChild->GetPropVal("name",&recpName);
					currentChild->DeleteProperty("name");
					currentChild->AddProperty("name",wxString::Format(_("Copy of %s"),recpName));
					E_Scene_Recepteursp* newel=new E_Scene_Recepteursp(currentChild,this);
					newel->SetUserGroup();
					newel->ForceBottomModified();
					this->AppendFils(newel);
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
			this->Modified(this);
		}

	}
	void SetUserGroup()
	{
		this->elementInfo.userDestroyable=true;
	}

	void OnBeginLabelEdit(wxTreeEvent& treeEvent)
	{
		if(!this->elementInfo.userDestroyable)
			Element::OnBeginLabelEdit(treeEvent);
	}

	void OnEndLabelEdit(wxTreeEvent& treeEvent)
	{
		if(this->elementInfo.userDestroyable && treeEvent.GetLabel()!="")
		{
			this->elementInfo.libelleElement=treeEvent.GetLabel();
			this->Modified(this); //Indique le l'element a été modifié
		}
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(!this->elementInfo.userDestroyable)
		{
			wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"recepteursp");
			Element::SaveXMLCoreDoc(thisNode);
			return thisNode;
		}else{
			return Element::SaveXMLCoreDoc(NoeudParent);
		}
	}
	E_Scene_Recepteursp( Element* parent)
		:Element(parent,"Punctual receivers",Element::ELEMENT_TYPE_SCENE_RECEPTEURSP)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_PUNCTUAL_RECEIVERS_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_PUNCTUAL_RECEIVERS_CLOSE);
		insertPasteMenu=true;
	}



	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle="")
	{
		if(etypefils==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
		{
			if(libelle=="")
				return this->AppendFils(new E_Scene_Recepteursp_Recepteur(this));
			else
				return this->AppendFils(new E_Scene_Recepteursp_Recepteur(this,libelle));
		}else if(etypefils==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP)
		{
			E_Scene_Recepteursp* newgroup=new E_Scene_Recepteursp(this);
			newgroup->SetUserGroup();
			return this->AppendFils(newgroup);
		}
		return Element::AppendFilsByType(etypefils,libelle);
	}
	void OnRightClic(wxMenu* leMenu)
	{
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_RECEPTEUR_P, _("New receiver"),"./Bitmaps/popup_new.png"));
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_RECEPTEURP_GROUP, _("New group"),"./Bitmaps/popup_new.png"));
		Element::OnRightClic(leMenu);
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("recepteursp"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}

	void BeginDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree)
	{
		if(this->elementInfo.userDestroyable)
			treeEvent.Allow();
		else
			Element::BeginDrag(treeEvent,tree);
	}

	void EndDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree,std::vector<Element*> dragLst)
	{
		for(int i=0;i<dragLst.size();i++)
		{
			Element::t_elementInfo infosDragEl=dragLst[i]->GetElementInfos();
			if(infosDragEl.typeElement==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP && infosDragEl.idElement!=this->elementInfo.idElement)
			{
				dragLst[i]->Reparent(this);
			}else if(infosDragEl.typeElement==Element::ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR && infosDragEl.idParent!=this->elementInfo.idElement)
			{
				dragLst[i]->Reparent(this);
			}
		}
	}
};
