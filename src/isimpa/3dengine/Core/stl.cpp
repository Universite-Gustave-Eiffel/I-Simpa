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

#include "stl.hpp"
#include <stdio.h>
#include <list>
#include <string>
#include <string.h>
#include "manager/stringTools.h"
namespace formatSTL
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

	float ToFloat(const char* charArray)
	{
		return Convertor::ToFloat(std::string(charArray));
	}


	CStl::CStl()
	{



	}

bool CStl::ExportSTL(t_model& scene, const char* mfilename)
{

  FILE *outfile;

  outfile = fopen(mfilename, "w");
  if (outfile == (FILE *) NULL) {
    printf("File I/O Error:  Cannot create file %s.\n", mfilename);
    return false;
  }
  fclose(outfile);
  return true;
}

struct words_destroyer
{
	words_destroyer(char **_words):words(_words){};
	~words_destroyer(){ free(words);}
	char **words;
};
bool CStl::ImportSTL(t_model& scene,const char* mfilename)
{

  char tmpBuffer[250];
  strcpy(tmpBuffer,"Empty buffer");
  FILE *infile;
  infile = fopen(mfilename, "r+");
  if (infile == (FILE *) NULL) {
    //throw -1; //("File I/O Error:  Cannot open file.\n");
    return false;
  }
  int nwords=0;
  char **words;
  char *orig_line;
  unsigned int sizeVertices=0;
  unsigned int sizeFaces=0;

  /* Lecture du fichier d'entete */
  //Lecture des 5 premier caractere, si "solid" alors format ascii
  fread(tmpBuffer,1,sizeof(char) * 6,infile);
  fclose(infile);
  infile = fopen(mfilename, "r+");
  if (infile == (FILE *) NULL) {
    //throw -1; //("File I/O Error:  Cannot open file.\n");
    return false;
  }

  if(std::string(tmpBuffer).find("solid")!=std::string::npos)
  {
	  //ASCII

	  std::list<vec3> vlist;
	  std::list<ivec3> flist;
	  unsigned int vsize=0;
	  unsigned int fsize=0;
	   while(!feof(infile))
	  {
		vec3 a,b,c;
		//Go to facet word
		while(!feof(infile))
		{
			words = get_words (infile, &nwords, &orig_line);
			words_destroyer free_words(words);
			if(nwords>1)
			{
				if(strcmp(words[0],"facet")==0)
					break;
			}
		}
		//Skip "outer loop"
		words = get_words (infile, &nwords, &orig_line);
		words_destroyer free_words(words);

		t_face newFace;
		//
		// VERTEX A
		words = get_words (infile, &nwords, &orig_line);
		words_destroyer free_words_va(words);
		if(nwords==4)
		{
			a.x = Convertor::ToFloat(std::string(words[1]));
			a.y = Convertor::ToFloat(std::string(words[2]));
			a.z = Convertor::ToFloat(std::string(words[3]));
		}
		//
		// VERTEX B
		words = get_words (infile, &nwords, &orig_line);
		words_destroyer free_words_vb(words);
		if(nwords==4)
		{
			b.x = Convertor::ToFloat(std::string(words[1]));
			b.y = Convertor::ToFloat(std::string(words[2]));
			b.z = Convertor::ToFloat(std::string(words[3]));
		}
		//
		// VERTEX C
		words = get_words (infile, &nwords, &orig_line);
		words_destroyer free_words_vc(words);
		if(nwords==4)
		{
			c.x = Convertor::ToFloat(std::string(words[1]));
			c.y = Convertor::ToFloat(std::string(words[2]));
			c.z = Convertor::ToFloat(std::string(words[3]));
		}
		//Append face
		ivec3 face(vsize,vsize+1,vsize+2);
		fsize++;
		flist.push_front(face);
		vlist.push_back(a);
		vsize++;
		vlist.push_back(b);
		vsize++;
		vlist.push_back(c);
		vsize++;
	  }

	  scene.modelVertices.reserve(vsize);
	  for(std::list<vec3>::iterator itvert=vlist.begin();itvert!=vlist.end();itvert++)
	  {
		  scene.modelVertices.push_back(*itvert);
	  }
	  scene.modelFaces.reserve(fsize);
	  for(std::list<ivec3>::iterator itface=flist.begin();itface!=flist.end();itface++)
	  {
		  scene.modelFaces.push_back(t_face(*itface));
	  }
  }else{
	  //BINARY
	  char header[80];
	  unsigned int triangles_count(0);
	  float normal[3],vertexA[3],vertexB[3],vertexC[3];
	  unsigned short bytecount(0);
	  //Read header
	  fread(header,1,sizeof(char) * 80,infile);
	  //Read number of triangle
	  fread(&triangles_count,1,sizeof(unsigned int),infile);
	  scene.modelFaces.assign(triangles_count,t_face());
	  scene.modelVertices.assign(triangles_count*3,vec3());
	  for(unsigned int idtri=0;idtri<triangles_count;idtri++)
	  {
		  unsigned int a(0),b(0),c(0);
		  //Read face
		  fread(normal,1,sizeof(float) * 3,infile);
		  fread(vertexA,1,sizeof(float) * 3,infile);
		  fread(vertexB,1,sizeof(float) * 3,infile);
		  fread(vertexC,1,sizeof(float) * 3,infile);
		  fread(&bytecount,1,sizeof(unsigned short),infile);

		  scene.modelFaces[idtri].indicesSommets.set(idtri,idtri*3+1,idtri*3+2);
		  scene.modelVertices[idtri]=vertexA;
		  scene.modelVertices[idtri+1]=vertexB;
		  scene.modelVertices[idtri+2]=vertexC;
	  }
  }

  //Lectures des triangles
  fclose(infile);

  return true;
}

} //fin namespace
