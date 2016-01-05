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

#include "var.h"
#include "manager/stringTools.h"
#include "en_numeric.hpp"
#include "last_cpp_include.hpp"

namespace formatVAR
{

	CVar::CVar()
	{



	}

bool CVar::BuildVar(t_file& sceneconst,const char* mfilename)
{
  EnglishTemporaryLocale dotNumericOnly;
  FILE *outfile;
  char mefilename[FILENAMESIZE];

  outfile = fopen(mfilename, "w");
  if (outfile == (FILE *) NULL) {
    printf("File I/O Error:  Cannot create file %s.\n", mefilename);
    return false;
  }

  unsigned int quantConstraints=sceneconst.facetsConstraints.size();
  fprintf(outfile, "# Facet constraints\n");
  fprintf(outfile, "%i\n",quantConstraints);
  for(int v=0; v < quantConstraints ;v++)
  {
	  /*
	fprintf(outfile, "%i  %i  %s\n",
	v+1,
	sceneconst.facetsConstraints[v].boundaryMarker,
	Convertor::ToString(sceneconst.facetsConstraints[v].areaConstraint,'.'));
	*/
	  fprintf(outfile, wxString::Format("%i  %i %g\n",v+1,
	sceneconst.facetsConstraints[v].boundaryMarker,
	sceneconst.facetsConstraints[v].areaConstraint
	));
  }
  fprintf(outfile, "# Segment constraints\n");
  fprintf(outfile, "0");
  fclose(outfile);

  return true;
}

} //fin namespace