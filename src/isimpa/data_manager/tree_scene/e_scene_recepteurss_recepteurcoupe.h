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
#include "data_manager/appconfig.h"
#include "e_scene_recepteurss_recepteurcoupe_rendu.h"
#include "e_scene_recepteurss_recepteurcoupe_proprietes.h"

/** \file e_scene_recepteurss_recepteurcoupe.h
   \brief Element correspondant à un récépteur de surface en coupe

   Cet élément hérite de E_Drawable afin d'être représenté dans la vue 3D.
*/

/**
   \brief Element correspondant à un récépteur de surface en coupe

   Cet élément hérite de E_Drawable afin d'être représenté dans la vue 3D.
*/
class E_Scene_Recepteurss_RecepteurCoupe: public E_Drawable
{
public:
	E_Scene_Recepteurss_RecepteurCoupe( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Drawable(parent,"Plane Receiver",Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE,noeudCourant)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACE_RECEIVER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACE_RECEIVER_CLOSE);
		this->AllowMultipleSelection();
		this->elementInfo.userDestroyable=true;
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurS++;
		wxGetTranslation("Plane Receiver");
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
					
					if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_PROPRIETES)
					{
						this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe_Proprietes(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_RENDU)
					{
						this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe_Rendu(currentChild,this));
					}
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	/**
	 * Constructeur de création, appelé lors de l'ajout d'un nouveau récepteur.
	 * @param parent Element parent
	 * @param nom Libellé du recepteur
	 */
	E_Scene_Recepteurss_RecepteurCoupe( Element* parent,wxString nom="Plane Receiver")
		:E_Drawable(parent,nom,Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_SURFACE_RECEIVER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_SURFACE_RECEIVER_CLOSE);
		vec3 boundingBoxMin=ApplicationConfiguration::GetVecParam(ApplicationConfiguration::PARAM_MODEL_MINBOX);
		vec3 boundingBoxMax=ApplicationConfiguration::GetVecParam(ApplicationConfiguration::PARAM_MODEL_MAXBOX);
		boundingBoxMin.set(boundingBoxMin.x,-boundingBoxMin.z,boundingBoxMin.y);
		boundingBoxMax.set(boundingBoxMax.x,-boundingBoxMax.z,boundingBoxMax.y);
		this->AllowMultipleSelection();
		this->elementInfo.userDestroyable=true;
		this->AppendPropertyPosition("verta","Vertex A",vec3(boundingBoxMin.x,boundingBoxMax.y,boundingBoxMin.z+1.6));
		this->AppendPropertyPosition("vertb","Vertex B",vec3(boundingBoxMin.x,boundingBoxMin.y,boundingBoxMin.z+1.6));
		this->AppendPropertyPosition("vertc","Vertex C",vec3(boundingBoxMax.x,boundingBoxMin.y,boundingBoxMin.z+1.6));
		AppendFilsByType(ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_PROPRIETES);
		AppendFilsByType(ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_RENDU);
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurS++;
	}
	virtual Element* AppendFilsByType(ELEMENT_TYPE etypefils,const wxString& libelle="")
	{
		if(etypefils==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_PROPRIETES)
		{
			return this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe_Proprietes(this));
		}else if(etypefils==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_RENDU)
		{
			return this->AppendFils(new E_Scene_Recepteurss_RecepteurCoupe_Rendu(this));
		}
		return NULL;
	}
	~E_Scene_Recepteurss_RecepteurCoupe()
	{
		ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_RecepteurS--;
	}
	void DrawPlan( vec4 unitizeValue,vec3 pas,vec3 extremitea,vec3 extremiteb,int rangebegin,int rangeend,vec3 couleurGrille,vec3 couleurElement)
	{
		BeginLines();
		for(int i=rangebegin;i<=rangeend;i++)
			if(i!=0)
				DrawLine(extremiteb+(pas*i),extremitea+(pas*i),couleurGrille);
		DrawLine(extremiteb,extremitea,couleurElement);
		EndLines();
	}
	void DrawItem( vec4 unitizeValue )
	{
		vec3 Avert=this->GetPositionConfig("verta");
		vec3 Bvert=this->GetPositionConfig("vertb");
		vec3 Cvert=this->GetPositionConfig("vertc");
		float resolution=1;
		bool rendergrille(false);
		bool renderlabels(false);
		Element* renderele=this->GetElementByType(ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_RENDU);
		Element* propele=this->GetElementByType(ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_PROPRIETES);
		vec4 rscolor;
		if(renderele)
		{
			rscolor=renderele->GetColorConfig("planecolor")/255;
			rendergrille=renderele->GetBoolConfig("showplane");
			renderlabels=renderele->GetBoolConfig("showlabels");
			rscolor.w=1;
		}
		if(propele)
		{
			resolution=propele->GetDecimalConfig("resolution");
			rendergrille=(rendergrille && propele->GetBoolConfig("enabled"));
			renderlabels=(renderlabels && propele->GetBoolConfig("enabled"));
		}
		if(rendergrille)
		{
			vec3 BC=Cvert-Bvert;
			vec3 BA=Avert-Bvert;
			int NbCellU=(int)ceilf(BC.length()/resolution);
			int NbCellV=(int)ceilf(BA.length()/resolution);
			float Ucell_size=BC.length()/NbCellU;
			float Vcell_size=BA.length()/NbCellV;
			vec3 stepU=BC/BC.length()*Ucell_size;
			vec3 stepV=BA/BA.length()*Vcell_size;


			DrawPlan( unitizeValue,
			vec3(stepU.x,stepU.z,-stepU.y)*unitizeValue.w,
			coordsOperation::CommonCoordsToGlCoords(unitizeValue,Bvert),
			coordsOperation::CommonCoordsToGlCoords(unitizeValue,Avert),
			0,
			NbCellU,
			rscolor,
			rscolor
			);
			
			DrawPlan( unitizeValue,
			vec3(stepV.x,stepV.z,-stepV.y)*unitizeValue.w,
			coordsOperation::CommonCoordsToGlCoords(unitizeValue,Bvert),
			coordsOperation::CommonCoordsToGlCoords(unitizeValue,Cvert),
			0,
			NbCellV,
			rscolor,
			rscolor
			);
		}
		this->labelInfo.clear();
		if(renderlabels)
		{
			this->AddText("A",coordsOperation::CommonCoordsToGlCoords(unitizeValue,Avert),rscolor*255);
			this->AddText("B",coordsOperation::CommonCoordsToGlCoords(unitizeValue,Bvert),rscolor*255);
			this->AddText("C",coordsOperation::CommonCoordsToGlCoords(unitizeValue,Cvert),rscolor*255);
		}
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		Element* propel=this->GetElementByType(ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_PROPRIETES);
		if(propel && propel->GetBoolConfig("enabled"))
		{
			vec3 Avert=this->GetPositionConfig("verta");
			vec3 Bvert=this->GetPositionConfig("vertb");
			vec3 Cvert=this->GetPositionConfig("vertc");
			wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"recepteur_surfacique_coupe");
			thisNode->AddAttribute("id",Convertor::ToString(elementInfo.xmlIdElement));
			thisNode->AddAttribute("name",elementInfo.libelleElement);
			thisNode->AddAttribute("ax",Convertor::ToString(Avert.x));
			thisNode->AddAttribute("ay",Convertor::ToString(Avert.y));
			thisNode->AddAttribute("az",Convertor::ToString(Avert.z));
			thisNode->AddAttribute("bx",Convertor::ToString(Bvert.x));
			thisNode->AddAttribute("by",Convertor::ToString(Bvert.y));
			thisNode->AddAttribute("bz",Convertor::ToString(Bvert.z));
			thisNode->AddAttribute("cx",Convertor::ToString(Cvert.x));
			thisNode->AddAttribute("cy",Convertor::ToString(Cvert.y));
			thisNode->AddAttribute("cz",Convertor::ToString(Cvert.z));
			return E_Drawable::SaveXMLCoreDoc(thisNode);
		}else{
			return E_Drawable::SaveXMLCoreDoc(NoeudParent);
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


	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("recepteurscoupe"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
};
