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
#include "e_scene_sources_source.h"


/**  \file e_scene_sources.h
 *   \brief Elément dossier contenant la liste de toutes les sources de la scène
 */

/**
 *   \brief Elément dossier contenant la liste de toutes les sources de la scène
 * 
 */
class E_Scene_Sources: public Element
{
public:
	E_Scene_Sources( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Sound sources",Element::ELEMENT_TYPE_SCENE_SOURCES,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SOUND_SOURCES_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SOUND_SOURCES_CLOSE);
		insertPasteMenu=true;
		wxGetTranslation("Sound sources");
		if(noeudCourant!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va créer les fils de notre noeudCourant
			wxString propValue;
			#ifdef _DEBUG
			std::size_t nbchilds(0);
			while(currentChild!=NULL)
			{
				nbchilds++;
				currentChild = currentChild->GetNext();
			}
			currentChild = noeudCourant->GetChildren();
			std::size_t curId(0);
			#endif
			while(currentChild!=NULL)
			{
				#ifdef _DEBUG
				curId++;
				#endif
				if(currentChild->GetAttribute("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
					{
						this->AppendFils(new E_Scene_Sources_Source(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_SOURCES)
					{
						E_Scene_Sources* newgroup=new E_Scene_Sources(currentChild,this);
						newgroup->SetUserGroup();
						this->AppendFils(newgroup);
					}
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Sources( Element* parent)
		:Element(parent,"Sound sources",Element::ELEMENT_TYPE_SCENE_SOURCES)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SOUND_SOURCES_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SOUND_SOURCES_CLOSE);
		insertPasteMenu=true;
	}

	
	void BeginDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree)
	{
		if(this->elementInfo.userDestroyable)
			treeEvent.Allow();
		else
			Element::BeginDrag(treeEvent,tree);
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

	void EndDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree,std::vector<Element*> dragLst)
	{
		for(int i=0;i<dragLst.size();i++)
		{
			Element::t_elementInfo infosDragEl=dragLst[i]->GetElementInfos();
			if(infosDragEl.typeElement==Element::ELEMENT_TYPE_SCENE_SOURCES && infosDragEl.idElement!=this->elementInfo.idElement)
			{
				dragLst[i]->Reparent(this);				
			}else if(infosDragEl.typeElement==Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE && infosDragEl.idParent!=this->elementInfo.idElement)
			{
				dragLst[i]->Reparent(this);
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
			if(currentChild->GetAttribute("eid",&propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				if(typeEle==Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
				{
					wxString elName;
					currentChild->GetAttribute("name",&elName);
					currentChild->DeleteAttribute("name");
					currentChild->AddAttribute("name",wxString::Format(wxGetTranslation("Copy of %s"),elName));
					this->AppendFils(new E_Scene_Sources_Source(currentChild,this))->ForceBottomModified();
					somethingAdded=true;
				}else if(typeEle==Element::ELEMENT_TYPE_SCENE_SOURCES)
				{
					wxString elName;
					currentChild->GetAttribute("name",&elName);
					currentChild->DeleteAttribute("name");
					currentChild->AddAttribute("name",wxString::Format(wxGetTranslation("Copy of %s"),elName));
					E_Scene_Sources* newfils=new E_Scene_Sources(currentChild,this);
					this->AppendFils(newfils);
					newfils->SetUserGroup();
					newfils->ForceBottomModified();
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
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(!this->elementInfo.userDestroyable)
		{
			wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"sources");
			Element::SaveXMLCoreDoc(thisNode);
			return thisNode;
		}else{
			return Element::SaveXMLCoreDoc(NoeudParent);
		}
	}
	
	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle="")
	{
		if(etypefils==Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
		{
			return this->AppendFils(new E_Scene_Sources_Source(this));
		}else if(etypefils==Element::ELEMENT_TYPE_SCENE_SOURCES)
		{
			E_Scene_Sources* newgroup=new E_Scene_Sources(this);
			newgroup->SetUserGroup();
			return this->AppendFils(newgroup);
		}
		return Element::AppendFilsByType(etypefils,libelle);
	}

	void OnRightClic(wxMenu* leMenu)
	{
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_SOURCE, wxGetTranslation("New source"),"./Bitmaps/popup_new.png"));
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_SOURCE_GROUP, wxGetTranslation("New group"),"./Bitmaps/popup_new.png"));
		Element::OnRightClic(leMenu);
	}

	void SetUserGroup()
	{
		this->elementInfo.userDestroyable=true;
	}
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("sources"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
