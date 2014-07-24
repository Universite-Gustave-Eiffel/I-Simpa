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


/**  \file e_scene_projet_rendu_origine.h
 *   \brief Elément de configuration du rendu de l'origine et de la grille des plans de la scène 3D
 */

/**
 *   \brief Elément de configuration du rendu de l'origine et de la grille des plans de la scène 3D
 * 
 *   Cet élément est visible dans la vue 3D sous forme de 3 flèches et de 3 grilles
 */
class E_Scene_Rendu_Origine: public E_Drawable
{
private:

	void InitProps()
	{
		this->AppendPropertyColor("couleurx","Couleur flèche x",255,0,0);
		this->AppendPropertyColor("couleury","Couleur flèche y",0,255,0);
		this->AppendPropertyColor("couleurz","Couleur flèche z",0,0,255);
		this->AppendPropertyDecimal("arrowsize","Longueur flèche",.15f);
		this->AppendPropertyDecimal("arrowwidth","Largeur flèche",.008f);
		this->AppendPropertyBool("show","Afficher repère",true);
		this->AppendPropertyBool("showgxy","Grille plan xy",true);
		this->AppendPropertyBool("showgxz","Grille plan xz",false);
		this->AppendPropertyBool("showgyz","Grille plan yz",false);
		this->AppendPropertyColor("couleurgrille","Couleur de la grille",90,90,90);
		this->AppendPropertyEntier("echellegrille","Echelle de la grille",1);
		_("Arrow length");
		_("Arrow width");
		_("Arrow color (x)");
		_("Arrow color (y)");
		_("Arrow color (z)");
		_("Show XYZ axis");
		_("XY Grid");
		_("XZ Grid");
		_("YZ Grid");
		_("Grid color");
	}
public:
	E_Scene_Rendu_Origine( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Drawable(parent,"Repère",Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_ORIGIN);
		_("XYZ axis");
		this->InitProps();
	}

	E_Scene_Rendu_Origine( Element* parent)
		:E_Drawable(parent,"Repère",Element::ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_ORIGIN);
		this->InitProps();
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
		int echelle=this->GetEntierConfig("echellegrille");
		if(echelle<=0)
			return;
		vec3 minVar=ApplicationConfiguration::GetVecParam(ApplicationConfiguration::PARAM_MODEL_MINBOX);
		vec3 maxVar=ApplicationConfiguration::GetVecParam(ApplicationConfiguration::PARAM_MODEL_MAXBOX);
		labelInfo.clear();
		vec3 couleurElementx=this->GetColorConfig("couleurx")/255;
		vec3 couleurElementy=this->GetColorConfig("couleury")/255;
		vec3 couleurElementz=this->GetColorConfig("couleurz")/255;
		vec3 couleurGrille=this->GetColorConfig("couleurgrille")/255;
		if(this->GetBoolConfig("show"))
		{
			float awidth=this->GetDecimalConfig("arrowwidth");
			float asize=this->GetDecimalConfig("arrowsize");
			vec3 origine=coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,0,0));
			vec3 pointx=coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(-asize/unitizeValue.w,0,0));
			vec3 pointy=coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,-asize/unitizeValue.w,0));
			vec3 pointz=coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,0,-asize/unitizeValue.w));
			this->DrawArrow(origine,pointx,couleurElementx,awidth);
			this->DrawArrow(origine,pointy,couleurElementy,awidth);
			this->DrawArrow(origine,pointz,couleurElementz,awidth);
			vec3 pointtx=coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(asize/unitizeValue.w,0,0));
			vec3 pointty=coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,asize/unitizeValue.w,0));
			vec3 pointtz=coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,0,asize/unitizeValue.w));
			AddText("x",pointtx,couleurElementx*255);
			AddText("y",pointty,couleurElementy*255);
			AddText("z",pointtz,couleurElementz*255);
		}
		if(this->GetBoolConfig("showgxy"))	//Dessin de l'espace xy
		{
			DrawPlan( unitizeValue,
				vec3(0,0,-unitizeValue.w*echelle),
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(maxVar.x,0,0)),
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(minVar.x,0,0)),
				(-maxVar.z)/echelle,
				(-minVar.z)/echelle,
				couleurGrille,
				couleurElementx
				);
			
			DrawPlan( unitizeValue,					//Echelle
				vec3(unitizeValue.w*echelle,0,0),	//Pas
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,-minVar.z,0)),
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,-maxVar.z,0)),
				(minVar.x)/echelle,
				(maxVar.x)/echelle,
				couleurGrille,
				couleurElementy
				);
		}

		if(this->GetBoolConfig("showgxz"))	//Dessin de l'espace xz
		{
			DrawPlan( unitizeValue,
				vec3(0,-unitizeValue.w*echelle,0),
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(maxVar.x,0,0)),
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(minVar.x,0,0)),
				(-maxVar.y)/echelle,
				(-minVar.y)/echelle,
				couleurGrille,
				couleurElementx
				);
			
			DrawPlan( unitizeValue,					//Echelle
				vec3(unitizeValue.w*echelle,0,0),	//Pas
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,0,minVar.y)),					//
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,0,maxVar.y)),
				(minVar.x)/echelle,
				(maxVar.x)/echelle,
				couleurGrille,
				couleurElementz
				);
				
		}
		if(this->GetBoolConfig("showgyz"))	//Dessin de l'espace xz
		{
			DrawPlan( unitizeValue,
				vec3(0,-unitizeValue.w*echelle,0),
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,-maxVar.z,0)),
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,-minVar.z,0)),
				(-maxVar.y)/echelle,
				(-minVar.y)/echelle,
				couleurGrille,
				couleurElementy
				);
			
			DrawPlan( unitizeValue,					//Echelle
				vec3(0,0,unitizeValue.w*echelle),	//Pas
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,0,minVar.y)),					//
				coordsOperation::CommonCoordsToGlCoords(unitizeValue,vec3(0,0,maxVar.y)),
				(minVar.z)/echelle,
				(maxVar.z)/echelle,
				couleurGrille,
				couleurElementz
				);
				
		}
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("origine"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
