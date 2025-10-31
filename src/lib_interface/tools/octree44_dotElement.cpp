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

/*
 * Code source �crit dans le cadre du projet OPALHA contrat ADEME n�06.04.C.0070 (2007)
 */
#include "octree44_dotElement.hpp"

#include <math.h>

namespace octreeTool
{


	////////////////////////////////////////////////////////////////////////////////////////////////
	//// Impl�mentation de triangleElement
	////////////////////////////////////////////////////////////////////////////////////////////////

	dotElement::dotElement(coordPrecision position[3])

	{
		Set(position);
	}
	void dotElement::Set(coordPrecision position[3])
	{
		COPYVEC(position,CoordPosition);
	}

	coordPrecision dotElement::GetPositionX( )
	{
		return CoordPosition[0];
	}
	coordPrecision dotElement::GetPositionY( )
	{
		return CoordPosition[1];
	}
	coordPrecision dotElement::GetPositionZ( )
	{
		return CoordPosition[2];
	}
	bool dotElement::IsOverlap(coordPrecision boxcenter[3],const coordPrecision& boxhalfsize)
	{
		//Test X Y Z
		if(fabs(boxcenter[0]-CoordPosition[0])>boxhalfsize)
			return false;
		if(fabs(boxcenter[1]-CoordPosition[1])>boxhalfsize)
			return false;
		if(fabs(boxcenter[2]-CoordPosition[2])>boxhalfsize)
			return false;
		return true;
	}
	void dotElement::GetMinMax(coordPrecision xyzMin[3],coordPrecision xyzMax[3])
	{
		COPYVEC(CoordPosition,xyzMin);
		COPYVEC(CoordPosition,xyzMax);
	}
}
