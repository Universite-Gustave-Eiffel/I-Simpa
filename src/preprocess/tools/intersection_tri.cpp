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

#include "intersection_tri.h"
#include <tools/collision.h>

namespace CollisionAlgo
{
	bool IntersectionVecTri(const vec3& origin, const vec3& dest,const vec3& a, const vec3& b, const vec3& c,float* t,float* u,float* v, vec3* posIntersect)
	{
		using namespace collision_manager;
		if(intersect_triangle(origin,dest-origin,a,b,c,t,u,v)==0)
		{
			return false;
		}else{
			if(*t>=0 && *t<=1)
			{
				vec3 i=origin+((origin-dest)*(*t));
				if(posIntersect!=NULL)
				{
					*posIntersect=i;
				}
				if(i.barelyEqual(a) || i.barelyEqual(b)  || i.barelyEqual(c))
					return false;
				return true;
			}else{
				return false;
			}
		}
	}
	//TODO Output two intersection
	bool IntersectionTriTri(const vec3& a, const vec3& b, const vec3& c, const vec3& d, const vec3& e, const vec3& f, vec3* isect, vec3* isecB)
	{
		float t,u,v;
		bool hasIntersection=false;
		vec3 tempInt;
		if(IntersectionVecTri(b,a,d,e,f,&t,&u,&v,&tempInt))
		{
			if(!hasIntersection)
			{
				hasIntersection=true;
				*isect=tempInt;
			}else{
				*isecB=tempInt;
				return true;
			}
		}
		if(IntersectionVecTri(c,a,d,e,f,&t,&u,&v,&tempInt))
		{
			if(!hasIntersection)
			{
				hasIntersection=true;
				*isect=tempInt;
			}else{
				*isecB=tempInt;
				return true;
			}
			
		}
		if(IntersectionVecTri(b,c,d,e,f,&t,&u,&v,&tempInt))
		{
			if(!hasIntersection)
			{
				hasIntersection=true;
				*isect=tempInt;
			}else{
				*isecB=tempInt;
				return true;
			}

		}
		if(IntersectionVecTri(d,e,a,b,c,&t,&u,&v,&tempInt))
		{
			if(!hasIntersection)
			{
				hasIntersection=true;
				*isect=tempInt;
			}else{
				*isecB=tempInt;
				return true;
			}
		}
		if(IntersectionVecTri(d,f,a,b,c,&t,&u,&v,&tempInt))
		{
			if(!hasIntersection)
			{
				hasIntersection=true;
				*isect=tempInt;
			}else{
				*isecB=tempInt;
				return true;
			}

		}
		if(IntersectionVecTri(f,e,a,b,c,&t,&u,&v,&tempInt))
		{
			if(!hasIntersection)
			{
				hasIntersection=true;
				*isect=tempInt;
			}else{
				*isecB=tempInt;
				return true;
			}
		}
		*isecB=*isect;
		return hasIntersection;
	}

}