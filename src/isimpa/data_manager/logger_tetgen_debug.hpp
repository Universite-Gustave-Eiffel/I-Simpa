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

/**
 * @file inter_logger.hpp
 * This file define the interface methods to retrieve processManager messages.
 */
#include "first_header_include.hpp"
#include "manager/interf_logger.hpp"
#include <wx/string.h>
#include <vector>
#include <wx/regex.h>

#ifndef __TETGEINTERFACE_LOGGER__
#define __TETGEINTERFACE_LOGGER__
class TetgenDebugLogger : public InterfLogger
{
private:
	std::vector<int> faceIntersect;
	wxRegEx invalidPolyHeader;
	wxRegEx intersectTriHeader;
public:
	TetgenDebugLogger()
		:  invalidPolyHeader(wxT("  Facet \\(([0-9]*), ([0-9]*), ([0-9]*)\\) \\(([0-9]*)\\) is not a valid polygon.")),
		 intersectTriHeader(wxT(" Facet #([0-9]*) intersects facet #([0-9]*) at triangles:"))
	{

	}
	virtual void LogMessage(const wxString& mess)
	{
		if(invalidPolyHeader.Matches(mess))
		{
			const wxString& sfaceInd(invalidPolyHeader.GetMatch(mess,4));
			long faceInd(0);
			if(sfaceInd.ToLong(&faceInd))
				faceIntersect.push_back(faceInd);
		}else if(intersectTriHeader.Matches(mess))
		{
			const wxString& sfaceInd(intersectTriHeader.GetMatch(mess,1));
			long faceInd(0);
			if(sfaceInd.ToLong(&faceInd))
				faceIntersect.push_back(faceInd);
			const wxString& sface2Ind(intersectTriHeader.GetMatch(mess,2));
			if(sface2Ind.ToLong(&faceInd))
				faceIntersect.push_back(faceInd);
		}
	}
	virtual void LogError(const wxString& mess)
	{

	}
	virtual void LogWarning(const wxString& mess)
	{

	}
	std::vector<int>& GetFaces()
	{
		return faceIntersect;
	}
};
#endif
