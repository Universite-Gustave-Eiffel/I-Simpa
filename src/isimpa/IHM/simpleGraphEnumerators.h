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

#include <wx/dynarray.h>
#include <wx/string.h>

#ifndef __SIMPLEGRAPH_ENUMERATORS__
#define __SIMPLEGRAPH_ENUMERATORS__

namespace sgSpace
{
	/**
	 * @brief Permet d'associer un nombre à une chaine de caractère
	 *
	 */
	struct t_LstEnums
	{
		t_LstEnums(const wxString& _lblEnum,wxInt32 _enumId):lblEnum(_lblEnum),enumId(_enumId){}
		wxString lblEnum;
		wxInt32 enumId;
	};
	WX_DECLARE_OBJARRAY(t_LstEnums, StyleArray);

	class SG_EnumeratorFiller
	{
	public:
		static void FillStyleEnumeration(StyleArray& arrayToFeed);
		static void FillPenStyleEnumeration(StyleArray& arrayToFeed);
		static void FillBrushStyleEnumeration(StyleArray& arrayToFeed);
		static void FillMarkersStyleEnumeration(StyleArray& arrayToFeed);
		static const wxString& FindStringWithIndex( sgSpace::StyleArray* dataSrc , wxInt32 enumIndex);
	};
}
#endif