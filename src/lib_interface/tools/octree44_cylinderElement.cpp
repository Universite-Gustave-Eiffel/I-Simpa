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
#include "octree44_cylinderElement.hpp"

#include <math.h>


#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])


#define SUB(dest,v1,v2) \
		  dest[0]=v1[0]-v2[0]; \
		  dest[1]=v1[1]-v2[1]; \
		  dest[2]=v1[2]-v2[2];
#define ADD(dest,v1,v2) \
		  dest[0]=v1[0]+v2[0]; \
		  dest[1]=v1[1]+v2[1]; \
		  dest[2]=v1[2]+v2[2];

#define MULTBYVEC(dest,v1,v2) \
		  dest[0]=v1[0]*v2[0]; \
		  dest[1]=v1[1]*v2[1]; \
		  dest[2]=v1[2]*v2[2];
#define MULTBYFLOAT(dest,v1,f2) \
		  dest[0]=v1[0]*f2; \
		  dest[1]=v1[1]*f2; \
		  dest[2]=v1[2]*f2;

namespace octreeTool
{
	cylinderElement::cylinderElement(const coordPrecision positionA[3],const coordPrecision positionB[3],const float& _radius)
	{
		Set(positionA,positionB,radius);
	}
	cylinderElement::cylinderElement( )
	{
		SETVEC(0,CoordPositionA);
		SETVEC(0,CoordPositionB);
		radius=0;
	}
	bool cylinderElement::IsOverlap(coordPrecision boxcenter[3],const coordPrecision& boxhalfsize)
	{
		//Todo am�liorer test
		float res[3];
		closestPointOnSegment(boxcenter,CoordPositionA,CoordPositionB,res);

		float rad_cub=sqrtf(2)*boxhalfsize;

		double dx = res[0] - boxcenter[0];
		double dy = res[1] - boxcenter[1];
		double dz = res[2] - boxcenter[2];
		float distBetweenShortestPointAndCenterCube=(float)sqrt( dx * dx + dy * dy + dz * dz );
		if(distBetweenShortestPointAndCenterCube>rad_cub+radius)
			return false;
		return true;
		
	}

	void cylinderElement::GetMinMax(coordPrecision xyzMin[3],coordPrecision xyzMax[3])
	{
		xyzMin[0]=MIN(CoordPositionA[0],CoordPositionB[0]);
		xyzMin[1]=MIN(CoordPositionA[1],CoordPositionB[1]);
		xyzMin[2]=MIN(CoordPositionA[2],CoordPositionB[2]);
		xyzMax[0]=MAX(CoordPositionA[0],CoordPositionB[0]);
		xyzMax[1]=MAX(CoordPositionA[1],CoordPositionB[1]);
		xyzMax[2]=MAX(CoordPositionA[2],CoordPositionB[2]);
		xyzMin[0]-=radius;
		xyzMin[1]-=radius;
		xyzMin[2]-=radius;
		xyzMax[0]+=radius;
		xyzMax[1]+=radius;
		xyzMax[2]+=radius;
	}
	void cylinderElement::Set(const coordPrecision positionA[3],const coordPrecision positionB[3],const float& _radius)
	{
		COPYVEC(positionA,CoordPositionA);
		COPYVEC(positionB,CoordPositionB);
		radius=_radius;
	}

	void cylinderElement::projectionOnLine( float point[3],float VA[3],float VB[3], float* res)
	{
		float V[3];
		float subValue[3];
		float denumerateur;
		SUB(V,VB,VA);
		denumerateur=DOT(V,V);
		SUB(subValue,point,VA);
		*res=DOT(V,subValue)/denumerateur;
	}
	void cylinderElement::closestPointOnSegment(float point[3],float VA[3],float VB[3],float res[3])
	{
		float factor;
		projectionOnLine(point,VA,VB,&factor);
		if (factor <= 0.0f)
		{
			COPYVEC(VA,res);
		}else if (factor >= 1.0f)
		{
			COPYVEC(VB,res);
		}else{
			float subValue[3];
			SUB(subValue,VB,VA);
			MULTBYFLOAT(subValue,subValue,factor);
			ADD(res,subValue,VA);
		}
	}
}
