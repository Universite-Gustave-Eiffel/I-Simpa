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

#include "uiTreeCtrl.h"
#include "data_manager/python_interface/instanceManager.hpp"
#include <wx/log.h>
#include "data_manager/appconfig.h"
#include "last_cpp_include.hpp"

BEGIN_EVENT_TABLE(uiTreeCtrl, wxTreeCtrl)
	EVT_TREE_BEGIN_DRAG(-1, uiTreeCtrl::BeginDrag)
	EVT_TREE_END_DRAG(-1, uiTreeCtrl::EndDrag)
	EVT_LEFT_DOWN(uiTreeCtrl::OnLeftClic)
	EVT_TREE_SEL_CHANGED(-1, uiTreeCtrl::OnSelectTreeItem)
	EVT_TREE_ITEM_ACTIVATED(-1, uiTreeCtrl::OnDoubleClic)
	EVT_TREE_ITEM_RIGHT_CLICK(-1, uiTreeCtrl::OnRightClic)
	EVT_TREE_BEGIN_LABEL_EDIT(-1, uiTreeCtrl::OnBeginLabelEdit)
	EVT_TREE_END_LABEL_EDIT(-1, uiTreeCtrl::OnEndLabelEdit)
	EVT_TREE_ITEM_EXPANDING(-1, uiTreeCtrl::OnExpanding)
	EVT_TREE_ITEM_COLLAPSED(-1, uiTreeCtrl::OnCollapsing)
	EVT_MENU(-1, uiTreeCtrl::OnElementEvent)
    EVT_TIMER(100, uiTreeCtrl::OnTimer)
	EVT_MOTION(uiTreeCtrl::OnMouseMove)
	EVT_KEY_UP(uiTreeCtrl::OnKeyUp)
END_EVENT_TABLE()

bool RunPyManagedMenu(const int& elementType,const int& elementId, wxMenu& menu);

class ElementLinkWithTree : public wxTreeItemData
{
public:
	ElementLinkWithTree()
		:wxTreeItemData()
	{
		link=NULL;
	}
	ElementLinkWithTree(Element* ellink)
		:wxTreeItemData()
	{
		link=ellink;
	}
	void SetElement(Element* ellink)
	{
		link=ellink;
	}
	Element* GetElement()
	{
		return link;
	}
protected:
	Element* link;
};
/**
 * Cette structure nous permet de lier l'existence des deux objets entre eux
 * Ainsi l'élément ne sera plus visible dans l'arbre si il a été supprimé
 * Et s'il l'a déjà été, on ne cherchera pas à le supprimer
 */
struct UiTreeWitness : public t_outsideOperator
{
	UiTreeWitness(uiTreeCtrl* _treectrl,smart_ptr<bool>& _alive,const wxTreeItemId& _idel)
		:t_outsideOperator(),treectrl(_treectrl),idel(_idel),alive(_alive)
	{
	}
	virtual ~UiTreeWitness()
	{
		if(*alive)
		{
			if(treectrl->GetRootItem()!=idel)
				treectrl->Delete(idel);
		}
	}
	uiTreeCtrl* treectrl;
	const wxTreeItemId idel;
	smart_ptr<bool> alive;
};

void uiTreeCtrl::OnKeyUp(wxKeyEvent& evt)
{
/*
	//Déclaration des raccourcis pour la grille de données ( propriété d'un élément et les feuilles de données)
	wxAcceleratorEntry entries[3];
	entries[0].Set(wxACCEL_CTRL, (int) 'C',     Element::IDEVENT_COPIER);
	entries[1].Set(wxACCEL_CTRL,  (int) 'V',    Element::IDEVENT_COLLER);
	entries[2].Set(wxACCEL_NORMAL,  WXK_DELETE, Element::IDEVENT_DELETE_ELEMENT);
	wxAcceleratorTable accel(3, entries);
	this->SetAcceleratorTable(accel);
	*/
wxCommandEvent evtMenu;
	evtMenu.SetEventType(-1);
	int keycod=evt.GetKeyCode();
	if(evt.ControlDown() && keycod == (int)'C')
		evtMenu.SetId(Element::IDEVENT_COPIER);
	else if(evt.ControlDown() && keycod == (int)'V')
		evtMenu.SetId(Element::IDEVENT_COLLER);
	else if(keycod == WXK_DELETE )
		evtMenu.SetId(Element::IDEVENT_DELETE_ELEMENT);
	else if(keycod == WXK_F2 )
		evtMenu.SetId(Element::IDEVENT_RENAME_ELEMENT);
	else
	{
		evt.Skip();
		return;
	}
	OnElementEvent(evtMenu);
	evt.Skip();
}

void uiTreeCtrl::InitTree()
{

	wxString ressourceFolder=ApplicationConfiguration::CONST_RESOURCE_FOLDER;
	wxImageList *imglist = new wxImageList(16, 16, true, 2);

	for(std::size_t graph_id=0;graph_id<Element::GRAPH_LAST_STATIC_GRAPH;graph_id++)
	{

		switch(graph_id)
		{
			case Element::GRAPH_FOLDER_OPEN:
			case Element::GRAPH_DISK_FOLDER_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_disk_folder_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_FOLDER:
			case Element::GRAPH_DISK_FOLDER_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_disk_folder_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_ITEM:
			case Element::GRAPH_DISK_DEFAULT_FILE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_disk_default_file.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_FITTINGS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_fittings_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_FITTINGS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_fittings_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_FITTING_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_fitting_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_FITTING_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_fitting_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PUNCTUAL_RECEIVERS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_punctual_receivers_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PUNCTUAL_RECEIVERS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_punctual_receivers_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SURFACE_RECEIVERS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_surface_receivers_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SURFACE_RECEIVERS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_surface_receivers_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SOUND_SOURCES_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_sound_sources_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SOUND_SOURCES_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_sound_sources_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SURFACES_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_surfaces_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SURFACES_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_surfaces_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_VOLUMES_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_volumes_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_VOLUMES_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_volumes_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_VOLUME_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_volume_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_VOLUME_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_volume_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PROJECT_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_project_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PROJECT_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_project_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_DATA_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_data_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_DATA_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_data_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_MATERIALS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_user_materials_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_MATERIALS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_user_materials_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_APPLICATION_MATERIALS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_application_materials_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_APPLICATION_MATERIALS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_application_materials_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_DATABASE_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_database_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_DATABASE_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_database_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_SPECTRUMS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_user_spectrums_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_SPECTRUMS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_user_spectrums_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_APPLICATION_SPECTRUMS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_application_spectrums_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_APPLICATION_SPECTRUMS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_application_spectrums_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_MATERIAL_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_material_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_MATERIAL_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_material_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_STANDARTCORE_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_standartcore_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_STANDARTCORE_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_standartcore_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_CORES_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_cores_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_CORES_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_cores_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SPPSCORE_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_sppscore_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SPPSCORE_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_sppscore_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_ENVIRONMENT:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_environment.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PROJECT_AUTHOR:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_project_author.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_INFORMATION:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_information.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_DISK_GABE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_disk_gabe.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_DISK_RS:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_disk_rs.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_DISK_PARTICLE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_disk_particle.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_EL_CONFIGURATION:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_el_configuration.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_EL_3D_DISPLAY:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_el_3d_display.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_EL_POSITION:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_el_position.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_EL_TRIANGLE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_el_triangle.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SPECTRUM:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_spectrum.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_ORIGIN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_origin.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_TETMESH_PARAMETERS:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_tetmesh_parameters.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_RENDERING_FOLDER_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_rendering_folder_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_RENDERING_FOLDER_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_rendering_folder_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_ROOT_MATERIALS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_root_materials_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_ROOT_MATERIALS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_root_materials_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_ROOT_SPECTRUMS_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_root_spectrums_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_ROOT_SPECTRUMS_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_root_spectrums_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SOUND_SOURCE_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_sound_source_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SOUND_SOURCE_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_sound_source_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PUNCTUAL_RECEIVER_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_punctual_receiver_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PUNCTUAL_RECEIVER_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_punctual_receiver_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SURFACE_RECEIVER_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_surface_receiver_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_SURFACE_RECEIVER_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_surface_receiver_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PREF_ANIMATION:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_pref_animation.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PREF_GENERAL:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_pref_general.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PREF_LEGEND:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_pref_legend.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PREF_NOISE_MAP:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_pref_noise_map.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_PREF_PARTICLES:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_pref_particles.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_PREF_ROOT_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_user_pref_root_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_PREF_ROOT_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_user_pref_root_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_PREF_MAINCONFIGURATION_OPEN:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_el_mainconfiguration_open.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_PREF_MAINCONFIGURATION_CLOSE:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_el_mainconfiguration_close.png", wxBITMAP_TYPE_PNG));
				break;
			case Element::GRAPH_USER_PREF_HISTORY:
				imglist->Add(wxBitmap(ressourceFolder+"/Bitmaps/tree_el_history.png", wxBITMAP_TYPE_PNG));
				break;
		}
	}


	AssignImageList(imglist);
	this->pointeurElementEvent=NULL;
	selectionEventState=true;
	highlightEl.compteurHl=0;

}
uiTreeCtrl::uiTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
:wxTreeCtrl(parent, id, pos, size, style),alive(new bool(true))
{
	InitTree();
}
uiTreeCtrl::uiTreeCtrl()
:wxTreeCtrl(),alive(new bool(true))
{
	InitTree();
}

uiTreeCtrl::~uiTreeCtrl()
{
	*alive=false;
}
void uiTreeCtrl::OnMouseMove(wxMouseEvent& event)
{
	if(m_Timer.IsRunning())
	{
		mousePos=event.GetPosition();
	}
}
void uiTreeCtrl::OnTimer( wxTimerEvent& event)
{
	if(DragLst.size()>0)
	{
		//En cours de drag&drop
		//On recupere l'element en cours de sélection
		wxTreeItemId currentSelection=this->HitTest(mousePos);
		if(currentSelection.IsOk())
		{
			if(currentSelection==highlightEl.el)
			{
				highlightEl.compteurHl+=.1f;
				if(highlightEl.compteurHl>=1)
				{
					if(!this->IsExpanded(currentSelection))
					{
						this->Expand(currentSelection);
						highlightEl.compteurHl=0;
					}
				}
			}else{
				highlightEl.el=currentSelection;
				highlightEl.compteurHl=0;
				// Permet d'effacer le bug graphique lors d'un drag&drop, suivie par un scoll vertical
				if(this->GetScrollPos(wxVERTICAL)!=highlightEl.scrollPos)
				{
					this->Refresh(true);
					highlightEl.scrollPos=this->GetScrollPos(wxVERTICAL);
				}
			}
		}
		m_Timer.Start(100,true);
	}
	if(wxGetKeyState(WXK_ESCAPE))
	{
		DragLst.clear();
		this->Refresh();
	}
}
void uiTreeCtrl::OnExpanding(wxTreeEvent& treeEvent)
{
	Element* elementDest;

	//On récupère lidentifiant de l'element correspondant dans l'événement
	wxTreeItemId idElement=treeEvent.GetItem();
	//On extrait l'élément de la racine
	if(idElement.IsOk())
	{
		elementDest=GetElement(idElement);
		if(elementDest!=NULL) //si on a bien récupéré l'element
		{
			//Maintenant on appel l'événement
			elementDest->OnExpanding(treeEvent);
		}
	}
}


void uiTreeCtrl::OnCollapsing(wxTreeEvent& treeEvent)
{
	Element* elementDest;

	//On récupère lidentifiant de l'element correspondant dans l'événement
	wxTreeItemId idElement=treeEvent.GetItem();
	//On extrait l'élément de la racine
	if(idElement.IsOk())
	{
		elementDest=GetElement(idElement);
		if(elementDest!=NULL) //si on a bien récupéré l'element
		{
			//Maintenant on appel l'événement
			elementDest->OnCollapsing(treeEvent);
		}
	}
}

void uiTreeCtrl::OnElementEvent(wxCommandEvent& eventElement)
{
	if(this->pointeurElementEvent)
		this->pointeurElementEvent(eventElement);
}

void uiTreeCtrl::BindElementEvent(void (*fevent)(wxCommandEvent&))
{
	this->pointeurElementEvent=fevent;
}


void uiTreeCtrl::Init(Element* rootElement)
{
	this->DeleteAllItems();
	this->rootItem=rootElement;
	rootElement->FillWxTree(this); //on complète

}

void uiTreeCtrl::Delete(const wxTreeItemId &item)
{
	LockElementEvent();
	wxTreeCtrl::Delete(item);
	LockElementEvent(false);

}

void uiTreeCtrl::AddElement(Element* newElement)
{
	//Ajout d'un élément dans l'arbre (appelé de facon hierarchique par element.cpp)
	const Element::t_elementInfo& elementInfo = newElement->GetElementInfos();

	wxTreeItemId idElement;

	if(!elementInfo.idParent.IsOk() && (this->GetWindowStyle() & wxTR_HIDE_ROOT) != 0)
		idElement=this->AddRoot("root");
	else
		idElement=this->AppendItem(elementInfo.idParent,_(newElement->GetTreeLabel()),elementInfo.graphElement,-1,new ElementLinkWithTree(newElement));

	this->SetItemImage(idElement,elementInfo.graphElementStateOpen,wxTreeItemIcon_Expanded);
	this->SetItemImage(idElement,elementInfo.graphElement,wxTreeItemIcon_Normal);


	newElement->SetTreeItemId(idElement); //on affecte à l'element sont indice dans cet arbre
	if(newElement->HasChildren()) //Est-ce que l'élément veut forcer l'arbre à reconnaître qu'il y a des enfants sur cet élément ?
		this->SetItemHasChildren(idElement);
	newElement->AppendWitness(LifeTimeWitness(new UiTreeWitness(this,this->alive,idElement)));
}

void uiTreeCtrl::OnDoubleClic(wxTreeEvent& treeEvent)
{
	Element* elementDest;
	treeEvent.Skip();
	//On récupère lidentifiant de l'element correspondant dans l'événement
	wxTreeItemId idElement=treeEvent.GetItem();
	//On extrait l'élément du map
	elementDest=GetElement(idElement);
	if(elementDest!=NULL) //si on a bien récupéré l'element
	{
		wxCommandEvent cmdEv;
		cmdEv.SetId(Element::IDEVENT_GETPROPERTIES);
		this->OnElementEvent(cmdEv);
	}
}

void uiTreeCtrl::OnEndLabelEdit(wxTreeEvent& treeEvent)
{
	Element* elementDest;

	//On récupère lidentifiant de l'element correspondant dans l'événement
	wxTreeItemId idElement=treeEvent.GetItem();
	//On extrait l'élément du map
	elementDest=GetElement(idElement);
	if(elementDest!=NULL) //si on a bien récupéré l'element
	{
		//Maintenant on appel l'événement
		wxMenu menuClicDroit;
		elementDest->OnEndLabelEdit(treeEvent);
	}
}

wxTreeItemId uiTreeCtrl::GetSelection() const
{
	wxTreeItemId lastSelection;
	wxArrayTreeItemIds selection;
	this->GetSelections(selection);
	if(selection.size()>0)
	{
		return selection[0];
	}else{
		return lastSelection;
	}
}

void uiTreeCtrl::OnBeginLabelEdit(wxTreeEvent& treeEvent)
{
	using namespace std;
	Element* elementDest;

	//On récupère lidentifiant de l'element correspondant dans l'événement
	wxTreeItemId idElement=treeEvent.GetItem();
	//On extrait l'élément du map
	elementDest=GetElement(idElement);
	if(elementDest!=NULL) //si on a bien récupéré l'element
	{
		//Maintenant on appel l'événement
		wxMenu menuClicDroit;
		elementDest->OnBeginLabelEdit(treeEvent);
	}

}


#ifdef USE_PYTHON
Element::ELEMENT_TYPE GetPythonExtBaseType( Element* pythonEl);
#endif

void uiTreeCtrl::OnRightClic(wxTreeEvent& treeEvent)
{
	using namespace std;
	Element* elementDest;

	//On récupère lidentifiant de l'element correspondant dans l'événement
	wxTreeItemId idElement= treeEvent.GetItem();
	//On extrait l'élément du map
	if(idElement.IsOk())
	{
		elementDest=GetElement(idElement);
		if(elementDest!=NULL) //si on a bien récupéré l'element
		{
			Element::t_elementInfo elinfos=elementDest->GetElementInfos();
			//this->UnselectAll();
			this->SelectItem(idElement, true);
			//Maintenant on appel l'événement
			wxMenu menuClicDroit;
			elementDest->OnRightClic(&menuClicDroit);


			#ifdef USE_PYTHON
			//Gestion des fonctions supplémentaires
			//implémenté en python
			if(elinfos.typeElement==Element::ELEMENT_TYPE_PYTHON_EXTENSION)
			{
				elinfos.typeElement=GetPythonExtBaseType(elementDest);
			}
			RunPyManagedMenu((int)(elinfos.typeElement),elinfos.xmlIdElement,menuClicDroit);
			#endif
			//affichage du menu
			this->PopupMenu(&menuClicDroit, treeEvent.GetPoint());
		}
	}
}

void uiTreeCtrl::OnLeftClic(wxMouseEvent& evt)
{

	wxPoint coordItem=evt.GetPosition();
	wxTreeItemId itId=this->HitTest(coordItem);
	if(itId.IsOk())
	{
		wxTreeEvent treeev(wxEVT_COMMAND_TREE_SEL_CHANGED,this,itId);
		this->OnSelectTreeItem(treeev);
	}
	evt.Skip(true);
}

void uiTreeCtrl::OnSelectTreeItem(wxTreeEvent& treeEvent)
{
	if(this->selectionEventState)
	{
		//On récupère lidentifiant de l'element correspondant dans l'événement
		wxTreeItemId idElement= treeEvent.GetItem();
		if(idElement.IsOk())
		{
			Element* linkedEl=this->GetElement(idElement);
			if(linkedEl)
			{
				linkedEl->OnSelectTreeItem(treeEvent);	//Evenement sur l'élément
				treeEvent.SetId(Element::IDEVENT_SELECT_TREE_ITEM);			//Evenement sur le projet
				this->OnElementEvent(treeEvent);
			}
		}
	}
}

void uiTreeCtrl::BeginDrag(wxTreeEvent& treeEvent)
{
	using namespace std;
	Element* elementDest;
	wxArrayTreeItemIds userDrag;
	this->GetSelections(userDrag);
	this->UnselectAll();
	DragLst.clear();
	//On récupère lidentifiant de l'element correspondant dans l'événement
	for(int i=0;i<userDrag.size();i++)
	{
		wxTreeItemId idElement=userDrag[i];
		//On extrait l'élément du map
		if(idElement.IsOk())
		{
			elementDest=GetElement(idElement);
			if(elementDest!=NULL) //si on a bien récupéré l'element
			{
				elementDest->BeginDrag(treeEvent,this);
				if(treeEvent.IsAllowed())
					DragLst.push_back(elementDest);
				else
					break;
			}
		}
	}
	if(!treeEvent.IsAllowed())
		DragLst.clear();
	else
	{
		m_Timer.SetOwner(this,100);
		m_Timer.Start(100,true); //permet d'etandre les dossier si drag d'un objet sur un dossier fermé)
	}
}

void uiTreeCtrl::EndDrag(wxTreeEvent& treeEvent)
{
	wxTreeItemId idElement=treeEvent.GetItem();
	Element* elementDest;
	if(idElement.IsOk())
	{
		elementDest=GetElement(idElement);
		if(elementDest!=NULL) //si on a bien récupéré l'element
		{
			if(DragLst.size()>0)
			{
				elementDest->EndDrag(treeEvent,this,DragLst);
				if(!treeEvent.IsAllowed())
					wxBell();
			}
		}
	}
	DragLst.clear();
	this->Refresh(); //evite les bug graphiques du drag & drop
}

wxInt32 uiTreeCtrl::GetXmlId(const wxTreeItemId& itemId)
{
	Element* itemDat=GetElement(itemId);
	if(itemDat)
	{
		return itemDat->GetXmlId();
	}else{
		return -1;
	}
}


Element* uiTreeCtrl::GetElement(const wxTreeItemId& itemId)
{
	/*
	void* ptVoid;
	if(	InstanceManager::GetReference(OBJECT_REF_TYPE_ELEMENT,GetXmlId(itemId),&ptVoid))
	{
		return (Element*)ptVoid;
	}else{
		wxLogDebug("Référence introuvable uiTreeCtrl::GetElement");
		return NULL;
	}
	*/

	wxTreeItemData* itemdata=this->GetItemData(itemId);
	ElementLinkWithTree* itemDat=dynamic_cast<ElementLinkWithTree*>(itemdata);
	if(itemDat)
	{
		return itemDat->GetElement();
	}else{
		return NULL;
	}
}
