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

#include "e_scene_groupesurfaces_groupe.h"

#include <wx/log.h>
#include <algorithm>
#include "manager/stringTools.h"
#include "data_manager/appconfig.h"
#include "data_manager/grpInfo/data_group_info.h"
#include "data_manager/e_data_tree.h"
#include <wx/progdlg.h>
#include "last_cpp_include.hpp"


E_Scene_Groupesurfaces_Groupe::E_Scene_Groupesurfaces_Groupe( wxXmlNode* noeudCourant ,  Element* parent)
	:E_Drawable(parent,"Group",Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE,noeudCourant)
{
	SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACES_OPEN);
	SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACES_CLOSE);
	vertexFileIsLoaded=false;
	ignoreModification=false;
	_("Group");
	isPointerGroup=false;
	vertexInFile=true;
	if(noeudCourant!=NULL)
	{
		wxXmlNode* currentChild;
		currentChild = noeudCourant->GetChildren();
		// On va créer les fils de notre noeudCourant
		wxString propValue;
		if(noeudCourant->GetPropVal("pointer",&propValue))
			isPointerGroup=true;
		if(noeudCourant->GetPropVal("facesFile",&propValue))
			faceFileName=propValue; //Copie de l'adresse du fichier de faces
	}
	InitGroupProp();
	if(!isPointerGroup)
	{
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_SurfGroup++;
		ApplicationConfiguration::AppendRefElement(this,ApplicationConfiguration::ELEMENT_REF_TYPE_FACEGROUP);
		this->elementInfo.userDestroyable=true;
	}
	ignoreModification=true;
	ignoreModification=false;
}
E_Scene_Groupesurfaces_Groupe::E_Scene_Groupesurfaces_Groupe( Element* parent,wxString name,bool isPointer)
	:E_Drawable(parent,name,Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE)
{
	SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACES_OPEN);
	SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACES_CLOSE);
	vertexFileIsLoaded=true;
	ignoreModification=false;
	vertexInFile=true;
	isPointerGroup=isPointer;
	InitGroupProp();
	faceFileName=Convertor::ToString(this->elementInfo.xmlIdElement);
	wxString faceFileExt=".finfo";
	wxString faceFileNum="";
	int idfile=0;
	while(wxFileExists(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+faceFileName+faceFileNum+faceFileExt))
		faceFileNum="."+Convertor::ToString(idfile++);
	faceFileName+=faceFileNum+faceFileExt;
	if(!isPointerGroup)
	{
		this->elementInfo.userDestroyable=true;
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_SurfGroup++;
		ApplicationConfiguration::AppendRefElement(this,ApplicationConfiguration::ELEMENT_REF_TYPE_FACEGROUP);
		this->AppendPropertyEntier("idmat","idmateriau",0,true)->Hide();
	}
	this->Modified(this);
}
E_Scene_Groupesurfaces_Groupe::~E_Scene_Groupesurfaces_Groupe()
{
	if(!isPointerGroup)
	{
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_SurfGroup--;
		ApplicationConfiguration::DeleteRefElement(this,ApplicationConfiguration::ELEMENT_REF_TYPE_FACEGROUP);
	}
}

void E_Scene_Groupesurfaces_Groupe::InitGroupProp()
{
	if(!this->IsPropertyExist("aire"))
	{
		this->AppendPropertyDecimal("aire","Surface area (m²)",0,true,2);
		_("Surface area (m²)");
	}else{
		this->UpdateDecimalConfig("aire",0);
	}
}

void E_Scene_Groupesurfaces_Groupe::BeginDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree)
{
	treeEvent.Allow();
}
void E_Scene_Groupesurfaces_Groupe::sendStringToVector()
{
	vertexFileIsLoaded=true;
	//Lecture du fichier de faces
	using namespace formatGRPINFO;
	wxString fullPath=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+faceFileName;
	if(wxFileExists(fullPath))
	{
		this->vertexLst.clear();
		exchangeMainData mainData;
		DGIBIN fileReader;
		fileReader.ImportBIN(fullPath,mainData);
		for(long idface=0;idface<mainData.header.quantFace;idface++)
		{
			this->vertexLst.push_back(t_faceIndex(mainData.tabFaces[idface].face,mainData.tabFaces[idface].group));
		}
	}
}
bool E_Scene_Groupesurfaces_Groupe::mod_equal ( t_faceIndex elem1, t_faceIndex elem2 )
{
   return ((elem1.face == elem2.face) && (elem1.group == elem2.group));
}

bool E_Scene_Groupesurfaces_Groupe::mod_diff ( t_faceIndex elem1, t_faceIndex elem2 )
{
   if(elem1.group == elem2.group)
   {
		return (elem1.face < elem2.face);
   }else{
		return (elem1.group < elem2.group);
   }
}

void E_Scene_Groupesurfaces_Groupe::DeleteAllElementByTypeR(ELEMENT_TYPE typeElementToDelete)
{
	if(typeElementToDelete!=ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
		Element::DeleteAllElementByTypeR(typeElementToDelete);
	else
	{
		ClearGroup();
	}
}
void E_Scene_Groupesurfaces_Groupe::ClearGroup()
{
	//On efface les vertex
	wxString fullPath=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+faceFileName;
	if(wxFileExists(fullPath))
		wxRemoveFile(fullPath);
	if(!this->IsEmpty())
	{
		this->vertexLst.clear();
		E_Drawable::DeleteAllElementByType(ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX);
		if(this->treeCtrl!= NULL)
		{
			this->treeCtrl->SetItemHasChildren(this->elementInfo.idElement,false);
		}
		this->Modified(this);
	}
}
void E_Scene_Groupesurfaces_Groupe::DeleteDoublons()
{

	bool hided=false;
	if(!vertexInFile)
	{
		this->HideFaces();
		hided=true;
	}
	//std::sort(vertexLst.begin(),vertexLst.end(),mod_diff);
	vertexLst.sort(mod_diff);
	std::list<t_faceIndex>::iterator finIt=std::unique(vertexLst.begin(),vertexLst.end(),mod_equal);
	vertexLst.erase(finIt,vertexLst.end());
	if(hided)
		this->ShowFaces();

}

void E_Scene_Groupesurfaces_Groupe::InitProp()
{
	if(this->pere!=NULL)
	{
		if(this->pere->GetElementInfos().typeElement==ELEMENT_TYPE_SCENE_GROUPESURFACES)
		{
			ignoreModification=true;
			this->DeleteAllElementByType(ELEMENT_TYPE_TREE_LIST);
			Element* rootMateriaux=ApplicationConfiguration::GetRootScene()->GetElementByType(Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX);
			if(rootMateriaux)
			{
				Element* defaultEle=ApplicationConfiguration::GetMateriau(this->GetEntierConfig("idmat"));
				if(defaultEle)
					defaultEle=defaultEle->GetElementParent();
				std::list<Element::ELEMENT_TYPE> filterTree;
				filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_MATERIAUX);
				filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP);
				filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_GROUP);
				filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_MATERIAU);
				filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER);
				filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_GROUP);
				filterTree.push_back(ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_MATERIAU);

				this->AppendFils(new E_Data_Tree(this,"typesurface","Matériau",rootMateriaux,filterTree,defaultEle,false,1));
				this->SetDrawable();
			}
			ignoreModification=false;
		}
	}
}

void E_Scene_Groupesurfaces_Groupe::PushFace(std::vector<std::vector<ApplicationConfiguration::t_PropFace> >& vectorToFeed,t_faceIndex faceIndex)
{
	while(faceIndex.group+1>vectorToFeed.size())
	{
		std::vector<ApplicationConfiguration::t_PropFace> nvGroup;
		vectorToFeed.push_back(nvGroup);
	}
	while(faceIndex.face+1>vectorToFeed[faceIndex.group].size())
	{
		vectorToFeed[faceIndex.group].push_back(ApplicationConfiguration::t_PropFace());
	}
	//Il faut renseigner l'element de la face en fonction du type de groupe
	if(!isPointerGroup)
		vectorToFeed[faceIndex.group][faceIndex.face].idMaterial=this->GetEntierConfig("idmat");
	else
	{
		if(this->pere)
		{
			t_elementInfo parentInfos(this->pere->GetElementInfos());
			if(parentInfos.typeElement==ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR)
				vectorToFeed[faceIndex.group][faceIndex.face].idRecepteurSurfacique=parentInfos.xmlIdElement;
			else if(parentInfos.typeElement==ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT || parentInfos.typeElement==ELEMENT_TYPE_SCENE_VOLUMES_VOLUME)
				vectorToFeed[faceIndex.group][faceIndex.face].idEncombrement=parentInfos.xmlIdElement;
		}
	}
}

void E_Scene_Groupesurfaces_Groupe::GetMaterialsLink( std::vector<std::vector<ApplicationConfiguration::t_PropFace> > &matFacesAssociations)
{
	if(!isPointerGroup && !ApplicationConfiguration::IsIdMateriauExist(this->GetEntierConfig("idmat")))
		wxLogError(_("Material of the surface group %s has not be defined: the default material is applied"),this->elementInfo.libelleElement);
	if(vertexInFile)
	{
		if(!vertexFileIsLoaded)
		{
			sendStringToVector();
		}
		for(std::list<t_faceIndex>::iterator itface=this->vertexLst.begin();itface!=this->vertexLst.end();itface++)
			PushFace(matFacesAssociations,t_faceIndex((*itface).face,(*itface).group));
	}else{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
			{
				E_Scene_Groupesurfaces_Groupe_Vertex* vertex=dynamic_cast<E_Scene_Groupesurfaces_Groupe_Vertex*>((*itfils));
				PushFace(matFacesAssociations,t_faceIndex(vertex->GetFace(),vertex->GetGroup()));
			}
		}
	}
}

void E_Scene_Groupesurfaces_Groupe::GetFaces(std::vector<t_faceIndex>& vectorToFeed)
{
	if(vertexInFile)
	{
		if(!vertexFileIsLoaded)
		{
			sendStringToVector();
		}
		vectorToFeed.insert(vectorToFeed.begin(),this->vertexLst.begin(),this->vertexLst.end());
	}else{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
			{
				E_Scene_Groupesurfaces_Groupe_Vertex* vertex=dynamic_cast<E_Scene_Groupesurfaces_Groupe_Vertex*>((*itfils));
				vectorToFeed.push_back(t_faceIndex(vertex->GetFace(),vertex->GetGroup()));
			}
		}
	}
}
void E_Scene_Groupesurfaces_Groupe::GetFaces(std::list<t_faceIndex>& vectorToFeed)
{
	if(vertexInFile)
	{
		if(!vertexFileIsLoaded)
		{
			sendStringToVector();
		}
		vectorToFeed.insert(vectorToFeed.begin(),this->vertexLst.begin(),this->vertexLst.end());
	}else{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
			{
				E_Scene_Groupesurfaces_Groupe_Vertex* vertex=dynamic_cast<E_Scene_Groupesurfaces_Groupe_Vertex*>((*itfils));
				vectorToFeed.push_back(t_faceIndex(vertex->GetFace(),vertex->GetGroup()));
			}
		}
	}
}
wxXmlNode* E_Scene_Groupesurfaces_Groupe::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	//On place les faces dans notre liste, on ne les sauvegarde pas en tant que sous élements en XML



	wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
	thisNode->DeleteProperty("exp"); //ferme l'element pour le prochain chargement du projet
	thisNode->SetName("gr"); // Nom de la balise xml ( pas d'espace autorise )

	thisNode->DeleteProperty("facesFile");
	thisNode->AddProperty("facesFile",faceFileName);

	////////////////////////////
	// Sauvegarde des faces si chargé
	if(vertexFileIsLoaded)
	{
		std::vector<t_faceIndex> tmpFacetList;
		GetFaces(tmpFacetList);

		using namespace formatGRPINFO;
		wxString fullPath=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+faceFileName;
		exchangeMainData mainData;
		mainData.header.quantFace=tmpFacetList.size();
		mainData.tabFaces=new binface[mainData.header.quantFace];
		DGIBIN fileWriter;
		for(long i=0;i<mainData.header.quantFace;i++)
		{
			mainData.tabFaces[i].face=tmpFacetList[i].face;
			mainData.tabFaces[i].group=tmpFacetList[i].group;
		}
		fileWriter.ExportBIN(fullPath,mainData);
	}


	if(isPointerGroup)
	{
		thisNode->DeleteProperty("pointer");
		thisNode->AddProperty("pointer","1");
	}

	return thisNode;
}

void E_Scene_Groupesurfaces_Groupe::EndDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree,std::vector<Element*> dragLst)
{
	bool somethingModified=false;
	//On va receptionner des vertex, sinon on ne fait rien
	//Pour faire ceci on supprime l'ancien élément et on en créé un nouveau
	for(int i=0;i<dragLst.size();i++)
	{
		Element::t_elementInfo infosDragEl=dragLst[i]->GetElementInfos();
		if(infosDragEl.typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX && infosDragEl.idParent!=this->elementInfo.idElement)
		{
			E_Scene_Groupesurfaces_Groupe_Vertex* ansFace=dynamic_cast<E_Scene_Groupesurfaces_Groupe_Vertex*>(dragLst[i]);
			E_Scene_Groupesurfaces_Groupe* ansGroup=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(ansFace->GetElementParent());
			if(!(this->IsPointer()==false && ansGroup->IsPointer()==true)) //drag pointer groupe vers groupe annulé
			{
				this->AddFace(ansFace->GetFace(),ansFace->GetGroup());
				somethingModified=true;
				if(!isPointerGroup) //On ne supprime pas de l'element source si nous sommes un pointeur
					ansGroup->DeleteElementByTreeId(infosDragEl.idElement);
			}else{
				wxLogInfo(_("The drag and drop of vertex is allowed only from surface groups"));
			}
		}else if(infosDragEl.typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE && infosDragEl.idElement!=this->elementInfo.idElement)
		{
			//Drag&Drop de groupes
			E_Scene_Groupesurfaces_Groupe* draggingGroup=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(dragLst[i]);
			if(!(this->IsPointer()==false && draggingGroup->IsPointer()==true))
			{
				if(!vertexFileIsLoaded)
				{
					sendStringToVector();
				}
				if(!this->vertexInFile)
				{ //Si les éléments de this sont affichés
					std::vector<t_faceIndex> facesToAdd;
					draggingGroup->GetFaces(facesToAdd);
					for(long i=0;i<facesToAdd.size();i++)
						this->AddFace(facesToAdd[i].face,facesToAdd[i].group);
				}else{
					draggingGroup->GetFaces(vertexLst);
				}
				somethingModified=true;
				if(!isPointerGroup) //Glissé déposé d'un groupe de surface vers un autre, appartenant tout deux aux ensembles de surfaces définissant les matériaux
				{ //On efface les faces de l'ancien groupe car ces faces sont maintenant dans this
					draggingGroup->ClearGroup();
				}
			}else{
				wxLogInfo(_("The drag and drop of a group is allowed only from main surface groups"));
			}

		}
	}
	if(somethingModified)
	{
		this->Modified(this);
		this->UpdateDecimalConfig("aire",0);
		this->FillWxTree();
	}
	DeleteDoublons();
}
bool E_Scene_Groupesurfaces_Groupe::IsContainsProperties()
{
	//Si un groupe standard on active le menu propriété (car le groupe standard possède la propriété type de surface)
	//On est obligé de faire cette opération car cette propriété est généré seulement avant l'affichage de celle-ci
	if(this->pere!=NULL)
		if(this->pere->GetElementInfos().typeElement==ELEMENT_TYPE_SCENE_GROUPESURFACES)
			return true;
	return Element::IsContainsProperties();
}

void E_Scene_Groupesurfaces_Groupe::OnRightClic(wxMenu* leMenu)
{



	if(isPointerGroup)
	{
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_EMPTY_POINTER_VERTEX_GROUP, _("Clean")));
		leMenu->AppendSeparator();
	}
	leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_INVERT_FACE_ORIENTATION, _("Invert face normal")));
	Element::OnRightClic(leMenu);
}

bool E_Scene_Groupesurfaces_Groupe::OnElementRemoved()
{
	if(E_Drawable::OnElementRemoved())
	{
		this->ClearGroup();
		return true;
	}else{
		return false;
	}
}

//Ajout d'une face
void E_Scene_Groupesurfaces_Groupe::AddFace(long face,long group,bool updateTreeCtrl)
{
	if(vertexInFile)
	{
		if(!vertexFileIsLoaded)
			sendStringToVector();
		this->vertexLst.push_back(t_faceIndex(face,group));
		if(updateTreeCtrl)
			this->Modified(this);
	}else{
		Element* newEl=this->AppendFils(new E_Scene_Groupesurfaces_Groupe_Vertex(this,face,group,this->isPointerGroup));
		if(updateTreeCtrl && this->treeCtrl)
		{
			newEl->FillWxTree(this->treeCtrl);
			//Ajout de l'élément dans le tableau de recherche optimisé des éléments
			faceIndexToElementTreeId.insert(mapPairInsert_t(t_faceIndex(face,group).GetHashValue(),mapValue_t(t_faceIndex(face,group),newEl->GetElementInfos().idElement)));
		}
	}
}
//Ajout de plusieurs face
void E_Scene_Groupesurfaces_Groupe::AddFaces(const std::list<t_faceIndex>& faceList)
{

	wxProgressDialog* progDialog(NULL);
	std::size_t faceCount(faceList.size());
	if(faceCount>500)
		progDialog=new wxProgressDialog(_("Loading faces"),_("Loading faces of 3D scene"),100,NULL, wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
	this->ignoreModification=true;
	std::size_t i(0);
	wxDateTime lastUpdate(wxDateTime::UNow());
	for(std::list<t_faceIndex>::const_iterator itface=faceList.begin();itface!=faceList.end();itface++)
	{
		AddFace(itface->face,itface->group,false);
		if(progDialog)
		{
			if(wxDateTime::UNow()-lastUpdate>1)
			{
				int newperc=int((float(i)/float(faceCount))*100);
				progDialog->Update(newperc,wxString::Format(_("Loading face %i/%i"),i,faceCount));
				lastUpdate=wxDateTime::UNow();
			}
		}
		i++;
	}
	this->ignoreModification=false;
	this->Modified(this);
	if(this->treeCtrl)
		this->FillWxTree(this->treeCtrl);
	//Mise à jour du hashmap

	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
		{
			E_Scene_Groupesurfaces_Groupe_Vertex* vertex=dynamic_cast<E_Scene_Groupesurfaces_Groupe_Vertex*>((*itfils));
			faceIndexToElementTreeId.insert(mapPairInsert_t(t_faceIndex(vertex->GetFace(),vertex->GetGroup()).GetHashValue(),mapValue_t(t_faceIndex(vertex->GetFace(),vertex->GetGroup()),vertex->GetElementInfos().idElement)));
		}
	}


	if(progDialog)
		delete progDialog;
}
void E_Scene_Groupesurfaces_Groupe::OnExpanding(wxTreeEvent& evt)
{
	this->ShowFaces();
	this->elementInfo.expanded=false;
	this->FillWxTree();
	Element::OnExpanding(evt);
}

void E_Scene_Groupesurfaces_Groupe::OnCollapsing(wxTreeEvent& evt)
{
	this->HideFaces();
	Element::OnCollapsing(evt);
}
void E_Scene_Groupesurfaces_Groupe::ShowFaces( )
{
	if(vertexInFile)
	{
		ignoreModification=true;
		vertexInFile=false;
		if(!vertexFileIsLoaded)
			sendStringToVector();
		//Liste->Element
    	AddFaces(this->vertexLst);
		this->vertexLst.clear();
		ignoreModification=false;
	}
}

bool E_Scene_Groupesurfaces_Groupe::IsEmpty()
{
	if(vertexInFile)
	{
		if(vertexFileIsLoaded && this->vertexLst.empty())
		{
			return true;
		}else{
			return false;
		}
	}else{
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
			{
				return false;
			}
		}
		return true;
	}

}
bool E_Scene_Groupesurfaces_Groupe::HasChildren( )
{
	return !IsEmpty();
}
wxTreeItemId E_Scene_Groupesurfaces_Groupe::GetIdElementByFaceAndGroup(long face,long group)
{
	wxTreeItemId emptyId;
	if(vertexInFile)
	{ //si le groupe n'est pas edité et que la face se trouve dans notre groupe on sélectionne le groupe
		if(!vertexFileIsLoaded)
			sendStringToVector();
		for(std::list<t_faceIndex>::iterator itface=this->vertexLst.begin();itface!=this->vertexLst.end();itface++)
		{
			if((*itface).face==face && (*itface).group==group)
			{
				//L'élement fait parti de notre groupe
				//il faut le déplier puis retourner l'indice de l'élément recherché
				this->ShowFaces();
				break;
			}
		}
	}
	if(!vertexInFile)
	{
		t_faceIndex searchIndex(face,group);
		std::size_t hashFace(searchIndex.GetHashValue());
		faceIndexMapType::iterator faceIt(faceIndexToElementTreeId.find(hashFace));
		while(faceIt!=faceIndexToElementTreeId.end() && faceIt->first==hashFace)
		{

			if(searchIndex==faceIt->second.first)
			{
				return faceIt->second.second;
			}
			faceIt++;
		}
	}
	return emptyId;
}

void E_Scene_Groupesurfaces_Groupe::HideFaces()
{
	//Element->Liste
	if(!vertexInFile)
	{
		ignoreModification=true;
		vertexInFile=true;
		this->vertexLst.clear();
		this->faceIndexToElementTreeId.clear();
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
			{
				E_Scene_Groupesurfaces_Groupe_Vertex* vertex=dynamic_cast<E_Scene_Groupesurfaces_Groupe_Vertex*>((*itfils));
				this->AddFace(vertex->GetFace(),vertex->GetGroup());
			}
		}
		this->DeleteAllElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX);
	}
	ignoreModification=false;
}


bool E_Scene_Groupesurfaces_Groupe::IsPointer()
{
	return isPointerGroup;
}


void E_Scene_Groupesurfaces_Groupe::OnBeginLabelEdit(wxTreeEvent& treeEvent)
{
	if(isPointerGroup)
		Element::OnBeginLabelEdit(treeEvent);
	//On autorise l'edition en surchargeant l'événement
}

void E_Scene_Groupesurfaces_Groupe::OnEndLabelEdit(wxTreeEvent& treeEvent)
{
	if(treeEvent.GetLabel()!="")
	{
		this->elementInfo.libelleElement=treeEvent.GetLabel();
		this->Modified(this); //Indique le l'element a été modifié
	}
}

void E_Scene_Groupesurfaces_Groupe::Modified(Element* eModif)
{
	if(!ignoreModification)
	{
		//Mise à jour de l'identifiant de typedesurface
		t_elementInfo elInfo=eModif->GetElementInfos();
		//Si l'utilisateur a choisi un autre matériau
		if(elInfo.typeElement==ELEMENT_TYPE_TREE_LIST && elInfo.libelleElement=="typesurface")
		{
			E_Data_Tree* ElementTree=dynamic_cast<E_Data_Tree*>(eModif);
			if(ElementTree)
			{
				this->UpdateEntierConfig("idmat",ApplicationConfiguration::GetMateriauId(ElementTree->GetChoice()));
			}
		}

		if(treeCtrl!=NULL ) // si à la suite de la modification ce groupe ne contient plus d'enfants alors on indique à l'arbre que l'element est vide
		{
			bool vide=this->IsEmpty();
			treeCtrl->SetItemHasChildren(this->elementInfo.idElement,!vide);
		}

		if(!(elInfo.typeElement==ELEMENT_TYPE_FLOAT && elInfo.libelleElement=="aire")) //On ignore l'ajout ou la mise à jour de la propriété d'aire
			Element::Modified(eModif);

	}
}


void E_Scene_Groupesurfaces_Groupe::DrawTriangles(vec4 unitizeValue,std::vector<t_el_draw_object> &drawObject)
{

	if(!isPointerGroup && ApplicationConfiguration::GLOBAL_VAR.drawMaterialColors)
	{
		vec3 couleurMat;
		/////////////////////////////////
		// Récupération du matériau correspondant
		E_Materiau* defaultEle=ApplicationConfiguration::GetMateriau(this->GetEntierConfig("idmat"));
		if(defaultEle)
		{
			Element* renduEle=defaultEle->GetElementParent()->GetElementByType(Element::ELEMENT_TYPE_SCENE_BDD_MATERIAUX_MATERIAU_RENDER);
			if(renduEle)
				couleurMat=renduEle->GetColorConfig("mat_color")/255;
			std::vector<t_faceIndex> vectorToFeed;
			this->GetFaces(vectorToFeed);
			if(vectorToFeed.size()>0)
			{
				drawObject.push_back(t_el_draw_object());
				t_el_draw_object* nvEl=&drawObject[drawObject.size()-1];
				nvEl->colours.colorA=vec4(couleurMat,1);
				nvEl->colours.colorD=vec4(couleurMat*.75,1);
				nvEl->colours.colorS=vec4(couleurMat,1);
				for(int idface=0;idface<vectorToFeed.size();idface++)
				{
					nvEl->Scenetriangles.push_back(vectorToFeed[idface]);
				}
			}
		}
	}
}
