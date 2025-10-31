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

#include "first_header_include.hpp"

#include "data_manager/e_data.h"
#include "customEditor/wxGridCellTreeEditor.h"

#ifndef __ELEMENT_TREE_LIST__
#define __ELEMENT_TREE_LIST__

/** \file e_data_tree.h
   \brief Classe spécialisant E_Data afin de représenter une liste de valeur sous forme d'arbre
   @see ComboTreeCtrl
   @see E_ComboTreeVisible
   @see wxGridCellTreeEditor
*/

/**
	\brief Classe spécialisant E_Data afin de représenter une liste de valeur sous forme d'arbre
	@see MainPropGrid
*/
class E_Data_Tree: public E_Data
{
protected:
	Element* rootItem;
	Element* userChoice;
public:	
	/**
	 *	@see Element::AppendPropertyList()
	 *	@param parent Element parent
	 *	@param[in] dataName Nom du champ ( non visible par l'utilisateur)
	 *	@param[in] dataLabel Libellé du champ
	 *	@param[in] _eList Liste des valeurs de sélection possibles
	 *  @param[in] _iList Liste des indices correspondant aux valeurs.
	 *	@param[in] defaultChoice Valeur par défaut
	 *	@param[in] asTitle Ce champ doit-il être visible en premier dans la liste des propriétés
	 *	@param[in] hSize Combien de cellules ce champ va occuper
	 */
	E_Data_Tree( Element* parent, wxString dataName,wxString dataLabel, Element* treeRoot,const std::list<Element::ELEMENT_TYPE>& elementFilter,Element* defaultValue=NULL, bool asTitle=false, int hSize=1)
		:E_Data(parent,dataName,dataLabel,Element::ELEMENT_TYPE_TREE_LIST,NULL,asTitle,hSize),
		rootItem(treeRoot),
		userChoice(defaultValue),
		showElementsFilter(elementFilter)
	{

	}

	//Ce champ n'est pas sauvegardé car les élément de séléction ne sont pas constant ( pointeur vers un élement du projet)
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{		
		return NoeudParent;
	}

	virtual void FillWxGrid(wxGrid* gridToFeed,int col=0)
	{
		E_Data::FillWxGrid(gridToFeed,col);
		int row=gridToFeed->GetNumberRows()-1;
		gridToFeed->SetCellEditor(row, col, new wxGridCellTreeEditor(rootItem,userChoice,showElementsFilter) );
		SetGridValue(gridToFeed,row,col);
	}
	void UpdateValue( wxGrid* gridCtrl, int row, int col=0)
	{
		E_Data::UpdateValue(gridCtrl,row,col);
		wxString currentLibel;
		if(userChoice)
			currentLibel=userChoice->GetElementInfos().libelleElement;
		if(gridCtrl->GetCellValue(row,col)!= currentLibel)
			SetGridValue(gridCtrl,row,col);
	}
	/**
	 * Mettre à jour la valeur du champ
	 * @return Entier, indice désignant l'élément par défaut.
	 * @see Element::GetListConfig()
	 */
	Element* GetChoice()
	{
		return userChoice;
	}
	/**
	 * Mettre à jour la valeur du champ
	 * @param[in] NewChoice Entier, indice désignant le nouvel élément par défaut.Si l'indice est invalide aucune modification est effectuée.
	 * @see Element::UpdateListConfig()
	 */
	void SetValue(Element* NewChoice)
	{
		if(userChoice!=NewChoice)
		{
			userChoice=NewChoice;
			Modified(this);
		}
	}

	virtual void OnValueChanged(wxGridEvent& ev,wxGrid* gridCtrl)
	{
		wxGridCellEditor* cellEdit=gridCtrl->GetCellEditor(ev.GetRow(),ev.GetCol());
		wxGridCellTreeEditor* cellEditTreeCtrl=dynamic_cast<wxGridCellTreeEditor*>(cellEdit);
		if(cellEditTreeCtrl)
		{
			Element* newVal=cellEditTreeCtrl->GetSelection();
			if(newVal)
				SetValue(newVal);
		}else{
			ev.Veto();
		}
	}
private:
		
	std::list<Element::ELEMENT_TYPE> showElementsFilter;
	void SetGridValue( wxGrid* gridToFeed, int row, int col=0)
	{
		if(userChoice)
			gridToFeed->SetCellValue(row,col,userChoice->GetElementInfos().libelleElement);
	}
};

#endif