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

#include "e_scene.h"

#include "e_scene_donnees.h"
#include "e_scene_projet.h"
#include "last_cpp_include.hpp"


E_Scene::E_Scene( wxXmlNode* noeudCourant )
	:Element(NULL,"Scène",Element::ELEMENT_TYPE_SCENE_ROOT,noeudCourant)
{
	wxGetTranslation("Scene");
	eventBinded=false;
	this->Init();
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
				if(typeEle==Element::ELEMENT_TYPE_SCENE_DONNEES)
				{
					this->AppendFils(new E_Scene_Donnees(currentChild,this));
				}else if(typeEle==Element::ELEMENT_TYPE_SCENE_PROJET)
				{
					if(currentChild->GetAttribute("name") == "Projet") {
						currentChild->DeleteAttribute("name");
						currentChild->AddAttribute("name", wxTRANSLATE("Project"));
					}
					this->AppendFils(new E_Scene_Projet(currentChild,this));
				}
			}
			currentChild = currentChild->GetNext();
		}
	}
}

E_Scene::E_Scene( )
	:Element(NULL,"Scène",Element::ELEMENT_TYPE_SCENE_ROOT)
{
	eventBinded=false;
	this->AppendFils(new E_Scene_Donnees(this));
	this->AppendFils(new E_Scene_Projet(this));
	//this->elementInfo.expanded=true;
}

/**
	Initialise le pointeur de méthode vers le gestionnaire de projet
	@param _pointeurFonctionEvent Méthode d'événement de mise à jour
	@see OnElementRootUpdate()
*/
void E_Scene::BindUpdateEvent(void (*_pointeurFonctionEvent)(Element*))
{
	pointeurFonctionEvent=_pointeurFonctionEvent;
	eventBinded=true;
}

void E_Scene::Init()
{


}







wxXmlNode* E_Scene::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
	thisNode->SetName("Scene"); // Nom de la balise xml ( pas d'espace autorise )

	return thisNode;
}
void E_Scene::Modified(Element* eModif)
{
	Element::Modified(eModif);
	if(eventBinded)
		(*pointeurFonctionEvent)(eModif);
}
