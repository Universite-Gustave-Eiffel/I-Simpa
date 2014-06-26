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

#include "data_manager/drawable_element.h"
#include "e_scene_sources_source_rendu.h"
#include "e_scene_sources_source_proprietes.h"
#include "data_manager/generic_element/e_property_freq.h"

/**  \file e_scene_sources_source.h
 *   \brief Source sonore ponctuelle de la scène
 */

/**
 *   \brief Source sonore ponctuelle de la scène
 * 
 */
class E_Scene_Sources_Source: public E_Drawable
{
public:
	E_Scene_Sources_Source( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Drawable(parent,"Source",Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SOUND_SOURCE_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SOUND_SOURCE_CLOSE);
		this->AllowMultipleSelection();
		this->elementInfo.userDestroyable=true;
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources++;
		_("Source");
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
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_RENDU)
					{
						this->AppendFils(new E_Scene_Sources_Source_Rendu(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES)
					{
						this->AppendFils(new E_Scene_Sources_Source_Proprietes(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_PROPERTY_FREQ)
					{
						this->AppendFils(new E_Property_Freq(currentChild,this));
					}
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Sources_Source( Element* parent,wxString nom=wxString::Format(_("Source %i"),ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources+1))
		:E_Drawable(parent,nom,Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SOUND_SOURCE_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SOUND_SOURCE_CLOSE);
		this->AllowMultipleSelection();
		this->elementInfo.userDestroyable=true;
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources++;
		this->AppendFils(new E_Scene_Sources_Source_Proprietes(this));
		this->AppendFils(new E_Scene_Sources_Source_Rendu(this));
		this->AppendFils(new E_Property_Freq(this,"Puissance"));
		_("Puissance");
		this->AppendPropertyPosition("pos_source","Position",vec3(0,0,0),true);
		_("Position");
	}

	~E_Scene_Sources_Source()
	{
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Sources--;
	}
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		bool useThisSource=true;
		Element* elFilsProperty=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES);
		if(elFilsProperty)
		{
			useThisSource=elFilsProperty->GetBoolConfig("enable");
		}
		if(useThisSource)
		{
			wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"source");
			thisNode->AddProperty("id",Convertor::ToString(elementInfo.xmlIdElement));
			thisNode->AddProperty("name",elementInfo.libelleElement);
			return Element::SaveXMLCoreDoc(thisNode);
		}else{
			return NoeudParent;
		}
	}

	void DrawItem( vec4 unitizeValue )
	{
		bool showSource=true;

		vec3 position=this->GetPositionConfig("pos_source");
		vec3 couleurElement;
		vec3 destinationFleche;
		Element* elFils=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_RENDU);
		Element* elFilsProperty=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES);
		if(elFils!=NULL)
			couleurElement=elFils->GetColorConfig("couleur");
		bool showArrow=false;
		if(elFilsProperty)
		{
			destinationFleche.set(-elFilsProperty->GetDecimalConfig("u"),-elFilsProperty->GetDecimalConfig("v"),-elFilsProperty->GetDecimalConfig("w"));
			destinationFleche=position+(destinationFleche/destinationFleche.length())/unitizeValue.w*.1f*destinationFleche.length();
			showArrow=elFilsProperty->GetListConfig("directivite")==E_Scene_Sources_Source_Proprietes::DIRECTIVITE_SOURCE_UNIDIRECTIONNEL;
			showSource=elFilsProperty->GetBoolConfig("enable");
		}
		labelInfo.clear();
		if(showSource)
		{
			position=coordsOperation::CommonCoordsToGlCoords(unitizeValue,position);
			destinationFleche=coordsOperation::CommonCoordsToGlCoords(unitizeValue,destinationFleche);
			this->DrawPoint( position,couleurElement,5);
			if(showArrow)
				this->DrawArrow( position,destinationFleche,couleurElement/255,.008f);
			bool drawText=elFils->GetBoolConfig("showlabel");
			if(drawText)
			{
				AddText(" "+this->elementInfo.libelleElement,position,couleurElement);
			}
		}
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
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("source"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};