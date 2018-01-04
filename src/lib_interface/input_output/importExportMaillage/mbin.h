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

#include <vector>
#include <string.h>
#include <cstdint>
#include "../../Core/mathlib.h"

#ifndef __MBIN__
#define __MBIN__

/*! \file mbin.h
    \brief Import export du maillage tétrahedrique
*/

/**
 * \brief Import export du maillage tétrahedrique
 */
namespace formatMBIN
{

//Types Import/Export

typedef uint16_t bCourt;
typedef int32_t Intb;
typedef float Floatb;
typedef uint32_t Longb ;
typedef bool Boolb ;

	
struct t_binNode
{
	t_binNode()
		: node {0,0,0}
	{
	}
	t_binNode(const Floatb& x,const Floatb& y,const Floatb& z): node {x,y,z}
	{
	}
	Floatb node[3];
	int operator==(const t_binNode &_f) {
		return node[0]==_f.node[0] && node[1]==_f.node[1] && node[2]==_f.node[2];
	}
	operator float*() { return this->node; }
	const float &operator[](int _i) const { return this->node[_i]; }
};

/**
 * @brief Tetrahedra face structure
 */
	struct bintetraface {
		bintetraface() {}
		bintetraface(Intb a, Intb b, Intb c, Intb _marker, Intb _neighboor) 
			: vertices(a,b,c), marker(_marker), neighbor(_neighboor) {
		}
		ivec3 vertices;
		Intb marker = -1;    /*!< Associated model face -1 nothing */
		Intb neighbor = -2; /*!< Tetrahedra neighbor index [0-n]. -2 no neighbor */
		int operator==(const bintetraface &_f) {
			return vertices == _f.vertices && _f.marker == marker && _f.neighbor == neighbor;
		}
	};

/**
 * @brief Structure d'un tétrahedre du maillage
 *
 */
	struct bintetrahedre {
		bintetrahedre(const Intb& _a, const Intb& _b, const Intb& _c, const Intb& _d, const Intb& _idVolume, 
			const bintetraface& faceA, const bintetraface& faceB, const bintetraface& faceC, const bintetraface& faceD)
			: vertices( _a, _b, _c, _d ), idVolume(_idVolume), tetrafaces { faceA , faceB , faceC , faceD }
		{
		 }
		bintetrahedre() {}
		ivec4 vertices;
		Intb idVolume = 0;
		bintetraface tetrafaces[4]{bintetraface(), bintetraface(), bintetraface(), bintetraface() };

		int operator==(const bintetrahedre &_f) {
			return vertices == _f.vertices &&
				   idVolume == _f.idVolume &&
				   tetrafaces[0] == _f.tetrafaces[0] &&
				   tetrafaces[1] == _f.tetrafaces[1] &&
				   tetrafaces[2] == _f.tetrafaces[2] &&
				   tetrafaces[3] == _f.tetrafaces[3];
		}
	};

struct trimeshmodel
{
	std::vector<bintetrahedre> tetrahedres;
	std::vector<t_binNode> nodes;
};


/**
 *	\brief Classe de sauvegarde et de chargement du maillage tétrahedrique
 */
class CMBIN
{
public:
	/** Constructeur
	 */
	CMBIN(){}
	/** Destructeur
	 */
	~CMBIN(){}
	/// @cond INCLUDE_PYTHON_HIDDEN_MEMBERS
	/**
	 * Méthode d'importation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @param tabTetra Tableau de tetrahèdre
	 * @param tabNodes Tableau de noeuds
	 * @param sizeTetra Taille du tableau de tetrahèdre
	 * @param sizeNodes Taille du tableau de tetrahèdre
	 * @return Vrai si l'opération a réussi
	 */
    bool ImportBIN(const char *strFileName,bintetrahedre **tabTetra,t_binNode **tabNodes,unsigned int &sizeTetra,unsigned int &sizeNodes);
	/**
	 * Méthode d'exportation d'un modèle 3D
	 * @param strFileName Chemin du fichier
	 * @param tabTetra Tableau de tetrahèdre
	 * @param tabNodes Tableau de noeuds
	 * @param sizeTetra Taille du tableau de tetrahèdre
	 * @param sizeNodes Taille du tableau de tetrahèdre
	 * @return Vrai si l'opération a réussi
	 */
	bool ExportBIN(const char *strFileName,const bintetrahedre* tabTetra,const t_binNode* tabNodes,unsigned int sizeTetra,unsigned int sizeNodes);
	/// @endcond
	/**
	 * Méthode d'importation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @return Le maillage
	 */
	trimeshmodel LoadMesh(const char *strFileName);
	/**
	 * Méthode d'exportation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @param trimesh Le maillage
	 */
	bool SaveMesh(const char *strFileName,trimeshmodel& trimesh);
	/*
	 * Calcul la somme des volumes des tetrahèdre contenu de le modèle
	 * @return Volume m3
	 */
	static double ComputeVolume(trimeshmodel& trimesh);
};


} //namespace
#endif
