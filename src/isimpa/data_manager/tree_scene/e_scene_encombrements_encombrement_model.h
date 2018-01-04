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
#include "e_scene_encombrements_encombrement_rendu.h"
#include "data_manager/drawable_element.h"
#include "e_scene_encombrements_encombrement_proprietes.h"
#include "data_manager/generic_element/e_gammeabsorption.h"

/** \file e_scene_encombrements_encombrement_model.h
   \brief Cet élément est un volume d'encombrement défini à partir de la scène par l'utilisateur
*/

/**
   \brief Cet élément est un volume d'encombrement défini à partir de la scène par l'utilisateur

   Cet encombrement est défini par des faces de la scène.
   L'utilisateur fait un glisser-déposer des faces qui doivent être considérées comme la limite d'un encombrement
*/
class E_Scene_Encombrements_Encombrement_Model: public E_Drawable
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
	void InitEncombrementProp()
	{
		this->elementInfo.userDestroyable=true;
	}
public:
	E_Scene_Encombrements_Encombrement_Model( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Drawable(parent,"Encombrement",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_FITTING_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_FITTING_CLOSE);
		this->elementInfo.userDestroyable=true;
		_("Fitting zone");
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
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)
					{
						this->AppendFils(new E_Scene_Encombrements_Encombrement_Proprietes(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE)
					{
						// Change label issue #142
						if(currentChild->GetAttribute("label","") != "Surface") {
							currentChild->DeleteAttribute("label");
							currentChild->AddAttribute("label", wxTRANSLATE("Surfaces"));
						}
						this->AppendFils(new E_Scene_Groupesurfaces_Groupe(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_GAMMEABSORPTION)
					{
						this->AppendFils(new E_GammeAbsorption(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU)
					{
						this->AppendFils(new E_Scene_Encombrements_Encombrement_Rendu(currentChild,this));
					}	
				}
				currentChild = currentChild->GetNext();
			}
		}
		if(this->GetElementByType(ELEMENT_TYPE_POSITION)==NULL) //maj version < 4/11/2008
		{
			this->AppendPropertyPosition("volpos",_("Inside position"),vec3(),true);
			wxLogWarning(_("Please define a point inside the fitting zone %s"),this->elementInfo.libelleElement);
		}
	}

	E_Scene_Encombrements_Encombrement_Model( Element* parent,wxString nom=wxString::Format(_("Fitting zone %i"),ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement+1))
		:E_Drawable(parent,nom,Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_FITTING_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_FITTING_CLOSE);
		this->elementInfo.userDestroyable=true;
		this->AppendFils(new E_Scene_Encombrements_Encombrement_Proprietes(this));
		this->AppendFils(new E_Scene_Groupesurfaces_Groupe(this,wxTRANSLATE("Surfaces"),true));
		this->AppendFils(new E_GammeAbsorption(this));
		this->AppendFils(new E_Scene_Encombrements_Encombrement_Rendu(this));
		this->AppendPropertyPosition("volpos",_("Inside position"),vec3(),true);
		wxLogWarning(_("Please define a point inside the fitting zone %s"),this->elementInfo.libelleElement);
		InitEncombrementProp(); 
	}

	void DrawItem( vec4 unitizeValue )
	{
		if(this->GetElementByType(ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)->GetBoolConfig("useforcalculation"))
		{
			Element* elRendu=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU);
			if(elRendu!=NULL && elRendu->GetBoolConfig("showlabel"))
			{
				//On va dessiner le point au sein du volume
				vec3 position=coordsOperation::CommonCoordsToGlCoords(unitizeValue,this->GetPositionConfig("volpos"));
				vec3 couleurElement(125,125,0);
				labelInfo.clear();
				AddText(" "+this->elementInfo.libelleElement,position,couleurElement);
				this->DrawPoint( position,couleurElement,5);
			}
		}
	}
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(this->GetElementByType(ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)->GetBoolConfig("useforcalculation"))
		{
			wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"encombrement");
			thisNode->AddAttribute("id",Convertor::ToString(elementInfo.xmlIdElement));
			Element::SaveXMLCoreDoc(thisNode);
			return thisNode;
		}else{
			return NoeudParent;
		}	
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
		if(this->GetElementByType(ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)->GetBoolConfig("useforcalculation"))
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


	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("encombrement"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
