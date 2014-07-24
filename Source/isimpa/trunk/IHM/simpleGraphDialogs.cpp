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

#include "simpleGraphDialogs.h"
#include <wx/bookctrl.h>
#include <wx/arrimpl.cpp> 
#include <wx/wupdlock.h>
#include <wx/button.h>
#include <wx/collpane.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/statbox.h>
#include <wx/dcmemory.h>
#include <wx/fontdlg.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/image.h>
#include <wx/textctrl.h>
#include "last_cpp_include.hpp"


namespace sgSpace
{


	/**
	 * @brief Données temporaire indiquant l'etat transitoire entre l'etat actuel de l'élément et l'etat modifié.
	 */
	struct t_CurrentModification
	{
		t_CurrentModification( wxInt32 _elIndex,SG_El_Params _elStyle,const wxString& _label,bool _visible):elIndex(_elIndex),elStyle(_elStyle),label(_label),modified(false),visible(_visible){}
		wxInt32 elIndex;
		SG_El_Params elStyle;
		wxString label;
		bool visible;
		bool modified;
	};

	/**
	 * @brief Enumeration des type de données des propriétés générales
	 */
	enum MAIN_PROPERTY_TYPE
	{
		MAIN_PROPERTY_TYPE_INTEGER,
		MAIN_PROPERTY_TYPE_BOOL,
		MAIN_PROPERTY_TYPE_FLOAT
	};
	/**
	 * @brief Cette structure définit une ligne de propriétés
	 */
	struct MAIN_PROPERTY_GRID_ELEMENT
	{
		MAIN_PROPERTY_GRID_ELEMENT(MAIN_PROPERTY_TYPE _propertyType,wxInt32 _propertyIndex,const wxString& _propertyLabel):propertyType(_propertyType),propertyIndex(_propertyIndex),propertyLabel(_propertyLabel){}
		MAIN_PROPERTY_TYPE propertyType;	/*!< Type de base de la propritété */
		wxInt32 propertyIndex;				/*!< Indice de la propriété (ex.I_PARAM_Y_BORDER_AXIS) */
		wxString propertyLabel;				/*!< Libellé de la propriété visible dans la grille */
	};

	/**
	 * @brief Enumeration des controles dans le panneau de configuration
	 */
	enum ID_CURVE_CONTROL
	{
		ID_CURVE_CONTROL_VISIBILITY=700,
		ID_CURVE_CONTROL_CURVE_CHOICE,
		ID_CURVE_CONTROL_FONT_BUTTON,
		ID_CURVE_CONTROL_DRAWING_METHOD,
		ID_CURVE_CONTROL_MARKER,
		ID_CURVE_CONTROL_LABEL,
		ID_CURVE_CONTROL_TEXT_ROTATION,
		ID_CURVE_CONTROL_PEN_COLOR_BUTTON,
		ID_CURVE_CONTROL_PEN_STYLE_LIST,
		ID_CURVE_CONTROL_PEN_BITMAP_STIPPLE_BUTTON,
		ID_CURVE_CONTROL_PEN_WIDTH,
		ID_CURVE_CONTROL_BRUSH_COLOR_BUTTON,
		ID_CURVE_CONTROL_BRUSH_BITMAP_STIPPLE_BUTTON,
		ID_CURVE_CONTROL_BRUSH_STYLE_LIST,
		ID_CURVE_CONTROL_SAMPLE_AREA
	};

	WX_DEFINE_OBJARRAY(ArrayOfProperties);
	WX_DEFINE_OBJARRAY(ArrayOfElementsModification);
	//WX_DEFINE_OBJARRAY(StyleArray);


//////////////////////////////////////////////////////////////
///////////// SG_SampleArea
	class SG_SampleArea : public wxControl
	{
	public:
		SG_SampleArea(wxWindow *parent,SG_PropertySheetDialog* _mainParent, wxWindowID id,
						const wxString& label,
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = 0,
						const wxString& name = wxStaticBoxNameStr )
						:wxControl(parent,id,pos,size,style,wxDefaultValidator,name),
						bestSize(size),
						mainParent(_mainParent)
		{
			_("OK");
			_("Cancel");
			wxSize clientSize=this->GetClientSize();
			areaData.Create(clientSize.GetWidth(),clientSize.GetHeight());
			currentSample.SelectObject(areaData);
			currentSample.SetBrush(*wxWHITE_BRUSH);
			currentSample.Clear();
		}

		wxSize DoGetBestSize() const
		{
			return bestSize;
		}
		void RefreshDrawingArea()
		{
			//Redimensionne l'élément chargé si nécessaire
			wxSize sampleSize=currentSample.GetSize();
			wxSize clientSize=this->GetClientSize();
			if(clientSize!=sampleSize)
			{
				currentSample.SelectObject(wxNullBitmap);
				areaData.Create(clientSize.GetWidth(),clientSize.GetHeight());
				currentSample.SelectObject(areaData);
				sampleSize=currentSample.GetSize();
			}
			if(mainParent)
			{
				mainParent->DrawSample(&currentSample);
			}
		}

		
		void OnPaint( wxPaintEvent &event )
		{
			wxPaintDC drawContext(this);
			if(currentSample.IsOk())
			{
				wxSize drawSize=currentSample.GetSize();
				wxSize clientSize=this->GetClientSize();
				if(drawSize!=clientSize)
				{
					RefreshDrawingArea();
				}
				drawSize=currentSample.GetSize();
				drawContext.Blit(0,0,drawSize.GetWidth(),drawSize.GetHeight(),&currentSample,0,0);
			}
		}
	private:
		SG_PropertySheetDialog* mainParent;
		wxSize bestSize;
		wxBitmap areaData;
		wxMemoryDC currentSample;
		DECLARE_EVENT_TABLE()
	};

	BEGIN_EVENT_TABLE(SG_SampleArea, wxControl)
		EVT_PAINT( SG_SampleArea::OnPaint)
	END_EVENT_TABLE()
//////////////////////////////////////////////////////////////
///////////// Méthodes statiques
	void FillComboBox( wxComboBox* lstBox, StyleArray* dataSrc )
	{
		for(wxInt32 idligne=0;idligne<dataSrc->GetCount();idligne++)
		{
			lstBox->Append(dataSrc->Item(idligne).lblEnum);
		}
	}


	wxComboBox* GetComboBox( wxWindow* containerWin, ID_CURVE_CONTROL controlId )
	{
		return wxDynamicCast(containerWin->FindWindow(controlId),wxComboBox);
	}
	wxCheckBox* GetCheckBox( wxWindow* containerWin, ID_CURVE_CONTROL controlId )
	{
		return wxDynamicCast(containerWin->FindWindow(controlId),wxCheckBox);
	}
	wxSpinCtrl* GetSpinCtrl( wxWindow* containerWin, ID_CURVE_CONTROL controlId )
	{
		return wxDynamicCast(containerWin->FindWindow(controlId),wxSpinCtrl);
	}
	wxTextCtrl* GetTextCtrl( wxWindow* containerWin, ID_CURVE_CONTROL controlId )
	{
		return wxDynamicCast(containerWin->FindWindow(controlId),wxTextCtrl);
	}
	SG_SampleArea* GetStaticBox( wxWindow* containerWin, ID_CURVE_CONTROL controlId )
	{
		return wxDynamicCast(containerWin->FindWindow(controlId),SG_SampleArea);
	} 
IMPLEMENT_CLASS(SG_PropertySheetDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(SG_PropertySheetDialog, wxPropertySheetDialog)
	EVT_GRID_CELL_CHANGE( SG_PropertySheetDialog::OnCellValueChanged )
	EVT_BUTTON( wxID_APPLY , SG_PropertySheetDialog::OnApply )
	EVT_BUTTON( wxID_OK , SG_PropertySheetDialog::OnOK )
	//2eme onglet
	EVT_COMBOBOX( ID_CURVE_CONTROL_CURVE_CHOICE, SG_PropertySheetDialog::OnSelectingAnotherCurve )
	EVT_BUTTON ( ID_CURVE_CONTROL_FONT_BUTTON, SG_PropertySheetDialog::OnUserChooseFont )
	EVT_COMBOBOX ( ID_CURVE_CONTROL_DRAWING_METHOD, SG_PropertySheetDialog::OnUserChooseDrawingMethod )
	EVT_COMBOBOX ( ID_CURVE_CONTROL_MARKER, SG_PropertySheetDialog::OnUserChooseMarker )
	EVT_TEXT ( ID_CURVE_CONTROL_LABEL, SG_PropertySheetDialog::OnUserChangeLabel )
	EVT_SPINCTRL( ID_CURVE_CONTROL_TEXT_ROTATION, SG_PropertySheetDialog::OnUserChangeTextRotation )
	EVT_BUTTON( ID_CURVE_CONTROL_PEN_COLOR_BUTTON , SG_PropertySheetDialog::OnUserChoosePenColor )
	EVT_COMBOBOX( ID_CURVE_CONTROL_PEN_STYLE_LIST, SG_PropertySheetDialog::OnUserChoosePenStyle )
	EVT_BUTTON( ID_CURVE_CONTROL_PEN_BITMAP_STIPPLE_BUTTON , SG_PropertySheetDialog::OnUserChoosePenStippleBitmap )
	EVT_SPINCTRL( ID_CURVE_CONTROL_PEN_WIDTH, SG_PropertySheetDialog::OnUserChangePenWidth )
	EVT_BUTTON( ID_CURVE_CONTROL_BRUSH_COLOR_BUTTON , SG_PropertySheetDialog::OnUserChooseBrushColor )
	EVT_BUTTON( ID_CURVE_CONTROL_BRUSH_BITMAP_STIPPLE_BUTTON , SG_PropertySheetDialog::OnUserChooseBrushStippleBitmap )
	EVT_COMBOBOX( ID_CURVE_CONTROL_BRUSH_STYLE_LIST, SG_PropertySheetDialog::OnUserChooseBrushStyle )
	EVT_CHECKBOX( ID_CURVE_CONTROL_VISIBILITY, SG_PropertySheetDialog::OnUserChooseVisibility )
END_EVENT_TABLE()



int SortByPropertyLabels(MAIN_PROPERTY_GRID_ELEMENT **first, MAIN_PROPERTY_GRID_ELEMENT **second)
{
	return (*first)->propertyLabel.CmpNoCase((*second)->propertyLabel);
}



SG_PropertySheetDialog::SG_PropertySheetDialog(simpleGraph* parent, wxWindowID id,
                       const wxString& title,
                       const wxPoint& pos,
                       const wxSize& sz)
:linkedGraph(parent)
{
	generalPropGrid=NULL;
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);

    int resizeBorder = wxRESIZE_BORDER;


    Create(parent, id, title, pos, sz,
        wxDEFAULT_DIALOG_STYLE| (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, resizeBorder)
    );

	wxSizer *buttonSizer = CreateButtonSizer( wxOK | wxCANCEL );
	ApplyButton=NULL;
    if( buttonSizer )
    {
		ApplyButton=new wxButton(m_innerSizer->GetContainingWindow(),wxID_APPLY,_("Apply"));
		ApplyButton->Disable();
		buttonSizer->Add( ApplyButton);
        m_innerSizer->Add( buttonSizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT|wxRIGHT, 2);
        m_innerSizer->AddSpacer(2);
    }

    wxBookCtrlBase* notebook = GetBookCtrl();


    wxPanel* generalSettings = CreateGeneralSettingsPage(notebook);
    wxPanel* curvesSettings = CreateCurvesSettingsPage(notebook);

    notebook->AddPage(generalSettings, _("General parameters"), true);
    notebook->AddPage(curvesSettings, _("Data style"), false);

    LayoutDialog();
}

void SG_PropertySheetDialog::OnOK(wxCommandEvent& event)
{
	ApplyModifications();
	event.Skip();
}

void SG_PropertySheetDialog::OnApply(wxCommandEvent& event)
{
	ApplyModifications();
	ApplyButton->Disable();
}

void SG_PropertySheetDialog::OnCellValueChanged(wxGridEvent& ev)
{
	//L'utilisateur a modifié une valeur
	ApplyButton->Enable();
}

void SG_PropertySheetDialog::OnSelectingAnotherCurve( wxCommandEvent& ev)
{
	wxComboBox* lstCurves=GetComboBox(this,ID_CURVE_CONTROL_CURVE_CHOICE);
	if(lstCurves)
	{
		wxInt32 selected=lstCurves->GetSelection();
		if(selected!=wxNOT_FOUND)
		{
			selectedElementIndice=selected;
			LoadCurveElement(&elementsModifications.Item(selectedElementIndice));
		}
	}
}
void SG_PropertySheetDialog::OnCurveChange()
{
	elementsModifications.Item(selectedElementIndice).modified=true;
	RefreshSample();
	ApplyButton->Enable();
}
void SG_PropertySheetDialog::OnUserChooseFont(wxCommandEvent& ev)
{
	wxFontData curveFont;
	curveFont.SetInitialFont(elementsModifications.Item(selectedElementIndice).elStyle.GetFont());
	curveFont.SetColour(elementsModifications.Item(selectedElementIndice).elStyle.GetTextForeGroundColor());
	wxFontDialog fontDialogChooser(this,curveFont);
	if(fontDialogChooser.ShowModal()==wxID_OK)
	{
		wxFontData retData = fontDialogChooser.GetFontData();
		elementsModifications.Item(selectedElementIndice).elStyle.SetFont(retData.GetChosenFont());
		elementsModifications.Item(selectedElementIndice).elStyle.SetTextForeGroundColor(retData.GetColour());
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChooseDrawingMethod(wxCommandEvent& ev)
{
	wxComboBox* drawingMethodCombo=GetComboBox(this,ID_CURVE_CONTROL_DRAWING_METHOD);
	if(drawingMethodCombo)
	{
		elementsModifications.Item(selectedElementIndice).elStyle.SetDrawingMethod((DRAWING_METHOD)drawingMethodStyles.Item(drawingMethodCombo->GetSelection()).enumId);
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChooseMarker(wxCommandEvent& ev)
{
	wxComboBox* drawingMarkerCombo=GetComboBox(this,ID_CURVE_CONTROL_MARKER);
	if(drawingMarkerCombo)
	{
		elementsModifications.Item(selectedElementIndice).elStyle.SetDrawingMarker((DRAWING_MARKER)markersStyles.Item(drawingMarkerCombo->GetSelection()).enumId);
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChangeLabel(wxCommandEvent& ev)
{
	wxTextCtrl* labelCtrl=GetTextCtrl(this,ID_CURVE_CONTROL_LABEL);
	if(labelCtrl)
	{
		elementsModifications.Item(selectedElementIndice).label=labelCtrl->GetValue();
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChangeTextRotation(wxSpinEvent& ev)
{
	wxSpinCtrl* spinControl=GetSpinCtrl(this,ID_CURVE_CONTROL_TEXT_ROTATION);
	if(spinControl)
	{
		elementsModifications.Item(selectedElementIndice).elStyle.SetTextRotation((wxFloat32)spinControl->GetValue());
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChoosePenColor(wxCommandEvent& ev)
{
	wxPen currentPen=elementsModifications.Item(selectedElementIndice).elStyle.GetPen();
	wxColourData data;
	data.SetChooseFull(true);
	for (int i = 0; i < 16; i++)
	{
		wxColour colour(i*16, i*16, i*16);
		data.SetCustomColour(i, colour);
	}
	data.SetColour(currentPen.GetColour());
	wxColourDialog dialog(this, &data);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxColourData retData = dialog.GetColourData();
		currentPen.SetColour(retData.GetColour());
		elementsModifications.Item(selectedElementIndice).elStyle.SetPen(&currentPen);
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChoosePenStyle(wxCommandEvent& ev)
{
	wxComboBox* drawingPenStyleCombo=GetComboBox(this,ID_CURVE_CONTROL_PEN_STYLE_LIST);
	if(drawingPenStyleCombo)
	{
		wxPen drawingPen=elementsModifications.Item(selectedElementIndice).elStyle.GetPen();
		drawingPen.SetStyle(penStyles.Item(drawingPenStyleCombo->GetSelection()).enumId);
		elementsModifications.Item(selectedElementIndice).elStyle.SetPen(&drawingPen);
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChoosePenStippleBitmap(wxCommandEvent& ev)
{
	wxPen currentPen=elementsModifications.Item(selectedElementIndice).elStyle.GetPen();
	wxFileDialog openFileDialog( this, _("Load image"), "All image files|*.bmp;*.png;*.jpg;*.jpeg;*.gif;*.pcx;*.tiff;*.xpm;*.ico;*.cur;*.ani", "All image files|*.bmp;*.png;*.jpg;*.jpeg;*.gif;*.pcx;*.tiff;*.xpm;*.ico;*.cur;*.ani", _("All image files|*.bmp;*.png;*.jpg;*.jpeg;*.gif;*.pcx;*.tiff;*.xpm;*.ico;*.cur;*.ani"));
	if (openFileDialog.ShowModal() == wxID_OK)
	{
		wxString FileName(openFileDialog.GetPath());
		if(wxFileExists(FileName))
		{
			wxInitAllImageHandlers();
			wxImage loadedImage(FileName);
			if(loadedImage.IsOk())
			{
				wxBitmap bmpToLoad=loadedImage;
				currentPen.SetStipple(bmpToLoad);
				elementsModifications.Item(selectedElementIndice).elStyle.SetPen(&currentPen);
				OnCurveChange();
			}
		}
	}
}
void SG_PropertySheetDialog::OnUserChangePenWidth(wxSpinEvent& ev)
{
	wxSpinCtrl* spinControl=GetSpinCtrl(this,ID_CURVE_CONTROL_PEN_WIDTH);
	if(spinControl)
	{
		wxPen drawingPen=elementsModifications.Item(selectedElementIndice).elStyle.GetPen();
		drawingPen.SetWidth((wxInt32)spinControl->GetValue());
		elementsModifications.Item(selectedElementIndice).elStyle.SetPen(&drawingPen);
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChooseBrushColor(wxCommandEvent& ev)
{
	wxBrush currentBrush=elementsModifications.Item(selectedElementIndice).elStyle.GetBrush();
	wxColourData data;
	data.SetChooseFull(true);
	for (int i = 0; i < 16; i++)
	{
		wxColour colour(i*16, i*16, i*16);
		data.SetCustomColour(i, colour);
	}
	data.SetColour(currentBrush.GetColour());
	wxColourDialog dialog(this, &data);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxColourData retData = dialog.GetColourData();
		currentBrush.SetColour(retData.GetColour());
		elementsModifications.Item(selectedElementIndice).elStyle.SetBrush(&currentBrush);
		OnCurveChange();
	}
}
void SG_PropertySheetDialog::OnUserChooseBrushStippleBitmap(wxCommandEvent& ev)
{

	wxBrush currentBrush=elementsModifications.Item(selectedElementIndice).elStyle.GetBrush();
	wxFileDialog openFileDialog( this, _("Load image"), "All image files|*.bmp;*.png;*.jpg;*.jpeg;*.gif;*.pcx;*.tiff;*.xpm;*.ico;*.cur;*.ani", "All image files|*.bmp;*.png;*.jpg;*.jpeg;*.gif;*.pcx;*.tiff;*.xpm;*.ico;*.cur;*.ani", _("All image files|*.bmp;*.png;*.jpg;*.jpeg;*.gif;*.pcx;*.tiff;*.xpm;*.ico;*.cur;*.ani"));
	if (openFileDialog.ShowModal() == wxID_OK)
	{
		wxString FileName(openFileDialog.GetPath());
		if(wxFileExists(FileName))
		{
			wxInitAllImageHandlers();
			wxImage loadedImage(FileName);
			if(loadedImage.IsOk())
			{
				wxBitmap bmpToLoad=loadedImage;
				currentBrush.SetStipple(bmpToLoad);
				elementsModifications.Item(selectedElementIndice).elStyle.SetBrush(&currentBrush);
				OnCurveChange();
			}
		}
	}

}


void SG_PropertySheetDialog::OnUserChooseVisibility(wxCommandEvent& ev)
{
	wxCheckBox* checkVisible=GetCheckBox(this,ID_CURVE_CONTROL_VISIBILITY);
	if(checkVisible)
	{
		elementsModifications.Item(selectedElementIndice).visible=checkVisible->GetValue();
		OnCurveChange();
	}
}

void SG_PropertySheetDialog::OnUserChooseBrushStyle(wxCommandEvent& ev)
{
	wxComboBox* drawingBrushStyleCombo=GetComboBox(this,ID_CURVE_CONTROL_BRUSH_STYLE_LIST);
	if(drawingBrushStyleCombo)
	{
		wxBrush drawingBrush=elementsModifications.Item(selectedElementIndice).elStyle.GetBrush();
		drawingBrush.SetStyle(brushStyles.Item(drawingBrushStyleCombo->GetSelection()).enumId);
		elementsModifications.Item(selectedElementIndice).elStyle.SetBrush(&drawingBrush);
		OnCurveChange();
	}
}

wxPanel* SG_PropertySheetDialog::CreateGeneralSettingsPage(wxWindow* parent)
{

    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );
	
    wxBoxSizer* itemSizer3 = new wxBoxSizer( wxHORIZONTAL );

	generalPropGrid=new PropGrid(panel,-1);
	generalPropGrid->AppendCols();
	FillGeneralPropGrid();

    itemSizer3->Add(generalPropGrid, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(itemSizer3, 0, wxGROW|wxALL, 0);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

void SG_PropertySheetDialog::FillGeneralPropGrid()
{

	//Ajout des paramètres généraux
	PushGeneralSetting(B_PARAM_USE_ANTIALIASING,_("Anti-aliasing"));

	PushGeneralSetting(B_PARAM_HIDE_LEGEND,_("Hide legend"));

	//PushGeneralSetting(I_PARAM_BORDER_TITLE,_("Titre, Bordure"));
	//PushGeneralSetting(B_PARAM_TITLE_IS_VISIBLE,_("Titre, Visible"));

	PushGeneralSetting(I_PARAM_TIC_SIZE,_("Axis, dash size"));
	PushGeneralSetting(I_PARAM_X_BORDER_AXIS,_("X axis, line"));
	PushGeneralSetting(I_PARAM_Y_BORDER_AXIS,_("Y axis, line"));

	PushGeneralSetting(I_PARAM_LEGEND_ICON_SIZE,_("Legend, icon length"));



	PushGeneralSetting(F_PARAM_X_MIN,_("Scale, x minimum value"));
	PushGeneralSetting(F_PARAM_X_MAX,_("Scale, x maximum value"));
	PushGeneralSetting(F_PARAM_Y_MIN,_("Scale, y minimum value"));
	PushGeneralSetting(F_PARAM_Y_MAX,_("Scale, y maximum value"));
	PushGeneralSetting(F_PARAM_Y_TIC,_("Scale Y"));
	PushGeneralSetting(F_PARAM_X_TIC,_("Scale X"));
	PushGeneralSetting(F_PARAM_MARKER_SIZE,_("Marker, size (%)"));







	//Trie selon les libellés
	propertyArray.Sort(SortByPropertyLabels);

	//Insertion des lignes
	wxWindowUpdateLocker paintFreeze(generalPropGrid);
	generalPropGrid->SetLabelAlignment(wxVERTICAL,wxALIGN_LEFT);
	generalPropGrid->AppendRows(propertyArray.GetCount());
	
	generalPropGrid->SetLabelValue(wxHORIZONTAL,_("Data"),0);
	for(int idRow=0;idRow<propertyArray.GetCount();idRow++)
	{
		generalPropGrid->SetLabelValue(wxVERTICAL,propertyArray.Item(idRow).propertyLabel,idRow);
		if(propertyArray.Item(idRow).propertyType==MAIN_PROPERTY_TYPE_BOOL)
		{
			generalPropGrid->SetCellEditor(idRow, 0, new wxGridCellBoolEditor());
			generalPropGrid->SetCellRenderer(idRow,0,new wxGridCellBoolRenderer());
		}else if(propertyArray.Item(idRow).propertyType==MAIN_PROPERTY_TYPE_INTEGER)
		{
			generalPropGrid->SetCellEditor(idRow, 0, new wxGridCellNumberEditor());
			generalPropGrid->SetCellRenderer(idRow,0,new wxGridCellNumberRenderer());
		}else if(propertyArray.Item(idRow).propertyType==MAIN_PROPERTY_TYPE_FLOAT)
		{
			generalPropGrid->SetCellEditor(idRow, 0, new wxGridCellFloatEditor());
			generalPropGrid->SetCellRenderer(idRow,0,new wxGridCellFloatRenderer());
		}
		wxString propValue;
		GetGeneralSettingValue(propertyArray.Item(idRow),propValue);
		generalPropGrid->SetCellValue(idRow,0,propValue);
		generalPropGrid->AutoSizeLibelle(idRow);
	}

	generalPropGrid->AutoSizeColumns(false);
	generalPropGrid->SetRowLabelSize(wxGRID_AUTOSIZE);
	generalPropGrid->SetColLabelSize(wxGRID_AUTOSIZE);
}

void SG_PropertySheetDialog::PushGeneralSetting(I_PARAM paramIndex,wxString paramLabel)
{
	propertyArray.Add(MAIN_PROPERTY_GRID_ELEMENT(MAIN_PROPERTY_TYPE_INTEGER,paramIndex,paramLabel));
}
void SG_PropertySheetDialog::PushGeneralSetting(B_PARAM paramIndex,wxString paramLabel)
{
	propertyArray.Add(MAIN_PROPERTY_GRID_ELEMENT(MAIN_PROPERTY_TYPE_BOOL,paramIndex,paramLabel));
}
void SG_PropertySheetDialog::PushGeneralSetting(F_PARAM paramIndex,wxString paramLabel)
{
	propertyArray.Add(MAIN_PROPERTY_GRID_ELEMENT(MAIN_PROPERTY_TYPE_FLOAT,paramIndex,paramLabel));
}

void SG_PropertySheetDialog::GetGeneralSettingValue(const MAIN_PROPERTY_GRID_ELEMENT& paramSettings, wxString& sValue)
{
	sValue.clear();
	switch(paramSettings.propertyType)
	{
		case MAIN_PROPERTY_TYPE_BOOL:
			if(linkedGraph->parameters.isEnable((B_PARAM)paramSettings.propertyIndex))
				sValue="1";
			break;
		case MAIN_PROPERTY_TYPE_INTEGER:
			sValue=wxString::Format("%i",linkedGraph->parameters.GetCfg((I_PARAM)paramSettings.propertyIndex));
			break;
		case MAIN_PROPERTY_TYPE_FLOAT:
			sValue=wxString::Format("%g",linkedGraph->parameters.GetCfg((F_PARAM)paramSettings.propertyIndex));
			break;
	}
}

void SG_PropertySheetDialog::AddControlsToGeneralSubPane(wxWindow* collapsidePaneArea)
{
    wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);
	paneSz->Add(new wxCheckBox(collapsidePaneArea, ID_CURVE_CONTROL_VISIBILITY, _("Visible on/off")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxButton(collapsidePaneArea,ID_CURVE_CONTROL_FONT_BUTTON, _("Character font")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxStaticText(collapsidePaneArea,wxID_ANY,_("Drawing method")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxComboBox(collapsidePaneArea,ID_CURVE_CONTROL_DRAWING_METHOD,"",wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_DROPDOWN ), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxStaticText(collapsidePaneArea,wxID_ANY,_("Marker")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxComboBox(collapsidePaneArea,ID_CURVE_CONTROL_MARKER,"",wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_DROPDOWN ), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxStaticText(collapsidePaneArea,wxID_ANY,_("Labelled")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxTextCtrl(collapsidePaneArea,ID_CURVE_CONTROL_LABEL), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxStaticText(collapsidePaneArea,wxID_ANY,_("Text rotation (degree)")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxSpinCtrl(collapsidePaneArea,ID_CURVE_CONTROL_TEXT_ROTATION,wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -180, 180,  0), 1, wxGROW|wxTOP, 2);
    collapsidePaneArea->SetSizer(paneSz);
    paneSz->SetSizeHints(collapsidePaneArea);
}
void SG_PropertySheetDialog::AddControlsToPenSubPane(wxWindow* collapsidePaneArea)
{
    wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);
	paneSz->Add(new wxButton(collapsidePaneArea,ID_CURVE_CONTROL_PEN_COLOR_BUTTON, _("Color")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxButton(collapsidePaneArea,ID_CURVE_CONTROL_PEN_BITMAP_STIPPLE_BUTTON, _("Loading image ...")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxStaticText(collapsidePaneArea,wxID_ANY,_("Drawing style")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxComboBox(collapsidePaneArea,ID_CURVE_CONTROL_PEN_STYLE_LIST,"",wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_DROPDOWN ), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxStaticText(collapsidePaneArea,wxID_ANY,_("Line thickness")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxSpinCtrl(collapsidePaneArea,ID_CURVE_CONTROL_PEN_WIDTH,wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 5000,  1), 1, wxGROW|wxTOP, 2);
    collapsidePaneArea->SetSizer(paneSz);
    paneSz->SetSizeHints(collapsidePaneArea);
}

void SG_PropertySheetDialog::AddControlsToBrushSubPane(wxWindow* collapsidePaneArea)
{
    wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);
	paneSz->Add(new wxButton(collapsidePaneArea,ID_CURVE_CONTROL_BRUSH_COLOR_BUTTON, _("Color")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxButton(collapsidePaneArea,ID_CURVE_CONTROL_BRUSH_BITMAP_STIPPLE_BUTTON, _("Loading image ...")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxStaticText(collapsidePaneArea,wxID_ANY,_("Filling style")), 1, wxGROW|wxTOP, 2);
	paneSz->Add(new wxComboBox(collapsidePaneArea,ID_CURVE_CONTROL_BRUSH_STYLE_LIST,"",wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_DROPDOWN ), 1, wxGROW|wxTOP, 2);
    collapsidePaneArea->SetSizer(paneSz);
    paneSz->SetSizeHints(collapsidePaneArea);
}
wxPanel* SG_PropertySheetDialog::CreateCurvesSettingsPage(wxWindow* parent)
{

    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

	///////////////////////
	// Ajout du choix des éléments de dessin
	topSizer->AddSpacer(5);
	topSizer->Add(new wxStaticText(panel,wxID_ANY,_("Drawing element:")));
	topSizer->Add(new wxComboBox(panel,ID_CURVE_CONTROL_CURVE_CHOICE,"",wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_DROPDOWN ), 0, wxGROW|wxALL, 5 );

	///////////////////////
	// Ajout de l'apercu
	//topSizer->Add(new wxStaticText(panel,wxID_ANY,_("Aperçu :")), 1, wxGROW|wxTOP, 2);
	topSizer->AddSpacer(3);
	topSizer->Add(new SG_SampleArea(panel,this,ID_CURVE_CONTROL_SAMPLE_AREA,"",wxDefaultPosition,wxSize(-1,20)), 0, wxGROW|wxBOTTOM, 2);
		
	///////////////////////
	// Ajout du sous-panneau Paramètres généraux
	wxCollapsiblePane *collpaneGeneral = new wxCollapsiblePane(panel, wxID_ANY, _("General parameters:"));
	topSizer->Add(collpaneGeneral, 0, wxGROW|wxALL );
	AddControlsToGeneralSubPane(collpaneGeneral->GetPane());
	///////////////////////
	// Ajout du sous-panneau Trait
	wxCollapsiblePane *collpanePen = new wxCollapsiblePane(panel, wxID_ANY, _("Outline:"));
	topSizer->Add(collpanePen, 0, wxGROW|wxALL );
	AddControlsToPenSubPane(collpanePen->GetPane());
	///////////////////////
	// Ajout du sous-panneau Remplissage
	wxCollapsiblePane *collpaneBrush = new wxCollapsiblePane(panel, wxID_ANY, _("Filling:"));
	topSizer->Add(collpaneBrush, 0, wxGROW|wxALL );
	AddControlsToBrushSubPane(collpaneBrush->GetPane());

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

	InitCurveControls();
	
	if(elementsModifications.GetCount()==0)
	{
		panel->Enable(false);
	}
	else
	{
		selectedElementIndice=0;
		LoadCurveElement(&elementsModifications.Item(0));
	}
    return panel;
}


void SG_PropertySheetDialog::InitCurveControls()
{
	//Chargement de la liste des éléments de dessins
	ArrayOfGraphEl::iterator it;
	wxComboBox* lstCurves=GetComboBox(this,ID_CURVE_CONTROL_CURVE_CHOICE);
	for( it = linkedGraph->Begin(); it != linkedGraph->End(); ++it )
	{
		elementsModifications.Add(t_CurrentModification(it->first,it->second->el_style,it->second->GetLabel(),it->second->IsVisible()));
		lstCurves->Append(it->second->GetLabel());
	}
	//Chargement des styles de tracés
	SG_EnumeratorFiller::FillStyleEnumeration(drawingMethodStyles);
	//Chargement des styles de traits
	SG_EnumeratorFiller::FillPenStyleEnumeration(penStyles);
	//Chargement des styles de remplissage
	SG_EnumeratorFiller::FillBrushStyleEnumeration(brushStyles);
	//Chargement des marqueurs
	SG_EnumeratorFiller::FillMarkersStyleEnumeration(markersStyles);

	FillComboBox(GetComboBox(this,ID_CURVE_CONTROL_DRAWING_METHOD),&drawingMethodStyles);
	FillComboBox(GetComboBox(this,ID_CURVE_CONTROL_PEN_STYLE_LIST),&penStyles);
	FillComboBox(GetComboBox(this,ID_CURVE_CONTROL_BRUSH_STYLE_LIST),&brushStyles);
	FillComboBox(GetComboBox(this,ID_CURVE_CONTROL_MARKER),&markersStyles);

}

void SG_PropertySheetDialog::DrawSample(wxDC* drawingAreaContext)
{
	t_CurrentModification* eleToDraw=&elementsModifications.Item(selectedElementIndice);
	if(drawingAreaContext->IsOk())
	{
		SG_Renderer drawingRender(*drawingAreaContext,linkedGraph->parameters);
		drawingRender.Begin(eleToDraw->elStyle);
		drawingAreaContext->Clear();
		linkedGraph->GetDrawingElement(eleToDraw->elIndex)->DrawSample(drawingRender,wxRect(wxPoint(0,0),drawingAreaContext->GetSize()));
	}
}
void SG_PropertySheetDialog::LoadCurveElement(t_CurrentModification* eleToLoad)
{
	if(eleToLoad!=NULL)
	{
		//Libellé
		GetComboBox(this,ID_CURVE_CONTROL_CURVE_CHOICE)->SetValue(eleToLoad->label);
		GetTextCtrl(this,ID_CURVE_CONTROL_LABEL)->ChangeValue(eleToLoad->label);
		//Visible
		GetCheckBox(this,ID_CURVE_CONTROL_VISIBILITY)->SetValue(eleToLoad->visible);
		//Methode de tracé
		GetComboBox(this,ID_CURVE_CONTROL_DRAWING_METHOD)->SetValue(SG_EnumeratorFiller::FindStringWithIndex(&drawingMethodStyles,eleToLoad->elStyle.GetDrawingMethod()));
		//wxPenStyle
		GetComboBox(this,ID_CURVE_CONTROL_PEN_STYLE_LIST)->SetValue(SG_EnumeratorFiller::FindStringWithIndex(&penStyles,eleToLoad->elStyle.GetPen().GetStyle()));
		//wxBrushStyle
		GetComboBox(this,ID_CURVE_CONTROL_BRUSH_STYLE_LIST)->SetValue(SG_EnumeratorFiller::FindStringWithIndex(&brushStyles,eleToLoad->elStyle.GetBrush().GetStyle()));
		//marker
		GetComboBox(this,ID_CURVE_CONTROL_MARKER)->SetValue(SG_EnumeratorFiller::FindStringWithIndex(&markersStyles,eleToLoad->elStyle.GetDrawingMarker()));
		//Epaisseur du trait
		GetSpinCtrl(this,ID_CURVE_CONTROL_PEN_WIDTH)->SetValue(eleToLoad->elStyle.GetPen().GetWidth());
		//Orientation du texte
		GetSpinCtrl(this,ID_CURVE_CONTROL_TEXT_ROTATION)->SetValue((wxInt32)eleToLoad->elStyle.GetTextRotation());
		//Mis à jour de l'exemple
		RefreshSample();
	}
}

void SG_PropertySheetDialog::RefreshSample()
{
	SG_SampleArea* ourSample=GetStaticBox(this,ID_CURVE_CONTROL_SAMPLE_AREA);
	if(ourSample)
	{
		ourSample->RefreshDrawingArea();
		ourSample->Refresh();
	}
}
	void SG_PropertySheetDialog::ApplyModifications()
	{
		////////////////////////////////////////////////////////////////////
		//Il faut reporter les modification vers les paramètres généraux modifié
		for(int idRow=0;idRow<propertyArray.GetCount();idRow++)
		{
			wxString rawCellValue=generalPropGrid->GetCellValue(idRow,0);
			wxString ansCellValue;
			GetGeneralSettingValue(propertyArray.Item(idRow),ansCellValue);
			if(rawCellValue!=ansCellValue)
			{
				switch(propertyArray.Item(idRow).propertyType)
				{
					case MAIN_PROPERTY_TYPE_BOOL:
						if(rawCellValue=="1")
							linkedGraph->parameters.EnableCfg((B_PARAM)propertyArray.Item(idRow).propertyIndex);
						else
							linkedGraph->parameters.DisableCfg((B_PARAM)propertyArray.Item(idRow).propertyIndex);
						break;
					case MAIN_PROPERTY_TYPE_INTEGER:
						long paramIVal;
						if(rawCellValue.ToLong(&paramIVal))
							linkedGraph->parameters.SetCfg((I_PARAM)propertyArray.Item(idRow).propertyIndex,paramIVal);
						break;
					case MAIN_PROPERTY_TYPE_FLOAT:
						wxFloat64 paramFVal;
						if(rawCellValue.ToDouble(&paramFVal))
							linkedGraph->parameters.SetCfg((F_PARAM)propertyArray.Item(idRow).propertyIndex,(wxFloat32)paramFVal);
						break;
				}
			}
		}
		////////////////////////////////////////////////////////////////////
		//Il faut reporter les modification des courbes modifiés
		for(wxInt32 tabIndice=0;tabIndice<elementsModifications.GetCount();tabIndice++)
		{
			if(elementsModifications.Item(tabIndice).modified)
			{
				SG_Element* modifiedElement=linkedGraph->GetDrawingElement(elementsModifications.Item(tabIndice).elIndex);
				if(modifiedElement)
				{
					//Copie du style
					modifiedElement->el_style=elementsModifications.Item(tabIndice).elStyle;
					//Copie du libellé
					modifiedElement->SetLabel(elementsModifications.Item(tabIndice).label);
					//Etat du graph
					modifiedElement->Hide(elementsModifications.Item(tabIndice).visible);
				}
				elementsModifications.Item(tabIndice).modified=false;
			}
		}
		linkedGraph->Refresh();

	}
} //Fin namespace
