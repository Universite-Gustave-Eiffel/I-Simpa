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

/*
 * Code source écrit dans le cadre du projet OPALHA contrat ADEME n°06.04.C.0070 (2007)
 */
#include "octree44_sphereElement.hpp"

#include <math.h>

namespace octreeTool
{

	#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
	#define SUB(dest,v1,v2) \
			  dest[0]=v1[0]-v2[0]; \
			  dest[1]=v1[1]-v2[1]; \
			  dest[2]=v1[2]-v2[2];
	#define ADD(dest,v1,v2) \
			  dest[0]=v1[0]+v2[0]; \
			  dest[1]=v1[1]+v2[1]; \
			  dest[2]=v1[2]+v2[2];

	////////////////////////////////////////////////////////////////////////////////////////////////
	//// Implémentation de triangleElement
	////////////////////////////////////////////////////////////////////////////////////////////////

	sphereElement::sphereElement(const coordPrecision position[3],const coordPrecision& _radius)

	{
		Set(position);
		SetRadius(_radius);
	}
	sphereElement::sphereElement()
	{
		SETVEC(0,CoordPosition);
		radius=1.;
	}
	void sphereElement::Set(const coordPrecision position[3])
	{
		COPYVEC(position,CoordPosition);
	}

	void sphereElement::SetRadius(const coordPrecision& _radius)
	{
		radius=_radius;
	}

	coordPrecision sphereElement::GetPositionX( )
	{
		return CoordPosition[0];
	}
	coordPrecision sphereElement::GetPositionY( )
	{
		return CoordPosition[1];
	}
	coordPrecision sphereElement::GetPositionZ( )
	{
		return CoordPosition[2];
	}
	coordPrecision sphereElement::GetRadius( )
	{
		return radius;
	}
	bool sphereElement::IsOverlap(coordPrecision boxcenter[3],const coordPrecision& boxhalfsize)
	{

		//Test X Y Z
		if(fabs(boxcenter[0]-CoordPosition[0])>boxhalfsize+radius)
			return false;
		if(fabs(boxcenter[1]-CoordPosition[1])>boxhalfsize+radius)
			return false;
		if(fabs(boxcenter[2]-CoordPosition[2])>boxhalfsize+radius)
			return false;
		return true;

		/*
		//Calcul de la longueur entre boxcenter et CoordPosition
		coordPrecision BS[3];
		SUB(BS,boxcenter,CoordPosition);
		coordPrecision length=sqrtf(DOT(BS,BS));

		if(length>boxhalfsize+radius)
			return false;
		return true;
		*/
	}
	void sphereElement::GetMinMax(coordPrecision xyzMin[3],coordPrecision xyzMax[3])
	{
		coordPrecision tmp[3];
		SETVEC(radius,tmp)
		SUB(xyzMin,CoordPosition,tmp);
		ADD(xyzMax,CoordPosition,tmp);
	}

}
