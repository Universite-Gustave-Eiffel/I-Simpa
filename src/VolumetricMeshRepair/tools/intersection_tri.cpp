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

#include "intersection_tri.h"
#include <tools/collision.h>

namespace CollisionAlgo
{
	bool IntersectionVecTri(const vec3& origin, const vec3& dest,const vec3& a, const vec3& b, const vec3& c,float* t,float* u,float* v)
	{
		using namespace collision_manager;
		if(intersect_triangle(origin,dest-origin,a,b,c,t,u,v)==0)
		{
			return false;
		}else{
			if(*t>=0 && *t<=1)
			{
				if(origin.barelyEqual(a) || origin.barelyEqual(b)  || origin.barelyEqual(c)
					|| dest.barelyEqual(a) || dest.barelyEqual(b)  || dest.barelyEqual(c))
					return false;
				//vec3 posIntersect=a+(b-a)*(*u)+(c-a)*(*v);
				return true;
			}else{
				return false;
			}
		}
	}
	bool IntersectionTriTri(const vec3& a, const vec3& b, const vec3& c, const vec3& d, const vec3& e, const vec3& f)
	{
		float t,u,v;
		if(IntersectionVecTri(b,a,d,e,f,&t,&u,&v))
			return true;
		if(IntersectionVecTri(c,a,d,e,f,&t,&u,&v))
			return true;
		if(IntersectionVecTri(b,c,d,e,f,&t,&u,&v))
			return true;
		if(IntersectionVecTri(d,e,a,b,c,&t,&u,&v))
			return true;
		if(IntersectionVecTri(d,f,a,b,c,&t,&u,&v))
			return true;
		if(IntersectionVecTri(f,e,a,b,c,&t,&u,&v))
			return true;

		return false;
	}

}