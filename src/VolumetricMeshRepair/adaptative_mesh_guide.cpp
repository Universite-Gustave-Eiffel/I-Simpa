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

#include "adaptative_mesh_guide.hpp"
#include <tools/octree44_triangleElement.hpp>
#include <tools/octree44_sphereElement.hpp>
#include <map>
#include <utility>


namespace Triangulators
{
	#ifndef MIN
		#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
	#endif
	#ifndef MAX
		#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
	#endif

	#ifndef MINREF
		#define MINREF(a, b)  if(a>b) a=b;
	#endif
	#ifndef MAXREF
		#define MAXREF(a, b)  if(a<b) a=b;
	#endif
	#ifndef MINVEC
		#define MINVEC(av, bv)  MINREF(av[0],bv[0]);MINREF(av[1],bv[1]);MINREF(av[2],bv[2]);
	#endif
	#ifndef MAXVEC
		#define MAXVEC(av, bv)  MAXREF(av[0],bv[0]);MAXREF(av[1],bv[1]);MAXREF(av[2],bv[2]);
	#endif

	AdaptativeMeshGuide::AdaptativeMeshGuide()
		:AdaptativeMeshGuideData(shared_ptr_data(new AdaptativeMeshGuideData_t))
	{


	}

	void AdaptativeMeshGuide::EndTriangleFeeding(const unsigned short maxDepth)
	{
		using namespace octreeTool;
		//
		this->AdaptativeMeshGuideData->octreeSpace=PTR<octreeTool::Octree44>(new octreeTool::Octree44(this->AdaptativeMeshGuideData->minBox,this->AdaptativeMeshGuideData->maxBox));
		for(trianglesMap_iterator_t mapit=this->AdaptativeMeshGuideData->trianglesMap.begin();mapit!=this->AdaptativeMeshGuideData->trianglesMap.end();mapit++)
		{
			tri_t& srcTri=*mapit->second.get();
			octreeTool::triangleElement newTri(srcTri.A,srcTri.B,srcTri.C);
			this->AdaptativeMeshGuideData->octreeSpace->PushNewSpaceElement(&newTri,mapit->first,maxDepth);
		}
	}


	void AdaptativeMeshGuide::AddTri(const vec3& A,const vec3& B,const vec3& C)
	{
		std::size_t idObject=this->AdaptativeMeshGuideData->trianglesMap.size();
		if(idObject==0)
			this->AdaptativeMeshGuideData->maxBox=this->AdaptativeMeshGuideData->minBox=A;

		MAXVEC(this->AdaptativeMeshGuideData->maxBox,A);
		MINVEC(this->AdaptativeMeshGuideData->minBox,A);
		MAXVEC(this->AdaptativeMeshGuideData->maxBox,B);
		MINVEC(this->AdaptativeMeshGuideData->minBox,B);
		MAXVEC(this->AdaptativeMeshGuideData->maxBox,C);
		MINVEC(this->AdaptativeMeshGuideData->minBox,C);
		this->AdaptativeMeshGuideData->trianglesMap.insert(trianglesMap_pair_t(idObject,tri_ptr_t(new tri_t(A,B,C))));
	}
	struct ClosestPointResult_t
	{
		decimal distance;
		vec3 position;
	};
	bool ClosestSortPredicate(const Candidate_t& left,const Candidate_t& right)
	{
		return left.first<right.first;
	}
	void AdaptativeMeshGuide::SortByDistance(ClosestCandidates_t& foundPositions)
	{
		foundPositions.sort(ClosestSortPredicate);
		//std::sort(foundPositions.begin(),foundPositions.end(),&ClosestSortPredicate);

	}

	bool AdaptativeMeshGuide::GetClosestPoint(const vec3& OriginPoint,const decimal& maxDist,ClosestCandidates_t& foundPositions)
	{
		using namespace octreeTool;
		sphereElement testZone(OriginPoint,maxDist);
		std::vector<elementSize> candidates;
		this->AdaptativeMeshGuideData->octreeSpace->GetAllCollisionCandidates(testZone,candidates);
		bool foundSomething(false);
		for(std::vector<elementSize>::iterator itCandidates=candidates.begin();itCandidates!=candidates.end();itCandidates++)
		{
			trianglesMap_iterator_t tri_it=this->AdaptativeMeshGuideData->trianglesMap.find(*itCandidates);
			if(tri_it!=this->AdaptativeMeshGuideData->trianglesMap.end())
			{
				decimal pfs;
				decimal pft;
				decimal distance;
				tri_t& curTriangle(*(tri_it->second.get()));
				distance=ClosestDistanceBetweenDotAndTriangle(curTriangle.A,
					curTriangle.B,
					curTriangle.C,
					OriginPoint,
					&pfs,&pft);
				if(distance<maxDist)
				{
					foundSomething=true;
					vec3 position=curTriangle.A+(curTriangle.B-curTriangle.A)*pfs+(curTriangle.C-curTriangle.A)*pft;
					foundPositions.push_back(Candidate_t(distance,position));
				}
			}
		}

		#ifdef _DEBUG
		if(!foundSomething)
			printf("rejet� ! \n");
		#endif
		return foundSomething;
	}

	/*
	bool AdaptativeMeshGuide::GetClosestPoint(const vec3& OriginPoint,const decimal& maxDist,ClosestCandidates_t& foundPositions)
	{
		bool foundSomething(false);

		//for(std::vector<elementSize>::iterator itCandidates=candidates.begin();itCandidates!=candidates.end();itCandidates++)
		for(trianglesMap_iterator_t tri_it=this->AdaptativeMeshGuideData->trianglesMap.begin();tri_it!=this->AdaptativeMeshGuideData->trianglesMap.end();tri_it++)
		{
			//trianglesMap_iterator_t tri_it=this->AdaptativeMeshGuideData->trianglesMap.find(*itCandidates);
			if(tri_it!=this->AdaptativeMeshGuideData->trianglesMap.end())
			{
				decimal pfs;
				decimal pft;
				decimal distance;
				tri_t& curTriangle(*(tri_it->second.get()));
				distance=ClosestDistanceBetweenDotAndTriangle(curTriangle.A,
					curTriangle.B,
					curTriangle.C,
					OriginPoint,
					&pfs,&pft);
				if(distance<maxDist)
				{
					foundSomething=true;
					vec3 position=curTriangle.A+(curTriangle.B-curTriangle.A)*pfs+(curTriangle.C-curTriangle.A)*pft;
					foundPositions.push_back(Candidate_t(distance,position));
				}
			}
		}

		#ifdef _DEBUG
		if(!foundSomething)
			printf("rejet� ! \n");
		#endif
		return foundSomething;
	}
	*/
};
