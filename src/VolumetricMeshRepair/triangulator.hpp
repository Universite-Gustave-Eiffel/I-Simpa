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

#include "scalar_field_creator.hpp"
#include "adaptative_mesh_guide.hpp"
#include <input_output/progressionInfo.h>
#include <vector>
#include <list>
#include <fstream>

#ifndef __TRIANGULATOR__BASE__
#define __TRIANGULATOR__BASE__

//#define SAVE_FOUND_COLLISION

namespace Triangulators
{
	struct MergeVerticesOperationObjects_t;
	class BaseTriangulator
	{
	protected:
		std::vector<vec3> vertices;
		std::vector<ivec3> faces;
		decimal quality_constraint;
		decimal merge_normal_constraint;
		bool verbose;

		PTR<MergeVerticesOperationObjects_t> mergeVerticesOperationObjects;
		#ifdef SAVE_FOUND_COLLISION
		std::ofstream stlFile;
		#endif

		bool TestCollision(std::list<std::size_t>& facesToTest, const ivec3& faceTest, const std::size_t& exceptVertice );

		std::list<vec3> collision_Debug;
		void SaveXYZCollision();
		bool RefineVertice(vec3* itvert,const std::size_t& vertId,std::vector<std::list<std::size_t> >& verticeToFace,AdaptativeMeshGuide& refinementGuide,const decimal& cellSize);
	public:
		BaseTriangulator();
		virtual ~BaseTriangulator();

		/**
		 * Affecte la contrainte de qualit�. Par d�faut elle est de 0.1
		 * Plus la contrainte est haute plus les triangles g�n�r�s sont r�guliers.
		 */
		void SetQualityConstraint(const decimal& _quality_constraint) { quality_constraint=_quality_constraint; }
		void SetMergeQualityConstraint(const decimal& _merge_quality_constraint) { merge_normal_constraint=_merge_quality_constraint; }
		void SetVerbose(bool _verbose=true) { verbose=_verbose; }
		/**
		 * Start Adding a new face within this object
		 */
		void BeginFeedingFaces();
		/**
		 * Ajout un triangle
		 */
		void AddFace(const vec3& vertA,const vec3& vertB,const vec3& vertC);
		/**
		 * Fin de l'ajout des triangles
		 * Le mod�le peut �tre trait�.
		 */
		void FinishFeedingFaces();
		/**
		 * R�duit le nombre de polygones jusqu'a atteindre au minimum la qualit� en param�tre.En respenctant la contrainte PLC.
		 */
		void PolygonReduction(progressOperation& mainProcess);


		/**
		 * Modifie la position des sommets des triangles de facon � s'approcher du mod�le d'origine
		 */
		void RefineTriangulationGrid(AdaptativeMeshGuide& refinementGuide,const decimal& cellSize, progressOperation& mainProcess,progressionInfo& progressInfo);
		/**
		 * Sauvegarde le mod�le dans un fichier .PLY
		 */

		bool SaveAs(const std::string& filename);

		void InvertFacesNormal();
	};
}

#endif
