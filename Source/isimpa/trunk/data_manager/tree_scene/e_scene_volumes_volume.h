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

#include "e_scene_groupesurfaces_groupe.h"
#include "data_manager/drawable_element.h"
#include "e_scene_volumes_volume_proprietes.h"
#include "e_scene_volumes_volume_rendu.h"


/** \file e_scene_volumes_volume.h
   \brief Cet élément est un volume composant le modèle
*/

/**
 *  \brief Cet élément est un volume composant le modèle
 */
class E_Scene_Volumes_Volume: public E_Drawable
{

private:

	bool IsVertexGroupEmpty()
	{
		Element* targ=this->GetElementByType(ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE);
		if(targ)
		{
			E_Scene_Groupesurfaces_Groupe* grpE=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(targ);
			return grpE->IsEmpty();
		}
		return true;
	}
public:
	E_Scene_Volumes_Volume( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Drawable(parent,"Volume",Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME,noeudCourant)
		{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_VOLUME_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_VOLUME_CLOSE);
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Volumes++;
		this->elementInfo.userDestroyable=true;
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
					if(typeEle==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE)
					{
						this->AppendFils(new E_Scene_Groupesurfaces_Groupe(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_RENDU)
					{
						this->AppendFils(new E_Scene_Volumes_Volume_Rendu(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_PROPRIETES)
					{
						this->AppendFils(new E_Scene_Volumes_Volume_Proprietes(currentChild,this));
					}
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Volumes_Volume( Element* parent,const wxString& nom=wxString::Format(_("Volume %i"),ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Volumes+1))
		:E_Drawable(parent,nom,Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_VOLUME_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_VOLUME_CLOSE);
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Volumes++;
		this->elementInfo.userDestroyable=true;
		_("Volume boundary");
		this->AppendFils(new E_Scene_Volumes_Volume_Proprietes(this));
		this->AppendFils(new E_Scene_Volumes_Volume_Rendu(this));
		this->AppendFils(new E_Scene_Groupesurfaces_Groupe(this,"Limites intérieures",true));
		this->AppendPropertyPosition("volpos","Intérieur du volume",vec3(),true);
		this->AppendPropertyEntier("idvol","idvol",GetFreeId(parent),true)->Hide();

		_("Inside position");
	}

	~E_Scene_Volumes_Volume()
	{
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Volumes--;
	}
	int GetFreeId(Element* parent)
	{
		std::list<Element*> childs;
		parent->GetChildElement(childs);
		int freeid=1;
		for(std::list<Element*>::iterator itfils=childs.begin();itfils!=childs.end();itfils++)
		{
			if((*itfils)->IsPropertyExist("idvol"))
			{
				int idvol=(*itfils)->GetEntierConfig("idvol");
				if(idvol>freeid)
					freeid=idvol+1;
			}
		}
		return freeid;
	}
	void DrawItem( vec4 unitizeValue )
	{
		//On va dessiner le point au sein du volume
		vec3 position=coordsOperation::CommonCoordsToGlCoords(unitizeValue,this->GetPositionConfig("volpos"));
		vec3 couleurElement(180,160,240);

		Element* elprop=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_RENDU);
		if(elprop)
		{
			couleurElement=elprop->GetColorConfig("volcolor");
		}
		labelInfo.clear();
		AddText(" "+this->elementInfo.libelleElement,position,couleurElement);
		this->DrawPoint( position,couleurElement,5);
	}
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"volume");
		thisNode->AddProperty("id",Convertor::ToString(elementInfo.xmlIdElement));
		thisNode->AddProperty("name",elementInfo.libelleElement);
		Element::SaveXMLCoreDoc(thisNode);
		return thisNode;
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

	bool GetMaillageVolumeInfos(t_el_vol_infos& drawableElementInformations)
	{
		if(true)
		{
			drawableElementInformations.dotInsideVol=this->GetPositionConfig("volpos");
			Element* grpFaceElement=this->GetElementByType(ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE);
			if(grpFaceElement)
			{
				E_Scene_Groupesurfaces_Groupe* groupeEl=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(grpFaceElement);
				std::vector<t_faceIndex> grpFaces;
				groupeEl->GetFaces(grpFaces);
				if(grpFaces.size()>0)
				{
					drawableElementInformations.firstFace=grpFaces[0];
				}
				return true;
			}else{
				return false;
			}
		}else{
			return E_Drawable::GetMaillageVolumeInfos(drawableElementInformations);
		}
	}

	void OnRightClic(wxMenu* leMenu)
	{
		leMenu->Append(GetMenuItem(leMenu,Element::IDEVENT_CONVERT_VOL_TO_FITTING, _("Convert to fitting")));
		Element::OnRightClic(leMenu);
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("volume"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
