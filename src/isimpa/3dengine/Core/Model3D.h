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

#include "first_header_include.hpp"

#ifndef _HMODEL
#define _HMODEL
/**
 * @file Model3D.h
 *
 * @brief Définition de types en rapport avec les modèles 3D
 */
#include <string>
#include <string.h>
#include <utility>
#include <vector>
#include "3dengine/Core/Mathlib.h"
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

	int operator==(const t_faceIndex &_faceIndex) const { return (this->f == _faceIndex.f && this->g == _faceIndex.g ); }
	int operator!=(const t_faceIndex &_faceIndex) const  { return !(*this == _faceIndex); }

	std::size_t GetHashValue() const;
	void Set(long _face,long _group) { f=_face;g=_group; }
	void Set(const t_faceIndex *_faceIndex) { f=_faceIndex->f;g=_faceIndex->g; }
	void Set(const t_faceIndex &_faceIndex) { f=_faceIndex.f;g=_faceIndex.g;}
	bool IsOk() { if(f ==-1 || g==-1) return false; else return true; }
	union {
		struct {long f,g;};
		struct {long face,group;};
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
 * @brief Structure d'une face d'un tetrahedre
 */
struct tetraface
{
	ivec3 sommets;
	unsigned int marker;
};

/**
 * @brief Structure d'un tétrahedre du maillage
 *
 */
struct tetrahedre
{
	ivec4 sommets;
	tetraface tetrafaces[4];
	ivec4 tetraNeighboor;		/*!< Si tetraNeighboor[n]==-2 pas de voisin (n pour la face n) */
	int idVolume;				/*!< Indice du volume. 0 Volume général, indice de l'encombrement (xmlId de l'élément) */
};


/**
 * Structure d'un plan de coupe du maillage
 */
struct t_cutPlane{
	  bool actif;	/*!< Activer ou non le plan de coupe */
	  int sign;		/*!< -1 +1 Sens de coupe */
	  float value;	/*!< -1. +1. valeur de coupe (coordonné > ou < non affiché) */
	  int plane;	/*!< x,y,z plan de coupe x:0 y:1 z:2 */
};
/**
 * Structure complète d'une face en 3D
 */
struct SFace3D {
	SFace3D() : selected(false), internalFace(false), idMaterial(-1) {
		memset(Diff, 1, sizeof(bool));
	}

	bool selected;
	ivec3 Vertices;
	ivec3 Normals;
	vec3 FaceNormals;
	ivec3 TexCoords;
	bool Diff[3]{}; /*!< Affichage ou non des lignes du triangle A B C */
	bool internalFace;
	short idMaterial; /*!< Identifiant du matériau d'origine du modèle */
};

/**
 * Structure complète d'un groupe de surface
 */
struct SGroup3D {
	SGroup3D() : Material(-1) {
	};

	explicit SGroup3D(std::string groupName)
		: Name(std::move(groupName)), Material(-1) {
	};
	std::string Name;
	std::vector<SFace3D> pFaces;
	long Material; /*!< Identifiant du matériau d'origine du modèle */
};

#endif
