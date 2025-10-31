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
#include <wx/window.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <vector>
#include <wx/treectrl.h>
#include <wx/timer.h>
#include "../data_manager/element.h"

#ifndef __uiTreeCtrl_H__
#define __uiTreeCtrl_H__


/** 
 * @file uiTreeCtrl.h
 * @brief Déclaration de la classe uiTreeCtrl
 */

/**
 * @class uiTreeCtrl
 * @brief Classe de gestion de l'affichage des éléments sous forme hiérarchique
 *
 * Surcharge de wxTreeCtrl afin d'intéragir avec les instances de la classe Element 
 */
wxDECLARE_EVENT(MENU_ITEM_SELECTED, wxCommandEvent);

class uiTreeCtrl : public wxTreeCtrl
{
	private : 
		void (*pointeurElementEvent)(wxCommandEvent&);
		bool selectionEventState; //A faux les évenements générés par les éléments sont désactivés
		std::vector<Element*> DragLst;
		wxTimer m_Timer;
		struct t_highlightEl
		{
			wxTreeItemId el;
			float compteurHl;
			int scrollPos;
		};
		t_highlightEl highlightEl;
		void OnTimer( wxTimerEvent& event );
		void OnMouseMove(wxMouseEvent& event);
		wxPoint mousePos;
		void InitTree();
		smart_ptr<bool> alive;
		const bool openOnSimpleClick;
	protected :

		DECLARE_EVENT_TABLE()

		Element* rootItem;
		void BeginDrag(wxTreeEvent&);
		void OnDoubleClic(wxTreeEvent&);
		void OnRightClic(wxTreeEvent&);
		void OnLeftClic(wxMouseEvent&); 
		void OnSelectTreeItem(wxTreeEvent&);
		void EndDrag(wxTreeEvent&);
		void OnBeginLabelEdit(wxTreeEvent&);
		void OnEndLabelEdit(wxTreeEvent&);
		void OnExpanding(wxTreeEvent& treeEvent);
		void OnCollapsing(wxTreeEvent& treeEvent);
		void OnKeyUp(wxKeyEvent&); 
        void OnMenuItemClosed(wxCommandEvent& commandEvent);
	public :
		/**
		 * Constructeur de la fenêtre
		 * @param parent Fenêtre parente
		 * @param id Indice de la fenêtre
		 * @param pos Position de la fenêtre
		 * @param size Taille de la fenêtre
		 * @param style Style de la fenêtre
		 */
		uiTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS, bool openOnSimpleClick = true);
		uiTreeCtrl();
		/**
		 * Destructeur
		 */
		~uiTreeCtrl();
		/**
		 * Lie l'événement validation de menu d'un Element à une méthode externe
		 * @param fevent Méthode externe
		 */
		void BindElementEvent(void (*fevent)(wxCommandEvent&));
		/**
		 * Evenement de clic sur un menu popup
		 * @param eventElement Objet lié à l'événement
		 */
		void OnElementEvent(wxCommandEvent&);
		/**
		 * Initialise le controle avec le premier élément
		 */
		void Init(Element* rootElement);
		/**
		 * Charge un nouvel élément au sein de l'arbre ( appelé par une instance de Element)
		 */
		void AddElement(Element* newElement);
		/**
		 * Cette surcharge désactive l'événement de sélection le temps de la suppression d'un élément dans l'arbre
		 * @param item Indice de l'objet supprimé
		 */
		virtual void Delete(const wxTreeItemId &item);
		/**
		 * Cette surcharge permet de recupérer l'élément sélectionné ou bien le dernier élément sélectionné par ceux sélectionnée
		 */
		virtual wxTreeItemId GetSelection() const;
		wxInt32 GetXmlId(const wxTreeItemId& itemId);
		Element* GetElement(const wxTreeItemId& itemId);
		/**
		 * Empêche les éléments de l'arbre de générer des évenements
		 */
		void LockElementEvent(bool lock=true) { selectionEventState=!lock; }
};

#endif