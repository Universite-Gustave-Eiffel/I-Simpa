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

#include "mesh.h"
#include "en_numeric.hpp"
#include "manager/stringTools.h"
#include <locale.h>
#include "last_cpp_include.hpp"

namespace formatMESH
{

	CMesh::CMesh()
	{



	}
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// ExportMESH()    Format mesh créé par INRIA
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
void CMesh::ExportMESH(vec4 UnitizeValue,std::vector<SGroup3D> &pModel, std::vector<vec3> &Vertices, const std::string& mfilename, long nbFaces)
{
  FILE *outfile;
  EnglishTemporaryLocale();
  outfile = fopen(mfilename.c_str(), "w");
  if (outfile == (FILE *) NULL) {
    printf("File I/O Error:  Cannot create file %s.\n", mfilename.c_str());
    return;
  }

  fprintf(outfile, "MeshVersionFormatted 1\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "Dimension\n");
  fprintf(outfile, "3\n");
  fprintf(outfile, "\n");

  fprintf(outfile, "\n# Set of mesh vertices\n");
  fprintf(outfile, "Vertices\n");
  fprintf(outfile, "%ld\n", Vertices.size());
  // Medit need start number form 1.
  for(int v=0; v < Vertices.size() ;v++)
  {
	// Face j Point coordinates. Vertice 1 of 3
	vec3 realCoords=coordsOperation::GlCoordsToCommonCoords(UnitizeValue,Vertices[v]);
	fprintf(outfile, "%f    %f    %f ",
	realCoords.x, //On remet les points à leur etat d'origine est l'on exporte
	realCoords.y,
	realCoords.z);
	fprintf(outfile, "    0\n");
  }
  fprintf(outfile, "\n# Set of Triangles\n");
  fprintf(outfile, "Triangles\n");
  fprintf(outfile, "%ld\n", nbFaces);

  for(int g=0; g < pModel.size() ;g++)
  {
		for(long j=0; j < pModel[g].pFaces.size() ;j++)
		{
			fprintf(outfile, "%5d  %5d  %5d",
					pModel[g].pFaces[j].Vertices.a+1,
					pModel[g].pFaces[j].Vertices.b+1,
					pModel[g].pFaces[j].Vertices.c+1);
			fprintf(outfile, "    0\n");
		}
  }

  fprintf(outfile, "\nEnd\n");
  fclose(outfile);
}

} //fin namespace
