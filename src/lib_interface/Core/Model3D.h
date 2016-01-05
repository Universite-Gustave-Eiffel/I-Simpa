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

#ifndef _HMODEL
#define _HMODEL

/**
 * @file Model3D.h
 * 
 * @brief Définition de types en rapport avec les modèles 3D
 */
#include <vector>

/**
 * @brief Indice d'une face
 * 
 * Cette classe gère l'indexation d'une face
 */
class t_faceIndex {
public:
	t_faceIndex(void) : f(0), g(0) { }
	t_faceIndex(long _face,long _group) : f(_face), g(_group) { }
	t_faceIndex(const t_faceIndex *_faceIndex) : f(_faceIndex->f), g(_faceIndex->g) { }
	t_faceIndex(const t_faceIndex &_faceIndex) : f(_faceIndex.f), g(_faceIndex.g) { }

	int operator==(const t_faceIndex &_faceIndex) { return (this->f == _faceIndex.f && this->g == _faceIndex.g ); }
	int operator!=(const t_faceIndex &_faceIndex) { return !(*this == _faceIndex); }
	
	void Set(long _face,long _group) { f=_face;g=_group; }
	void Set(const t_faceIndex *_faceIndex) { f=_faceIndex->f;g=_faceIndex->g; }
	void Set(const t_faceIndex &_faceIndex) { f=_faceIndex.f;g=_faceIndex.g;}

	union {
		struct {long f,g;};
	};
};

/**
 * Structure d'un triangle
 */
struct triangleFace {
    float a[3];           
	float b[3];
	float c[3];
};

/**
 * Structure complète d'une face en 3D
 */
struct SFace3D 
{
	ivec3 Vertices;   
	vec3 FaceNormals; 
};

/**
 * Structure complète d'un groupe de surface
 */
struct SGroup3D 
{
	std::vector<SFace3D> pFaces; 
	long numOfVerts;
};

#endif