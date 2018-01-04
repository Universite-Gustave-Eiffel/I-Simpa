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
    \brief Poly file driver (*.poly) 
*/
#include "Core/mathlib.h"
#include <vector>

/*! \brief Poly file driver (*.poly) 
*/
namespace formatPOLY
{

	/**
	 * @brief Region data structure (region is a closed 3d area)
	 */
	struct t_region
	{	
		int regionIndex;					/*!< region id, default is 0 */
		vec3 dotInRegion;					/*!< Coordinate of a point in the volume */
		float regionRefinement;				/*!< (m3) constraint of mesh maximum volume -1 for no constraint */
		t_region(){regionRefinement=-1;}
	};	
	/**
	 * @brief Triangular face
	 */
	struct t_face
	{
		t_face() {};
		t_face(const int& a, const int& b, const int& c, const unsigned int& _faceIndex) :
			indicesSommets(ivec3(a,b,c)), faceIndex(_faceIndex) {
			
		}
		int operator==(const t_face &_f) {
			return _f.faceIndex == faceIndex && (indicesSommets == _f.indicesSommets);
		}

		ivec3 indicesSommets;
		unsigned int faceIndex;
	};

	/**
	 * @brief Main 3D scene structure
	 *
	 */
	struct t_model
	{
		bool saveFaceIndex;
		std::vector<t_face> userDefinedFaces; // Added faces
		std::vector<t_face> modelFaces;
		std::vector<dvec3> modelVertices;
		std::vector<t_region> modelRegions; /*!< Region list */
	};

/**
 *	\class Cpoly  
 *	\brief Poly file driver
 */
class CPoly
{
public:

	/**
	 * Default constructor
	 */
	CPoly();
	/**
	 * Write 3D file
	 */
	bool ExportPOLY(t_model& sceneconst,const std::string& mfilename);
	/**
	 * Read 3D file
	 */
	bool ImportPOLY(t_model& sceneconst,const std::string& mfilename);

};





} 
#endif

