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

#include "data_manager/e_data.h"

#ifndef __ELEMENT_TEXT__
#define __ELEMENT_TEXT__

/** \file e_data_text.h
   \brief Classe spécialisant E_Data afin de représenter un champ texte
*/

/**
	\brief Classe spécialisant E_Data afin de représenter un champ texte
*/
class E_Data_Text: public E_Data
{
private:
	wxString textElement;
public:
	E_Data_Text( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Data(parent,"Unnamedprop","",Element::ELEMENT_TYPE_TEXT,noeudCourant)
	{
		wxString propVal;
		//Affecte la couleur de l'element
		if(noeudCourant->GetAttribute("textValue",&propVal))
			textElement=propVal;
		if(noeudCourant->GetAttribute("readonly",&propVal))
			readOnly=true;
	}

	/**
	 *	@param parent Element parent
	 *	@param[in] dataName Nom du champ ( non visible par l'utilisateur)
	 *	@param[in] dataLabel Libellé de la ligne
	 *	@param[in] defaultDataValue Type de ligne ( par défaut ligne standard )
	 *	@param[in] _readOnly Champ en lecture seule
	 *  @see Element::AppendPropertyText()
	 */
	E_Data_Text( Element* parent, wxString dataName,wxString dataLabel, wxString defaultDataValue="",bool _readOnly=false)
		:E_Data(parent,dataName,dataLabel,Element::ELEMENT_TYPE_TEXT)
	{
		textElement=defaultDataValue;
		readOnly=_readOnly;
	}
	virtual void FillWxGrid(wxGrid* gridToFeed,int col=0)
	{
		int row=gridToFeed->GetNumberRows()-1;
		E_Data::FillWxGrid(gridToFeed,col);
		gridToFeed->SetCellValue(row,col,textElement);
	}
	virtual void OnValueChanged(wxGridEvent& ev,wxGrid* gridCtrl)
	{
		if(!readOnly)
		{
			textElement=gridCtrl->GetCellValue(ev.GetRow(),ev.GetCol());
			this->Modified(this);
		}
	}


	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Data::SaveXMLDoc(NoeudParent);
		thisNode->DeleteAttribute("textValue");
		thisNode->AddAttribute("textValue",textElement);
		return thisNode;
	}
	/**
	 * Affecte une nouvelle valeur
	 * @param[in] newValue Nouvelle valeur à inserer
	 * @see Element::UpdateStringConfig()
	 */
	void SetValue(wxString newValue)
	{
		if(newValue!=this->textElement)
		{
			this->textElement=newValue;
			this->Modified(this);
		}
	}
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(exportToCore)
		{
			NoeudParent->DeleteAttribute(this->elementInfo.libelleElement);
			NoeudParent->AddAttribute(this->elementInfo.libelleElement,textElement);
		}
		return NoeudParent;
	}
	/**
	 * Obtenir la valeur de l'élément
	 * @return Valeur du champ
	 * @see Element::GetStringConfig()
	 */
	wxString GetValue()
	{
		return this->textElement;
	}
private:
};

#endif