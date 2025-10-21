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


#define USE_OCTREE 0

#include "data_manager/element.h"
#include "e_core_sppscore.h"
#if USE_OCTREE
#include "e_core_sppsoctreecore.h"
#endif
#include "e_core_tccore.h"
#include "e_core_tlmcore.h"


/*! \file e_core.h
    \brief Correspond à l'élément de racine de l'arbre de calcul
*/
/**
	\brief Correspond à l'élément de racine de l'arbre de calcul

	Cette classe propage l'événement de mise à jour jusqu'à la classe de gestion de projet
*/
class E_Core: public Element
{
private:
void (*pointeurFonctionEvent)(Element*);
bool eventBinded;
public:
	E_Core( wxXmlNode* noeudCourant )
		:Element(NULL,"Code de calcul",Element::ELEMENT_TYPE_CORE_ROOT,noeudCourant)
	{

		this->elementInfo.expanded=true;
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_CORES_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_CORES_CLOSE);
		eventBinded=false;
		wxGetTranslation("Calculation code");
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
					if(typeEle==Element::ELEMENT_TYPE_CORE_SPPS)
						this->AppendFils(new E_Core_Spps(this,currentChild));
					else if(typeEle==Element::ELEMENT_TYPE_CORE_TC)
						this->AppendFils(new E_Core_Tc(this,currentChild));
					else if(typeEle==Element::ELEMENT_TYPE_CORE_TLM)
						this->AppendFils(new E_Core_Tlm(this,currentChild));

				}
				currentChild=currentChild->GetNext();
			}
		}
	}

	E_Core( )
		:Element(NULL,"Code de calcul",Element::ELEMENT_TYPE_CORE_ROOT)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_CORES_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_CORES_CLOSE);
		eventBinded=false;
		this->AppendFils(new E_Core_Spps(this));
		this->AppendFils(new E_Core_Tc(this));
		this->elementInfo.expanded=true;
	}
	/**
		Initialise le pointeur de méthode vers le gestionnaire de projet
		@param _pointeurFonctionEvent Méthode d'événement de mise à jour
		@see OnElementRootUpdate()
	*/
	void BindUpdateEvent(void (*_pointeurFonctionEvent)(Element*))
	{
		pointeurFonctionEvent=_pointeurFonctionEvent;
		eventBinded=true;
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("Core"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
protected:

	void Modified(Element* eModif)
	{
		Element::Modified(eModif);
		if(eventBinded)
			(*pointeurFonctionEvent)(eModif);
	}
};
