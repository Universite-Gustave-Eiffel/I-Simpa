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
#include "e_scene_encombrements_encombrement_proprietes.h"
#include "data_manager/generic_element/e_gammeabsorption.h"
#include "e_scene_encombrements_encombrement_rendu.h"

/** \file e_scene_encombrements_encombrement_cuboide.h
   \brief Cet élément est un volume d'encombrement défini par l'utilisateur
*/

/**
   \brief Cet élément est un volume d'encombrement défini par l'utilisateur

   Contrairement à E_Scene_Encombrements_Encombrement_Model cet encombrement n'a pas besoin d'avoir été modélisé dans la scène.
   L'utilisateur défini les coordonnées de l'encombrement et le logiciel va automatiquement faire une extrusion de cet encombrement à partir du modèle ( si nécessaire )
*/
class E_Scene_Encombrements_Encombrement_Cuboide: public E_Drawable
{
private:
	bool needBuild;
public:
	E_Scene_Encombrements_Encombrement_Cuboide( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Drawable(parent,"Encombrement",Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE,noeudCourant)
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
					}else if(typeEle==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU)
					{
						this->AppendFils(new E_Scene_Encombrements_Encombrement_Rendu(currentChild,this));
					}else if(typeEle==Element::ELEMENT_TYPE_GAMMEABSORPTION)
					{
						this->AppendFils(new E_GammeAbsorption(currentChild,this));
					}	
				}
				currentChild = currentChild->GetNext();
			}
		}
		needBuild=true;
	}

	E_Scene_Encombrements_Encombrement_Cuboide( Element* parent,wxString nom=wxString::Format(_("Fitting zone %i"),ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Encombrement+1))
		:E_Drawable(parent,nom,Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_FITTING_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_FITTING_CLOSE);
		this->elementInfo.userDestroyable=true;
		needBuild=false;
		this->AppendFils(new E_Scene_Encombrements_Encombrement_Proprietes(this));
		this->AppendFils(new E_GammeAbsorption(this));
		this->AppendFils(new E_Scene_Encombrements_Encombrement_Rendu(this));
		_("Surface");
		InitEncombrementProp();
	}
	void InitEncombrementProp()
	{
		this->AppendPropertyPosition("ba","Origin",vec3(0,0,0));
		this->AppendPropertyPosition("hc","Destination volume",vec3(0,0,0));
		_("Origin");
		_("Destination");
	}
	/**
	 *  Construction du cuboide
	 *  @param unitizeValue Vecteur de redimensionnement
	 *  @param origine Position d'origine
	 *  @param destination Position de fin du volume
	 */
	void BuildModel(vec4 unitizeValue,vec3 origine,vec3 destination)
	{
		needBuild=false;
		consistentModel.clear();
		//A partir des deux position construit le modèle 3D
		//	 HA_____HB
		//	 /	   /\
		//HD/ BA  / / BB
		// /___HC/ /
		// \BD___\/BC
		vec3 HA,HB,HC,HD,BA,BB,BC,BD,tmpVec; //1 par arrête du cube
		if(origine==destination)
			return;
		if(origine.x-destination.x>0)
		{
			tmpVec=origine;
			origine.x=destination.x;
			destination.x=tmpVec.x;
		}
		if(origine.y-destination.y>0)
		{
			tmpVec=origine;
			origine.y=destination.y;
			destination.y=tmpVec.y;
		}
		if(origine.z-destination.z>0)
		{
			tmpVec=origine;
			origine.z=destination.z;
			destination.z=tmpVec.z;
		}
		BA=origine;
		HC=destination;
		HA.set(BA.x,HC.y,BA.z);
		HB.set(HC.x,HC.y,BA.z);
		HD.set(BA.x,HC.y,HC.z);
		BB.set(HC.x,BA.y,BA.z);
		BC.set(HC.x,BA.y,HC.z);
		BD.set(BA.x,BA.y,HC.z);

		vec3 couleur(1,0,0);
		labelInfo.clear();

		//Face du bas
		this->PushTriangle(BC,BD,BA);
		this->PushTriangle(BC,BA,BB);
		//Coté ab		
		this->PushTriangle(BA,HA,HB);
		this->PushTriangle(BA,HB,BB);
		//Coté ad	
		this->PushTriangle(BA,HD,HA);
		this->PushTriangle(BA,BD,HD);
		//Coté dc
		this->PushTriangle(BD,BC,HC);
		this->PushTriangle(HD,BD,HC);
		//coté cb
		this->PushTriangle(BC,BB,HB);
		this->PushTriangle(HC,BC,HB);
		//Construction de la face du haut
		this->PushTriangle(HC,HB,HA);
		this->PushTriangle(HD,HC,HA);
	}

	void DrawTriangles(vec4 unitizeValue,std::vector<t_el_draw_object> &drawObject)
	{
		labelInfo.clear();
		if(needBuild)
			BuildModel(unitizeValue,coordsOperation::CommonCoordsToGlCoords(unitizeValue,this->GetPositionConfig("ba")),coordsOperation::CommonCoordsToGlCoords(unitizeValue,this->GetPositionConfig("hc")));
		vec3 couleurElement(1,0,0);
		bool afficher=true;
		bool afficherTexte=false;
		float transp=.5;


		Element* elProp=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES);
		if(elProp)
		{
			afficher=elProp->GetBoolConfig("useforcalculation");
		}
		if(afficher)
		{
			Element* elRendu=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU);
			if(elRendu!=NULL)
			{
				couleurElement=elRendu->GetColorConfig("couleur");
				afficherTexte=elRendu->GetBoolConfig("showlabel");
				transp=elRendu->GetDecimalConfig("obj_transp");
				bool showface=(elRendu->GetListConfig("rendermode")==0);

				if(afficherTexte)
				{
					vec3 posTxt=coordsOperation::CommonCoordsToGlCoords(unitizeValue,this->GetPositionConfig("hc"));
					posTxt.z=posTxt.z+.1;
					AddText(this->elementInfo.libelleElement,posTxt,couleurElement);
				}
				if(showface)
				{
					t_el_draw_object nouvDraw;
					couleurElement/=255;
					nouvDraw.colours.colorA=vec4(couleurElement,transp);
					nouvDraw.colours.colorD=vec4(couleurElement,transp);
					nouvDraw.colours.colorS=vec4(couleurElement,transp);
					nouvDraw.colours.colorD*.75;
					nouvDraw.colours.colorD.w=transp;
					for(int i=0;i<consistentModel.size();i++)
					{
						t_triangle cpTriangle=consistentModel[i];
						vec3 ansA=cpTriangle.a;
						cpTriangle.a=cpTriangle.c;
						cpTriangle.c=ansA;
						nouvDraw.triangles.push_back(cpTriangle);

					}
					drawObject.push_back(nouvDraw);
				}
			}
		}
		
	}
	void DrawItem( vec4 unitizeValue )
	{
		bool afficher=false;
		Element* elRendu=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU);
		Element* elProp=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES);
		if(elProp)
		{
			afficher=elProp->GetBoolConfig("useforcalculation");
		}
		if(elRendu!=NULL && afficher)
		{	
			bool showlines=(elRendu->GetListConfig("rendermode")==1);

			if(showlines)
			{
				vec4 couleurElement;
				Element* elRendu=this->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU);
				if(elRendu!=NULL)
				{
					couleurElement=elRendu->GetColorConfig("couleur")/255;
				}
				vec3 origine=coordsOperation::CommonCoordsToGlCoords(unitizeValue,this->GetPositionConfig("ba"));
				vec3 destination=coordsOperation::CommonCoordsToGlCoords(unitizeValue,this->GetPositionConfig("hc"));

				vec3 HA,HB,HC,HD,BA,BB,BC,BD,tmpVec;
				if(origine==destination)
					return;
				if(origine.x-destination.x>0)
				{
					tmpVec=origine;
					origine.x=destination.x;
					destination.x=tmpVec.x;
				}
				if(origine.y-destination.y>0)
				{
					tmpVec=origine;
					origine.y=destination.y;
					destination.y=tmpVec.y;
				}
				if(origine.z-destination.z>0)
				{
					tmpVec=origine;
					origine.z=destination.z;
					destination.z=tmpVec.z;
				}
				BA=origine;
				HC=destination;
				HA.set(BA.x,HC.y,BA.z);
				HB.set(HC.x,HC.y,BA.z);
				HD.set(BA.x,HC.y,HC.z);
				BB.set(HC.x,BA.y,BA.z);
				BC.set(HC.x,BA.y,HC.z);
				BD.set(BA.x,BA.y,HC.z);

				this->BeginLines();

				//Rectangle du bas
				this->DrawLine(BD,BC,couleurElement);
				this->DrawLine(BC,BB,couleurElement);
				this->DrawLine(BB,BA,couleurElement);
				this->DrawLine(BA,BD,couleurElement);
				//Rectangle du haut
				this->DrawLine(HD,HC,couleurElement);
				this->DrawLine(HC,HB,couleurElement);
				this->DrawLine(HB,HA,couleurElement);
				this->DrawLine(HA,HD,couleurElement);
				//cotés
				this->DrawLine(BD,HD,couleurElement);
				this->DrawLine(BC,HC,couleurElement);
				this->DrawLine(BB,HB,couleurElement);
				this->DrawLine(BA,HA,couleurElement);
				
				this->EndLines();
			}		
		}
	}

	virtual void GetConsistentModel(std::vector<t_triangle>& triangleLst)
	{
		if(this->GetElementByType(ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)->GetBoolConfig("useforcalculation"))
		{
			E_Drawable::GetConsistentModel(triangleLst);
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
			vec3 posDeb=this->GetPositionConfig("ba");
			vec3 posFin=this->GetPositionConfig("hc");
			drawableElementInformations.dotInsideVol=posDeb+(posFin-posDeb)*EPSILON;
			return true;
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
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(this->GetElementByType(ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)->GetBoolConfig("useforcalculation"))
		{
			wxXmlNode* thisNode = new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"encombrement");
			thisNode->AddAttribute("id",Convertor::ToString(elementInfo.xmlIdElement));
			Element::SaveXMLCoreDoc(thisNode);
			return thisNode;
		}else{
			wxLogWarning(_("Fitting zone %s is not taken into account in the calculation (user choice)"),this->elementInfo.libelleElement);
			return NoeudParent;
		}
	}
	virtual void Modified(Element* eModif)
	{
		Element* pereEModif=eModif->GetElementParent();
		t_elementInfo elInfo=eModif->GetElementInfos();
		if(pereEModif!=NULL)
		{
			t_elementInfo pereInfo=pereEModif->GetElementInfos();
			if(pereInfo.typeElement==ELEMENT_TYPE_POSITION || elInfo.typeElement==ELEMENT_TYPE_POSITION)				
				needBuild=true;
		}
		E_Drawable::Modified(eModif);
	}
};
