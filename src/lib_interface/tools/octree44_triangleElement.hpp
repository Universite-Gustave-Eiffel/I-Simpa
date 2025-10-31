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

/*
 * Code source écrit dans le cadre du projet OPALHA contrat ADEME n°06.04.C.0070 (2007)
 */
#include "octree44.hpp"


#ifndef __OCTREE44_TRIELEMENT_H__
#define __OCTREE44_TRIELEMENT_H__
/**
 * Indique si un triangle et un cube se superpose
 * @param[in] boxcenter Centre de la boite
 * @param[out] boxhalfsize Dimension de la moitiée de la boite
 * @param[out] triverts Position des sommets des triangles
 * @return 0 si aucun contact, 1 si il y a une superposition
 */
namespace boxtri_test
{
	int triBoxOverlap(const float boxcenter[3],const float boxhalfsize[3],const float A[3],const float B[3],const float C[3]);
}
namespace octreeTool
{
	/**
	 * @brief Element de type triangle
	 */
	class triangleElement : public spaceElement
	{
		public:
			triangleElement(const coordPrecision sommetA[3],const coordPrecision sommetB[3],const coordPrecision sommetC[3]);
			triangleElement( );
			/**
			 * Retourne la position X du sommet
			 * @param indexSommet A:0 B:1 C:2
			 */
			coordPrecision GetSommetX( sommetIndex indexSommet );
			/**
			 * Retourne la position Y du sommet
			 * @param indexSommet A:0 B:1 C:2
			 */
			coordPrecision GetSommetY( sommetIndex indexSommet );
			/**
			 * Retourne la position Z du sommet
			 * @param indexSommet A:0 B:1 C:2
			 */
			coordPrecision GetSommetZ( sommetIndex indexSommet );
			virtual ~triangleElement() {};
			virtual bool IsOverlap(coordPrecision boxcenter[3],const coordPrecision& boxhalfsize);
			virtual void GetMinMax(coordPrecision xyzMin[3],coordPrecision xyzMax[3]);
			void Set(const coordPrecision sommetA[3],const coordPrecision sommetB[3],const coordPrecision sommetC[3]);
	protected:
			coordPrecision sommets[3][3];

	};
}

#endif
