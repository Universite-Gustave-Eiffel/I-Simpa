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
		arrayToFeed.Add(t_LstEnums(_("Curve"),DRAWING_METHOD_LINE));
		arrayToFeed.Add(t_LstEnums(_("Histogram"),DRAWING_METHOD_COLS));
		arrayToFeed.Add(t_LstEnums(_("Echogram"),DRAWING_METHOD_ECHOGRAM));
		arrayToFeed.Add(t_LstEnums(_("Scatter image"),DRAWING_METHOD_STIPPLE));
		arrayToFeed.Add(t_LstEnums(_("Scatter marker"),DRAWING_METHOD_DOT_CLOUD));
		arrayToFeed.Add(t_LstEnums(_("Spline"),DRAWING_METHOD_SPLINE));
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
		arrayToFeed.Add(t_LstEnums(_("Transparent"),wxPENSTYLE_TRANSPARENT));
		arrayToFeed.Add(t_LstEnums(_("Continuous line"), wxPENSTYLE_SOLID));
		arrayToFeed.Add(t_LstEnums(_("Point"), wxPENSTYLE_DOT));
		arrayToFeed.Add(t_LstEnums(_("Left diagonal dash"), wxPENSTYLE_BDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Cross"), wxPENSTYLE_CROSS_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Horizontal line"), wxPENSTYLE_HORIZONTAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Vertical line"), wxPENSTYLE_VERTICAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Dotted-dashed line"), wxPENSTYLE_DOT_DASH));
		arrayToFeed.Add(t_LstEnums(_("Small dash"), wxPENSTYLE_SHORT_DASH));
		arrayToFeed.Add(t_LstEnums(_("Long dash"), wxPENSTYLE_LONG_DASH));
	}
	void SG_EnumeratorFiller::FillBrushStyleEnumeration(StyleArray& arrayToFeed)
	{
		arrayToFeed.Add(t_LstEnums(_("Transparent"), wxBRUSHSTYLE_TRANSPARENT));
		arrayToFeed.Add(t_LstEnums(_("Full"), wxBRUSHSTYLE_SOLID));
		arrayToFeed.Add(t_LstEnums(_("Left diagonal dash"), wxBRUSHSTYLE_BDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Cross"), wxBRUSHSTYLE_CROSSDIAG_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Right diagonal dash"), wxBRUSHSTYLE_FDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Plus"), wxBRUSHSTYLE_CROSS_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Horizontal line"), wxBRUSHSTYLE_HORIZONTAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Vertical line"), wxBRUSHSTYLE_VERTICAL_HATCH));
	}
	void SG_EnumeratorFiller::FillMarkersStyleEnumeration(StyleArray& arrayToFeed)
	{
		arrayToFeed.Add(t_LstEnums(_("None"),DRAWING_MARKER_NONE));
		arrayToFeed.Add(t_LstEnums(_("Circle"),DRAWING_MARKER_DOT));
		arrayToFeed.Add(t_LstEnums(_("Square"),DRAWING_MARKER_SQUARE));
		arrayToFeed.Add(t_LstEnums(_("Cross"),DRAWING_MARKER_CROSS_D));
	}
}
