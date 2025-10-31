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

#include "first_header_include.hpp"

/**
 * @file modelRetriever.hpp
 * @brief Cette classe permet d'associer les faces d'un ancien modèle avec les faces d'un nouveau modèle
 */

#include <tools/octree44.hpp> //Optimisation de la recherche de faces
#include <list>


#ifndef __TOOLS_MODEL_RETRIEVER

/**
 *
 * On définit plusieurs groupes et pour chaque groupe on insère plusieurs faces
 */

class ModelRetriever
{

private:
	struct groupe_octree
	{
		groupe_octree()
		{
			octreeModel=NULL;
		}
		~groupe_octree()
		{
			delete octreeModel;
			for(octreeTool::elementSize idface=0;idface<faces.size();idface++)
				delete faces[idface];
		}
		octreeTool::spaceElementContainer faces;
		octreeTool::Octree44* octreeModel;
	};
	
	typedef std::list<groupe_octree> ModelRetrieverVector;
	typedef ModelRetrieverVector::iterator ModelRetrieverIterator;
	ModelRetrieverVector groupes;
public:

	ModelRetriever();

	void AddFace(octreeTool::coordPrecision sommetA[3],octreeTool::coordPrecision sommetB[3],octreeTool::coordPrecision sommetC[3]);
	int CloseGroup();
	/**
	 * Donne la liste des groupes correspondant aux surfaces les plus proches de dot
	 * @param epsilon Distance maximale des surfaces d'origine
	 */
	void GetGroupByDotCoord(octreeTool::coordPrecision dot[3],std::vector<octreeTool::elementSize>& outputGroupIndices,std::vector<float>& outputDistance,const float& epsilon);
};

#endif