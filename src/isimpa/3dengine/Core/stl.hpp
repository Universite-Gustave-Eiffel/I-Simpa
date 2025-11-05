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

#ifndef _HSTL
#define _HSTL

/*! \file stl.hpp
    \brief Impl�mentation de l'interpr�teur de fichier mod�le STL (*.stl) 
*/
#include "Core/mathlib.h"
#include <vector>

/*! \brief Impl�mentation de l'interpr�teur de fichier mod�le STL (*.stl) 
*/
namespace formatSTL
{

	/**
	 * @brief Structure de donn�es d'echange avec la classe
	 */
	struct t_face
	{
		t_face()
		{ }
		t_face(const ivec3& sommets)
			: indicesSommets(sommets)
		{ }
		ivec3 indicesSommets;
	};

	/**
	 * @brief Structure de donn�es du mod�le
	 *
	 */
	struct t_model
	{
		std::vector<t_face> modelFaces;
		std::vector<vec3> modelVertices;
	};

/**
 *	\class Cstl
 *	\brief Classe de sauvegarde de fichier STL
 */
class CStl
{
public:

	/**
	 * Constructeur
	 */
	CStl();
	/**
	 * M�thode d'exportation d'un mod�le 3D
	 */
	bool ExportSTL(t_model& sceneconst,const char* mfilename);
	/**
	 * M�thode d'importation d'un mod�le 3D
	 */
	bool ImportSTL(t_model& sceneconst,const char* mfilename);

};

} //Fin namespace
#endif