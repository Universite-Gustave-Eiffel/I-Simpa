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

#include "modelRetriever.hpp"
#include <tools/octree44_triangleElement.hpp>
#include <tools/octree44_dotElement.hpp>
#include <tools/octree44_sphereElement.hpp>
#include <Core/mathlib.h>
#include "last_cpp_include.hpp"


class DotTriangleTest
{

public:
	DotTriangleTest(octreeTool::coordPrecision _dot[3],octreeTool::spaceElementContainer* _faces,const float& _epsilon)
		:epsilon(_epsilon),foundDist(2*_epsilon)
	{
		isCandidatesIncludeDot=false;
		COPYVEC(_dot,dot);
		faces=_faces;
	}
	bool CheckCandidate(const octreeTool::elementSize& staticCandidateIndex)
	{
		float ecart=0;
		using namespace octreeTool;
		triangleElement* candidate=dynamic_cast<triangleElement*>(faces->at(staticCandidateIndex));
		if(candidate) //Si le candidat est bien un triangle
		{
			float dist=ClosestDistanceBetweenDotAndTriangle(vec3(candidate->GetSommetX(0),candidate->GetSommetY(0),candidate->GetSommetZ(0)),
				vec3(candidate->GetSommetX(1),candidate->GetSommetY(1),candidate->GetSommetZ(1)),
				vec3(candidate->GetSommetX(2),candidate->GetSommetY(2),candidate->GetSommetZ(2)),
				dot,NULL,NULL);
			/*
			if(DotIsInVertex(dot,vec3(candidate->GetSommetX(0),candidate->GetSommetY(0),candidate->GetSommetZ(0)),
				vec3(candidate->GetSommetX(1),candidate->GetSommetY(1),candidate->GetSommetZ(1)),
				vec3(candidate->GetSommetX(2),candidate->GetSommetY(2),candidate->GetSommetZ(2)),
				&ecart) && ecart<BARELY_EPSILON)
				*/
			if(dist<epsilon)
			{
				if(dist<foundDist)
					foundDist=dist;
				isCandidatesIncludeDot=true;
				//return false; On continue pour trouver la distance la plus proche
			}
		}
		return true;
	}
	bool dotIsInGroup(float& distance)
	{
		distance=foundDist;
		return isCandidatesIncludeDot;
	}
private:
	bool isCandidatesIncludeDot;
	float epsilon;
	float foundDist;
	octreeTool::coordPrecision dot[3];
	octreeTool::spaceElementContainer* faces;
};


ModelRetriever::ModelRetriever()
{
	//1er groupe
	groupes.push_back(groupe_octree());
}

void ModelRetriever::AddFace(octreeTool::coordPrecision sommetA[3],octreeTool::coordPrecision sommetB[3],octreeTool::coordPrecision sommetC[3])
{
	using namespace octreeTool;
	groupes.rbegin()->faces.push_back(new triangleElement(sommetA,sommetB,sommetC));
}
int ModelRetriever::CloseGroup()
{
	groupes.rbegin()->octreeModel=new octreeTool::Octree44(groupes.rbegin()->faces,6);
	groupes.push_back(groupe_octree());
	return groupes.size()-2;
}

void ModelRetriever::GetGroupByDotCoord(octreeTool::coordPrecision dot[3],std::vector<octreeTool::elementSize>& outputGroupIndices,std::vector<float>& outputDistance,const float& epsilon)
{
	//Pour chaque groupe on lance un test de collision point->triangle
	using namespace octreeTool;
	using namespace std;
	sphereElement dotEl(dot,epsilon);
	spaceElement* testElement=&dotEl;
	ModelRetrieverIterator groupIterator;
	elementSize idgroupe=0;
	float distance;
	for( groupIterator = groupes.begin(); groupIterator != groupes.end(); groupIterator++ )
	{
		if(groupIterator->octreeModel)
		{
			DotTriangleTest collisionManager(dot,&(groupIterator->faces),epsilon);
			groupIterator->octreeModel->GetDynamicCollisionCandidates<DotTriangleTest>(*testElement,&collisionManager,&DotTriangleTest::CheckCandidate);
			if(collisionManager.dotIsInGroup(distance))
			{
				outputGroupIndices.push_back(idgroupe);
				outputDistance.push_back(distance);
			}
		}
		idgroupe++;
	}
}

