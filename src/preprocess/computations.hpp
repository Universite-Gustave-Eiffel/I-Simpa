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
 * @file computations.hpp 
 * @brief Ces méthodes regroupent les méthodes permettant l'adaptation d'un modèle non conforme à tetgen.
 */
#include "input_output/poly.h"
#include "input_output/progressionInfo.h"
#include <tools/octree44.hpp>
#include <tools/octree44_triangleElement.hpp>


#ifndef __MESH_COMPUTATIONS__
#define __MESH_COMPUTATIONS__


class OctreeResult
{
	public:
		OctreeResult(octreeTool::elementSize _exception):exception(_exception){};
	bool DotTriangleCollisionTest(const octreeTool::elementSize& staticCandidateIndex)
	{
		if(exception<staticCandidateIndex)
			results.push_back(staticCandidateIndex);
		return true;
	}
	std::vector<octreeTool::elementSize> results;
	octreeTool::elementSize exception;
};



class meshOperation
{
private:



	unsigned int stat_NbFaceSplitting;
	unsigned int stat_DestroyedFaces;
	unsigned int stat_NbPointsMerged;
	formatPOLY::t_model modelData;
	octreeTool::spaceElementContainer lstFaces;
	octreeTool::Octree44* modelOctree;
	
	int FindFaceWithSommet( const ivec3& sommetsSearch );
	//Retourne -1 si aucun point correspondant
	int FindIndexWithPosition( const dvec3& position );
	//return vrai si la division a eu lieu
	//addedFaceOne -1 ou idface (si inséré)
	//addedFaceTwo -1 ou idface (si inséré)
	bool SplitTriangleByThree( formatPOLY::t_face& triangleToSplit , const int& splitCentre,int* addedFaceOne,int* addedFaceTwo  );

	//return faux si aucun traitement effectué
	bool OnCollisionDetectedSplitIt( formatPOLY::t_face& triangleOne, formatPOLY::t_face& triangleTwo, const dvec3& position );
	bool OnCollisionDetectedDestroyIt( formatPOLY::t_face& triangleOne, formatPOLY::t_face& triangleTwo, const dvec3& position );
	void PushNewFace(const formatPOLY::t_face& newFace);
	void ReBuildOctree();
	bool coplanarIntersection(int idface,int idfaceTest);
public:
	meshOperation(const char* polyFileName);
	~meshOperation();

	void mergeVertices();
	/**
	 * Correction d'une erreur dans le modèle
	 * @return Vrai si le modèle a été modifié à cause de collision ou de superposition de triangles
	 */
	bool MeshReconstruction();
	/**
	 * Supprime les triangles qui rentrent en intersection
	 */
	bool MeshDestroyIntersectingTriangles();
	/**
	 *  Supprime les faces ayant une aire égale à 0 m²
	 */
	void DestroyNoAreaFaces();
	//Les faces définies dans la structure en tant que faces à tester sont transférées dans la liste des faces standart.
	void TransferUserFaceToGlobalFaces();
	bool MeshDestroyCoplanarFaces();
	void ShowStats();
	void Save(const char* polyFileName);
	bool IsOk();
};


#endif