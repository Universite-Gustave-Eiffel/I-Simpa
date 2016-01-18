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

#ifndef _HPOLY
#define _HPOLY

/*! \file poly.h
    \brief Implémentation de l'interpréteur de fichier modèle Poly (*.poly) 
*/
#include "Core/mathlib.h"
#include <vector>

/*! \brief Implémentation de l'interpréteur de fichier modèle Poly (*.poly) 
*/
namespace formatPOLY
{

	/**
	 * @brief Structure de données de définition d'une région
	 */
	struct t_region
	{	
		int regionIndex;					/*!< 0 région par défaut */
		vec3 dotInRegion;					/*!< Coordonnées d'un point dans la région */
		float regionRefinement;				/*!< -1 contrainte de volume par défaut (m3) */
		t_region(){regionRefinement=-1;}
	};	
	/**
	 * @brief Structure de données d'echange avec la classe
	 */
	struct t_face
	{
		ivec3 indicesSommets;
		unsigned int faceIndex;
	};

	/**
	 * @brief Structure de données du modèle
	 *
	 */
	struct t_model
	{
		bool saveFaceIndex;
		std::vector<t_face> userDefinedFaces; //Faces rajoutés au modèle
		std::vector<t_face> modelFaces;
		std::vector<vec3> modelVertices;
		std::vector<t_region> modelRegions; /*!< Liste des régions du modèle */
	};

/**
 *	\class Cpoly  
 *	\brief Classe de sauvegarde de fichier POLY
 */
class CPoly
{
public:

	/**
	 * Constructeur
	 */
	CPoly();
	/**
	 * Méthode d'exportation d'un modèle 3D
	 */
	bool ExportPOLY(t_model& sceneconst,const char* mfilename);
	/**
	 * Méthode d'importation d'un modèle 3D
	 */
	bool ImportPOLY(t_model& sceneconst,const char* mfilename);

};





} //Fin namespace
#endif