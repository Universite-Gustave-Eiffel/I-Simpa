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
/**
 * @file octree44_sphereElement.hpp Implémentation de l'élément de type sphérique
 */

#ifndef __OCTREE44_SPHEREELEMENT_H__
#define __OCTREE44_SPHEREELEMENT_H__

namespace octreeTool
{
	/**
	 * @brief Element de type sphere
	 */
	class sphereElement : public spaceElement
	{
		public:
			sphereElement(const coordPrecision position[3],const coordPrecision& _radius);
			sphereElement( );

			coordPrecision GetPositionX( );
			coordPrecision GetPositionY( );
			coordPrecision GetPositionZ( );
			coordPrecision GetRadius( );
			virtual ~sphereElement() {};
			virtual bool IsOverlap(coordPrecision boxcenter[3],const coordPrecision& boxhalfsize);
			virtual void GetMinMax(coordPrecision xyzMin[3],coordPrecision xyzMax[3]);
			void Set(const coordPrecision position[3]);
			void SetRadius(const coordPrecision& _radius);
	protected:
			coordPrecision CoordPosition[3];
			coordPrecision radius;
	};
}

#endif
