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

/**
 * Cette classe permet de connaitre de facon rapide le point le plus proche des surfaces à partir d'un point de référence
 * Ainsi la classe BaseTriangulator est capable de modifier sa grille afin de coller au plus proche du modèle d'origine. Tout en garantissant la réussite d'un maillage tétraèdrique
 */
#include "smart_macro.h"
#include <Core/mathlib.h> //Mathlib de libinterface
#include <list>
#include <utility>

#ifndef __ADAPTATIVE_MESH_GUIDE__
#define __ADAPTATIVE_MESH_GUIDE__


namespace Triangulators
{
	typedef std::pair<decimal,vec3> Candidate_t;
	typedef std::list<Candidate_t> ClosestCandidates_t;
	class AdaptativeMeshGuide
	{
	private:
		struct AdaptativeMeshGuideData_t;
		typedef PTR<AdaptativeMeshGuideData_t> shared_ptr_data;
		shared_ptr_data AdaptativeMeshGuideData;
	public:
		/**
		 * Constructeur
		 * @param minBox Position x,y,z minimum de la boite englobante
		 * @param maxBox Position x,y,z maximum de la boite englobante
		 */
		AdaptativeMeshGuide();
		/**
		 * Ajoute un triangle dans l'espace de l'instance de AdaptativeMeshGuide
		 */
		void AddTri(const vec3& A,const vec3& B,const vec3& C);
		/**
		 * @param maxDepth Discretisation, maxDepth=log(cellCount)/log(2)
		 */
		void EndTriangleFeeding(const unsigned short maxDepth=5);
		/**
		 *  Recherche le point planaire par rapport aux surfaces du domaine le plus proche du point d'origine en paramètre.
		 *  @param OriginPoint Point à partir duquel ce fait le test
		 *  @param PointNormal Normal du point. Cette normale permet de rendre en priorité le point le plus proche dans la direction de la normale
		 *  @param maxDist Contrainte de test des surfaces autour du point
		 */
		bool GetClosestPoint(const vec3& OriginPoint,const decimal& maxDist,ClosestCandidates_t& foundPositions);

		void SortByDistance(ClosestCandidates_t& foundPositions);
	};
};

#endif
