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
#include <limits>

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
	os.precision(std::numeric_limits< double >::max_digits10);
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
			scene.modelFaces[v].indicesSommets.c + 1 << "\n";
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
	enum PARSE_PART {NODE_HEADER, NODE_CONTENT, FACET_HEADER, FACET_CONTENT_INFO, FACET_CONTENT_VALUE, HOLE_HEADER, HOLE_CONTENT, REGION_HEADER,
		REGION_CONTENT, USER_FACE_HEADER, USER_FACE_CONTENT_INFO, USER_FACE_CONTENT_VALUE };
	PARSE_PART state = NODE_HEADER;
	std::string line;

	unsigned int sizeVertices = 0;
	unsigned int parsedVertices = 0;
	unsigned int sizeFaces=0;
	unsigned int parsedFaces = 0;
	unsigned int useFacesIndex=0;
	int nbUseFace = 0;
	int parsedUserFace = 0;
	scene.saveFaceIndex = false;
	t_face newFace;
	t_region newRegion;
	int idvert;
	int unused1, unused2;
	dvec3 pt;
	int polygon, hole;
	int holeNb;
	int parsedHoles = 0;
	int parsedRegion = 0;
	int vertcount, nbHoles;
	int nbRegion = -1;
	ivec4 quadFace;
	int regionNb;
	int regionId;
	int regionAttr;

	while(std::getline(is, line)) {
		// Skip comments
		if(line.find("#") == std::string::npos) {
			std::istringstream iss(line);
			iss.imbue(std::locale::classic());
			switch(state) {
			case NODE_HEADER:
				if(iss >> sizeVertices >> unused1 >> unused2) {
					scene.modelVertices.reserve(sizeVertices);
					if(sizeVertices > 0) {
						state = NODE_CONTENT;
					} else {
						state = FACET_HEADER;
					}
				}
				break;			
			case NODE_CONTENT:
				if (iss >> idvert >> pt.x >> pt.y >> pt.z) {
					scene.modelVertices.push_back(pt);
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
					if(sizeFaces > 0) {
						state = FACET_CONTENT_INFO;	
					} else {
						state = HOLE_HEADER;
					}
				}
				break;
			case FACET_CONTENT_INFO:
				if (iss >> polygon  >> hole >> newFace.faceIndex) {
					state = FACET_CONTENT_VALUE;
				}
				break;
			case FACET_CONTENT_VALUE:
				if (iss >> vertcount) {
					if(vertcount==3 && iss >> quadFace.a >> quadFace.b >> quadFace.c) {
						newFace.indicesSommets.set(quadFace.a - 1, quadFace.b - 1, quadFace.c - 1);
						scene.modelFaces.push_back(newFace);
					} else if(vertcount==4 && iss >> quadFace.a >> quadFace.b >> quadFace.c >> quadFace.d) {
						newFace.indicesSommets.set(quadFace.a-1,quadFace.b-1,quadFace.c-1);
						scene.modelFaces.push_back(newFace);
						newFace.indicesSommets.set(quadFace.c-1,quadFace.d-1,quadFace.a-1);
						scene.modelFaces.push_back(newFace);
					}
					parsedFaces++;
					if(parsedFaces < sizeFaces) {
						state = FACET_CONTENT_INFO;
					} else {
						state = HOLE_HEADER;
					}
				}
				break;
			case HOLE_HEADER:
				if (iss >> nbHoles) {
					if(nbHoles > 0) {
						state = HOLE_CONTENT;
					} else {
						state = REGION_HEADER;
					}
				}
				break;
			case HOLE_CONTENT:
				if (iss >> holeNb >> pt.x >> pt.y >> pt.z) {
					parsedHoles++;
				}
				if(parsedHoles >= nbHoles) {
					state = REGION_HEADER;
				}
				break;
			case REGION_HEADER:
				if (iss >> nbRegion) {
					if(nbRegion>0) {
						state = REGION_CONTENT;
					} else {
						state = USER_FACE_HEADER;
					}
				}
				break;
			case REGION_CONTENT:
				if (iss >> regionNb >> newRegion.dotInRegion.x >> newRegion.dotInRegion.y >> newRegion.dotInRegion.z >> newRegion.regionIndex >> newRegion.regionRefinement) {
					scene.modelRegions.push_back(newRegion);
					parsedRegion++;
				}
				if (parsedRegion >= nbRegion) {
					state = USER_FACE_HEADER;
				}
				break;
			case USER_FACE_HEADER:
				if (iss >> nbUseFace) {
					if (nbUseFace>0) {
						state = USER_FACE_CONTENT_INFO;
					}
					else {
						break;
					}
				}
				break;
			case USER_FACE_CONTENT_INFO:
				if (iss >> polygon >> hole >> newFace.faceIndex) {
					state = USER_FACE_CONTENT_VALUE;
				}
				break;
			case USER_FACE_CONTENT_VALUE:
				if (iss >> vertcount) {
					if (vertcount == 3 && iss >> quadFace.a >> quadFace.b >> quadFace.c) {
						newFace.indicesSommets.set(quadFace.a - 1, quadFace.b - 1, quadFace.c - 1);
						scene.userDefinedFaces.push_back(newFace);
					}
					else if (vertcount == 4 && iss >> quadFace.a >> quadFace.b >> quadFace.c >> quadFace.d) {
						newFace.indicesSommets.set(quadFace.a - 1, quadFace.b - 1, quadFace.c - 1);
						scene.userDefinedFaces.push_back(newFace);
						newFace.indicesSommets.set(quadFace.c - 1, quadFace.d - 1, quadFace.a - 1);
						scene.userDefinedFaces.push_back(newFace);
					}
					parsedFaces++;
					if (parsedFaces < sizeFaces) {
						state = USER_FACE_CONTENT_INFO;
					}
					else {
						break;
					}
				}
				break;
			}
		}	
	}

}

} //fin namespace