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

#ifndef _HPLY
#define _HPLY

/*! \file ply.h
    \brief Implémentation de l'interpréteur de fichier modèle Ply (*.ply) 
	@see 
*/
#include "Mathlib.h"
#include <vector>

/*! \brief Implémentation de l'interpréteur de fichier modèle Poly (*.poly) 
*/
namespace formatPLY
{

	/**
	 * @brief Structure de données d'echange avec la classe
	 */
	struct t_face
	{
		ivec3 indicesSommets;
	};

	/**
	 * @brief Structure de données du modèle
	 *
	 */
	struct t_model
	{
		std::vector<t_face> modelFaces;
		std::vector<vec3> modelVertices;
	};

/**
 *	\class Cply  
 *	\brief Classe de sauvegarde et de chargement de fichier PLY (stanford)
 */
class CPly
{
public:

	/**
	 * Constructeur
	 */
	CPly();
	/**
	 * Méthode d'importation d'un modèle 3D
	 */
	bool ImportPly(t_model& sceneconst,const char* mfilename);
	/**
	 * Méthode d'exportation d'un modèle 3D
	 */
	bool ExportPly(t_model& scene, const char* mfilename);
private:
	enum {FILENAMESIZE = 1024};

};





} //Fin namespace
#endif