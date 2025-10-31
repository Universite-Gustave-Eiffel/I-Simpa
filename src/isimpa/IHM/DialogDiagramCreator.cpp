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

#include "DialogDiagramCreator.h"
#include "IHM/simpleGraphManager.h"
#include <wx/aui/auibook.h>
#include <wx/bookctrl.h>
#include <wx/wupdlock.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include "manager/stringTools.h"
#include "data_manager/appconfig.h"
#include "last_cpp_include.hpp"

/**
 * @brief Enumeration des controles dans le panneau de configuration
 */
enum ID_PANEL_CONTROL
{
	ID_PANEL_CONTROL_RADIO_DATA=700,
	ID_PANEL_CONTROL_CHECK_CONVERTX_INTO_NUMBER,
	ID_PANEL_CONTROL_SELECT_DEFAULT_STYLE,
	ID_PANEL_CONTROL_X_AXIS_LABEL,
	ID_PANEL_CONTROL_Y_AXIS_LABEL,
	ID_PANEL_CONTROL_TAB_LABEL
};

void FillComboBox( wxComboBox* lstBox, sgSpace::StyleArray* dataSrc )
{
	for(wxInt32 idligne=0;idligne<dataSrc->GetCount();idligne++)
	{
		lstBox->Append(dataSrc->Item(idligne).lblEnum);
	}
}
wxCheckBox* GetCheckBox( wxWindow* containerWin, ID_PANEL_CONTROL controlId )
{
	return wxDynamicCast(containerWin->FindWindow(controlId),wxCheckBox);
}
wxRadioBox* GetRadioBox( wxWindow* containerWin, ID_PANEL_CONTROL controlId )
{
	return wxDynamicCast(containerWin->FindWindow(controlId),wxRadioBox);
}
wxTextCtrl* GetTextCtrl( wxWindow* containerWin, ID_PANEL_CONTROL controlId )
{
	return wxDynamicCast(containerWin->FindWindow(controlId),wxTextCtrl);
}

wxComboBox* GetComboBox( wxWindow* containerWin, ID_PANEL_CONTROL controlId )
{
	return wxDynamicCast(containerWin->FindWindow(controlId),wxComboBox);
}
BEGIN_EVENT_TABLE(BuildGraphSheetDialog, wxPropertySheetDialog)
	EVT_BUTTON( wxID_OK , BuildGraphSheetDialog::OnOK )
END_EVENT_TABLE()

IMPLEMENT_CLASS(BuildGraphSheetDialog, wxPropertySheetDialog)

BuildGraphSheetDialog::BuildGraphSheetDialog(wxWindow* parent,wxWindow* auiNotebookParent, wxWindowID id,
                   const wxString& title,
                   const wxPoint& pos,
                   const wxSize& sz)
 : auiNotebook(auiNotebookParent)
{

    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);

    int resizeBorder = wxRESIZE_BORDER;


    Create(parent, id, title, pos, sz,
        wxDEFAULT_DIALOG_STYLE| (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, resizeBorder)
    );

	CreateButtons( wxOK | wxCANCEL );
    wxBookCtrlBase* notebook = GetBookCtrl();


    wxPanel* generalSettings = CreateMainSettingsPage(notebook);

    notebook->AddPage(generalSettings, wxGetTranslation("Configuration"), true);

    LayoutDialog();

}

void BuildGraphSheetDialog::LoadData(std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<float>& cellsValue)
{
	cols=lblCols;
	rows=lblRows;
	curveValues=cellsValue;
	//On place les valeurs par défaut des contrôles en fonction des données en paramètres

	wxRadioBox* rowcolRadio=GetRadioBox(this,ID_PANEL_CONTROL_RADIO_DATA);
	wxCheckBox* dataCheck=GetCheckBox(this,ID_PANEL_CONTROL_CHECK_CONVERTX_INTO_NUMBER);
	if(rowcolRadio && dataCheck)
	{
		//Par défaut en Histogramme
		dataCheck->SetValue(true);
		if(lblCols.size()>lblRows.size())
		{
			rowcolRadio->Select(1); // + de colonnes que de lignes donc une série de données par ligne
			if(lblCols.size()>30)
				dataCheck->SetValue(false);
		}else{
			rowcolRadio->Select(0); // + de lignes que de colonnes donc une série de données par colonne
			if(lblRows.size()>30)
				dataCheck->SetValue(false);
		}
	}
}

wxPanel* BuildGraphSheetDialog::CreateMainSettingsPage(wxWindow* parent)
{
	using namespace sgSpace;
	//Chargement des styles de tracés
	SG_EnumeratorFiller::FillStyleEnumeration(drawingMethodStyles);

    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *mainSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

	topSizer->Add(new wxStaticText(panel,wxID_ANY,wxGetTranslation("Tab name:")),0,wxALL,2);
	topSizer->Add(new wxTextCtrl(panel,ID_PANEL_CONTROL_TAB_LABEL,wxGetTranslation("User diagram")),0,wxALL,2);
	topSizer->Add(new wxStaticText(panel,wxID_ANY,wxGetTranslation("Name of X axis")),0,wxALL,2);
	topSizer->Add(new wxTextCtrl(panel,ID_PANEL_CONTROL_X_AXIS_LABEL,""),0,wxALL,2);
	topSizer->Add(new wxStaticText(panel,wxID_ANY,wxGetTranslation("Name of Y axis")),0,wxALL,2);
	topSizer->Add(new wxTextCtrl(panel,ID_PANEL_CONTROL_Y_AXIS_LABEL,""),0,wxALL,2);
	wxArrayString radioChoice;
	radioChoice.Add(wxGetTranslation("Column"));
	radioChoice.Add(wxGetTranslation("Lines"));
	topSizer->Add(new wxRadioBox(panel,ID_PANEL_CONTROL_RADIO_DATA,wxGetTranslation("Data alignment"),wxDefaultPosition,wxDefaultSize,radioChoice), 0, wxGROW|wxTOP, 2);
	topSizer->Add(new wxStaticText(panel,wxID_ANY,wxGetTranslation("Label:")),0,wxALL,2);
	wxCheckBox* convertCheck=new wxCheckBox(panel,ID_PANEL_CONTROL_CHECK_CONVERTX_INTO_NUMBER,wxGetTranslation("Text label for X"));
	topSizer->Add(convertCheck,0,wxALL,2);
	convertCheck->SetToolTip(wxGetTranslation("In text mode, X data are regularly spaced (unit step)")
		+wxString(wxGetTranslation("If this field is not activated, label are converted in numbers and applied on the X axis")));
	topSizer->Add(new wxStaticText(panel,wxID_ANY,wxGetTranslation("Default style")),0,wxALL,2);
	wxComboBox* lstStyles=new wxComboBox(panel,ID_PANEL_CONTROL_SELECT_DEFAULT_STYLE,"",wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_DROPDOWN );
	topSizer->Add(lstStyles,0,wxALL,2);
	FillComboBox(lstStyles,&drawingMethodStyles);
	lstStyles->SetValue(SG_EnumeratorFiller::FindStringWithIndex(&drawingMethodStyles,DRAWING_METHOD_DOT_CLOUD));

	mainSizer->Add(topSizer);
    panel->SetSizer(mainSizer);
    mainSizer->Fit(panel);

    return panel;
}

void BuildGraphSheetDialog::OnOK(wxCommandEvent& event)
{
	using namespace sgSpace;

	wxComboBox* drawingMethodCombo=GetComboBox(this,ID_PANEL_CONTROL_SELECT_DEFAULT_STYLE);
	if(!drawingMethodCombo)
		return;
	DRAWING_METHOD drawMethod=(DRAWING_METHOD)drawingMethodStyles.Item(drawingMethodCombo->GetSelection()).enumId;
	/////////////////////////////////////////////////////////////////////////
	// Création du graphique selon les paramètres utilisateurs

	wxRadioBox* rowcolRadio=GetRadioBox(this,ID_PANEL_CONTROL_RADIO_DATA);
	wxCheckBox* dataCheck=GetCheckBox(this,ID_PANEL_CONTROL_CHECK_CONVERTX_INTO_NUMBER);
	wxTextCtrl* textCtrlTab=GetTextCtrl(this,ID_PANEL_CONTROL_TAB_LABEL);
	wxTextCtrl* textCtrlXAxis=GetTextCtrl(this,ID_PANEL_CONTROL_X_AXIS_LABEL);
	wxTextCtrl* textCtrlYAxis=GetTextCtrl(this,ID_PANEL_CONTROL_Y_AXIS_LABEL);
	wxAuiNotebook* noteBookWin=wxDynamicCast(auiNotebook,wxAuiNotebook);

	if(noteBookWin && rowcolRadio && dataCheck && textCtrlXAxis && textCtrlYAxis)
	{
		wxString tabLabel=textCtrlTab->GetValue();
		wxString xLabel=textCtrlXAxis->GetValue();
		wxString yLabel=textCtrlYAxis->GetValue();
		bool DrawHistoGram=dataCheck->GetValue();
		bool DataSeriesInRows=(rowcolRadio->GetSelection()==1);

		wxWindowUpdateLocker lockWin(noteBookWin);
		MainSimpleGraphWindow* graphPage=new MainSimpleGraphWindow(noteBookWin,-1);
		graphPage->SetDefaultSaveGraphSavePath(dataFolder);
		//Ajout des données au graphique
		wxInt32 nbValues;
		wxInt32 nbCurves;
		std::vector<wxString>* Xlabels;
		std::vector<wxString>* curvesLegendLabels;
		if(DataSeriesInRows)
		{
			nbValues=cols.size();
			nbCurves=rows.size();
			Xlabels=&cols;
			curvesLegendLabels=&rows;
		}else{
			nbValues=rows.size();
			nbCurves=cols.size();
			Xlabels=&rows;
			curvesLegendLabels=&cols;
		}
		
		noteBookWin->AddPage(graphPage,tabLabel);

		if(DrawHistoGram)
		{
			///////////////////////////////////////////
			//La valeur des X n'est pas interprété


			////////////////////////////////////////
			// Ajout des courbes

			for(int idCurve=0;idCurve<nbCurves;idCurve++)
			{
				SG_Element_List* drawingEl=new SG_Element_List(graphPage,(*curvesLegendLabels)[idCurve],nbValues);
				drawingEl->el_style.SetDrawingMethod(drawMethod);
				drawingEl->el_style.Randomize();
				if(drawMethod==DRAWING_METHOD_DOT_CLOUD)
					drawingEl->el_style.RandomizeMarker();
				for(int idValue=0;idValue<nbValues;idValue++)
				{
					wxInt32 index;
					if(DataSeriesInRows)
					{
						index=(idValue*nbCurves)+idCurve;
					}else{
						index=(idCurve*nbValues)+idValue;
					}
					drawingEl->PushValue(curveValues[index]);
				}
			}

			////////////////////////////////////////
			// Ajout des Axes

			SG_Element_Axis* axeEly=new SG_Element_Axis(graphPage,wxVERTICAL,0.f,yLabel);
			axeEly->el_style.SetDrawingMethod(drawMethod); //Ainsi le style de l'axe dépend de la méthode de dessin général
			SG_Element_Axis_Labeled* axeElx=new SG_Element_Axis_Labeled(graphPage,wxHORIZONTAL,1.f,xLabel);
			axeElx->el_style.SetDrawingMethod(drawMethod); //Ainsi le style de l'axe dépend de la méthode de dessin général
			axeElx->el_style.SetTextRotation(90);
			for(int xx=0;xx<nbValues;xx++)
			{
				axeElx->PushLabel((*Xlabels)[xx]);
			}
		}else{
			///////////////////////////////////////////
			//La valeur des X doit être convertie
			////////////////////////////////////////
			// Ajout des courbes
			wxFloat32 yminValue=curveValues[0];
			wxFloat32 ymaxValue=curveValues[0];
			std::vector<wxFloat32> xxValues(nbValues);
			for(int xx=0;xx<nbValues;xx++)
			{
				wxString formatedLbl((*Xlabels)[xx].Trim());
				
				if(formatedLbl.Contains(" "))
					formatedLbl=formatedLbl.Left(formatedLbl.find(" "));
				xxValues[xx]=Convertor::ToFloat(formatedLbl);
			}

			for(int idCurve=0;idCurve<nbCurves;idCurve++)
			{
				SG_Element_Data* drawingEl=new SG_Element_Data(graphPage,(*curvesLegendLabels)[idCurve],nbValues);
				drawingEl->el_style.SetDrawingMethod(drawMethod);
				drawingEl->el_style.Randomize();
				if(drawMethod==DRAWING_METHOD_DOT_CLOUD)
					drawingEl->el_style.RandomizeMarker();
				if(idCurve==0) //Le premier style doit etre une ligne continue
				{
					wxPen firstCurvePen=drawingEl->el_style.GetPen();
					firstCurvePen.SetStyle(wxPENSTYLE_SOLID);
					drawingEl->el_style.SetPen(&firstCurvePen);
				}
				for(int idValue=0;idValue<nbValues;idValue++)
				{
					wxInt32 index;
					if(DataSeriesInRows)
					{
						index=(idValue*nbCurves)+idCurve;
					}else{
						index=(idCurve*nbValues)+idValue;
					}
					drawingEl->PushValue(xxValues[idValue],curveValues[index]);
				}
			}

			////////////////////////////////////////
			// Ajout des Axes
			SG_Element_Axis* axeEly=new SG_Element_Axis(graphPage,wxVERTICAL,0.f,yLabel);
			SG_Element_Axis* axeElx=new SG_Element_Axis(graphPage,wxHORIZONTAL,0.f,xLabel);
		}




		//Ajout de la légende
        SG_Legend::Add(new SG_Legend(graphPage, LEGEND_PLACEMENT_HORIZONTAL, -1), wxSizerFlags(0).Expand());

		//////////////
		graphPage->ZoomFit();
		graphPage->LoadConfig(ApplicationConfiguration::GetFileConfig(),ApplicationConfiguration::CONST_GRAPH_CONFIG_PATH,true);
	}
	event.Skip();
}
