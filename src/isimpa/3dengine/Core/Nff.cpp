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

#include "Nff.h"
#include <stdio.h>
#include "manager/stringTools.h"
#include "last_cpp_include.hpp"
namespace formatNFF
{

	CNff::CNff()
	{



	}
	///////////////////////////////////////////////////////////////////////////////
	//                                                                           //
	// ExportNFF
	//                                                                           //
	///////////////////////////////////////////////////////////////////////////////
	void CNff::ExportNFF(vec4 UnitizeValue,std::vector<SGroup3D> &pModel, std::vector<vec3> &Vertices, const char *mfilename,long nbVertices, long nbFaces)
	{
	  FILE *outfile;

	  outfile = fopen(mfilename, "w");
	  if (outfile == (FILE *) NULL) {
		return;
	  }

	  fprintf(outfile, "v\n");
	  fprintf(outfile, "from X Y Z\n");
	  fprintf(outfile, "at X Y Z\n");
	  fprintf(outfile, "up 0 1 0\n");
	  fprintf(outfile, "angle 50\n");
	  fprintf(outfile, "hither 1\n");
	  fprintf(outfile, "resolution 512 512\n");
	  fprintf(outfile, "b 0.1 0.5 0.7\n");
	  fprintf(outfile, "l X Y Z\n");
	  fprintf(outfile, "f 0.000 0.000 1.000 1 0 0 0 1\n");

	  for(int g=0; g < pModel.size() ;g++)
	  {
			for(long j=0; j < pModel[g].pFaces.size() ;j++)
			{
				vec3 realCoordsa=coordsOperation::GlCoordsToCommonCoords(UnitizeValue,Vertices[pModel[g].pFaces[j].Vertices.a]);
				fprintf(outfile, "p 3\n");
				fprintf(outfile, "%s  %s  %s\n",
				Convertor::ToString(realCoordsa.x).c_str(), Convertor::ToString(realCoordsa.y).c_str(), Convertor::ToString(realCoordsa.z).c_str());
				vec3 realCoordsb=coordsOperation::GlCoordsToCommonCoords(UnitizeValue,Vertices[pModel[g].pFaces[j].Vertices.b]);
				fprintf(outfile, "%s  %s  %s\n", Convertor::ToString(realCoordsb.x).c_str(), Convertor::ToString(realCoordsb.y).c_str(), Convertor::ToString(realCoordsb.z).c_str());
				vec3 realCoordsc=coordsOperation::GlCoordsToCommonCoords(UnitizeValue,Vertices[pModel[g].pFaces[j].Vertices.c]);
				fprintf(outfile, "%s  %s  %s\n", Convertor::ToString(realCoordsc.x).c_str(), Convertor::ToString(realCoordsc.y).c_str(), Convertor::ToString(realCoordsc.z).c_str());
			}
	  }
	  fclose(outfile);
	}



	void CNff::ExportTetraNFF(vec4 UnitizeValue,triangleFace *tabVertexMaillage, const char *mfilename, long nbFaces)
	{
		if(nbFaces<4)
			return;
		FILE *outfile;

		outfile = fopen(mfilename, "w");
		if (outfile == (FILE *) NULL) {
		return;
		}


		fprintf(outfile, "v\n");
		fprintf(outfile, "at X Y Z\n");
		fprintf(outfile, "at X Y Z\n");
		fprintf(outfile, "up 0 1 0\n");
		fprintf(outfile, "angle 50\n");
		fprintf(outfile, "hither 1\n");
		fprintf(outfile, "resolution 512 512\n");
		fprintf(outfile, "b 0.1 0.5 0.7\n");
		fprintf(outfile, "l X Y Z\n");
		fprintf(outfile, "f 0.000 0.000 1.000 1 0 0 0 1\n");

		for(long f=0; f < nbFaces ;f++)
		{
			fprintf(outfile, "p 3\n");
			fprintf(outfile, "%s  %s  %s\n",
				Convertor::ToString((tabVertexMaillage[f].a[0]/UnitizeValue.w)+UnitizeValue.x).c_str(),
				Convertor::ToString((tabVertexMaillage[f].a[1]/UnitizeValue.w)+UnitizeValue.y).c_str(),
				Convertor::ToString((tabVertexMaillage[f].a[2]/UnitizeValue.w)+UnitizeValue.z).c_str());
			fprintf(outfile, "%s  %s  %s\n",
				Convertor::ToString((tabVertexMaillage[f].b[0]/UnitizeValue.w)+UnitizeValue.x).c_str(),
				Convertor::ToString((tabVertexMaillage[f].b[1]/UnitizeValue.w)+UnitizeValue.y).c_str(),
				Convertor::ToString((tabVertexMaillage[f].b[2]/UnitizeValue.w)+UnitizeValue.z).c_str());
			fprintf(outfile, "%s  %s  %s\n",
			Convertor::ToString((tabVertexMaillage[f].c[0]/UnitizeValue.w)+UnitizeValue.x).c_str(),
			Convertor::ToString((tabVertexMaillage[f].c[1]/UnitizeValue.w)+UnitizeValue.y).c_str(),
			Convertor::ToString((tabVertexMaillage[f].c[2]/UnitizeValue.w)+UnitizeValue.z).c_str());
		}

		fclose(outfile);
	}

} //fin namespace
