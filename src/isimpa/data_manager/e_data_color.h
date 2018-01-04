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
#ifndef __ELEMENT_COLOR__
#define __ELEMENT_COLOR__
#include "data_manager/customEditor/wxGridCellColorEditor.h"

/** \file e_data_color.h
   \brief Classe spécialisant E_Data afin de représenter une valeur de couleur
*/

/**
	\brief Classe spécialisant E_Data afin de représenter une valeur de couleur
	@see MainPropGrid
*/


class E_Data_Color: public E_Data
{
protected:
	wxColour valeurEl;
public:
	E_Data_Color( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Data(parent,"Unnamedprop","",Element::ELEMENT_TYPE_COLOR,noeudCourant)
	{

		//Creation de la couleur à partir du document XML
		wxString propRed;
		wxString propGreen;
		wxString propBlue;

		
		if(noeudCourant->GetAttribute("r",&propRed) && noeudCourant->GetAttribute("g",&propGreen)
			&& noeudCourant->GetAttribute("b",&propBlue))
		{
			long rVal;
			long gVal;
			long bVal;
			propRed.ToLong(&rVal);
			propGreen.ToLong(&gVal);
			propBlue.ToLong(&bVal);
			wxColour nvCouleur( rVal,gVal,bVal);
			valeurEl=nvCouleur;
		}
	}
	/**
	 * @see Element::AppendPropertyColor()
	 * @param parent Element parent
	 * @param dataName Nom du champ ( non visible par l'utilisateur)
	 * @param dataLabel Libellé du champ
	 * @param defaultColor Valeur par défaut
	 */
	E_Data_Color( Element* parent, wxString dataName,wxString dataLabel, wxColor defaultColor)
		:E_Data(parent,dataName,dataLabel,Element::ELEMENT_TYPE_COLOR)
	{
		valeurEl=defaultColor;
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Data::SaveXMLDoc(NoeudParent);
		wxString compR;
		wxString compG;
		wxString compB;
		compR<<((int)valeurEl.Red());
		compG<<((int)valeurEl.Green());
		compB<<((int)valeurEl.Blue());
		thisNode->DeleteAttribute("r");
		thisNode->AddAttribute("r",compR);
		thisNode->DeleteAttribute("g");
		thisNode->AddAttribute("g",compG);
		thisNode->DeleteAttribute("b");
		thisNode->AddAttribute("b",compB);

		return thisNode;
	}

	virtual void FillWxGrid(wxGrid* gridToFeed,int col=0)
	{
		E_Data::FillWxGrid(gridToFeed,col);
		int row=gridToFeed->GetNumberRows()-1;
		gridToFeed->SetCellEditor(row, col, new wxGridCellColorEditor());
		gridToFeed->SetCellBackgroundColour(row,col,valeurEl);
	}

	/**
	 * Retourne la valeur RGB de l'élément sous forme d'un vecteur (0-255,0-255,0-255) 
	 * @return Valeur vec3 correspondant à la composante rouge vert bleu
	 */
	vec3 GetValue()
	{
		return vec3(valeurEl.Red(),valeurEl.Green(),valeurEl.Blue());
	}
	void SetValue(const wxColor& newColor)
	{
		if(newColor!=valeurEl)
		{
			valeurEl=newColor;
			Modified(this);
		}
	}
	virtual void OnValueChanged(wxGridEvent& ev,wxGrid* gridCtrl)
	{
		int row=ev.GetRow();
		int col=ev.GetCol();
		SetValue(gridCtrl->GetCellBackgroundColour(row,col));
	}

};

#endif