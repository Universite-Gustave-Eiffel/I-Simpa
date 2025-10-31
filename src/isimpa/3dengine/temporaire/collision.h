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

#ifndef __HCOLLIDE__
#define __HCOLLIDE__
#define TEST_CULL

namespace collision_manager
{

	//#define EPSILON 0.000001
	inline void CROSS(float dest[3],float v1[3],float v2[3]){
			  dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; 
			  dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; 
			  dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; }
	#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
	inline void SUB(float dest[3],float v1[3],float v2[3]){
			  dest[0]=v1[0]-v2[0]; 
			  dest[1]=v1[1]-v2[1]; 
			  dest[2]=v1[2]-v2[2]; 
	}





	inline int	intersect_triangle(float orig[3], float dir[3],
					   float vert0[3], float vert1[3], float vert2[3],
					   float *t, float *u, float *v)
	{
	   float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	   float det,inv_det;

	   /* find vectors for two edges sharing vert0 */
	   SUB(edge1, vert1, vert0);
	   SUB(edge2, vert2, vert0);

	   /* begin calculating determinant - also used to calculate U parameter */
	   CROSS(pvec, dir, edge2);

	   /* if determinant is near zero, ray lies in plane of triangle */
	   det = DOT(edge1, pvec);

	#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
	   if (det < EPSILON)
		  return 0;

	   /* calculate distance from vert0 to ray origin */
	   SUB(tvec, orig, vert0);

	   /* calculate U parameter and test bounds */
	   *u = DOT(tvec, pvec);
	   if (*u < 0.0 || *u > det)
		  return 0;

	   /* prepare to test V parameter */
	   CROSS(qvec, tvec, edge1);

		/* calculate V parameter and test bounds */
	   *v = DOT(dir, qvec);
	   if (*v < 0.0 || *u + *v > det)
		  return 0;

	   /* calculate t, scale parameters, ray intersects triangle */
	   *t = DOT(edge2, qvec);
	   inv_det = 1.0 / det;
	   *t *= inv_det;
	   *u *= inv_det;
	   *v *= inv_det;
	#else                    /* the non-culling branch */
	   if (det > -EPSILON && det < EPSILON)
		 return 0;
	   inv_det = 1.0 / det;

	   /* calculate distance from vert0 to ray origin */
	   SUB(tvec, orig, vert0);

	   /* calculate U parameter and test bounds */
	   *u = DOT(tvec, pvec) * inv_det;
	   if (*u < 0.0 || *u > 1.0)
		 return 0;

	   /* prepare to test V parameter */
	   CROSS(qvec, tvec, edge1);

	   /* calculate V parameter and test bounds */
	   *v = DOT(dir, qvec) * inv_det;
	   if (*v < 0.0 || *u + *v > 1.0)
		 return 0;

	   /* calculate t, ray intersects triangle */
	   *t = DOT(edge2, qvec) * inv_det;
	#endif
	   return 1;
	}
} //fin namespca
#endif