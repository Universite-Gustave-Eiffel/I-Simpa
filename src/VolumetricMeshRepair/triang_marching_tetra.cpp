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

#include "triang_marching_tetra.hpp"

namespace PolygoniseAlgo
{
	//http://local.wasp.uwa.edu.au/~pbourke/geometry/polygonise/
	typedef struct {
	   decimal x,y,z;
	} XYZ;
	typedef struct {
	   XYZ p[3];
	} TRIANGLE;
	typedef struct {
	   XYZ p[8];
	   decimal val[8];
	} GRIDCELL;
	#ifndef ABS
	#define ABS abs
	#endif
	/*
	   Linearly interpolate the position where an isosurface cuts
	   an edge between two vertices, each with their own scalar value
	*/
	XYZ VertexInterp(const decimal& isolevel,XYZ& p1,XYZ& p2,const decimal& valp1,const decimal& valp2)
	{
	   decimal mu;
	   XYZ p;

	   if (ABS(isolevel-valp1) < 0.00001)
		  return(p1);
	   if (ABS(isolevel-valp2) < 0.00001)
		  return(p2);
	   if (ABS(valp1-valp2) < 0.00001)
		  return(p1);
	   mu = (isolevel - valp1) / (valp2 - valp1);
	   p.x = p1.x + mu * (p2.x - p1.x);
	   p.y = p1.y + mu * (p2.y - p1.y);
	   p.z = p1.z + mu * (p2.z - p1.z);

	   return(p);
	}

	/*
	   Polygonise a tetrahedron given its vertices within a cube
	   This is an alternative algorithm to polygonisegrid.
	   It results in a smoother surface but more triangular facets.

						  + 0
						 /|\
						/ | \
					   /  |  \
					  /   |   \
					 /    |    \
					/     |     \
				   +-------------+ 1
				  3 \     |     /
					 \    |    /
					  \   |   /
					   \  |  /
						\ | /
						 \|/
						  + 2

	   It's main purpose is still to polygonise a gridded dataset and
	   would normally be called 6 times, one for each tetrahedron making
	   up the grid cell.
	   Given the grid labelling as in PolygniseGrid one would call
		  PolygoniseTri(grid,iso,triangles,0,2,3,7);
		  PolygoniseTri(grid,iso,triangles,0,2,6,7);
		  PolygoniseTri(grid,iso,triangles,0,4,6,7);
		  PolygoniseTri(grid,iso,triangles,0,6,1,2);
		  PolygoniseTri(grid,iso,triangles,0,6,1,4);
		  PolygoniseTri(grid,iso,triangles,5,6,1,4);
	*/
	int PolygoniseTri(GRIDCELL g,decimal iso,
	   TRIANGLE *tri,int v0,int v1,int v2,int v3)
	{
	   int ntri = 0;
	   int triindex;

	   /*
		  Determine which of the 16 cases we have given which vertices
		  are above or below the isosurface
	   */
	   triindex = 0;
	   if (g.val[v0] < iso) triindex |= 1;
	   if (g.val[v1] < iso) triindex |= 2;
	   if (g.val[v2] < iso) triindex |= 4;
	   if (g.val[v3] < iso) triindex |= 8;

	   /* Form the vertices of the triangles for each case */
	   switch (triindex) {
	   case 0x00:
	   case 0x0F:
		  break;
	   case 0x0E:
	   case 0x01:
		  tri[0].p[0] = VertexInterp(iso,g.p[v0],g.p[v1],g.val[v0],g.val[v1]);
		  tri[0].p[1] = VertexInterp(iso,g.p[v0],g.p[v2],g.val[v0],g.val[v2]);
		  tri[0].p[2] = VertexInterp(iso,g.p[v0],g.p[v3],g.val[v0],g.val[v3]);
		  ntri++;
		  break;
	   case 0x0D:
	   case 0x02:
		  tri[0].p[0] = VertexInterp(iso,g.p[v1],g.p[v0],g.val[v1],g.val[v0]);
		  tri[0].p[1] = VertexInterp(iso,g.p[v1],g.p[v3],g.val[v1],g.val[v3]);
		  tri[0].p[2] = VertexInterp(iso,g.p[v1],g.p[v2],g.val[v1],g.val[v2]);
		  ntri++;
		  break;
	   case 0x0C:
	   case 0x03:
		  tri[0].p[0] = VertexInterp(iso,g.p[v0],g.p[v3],g.val[v0],g.val[v3]);
		  tri[0].p[1] = VertexInterp(iso,g.p[v0],g.p[v2],g.val[v0],g.val[v2]);
		  tri[0].p[2] = VertexInterp(iso,g.p[v1],g.p[v3],g.val[v1],g.val[v3]);
		  ntri++;
		  tri[1].p[0] = tri[0].p[2];
		  tri[1].p[1] = VertexInterp(iso,g.p[v1],g.p[v2],g.val[v1],g.val[v2]);
		  tri[1].p[2] = tri[0].p[1];
		  ntri++;
		  break;
	   case 0x0B:
	   case 0x04:
		  tri[0].p[0] = VertexInterp(iso,g.p[v2],g.p[v0],g.val[v2],g.val[v0]);
		  tri[0].p[1] = VertexInterp(iso,g.p[v2],g.p[v1],g.val[v2],g.val[v1]);
		  tri[0].p[2] = VertexInterp(iso,g.p[v2],g.p[v3],g.val[v2],g.val[v3]);
		  ntri++;
		  break;
	   case 0x0A:
	   case 0x05:
		  tri[0].p[0] = VertexInterp(iso,g.p[v0],g.p[v1],g.val[v0],g.val[v1]);
		  tri[0].p[1] = VertexInterp(iso,g.p[v2],g.p[v3],g.val[v2],g.val[v3]);
		  tri[0].p[2] = VertexInterp(iso,g.p[v0],g.p[v3],g.val[v0],g.val[v3]);
		  ntri++;
		  tri[1].p[0] = tri[0].p[0];
		  tri[1].p[1] = VertexInterp(iso,g.p[v1],g.p[v2],g.val[v1],g.val[v2]);
		  tri[1].p[2] = tri[0].p[1];
		  ntri++;
		  break;
	   case 0x09:
	   case 0x06:
		  tri[0].p[0] = VertexInterp(iso,g.p[v0],g.p[v1],g.val[v0],g.val[v1]);
		  tri[0].p[1] = VertexInterp(iso,g.p[v1],g.p[v3],g.val[v1],g.val[v3]);
		  tri[0].p[2] = VertexInterp(iso,g.p[v2],g.p[v3],g.val[v2],g.val[v3]);
		  ntri++;
		  tri[1].p[0] = tri[0].p[0];
		  tri[1].p[1] = VertexInterp(iso,g.p[v0],g.p[v2],g.val[v0],g.val[v2]);
		  tri[1].p[2] = tri[0].p[2];
		  ntri++;
		  break;
	   case 0x07:
	   case 0x08:
		  tri[0].p[0] = VertexInterp(iso,g.p[v3],g.p[v0],g.val[v3],g.val[v0]);
		  tri[0].p[1] = VertexInterp(iso,g.p[v3],g.p[v2],g.val[v3],g.val[v2]);
		  tri[0].p[2] = VertexInterp(iso,g.p[v3],g.p[v1],g.val[v3],g.val[v1]);
		  ntri++;
		  break;
	   }

	   return(ntri);
	}
}

namespace Triangulators
{
	void MarchingTetraTriangulator::Triangulate(ScalarFieldBuilders::ScalarFieldCreator& inData)
	{


	}
}
