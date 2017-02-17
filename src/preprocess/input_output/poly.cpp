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

#include "poly.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "en_numeric.hpp"

namespace formatPOLY
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

	
	int equal_strings(char *s1, char *s2)
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
	char buffer[50];

	int ToInt(const char* charArray)
	{
		return atoi(charArray);
	}

	double ToFloat(const char* charArray)
	{
		return atof(charArray);
	}

	CPoly::CPoly()
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
bool CPoly::ExportPOLY(t_model& scene, const std::string& mfilename)
{
	std::ofstream os;
	os.imbue(std::locale::classic());
	os.open(mfilename);
	if(!os.is_open()) {
		return false;
	}
	os << "# Part 1 - node list\n";
	unsigned int sizeVertices = (unsigned int)scene.modelVertices.size();
	os << sizeVertices << "  3  0  0\n";
	for (int v = 0; v < sizeVertices; v++)
	{
		const dvec3& realCoords = scene.modelVertices[v];
		os << v + 1 << " " << realCoords.x << " " << realCoords.y << " " << realCoords.z << "\n";
	}
	os << "\n";

	os << "# Part 2 - facet list\n";

	size_t sizeFacets = scene.modelFaces.size();

	if (scene.saveFaceIndex)
		os << sizeFacets << " 1\n";
	else
		os << sizeFacets << " 0\n";

	for (int v = 0; v < sizeFacets; v++)
	{
		// [1 Polygon] [No hole] [Boundary Marker]
		os << "1  0  " << scene.modelFaces[v].faceIndex << "\n";
		// [vertices count] [index 1] [index 2] [index 3]
		os << "3  " << scene.modelFaces[v].indicesSommets.a + 1 << " " << scene.modelFaces[v].indicesSommets.b + 1 << " " <<
			scene.modelFaces[v].indicesSommets.c + 1) << "\n";
	}

	os << "\n";
	os << "# Part 3 - hole list\n";
	os << "0\n\n";
	os << "# Part 4 - region list\n";
	os << scene.modelRegions.size() << "\n";

	for (int r = 0; r < scene.modelRegions.size(); r++)
	{
		const formatPOLY::t_region& reg = scene.modelRegions[r];
		os << r + 1 << "  " << scene.modelRegions[r].dotInRegion.x << "  " <<
			reg.dotInRegion.y << "  " <<
			reg.dotInRegion.z << "  " <<
			reg.regionIndex << "  " <<
			reg.regionRefinement << "\n";
	}



	os << "\n";
	os << "# Part 5 - user facet list\n";

	unsigned int sizeUserFacets = (unsigned int)scene.userDefinedFaces.size();
	os << sizeUserFacets << "  1\n";


	for (int v = 0; v < sizeUserFacets; v++)
	{
		const t_face& modelFace = scene.userDefinedFaces[v];
		// [1 Polygon] [No hole] [Boundary Marker]
		os << "1  0  " << modelFace.faceIndex << "\n";
		// [vertices count] [index 1] [index 2] [index 3]
		os << "3  " << modelFace.indicesSommets.a + 1 <<
			"  " << modelFace.indicesSommets.b + 1 <<
			"  " << modelFace.indicesSommets.c + 1 << "\n";
	}
	return true;
}
struct words_destroyer
{
	words_destroyer(char **_words):words(_words){};
	~words_destroyer(){ free(words);}
	char **words;
};
bool CPoly::ImportPOLY(t_model& scene,const std::string& mfilename)
{

	std::ifstream is;
	is.imbue(std::locale::classic());
	is.open(mfilename);
	if (!is.is_open()) {
		return false;
	}
	enum PARSE_PART {NODE_HEADER, NODE_CONTENT, FACET_HEADER, FACET_CONTENT_INFO, FACET_CONTENT_VALUE, HOLE_HEADER, HOLE_CONTENT, REGION_HEADER, REGION_CONTENT};
	PARSE_PART state = NODE_HEADER;
	std::string line;

	unsigned int sizeVertices = 0;
	unsigned int parsedVertices = 0;
	unsigned int sizeFaces=0;
	unsigned int parsedFaces = 0;
	unsigned int useFacesIndex=0;
	scene.saveFaceIndex = false;
	t_face newFace;

	while(std::getline(is, line)) {
		// Skip comments
		if(line.find("#") == std::string::npos) {
			std::istringstream iss(line);
			switch(state) {
			case NODE_HEADER:
				int unused1, unused2;
				if(iss >> sizeVertices >> unused1 >> unused2) {
					scene.modelVertices.reserve(sizeVertices);
					state = NODE_CONTENT;
				}
				break;			
			case NODE_CONTENT:
				int idvert;
				dvec3 pt;
				if (iss >> idvert >> pt.x >> pt.y >> pt.z) {
					parsedVertices++;
				}
				if(parsedVertices >= sizeVertices) {
					state = FACET_HEADER;
				}
				break;
			case FACET_HEADER:
				if (iss >> sizeFaces >> useFacesIndex) {
					if(useFacesIndex==1)
						scene.saveFaceIndex=true;
					scene.modelFaces.reserve(sizeFaces);
					state = FACET_CONTENT_INFO;
				}
				break;
			case FACET_CONTENT_INFO:
				int polygon, hole;
				if (iss >> polygon  >> hole >> newFace.faceIndex) {
					state = FACET_CONTENT_VALUE;
				}
				break;
			case FACET_CONTENT_VALUE:
				int vertcount;
				if (iss >> vertcount >> newFace.indicesSommets.a >> newFace.indicesSommets.b >> newFace.indicesSommets.c) {
					parsedFaces++;
					if(parsedFaces < sizeFaces) {
						state = FACET_CONTENT_INFO;
					} else {
						state = FACET_HEADER;
					}
				}
				break;

			}
		}	
	}


		
	//	
	//	
 // EnglishTemporaryLocale dotNumericOnly;

 // char tmpBuffer[250];
 // FILE *infile;
 // infile = fopen(mfilename.c_str(), "r+");
 // if (infile == (FILE *) NULL) {
 //   //throw -1; //("File I/O Error:  Cannot open file.\n");
 //   return false;
 // }
 // int nwords=0;
 // char **words;
 // char *orig_line;
 // unsigned int sizeVertices=0;
 // unsigned int sizeFaces=0;
 // unsigned int useFacesIndex=0;

 // /* Lecture du fichier d'entete */

 // //On passe les commentaires
 // while(!feof(infile))
 // {
	//if(!fgets( tmpBuffer, 250, infile ) || (tmpBuffer[0]!=35 && tmpBuffer[0]!=10))
	//{
	//	int unused1, unused2;
	//	sscanf(tmpBuffer,"%i 3 %i %i\n",&sizeVertices,&unused1,&unused2);
	//	break;
	//}
 // }
	//  


 // //Lecture des vertices
 // unsigned int idvec=0;
 // 
 // char x[20];
 // char y[20];
 // char z[20];
 // scene.modelVertices.reserve(sizeVertices*2);
 // while(idvec<sizeVertices && !feof(infile))
 // {
	//dvec3 newVec;
	//fscanf(infile,"%i %20s %20s %20s\n",tmpBuffer,x,y,z);
	//newVec[0]=ToFloat(x);
	//newVec[1]=ToFloat(y);
	//newVec[2]=ToFloat(z);
	//scene.modelVertices.push_back(newVec);
	//idvec++;
 // }
 // //On passe les commentaires
 // while(!feof(infile))
 // {
	//if(!fgets( tmpBuffer, 250, infile ) || (tmpBuffer[0]!=35 && tmpBuffer[0]!=10))
	//{
	//	sscanf(tmpBuffer,"%i %i\n",&sizeFaces,&useFacesIndex);
	//	break;
	//}
 // }
 // scene.saveFaceIndex=false;
 // if(useFacesIndex==1)
	//  scene.saveFaceIndex=true;
 // //Lectures des triangles

 // unsigned int idtri=0;
 // 
 // int marker=0;
 // unsigned int nbsommets=0;
 // unsigned int a=0;
 // unsigned int b=0;
 // unsigned int c=0;
 // scene.modelFaces.reserve(sizeFaces*2);
 // while(idtri<sizeFaces && !feof(infile))
 // {
	//t_face newFace;
	//fscanf(infile,"%i %i %i\n",tmpBuffer,tmpBuffer,&marker);

	////fscanf(infile,"%i %i %i %i\n",tmpBuffer,&a,&b,&c);
	//
 //   words = get_words (infile, &nwords, &orig_line);
	//words_destroyer free_words(words);
	//nbsommets=ToInt(words[0]);
	//newFace.faceIndex=marker;
	//if(nbsommets==3)
	//{
	//	newFace.indicesSommets.set(ToInt(words[1])-1,ToInt(words[2])-1,ToInt(words[3])-1);
	//	scene.modelFaces.push_back(newFace);
	//}else if(nbsommets==4)
	//{
	//	newFace.indicesSommets.set(ToInt(words[1])-1,ToInt(words[2])-1,ToInt(words[3])-1);
	//	scene.modelFaces.push_back(newFace);
	//	newFace.indicesSommets.set(ToInt(words[3])-1,ToInt(words[4])-1,ToInt(words[1])-1);
	//	scene.modelFaces.push_back(newFace);
	//}
	//idtri++;
 // }
 // // Hole list
 // //On passe les commentaires
 // int nbHoles=-1;
 // while(!feof(infile))
 // {
	//if(!fgets( tmpBuffer, 250, infile ) || (tmpBuffer[0]!=35 && tmpBuffer[0]!=10))
	//{
	//	sscanf(tmpBuffer,"%i\n",&nbHoles);
	//	break;
	//}
 // }
 // //Region list
 // int nbRegion=-1;
 // while(!feof(infile))
 // {
	//if(!fgets( tmpBuffer, 250, infile ) || (tmpBuffer[0]!=35 && tmpBuffer[0]!=10))
	//{
	//	sscanf(tmpBuffer,"%i\n",&nbRegion);
	//	break;
	//}
 // }
 // int idregion=0;
 // char refinement[20];
 // int regionIndex=0;
 // while(idregion<nbRegion && !feof(infile))
 // {
	//t_region newRegion;
	//vec3 newVec;
	//fscanf(infile,"%i %20s %20s %20s %i %20s\n",tmpBuffer,x,y,z,&regionIndex,refinement);
	//newRegion.dotInRegion[0]=ToFloat(x);
	//newRegion.dotInRegion[1]=ToFloat(y);
	//newRegion.dotInRegion[2]=ToFloat(z);
	//newRegion.regionIndex=regionIndex;
	//newRegion.regionRefinement=ToFloat(refinement);
	//scene.modelRegions.push_back(newRegion);
	//idregion++;
 // }
	////////////////////////////////
 //   // User Defined Faces
	//int sizeUserFaces=0;
	////On passe les commentaires
	//while(!feof(infile))
	//{
	//if(!fgets( tmpBuffer, 250, infile ) || (tmpBuffer[0]!=35 && tmpBuffer[0]!=10))
	//{
	//	sscanf(tmpBuffer,"%i %i\n",&sizeUserFaces,tmpBuffer);
	//	break;
	//}
	//}
	//scene.userDefinedFaces.reserve(sizeUserFaces*2);
	//idtri=0;
	//while(idtri<sizeUserFaces && !feof(infile))
	//{
	//	t_face newFace;
	//	fscanf(infile,"%i %i %i\n",tmpBuffer,tmpBuffer,&marker);
	//	fscanf(infile,"%i %i %i %i\n",tmpBuffer,&a,&b,&c);
	//	newFace.indicesSommets.set(a-1,b-1,c-1);
	//	newFace.faceIndex=marker;
	//	scene.userDefinedFaces.push_back(newFace);
	//	idtri++;
	//}
 // fclose(infile);

 // return true;
}

} //fin namespace