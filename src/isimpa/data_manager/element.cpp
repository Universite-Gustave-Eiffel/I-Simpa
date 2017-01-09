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

#include <algorithm>
#include <wx/log.h>
#include "element.h"
#include "IHM/uiTreeCtrl.h"
#include "IHM/ComboTreeCtrl.h"
#include "e_data_text.h"
#include "e_data_color.h"
#include "e_data_file.h"
#include "e_data_list.h"
#include "e_data_entier.h"
#include "e_data_float.h"
#include "e_data_bool.h"
#include "e_position.h"
#include "e_data_row.h"
#include "e_data_font.h"
#include <wx/clipbrd.h>
#include "data_manager/python_interface/instanceManager.hpp"
#include <wx/wupdlock.h>
#include <wx/dataobj.h>
#include "IHM/customItem/uimenuitem.hpp"
#include "last_cpp_include.hpp"
#include "manager/alphanum.hpp"

long Element::nbElement=0;

bool ElementSortPredicate(Element* lEl,Element* rEl)
{
	return *lEl<*rEl;
}

unsigned int CountXmlSubNodes(wxXmlNode* parent)
{
	wxXmlNode* currentChild;
	currentChild = parent->GetChildren();
	unsigned int sizeOfChilds=0;
	while(currentChild!=NULL)
	{
		sizeOfChilds++;
		currentChild = currentChild->GetNext();
	}
	return sizeOfChilds;
}


#ifdef USE_PYTHON
Element* New_E_Python_Extension( Element* parent,wxXmlNode* noeudCourant);
#endif

void SortChildrensByProperty(wxXmlNode* node,const wxString& propertyName)
{
	wxXmlNode* childrenNode=node->GetChildren();
	if(!childrenNode)
		return;
	bool nodeSwap=true;
	wxString buffer;
	long currentid;
	long nextid;
	while(nodeSwap)
	{
		nodeSwap=false;
		childrenNode=node->GetChildren();
		wxXmlNode* previousNode=NULL;
		childrenNode->GetAttribute(propertyName,&buffer);
		buffer.ToLong(&currentid);
		while(childrenNode)
		{
			wxXmlNode* nextNode=childrenNode->GetNext();
			if(nextNode)
			{
				nextNode->GetAttribute(propertyName,&buffer);
				buffer.ToLong(&nextid);
				if(nextid<currentid)
				{
					nodeSwap=true;
					//On doit intervertir les deux noeuds
					//Le noeud suivant du noeud precedent change
					if(previousNode)
						previousNode->SetNext(nextNode);
					else
						node->SetChildren(nextNode);
					//Le noeud suivant de next node est le noeud courant
					childrenNode->SetNext(nextNode->GetNext());
					nextNode->SetNext(childrenNode);
					//On met à jour les pointeurs
					childrenNode=nextNode;
					nextid=currentid;
					currentid = nextid;
				}
			}
			previousNode=childrenNode;
			childrenNode=childrenNode->GetNext();
		}
	}
}

Element::Element(Element* parent,const wxString& Nom,ELEMENT_TYPE _type,wxXmlNode* nodeElement)
{
	this->elementInfo.userDestroyable=false;
	this->elementInfo.hidden=false;
	this->elementInfo.exportLblToCore=false;
	this->elementInfo.expanded=false;
	this->allow_multiple_selection=false;
	insertPasteMenu=false;
	this->pere=NULL;
	this->treeCtrl=NULL;
	drawable=false;
	ExportMode=false;

	this->pere=parent;
	if(parent!=NULL)
	{
		this->elementInfo.idParent=parent->elementInfo.idParent;
	}
	this->elementInfo.typeElement=_type;

	this->elementInfo.graphElement=GRAPH_FOLDER;
	this->elementInfo.graphElementStateOpen=GRAPH_FOLDER_OPEN;

	wxString propVal;
	this->elementInfo.libelleElement=Nom;

	SetXmlId();

	if(nodeElement==NULL)
	{   //Element initialisé SANS XML

		this->needUpdate=true;
		this->elementInfo.expanded=false;
	}else{
		nodeElement->DeleteAttribute("wxid");
		nodeElement->AddAttribute("wxid",Convertor::ToString(this->elementInfo.xmlIdElement));
		//Element initialisé AVEC Xml
		this->needUpdate=false;
		if(nodeElement->GetAttribute("name",&propVal))
			this->elementInfo.libelleElement=propVal;

		//Temporaire
		/*
		Element* testDbg=this;
		wxString fullName;
		while(testDbg!=NULL)
		{
			fullName.insert(0,wxString::Format("%i/",testDbg->GetElementInfos().xmlIdElement));
			testDbg=testDbg->GetElementParent();
		}
		wxLogDebug(fullName);
		*/
		//Fin temporaire

		if(nodeElement->GetAttribute("exp",&propVal))
		{
			this->elementInfo.expanded=true;
			//if(this->elementInfo.graphElement==Element::GRAPH_FOLDER)
			//	this->elementInfo.graphElement=Element::GRAPH_FOLDER_OPEN;
		}

		if(nodeElement->GetAttribute("hid",&propVal))
		{
			this->elementInfo.hidden=true;
		}
		//Tri des fils en fonction de leur identifiant
		SortChildrensByProperty(nodeElement,"wxid");

		// Ajout des fils de cet élément

		wxXmlNode* currentChild = nodeElement->GetChildren();

		// On va créer les fils de propriétés de notre noeudCourant
		wxString propValue;
		while(currentChild!=NULL)
		{
			if(currentChild->GetAttribute("eid",&propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				Element* newEle=NULL;
				switch(typeEle)
				{
					case ELEMENT_TYPE_FLOAT:
						newEle=new E_Data_Float(currentChild,this);
						break;
					case ELEMENT_TYPE_INTEGER:
						newEle=new E_Data_Integer(currentChild,this);
						break;
					case ELEMENT_TYPE_BOOL:
						newEle=new E_Data_Bool(currentChild,this);
						break;
					case ELEMENT_TYPE_TEXT:
						newEle=new E_Data_Text(currentChild,this);
						break;
					case ELEMENT_TYPE_LIST:
						newEle=new E_Data_List(currentChild,this);
						break;
					case ELEMENT_TYPE_ROW:
						newEle=new E_Data_Row(currentChild,this);
						break;
					case ELEMENT_TYPE_COLOR:
						newEle=new E_Data_Color(currentChild,this);
						break;
					case ELEMENT_TYPE_POSITION:
						newEle=new E_Position(currentChild,this);
						break;
					case ELEMENT_TYPE_FONT:
						newEle=new E_Data_Font(currentChild,this);
						break;

					#ifdef USE_PYTHON
					case ELEMENT_TYPE_PYTHON_EXTENSION:
						newEle=New_E_Python_Extension( this,currentChild);
						break;
					#endif
				};
				if(newEle)
				{
					this->AppendFils(newEle);
				}
			}
			currentChild = currentChild->GetNext();
		}
	}
	InstanceManager::AppendReference(OBJECT_REF_TYPE_ELEMENT,elementInfo.xmlIdElement,this);
}

void Element::OnPaste(wxXmlNode* nodeElement)
{
	wxBell(); //Avertissement de l'utilisateur que cette fonction n'est pas autorisée
}
bool Element::OnElementRemoved()
{
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		(*itfils)->OnElementRemoved();
	return true;
}
void Element::SetXmlId(int newXmlId)
{
	if(newXmlId>=0)
	{
		if(newXmlId>GetCompteur())
			SetCompteur(newXmlId);
		this->elementInfo.xmlIdElement=newXmlId;
	}else{
		this->elementInfo.xmlIdElement=GetCompteur()+1;
		SetCompteur(this->elementInfo.xmlIdElement);
	}
}

int Element::GetCompteur()
{
	return Element::nbElement;
}

void Element::SetCompteur(int newValue)
{
	Element::nbElement=newValue;
}

//Cet élément est differend que le fichier de sauvegarde dans /current/
void Element::Modified(Element* elementUpdated)
{
	this->needUpdate=true;
	if(!this->evt_catchlst.empty())
		for(std::list<EventCatcher>::iterator itevt_manager=this->evt_catchlst.begin();itevt_manager!=this->evt_catchlst.end();itevt_manager++)
			(*itevt_manager)->OnModified(elementUpdated->GetXmlId());
	if(this->pere!=NULL)
		this->pere->Modified(elementUpdated);
}
bool Element::IsDrawable()
{
	if(drawable)
		return true;
	else if(this->pere)
		return this->pere->IsDrawable();
	else
		return false;
}
void Element::SetDrawable()
{
	drawable=true;
	//for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	//	(*itfils)->SetDrawable();
}

//Recherche parmis les fils du parent lequel des noeuds correspond à notre élément
wxXmlNode* Element::SeekParentNodeToThisNode(wxXmlNode* noeudParent)
{
	wxXmlNode* noeudCourant=noeudParent->GetChildren();
	wxXmlNode* noeudRecherche=NULL;
	wxString PropVal;
	long xid;
	while(noeudCourant!=NULL && noeudRecherche==NULL)
	{
		if(noeudCourant->GetAttribute("wxid",&PropVal))
		{
			PropVal.ToLong(&xid);
			if(xid==this->elementInfo.xmlIdElement)
				noeudRecherche=noeudCourant;
		}
		noeudCourant=noeudCourant->GetNext();
	}
	return noeudRecherche;
}

Element::~Element()
{
	InstanceManager::RemoveReference(OBJECT_REF_TYPE_ELEMENT,elementInfo.xmlIdElement);

	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		delete (*itfils);

	//On se retire du contrôle
	//if(this->treeCtrl!=NULL && this->elementInfo.idElement.IsOk())
	//	this->treeCtrl->Delete(this->elementInfo.idElement);
}

void Element::GetAllElementByType(ELEMENT_TYPE typeElement,std::vector<Element*>& eleList)
{
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		if((*itfils)->GetElementInfos().typeElement==typeElement)
		{
			eleList.push_back((*itfils));
		}
		(*itfils)->GetAllElementByType(typeElement,eleList);
	}
}

void Element::Hide(bool visible)
{
	this->elementInfo.hidden=!visible;
}

Element* Element::GetElementByType(ELEMENT_TYPE typeElement)
{
	if(this->elementInfo.typeElement==typeElement)
		return this;
	else
	{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			Element* searchResult=(*itfils)->GetElementByType(typeElement);
			if(searchResult!=NULL)
				return searchResult;
		}
		return NULL;
	}

}

void Element::AppendEventCatcher(const EventCatcher& newEventCatcher)
{
	this->evt_catchlst.push_back(newEventCatcher);
}
void Element::AppendWitness(const LifeTimeWitness& newWitness)
{
	witnesses.push_back(newWitness);
}
Element* Element::GetElementByLibelle(wxString libelle)
{
	if(!this)
	{
	#ifndef _DEBUG
		return NULL;
	#else
		throw("NULL element access");
	#endif

	}
	if(this->elementInfo.libelleElement==libelle)
		return this;
	else
	{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			if((*itfils)->GetElementInfos().libelleElement==libelle)
				return (*itfils);
		}
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			Element* searchResult=(*itfils)->GetElementByLibelle(libelle);
			if(searchResult)
				return searchResult;
		}
		return NULL;
	}
}
Element* Element::GetElementByXMLId(int _xmlId)
{
	if(this->elementInfo.xmlIdElement==_xmlId)
		return this;
	else
	{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			Element* searchResult=(*itfils)->GetElementByXMLId(_xmlId);
			if(searchResult!=NULL)
				return searchResult;
		}
		return NULL;
	}
}
Element* Element::GetElementByTreeId(wxTreeItemId idElement)
{
	if(this->elementInfo.idElement==idElement)
		return this;
	else
	{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			Element* searchResult=(*itfils)->GetElementByTreeId(idElement);
			if(searchResult!=NULL)
				return searchResult;
		}
		return NULL;
	}

}

bool Element::DeleteElementByXmlId(const wxInt32& xmlIdElement,bool setModification)
{
	bool deleteCanceled=false;
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		if((*itfils)->elementInfo.xmlIdElement==xmlIdElement)
		{
			if((*itfils)->GetElementParent()==this)
			{
				if((*itfils)->OnElementRemoved())
				{
					delete (*itfils); //supprime l'element
				}else{
					deleteCanceled=true;
				}
			}
			if(!deleteCanceled)
			{
				this->fils.erase(itfils);	//retire le pointeur de la liste


				if(this->treeCtrl!= NULL && this->elementInfo.idElement.IsOk() && !this->treeCtrl->HasChildren(this->elementInfo.idElement))
				{
					this->treeCtrl->SetItemImage(this->elementInfo.idElement,this->elementInfo.graphElement);
					wxTreeEvent tEvt;
					this->OnCollapsing(tEvt);
				}


				if(setModification)
					this->Modified(this);
			}
			return !deleteCanceled;
		}
	}
	return false;
}
bool Element::DeleteElementByTreeId(wxTreeItemId idElement)
{
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		if((*itfils)->elementInfo.idElement==idElement)
		{
			this->DeleteElementByXmlId((*itfils)->elementInfo.xmlIdElement);
			return true;
		}
	}
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		if((*itfils)->DeleteElementByTreeId(idElement)==true)
			return true;
	}
	return false;
}

void Element::SetIcon( const Element::GRAPH_STATE& iconState, const Element::GRAPH& staticGraphId)
{
	switch(iconState)
	{
		case Element::GRAPH_STATE_EXPANDED:
			this->elementInfo.graphElementStateOpen=staticGraphId;
			break;
		case Element::GRAPH_STATE_NORMAL:
			this->elementInfo.graphElement=staticGraphId;
			break;
		case Element::GRAPH_STATE_ALL:
			this->elementInfo.graphElement=staticGraphId;
			this->elementInfo.graphElementStateOpen=staticGraphId;
			break;
	}
}

void Element::FillWxTree(wxTreeCtrl* treeToFeed, const wxTreeItemId& parentId)
{
	wxTreeItemId wxtreeidparent=parentId;
	if(treeToFeed==NULL)
		treeToFeed=this->treeCtrl;
	if(treeToFeed==NULL || this->elementInfo.hidden)
		return;
	//wxWindowUpdateLocker lockrefresh(treeToFeed);
	//On accède à la classe modifié
	uiTreeCtrl* customTree=dynamic_cast<uiTreeCtrl*>(treeToFeed);

	if(customTree)
	{
		if(this->treeCtrl!=treeToFeed)
			this->treeCtrl=treeToFeed;
		if(this->pere!=NULL)
			this->elementInfo.idParent=this->pere->GetElementInfos().idElement;

		if(!this->elementInfo.idElement.IsOk())
			customTree->AddElement(this);
	}else{
		//Arbre éphémére d'un popup de contrôle
		ComboTreeCtrl* comboTree=dynamic_cast<ComboTreeCtrl*>(treeToFeed);
		if(comboTree)
		{
			if(comboTree->ElementFitWithFilter(this))
				wxtreeidparent=comboTree->Associate(comboTree->AppendItem(parentId,_(this->elementInfo.libelleElement)),this);
		}
	}



	//On ajoute également les fils

	//On les trie par ordre alphabétique
	//std::sort(this->fils.begin( ), this->fils.end( ),ElementSortPredicate);
	std::size_t sonCount(this->fils.size());
	if(sonCount<500) //Optimisation, on désactive le tri si le nombre de fils est supérieur à 500
		this->fils.sort(ElementSortPredicate);
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		(*itfils)->FillWxTree(treeToFeed,wxtreeidparent);

	if(customTree)
	{
		if(this->elementInfo.expanded && this->elementInfo.idElement!=treeToFeed->GetRootItem())
		{
			if(treeToFeed->HasChildren(this->elementInfo.idElement))
			{
				treeToFeed->Expand(this->elementInfo.idElement);
			}else{
				this->elementInfo.expanded=false;
			}
		}
	}
}


void Element::SetTreeItemId(wxTreeItemId _idElement)
{
	this->elementInfo.idElement=_idElement;
}

const Element::t_elementInfo& Element::GetElementInfos( ) const
{
	return this->elementInfo;

}

wxInt32 Element::GetXmlId( ) const
{
	return this->elementInfo.xmlIdElement;
}
const wxString Element::GetElementNameForOrder() const
{
	return _(this->GetTreeLabel());
}
bool Element::operator>(Element &_rEl)
{
	wxString lft=this->GetElementNameForOrder();
	wxString rght=_rEl.GetElementNameForOrder();
	const char* left=WXSTRINGTOCHARPTR(lft);
	const char* right=WXSTRINGTOCHARPTR(rght);
	if( doj::alphanum_comp(left, right) > 0)
		return true;
	else
		return false;
}
bool Element::operator<(Element &_rEl)
{
	wxString lft=this->GetElementNameForOrder();
	wxString rght=_rEl.GetElementNameForOrder();
	const char* left=WXSTRINGTOCHARPTR(lft);
	const char* right=WXSTRINGTOCHARPTR(rght);
	if( doj::alphanum_comp(left, right) < 0)
		return true;
	else
		return false;
}

wxXmlNode* Element::GenericSaveXmlDoc(wxXmlNode* NoeudParent)
{
	//Sauvegarde les données de base
	//Les données spécifique sont sauvegardé par la classe spécifique

	//1ere étape on verifie si on doit effectuer une mise à jour
	//ou un ajout
	wxXmlNode* NoeudCourant;
	NoeudCourant=this->SeekParentNodeToThisNode(NoeudParent);

	if(NoeudCourant==NULL)
	{
		//On créé le xmlNode, premier fils du pere ou dernier frere
		NoeudCourant = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"NoNameNode");
	}
	//Mise à jour des données
	/*wxXmlProperty* proprietes=NoeudCourant->GetProperties();
	proprietes->*/
	NoeudCourant->DeleteAttribute("name");
	NoeudCourant->AddAttribute("name",this->elementInfo.libelleElement);
	NoeudCourant->DeleteAttribute("eid");
	NoeudCourant->AddAttribute("eid",Convertor::ToString(this->elementInfo.typeElement));

	NoeudCourant->DeleteAttribute("wxid");
	NoeudCourant->AddAttribute("wxid",Convertor::ToString(this->elementInfo.xmlIdElement));
	NoeudCourant->DeleteAttribute("exp");
	if(this->elementInfo.expanded)
		NoeudCourant->AddAttribute("exp","1");
	if(this->elementInfo.hidden)
	{
		NoeudCourant->DeleteAttribute("hid");
		NoeudCourant->AddAttribute("hid","1");
	}
	return NoeudCourant;
}
wxXmlNode* Element::SaveXMLCoreDoc(wxXmlNode* NoeudParent)
{
	if(this->elementInfo.exportLblToCore)
		NoeudParent->AddAttribute("lbl",this->elementInfo.libelleElement);
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		(*itfils)->SaveXMLCoreDoc(NoeudParent);
	}
	return NoeudParent;
}

wxXmlNode* Element::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	//Sauvegarde l'élément

	if(!ExportMode)
		this->needUpdate=false;

	wxXmlNode* NoeudCourant=this->GenericSaveXmlDoc(NoeudParent);

	if(!ExportMode)
		this->SaveChildren(NoeudCourant);

	return NoeudCourant;
}
wxXmlNode* Element::ExportToXMLDoc(wxXmlNode* NoeudParent)
{
	//Sauvegarde l'élément

	ExportMode=true;

	wxXmlNode* NoeudCourant=this->SaveXMLDoc(NoeudParent);

	ExportMode=false;

	//Sauvegarde des fils
	if(NoeudCourant)
	{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			(*itfils)->ExportToXMLDoc(NoeudCourant);
		}
	}

	return NoeudCourant;
}
wxXmlNode* Element::SaveXMLDocWithoutChildren(wxXmlNode* NoeudParent)
{
	//Sauvegarde l'élément

	if(!ExportMode)
		this->needUpdate=false;

	wxXmlNode* NoeudCourant=this->GenericSaveXmlDoc(NoeudParent);

	return NoeudCourant;

}

void Element::BeginDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree)
{

}

void  Element::AllowMultipleSelection(const bool& allow)
{
	this->allow_multiple_selection=allow;
}
long Element::CountXmlNodeChildren(wxXmlNode* NoeudCourant)
{
	wxXmlNode* NoeudCurseur=NoeudCourant->GetChildren();
	long nbFils=0;
	while(NoeudCurseur!=NULL)
	{
		nbFils++;
		NoeudCurseur=NoeudCurseur->GetNext();
	}
	return nbFils;
}

bool Element::IsAskUpdate( )
{
	return this->needUpdate;
}
bool Element::HasChildren( )
{
	return false; // on ne force pas l'arbre a nous mettre parent si nous n'avons pas d'elements fils (cette méthode est surchargé par un élément qui va créer ses fils durant l'execution et non au démmarage du projet)
}
void Element::SaveChildren(wxXmlNode* NoeudCourant)
{
		wxString lstSon="|";
		wxXmlNode* nodeToDelete;

		//Sauvegarde des fils
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			t_elementInfo infofils=(*itfils)->GetElementInfos();
			if((*itfils)->IsAskUpdate())
				(*itfils)->SaveXMLDoc(NoeudCourant);
			lstSon<<infofils.xmlIdElement<<"|";
		}

		//Suppression des noeuds fils XML n'existant plus dans les Elements
		wxXmlNode* NoeudCurseur=NoeudCourant->GetChildren();
		wxString propVal;
		while(NoeudCurseur!=NULL)
		{
			nodeToDelete=NULL;
			if(NoeudCurseur->GetAttribute("wxid",&propVal))
				if(!lstSon.Contains("|"+propVal+"|"))
					nodeToDelete=NoeudCurseur;
			NoeudCurseur=NoeudCurseur->GetNext();
			if(nodeToDelete!=NULL)
			{
				NoeudCourant->RemoveChild(nodeToDelete);
				delete nodeToDelete;
			}
		}
}

//Suppression d'élément sur un seul niveau
void Element::DeleteAllElementByType(ELEMENT_TYPE typeElementToDelete)
{
	bool isModified(true);
	while(isModified)
	{
		isModified=false;
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			Element::t_elementInfo filsElementInfo=(*itfils)->GetElementInfos();
			if(filsElementInfo.typeElement==typeElementToDelete && (*itfils)->GetElementParent()==this)
			{
				isModified=this->DeleteElementByXmlId(filsElementInfo.xmlIdElement);
				if(isModified)
					break;
			}
		}
	}
}

//Suppression d'élément sur plusieurs niveaux
void Element::DeleteAllElementByTypeR(ELEMENT_TYPE typeElementToDelete)
{
	this->DeleteAllElementByType(typeElementToDelete);
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		(*itfils)->DeleteAllElementByTypeR(typeElementToDelete);
	}
}
void Element::BrokeLinkToChild( Element* ChildToSeparate)
{
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		if((*itfils)==ChildToSeparate)	//si l'adresse des deux pointeurs est la même
		{
			this->fils.erase(itfils);
			break;
		}
	}
}

void Element::UnsetTreeId( )
{
	this->elementInfo.idElement.Unset();
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		(*itfils)->UnsetTreeId();
	}
}
void Element::Reparent( Element* newParent )
{
	//Contrôles
	//1 On ne peut pas nous assigner comme parent un de nos fils !
	if(newParent)
	{
		Element* nvParent=this->GetElementByTreeId(newParent->GetElementInfos().idElement);
		if(nvParent!=NULL)
		{
			wxLogMessage(_("Operation not allowed"));
			return;
		}
	}

	//Cet élément va changer de parent
	//On se retire du contrôle
	if(this->treeCtrl!=NULL && this->elementInfo.idElement.IsOk())
	{
		this->treeCtrl->Delete(this->elementInfo.idElement);
		this->UnsetTreeId();
	}
	//On se retire de l'ancien père
	if(this->pere!=NULL)
		this->pere->BrokeLinkToChild(this);
	//On se relie au nouveau pere
	this->pere=newParent;
	if(this->pere!=NULL)
	{
		this->pere->AppendFils(this);
		//On se rajoute dans l'arbre
		if(treeCtrl!=NULL)
			this->FillWxTree(treeCtrl);
		//On indique un changement d'etat
		this->Modified(this);
		this->ForceBottomModified();
	}
}

void Element::OnDoubleClic(wxTreeEvent& treeEvent)
{

}
Element* Element::AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle)
{
	return NULL;
}
Element* Element::AppendFils(Element* filsToAppend,bool appendToTree)
{
	this->fils.push_back(filsToAppend);
	Modified(this);

	if(appendToTree && this->treeCtrl)
		filsToAppend->FillWxTree(this->treeCtrl);
	//todo appel On_New_Element_Evt( parentId, elementId )
	if(!this->evt_catchlst.empty())
	{
		for(std::list<EventCatcher>::iterator itevt_manager=this->evt_catchlst.begin();itevt_manager!=this->evt_catchlst.end();itevt_manager++)
		{
			const t_elementInfo& einfo=filsToAppend->elementInfo;
			(*itevt_manager)->OnNewChildElement(einfo.typeElement,this->GetXmlId(),filsToAppend->GetXmlId());
		}
	}
	return filsToAppend;
}
wxMenuItem* Element::GetMenuItem(wxMenu* parent,int id,const wxString& label, wxMenu* subMenu ,const wxString& menuIcon)
{
	wxUiMenuItem *newmenu = new wxUiMenuItem(parent, id,label,"",wxITEM_NORMAL,subMenu);
	if(!menuIcon.IsEmpty())
	{
		wxBitmap iconBitmap(menuIcon, wxBITMAP_TYPE_ANY);
		if(iconBitmap.IsOk())
		{
			newmenu->SetBitmap(iconBitmap);
			newmenu->SetBitmapPath(menuIcon);

		}else{
			wxLogWarning(_("Popup icon file doesn't find:\n%s"),menuIcon);
		}
	}
	return newmenu;
}
wxMenuItem* Element::GetMenuItem(wxMenu* parent,int id,const wxString& label, const wxString& menuIcon)
{
	wxUiMenuItem *newmenu = new wxUiMenuItem(parent, id,label);
	if(!menuIcon.IsEmpty())
	{
		wxBitmap iconBitmap(menuIcon, wxBITMAP_TYPE_ANY);
		if(iconBitmap.IsOk())
		{
			newmenu->SetBitmap(iconBitmap);
			newmenu->SetBitmapPath(menuIcon);

		}else{
			wxLogWarning(_("Popup icon file doesn't find:\n%s"),menuIcon);
		}
	}
	return newmenu;
}
void Element::OnRightClic(wxMenu* leMenu)
{
	if(leMenu->GetMenuItemCount()>0)
		leMenu->AppendSeparator();
	//leMenu->Append(IDEVENT_COPIER, _("Copier"));
	leMenu->Append(GetMenuItem(leMenu,IDEVENT_COPIER, _("Copy"),"./Bitmaps/popup_copy.png"));
	wxMenuItem*	pasteMenu=leMenu->Append(GetMenuItem(leMenu,IDEVENT_COLLER, _("Paste"),"./Bitmaps/popup_paste.png"));

	wxTreeEvent renameEvent;
	renameEvent.Allow();
	this->OnBeginLabelEdit(renameEvent);

	if(renameEvent.IsAllowed() ||  this->elementInfo.userDestroyable)
		leMenu->AppendSeparator();
	if(renameEvent.IsAllowed())
		leMenu->Append(GetMenuItem(leMenu,IDEVENT_RENAME_ELEMENT, _("Rename"),"./Bitmaps/popup_rename.png"));

	if(this->elementInfo.userDestroyable)
		leMenu->Append(GetMenuItem(leMenu,IDEVENT_DELETE_ELEMENT, _("Delete"),"./Bitmaps/popup_delete.png"));

	//Si l'element accepte des données provenant de l'exterieur, et si il y a du texte dans le presse papier
	if(!insertPasteMenu || !wxTheClipboard->IsSupported(wxDataFormat(wxDF_TEXT)))
		pasteMenu->Enable(false);
	if(this->IsContainsProperties())
	{
		leMenu->AppendSeparator();
		leMenu->Append(GetMenuItem(leMenu,IDEVENT_GETPROPERTIES, _("Properties"),"./Bitmaps/popup_show_property.png"));
	}
}
void Element::ForceBottomModified()
{
	this->needUpdate=true;
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		(*itfils)->ForceBottomModified();
}
bool Element::IsContainsProperties()
{
	//On verifie si aucune propriétés n'est affiché, dans ce cas on désaction l'option de propriétés
	for(std::list<Element*>::iterator itchild=this->fils.begin();itchild!=fils.end();itchild++)
	{
		if(dynamic_cast<E_Data*>(*itchild))
		{
			if(!(*itchild)->GetElementInfos().hidden && (*itchild)->GetElementInfos().typeElement!=Element::ELEMENT_TYPE_POSITION )
				return true;
		}
	}
	return false;
}

bool Element::IsFittingWithThisType( const ELEMENT_TYPE& typeTest )
{
	if(typeTest==this->elementInfo.typeElement || typeTest==ELEMENT_TYPE_ELEMENT)
		return true;
	else
		return false;
}

wxString Element::GetColTitle(int col)
{
	return _("Value");
}

void Element::OnSelectTreeItem(wxTreeEvent& treeEvent)
{


}

void Element::EndDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree,std::vector<Element*> dragLst)
{
	treeEvent.Veto();
}

void Element::OnBeginLabelEdit(wxTreeEvent& treeEvent)
{
	treeEvent.Veto(); //L'élèment standard ne peut être renommé
}

void Element::OnEndLabelEdit(wxTreeEvent& treeEvent)
{
	treeEvent.Veto(); //L'élèment standard ne peut être renommé
}

void Element::OnCollapsing(wxTreeEvent& treeEvent)
{
	if(this->elementInfo.expanded)
	{
		this->elementInfo.expanded=false;
		this->Modified(this);
	}
}

void Element::OnExpanding(wxTreeEvent& treeEvent)
{
	if(!this->elementInfo.expanded)
	{
		this->elementInfo.expanded=true;
		this->Modified(this);
	}
}

void Element::GetChildElement(  std::list<Element*>& childs )
{
	childs.insert( childs.end( ), fils.begin( ), fils.end( ) );
	//childs.merge(childs);
}

void Element::InitProp()
{

}

bool Element::IsAskPropReload()
{
	return false;
}
bool Element::UpdateStringConfig(wxString name,wxString newValue)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_TEXT && infoEl.libelleElement==name)
		{
			E_Data_Text* dataEl=dynamic_cast<E_Data_Text*>((*itfils));
			dataEl->SetValue(newValue);
			return true;
		}
	}
	return false;
}

bool Element::UpdateDecimalConfig(wxString name,float newValue)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_FLOAT && infoEl.libelleElement==name)
		{
			E_Data_Float* dataEl=dynamic_cast<E_Data_Float*>((*itfils));
			dataEl->SetValue(newValue);
			return true;
		}
	}
	return false;
}

bool Element::UpdatePositionConfig(const wxString& name,const vec3& newValue)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_POSITION && infoEl.libelleElement==name)
		{
			E_Position* dataEl=dynamic_cast<E_Position*>((*itfils));
			dataEl->SetPosValue(newValue);
			return true;
		}
	}
	return false;
}
bool Element::UpdateEntierConfig(wxString name,int newValue)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_INTEGER && infoEl.libelleElement==name)
		{
			E_Data_Integer* dataEl=dynamic_cast<E_Data_Integer*>((*itfils));
			dataEl->SetValue(newValue);
			return true;
		}
	}
	return false;
}

bool Element::UpdateListConfig(wxString name,int newIndex)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_LIST && infoEl.libelleElement==name)
		{
			E_Data_List* dataEl=dynamic_cast<E_Data_List*>((*itfils));
			dataEl->SetValue(newIndex);
			return true;
		}
	}
	return false;
}

void Element::SetReadOnlyConfig(const wxString& name,bool readOnly,int col)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.libelleElement==name)
		{

			E_Data* elData=dynamic_cast<E_Data*>((*itfils));
			if(elData)
			{
				elData->SetReadOnly(readOnly,col);
				return;
			}
		}
	}
}

bool Element::IsReadOnlyConfig(const wxString& name,int col)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.libelleElement==name)
		{

			E_Data* elData=dynamic_cast<E_Data*>((*itfils));
			if(elData)
			{
				return elData->IsReadOnly(col);
			}
		}
	}
	#ifdef _DEBUG
	wxLogDebug(name+" n'existe pas !");
	#endif
	return true;
}
void Element::SetReadOnlyAllConfig(bool readOnly,int col)
{
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		E_Data* elData=dynamic_cast<E_Data*>((*itfils));
		if(elData)
		{
			elData->SetReadOnly(readOnly,col);
		}
	}
}
wxString Element::GetStringConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_TEXT && infoEl.libelleElement==name)
		{
			E_Data_Text* dataEl=dynamic_cast<E_Data_Text*>((*itfils));
			return dataEl->GetValue();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return "";
}

wxFont Element::GetFontConfig(const wxString& name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_FONT && infoEl.libelleElement==name)
		{
			E_Data_Font* dataEl=dynamic_cast<E_Data_Font*>((*itfils));
			return dataEl->GetValue();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return wxFont();
}

wxFileName Element::GetFileConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for (std::list<Element*>::iterator itfils = this->fils.begin(); itfils != this->fils.end(); itfils++)
	{
		const Element::t_elementInfo& infoEl = (*itfils)->GetElementInfos();
		if (infoEl.typeElement == Element::ELEMENT_TYPE_FILE && infoEl.libelleElement == name)
		{
			E_Data_File* dataEl = dynamic_cast<E_Data_File*>((*itfils));
			return dataEl->GetFile() ;
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"), this->elementInfo.libelleElement, name);
	return wxFileName();
}

vec3 Element::GetColorConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_COLOR && infoEl.libelleElement==name)
		{
			E_Data_Color* dataEl=dynamic_cast<E_Data_Color*>((*itfils));
			return dataEl->GetValue();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return vec3(0,0,0);
}

int Element::GetListConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_LIST && infoEl.libelleElement==name)
		{
			E_Data_List* dataEl=dynamic_cast<E_Data_List*>((*itfils));
			return dataEl->GetChoice();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return -1;
}
float Element::GetDecimalConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_FLOAT && infoEl.libelleElement==name)
		{
			E_Data_Float* dataEl=dynamic_cast<E_Data_Float*>((*itfils));
			return dataEl->GetValue();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return 0;
}

int Element::GetIntegerConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_INTEGER && infoEl.libelleElement==name)
		{
			E_Data_Integer* dataEl=dynamic_cast<E_Data_Integer*>((*itfils));
			return dataEl->GetValue();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return 0;
}

bool Element::UpdateBoolConfig(wxString name,bool newValue)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_BOOL && infoEl.libelleElement==name)
		{
			E_Data_Bool* dataEl=dynamic_cast<E_Data_Bool*>((*itfils));
			dataEl->SetValue(newValue);
			return true;
		}
	}
	return false;
}

bool Element::GetBoolConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_BOOL && infoEl.libelleElement==name)
		{
			E_Data_Bool* dataEl=dynamic_cast<E_Data_Bool*>((*itfils));
			return dataEl->GetValue();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return false;
}

void Element::LinkMenuWithPropBool(wxString name,wxMenuItem* menuItem)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_BOOL && infoEl.libelleElement==name)
		{
			E_Data_Bool* dataEl=dynamic_cast<E_Data_Bool*>((*itfils));
			dataEl->LinkMenu(menuItem);
			return;
		}
	}
	//L'élément n'a pas été trouve
	wxLogDebug("LinkMenuWithPropBool/ Element bool non trouvé");
}

vec3 Element::GetPositionConfig(wxString name)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const Element::t_elementInfo& infoEl=(*itfils)->GetElementInfos();
		if(infoEl.typeElement==Element::ELEMENT_TYPE_POSITION)
		{
			E_Position* dataEl=dynamic_cast<E_Position*>((*itfils));
			if(dataEl->GetElementInfos().libelleElement==name)
				return dataEl->GetPosValue();
		}
	}
	wxLogDebug(_("This field does not exist!\nName of element: %s Name of the field: %s"),this->elementInfo.libelleElement,name);
	return vec3(0,0,0);
}

bool Element::IsPropertyExist(wxString propertyName,Element** found)
{
	//On recherche les éléments qui porte le nom name
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		E_Data* dataEl=dynamic_cast<E_Data*>((*itfils));
		if(dataEl && (*itfils)->GetElementInfos().libelleElement==propertyName)
		{
			if(found!=NULL)
				*found=dataEl;
			return true;
		}
	}
	return false;
}

Element* Element::GetElementParent()
{
	return this->pere;
}

Element* Element::AppendPropertyText(wxString propertyName,wxString propertyLabel,wxString propertyDefaultValue,bool readOnly,bool exportToCore)
{
	Element* alreadyExist=NULL;
	if(!IsPropertyExist(propertyName,&alreadyExist))
	{
		E_Data_Text* nouvFils = new E_Data_Text(this,propertyName,propertyLabel,propertyDefaultValue,readOnly);
		nouvFils->SetXmlCoreVisibility(exportToCore);
		return this->AppendFils(nouvFils);
	}else{
		return alreadyExist;
	}
}

Element* Element::AppendPropertyPosition(wxString propertyName,wxString propertyLabel,vec3 propertyDefaultValue,bool exportToCore)
{
	Element* alreadyExist=NULL;
	if(!IsPropertyExist(propertyName,&alreadyExist))
	{
		return this->AppendFils(new E_Position(this,propertyName,propertyLabel,propertyDefaultValue,exportToCore));
	}else{
		return alreadyExist;
	}
}

Element* Element::AppendPropertyList(wxString propertyName,wxString propertyLabel,std::vector<wxString> &values,long defaultValue, bool asTitle, int hSize,std::vector<int> indiceValues,bool exportToCore)
{
	Element* alreadyExist=NULL;
	if(indiceValues.size()==0)
	{
		for(int i=0;i<values.size();i++)
			indiceValues.push_back(i);
	}
	if(!IsPropertyExist(propertyName,&alreadyExist))
	{
		E_Data_List* nouvFils = new E_Data_List(this,propertyName,propertyLabel,values,indiceValues,defaultValue, asTitle,hSize);
		nouvFils->SetXmlCoreVisibility(exportToCore);
		return this->AppendFils(nouvFils);
	}else{
		return alreadyExist;
	}
}

Element* Element::AppendPropertyColor(wxString propertyName,wxString propertyLabel,long defaultRed,long defaultGreen,long defaultBlue)
{
	Element* alreadyExist=NULL;
	if(!IsPropertyExist(propertyName,&alreadyExist))
		return this->AppendFils(new E_Data_Color(this,propertyName,propertyLabel,wxColor(defaultRed,defaultGreen,defaultBlue)));
	else
		return alreadyExist;
}

Element* Element::AppendPropertyFile(wxString propertyName, wxString propertyLabel, wxString storageFolder, wxString _dialogTitle, wxString _fileExtension)
{
	Element* alreadyExist = NULL;
	if (!IsPropertyExist(propertyName, &alreadyExist))
		return this->AppendFils(new E_Data_File(this, propertyName, propertyLabel, storageFolder, _dialogTitle, _fileExtension));
	else
		return alreadyExist;
}

Element* Element::AppendPropertyInteger(wxString propertyName,wxString propertyLabel,int propertyDefaultValue,bool exportToCore,bool isMaxValue,bool isMinValue,int maxValue,int minValue)
{
	Element* alreadyExist=NULL;
	if(!IsPropertyExist(propertyName,&alreadyExist))
	{
		E_Data_Integer *nouvfils=new E_Data_Integer(this,propertyName,propertyLabel,propertyDefaultValue);
		nouvfils->SetXmlCoreVisibility(exportToCore);
		if(isMaxValue)
			nouvfils->SetMaxValue(maxValue);
		if(isMinValue)
			nouvfils->SetMinValue(minValue);
		return this->AppendFils(nouvfils);
	}else{
		return alreadyExist;
	}
}
Element* Element::AppendPropertyDecimal(wxString propertyName,wxString propertyLabel,float propertyDefaultValue,bool readOnly,int precision,bool isMaxValue,bool isMinValue,float maxValue,float minValue,bool exportToCore)
{
	Element* alreadyExist=NULL;
	if(!IsPropertyExist(propertyName,&alreadyExist))
	{
		E_Data_Float* newData=new E_Data_Float(this,propertyName,propertyLabel,propertyDefaultValue,readOnly,precision);
		if(isMaxValue)
			newData->SetMaxValue(maxValue);
		if(isMinValue)
			newData->SetMinValue(minValue);
		newData->SetXmlCoreVisibility(exportToCore);
		return this->AppendFils(newData);
	}else{
		return alreadyExist;
	}
}


Element* Element::AppendPropertyBool(wxString propertyName,wxString propertyLabel,bool propertyDefaultValue,bool exportToCore)
{
	Element* alreadyExist=NULL;
	if(!IsPropertyExist(propertyName,&alreadyExist))
	{
		E_Data_Bool* nouvFils=new E_Data_Bool(this,propertyName,propertyLabel,propertyDefaultValue);
		this->AppendFils(nouvFils);
		nouvFils->SetXmlCoreVisibility(exportToCore);
		return nouvFils;
	}else{
		return alreadyExist;
	}
}


Element* Element::AppendPropertyFont(wxString propertyName,wxString propertyLabel,wxFont propertyDefaultValue)
{
	Element* alreadyExist=NULL;
	if(!IsPropertyExist(propertyName,&alreadyExist))
		return this->AppendFils(new E_Data_Font(this,propertyName,propertyLabel,propertyDefaultValue));
	else
		return alreadyExist;
}
