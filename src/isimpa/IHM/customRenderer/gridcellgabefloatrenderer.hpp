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
#include <wx/grid.h>

#ifndef __GRID_CELL_GABE_RENDERER__
#define __GRID_CELL_GABE_RENDERER__

class wxGridCellGabeFloatRenderer : public wxGridCellStringRenderer
{
public:
    wxGridCellGabeFloatRenderer(int precision = -1);

    // get/change formatting parameters
    int GetPrecision() const { return m_precision; }
    void SetPrecision(int precision) 
	 {
		 if(m_precision != precision)
		 {
			 m_precision = precision;
		 }	
	}

    // draw the string right aligned with given width/precision
    virtual void Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected);

    virtual wxSize GetBestSize(wxGrid& grid,
                               wxGridCellAttr& attr,
                               wxDC& dc,
                               int row, int col);

    // parameters string format is "precision"
    virtual void SetParameters(const wxString& params);

    virtual wxGridCellRenderer *Clone() const;

protected:
    virtual wxString GetString(const wxGrid& grid, int row, int col);

private:
    // formatting parameters
    short m_precision;
};

#endif