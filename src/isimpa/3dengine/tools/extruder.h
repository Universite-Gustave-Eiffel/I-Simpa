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

/*! \file extruder.h
    \brief Implémentation de la classe Extruder
*/


#include "intersection_seeker.h"

#ifndef __EXTRUDER_TOOL__
#define __EXTRUDER_TOOL__
/**
 * Panel de classes permettant la manipulation de modèle en 3D
 */
namespace ui_tools
{
	/**
	 * @brief Outil d'extrusion de volumes
	 * Retourne la nouvelle base d'une surface en collision avec un modèle défini par une liste de triangles
	 *
	 */
	class Extruder
	{
		private:
		typedef int t_point;
		struct ligne
		{
			t_point	a;
			t_point b;
		};
		static bool isEqual(ligne lLigne,ligne rLigne);
		void GetFaceToDelete(	std::vector<t_faceIndex> retFaces,std::vector<vec3> &_pVertices,std::vector<SGroup3D> &_pGroups,IntersectionSeeker& intersectionTool,std::vector<t_faceIndex> &volumeExtrusion);
		bool CreateWay(std::vector<ivec3> baseFaces,std::vector<ligne>& chemin);

		public:
			/**
			 * Constructeur
			 */
			Extruder();
			/**
			 * Destructeur
			 */
			~Extruder( );

		/**
		 * Effectuer l'extrusion d'un objet à partir d'un modèle
		 * @param[in] intersectionTool Classe permettant de détecter trés rapidement la collision de faces avec un volume
		 * @param _pVertices Tableau de points du modèle
		 * @param _pGroups Tableau de points du modèle
		 * @param[in] volumeExtrusion Indice des triangles contenues dans _pGroups correspondant au volume à créer
		 * @return En cas d'extrusion impossible alors qu'il y a des collisions alors retourne false, true si il n'y a rien a faire (volume flottant) ou si l'extrusion a été fait.
		 */
		bool DoExtrusion(IntersectionSeeker& intersectionTool,
			std::vector<vec3> &_pVertices,
			std::vector<SGroup3D> &_pGroups,
			std::vector<t_faceIndex> &volumeExtrusion
			);
	};

}

#endif
