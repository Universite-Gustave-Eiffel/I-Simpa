#include "first_header_include.hpp"

#include "data_manager/element.h"
#ifndef __ELEMENT_VERTEX__
#define __ELEMENT_VERTEX__

/** \file e_scene_groupesurfaces_groupe_vertex.h
   \brief Cet élément représente une face du modèle
*/

/**
   \brief Cet élément représente une face du modèle
*/

class E_Scene_Groupesurfaces_Groupe_Vertex: public Element
{
private:
    long face;
	long group;
public:

	E_Scene_Groupesurfaces_Groupe_Vertex( Element* parent, long _face, long _group, bool userCanDestroyThis)
		:Element(parent,"Face",Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_TRIANGLE);
		_("Face");
		face=_face;
		group=_group;
		wxString nom;
		nom<<_group<<" "<<_face;
		this->elementInfo.libelleElement=nom;
		this->AppendPropertyDecimal("aire","Aire de la surface (m²)",0,true,2);
		this->elementInfo.userDestroyable=userCanDestroyThis;
	}

	/**
	 * Retourne l'indice de la face
	 * @return numéro de face de la scène ( n° dans le groupe)
	 */
	long GetFace()
	{
		return face;
	}
	void OnRightClic(wxMenu* leMenu)
	{
		if(this->pere && this->pere->GetElementInfos().userDestroyable)
		{
			leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_SURFACE_GROUP_FROM_SELECTION,_("Envoyer vers un nouveau groupe"),"./Bitmaps/popup_new.png"));
			leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_INVERT_FACE_ORIENTATION, _("Inverser l'orientation des faces")));
		}
		Element::OnRightClic(leMenu);
	}
	/*

	*/
	/**
	 * Retourne l'indice du groupe
	 * @return numéro du groupe
	 */
	long GetGroup()
	{
		return group;
	}
	
	void BeginDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree)
	{
		treeEvent.Allow();
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		//Cet élément pas sauvegardé en tant qu'element dans le document xml ( trop de faces dans un modèle )
		return NoeudParent;
	}

};

#endif

