/* ----------------------------------------------------------------------
* I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
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
* For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or
* send an email to contact@noise-planet.org
*
* ----------------------------------------------------------------------*/

...
#include "e_scene_projet_demo.h"
...

class E_Scene_Projet: public Element
{
public:
	E_Scene_Projet( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Projet",Element::ELEMENT_TYPE_SCENE_PROJET,Element::GRAPH_FOLDER,noeudCourant)
	{
		wxGetTranslation("Projet");
		
		if(noeudCourant!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// Lecture des fils noeud courant et création des éléments correspondants
			wxString propValue;
			while(currentChild!=NULL)
			{
				if(currentChild->GetPropVal("element_type",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					if(typeEle==Element::ELEMENT_TYPE_SCENE_PROJET_DEMO)
						this->AppendFils(new E_Scene_Projet_Demo(currentChild,this));
					else if(...)
						...
					...
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Scene_Projet( Element* parent)
		:Element(parent,"Projet",Element::ELEMENT_TYPE_SCENE_PROJET,Element::GRAPH_FOLDER)
	{
		this->AppendFils(new E_Scene_Projet_Demo(this));
	}
	...
};