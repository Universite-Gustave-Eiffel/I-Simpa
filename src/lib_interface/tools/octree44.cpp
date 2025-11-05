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
 * Code source �crit dans le cadre du projet OPALHA contrat ADEME n�06.04.C.0070 (2007)
 */
#include "octree44.hpp"
#include <string.h>

namespace octreeTool
{
    /**
	 * Classe de test pour les collisions
	 */
	class AllCandidatesReceiver
	{
	public:
		AllCandidatesReceiver() {}
		bool ClientMethodReceiver(const octreeTool::elementSize& staticCandidateIndex)
		{
			candidatesLst.push_back(staticCandidateIndex);
			return true;
		}
		void fillVector(std::vector<octreeTool::elementSize>& vectorToFeed)
		{
			candidatesLst.sort();
			candidatesLst.unique();
			vectorToFeed.reserve(candidatesLst.size());
			for(std::list<octreeTool::elementSize>::iterator itcand=candidatesLst.begin();itcand!=candidatesLst.end();itcand++)
			{
				vectorToFeed.push_back(*itcand);
			}
		}
	protected:
		std::list<octreeTool::elementSize> candidatesLst;
	};


	////////////////////////////////////////////////////////////////////////////////////////////////
	//// Impl�mentation de t_Node
	////////////////////////////////////////////////////////////////////////////////////////////////
      
	////////////////////////////////////////////////////////////////////////////////////////////////
	//// Impl�mentation de Octree44
	////////////////////////////////////////////////////////////////////////////////////////////////

	Octree44::Octree44(spaceElementContainer& _staticElements,const depthSize& maxDepth,const depthSize& minDepth )
	{
		const elementSize& _sizeOf=_staticElements.size();
		sizeOfStaticElements=_sizeOf;
		/////////////////////////////////////////////////////////////////////////////////////
		//Calcul du min max permettant de d�duire les positions et tailles du noeud principal

		if(sizeOfStaticElements>0)
			_staticElements[0]->GetMinMax(boundingBoxMin,boundingBoxMax);
		for(elementSize idEl=1;idEl<sizeOfStaticElements;idEl++)
		{
			coordPrecision elementBoxMin[3];
			coordPrecision elementBoxMax[3];
			_staticElements[idEl]->GetMinMax(elementBoxMin,elementBoxMax);
			SetMinMaxVec(elementBoxMin,boundingBoxMin,boundingBoxMax);
			SetMinMaxVec(elementBoxMax,boundingBoxMin,boundingBoxMax);
		}

		InitFromBoundingBox();

		/////////////////////////////////////////////////////////////////////////////////////
		// Construction du noeud principal et des noeuds sous-jacents
		rootNode.Build(_staticElements,NULL,maxDepth,minDepth);

	}

	void Octree44::InitFromBoundingBox()
	{
		/////////////////////////////////////////////////////////////////////////////////////
		//Prend la taille maximal de tout les cot�s afin de d�terminer la largeur des boites
		coordPrecision halfSizeMax=(boundingBoxMax[0]-boundingBoxMin[0])/2.;
		halfSizeMax=MAX(halfSizeMax,(boundingBoxMax[1]-boundingBoxMin[1])/2.);
		halfSizeMax=MAX(halfSizeMax,(boundingBoxMax[2]-boundingBoxMin[2])/2.);


		//halfSizeDiff On ajoute 5% sur les limites
		//Ceci afin d'empecher des faces d'etres plan sur les cot�s des cubes (tr�s utile sur l'importation de mod�les plan )

		coordPrecision halfSizeDiff=halfSizeMax*.05;
		coordPrecision halfSizeDiffUnaxis=halfSizeMax*.02;
		halfSizeMax+=halfSizeDiff;
		boundingBoxMin[0]-=halfSizeDiff;
		boundingBoxMin[1]-=halfSizeDiff;
		boundingBoxMin[2]-=halfSizeDiff;
		boundingBoxMax[0]+=halfSizeDiff;
		boundingBoxMax[1]+=halfSizeDiff;
		boundingBoxMax[2]+=halfSizeDiff;

		//halfSizeDiffUnaxis Change l'alignement de 2%
		//Ceci afin d'empecher des faces d'etres plan sur les cot�s des cubes (tr�s utile sur l'importation de mod�les plan )


		rootNode.SetHalfSize(halfSizeMax);
		/////////////////////////////////////////////////////////////////////////////////////
		//Calcul des positions et tailles du noeud principal

		rootNode.SetPositionCenterX((boundingBoxMax[0]+boundingBoxMin[0])/2.-halfSizeDiffUnaxis);
		rootNode.SetPositionCenterY((boundingBoxMax[1]+boundingBoxMin[1])/2.-halfSizeDiffUnaxis);
		rootNode.SetPositionCenterZ((boundingBoxMax[2]+boundingBoxMin[2])/2.-halfSizeDiffUnaxis);
	}

	Octree44::Octree44( const coordPrecision _boundingBoxMin[3], const coordPrecision _boundingBoxMax[3])
		: sizeOfStaticElements(0)
	{
		memcpy(boundingBoxMin,_boundingBoxMin,sizeof(coordPrecision)*3);
		memcpy(boundingBoxMax,_boundingBoxMax,sizeof(coordPrecision)*3);
		InitFromBoundingBox();
	}

	/*
	template<class CallClass> void Octree44::GetDynamicCollisionCandidates( spaceElement& testElement, CALLER_DEF, CALLBACK_METHOD  )
	{
		rootNode.DynamicTestElement<CallClass>(testElement,caller,sendCollisionCandidatesMethod);
	}
	*/

	void Octree44::GetAllCollisionCandidates( spaceElement& testElement, std::vector<elementSize>& candidates )
	{
	    //typedef AllCandidatesReceiver CallClass;
		//CALLBACK_METHOD=&AllCandidatesReceiver::ClientMethodReceiver;
		AllCandidatesReceiver instanceReceive;
		GetDynamicCollisionCandidates<AllCandidatesReceiver>(testElement,&instanceReceive,&AllCandidatesReceiver::ClientMethodReceiver);
		instanceReceive.fillVector(candidates);
	}

	bool Octree44::InsideOctree(spaceElement& testElement)
	{
		return rootNode.IsOverlap(testElement);
	}

        void Octree44::PushNewSpaceElement(spaceElement* newElement,const elementSize& elementIndex, const depthSize& maxDepth)
	{
		sizeOfStaticElements++;
		rootNode.Append(newElement,elementIndex,maxDepth);
	}

	void Octree44::RemoveSpaceElementByElement(spaceElement* oldElement,const elementSize& elementIndex)
	{
		rootNode.Remove(oldElement,elementIndex);
	}
	void Octree44::RemoveSpaceElementByElementIndex(const elementSize& elementIndex)
	{
		rootNode.Remove(elementIndex);
	}

	elementSize Octree44::GetElementLstSize()
	{
		elementSize valRet=0;
		rootNode.GetTotalElementCount(valRet);
		return valRet;
	}
	elementSize Octree44::GetNodesSize()
	{
		elementSize valRet=0;
		rootNode.GetTotalNodes(valRet);
		return valRet;
	}

	void Octree44::PushBoxInfos(boxContainer& boxLst)
	{
		int nbBoxes=GetNodesSize();
		if(nbBoxes>0)
		{
			boxLst.reserve(nbBoxes);
			rootNode.PushBoxInfos(boxLst);
		}
	}
}
