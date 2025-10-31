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


#include "simpleGraphEnumerators.h"
#include <wx/arrimpl.cpp> 
#include "simpleGraph.h"
#include "last_cpp_include.hpp"

namespace sgSpace
{
	WX_DEFINE_OBJARRAY(StyleArray);

	void SG_EnumeratorFiller::FillStyleEnumeration(StyleArray& arrayToFeed)
	{
		//Chargement des styles de tracés
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Curve"),DRAWING_METHOD_LINE));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Histogram"),DRAWING_METHOD_COLS));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Echogram"),DRAWING_METHOD_ECHOGRAM));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Scatter image"),DRAWING_METHOD_STIPPLE));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Scatter marker"),DRAWING_METHOD_DOT_CLOUD));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Spline"),DRAWING_METHOD_SPLINE));
	}

	const wxString& SG_EnumeratorFiller::FindStringWithIndex( StyleArray* dataSrc , wxInt32 enumIndex)
	{
		for(wxInt32 idligne=0;idligne<dataSrc->GetCount();idligne++)
		{
			if(dataSrc->Item(idligne).enumId==enumIndex)
				return dataSrc->Item(idligne).lblEnum;
		}
		return dataSrc->Item(0).lblEnum;
	}
	void SG_EnumeratorFiller::FillPenStyleEnumeration(StyleArray& arrayToFeed)
	{
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Transparent"),wxPENSTYLE_TRANSPARENT));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Continuous line"), wxPENSTYLE_SOLID));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Point"), wxPENSTYLE_DOT));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Left diagonal dash"), wxPENSTYLE_BDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Cross"), wxPENSTYLE_CROSS_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Horizontal line"), wxPENSTYLE_HORIZONTAL_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Vertical line"), wxPENSTYLE_VERTICAL_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Dotted-dashed line"), wxPENSTYLE_DOT_DASH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Small dash"), wxPENSTYLE_SHORT_DASH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Long dash"), wxPENSTYLE_LONG_DASH));
	}
	void SG_EnumeratorFiller::FillBrushStyleEnumeration(StyleArray& arrayToFeed)
	{
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Transparent"), wxBRUSHSTYLE_TRANSPARENT));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Full"), wxBRUSHSTYLE_SOLID));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Left diagonal dash"), wxBRUSHSTYLE_BDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Cross"), wxBRUSHSTYLE_CROSSDIAG_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Right diagonal dash"), wxBRUSHSTYLE_FDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Plus"), wxBRUSHSTYLE_CROSS_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Horizontal line"), wxBRUSHSTYLE_HORIZONTAL_HATCH));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Vertical line"), wxBRUSHSTYLE_VERTICAL_HATCH));
	}
	void SG_EnumeratorFiller::FillMarkersStyleEnumeration(StyleArray& arrayToFeed)
	{
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("None"),DRAWING_MARKER_NONE));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Circle"),DRAWING_MARKER_DOT));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Square"),DRAWING_MARKER_SQUARE));
		arrayToFeed.Add(t_LstEnums(wxGetTranslation("Cross"),DRAWING_MARKER_CROSS_D));
	}
}
