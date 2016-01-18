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

typedef unsigned short bCourt;
typedef int Intb;
typedef float Floatb;
typedef unsigned long Longb ;
typedef bool Boolb ;
struct t_binNode
{
	t_binNode()
	{
		memset(node,0,sizeof(node));
	}
	t_binNode(const Floatb& x,const Floatb& y,const Floatb& z){node[0]=x;node[1]=y;node[2]=z;}
	Floatb node[3];
	int operator==(const t_binNode &_f) {
		return node[0]==_f.node[0] && node[1]==_f.node[1] && node[2]==_f.node[2];
	}
};
/**
 * @brief Structure d'une face d'un tetrahedre
 */
struct bintetraface
{
	Intb sommets[3];
	Intb marker;	/*!< Face du modèle -1 aucune */
	Intb neighboor; /*!< Indice de tetraHedre 0 à n. -2 pour aucun voisin sur cette face */
};

/**
 * @brief Structure d'un tétrahedre du maillage
 *
 */
struct bintetrahedre
{
	Intb sommets[4];
	Intb idVolume;
	bintetraface tetrafaces[4];
	int operator==(const bintetrahedre &_f) {
		return sommets[0]==_f.sommets[0] &&
			sommets[1]==_f.sommets[1] &&
			sommets[2]==_f.sommets[2] &&
			sommets[3]==_f.sommets[3];
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
	bool ExportBIN(const char *strFileName,bintetrahedre **tabTetra,t_binNode **tabNodes,unsigned int sizeTetra,unsigned int sizeNodes);
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
	void SaveMesh(const char *strFileName,trimeshmodel& trimesh);
	/*
	 * Calcul la somme des volumes des tetrahèdre contenu de le modèle
	 * @return Volume m3
	 */
	static double ComputeVolume(trimeshmodel& trimesh);
};


} //namespace
#endif
