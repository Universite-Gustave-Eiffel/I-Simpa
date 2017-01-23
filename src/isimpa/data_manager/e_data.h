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

#include "data_manager/element.h"
#include <wx/grid.h>
#include <wx/settings.h>
#ifndef __ELEMENT_DATA__
#define __ELEMENT_DATA__



/** \file e_data.h
   \brief Classe virtuelle spécialisant un élément afin qu'il puissent être visualisé dans l'élément MainPropGrid
*/

/**
	\brief Classe virtuelle spécialisant un élément afin qu'il puissent être visualisé dans l'élément MainPropGrid
	@see MainPropGrid
*/
class E_Data: public Element
{
	private:

	wxXmlNode* SaveXMLDataProperty(wxXmlNode* thisNode)
	{
		thisNode->DeleteAttribute("readonly");
		if(readOnly)
			thisNode->AddAttribute("readonly","1");	
		thisNode->DeleteAttribute("label");
		thisNode->AddAttribute("label",propertyLabel);
		//Maj valeur exportation vers le coeur de calcul
		thisNode->DeleteAttribute("ex");
		if(exportToCore)
			thisNode->AddAttribute("ex","1");
		return thisNode;
	}
protected:
	wxString propertyLabel;	/**< Libellé visible à gauche dans la feuille de propriété (MainPropGrid) */ 
	int hSize;				/**< Combien de cellules ce champ va occuper */ 
	bool onTop;				/**< Ce champ doit-il être visible en premier */ 
	/**
	 * Permet de connaître la couleur à utiliser lorsqu'un champ est actif ou non
	 * @param[in] enabled Vrai retourne la couleur du texte si le champ peut être édité
	 * @return Couleur du texte selon sont état
	 */
	wxColour GetTextColour(bool enabled)
	{
		if(!enabled)
			return wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
		else
			return wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);
	}
	bool exportToCore; /*!< Vrai si la propriété doit être exporté pour le coeur de calcul */
	bool readOnly;
public:
	/**
		Constructeur
		@param parent Element parent
		@param Nom Nom du champ ( non visible par l'utilisateur)
		@param Label Libellé du champ
		@param _type Type d'element
		@param nodeElement Si importation, pointe vers l'élément XML
		@param[in] asTitle Ce champ doit-il être visible en premier dans la liste des propriétés
		@param[in] _hSize Combien de cellules ce champ va occuper
	*/
	E_Data(Element* parent,wxString Nom,wxString Label,ELEMENT_TYPE _type,wxXmlNode* nodeElement=NULL, bool asTitle=false, int _hSize=1,bool _exportToCore=false)
		:Element(parent,Nom,_type,nodeElement)
	{
		
		readOnly=false;
		exportToCore=_exportToCore;
		hSize=_hSize;
		onTop=asTitle;
		wxString propVal;
		propertyLabel=Label;
		if(nodeElement!=NULL && nodeElement->GetAttribute("wxid",&propVal))
		{
			//Element initialisé AVEC Xml
	
			if(nodeElement->GetAttribute("readonly",&propVal))
				readOnly=true;
			if(nodeElement->GetAttribute("label",&propVal) && propVal!="")
				propertyLabel=propVal;
			if(nodeElement->GetAttribute("ex",&propVal))
				exportToCore=true;
		}else{
			//Modified(this); //Ajout de la propriété en cours d'execution
		}
	}
	/*
	 * Retourne La chaîne de caractère utilisé pour comparer l'ordre d'apparition du champ dans la grille de propriétés
	 */
	virtual const wxString GetElementNameForOrder() const
	{
		wxString add;
		if(onTop)
			add="000";
		if(this->elementInfo.libelleElement.IsNumber())
			return this->elementInfo.libelleElement;
		else
			return add+_(propertyLabel);
	}
	/**
	 * Passe la valeur de cet élément dans la grille de données
	 * @param gridToFeed Pointeur vers le contrôle grille
	 * @param col Indique quel colonne renseigner ( la ligne est la dernière du contrôle )
	 */
	virtual void FillWxGrid(wxGrid* gridToFeed,int col=0)
	{
		int row=gridToFeed->GetNumberRows()-1;
		gridToFeed->SetRowLabelValue(row,_(propertyLabel));
		if(hSize>gridToFeed->GetNumberCols())
			gridToFeed->AppendCols(hSize-gridToFeed->GetNumberCols());
		if(col>=0)
		{
			gridToFeed->SetCellSize(row,col,1,hSize);
			gridToFeed->SetReadOnly(row,col,readOnly);
			gridToFeed->SetCellTextColour(row,col,GetTextColour(readOnly));
		}
	}
	/**
	 * Lors de la mise à jour du champ par l'utilisateur (Après validation par l'utilisateur) cette méthode est appelée 
	 * @param ev Objet correspondant aux propriétés de l'évenement de modification
	 * @param gridCtrl Contrôle sur lequel la mise à jour c'est effectuée
	 */
	virtual void OnValueChanged(wxGridEvent& ev,wxGrid* gridCtrl)
	{

	}
	/**
	 * Met à jour la valeur de la cellule avec la valeur de l'élément
	 * @param gridCtrl Contrôle sur lequel la mise à jour c'est effectuée
	 * @param col Indique quel colonne renseigner
	 * @param row Indique quel ligne renseigner
	 */
	virtual void UpdateValue( wxGrid* gridCtrl, int row, int col=0)
	{
		if(col>=0 && gridCtrl->IsReadOnly(row,col)!=readOnly)
		{
			gridCtrl->SetReadOnly(row,col,readOnly);
			gridCtrl->SetCellTextColour(row,col,GetTextColour(readOnly));
		}
	}
	/**
	 * Cette méthode permet d'avoir la version traduite du libellé du champ
	 */
	virtual wxString GetPropertyLabel()
	{
		return _(propertyLabel);
	}

	/**
	 * Update the non-translated value of label
	 */
	virtual void SetPropertyLabel(const wxString& label) {
		propertyLabel = label;
	}

	/**
	 * Cette méthode est surchargé afin de ne pas afficher cet élément dans l'arbre du projet
	 */
	
	void FillWxTree(wxTreeCtrl* treeToFeed,const wxTreeItemId& parentId)
	{
		//Ne pas afficher cet élément dans l'arbre
	}
	
	/**
	 * Bloque/Active la possibilitée de mise à jour du champ par l'utilisateur
	 * @param readonly A vrai(defaut) le champ passe en lecture seule
	 * @param col Indique quel colonne du champ doit être mis en lecture seule.
	 */
	virtual void SetReadOnly(bool readonly=true,int col=0) 
	{
		if(this->readOnly!=readonly)
		{
			this->readOnly=readonly;
			Modified(this);
		}
	}
	/**
	 * @return Etat des droits de modification de l'utilisateur. Vrai=en lecture seule
	 */
	virtual bool IsReadOnly(int col=-1)
	{
		return this->readOnly;
	}
	/**
	 * Active ou désactive l'exportation de cette propriété vers le fichier XML des coeurs de calcul
	 * @param newMode Vrai si cet élément doit être visible par les coeurs de calculs
	 */
	void SetXmlCoreVisibility(bool newMode)
	{
		exportToCore=newMode;
	}





	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("p"); // Nom de la balise xml ( pas d'espace autorise )
		return SaveXMLDataProperty(thisNode);
	}
	wxXmlNode* SaveXMLDocWithoutChildren(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDocWithoutChildren(NoeudParent);
		thisNode->SetName("p"); // Nom de la balise xml ( pas d'espace autorise )
		return SaveXMLDataProperty(thisNode);
	}

	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(exportToCore)
			return Element::SaveXMLCoreDoc(NoeudParent);
		else
			return NoeudParent;
	}
};




#endif