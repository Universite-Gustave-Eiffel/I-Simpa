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

#include "ply.h"
#include "manager/stringTools.h"
#include <wx/log.h>
#include <wx/intl.h>
#include "last_cpp_include.hpp"
namespace formatPLY
{
	char **get_words(FILE *fp, int *nwords, char **orig_line)
	{
	#define BIG_STRING 4096
	  int i,j;
	  static char str[BIG_STRING];
	  static char str_copy[BIG_STRING];
	  char **words;
	  int max_words = 10;
	  int num_words = 0;
	  char *ptr,*ptr2;
	  char *result;

	  words = (char **) malloc (sizeof (char *) * max_words);

	  /* read in a line */
	  result = fgets (str, BIG_STRING, fp);
	  if (result == NULL) {
		*nwords = 0;
		*orig_line = NULL;
		return (NULL);
	  }

	  /* convert line-feed and tabs into spaces */
	  /* (this guarentees that there will be a space before the */
	  /*  null character at the end of the string) */

	  str[BIG_STRING-2] = ' ';
	  str[BIG_STRING-1] = '\0';

	  for (ptr = str, ptr2 = str_copy; *ptr != '\0'; ptr++, ptr2++) {
		*ptr2 = *ptr;
		if (*ptr == '\t') {
		  *ptr = ' ';
		  *ptr2 = ' ';
		}
		else if (*ptr == '\n') {
		  *ptr = ' ';
		  *ptr2 = '\0';
		  break;
		}
	  }

	  /* find the words in the line */

	  ptr = str;
	  while (*ptr != '\0') {

		/* jump over leading spaces */
		while (*ptr == ' ')
		  ptr++;

		/* break if we reach the end */
		if (*ptr == '\0')
		  break;

		/* allocate more room for words if necessary */
		if (num_words >= max_words) {
		  max_words += 10;
		  words = (char **) realloc (words, sizeof (char *) * max_words);
		}

		if (*ptr == '\"') {  /* a quote indidicates that we have a string */

		  /* skip over leading quote */
		  ptr++;

		  /* save pointer to beginning of word */
		  words[num_words++] = ptr;

		  /* find trailing quote or end of line */
		  while (*ptr != '\"' && *ptr != '\0')
			ptr++;

		  /* replace quote with a null character to mark the end of the word */
		  /* if we are not already at the end of the line */
		  if (*ptr != '\0')
		*ptr++ = '\0';
		}
		else {               /* non-string */

		  /* save pointer to beginning of word */
		  words[num_words++] = ptr;

		  /* jump over non-spaces */
		  while (*ptr != ' ')
		ptr++;

		  /* place a null character here to mark the end of the word */
		  *ptr++ = '\0';
		}
	  }

	  /* return the list of words */
	  *nwords = num_words;
	  *orig_line = str_copy;
	  return (words);
	}


	int equal_strings(const char *s1,const char *s2)
	{
	  int i;

	  while (*s1 && *s2)
		if (*s1++ != *s2++)
		  return (0);

	  if (*s1 != *s2)
		return (0);
	  else
		return (1);
	}
	CPly::CPly()
	{



	}
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// ExportMESH()    Write mesh to a .mesh file, which can be read and      //
//                    rendered by Medit (a free mesh viewer from INRIA).     //
//                                                                           //
// You can specify a filename (without suffix) in 'strFileName'.  If you don't //
// supply a filename (let mfilename be NULL), the default name stored in     //
// 'tetgenbehavior' will be used. The output file will have the suffix .mesh.//
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


bool CPly::ExportPly(t_model& scene, const char* mfilename)
{
  FILE *outfile;
  char mefilename[FILENAMESIZE];

  outfile = fopen(mfilename, "w");
  if (outfile == (FILE *) NULL) {
    printf("File I/O Error:  Cannot create file %s.\n", mefilename);
    return false;
  }

  unsigned int sizeVertices=scene.modelVertices.size();
  unsigned int sizeFacets=scene.modelFaces.size();
  fprintf(outfile, "ply\n");
  fprintf(outfile, "format ascii 1.0\n");
  fprintf(outfile, "element vertex %i\n",sizeVertices);
  fprintf(outfile, "property float x\n");
  fprintf(outfile, "property float y\n");
  fprintf(outfile, "property float z\n");
  fprintf(outfile, "element face %i\n",sizeFacets);
  fprintf(outfile, "property list uchar int vertex_indices\n");
  fprintf(outfile, "end_header\n");

  for(int v=0; v < sizeVertices ;v++)
  {
	vec3 realCoords=scene.modelVertices[v];
	fprintf(outfile, "%s  %s  %s \n",
	Convertor::ToString(realCoords.x).c_str(), //On remet les points à leur etat d'origine est l'on exporte
	Convertor::ToString(realCoords.y).c_str(),
	Convertor::ToString(realCoords.z).c_str());
  }
  for(int v=0; v < sizeFacets ;v++)
  {
		// [sommets] [indice 1] [indice 2] [indice 3]
		fprintf(outfile, "3  %li  %li  %li\n",scene.modelFaces[v].indicesSommets.a,
											scene.modelFaces[v].indicesSommets.b,
											scene.modelFaces[v].indicesSommets.c);
  }
  fprintf(outfile, "\n");
  fclose(outfile);
  return true;
}




bool CPly::ImportPly(t_model& scene, const char* mfilename)
{
  FILE *infile;
  char mefilename[FILENAMESIZE];
  char buffer[250];
  infile = fopen(mfilename, "r+");
  if (infile == (FILE *) NULL) {
    wxLogMessage(_("File I/O Error:  Cannot open file %s\n"), mefilename);
    return false;
  }

  unsigned int sizeVertices=0;
  unsigned int sizeFaces=0;
  int nwords=0;
  char **words;
  char *orig_line;

  /* Lecture du fichier d'entete */

  words = get_words (infile, &nwords, &orig_line);
  while (words)
  {
    //Pour chaque ligne
	if (nwords==3 && equal_strings (words[0], "element"))
	{
		//Le premier mot de la ligne est element
		if (equal_strings (words[1], "vertex")) //Nous indique le nombre de
		{
			sizeVertices= (unsigned int) Convertor::ToInt(words[2]);
		}else if(equal_strings (words[1], "face"))
		{
			sizeFaces= (unsigned int) Convertor::ToInt(words[2]);
		}
	}
	if (equal_strings (words[0], "end_header")) //Fin du bloc d'entete
	{
		break;
	}
	words = get_words (infile, &nwords, &orig_line);
  }

  //Lecture des vertices
  unsigned int idvec=0;

  char x[20];
  char y[20];
  char z[20];

  scene.modelVertices.reserve(sizeVertices);
  while(idvec<sizeVertices && !feof(infile))
  {
	vec3 newVec;
	fscanf(infile,"%20s %20s %20s\n",x,y,z);
	newVec[0]=Convertor::ToFloat(wxString(x));
	newVec[1]=Convertor::ToFloat(wxString(y));
	newVec[2]=Convertor::ToFloat(wxString(z));
	scene.modelVertices.push_back(newVec);
	idvec++;
  }
  //Lectures des triangles

  unsigned int idtri=0;

  short dim;
  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
  scene.modelFaces.reserve(sizeFaces);
  while(idtri<sizeFaces && !feof(infile))
  {

    words = get_words (infile, &nwords, &orig_line);
	if(nwords>0)
	{
		dim=Convertor::ToInt(words[0]);
		if(dim==3)
		{
			t_face newFace;
			newFace.indicesSommets.set(Convertor::ToInt(words[1]),Convertor::ToInt(words[2]),Convertor::ToInt(words[3]));
			scene.modelFaces.push_back(newFace);
		}else if(dim==4)
		{
			t_face newFace;
			newFace.indicesSommets.set(Convertor::ToInt(words[1]),Convertor::ToInt(words[2]),Convertor::ToInt(words[3]));
			scene.modelFaces.push_back(newFace);
			newFace.indicesSommets.set(Convertor::ToInt(words[3]),Convertor::ToInt(words[4]),Convertor::ToInt(words[1]));
			scene.modelFaces.push_back(newFace);
		}
		/*
		fscanf(infile,"%5s %10s %10s %10s\n",dim,a,b,c);
		*/
		idtri++;
	}
  }

  fclose(infile);

  return true;
}

} //fin namespace
