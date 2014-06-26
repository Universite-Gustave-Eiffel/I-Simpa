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

/*
 * Code source écrit dans le cadre du projet OPALHA contrat ADEME n°06.04.C.0070 (2007)
 */
#include "octree44.hpp"
/**
 * @file octree44_segmentElement.hpp
 *
 * @brief Implémentation de l'élément segment de droite
 */

#ifndef __OCTREE44_SEGMENTELEMENT_H__
#define __OCTREE44_SEGMENTELEMENT_H__

namespace octreeTool
{
	/**
	 * @brief Element de type segment de droite
	 *
	 * Surtout utile pour les tests au sein de l'octree. Il est possible de le dériver afin de définir un rayon et non un segment seul
	 */
	class segmentElement : public spaceElement
	{
		public:
			segmentElement(const coordPrecision positionA[3],const coordPrecision positionB[3]);
			segmentElement( );
			virtual bool IsOverlap(coordPrecision boxcenter[3],const coordPrecision& boxhalfsize);
			virtual void GetMinMax(coordPrecision xyzMin[3],coordPrecision xyzMax[3]);
			void Set(const coordPrecision positionA[3],const coordPrecision positionB[3]);
	protected:
			/**
			 * Projection du point vers un segment de droite définit par deux coordonnées VA et VB, res correspond alors au point de la projection
			 *
			 * @param[in] point Point dans l'espace a projeter sur la droite
			 * @param[in] VA Position du point A appartenant à la droite AB
			 * @param[in] VB Position du point B appartenant à la droite AB
			 * @param[out] res Position du point I appartenant au segment de droite AB où AI (ou BI) forme un angle droit avec IP. Cette position est limitée entre VA et VB.
			 * @warning
			 */
			static void closestPointOnSegment(float point[3],float VA[3],float VB[3],float res[3]);

			/**
			 * Projection du point vers la droite définit par deux coordonnées VA et VB.
			 *
			 * @param[in] point Point dans l'espace a projeter sur la droite
			 * @param[in] VA Position du point A appartenant à la droite AB
			 * @param[in] VB Position du point B appartenant à la droite AB
			 * @param[out] res Correspond à un facteur ( 0 correspond à VA et 1 correspond à VB )
			 * @warning Res appartient au domaine ]-Inf;+Inf[ car il s'agit d'une projection sur un droite et non sur un segment.
			 */
			static void projectionOnLine( float point[3],float VA[3],float VB[3], float* res);

			coordPrecision CoordPositionA[3];
			coordPrecision CoordPositionB[3];

	};
}

#endif
