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
#include <stdio.h>
#include <string.h>
//#define TEST_CULL

/* Ray-Triangle Intersection Test Routines          */
/* Different optimizations of my and Ben Trumbore's */
/* code from journals of graphics tools (JGT)       */
/* http://www.acm.org/jgt/                          */
/* by Tomas Moller, May 2000                        */
//http://www.cs.lth.se/home/Tomas_Akenine_Moller/raytri/
/* where t is the distance to the plane in which	*/
/* the triangle lies and (u, v) represents			*/
/* the coordinates inside the triangle				*/


/**
 * @file collision.h
 * @brief Test de collision vecteur triangle
 *
 * Basé sur une méthode optimisé.
 * @see http://www.cs.lth.se/home/Tomas_Akenine_Moller/raytri/
 */


namespace collision_manager
{

	//#define EPSILON 0.000001
	inline void CROSS(float dest[3],const float v1[3],const float v2[3]){
			  dest[0]=v1[1]*v2[2]-v1[2]*v2[1];
			  dest[1]=v1[2]*v2[0]-v1[0]*v2[2];
			  dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; }
	#ifndef DOT
	#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
	#endif
	inline void SUB(float dest[3],const float v1[3],const float v2[3]){
			  dest[0]=v1[0]-v2[0];
			  dest[1]=v1[1]-v2[1];
			  dest[2]=v1[2]-v2[2];
	}

	inline void ADD(float dest[3],const float v1[3],const float v2[3]){
			  dest[0]=v1[0]+v2[0];
			  dest[1]=v1[1]+v2[1];
			  dest[2]=v1[2]+v2[2];
	}

	inline void INPLACE_DIV(float v1[3],const float value){
			  v1[0]/=value;
			  v1[1]/=value;
			  v1[2]/=value;
	}


	/**
	 * @brief Méthode de test de collision rayon, triangle
	 *
	 * @param orig Origine du rayon
	 * @param dir Direction du rayon
	 * @param vert0 Sommet A du triangle
	 * @param vert1 Sommet B du triangle
	 * @param vert2 Sommet C du triangle
	 * @param t Distance entre l'origine du rayon et le point de collision. Cette distance correspond à t fois le vecteur de direction.
	 * @param u Position du point de collision sur le plan du triangle ( par rapport à un coté )
	 * @param v Position du point de collision sur le plan du triangle ( par rapport à un coté )
	 *
	 * @see http://www.cs.lth.se/home/Tomas_Akenine_Moller/raytri/
	 */
	inline int	intersect_triangle(const float orig[3], const float dir[3],
					   const float vert0[3], const float vert1[3],const  float vert2[3],
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
	   inv_det = 1.0f / det;

	   /* calculate distance from vert0 to ray origin */
	   SUB(tvec, orig, vert0);

	   /* calculate U parameter and test bounds */
	   *u = DOT(tvec, pvec) * inv_det;
	  if (*u < 0.0 || *u > 1.0 ) // originel  if (*u < 0.0 || *u > 1.0 ) Modif  if (*u < 0.0 || *u > 1.0 + BARELY_EPSILON)
		 return 0;

	   /* prepare to test V parameter */
	   CROSS(qvec, tvec, edge1);


	   /* calculate V parameter and test bounds */
	   *v = DOT(dir, qvec) * inv_det;
	   if (*v < 0.0 || *u + *v > 1.0 )  // Originel  if (*v < 0.0 || *u + *v > 1.0 ) Modif if (*v < 0.0 || *u + *v > 1.0 + BARELY_EPSILON)
		 return 0;

	   /* calculate t, ray intersects triangle */
	   *t = DOT(edge2, qvec) * inv_det;
	#endif
	   return 1;
	}

	inline int	intersect_parallelogram(const float orig[3],const  float dir[3],
					   const float vert0[3],const float vert1[3],const float vert2[3],
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
	   inv_det = 1.0f / det;

	   /* calculate distance from vert0 to ray origin */
	   SUB(tvec, orig, vert0);

	   /* calculate U parameter and test bounds */
	   *u = DOT(tvec, pvec) * inv_det;
	  if (*u < 0.0f || *u >= 1.0f )
		 return 0;

	   /* prepare to test V parameter */
	   CROSS(qvec, tvec, edge1);


	   /* calculate V parameter and test bounds */
	   *v = DOT(dir, qvec) * inv_det;
	   if (*v < 0.0f || *v >= 1.0f )
		 return 0;

	   /* calculate t, ray intersects triangle */
	   *t = DOT(edge2, qvec) * inv_det;
	#endif
	   return 1;
	}

#ifndef MIN
	#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
	#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif
#ifndef IFVECMINSET
#define IFVECMINSET(vecmin,othervec) vecmin[0]=MIN(vecmin[0],othervec[0]);\
	vecmin[1]=MIN(vecmin[1],othervec[1]);\
	vecmin[2]=MIN(vecmin[2],othervec[2]);
#endif
#ifndef IFVECMAXSET
#define IFVECMAXSET(vecmax,othervec) vecmax[0]=MAX(vecmax[0],othervec[0]);\
	vecmax[1]=MAX(vecmax[1],othervec[1]);\
	vecmax[2]=MAX(vecmax[2],othervec[2]);
#endif
#ifndef VECBETWEEN
#define VECBETWEEN(VECLEFT,VECMIDLE,VECRIGHT) (( VECLEFT[0]<=VECMIDLE[0] && VECMIDLE[0]<=VECRIGHT[0] ) && \
	( VECLEFT[1]<=VECMIDLE[1] && VECMIDLE[1]<=VECRIGHT[1] ) && \
	( VECLEFT[2]<=VECMIDLE[2] && VECMIDLE[2]<=VECRIGHT[2] ))
#endif
#ifndef VECINF
#define VECINF(V1,V2) (( V1[0]<V2[0] ) || \
	( V1[1]<V2[1] ) || \
	( V1[2]<V2[2] ))
#endif
#ifndef VECTET_TEST
#define VECTET_TEST(origin,dest)	if(intersect_triangle(origin,dest,tetA,tetB,tetC,&t,&u,&v) && t>=0 && t<=1.)\
					return 1;\
					if(intersect_triangle(origin,dest,tetA,tetC,tetD,&t,&u,&v) && t>=0 && t<=1.)\
					return 1;\
					if(intersect_triangle(origin,dest,tetA,tetB,tetD,&t,&u,&v) && t>=0 && t<=1.)\
					return 1;\
					if(intersect_triangle(origin,dest,tetB,tetC,tetD,&t,&u,&v) && t>=0 && t<=1.)\
					return 1;
#endif
#define DIST_TEST  && t>=0 && t<=1.
	const float EPSILON_VEC[3]={BARELY_EPSILON,BARELY_EPSILON,BARELY_EPSILON};
		inline int	tetrahedron_intersect_parallelogram(const float tetA[3], const float tetB[3], const float tetC[3], const float tetD[3],
					   const float vert0[3], const float vert1[3], const float vert2[3] )
		{
			size_t sizeofvec(sizeof(float)*3);
			/*
			 Test intersection boite englobante
			 */
			float minBoxTetra[3];
			float maxBoxTetra[3];
			memcpy(minBoxTetra,tetA,sizeofvec);
			memcpy(maxBoxTetra,tetA,sizeofvec);
			IFVECMINSET(minBoxTetra,tetB);
			IFVECMINSET(minBoxTetra,tetC);
			IFVECMINSET(minBoxTetra,tetD);
			IFVECMAXSET(maxBoxTetra,tetB);
			IFVECMAXSET(maxBoxTetra,tetC);
			IFVECMAXSET(maxBoxTetra,tetD);

			float minBoxParr[3];
			float maxBoxParr[3];
			memcpy(minBoxParr,vert0,sizeofvec);
			memcpy(maxBoxParr,vert0,sizeofvec);
			IFVECMINSET(minBoxParr,vert1);
			IFVECMINSET(minBoxParr,vert2);
			IFVECMAXSET(maxBoxParr,vert1);
			IFVECMAXSET(maxBoxParr,vert2);

			if( VECINF(maxBoxParr,minBoxTetra))
				return 0;

			if( !VECINF(minBoxParr,maxBoxTetra))
				return 0;

			if( VECINF(maxBoxTetra,minBoxParr))
				return 0;

			if( !VECINF(minBoxTetra,maxBoxParr))
				return 0;

			/*
			 Test intersection
			 AB -> Parallélogramme
			 AC -> Parallélogramme
			 AD -> Parallélogramme
			 BD -> Parallélogramme
			 BC -> Parallélogramme
			 CD -> Parallélogramme
			*/
			float t,u,v;
			float tmpvec[3];

			//AB
			SUB(tmpvec,tetB,tetA);
			if(intersect_parallelogram(tetA,tmpvec,vert0,vert1,vert2,&t,&u,&v) && t>=0 && t<=1.)
				return 1;
			//AC
			SUB(tmpvec,tetC,tetA);
			if(intersect_parallelogram(tetA,tmpvec,vert0,vert1,vert2,&t,&u,&v) && t>=0 && t<=1.)
				return 1;
			//AD
			SUB(tmpvec,tetD,tetA);
			if(intersect_parallelogram(tetA,tmpvec,vert0,vert1,vert2,&t,&u,&v) && t>=0 && t<=1.)
				return 1;
			//BD
			SUB(tmpvec,tetD,tetB);
			if(intersect_parallelogram(tetB,tmpvec,vert0,vert1,vert2,&t,&u,&v) && t>=0 && t<=1.)
				return 1;
			//BC
			SUB(tmpvec,tetC,tetB);
			if(intersect_parallelogram(tetB,tmpvec,vert0,vert1,vert2,&t,&u,&v) && t>=0 && t<=1.)
				return 1;
			//CD
			SUB(tmpvec,tetD,tetC);
			if(intersect_parallelogram(tetC,tmpvec,vert0,vert1,vert2,&t,&u,&v) && t>=0 && t<=1.)
				return 1;



			//vert0-vert2
			SUB(tmpvec,vert2,vert0);
			VECTET_TEST(vert0,tmpvec);

			//vert1-vert3
			float vert3[3];
			SUB(vert3,vert0,vert2);
			ADD(vert3,vert3,vert1);
			SUB(tmpvec,vert1,vert3);
			VECTET_TEST(vert3,tmpvec);

			return 0;
		}
} //fin namespca
#endif
