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
		arrayToFeed.Add(t_LstEnums(_("Courbe"),DRAWING_METHOD_LINE));
		arrayToFeed.Add(t_LstEnums(_("Histogramme"),DRAWING_METHOD_COLS));
		arrayToFeed.Add(t_LstEnums(_("Echogramme"),DRAWING_METHOD_ECHOGRAM));
		arrayToFeed.Add(t_LstEnums(_("Nuage d'images"),DRAWING_METHOD_STIPPLE));
		arrayToFeed.Add(t_LstEnums(_("Nuage de marqueurs"),DRAWING_METHOD_DOT_CLOUD));
		arrayToFeed.Add(t_LstEnums(_("Courbe arrondi (méthode des splines)"),DRAWING_METHOD_SPLINE));
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
		arrayToFeed.Add(t_LstEnums(_("Transparent"),wxTRANSPARENT));
		arrayToFeed.Add(t_LstEnums(_("Trait continu"),wxSOLID));
		arrayToFeed.Add(t_LstEnums(_("Point"),wxDOT));
		arrayToFeed.Add(t_LstEnums(_("Tiret diagonale (gauche)"),wxBDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Croix"),wxCROSS_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Trait horizontal"),wxHORIZONTAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Trait vertical"),wxVERTICAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Trait long et point"),wxDOT_DASH));
		arrayToFeed.Add(t_LstEnums(_("Tirets court"),wxSHORT_DASH));
		arrayToFeed.Add(t_LstEnums(_("Tirets longs"),wxLONG_DASH));
	}
	void SG_EnumeratorFiller::FillBrushStyleEnumeration(StyleArray& arrayToFeed)
	{
		arrayToFeed.Add(t_LstEnums(_("Transparent"),wxTRANSPARENT));
		arrayToFeed.Add(t_LstEnums(_("Plein"),wxSOLID));
		arrayToFeed.Add(t_LstEnums(_("Tiret diagonale (gauche)"),wxBDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Croix"),wxCROSSDIAG_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Tiret diagonale (droite)"),wxFDIAGONAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Croix 2"),wxCROSS_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Trait horizontal"),wxHORIZONTAL_HATCH));
		arrayToFeed.Add(t_LstEnums(_("Trait vertical"),wxVERTICAL_HATCH));
	}
	void SG_EnumeratorFiller::FillMarkersStyleEnumeration(StyleArray& arrayToFeed)
	{
		arrayToFeed.Add(t_LstEnums(_("Aucun"),DRAWING_MARKER_NONE));
		arrayToFeed.Add(t_LstEnums(_("Rond"),DRAWING_MARKER_DOT));
		arrayToFeed.Add(t_LstEnums(_("Carré"),DRAWING_MARKER_SQUARE));
		arrayToFeed.Add(t_LstEnums(_("Croix diagonale"),DRAWING_MARKER_CROSS_D));
	}
}