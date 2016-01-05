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

#include "intersection_seeker.h"
#include <tools/octree44_dotElement.hpp>
#include <tools/octree44_segmentElement.hpp>
#include <tools/octree44_sphereElement.hpp>
#include <wx/hashmap.h>
#include <wx/log.h>
#ifdef _DEBUG
#include <wx/datetime.h>
#include "last_cpp_include.hpp"
#endif

namespace ui_tools
{
	
	WX_DECLARE_HASH_MAP( int, bool, wxIntegerHash, wxIntegerEqual, BoolHashMap );

	IntersectionSeeker::IntersectionSeeker(std::vector<vec3> &_pVertices,std::vector<SGroup3D> &_pGroups)
	{
		using namespace octreeTool;
		#ifdef _DEBUG
		wxDateTime timeDebCalculation=wxDateTime::UNow();
		#endif
		//Compte le nombre de faces afin de réserver l'espace mémoire
		size_t n=0;
		for(size_t g=0; g < _pGroups.size(); n+=_pGroups[g++].pFaces.size());
		modelFaces.reserve(n);
		//octreeTool::spaceElementContainer octreeFaces;
		//octreeFaces.reserve(n);
		vec3 Lo,Hi;
		if(_pVertices.size()>0)
		{
			Lo=_pVertices[0];
			Hi=Lo;
		}

		for(std::size_t v = 0; v < _pVertices.size() ;v++) 
		{
			if(_pVertices[v].x < Lo.x) Lo.x = _pVertices[v].x;
			if(_pVertices[v].y < Lo.y) Lo.y = _pVertices[v].y;
			if(_pVertices[v].z < Lo.z) Lo.z = _pVertices[v].z;
			if(_pVertices[v].x > Hi.x) Hi.x = _pVertices[v].x;
			if(_pVertices[v].y > Hi.y) Hi.y = _pVertices[v].y;
			if(_pVertices[v].z > Hi.z) Hi.z = _pVertices[v].z;
		}

		
		modelOctree= new Octree44(Lo,Hi); //(int)(logf(n+1)*2)

		short depth=(int)(log10f(n)+3);
		depth=MIN(depth,9);
		depth=MAX(depth,4);
		std::size_t idface(0);
		for(size_t g=0; g < _pGroups.size();g++)
		{
			for(size_t f=0; f < _pGroups[g].pFaces.size();f++)
			{
				pspsElement* newEl=new pspsElement(t_faceIndex(f,g),_pVertices,_pGroups[g].pFaces[f].Vertices);
				modelFaces.push_back(newEl);
				modelOctree->PushNewSpaceElement(newEl,idface,depth);
				idface++;
			}
		}
		
		/*
		octreeTool::spaceElementContainer octreeFaces;
		octreeFaces.reserve(n);
		std::size_t idface(0);
		for(size_t g=0; g < _pGroups.size();g++)
		{
			for(size_t f=0; f < _pGroups[g].pFaces.size();f++)
			{
				pspsElement* newEl=new pspsElement(t_faceIndex(f,g),_pVertices,_pGroups[g].pFaces[f].Vertices);
				modelFaces.push_back(newEl);
				octreeFaces.push_back(newEl);
				idface++;
			}
		}

		
		modelOctree= new Octree44(octreeFaces,7); //(int)(log10f(n)+3)
		*/




		#ifdef _DEBUG
		wxLogDebug("Arbre octree construit. Nombre total de noeuds %i, élément ajoutés %i.",modelOctree->GetNodesSize(),modelOctree->GetElementLstSize());

		wxLongLong durationCalculation=wxDateTime::UNow().GetValue()-timeDebCalculation.GetValue();
		wxLogDebug("Arbre octree construit en %i ms",durationCalculation.GetValue());
		#endif

	}


	IntersectionSeeker::~IntersectionSeeker( )
	{
		delete modelOctree;
		using namespace std;
		for(size_t i=0;i<modelFaces.size();i++)
			delete modelFaces[i];
	}

	void IntersectionSeeker::GetCollisionLst(vec3 origine,vec3 destination,std::vector<t_faceIndex>& vectorToFeed )
	{

	}
	bool IntersectionSeeker::DotTriangleCollisionTest(const octreeTool::elementSize& staticCandidateIndex)
	{
		currentCollisionTest.testStat++;
		pspsElement* testElement=modelFaces[staticCandidateIndex];

		vec3 Atest(testElement->GetSommetX(0),testElement->GetSommetY(0),testElement->GetSommetZ(0));
		vec3 Btest(testElement->GetSommetX(1),testElement->GetSommetY(1),testElement->GetSommetZ(1));
		vec3 Ctest(testElement->GetSommetX(2),testElement->GetSommetY(2),testElement->GetSommetZ(2));
		float ecart,pfs,pft;
		ecart=ClosestDistanceBetweenDotAndTriangle(Atest,Btest,Ctest,currentCollisionTest.testCoord,&pfs,&pft);

		if(ecart<currentCollisionTest.bestDist || currentCollisionTest.bestDist==-1)
		{
			currentCollisionTest.bestDist=ecart;
			currentCollisionTest.BestResult=t_result(testElement->faceIndices,ecart);
		}
		return !currentCollisionTest.stopAtFirst;	//On stop la recherche si currentCollisionTest.stopAtFirst à vrai
	}
	
	bool IntersectionSeeker::DotTriangleSommetIncludeTest(const octreeTool::elementSize& staticCandidateIndex)
	{
		currentCollisionTest.testStat++;
		pspsElement* testElement=modelFaces[staticCandidateIndex];

		vec3 Atest(testElement->GetSommetX(0),testElement->GetSommetY(0),testElement->GetSommetZ(0));
		vec3 Btest(testElement->GetSommetX(1),testElement->GetSommetY(1),testElement->GetSommetZ(1));
		vec3 Ctest(testElement->GetSommetX(2),testElement->GetSommetY(2),testElement->GetSommetZ(2));

		if(!(Atest==currentCollisionTest.testCoord || Btest==currentCollisionTest.testCoord || Ctest==currentCollisionTest.testCoord))
			return true;//On continu la recherche
		
		currentCollisionTest.results.push_back(testElement->faceIndices);
		return !currentCollisionTest.stopAtFirst;	//On stop la recherche si currentCollisionTest.stopAtFirst à vrai
	}
	t_faceIndex IntersectionSeeker::GetSingleCollision( vec3 position )
	{
		if(!this)
			return t_faceIndex(-1,-1);
		using namespace octreeTool;

		currentCollisionTest.clear();
		currentCollisionTest.testCoord=position;
		currentCollisionTest.stopAtFirst=false;
		sphereElement dotTest(position,0.01f); //BoundingBox 2;2;2
		spaceElement* elementATester=&dotTest;
		modelOctree->GetDynamicCollisionCandidates(*elementATester,this,&IntersectionSeeker::DotTriangleCollisionTest);

		wxLogDebug("%i Candidats testés [%g;%g;%g]",currentCollisionTest.testStat,position.x,position.y,position.z);
		if(currentCollisionTest.bestDist!=-1 && currentCollisionTest.BestResult.dist<0.01)
		{
			//On trie selon la distance par rapport au point de recherche
			return currentCollisionTest.BestResult.faceCollison;
		}else{
			return t_faceIndex(-1,-1);
		}
	}

	void IntersectionSeeker::GetRegionFaceLst( vec3 position,std::vector<t_faceIndex>& results )
	{
		using namespace octreeTool;

		spaceElement* elementATester;
		

		dotElement dotTest(position);
		elementATester=&dotTest;




		std::vector<elementSize> candidates;
		modelOctree->GetAllCollisionCandidates(*elementATester,candidates);
		for(elementSize idcandidates=0;idcandidates<candidates.size();idcandidates++)
			results.push_back(modelFaces[candidates[idcandidates]]->faceIndices);
	}
	void IntersectionSeeker::PushBoxInfos(octreeTool::boxContainer& boxLst)
	{
		modelOctree->PushBoxInfos(boxLst);
	}
	void IntersectionSeeker::GetSommetFaceNeighboors( vec3 positionSommet, std::vector<t_faceIndex>& results )
	{

		using namespace octreeTool;

		currentCollisionTest=t_currentCollisionTest();
		currentCollisionTest.testCoord=positionSommet;
		currentCollisionTest.stopAtFirst=false;
		dotElement dotTest(positionSommet);
		spaceElement* elementATester=&dotTest;
		modelOctree->GetDynamicCollisionCandidates(*elementATester,this,&IntersectionSeeker::DotTriangleSommetIncludeTest);
		results=currentCollisionTest.results;
		//wxLogDebug("%i Candidats testés",currentCollisionTest.testStat);

	}
}