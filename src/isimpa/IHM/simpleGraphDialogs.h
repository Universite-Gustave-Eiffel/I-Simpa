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

#include "simpleGraph.h"
#include "PropGrid.h"

#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>
#include <wx/spinctrl.h>
#include "simpleGraphEnumerators.h"

#ifndef __SIMPLEGRAPH_DIALOGS__
#define __SIMPLEGRAPH_DIALOGS__

namespace sgSpace
{

	struct MAIN_PROPERTY_GRID_ELEMENT;
	WX_DECLARE_OBJARRAY(MAIN_PROPERTY_GRID_ELEMENT, ArrayOfProperties);
	struct t_CurrentModification;
	WX_DECLARE_OBJARRAY(t_CurrentModification, ArrayOfElementsModification);


	/**
	 * @brief Fenetre de configuration de rendu du graphique
	 */
	class SG_PropertySheetDialog : public wxPropertySheetDialog
	{
		DECLARE_CLASS(SG_PropertySheetDialog)
	public:
		SG_PropertySheetDialog(simpleGraph* parent, wxWindowID id,
                       const wxString& title,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& sz = wxDefaultSize);
		/**
		 * Dessine l'objet courant dans la zone de dessin passé en paramètre
		 * @see RefreshSample
		 */
		void DrawSample(wxDC* drawingAreaContext);
	protected:
		PropGrid* generalPropGrid;

		///////////////////////////////////
		//Ajout des contrôles
		wxPanel* CreateGeneralSettingsPage(wxWindow* parent);
		wxPanel* CreateCurvesSettingsPage(wxWindow* parent);
		void AddControlsToGeneralSubPane(wxWindow* collapsidePaneArea);
		void AddControlsToPenSubPane(wxWindow* collapsidePaneArea);
		void AddControlsToBrushSubPane(wxWindow* collapsidePaneArea);

		wxButton* ApplyButton;
		///////////////////////////////////
		// Intialisation des valeurs
		void FillGeneralPropGrid();
		void InitCurveControls();
		void LoadCurveElement(t_CurrentModification* eleToLoad);


		/////////////////////////////////////////////
		// Gestion des événements générés par l'utilisateur
		void OnOK(wxCommandEvent& event);
		void OnApply(wxCommandEvent& event);
		void OnCellValueChanged(wxGridEvent& ev);
		void OnSelectingAnotherCurve( wxCommandEvent& ev);
		void OnUserChooseFont(wxCommandEvent& ev);
		void OnUserChooseDrawingMethod(wxCommandEvent& ev);
		void OnUserChooseMarker(wxCommandEvent& ev);
		void OnUserChangeLabel(wxCommandEvent& ev);
		void OnUserChangeTextRotation(wxSpinEvent& ev);
		void OnUserChoosePenColor(wxCommandEvent& ev);
		void OnUserChoosePenStyle(wxCommandEvent& ev);
		void OnUserChoosePenStippleBitmap(wxCommandEvent& ev);
		void OnUserChangePenWidth(wxSpinEvent& ev);
		void OnUserChooseBrushColor(wxCommandEvent& ev);
		void OnUserChooseBrushStippleBitmap(wxCommandEvent& ev);
		void OnUserChooseBrushStyle(wxCommandEvent& ev);
		void OnUserChooseVisibility(wxCommandEvent& ev);








		void PushGeneralSetting(I_PARAM paramIndex,wxString paramLabel);
		void PushGeneralSetting(B_PARAM paramIndex,wxString paramLabel);
		void PushGeneralSetting(F_PARAM paramIndex,wxString paramLabel);

		void GetGeneralSettingValue(const MAIN_PROPERTY_GRID_ELEMENT& paramSettings, wxString& sValue);

		void RefreshSample();
		void ApplyModifications();
		void OnCurveChange(); //Marque l'objet de dessin ouvert comme modifié
		simpleGraph* linkedGraph;
		ArrayOfProperties propertyArray;
		ArrayOfElementsModification elementsModifications; //Ca tableau contient les états intermédiaire des tracés.
		wxInt32 selectedElementIndice; //numero de ligne séléctionné dans le tableau de elementsModifications
		StyleArray drawingMethodStyles;
		StyleArray penStyles;
		StyleArray brushStyles;
		StyleArray markersStyles;


		DECLARE_EVENT_TABLE()
	};
}

#endif