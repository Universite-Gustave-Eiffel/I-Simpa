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

#include "gridcellgabefloatrenderer.hpp"
#include <wx/log.h>
#include <float.h>
#include <std_tools.hpp>
#include "last_cpp_include.hpp"

// ----------------------------------------------------------------------------
// wxGridCellGabeFloatRenderer
// ----------------------------------------------------------------------------

wxGridCellGabeFloatRenderer::wxGridCellGabeFloatRenderer(int precision)
{
    SetPrecision(precision);
}

wxGridCellRenderer *wxGridCellGabeFloatRenderer::Clone() const
{
    wxGridCellGabeFloatRenderer *renderer = new wxGridCellGabeFloatRenderer;
    renderer->m_precision = m_precision;
    return renderer;
}

wxString wxGridCellGabeFloatRenderer::GetString(const wxGrid& grid, int row, int col)
{
    wxGridTableBase *table = grid.GetTable();

    bool hasDouble;
    double val;
    wxString text;
    if ( table->CanGetValueAs(row, col, wxGRID_VALUE_FLOAT) )
    {
        val = table->GetValueAsDouble(row, col);
        hasDouble = true;
    }
    else
    {
        text = table->GetValue(row, col);
        hasDouble = text.ToDouble(&val);
    }

    if ( hasDouble )
    {
		if(st_isfinite(val))
		{
			text=wxString::Format("%."+wxString::Format("%i",m_precision)+"f",val);
		}else{
			if(val>0)
				text="+Inf";
			else
				text="-Inf";
		}
    }
    //else: text already contains the string

    return text;
}

void wxGridCellGabeFloatRenderer::Draw(wxGrid& grid,
                                   wxGridCellAttr& attr,
                                   wxDC& dc,
                                   const wxRect& rectCell,
                                   int row, int col,
                                   bool isSelected)
{
    wxGridCellRenderer::Draw(grid, attr, dc, rectCell, row, col, isSelected);

    SetTextColoursAndFont(grid, attr, dc, isSelected);

    // draw the text right aligned by default
    int hAlign, vAlign;
    attr.GetAlignment(&hAlign, &vAlign);
    hAlign = wxALIGN_RIGHT;

    wxRect rect = rectCell;
    rect.Inflate(-1);

    grid.DrawTextRectangle(dc, GetString(grid, row, col), rect, hAlign, vAlign);
}

wxSize wxGridCellGabeFloatRenderer::GetBestSize(wxGrid& grid,
                                            wxGridCellAttr& attr,
                                            wxDC& dc,
                                            int row, int col)
{
    return DoGetBestSize(attr, dc, GetString(grid, row, col));
}

void wxGridCellGabeFloatRenderer::SetParameters(const wxString& params)
{
    if ( !params )
    {
        // reset to defaults
        SetPrecision(-1);
    }
    else
    {
		long precision;
        if ( params.ToLong(&precision) )
        {
            SetPrecision((int)precision);
        }
        else
        {
            wxLogDebug(_T("Invalid wxGridCellGabeFloatRenderer precision parameter string '%s ignored"), params.c_str());
        }
    }
}
