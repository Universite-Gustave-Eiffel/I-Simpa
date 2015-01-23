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
		this->AppendPropertyDecimal("aire","Surface area (m2)",0,true,2);
		this->elementInfo.userDestroyable=userCanDestroyThis;
		_("Surface area (m2)");
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
			leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_NEW_SURFACE_GROUP_FROM_SELECTION,_("Send to a new surface group"),"./Bitmaps/popup_new.png"));
			leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_INVERT_FACE_ORIENTATION, _("Invert face normal")));
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

