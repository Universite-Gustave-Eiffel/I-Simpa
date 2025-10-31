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

#include "simpleGraph.h"
#include <wx/stattext.h>

#include <wx/image.h>// Debug
#include <wx/artprov.h>
#include <math.h>
#include <wx/region.h>
#include <wx/tokenzr.h>

#ifndef EPSILON
#define EPSILON				0.0001f
#endif
//#define __SG_PROFILING__
#ifdef __SG_PROFILING__
#include <wx/datetime.h>
#endif
#include <wx/log.h>
#include "last_cpp_include.hpp"

namespace sgSpace
{
	wxFloat32 ComputeBestTickInterval(wxFloat32 vmin, wxFloat32 vmax)
	{

		//
		// Méthode issu de PlPlot (pldtik.c,v 1.15 2005/04/27)
		//
		wxFloat32 t1, t2, tick_reasonable;
		wxInt32 np, ns;



		t1 = (wxFloat32)log10(fabs((vmax - vmin)));
		np = (wxInt32)floor(t1);
		t1 = t1 - np;

		if (t1 > 0.7781512503) {
			t2 = 2.0;
			ns = 4;
		}
		else if (t1 > 0.4771212549) {
			t2 = 1.0;
			ns = 5;
		}
		else if (t1 > 0.1760912591) {
			t2 = 5.0;
			ns = 5;
			np = np - 1;
		}
		else {
			t2 = 2.0;
			ns = 4;
			np = np - 1;
		}

		tick_reasonable = t2 * pow(10.0, (double)np);

		return t2 * pow(10.0, (double)np);
	}
	/**
	* Cette méthode permet le tirage aléatoire d'un nombre décimal
	* @return Decimal de 0 à 1
	*/
	inline float GetRandValue()
	{
		return ((float)rand()) / (float)RAND_MAX;
	}
	inline bool isInside(const wxRealPoint& areaMin, const wxRealPoint& areaMax, const wxRealPoint& pointTest)
	{
		return (pointTest.x >= areaMin.x) && (pointTest.y >= areaMin.y) && (pointTest.y <= areaMax.y) && (pointTest.x <= areaMax.x);
	}
	inline bool isIntersect(const wxRect& areaOne, const wxRect& areaTwo)
	{
		return !(areaTwo.x>areaOne.GetRight() || areaTwo.GetRight() < areaOne.x || areaTwo.y > areaOne.GetBottom() || areaTwo.GetBottom() < areaOne.y);
	}
	inline bool isIntersect(const wxRealPoint& areaOneD, const wxRealPoint& areaOneF, const wxRealPoint& areaTwoD, const wxRealPoint& areaTwoF)
	{
		return !(areaTwoD.x>areaOneF.x || areaTwoF.x < areaOneD.x || areaTwoD.y > areaOneF.y || areaTwoF.y < areaOneD.y);
	}
	wxRealPoint GetPointCoordAfterRotation(const wxRealPoint& BasePoint, const wxFloat32& rotationRadian)
	{

		return wxRealPoint(cos((double)rotationRadian) * BasePoint.x - sin((double)rotationRadian) * BasePoint.y,
			sin((double)rotationRadian) * BasePoint.x + cos((double)rotationRadian) * BasePoint.y);
	}


	void GetRotatedTextCorners(wxSize baseTextSize, wxFloat32 rotationDegree, wxPoint& newTopLeftCorner, wxPoint& newBottomRightCorner)
	{
		wxFloat32 radian = (wxFloat32) (M_PI * 2 / 360 * rotationDegree);
		wxRealPoint topRightCorner = GetPointCoordAfterRotation(wxRealPoint(baseTextSize.GetWidth(), 0), radian);
		wxRealPoint bottomLeftCorner = GetPointCoordAfterRotation(wxRealPoint(0, baseTextSize.GetHeight()), radian);
		wxRealPoint bottomRightCorner = GetPointCoordAfterRotation(wxRealPoint(baseTextSize.GetWidth(), baseTextSize.GetHeight()), radian);
		newTopLeftCorner = wxPoint((int) wxMin(wxMin(topRightCorner.x, bottomRightCorner.x), wxMin(0, bottomLeftCorner.x))
			, (int) wxMin(wxMin(topRightCorner.y, bottomRightCorner.y), wxMin(0, bottomLeftCorner.y))
			);
		newBottomRightCorner = wxPoint(
                (int) wxMax(wxMax(topRightCorner.x, bottomRightCorner.x), wxMax(0, bottomLeftCorner.x))
			, (int) wxMax(wxMax(topRightCorner.y, bottomRightCorner.y), wxMax(0, bottomLeftCorner.y))
			);
	}

	wxSize GetRotatedTextExtent(wxSize baseTextSize, wxFloat32 rotationDegree)
	{
		wxPoint newTopLeftCorner;
		wxPoint newBottomRightCorner;
		GetRotatedTextCorners(baseTextSize, rotationDegree, newTopLeftCorner, newBottomRightCorner);
		return wxSize(newBottomRightCorner.x - newTopLeftCorner.x, newBottomRightCorner.y - newTopLeftCorner.y);
	}
	/**
	* Ajoute des caractères d'échappement sur les caractères dédiés à la sérialisation du style des tracés
	* @param s Chaine de configuration
	* @return Chaine de configuration traitée
	*/
	wxString EscapeDelimiters(const wxString& s)
	{
		wxString result;
		result.Alloc(s.length());
		const wxChar* ch = s.c_str();
		while (*ch)
		{
			if (*ch == wxT(';'))
				result += wxT("**dv");
			else if (*ch == wxT('='))
				result += wxT("**eq");
			else
				result += *ch;
			++ch;
		}
		return result;
	}

	/**
	* Retrouve la chaine d'origine
	* @param s Chaine de configuration
	* @return Chaine de configuration traitée
	*/
	wxString RetrieveDelimiters(const wxString& s)
	{
		wxString ret(s);
		ret.Replace("**dv", ";");
		ret.Replace("**eq", "=");
		return ret;
	}
	wxInt64 ToInt(const wxString& sval)
	{
		long rval;
		sval.ToLong(&rval);
		return (wxInt64)rval;
	}
	bool ToFloat(const wxString& sval, wxFloat32* floatVal)
	{
		double val = 0.f;
		if (sval.ToDouble(&val))
		{
			*floatVal = val;
			return true;
		}
		else {
			return false;
		}
	}

	void SplitString(const wxString& src, const wxString& delimiter, wxArrayString& dst)
	{
		wxStringTokenizer tkz(src, delimiter);
		while (tkz.HasMoreTokens())
		{
			dst.Add(tkz.GetNextToken());
		}
	}
	//////////////////////////////////////////////////
	// Définition de SG_El_Params

	SG_El_Params::SG_El_Params()
	{
		params.drawingMethod = DRAWING_METHOD_LINE;
		params.drawingMarker = DRAWING_MARKER_NONE;
		params.isTextOriented = false;
		textFont = *wxSWISS_FONT;
		this->drawFill = *wxTRANSPARENT_BRUSH;
		this->drawPen = *wxBLACK_PEN;
		params.textOrientation = 0;
		textForegroundColor = *wxBLACK;
		textBackgroundColor = wxColour(0, 0, 0, wxALPHA_TRANSPARENT);
	}
	/*

	chaineExport<<"drawing_method="<<this->params.drawingMethod
	<<";pen_color="<<this->drawPen.GetColour().GetAsString(wxC2S_HTML_SYNTAX)
	<<";pen_width="<<this->drawPen.GetWidth()
	<<";pen_style="<<this->drawPen.GetStyle()
	<<";font="<<EscapeDelimiters(this->textFont.GetNativeFontInfoDesc())
	<<";brush_color="<<this->drawFill.GetColour().GetAsString(wxC2S_HTML_SYNTAX)
	<<";brush_style="<<this->drawFill.GetStyle()
	<<";text_foreground="<<this->textForegroundColor.GetAsString(wxC2S_HTML_SYNTAX)
	<<";text_background="<<this->textBackgroundColor.GetAsString(wxC2S_HTML_SYNTAX)
	<<";drawing_marker="<<this->params.drawingMarker
	<<";text_orientation="<<this->params.textOrientation;

	*/
	wxString SG_El_Params::Serialize()
	{
		wxString chaineExport;
		chaineExport << "drawing_method=" << this->params.drawingMethod
			<< ";pen_color=" << this->drawPen.GetColour().GetAsString()
			<< ";pen_width=" << this->drawPen.GetWidth()
			<< ";pen_style=" << this->drawPen.GetStyle()
			<< ";font=" << EscapeDelimiters(this->textFont.GetNativeFontInfoDesc())
			<< ";brush_color=" << this->drawFill.GetColour().GetAsString()
			<< ";brush_style=" << this->drawFill.GetStyle()
			<< ";text_foreground=" << this->textForegroundColor.GetAsString()
			<< ";text_background=" << this->textBackgroundColor.GetAsString()
			<< ";drawing_marker=" << this->params.drawingMarker
			<< ";text_orientation=" << this->params.textOrientation;

		return chaineExport;
	}

	void SG_El_Params::FromSerialization(const wxString& data)
	{
		//De la forme drawing_method=fsdfs;pen_color=#FFFF00;pen_width...
		wxArrayString paramVector;
		SplitString(data, ";", paramVector);
		if (paramVector.size()>0)
		{
			for (wxInt32 i = 0; i<paramVector.size(); i++)
			{
				wxArrayString lineVector;
				SplitString(paramVector[i], "=", lineVector);
				if (lineVector.size() == 2)
				{
					this->LoadConfig(lineVector[0], lineVector[1]);
				}
			}
		}
	}

	void SG_El_Params::LoadConfig(const wxString& name, const wxString& value)
	{
		if (name == "drawing_method")
		{
			DRAWING_METHOD methodIndex = (DRAWING_METHOD)ToInt(value);
			this->params.drawingMethod = methodIndex;
		}
		else if (name == "pen_color") {
			this->drawPen.SetColour(value);
		}
		else if (name == "pen_width") {
			this->drawPen.SetWidth((int) ToInt(value));
		}
		else if (name == "pen_style") {
			this->drawPen.SetStyle((wxPenStyle) ToInt(value));
		}
		else if (name == "font") {
			this->textFont.SetNativeFontInfo(RetrieveDelimiters(value));
		}
		else if (name == "brush_color") {
			this->drawFill.SetColour(value);
		}
		else if (name == "brush_style") {
			this->drawFill.SetStyle((wxBrushStyle) ToInt(value));
		}
		else if (name == "text_foreground") {
			this->textForegroundColor.Set(value);
		}
		else if (name == "text_background") {
			this->textBackgroundColor.Set(value);
		}
		else if (name == "drawing_marker") {
			DRAWING_MARKER markerIndex = (DRAWING_MARKER) ToInt(value);
			this->params.drawingMarker = markerIndex;
		}
		else if (name == "text_orientation") {
			wxFloat32 textOrentation;
			if (ToFloat(value, &textOrentation))
			{
				this->SetTextRotation(textOrentation);
			}
		}
	}

	const wxFont& SG_El_Params::GetFont()
	{
		return textFont;
	}


	void SG_El_Params::SetFont(const wxFont& newFont)
	{
		textFont = newFont;
	}
	void SG_El_Params::SetTextRotation(const wxFloat32& textRotation)
	{
		params.textOrientation = textRotation;
		params.isTextOriented = (textRotation != (wxFloat32)0);
	}
	bool SG_El_Params::IsTextRotated()
	{
		return params.isTextOriented;
	}
	wxFloat32 SG_El_Params::GetTextRotation()
	{
		return params.textOrientation;
	}
	const wxPen& SG_El_Params::GetPen()
	{
		return this->drawPen;
	}

	const wxBrush& SG_El_Params::GetBrush()
	{
		return this->drawFill;
	}

	DRAWING_METHOD SG_El_Params::GetDrawingMethod()
	{
		return params.drawingMethod;
	}

	void SG_El_Params::SetPen(const wxPen* newPen)
	{
		this->drawPen = *newPen;
	}

	void SG_El_Params::SetBrush(const wxBrush* newBrush)
	{
		this->drawFill = *newBrush;
	}

	void SG_El_Params::SetDrawingMethod(DRAWING_METHOD _drawingMethod)
	{
		params.drawingMethod = _drawingMethod;
		if (_drawingMethod == DRAWING_METHOD_DOT_CLOUD)
			SetDrawingMarker(DRAWING_MARKER_DOT);
	}
	void SG_El_Params::SetDrawingMarker(DRAWING_MARKER _drawingMarker)
	{
		params.drawingMarker = _drawingMarker;
	}
	DRAWING_MARKER SG_El_Params::GetDrawingMarker()
	{
		return params.drawingMarker;
	}
	void  SG_El_Params::Randomize()
	{
		const int nbPenStyle = 10;
		const int nbBrushStyle = 7;
		wxInt32 penStyle[nbPenStyle] = { wxSOLID,wxSOLID,wxSOLID,wxDOT,wxBDIAGONAL_HATCH,wxCROSSDIAG_HATCH,wxFDIAGONAL_HATCH,wxCROSS_HATCH,wxHORIZONTAL_HATCH,wxVERTICAL_HATCH };
		wxInt32 brushStyle[nbBrushStyle] = { wxSOLID,wxBDIAGONAL_HATCH,wxCROSSDIAG_HATCH,wxFDIAGONAL_HATCH,wxCROSS_HATCH,wxHORIZONTAL_HATCH,wxVERTICAL_HATCH };
		this->drawPen = wxPen(wxColour((wxColourBase::ChannelType) (GetRandValue() * 245),(wxColourBase::ChannelType) (GetRandValue() * 245),(wxColourBase::ChannelType) (GetRandValue() * 245)),
							  (int) (GetRandValue() * 4), (wxPenStyle) penStyle[wxInt32(GetRandValue() * nbPenStyle)]);
		this->drawFill = wxBrush(wxColour((wxColourBase::ChannelType) (GetRandValue() * 245),(wxColourBase::ChannelType) (GetRandValue() * 245),(wxColourBase::ChannelType) (GetRandValue() * 245)),
								 (wxBrushStyle) brushStyle[wxInt32(GetRandValue()*nbBrushStyle)]);
	}

	void SG_El_Params::RandomizeMarker()
	{
		SetDrawingMarker((DRAWING_MARKER)((int)(GetRandValue() * 3 + 1)));
	}

	const wxColour& SG_El_Params::GetTextForeGroundColor()
	{
		return textForegroundColor;
	}
	const wxColour& SG_El_Params::GetTextBackGroundColor()
	{
		return textBackgroundColor;
	}
	void SG_El_Params::SetTextForeGroundColor(const wxColour& newColour)
	{
		textForegroundColor = newColour;
	}
	void SG_El_Params::SetTextBackGroundColor(const wxColour& newColour)
	{
		textBackgroundColor = newColour;
	}
	//////////////////////////////////////////////////
	// Définition de SG_Params
	SG_Params::SG_Params(simpleGraph* parent)
		:m_parent(parent)
	{
		memset(floatParamTab, 0, sizeof(wxFloat32)*F_PARAM_LAST);
		memset(integerParamTab, 0, sizeof(wxInt32)*I_PARAM_LAST);
		memset(booleanParamTab, 0, sizeof(bool)*B_PARAM_LAST);

	}

	void SG_Params::Init()
	{
		//Initialisation des valeurs par défaut
		SetCfg(F_PARAM_X_MIN, 0);
		SetCfg(F_PARAM_X_MAX, 1.f);
		SetCfg(F_PARAM_Y_MIN, 0);
		SetCfg(F_PARAM_Y_MAX, 1.f);
		SetCfg(I_PARAM_TIC_SIZE, 5);
		SetCfg(I_PARAM_X_BORDER_AXIS, 2);
		SetCfg(I_PARAM_Y_BORDER_AXIS, 2);
		SetCfg(I_PARAM_LEGEND_ICON_SIZE, 16);
		SetCfg(F_PARAM_MARKER_SIZE, 1);
		EnableCfg(B_PARAM_RIGHT_CLICK_ZOOM_FIT);
	}
	void SG_Params::SetCfg(I_PARAM indice, wxInt32 value)
	{
		integerParamTab[indice] = value;
		m_parent->OnUpdate(indice);
	}
	const wxInt32& SG_Params::GetCfg(I_PARAM indice)
	{
		return integerParamTab[indice];
	}
	void SG_Params::SetCfg(F_PARAM indice, wxFloat32 value)
	{
		floatParamTab[indice] = value;
		m_parent->OnUpdate(indice);
	}
	const wxFloat32& SG_Params::GetCfg(F_PARAM indice)
	{
		return floatParamTab[indice];
	}
	void SG_Params::EnableCfg(B_PARAM indice)
	{
		booleanParamTab[indice] = true;
		m_parent->OnUpdate(indice);
	}
	void SG_Params::DisableCfg(B_PARAM indice)
	{
		booleanParamTab[indice] = false;
		m_parent->OnUpdate(indice);
	}

	bool SG_Params::isEnable(B_PARAM indice)
	{
		return booleanParamTab[indice];
	}

	//////////////////////////////////////////////////
	// Définition de SG_Element

	SG_Element::SG_Element(simpleGraph* _parent, const wxString& elLabel, AREA_TYPE _drawingArea)
		:parent(_parent),
		drawingArea(_drawingArea),
		elementLabel(elLabel),
		visible(true)
	{
		_parent->AddDrawingElement(this);
	}

	bool SG_Element::IsVisible()
	{
		return visible;
	}
	void SG_Element::Hide(bool _visible)
	{
		visible = _visible;
	}
	wxSize SG_Element::GetBestSize() const
	{
		return wxSize(-1, -1);
	}

	void SG_Element::DrawSample(SG_Renderer& renderer, const wxRect& drawZone)
	{
		renderer.Begin(el_style);
		renderer.DrawSample(drawZone);
		renderer.End();
	}

	wxString  SG_Element::GetLabel() const
	{
		return elementLabel;
	}

	void SG_Element::SetLabel(const wxString& newLabel)
	{
		elementLabel = newLabel;
	}
	//////////////////////////////////////////////////
	// Définition de SG_Element_List

	SG_Element_List::SG_Element_List(simpleGraph* _parent, const wxString& elLabel, const wxInt32& sizeHint)
		:SG_Element_Data(_parent, elLabel, sizeHint)
	{
		lastXVal = -1;
	}
	void SG_Element_List::PushValue(const wxFloat32& ElValue)
	{
		lastXVal++;
		valuesY.Add(ElValue);
		valuesX.Add(lastXVal);
	}


	void SG_Element_List::GetMinMax(wxFloat32& xMin, wxFloat32& xMax, wxFloat32& yMin, wxFloat32& yMax)
	{
		SG_Element_Data::GetMinMax(xMin, xMax, yMin, yMax);
		xMin -= .5f;
		xMax += .5f;
	}

	//////////////////////////////////////////////////
	// Définition de SG_Element_Data



	SG_Element_Data::SG_Element_Data(simpleGraph* _parent, const wxString& elLabel, const wxInt32& sizeHint)
		:SG_Element(_parent, elLabel, AREA_TYPE_MAINGRAPH)
	{
		valuesY.Alloc(sizeHint);
		valuesX.Alloc(sizeHint);
	}
	wxInt32 SG_Element_Data::GetSize()
	{
		return valuesY.GetCount();
	}
	void SG_Element_Data::Draw(wxWindow* paintArea, SG_Renderer& renderer)
	{
		renderer.Begin(el_style);
		renderer.Draw(valuesY, valuesX);
		renderer.End();
	}

	void SG_Element_Data::PushValue(const wxFloat32& xx, const wxFloat32& yy)
	{
		valuesX.Add(xx);
		valuesY.Add(yy);
	}

	wxFloat32 SG_Element_Data::PushStringValue(const wxString& xx, const wxFloat32& yy)
	{
		char  *stopstring;
		wxFloat32 xVal = atof(xx.c_str());
		valuesX.Add(xVal);
		valuesY.Add(yy);
		return xVal;
	}

	void SG_Element_Data::GetMinMax(wxFloat32& xMin, wxFloat32& xMax, wxFloat32& yMin, wxFloat32& yMax)
	{
		//Calcul des valeurs Min et Max
		wxInt32 items = valuesX.GetCount();
		if (items>0)
		{
			if (wxFinite(valuesY.Item(0)))
			{
				yMin = valuesY.Item(0);
				yMax = valuesY.Item(0);
			}
			else {
				yMin = 0;
				yMax = 0;
			}
			xMin = valuesX.Item(0);
			xMax = valuesX.Item(0);
			for (int idel = 1; idel<items; idel++)
			{
				if (wxFinite(valuesY.Item(idel)))
				{
					yMin = wxMin(yMin, valuesY.Item(idel));
					yMax = wxMax(yMax, valuesY.Item(idel));
					xMin = wxMin(xMin, valuesX.Item(idel));
					xMax = wxMax(xMax, valuesX.Item(idel));
				}
			}
		}
		if (el_style.GetDrawingMethod() == DRAWING_METHOD_COLS)
		{
			xMin = 0.f;
			if (yMin>0)
				yMin = 0.f;
		}
	}
	//////////////////////////////////////////////////
	// Définition de SG_Element_Axis

	SG_Element_Axis::SG_Element_Axis(simpleGraph* _parent, wxOrientation _axisOrientation, wxFloat32 intervalValue, wxString unitLabel)
		:SG_Element(_parent, unitLabel, (((_axisOrientation) == (wxHORIZONTAL)) ? (AREA_TYPE_X_AXIS) : (AREA_TYPE_Y_AXIS)))
		, axisOrientation(_axisOrientation),
		intervalLabel(intervalValue)
	{
		if (intervalValue != 0.f)
		{
			if (this->axisOrientation == wxVERTICAL)
			{
				_parent->parameters.SetCfg(F_PARAM_Y_TIC, intervalValue);
			}
			else {
				_parent->parameters.SetCfg(F_PARAM_X_TIC, intervalValue);
			}
		}
        maxSizeElement = 50;
		sizeofTitle = wxSize(12, 12);
	}
	wxSize SG_Element_Axis::GetBestSize() const
	{
		if (this->axisOrientation == wxVERTICAL)
		{
            return wxSize(maxSizeElement + sizeofTitle.GetWidth(), -1);
		}
		else {
			return wxSize(-1, maxSizeElement + sizeofTitle.GetHeight());
		}
	}
	wxString SG_Element_Axis::GetTextValue(const wxFloat32& realValue, bool& drawTic)
	{
		drawTic = true;
		return wxString::Format("%g", realValue);
	}

	void SG_Element_Axis::AddTic(SG_Renderer& renderer, const wxPoint& from, const wxPoint& to, const wxFloat32& realValue)
	{
		bool drawTicIfTextHidden = true;
		wxString txtTic = GetTextValue(realValue, drawTicIfTextHidden);
		wxSize textSize = renderer.GetTextDeviceSize(txtTic);
		wxSize deviceSize = renderer.GetDeviceSize();
		wxPoint topLeftPos;
		if (this->axisOrientation == wxVERTICAL)
		{
			topLeftPos = wxPoint(to.x, to.y + 1);
			topLeftPos.x -= textSize.GetWidth();
			topLeftPos.y -= textSize.GetHeight() / 2.f;
			if (topLeftPos.y + textSize.GetHeight()>deviceSize.GetHeight())
				topLeftPos.y = deviceSize.GetHeight() - textSize.GetHeight();
			else if (topLeftPos.y<0)
				topLeftPos.y = 0;
			if (maxSizeElement<textSize.GetWidth())
				maxSizeElement = textSize.GetWidth();
		}
		else {
			topLeftPos = wxPoint(to.x, to.y + 1);
			topLeftPos.x -= textSize.GetWidth() / 2.f;
			if (topLeftPos.x + textSize.GetWidth()>deviceSize.GetWidth())
				topLeftPos.x = deviceSize.GetWidth() - textSize.GetWidth();
			else if (topLeftPos.x<0)
				topLeftPos.x = 0;
			if (maxSizeElement<textSize.GetHeight())
				maxSizeElement = textSize.GetHeight();
		}
		if (drawTicIfTextHidden)
			renderer.DrawLine(from, to);
		if ((this->axisOrientation == wxVERTICAL && topRightLabelPixelValue>topLeftPos.y + textSize.GetHeight()) || (this->axisOrientation == wxHORIZONTAL && topRightLabelPixelValue<topLeftPos.x))
		{
			if (!drawTicIfTextHidden)
				renderer.DrawLine(from, to);
			renderer.DrawText(topLeftPos, txtTic);

			if (this->axisOrientation == wxVERTICAL)
			{
				topRightLabelPixelValue = topLeftPos.y;
			}
			else {
				topRightLabelPixelValue = topLeftPos.x + textSize.GetWidth();
			}
		}

	}

	wxString SG_Element_Axis::GetLabel() const
	{
		if (SG_Element::GetLabel().IsEmpty())
		{
			if (this->axisOrientation == wxVERTICAL)
			{
				return wxGetTranslation("Y axis");
			}
			else {
				return wxGetTranslation("X axis");
			}
		}
		else {
			return SG_Element::GetLabel();
		}
	}
	void SG_Element_Axis::SetLabel(const wxString& newLabel)
	{
		if (newLabel != wxGetTranslation("X axis") && newLabel != wxGetTranslation("Y axis"))
		{
			SG_Element::SetLabel(newLabel);
		}
	}

	void SG_Element_Axis::DrawAxeWithTics(SG_Renderer& renderer)
	{

		//Dessin de la barre de l'axe
		wxSize sizeOfArea = renderer.GetDeviceSize();
		wxInt16 ticSize = renderer.GetMainStyle()->GetCfg(I_PARAM_TIC_SIZE);
		if (axisOrientation == wxHORIZONTAL)
		{
			renderer.DrawLine(wxPoint(0, 0), wxPoint(sizeOfArea.GetWidth() - 1, 0));
		}
		else {
			renderer.DrawLine(wxPoint(sizeOfArea.GetWidth() - 1, 0), wxPoint(sizeOfArea.GetWidth() - 1, sizeOfArea.GetHeight() - 1));
		}
		// Dessin des traits entre le libellé et la barre
		if (axisOrientation == wxHORIZONTAL)
		{
			wxFloat32 xMin = renderer.GetMainStyle()->GetCfg(F_PARAM_X_MIN);
			wxFloat32 xMax = renderer.GetMainStyle()->GetCfg(F_PARAM_X_MAX);
			wxInt16 nbLabels = floor((wxFloat32)((xMax - xMin) / intervalLabel));
			wxFloat64 labelValue = (xMin)-(wxFloat32)fmod((xMin), intervalLabel);

			if (labelValue<EPSILON && labelValue>-EPSILON)
				labelValue = 0.f;

			wxInt32 yCoord = 0;
			while (labelValue <= xMax)
			{
				wxPoint deviceCoord = renderer.LogicalToDeviceCoord((wxFloat32) labelValue, 0.f);
				if (deviceCoord.x >= 0)
					this->AddTic(renderer, wxPoint(deviceCoord.x, yCoord), wxPoint(deviceCoord.x, yCoord + ticSize), labelValue);
				labelValue += intervalLabel;
			}

		}
		else {
			wxFloat32 yMin = renderer.GetMainStyle()->GetCfg(F_PARAM_Y_MIN);
			wxFloat32 yMax = renderer.GetMainStyle()->GetCfg(F_PARAM_Y_MAX);
			wxFloat64 labelValue = (yMin)-(wxFloat32)fmod((yMin), intervalLabel);
			wxInt32 xCoord = sizeOfArea.GetWidth() - 1;
			if (labelValue<EPSILON && labelValue>-EPSILON)
				labelValue = 0.f;
			while (labelValue <= yMax)
			{
				wxPoint deviceCoord = renderer.LogicalToDeviceCoord(0.f, (wxFloat32) labelValue);
				if (deviceCoord.y >= 0 && sizeOfArea.GetHeight()>deviceCoord.y)
					this->AddTic(renderer, wxPoint(xCoord, deviceCoord.y), wxPoint(xCoord - ticSize, deviceCoord.y), labelValue);
				labelValue += intervalLabel;
			}
		}


	}

	void SG_Element_Axis::Draw(wxWindow* paintArea, SG_Renderer& renderer)
	{

		renderer.Begin(el_style);
		wxSize textSize = renderer.GetTextDeviceSize(elementLabel);
		sizeofTitle = textSize;
		maxSizeElement = 0;
		wxSize sizeOfArea = renderer.GetDeviceSize();
		//Dessin du libellé
		if (axisOrientation == wxHORIZONTAL)
		{
			if (!elementLabel.IsEmpty())
			{
				renderer.DrawText(wxPoint(sizeOfArea.GetWidth() - textSize.GetWidth(), sizeOfArea.GetHeight() - textSize.GetHeight()), elementLabel);
				//paintArea->SetInitialBestSize(wxSize(-1,textSize.GetHeight()*2));
			}
			topRightLabelPixelValue = -1;

			wxFloat32 xTicSize = renderer.GetMainStyle()->GetCfg(F_PARAM_X_TIC);
			if (xTicSize>0.f)
				intervalLabel = xTicSize;
			else
				intervalLabel = ComputeBestTickInterval(renderer.GetMainStyle()->GetCfg(F_PARAM_X_MIN), renderer.GetMainStyle()->GetCfg(F_PARAM_X_MAX));
		}
		else {
			if (!elementLabel.IsEmpty())
			{
				renderer.DrawText(wxPoint(0, 0), elementLabel);
			}
			topRightLabelPixelValue = sizeOfArea.GetHeight() * 2;
			wxFloat32 yTicSize = renderer.GetMainStyle()->GetCfg(F_PARAM_Y_TIC);
			if (yTicSize>0.f)
				intervalLabel = yTicSize;
			else
				intervalLabel = ComputeBestTickInterval(renderer.GetMainStyle()->GetCfg(F_PARAM_Y_MIN), renderer.GetMainStyle()->GetCfg(F_PARAM_Y_MAX));
		}

		//Dessin des axes


		DrawAxeWithTics(renderer);
		renderer.End();
	}
	void SG_Element_Axis::GetMinMax(wxFloat32& xMin, wxFloat32& xMax, wxFloat32& yMin, wxFloat32& yMax)
	{

	}

	//////////////////////////////////////////////////
	// Définition de SG_Element_Axis_Labeled
	SG_Element_Axis_Labeled::SG_Element_Axis_Labeled(simpleGraph* _parent, wxOrientation _axisOrientation, wxFloat32 intervalValue, wxString unitLabel)
		: SG_Element_Axis(_parent, _axisOrientation, intervalValue, unitLabel)
	{

	}

	void SG_Element_Axis_Labeled::PushLabel(wxString label)
	{
		labels.Add(label);
	}

	wxString SG_Element_Axis_Labeled::GetTextValue(const wxFloat32& realValue, bool& drawTic)
	{
		wxInt32 idCol = (wxInt32)realValue;
		if (idCol >= 0 && idCol<labels.GetCount())
		{
			drawTic = false;
			return labels.Item(idCol);
		}
		else {
			return "";
		}
	}
	//////////////////////////////////////////////////
	// Définition de SG_Renderer
	SG_Renderer::SG_Renderer(wxDC& drawingContext, SG_Params& mainStyle, wxSize drawSize)
	{
		histoGramWidth = 1.f / .9f;
		currentContext = &drawingContext;
		currentMainStyle = &mainStyle;
		currentElementStyle = NULL;


		if (drawSize.GetHeight() == -1 && drawSize.GetWidth() == -1)
		{
			wxSize drawRect = drawingContext.GetSize();
			mainStyle.SetCfg(I_PARAM_VIEW_WIDTH, drawRect.GetWidth());
			mainStyle.SetCfg(I_PARAM_VIEW_HEIGHT, drawRect.GetHeight());
		}
		else {
			mainStyle.SetCfg(I_PARAM_VIEW_WIDTH, drawSize.GetWidth());
			mainStyle.SetCfg(I_PARAM_VIEW_HEIGHT, drawSize.GetHeight());
		}
	}

	void SG_Renderer::Begin(SG_El_Params& el_style)
	{
		if (!currentElementStyle)
		{
			currentElementStyle = &el_style;
			currentContext->SetPen(el_style.GetPen());
			currentContext->SetBrush(el_style.GetBrush());
			currentContext->SetFont(el_style.GetFont());
			currentContext->SetTextForeground(el_style.GetTextForeGroundColor());
			currentContext->SetTextBackground(el_style.GetTextBackGroundColor());
			if (el_style.GetDrawingMethod() == DRAWING_METHOD_COLS)
			{
				wxInt32 penWidth = el_style.GetPen().GetWidth();
				histoGramWidth *= (1.f - (.1*penWidth));
			}
		}
	}

	SG_Params* SG_Renderer::GetMainStyle()
	{
		return currentMainStyle;
	}

	wxSize SG_Renderer::GetTextDeviceSize(const wxString& textString)
	{
		wxSize baseTextSize = currentContext->GetTextExtent(textString);
		baseTextSize.IncBy(1);
		if (!currentElementStyle->IsTextRotated())
		{
			return baseTextSize;
		}
		else {
			return GetRotatedTextExtent(baseTextSize, currentElementStyle->GetTextRotation());
		}
	}

	void SG_Renderer::DrawSample(const wxRect& drawZone)
	{
		wxInt32 centerY = (drawZone.GetTop() + drawZone.GetBottom()) / 2.f;
		wxInt32 centerX = (drawZone.GetLeft() + drawZone.GetRight()) / 2.f;
		wxPoint leftCenter(drawZone.GetLeft(), centerY);
		wxPoint rightCenter(drawZone.GetRight(), centerY);
		wxPoint MidleCenter(centerX, centerY);
		wxPoint BottomCenter(centerX, drawZone.GetBottom());
		DRAWING_METHOD eleMethod = currentElementStyle->GetDrawingMethod();
		DRAWING_MARKER eleMarker = currentElementStyle->GetDrawingMarker();

		if (eleMethod == DRAWING_METHOD_LINE || eleMethod == DRAWING_METHOD_SPLINE)
		{
			DrawLine(leftCenter, MidleCenter);
			DrawLine(MidleCenter, rightCenter);
		}
		else if (eleMethod == DRAWING_METHOD_STIPPLE)
		{

		}
		else if (eleMethod == DRAWING_METHOD_ECHOGRAM)
		{
			DrawLine(BottomCenter, MidleCenter);
		}
		else if (eleMethod == DRAWING_METHOD_LINES_AND_STIPPLE)
		{

		}
		else if (eleMethod == DRAWING_METHOD_COLS)
		{
			DrawRectangle(drawZone);
		}


		if (eleMethod == DRAWING_METHOD_DOT_CLOUD || (eleMarker != DRAWING_MARKER_NONE && (eleMethod == DRAWING_METHOD_LINE || eleMethod == DRAWING_METHOD_SPLINE || eleMethod == DRAWING_METHOD_ECHOGRAM)))
		{
			wxPoint points[1] = { MidleCenter };
			DrawMarkers(1, points, eleMarker);
		}

	}

	void SG_Renderer::DrawText(wxPoint topLeftPosition, const wxString& text)
	{
		if (!currentElementStyle || !currentElementStyle->IsTextRotated())
		{
			currentContext->DrawText(text, topLeftPosition.x, topLeftPosition.y);
		}
		else
		{
			wxPoint nTopLeftDot;
			wxPoint nBottomRightDot;
			GetRotatedTextCorners(currentContext->GetTextExtent(text), currentElementStyle->GetTextRotation(), nTopLeftDot, nBottomRightDot);
			wxPoint calculatedTopLeft(nBottomRightDot.x + topLeftPosition.x, nBottomRightDot.y + topLeftPosition.y);

			currentContext->DrawRotatedText(text, calculatedTopLeft.x, calculatedTopLeft.y, currentElementStyle->GetTextRotation());
		}
	}
	void SG_Renderer::Draw(const ArrayOfFloats& Yvalues, const ArrayOfFloats& Xvalues)
	{
		if (Xvalues.GetCount() != Yvalues.GetCount() || Yvalues.GetCount() == 0)
		{
			return;
		}
		DRAWING_METHOD eleMethod = currentElementStyle->GetDrawingMethod();

		//////////////////////////////////////////////////////////////
		//On filtre les valeurs qui ne peuvent être visibles
		wxSize deviceSize = GetDeviceSize();
		wxRealPoint areaDeb(currentMainStyle->GetCfg(F_PARAM_X_MIN), currentMainStyle->GetCfg(F_PARAM_Y_MIN));
		wxRealPoint areaFin(currentMainStyle->GetCfg(F_PARAM_X_MAX), currentMainStyle->GetCfg(F_PARAM_Y_MAX));


		// Filtrage des valeurs dans le but d'afficher que les droites visibles
		// Il en est de même pour les valeurs infini, leurs valeur Y deviennent la limites basse
		wxInt32 sizeOf = Yvalues.GetCount();
		ArrayOfFloats filteredYvalues;
		ArrayOfFloats filteredXvalues;
		filteredYvalues.Alloc(sizeOf);
		filteredXvalues.Alloc(sizeOf);
		bool lastValAdded = false;
		//Si le dessin ce fin de la valeur actuel Y jusqu'a la valeur y=0
		bool drawFromYvalToZeroBase = ((eleMethod == DRAWING_METHOD_COLS) || (eleMethod == DRAWING_METHOD_ECHOGRAM));
		bool drawLinkedElements = !(eleMethod == DRAWING_METHOD_DOT_CLOUD);
		if (Yvalues.GetCount() >= 2)
		{
			for (int idel = 1; idel<sizeOf; idel++)
			{
				wxRealPoint firstPoint(Xvalues.Item(idel - 1), Yvalues.Item(idel - 1));
				wxRealPoint secondPoint(Xvalues.Item(idel), Yvalues.Item(idel));

				wxRealPoint minCoord(wxMin(firstPoint.x, secondPoint.x), wxMin(firstPoint.y, secondPoint.y));
				wxRealPoint maxCoord(wxMax(firstPoint.x, secondPoint.x), wxMax(firstPoint.y, secondPoint.y));

				if (drawFromYvalToZeroBase)
				{
					minCoord = wxRealPoint(minCoord.x, areaDeb.y);
					maxCoord = wxRealPoint(maxCoord.x, areaFin.y);
				}

				if (drawLinkedElements || wxFinite(secondPoint.y))
				{
					if (!wxFinite(firstPoint.y))
					{
						if (!drawFromYvalToZeroBase)
							firstPoint.x = secondPoint.x;
						if (firstPoint.y<0) //- l'infini
						{
							firstPoint.y = areaDeb.y;
						}
						else {			   //+ l'infini
							firstPoint.y = areaFin.y;
						}
					}
					if (!wxFinite(secondPoint.y))
					{
						if (!drawFromYvalToZeroBase)
							secondPoint.x = firstPoint.x;
						if (secondPoint.y<0) //- l'infini
						{
							secondPoint.y = areaDeb.y;
						}
						else {			   //+ l'infini
							secondPoint.y = areaFin.y;
						}

					}


					bool rectCollision = isIntersect(areaDeb, areaFin, minCoord, maxCoord);

					if (rectCollision && !lastValAdded)
					{
						filteredYvalues.Add(firstPoint.y);
						filteredXvalues.Add(firstPoint.x);
					}
					if (rectCollision)
					{
						lastValAdded = true;
						filteredYvalues.Add(secondPoint.y);
						filteredXvalues.Add(secondPoint.x);
					}
					else {
						lastValAdded = false;
					}
				}
			}
		}
		else {
			filteredYvalues = Yvalues;
			filteredXvalues = Xvalues;
		}



		//////////////////////////////////////
		//Fin du filtrage
		if (!filteredYvalues.IsEmpty())
		{
			if (eleMethod == DRAWING_METHOD_LINE || eleMethod == DRAWING_METHOD_SPLINE)
				DrawLines(filteredYvalues, filteredXvalues);
			else if (eleMethod == DRAWING_METHOD_ECHOGRAM)
				DrawEchogram(filteredYvalues, filteredXvalues);
			else if (eleMethod == DRAWING_METHOD_COLS)
				DrawHistogram(filteredYvalues, filteredXvalues);
			else if (eleMethod == DRAWING_METHOD_STIPPLE)
				DrawStipples(filteredYvalues, filteredXvalues, false);
			else if (eleMethod == DRAWING_METHOD_LINES_AND_STIPPLE)
				DrawStipples(filteredYvalues, filteredXvalues, true);
			else if (eleMethod == DRAWING_METHOD_DOT_CLOUD)
				DrawDotsCloud(filteredYvalues, filteredXvalues);
		}
	}

	void SG_Renderer::DrawMarkers(wxInt32 n, wxPoint points[], DRAWING_MARKER eleMarker)
	{
		wxPen oldPen = currentElementStyle->GetPen();
		wxPen newPen = oldPen;
		newPen.SetWidth(1);
		newPen.SetStyle(wxPENSTYLE_SOLID);
		currentContext->SetPen(newPen);


		wxInt32 sizeBase = currentMainStyle->GetCfg(I_PARAM_COMPUTED_MARKER_SIZE);
		wxSize markerSize(sizeBase, sizeBase);

		if (eleMarker == DRAWING_MARKER_CROSS_D)
			DrawCrossDMarker(n, points, markerSize);
		else if (eleMarker == DRAWING_MARKER_DOT)
			DrawDotMarker(n, points, markerSize);
		else if (eleMarker == DRAWING_MARKER_SQUARE)
			DrawRectMarker(n, points, markerSize);



		currentContext->SetPen(oldPen);
	}

	void SG_Renderer::DrawDotMarker(wxInt32 n, wxPoint points[], const wxSize& markerSize)
	{
		int radius = markerSize.GetWidth();
		for (int idel = 0; idel<n; idel++)
		{
			currentContext->DrawCircle(points[idel].x, points[idel].y, radius);
		}
	}
	void SG_Renderer::DrawCrossDMarker(wxInt32 n, wxPoint points[], const wxSize& markerSize)
	{
		wxUint16 widthDiv2 = (wxUint16)(markerSize.GetWidth() / 2.f);
		wxUint16 heightDiv2 = (wxUint16)(markerSize.GetHeight() / 2.f);
		for (int idel = 0; idel<n; idel++)
		{
			currentContext->DrawLine(points[idel].x - widthDiv2, points[idel].y - heightDiv2, points[idel].x + widthDiv2 + 1, points[idel].y + heightDiv2 + 1);
			currentContext->DrawLine(points[idel].x - widthDiv2, points[idel].y + heightDiv2, points[idel].x + widthDiv2 + 1, points[idel].y - heightDiv2 - 1);
		}
	}
	void SG_Renderer::DrawRectMarker(wxInt32 n, wxPoint points[], const wxSize& markerSize)
	{
		wxCoord width = (wxCoord)(markerSize.GetWidth());
		wxCoord height = (wxCoord)(markerSize.GetHeight());
		wxUint16 widthDiv2 = (wxUint16)(width / 2.f);
		wxUint16 heightDiv2 = (wxUint16)(height / 2.f);
		for (int idel = 0; idel<n; idel++)
		{
			currentContext->DrawRectangle(points[idel].x - widthDiv2, points[idel].y - heightDiv2, width, height);
		}
	}
	void SG_Renderer::DrawLines(const ArrayOfFloats& Yvalues, const ArrayOfFloats& Xvalues)
	{
		DRAWING_METHOD eleMethod = currentElementStyle->GetDrawingMethod();
		DRAWING_MARKER eleMarker = currentElementStyle->GetDrawingMarker();

		wxInt32 sizeOf = Yvalues.GetCount();
		wxPoint* ptList = new wxPoint[sizeOf];
		for (int idel = 0; idel<sizeOf; idel++)
		{
			ptList[idel] = LogicalToDeviceCoord(Xvalues.Item(idel), Yvalues.Item(idel));
		}
		if (eleMethod == DRAWING_METHOD_LINE)
		{
			currentContext->DrawLines(sizeOf, ptList);
		}
		else {
			if (sizeOf>2)
				currentContext->DrawSpline(sizeOf, ptList);
		}
		//Dessin des marqueurs
		DrawMarkers(sizeOf, ptList, eleMarker);

		delete[] ptList;
	}

	void SG_Renderer::DrawEchogram(const ArrayOfFloats& Yvalues, const ArrayOfFloats& Xvalues)
	{
		wxInt32 sizeOf = Yvalues.GetCount();
		wxInt32 zeroPos = LogicalToDeviceCoord(0, 0).y;
		for (int idel = 0; idel<sizeOf; idel++)
		{
			wxPoint histoPixelValue = LogicalToDeviceCoord(Xvalues.Item(idel), Yvalues.Item(idel));
			currentContext->DrawLine(histoPixelValue.x, histoPixelValue.y, histoPixelValue.x, zeroPos);
		}
	}
	void SG_Renderer::DrawHistogram(const ArrayOfFloats& Yvalues, const ArrayOfFloats& Xvalues)
	{
		wxInt32 sizeOf = Yvalues.GetCount();
		wxInt32 rectWidth = LogicalToDeviceCoord(histoGramWidth, 0).x - LogicalToDeviceCoord(0, 0).x;
		wxInt32 rectWidthDiv2 = (wxInt32)(rectWidth / 2.f);
		wxInt32 zeroPos = LogicalToDeviceCoord(0, 0).y;
		for (int idel = 0; idel<sizeOf; idel++)
		{
			wxPoint histoPixelValue = LogicalToDeviceCoord(Xvalues.Item(idel), Yvalues.Item(idel));
			wxRect rectangleShape(histoPixelValue.x - rectWidthDiv2, histoPixelValue.y, rectWidth - 1, zeroPos - histoPixelValue.y);
			DrawRectangle(rectangleShape);
		}
	}
	void SG_Renderer::DrawRectangle(const wxRect& rectArea)
	{
		if (currentElementStyle->GetBrush().GetStyle() != wxBRUSHSTYLE_TRANSPARENT)
		{
			currentContext->SetBrush(wxBrush(currentElementStyle->GetPen().GetColour(), wxBRUSHSTYLE_SOLID));
			currentContext->DrawRectangle(rectArea);
		}
		//Dessin du rect de style
		currentContext->SetBrush(currentElementStyle->GetBrush());
		currentContext->DrawRectangle(rectArea);

	}
	void SG_Renderer::DrawDotsCloud(const ArrayOfFloats& Yvalues, const ArrayOfFloats& Xvalues)
	{
		DRAWING_METHOD eleMethod = currentElementStyle->GetDrawingMethod();
		DRAWING_MARKER eleMarker = currentElementStyle->GetDrawingMarker();

		wxInt32 sizeOf = Yvalues.GetCount();
		wxPoint* ptList = new wxPoint[sizeOf];
		for (int idel = 0; idel<sizeOf; idel++)
		{
			ptList[idel] = LogicalToDeviceCoord(Xvalues.Item(idel), Yvalues.Item(idel));
		}
		//Dessin des marqueurs
		DrawMarkers(sizeOf, ptList, eleMarker);
		delete[] ptList;
	}

	void SG_Renderer::DrawStipples(const ArrayOfFloats& Yvalues, const ArrayOfFloats& Xvalues, bool linkDots)
	{
		wxInt32 height = currentMainStyle->GetCfg(I_PARAM_VIEW_HEIGHT);
		wxPen usedPen(currentElementStyle->GetPen());
		wxBitmap* stippleBitmap = usedPen.GetStipple();
		if (!stippleBitmap->IsOk())
		{
			*stippleBitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(16, 16));
		}
		wxPoint BmpOffset(-(stippleBitmap->GetWidth() / 2.f), -(stippleBitmap->GetHeight() / 2.f));
		wxInt32 sizeOf = Yvalues.GetCount();
		wxPoint from;
		for (int idel = 0; idel<sizeOf; idel++)
		{
			wxPoint to = LogicalToDeviceCoord(Xvalues.Item(idel), Yvalues.Item(idel));
			currentContext->DrawBitmap(*stippleBitmap, to.x + BmpOffset.x, to.y + BmpOffset.y, true);
			if (linkDots)
			{
				if (idel)
					currentContext->DrawLine(from.x, from.y, to.x, to.y);
				from = to;
			}
		}
	}
	wxSize SG_Renderer::GetDeviceSize()
	{
		return wxSize(currentMainStyle->GetCfg(I_PARAM_VIEW_WIDTH), currentMainStyle->GetCfg(I_PARAM_VIEW_HEIGHT));
	}
	wxFloat32  SG_Renderer::GetXOffset()
	{
		return ((currentMainStyle->GetCfg(I_PARAM_VIEW_WIDTH) - 1) / (wxFloat32)(currentMainStyle->GetCfg(F_PARAM_X_MAX) - currentMainStyle->GetCfg(F_PARAM_X_MIN)));
	}
	wxFloat32  SG_Renderer::GetYOffset()
	{
		return ((wxFloat32)(currentMainStyle->GetCfg(I_PARAM_VIEW_HEIGHT) - 1) / (wxFloat32)(currentMainStyle->GetCfg(F_PARAM_Y_MAX) - currentMainStyle->GetCfg(F_PARAM_Y_MIN)));
	}
	wxPoint SG_Renderer::LogicalToDeviceCoord(wxFloat32 xx, wxFloat32 yy)
	{
		return wxPoint(
			(xx - currentMainStyle->GetCfg(F_PARAM_X_MIN))*GetXOffset()
			,
			(currentMainStyle->GetCfg(I_PARAM_VIEW_HEIGHT) - 1) - (yy - currentMainStyle->GetCfg(F_PARAM_Y_MIN))*GetYOffset()
			);
	}
	wxRealPoint SG_Renderer::DeviceToLogicalCoord(wxInt32 xx, wxInt32 yy)
	{
		return wxRealPoint(
			(xx / GetXOffset()) + currentMainStyle->GetCfg(F_PARAM_X_MIN)
			,
			(((currentMainStyle->GetCfg(I_PARAM_VIEW_HEIGHT) - 1) - yy) / GetYOffset()) + currentMainStyle->GetCfg(F_PARAM_Y_MIN)
			);
	}
	void SG_Renderer::End()
	{
		currentElementStyle = NULL;
	}


	void SG_Renderer::DrawLine(wxRealPoint from, wxRealPoint to)
	{
		wxPoint fromDeviceCoords = LogicalToDeviceCoord(from.x, from.y);
		wxPoint toDeviceCoords = LogicalToDeviceCoord(to.x, to.y);
		currentContext->DrawLine(fromDeviceCoords.x, fromDeviceCoords.y, toDeviceCoords.x, toDeviceCoords.y);
	}

	void SG_Renderer::DrawLine(wxPoint from, wxPoint to)
	{
		currentContext->DrawLine(from.x, from.y, to.x, to.y);
	}
	//////////////////////////////////////////////////
	// Définition de SG_Area

	BEGIN_EVENT_TABLE(SG_Area, wxControl)
		EVT_PAINT(SG_Area::OnPaint)
		EVT_SIZE(SG_Area::OnResize)
		EVT_MOUSE_EVENTS(SG_Area::OnMouse)
		EVT_ERASE_BACKGROUND(SG_Area::OnErase)
		END_EVENT_TABLE()

		SG_Area::SG_Area(simpleGraph *parent, AREA_TYPE _areaType, wxSize windowSize)
		:m_owner(parent),
		wxControl(parent, wxID_ANY, wxDefaultPosition, windowSize, wxNO_BORDER | wxCLIP_CHILDREN, wxDefaultValidator, _T("SG_Area")),
		areaType(_areaType),
		backingStoreDc(NULL)
	{
		InheritAttributes();
		wxPen pen = wxPen(*wxBLACK, 1, wxPENSTYLE_DOT);
		zoomAreaHintStyle.SetPen(&pen);
		zoomAreaHintStyle.SetBrush(wxTRANSPARENT_BRUSH);
	}
	SG_Area::~SG_Area()
	{
		delete backingStoreDc;
	}
	void SG_Area::OnErase(wxEraseEvent& evt)
	{

	}

	void SG_Area::OnMouse(wxMouseEvent &event)
	{
		wxPoint mousePos = event.GetPosition();
		if (areaType == AREA_TYPE_MAINGRAPH)
		{
			if (event.LeftIsDown())
			{
				if (event.Dragging())
				{
					OnDragging(mousePos);
				}
				else {
					OnBeginSelection(mousePos);
				}
			}
			else if (event.LeftUp())
			{
				OnEndSelection(mousePos);
			}
		}

		event.Skip();
		if (event.RightUp() || areaType != AREA_TYPE_MAINGRAPH)
			event.ResumePropagation(1);
	}

	void SG_Area::OnBeginSelection(wxPoint mousePos)
	{
		if (lastZoomHintRect == wxRect())
		{
			wxClientDC drawDC(this);
			SG_Renderer drawingRenderer(drawDC, m_owner->parameters);
			DrawZoomHintRectangle(mousePos, false);
			if (lastZoomHintRect.GetWidth() != 0 && lastZoomHintRect.GetHeight() != 0)
				m_owner->OnZoomIn(drawingRenderer.DeviceToLogicalCoord(lastZoomHintRect.GetPosition().x, lastZoomHintRect.GetPosition().y), drawingRenderer.DeviceToLogicalCoord(mousePos.x, mousePos.y));
			lastZoomHintRect = wxRect(mousePos, wxSize(0, 0));
		}
	}
	void SG_Area::OnDragging(wxPoint mousePos)
	{
		DrawZoomHintRectangle(mousePos);
	}

	void SG_Area::OnEndSelection(wxPoint mousePos)
	{
		wxClientDC drawDC(this);
		SG_Renderer drawingRenderer(drawDC, m_owner->parameters);
		DrawZoomHintRectangle(mousePos, false);
		if (lastZoomHintRect.GetWidth() != 0 && lastZoomHintRect.GetHeight() != 0)
			m_owner->OnZoomIn(drawingRenderer.DeviceToLogicalCoord(lastZoomHintRect.GetPosition().x, lastZoomHintRect.GetPosition().y), drawingRenderer.DeviceToLogicalCoord(mousePos.x, mousePos.y));
		lastZoomHintRect = wxRect();
	}

	void SG_Area::DrawZoomHintRectangle(wxPoint newPos, bool drawNewRect)
	{

		wxClientDC drawDC(this);
		SG_Renderer drawingRenderer(drawDC, m_owner->parameters);
		drawingRenderer.Begin(zoomAreaHintStyle);
		drawDC.SetLogicalFunction(wxINVERT);

		//On efface l'ancien rectangle dessiné
		if (lastZoomHintRect.GetWidth() != 0 && lastZoomHintRect.GetHeight() != 0)
			drawDC.DrawRectangle(lastZoomHintRect.GetX(), lastZoomHintRect.GetY(), lastZoomHintRect.GetWidth(), lastZoomHintRect.GetHeight());

		//On dessine le nouveau rectangle

		if (drawNewRect)
		{
			lastZoomHintRect.SetWidth(newPos.x - lastZoomHintRect.GetX());
			lastZoomHintRect.SetHeight(newPos.y - lastZoomHintRect.GetY());
			if (lastZoomHintRect.GetWidth() != 0 && lastZoomHintRect.GetHeight() != 0)
				drawDC.DrawRectangle(lastZoomHintRect.GetX(), lastZoomHintRect.GetY(), lastZoomHintRect.GetWidth(), lastZoomHintRect.GetHeight());
		}
		drawDC.SetLogicalFunction(wxCOPY);
		drawingRenderer.End();

	}

	void SG_Area::Draw(wxDC& thisDc, const wxSize& drawSize)
	{

		//wxBufferedDC thisDc(&directDc);

		thisDc.Clear();

		if (areaType == AREA_TYPE_MAINGRAPH)
		{

			wxInt16 sizeBase = (wxInt16)(drawSize.GetWidth()*(m_owner->parameters.GetCfg(F_PARAM_MARKER_SIZE) / 100));
			m_owner->parameters.SetCfg(I_PARAM_COMPUTED_MARKER_SIZE, sizeBase);
		}

		SG_Renderer drawingRenderer(thisDc, m_owner->parameters, drawSize);
		ArrayOfGraphEl::iterator it;


#ifdef __SG_PROFILING__
		wxDateTime beginT = wxDateTime::UNow();
#endif

		for (it = m_owner->elements.begin(); it != m_owner->elements.end(); ++it)
		{
			if (it->second->drawingArea == areaType && it->second->IsVisible())
			{
				it->second->Draw(this, drawingRenderer);
			}
		}

#ifdef __SG_PROFILING__
		wxLongLong durationOperation = wxDateTime::UNow().GetValue() - beginT.GetValue();
#endif

	}


	void SG_Area::OnResize(wxSizeEvent &event)
	{
		Refresh(false);
	}

	void SG_Area::DrawAntiAliased(wxDC& directDc)
	{
#ifdef __SG_PROFILING__
		wxDateTime beginT = wxDateTime::UNow();
#endif
		wxSize drawRect = directDc.GetSize();
		wxBitmap antiAliasBitmap(drawRect.GetWidth() * 2, drawRect.GetHeight() * 2);
		wxMemoryDC aliasDc(antiAliasBitmap);
		Draw(aliasDc, drawRect * 2);
		aliasDc.SelectObject(wxNullBitmap);
		wxImage operationObject = antiAliasBitmap.ConvertToImage();
		operationObject.Rescale(drawRect.GetWidth(), drawRect.GetHeight(), wxIMAGE_QUALITY_HIGH);
		antiAliasBitmap = wxBitmap(operationObject);
		directDc.DrawBitmap(operationObject, 0, 0);
#ifdef __SG_PROFILING__
		wxLongLong durationOperation = wxDateTime::UNow().GetValue() - beginT.GetValue();
		wxLogMessage(wxGetTranslation("Total draw time %i ms"), durationOperation.GetValue());
#endif
	}

	void SG_Area::OnPaint(wxPaintEvent &event)
	{
		//Si on doit redessiner une partie de la zone de dessin alors on utilise notre dernier rendu
		//Au cas où toute la zone de dessin doit être redessiné on rappel la méthode de dessin
		wxPaintDC tmpDc(this);

		wxClientDC clientDc(this);

		wxRegionIterator upd(GetUpdateRegion()); // get the update rect list
		wxSize sizeGraph = GetClientSize();

		if (upd)
		{
			wxRect rectUpdate(upd.GetRect());
			if (rectUpdate.GetWidth() == sizeGraph.GetWidth() && rectUpdate.GetHeight() == sizeGraph.GetHeight() || backingStoreSize != sizeGraph)
			{ //Si la zone complète du dessin doit être redessiné on execute les fonctions de dessins
				delete backingStoreDc;
				backingStoreDc = new wxBufferedDC(NULL, sizeGraph, wxBUFFER_VIRTUAL_AREA);
				backingStoreSize = sizeGraph;
				if (areaType == AREA_TYPE_MAINGRAPH && m_owner->parameters.isEnable(B_PARAM_USE_ANTIALIASING))
				{
					DrawAntiAliased(*backingStoreDc);
					wxInt16 sizeBase = (wxInt16)(GetClientSize().GetWidth()*(m_owner->parameters.GetCfg(F_PARAM_MARKER_SIZE) / 100));
					m_owner->parameters.SetCfg(I_PARAM_COMPUTED_MARKER_SIZE, sizeBase);
				}
				else {
					Draw(*backingStoreDc, GetClientSize());
				}
				wxSize calculatedSize = DoGetBestSize();
				if (m_bestSizeCache != calculatedSize)
				{
                    CacheBestSize(calculatedSize);
					wxPostEvent(m_owner, wxSizeEvent());
				}
			}
			//Tant que des zone de dessin doivent être affiché on tranfert les données
			while (upd)
			{
				rectUpdate = upd.GetRect();
				clientDc.Blit(rectUpdate.GetX(), rectUpdate.GetY(), rectUpdate.GetWidth(), rectUpdate.GetHeight(), backingStoreDc, rectUpdate.GetX(), rectUpdate.GetY());
				upd++;
			}
		}
	}

	void SG_Area::ScrollWindow(int dx, int dy, const wxRect *rect)
	{
		wxWindow::ScrollWindow(dx, dy, rect);
	}

	wxSize SG_Area::DoGetBestSize() const
	{
		wxSize maxSize(0, 0);
		if (areaType != AREA_TYPE_MAINGRAPH)
		{
			ArrayOfGraphEl::iterator it;
			for (it = m_owner->elements.begin(); it != m_owner->elements.end(); ++it)
			{
				if (it->second->drawingArea == areaType && it->second->IsVisible())
				{
					wxSize bestSizeObj = it->second->GetBestSize();
					if (maxSize.GetHeight()<bestSizeObj.GetHeight())
					{
						maxSize.SetHeight(bestSizeObj.GetHeight());
					}
					if (maxSize.GetWidth()<bestSizeObj.GetWidth())
					{
						maxSize.SetWidth(bestSizeObj.GetWidth());
					}

				}
			}
            wxInt16 ticSize = m_owner->parameters.GetCfg(I_PARAM_TIC_SIZE);
            if (areaType == AREA_TYPE_X_AXIS)
                maxSize.SetHeight(maxSize.GetHeight() + ticSize +  2); // Add tic size and offset
            else
                maxSize.SetWidth(maxSize.GetWidth() + ticSize + 2); // Add tic size and offset

		}

		return maxSize;
	}

	//////////////////////////////////////////////////
	// Définition de SG_Legend

	BEGIN_EVENT_TABLE(SG_Legend, wxControl)
		EVT_PAINT(SG_Legend::OnPaint)
		EVT_MOUSE_EVENTS(SG_Legend::OnMouse)
		EVT_ERASE_BACKGROUND(SG_Legend::OnErase)
		END_EVENT_TABLE()

		SG_Legend::SG_Legend(simpleGraph* parent, LEGEND_PLACEMENT _whereToInsert, wxWindowID id, const wxPoint& pos,
			const wxSize& size, long style)
		:
		wxControl(parent, id, pos, size, style | wxNO_BORDER, wxDefaultValidator, _T("legend_area")), //| ((whereToInsert==LEGEND_PLACEMENT_INSIDE_MAINGRAPH) ? wxTRANSPARENT_WINDOW : 0)
		m_owner(parent),
		whereToInsert(_whereToInsert)
	{
        this->SetBackgroundColour(*wxWHITE);
        CacheBestSize(wxSize(5, 5));
	}


    LEGEND_PLACEMENT  SG_Legend::GetLegendPlacement() const {
        return whereToInsert;
	}


    void SG_Legend::Prepend(SG_Legend* legend, const wxSizerFlags& sizerParams) {
        static_cast<simpleGraph*>(legend->GetParent())->GetSizer(legend->GetLegendPlacement())->Prepend(legend, sizerParams);
	}

    /** Attach Legend with graph after other components */
    void SG_Legend::Add(SG_Legend* legend, const wxSizerFlags& sizerParams) {
        static_cast<simpleGraph*>(legend->GetParent())->GetSizer(legend->GetLegendPlacement())->Add(legend, sizerParams);
	}

	wxSize SG_Legend::DoGetBestSize() const
	{
		return m_bestSizeCache;
	}

	void SG_Legend::OnErase(wxEraseEvent& evt)
	{
        evt.Skip();
	}
	void SG_Legend::drawLegend(wxDC& drawDC)
	{

		wxSize anscalculatedAreaSize = m_bestSizeCache;
		drawDC.Clear();
		SG_Renderer drawingRenderer(drawDC, m_owner->parameters);
		ArrayOfGraphEl::iterator it;
		wxInt32 pictSize = m_owner->parameters.GetCfg(I_PARAM_LEGEND_ICON_SIZE);
		wxPoint currentPenPosition(5, 5);
        wxSize calculatedAreaSize = wxSize(5, 5);
		if (!m_owner->parameters.isEnable(B_PARAM_HIDE_LEGEND))
		{
			for (it = m_owner->elements.begin(); it != m_owner->elements.end(); ++it)
			{
				if (it->second->drawingArea == AREA_TYPE_MAINGRAPH && it->second->IsVisible())
				{
					currentPenPosition.x = 1;
					//Dessin de la partie de la legende
					wxString elementLabel = it->second->GetLabel();
					drawingRenderer.Begin(it->second->el_style);
					wxSize labelSize(drawingRenderer.GetTextDeviceSize(elementLabel));
					wxInt32 legendH = wxMax(pictSize, labelSize.GetHeight());
					currentPenPosition.y += (wxInt32)(legendH / 2.f);
					//Dessin de l'apercu du tracé
					it->second->DrawSample(drawingRenderer, wxRect(wxPoint(currentPenPosition.x, currentPenPosition.y - pictSize / 2.f), wxSize(pictSize, pictSize)));
					drawingRenderer.Begin(it->second->el_style);
					currentPenPosition.x += pictSize + 1;
					//Dessin du texte
					drawingRenderer.DrawText(wxPoint(currentPenPosition.x, currentPenPosition.y - labelSize.GetHeight() / 2.f), elementLabel);
					currentPenPosition.x += labelSize.GetWidth();
					//On se déplace vers le bas
					currentPenPosition.y += (wxInt32)(legendH / 2.f) + 1;

					drawingRenderer.End();
					calculatedAreaSize.SetWidth(wxMax(calculatedAreaSize.GetWidth(), currentPenPosition.x));
				}
				calculatedAreaSize.SetHeight(currentPenPosition.y);
			}
		}
		if (calculatedAreaSize != anscalculatedAreaSize)
		{
            CacheBestSize(calculatedAreaSize);
			wxSizeEvent szevt;
			m_owner->GetEventHandler()->ProcessEvent(szevt);
		}
	}

	void SG_Legend::OnPaint(wxPaintEvent &event)
	{
		wxPaintDC paintDc(this);
		wxClientDC clientDc(this);
		if (!IsDoubleBuffered())
		{
			wxBufferedDC tmpDc(&clientDc);
			drawLegend(tmpDc);
		}
		else {
			drawLegend(clientDc);
		}
	}
	void SG_Legend::OnMouse(wxMouseEvent &event)
	{
		event.Skip();
		event.ResumePropagation(1);
	}
	//////////////////////////////////////////////////
	// Définition de simpleGraph

	BEGIN_EVENT_TABLE(simpleGraph, wxScrolledWindow)
		EVT_MOUSE_EVENTS(simpleGraph::OnMouseGeneral)
		END_EVENT_TABLE()

		simpleGraph::simpleGraph(wxWindow* parent, wxWindowID id, const wxPoint& pos,
			const wxSize& size, long style, const wxString& name)
		:wxScrolledWindow(parent, id, pos, size, style, name),
		parameters(this),
		constructionFinish(false)
	{
		parameters.Init();
		elCounter = 0;
		////////////////////////////////////////////////////////
		this->SetBackgroundColour(*wxWHITE);

		mainsizer = new wxBoxSizer(wxHORIZONTAL);

		plotsizer = new wxFlexGridSizer(2, 2, parameters.GetCfg(I_PARAM_X_BORDER_AXIS), parameters.GetCfg(I_PARAM_Y_BORDER_AXIS));


		//Add sizer to hold the title and plot.
		//Title to be added later.
		m_plotAndTitleSizer = new wxBoxSizer(wxVERTICAL);
		m_plotAndTitleSizer->Add(plotsizer, 1, wxEXPAND);

		plotsizer->Add(new SG_Area(this, AREA_TYPE_Y_AXIS), wxSizerFlags(0).Expand());
		mainGraphArea = new SG_Area(this, AREA_TYPE_MAINGRAPH);
		plotsizer->Add(mainGraphArea, wxSizerFlags(1).Expand());


		cellSizer = new wxBoxSizer(wxHORIZONTAL);
		plotsizer->Add(cellSizer, 1, wxEXPAND);
		plotsizer->Add(new SG_Area(this, AREA_TYPE_X_AXIS), wxSizerFlags(0).Expand());

		plotsizer->AddGrowableRow(0);
		plotsizer->AddGrowableCol(1);

		mainsizer->Add(m_plotAndTitleSizer, 1, wxEXPAND);

		SetSizerAndFit(mainsizer);

		SetScrollbars(1, 1, 100, 100);
		constructionFinish = true;
	}

	simpleGraph::~simpleGraph()
	{

		ArrayOfGraphEl::iterator it;
		for (it = elements.begin(); it != elements.end(); ++it)
		{
			SG_Element* drawingEl = it->second;
			it->second = NULL;
			delete drawingEl;
		}
	}
	wxInt32 simpleGraph::AddDrawingElement(SG_Element* drawingEl)
	{
		elements[elCounter] = drawingEl;
		return elCounter++;
	}

	void simpleGraph::OnZoomIn(const wxRealPoint& beginZone, const wxRealPoint& endZone)
	{
		//wxLogDebug("Zoom in (%g ; %g) to (%g ; %g)",beginZone.x,beginZone.y,endZone.x,endZone.y);
		parameters.SetCfg(F_PARAM_X_MIN, wxMin(beginZone.x, endZone.x));
		parameters.SetCfg(F_PARAM_X_MAX, wxMax(beginZone.x, endZone.x));
		parameters.SetCfg(F_PARAM_Y_MIN, wxMin(beginZone.y, endZone.y));
		parameters.SetCfg(F_PARAM_Y_MAX, wxMax(beginZone.y, endZone.y));
		Refresh(false);
	}
	void simpleGraph::OnZoomOut()
	{
		wxRealPoint oldBeginZone(parameters.GetCfg(F_PARAM_X_MIN), parameters.GetCfg(F_PARAM_Y_MIN));
		wxRealPoint oldEndZone(parameters.GetCfg(F_PARAM_X_MAX), parameters.GetCfg(F_PARAM_Y_MAX));
		ZoomFit();
		//Si le zoom a changé on redessine
		if (oldBeginZone != wxRealPoint(parameters.GetCfg(F_PARAM_X_MIN), parameters.GetCfg(F_PARAM_Y_MIN)) || oldEndZone != wxRealPoint(parameters.GetCfg(F_PARAM_X_MAX), parameters.GetCfg(F_PARAM_Y_MAX)))
			Refresh(false);
	}

	wxSizer* simpleGraph::GetSizer(LEGEND_PLACEMENT whereToInsert)
	{
		wxSizer* sizerChoice;
		switch (whereToInsert)
		{
		case LEGEND_PLACEMENT_VERTICAL:
			return m_plotAndTitleSizer;
			break;
		case LEGEND_PLACEMENT_BOTTOM_LEFT:
			return cellSizer;
			break;
		default:
			return mainsizer;
			break;
		}
	}
	void simpleGraph::ZoomFit()
	{
		//Modification des paramètre de zoom

		//Si premier élément
		wxFloat32 xMin, xMax, yMin, yMax;
		wxFloat32 _xMin, _xMax, _yMin, _yMax;
		bool first = true;

		ArrayOfGraphEl::iterator it;
		for (it = elements.begin(); it != elements.end(); ++it)
		{
			SG_Element* drawingEl = it->second;
			if (drawingEl->drawingArea == AREA_TYPE_MAINGRAPH && drawingEl->IsVisible())
			{
				if (first)
				{
					drawingEl->GetMinMax(xMin, xMax, yMin, yMax);
					first = false;
				}
				else {
					drawingEl->GetMinMax(_xMin, _xMax, _yMin, _yMax);
					xMin = wxMin(xMin, _xMin);
					xMax = wxMax(xMax, _xMax);
					yMin = wxMin(yMin, _yMin);
					yMax = wxMax(yMax, _yMax);
				}
			}
		}
		wxFloat32 rangeY = yMax - yMin;
		parameters.SetCfg(F_PARAM_X_MIN, xMin);
		parameters.SetCfg(F_PARAM_X_MAX, xMax);
		parameters.SetCfg(F_PARAM_Y_MIN, yMin);
		parameters.SetCfg(F_PARAM_Y_MAX, yMax);

	}

	void simpleGraph::DeleteDrawingElement(wxInt32 indexEl)
	{
		if (elements.find(indexEl) != elements.end())
		{
			SG_Element* drawingEl = elements[indexEl];
			elements[indexEl] = NULL;
			delete drawingEl;
			elements.erase(indexEl);
		}
	}

	SG_Element* simpleGraph::GetDrawingElement(wxInt32 indexEl)
	{
		if (elements.find(indexEl) != elements.end())
		{
			return elements[indexEl];
		}
		else {
			return NULL;
		}
	}

	ArrayOfGraphEl::iterator simpleGraph::Begin()
	{
		return elements.begin();
	}
	ArrayOfGraphEl::iterator simpleGraph::End()
	{
		return elements.end();
	}
	void simpleGraph::OnUpdate(I_PARAM indexParam)
	{
		if (constructionFinish)
		{
			bool sizerUpdated = false;
			if (indexParam == I_PARAM_X_BORDER_AXIS)
			{
				plotsizer->SetHGap(parameters.GetCfg(I_PARAM_X_BORDER_AXIS));
				sizerUpdated = true;
			}
			else if (indexParam == I_PARAM_Y_BORDER_AXIS)
			{
				plotsizer->SetVGap(parameters.GetCfg(I_PARAM_Y_BORDER_AXIS));
				sizerUpdated = true;
			}

			if (sizerUpdated)
			{
				plotsizer->Layout();
			}
		}

	}

	void simpleGraph::OnMouseGeneral(wxMouseEvent &event)
	{

		wxPoint mousePos = event.GetPosition();
		//Transformation des coordonnées entre la position de la fenetre effective et celle du graphique general :

		wxWindow* associatedWindow = wxDynamicCast(event.GetEventObject(), wxWindow);

		mousePos = mousePos + (associatedWindow->GetScreenPosition() - mainGraphArea->GetScreenPosition());
		if (event.LeftIsDown())
		{
			if (event.Dragging())
				mainGraphArea->OnDragging(mousePos);
			else
				mainGraphArea->OnBeginSelection(mousePos);
		}
		else if (event.LeftUp())
		{
			mainGraphArea->OnEndSelection(mousePos);
		}
		else if (event.RightUp() && parameters.isEnable(B_PARAM_RIGHT_CLICK_ZOOM_FIT))
		{
			OnZoomOut();
		}
		event.Skip();
	}
	void simpleGraph::OnUpdate(F_PARAM indexParam)
	{
		if (constructionFinish)
		{
		}
	}
	void simpleGraph::OnUpdate(B_PARAM indexParam)
	{
		if (constructionFinish)
		{
		}
	}
}
