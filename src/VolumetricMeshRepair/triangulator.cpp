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

#include "triangulator.hpp"
#include "tools/intersection_tri.h"
#include <input_output/ply/rply_interface.hpp>
#include <iostream>
#include <utility>
#include <map>
#include <functional>

namespace Triangulators
{

	#ifndef MIN
		#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
	#endif
	#ifndef MAX
		#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
	#endif

	typedef std::pair<vec3,std::size_t> vertPair_t;
	typedef std::pair<std::size_t,vertPair_t> multimap_pair;
	typedef std::multimap<std::size_t,vertPair_t> verticeMap_t;

	struct MergeVerticesOperationObjects_t
	{
		MergeVerticesOperationObjects_t():verticesCount(0) {}
		std::list<ivec3> faces;
		std::size_t verticesCount;
		verticeMap_t verticeToIndexMap;
		void clear()
		{
			faces.clear();
			verticesCount=0;
			verticeToIndexMap.clear();
		}
	};

	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2);
	}

	std::size_t GetVectorHash(const vec3& vecToHash)
	{
		std::size_t h1 = std::hash<decimal>()(vecToHash.x);
		hash_combine(h1, vecToHash.y);
		hash_combine(h1, vecToHash.z);
		return h1;
	}
	inline std::size_t FindOrCreateVertices(const vec3& vert, MergeVerticesOperationObjects_t& mergeProcessData)
	{
		std::size_t hashVert(GetVectorHash(vert));
		verticeMap_t::iterator vertFound(mergeProcessData.verticeToIndexMap.find(hashVert));
		while(vertFound!=mergeProcessData.verticeToIndexMap.end() && vertFound->first==hashVert)
		{

			if(vert.barelyEqual(vertFound->second.first))
			{
				return vertFound->second.second;
			}
			vertFound++;
		}
		//Si ce point n'a pas d�j� �t� d�fini
		mergeProcessData.verticeToIndexMap.insert(multimap_pair(hashVert,vertPair_t(vert,mergeProcessData.verticesCount)));
		//mergeProcessData.verticeToIndexMap[hashVert]=vertPair_t(vert,mergeProcessData.verticesCount);
		mergeProcessData.verticesCount++;
		return mergeProcessData.verticesCount-1;
	}

	void BaseTriangulator::AddFace(const vec3& vertA,const vec3& vertB,const vec3& vertC)
	{
		//Il faut trouver les indices des sommets correspondant � ces positions
		ivec3 sommetsFace(FindOrCreateVertices(vertA,*this->mergeVerticesOperationObjects),
			FindOrCreateVertices(vertB,*this->mergeVerticesOperationObjects),
			FindOrCreateVertices(vertC,*this->mergeVerticesOperationObjects));

		this->mergeVerticesOperationObjects->faces.push_back(sommetsFace);
	}

	void BaseTriangulator::BeginFeedingFaces()
	{
		this->mergeVerticesOperationObjects->clear();
		this->vertices.clear();
		this->faces.clear();
	}

	void BaseTriangulator::FinishFeedingFaces()
	{
		//

		this->vertices.insert(this->vertices.end(),this->mergeVerticesOperationObjects->verticesCount,vec3());
		verticeMap_t::iterator endit(this->mergeVerticesOperationObjects->verticeToIndexMap.end());
		for(verticeMap_t::iterator itvertlst=this->mergeVerticesOperationObjects->verticeToIndexMap.begin();itvertlst!=endit;itvertlst++)
		{
			this->vertices[itvertlst->second.second]=itvertlst->second.first;
		}
		this->mergeVerticesOperationObjects->verticeToIndexMap.clear();

		this->faces.reserve(this->mergeVerticesOperationObjects->faces.size());
		std::list<ivec3>::iterator enditfaces(this->mergeVerticesOperationObjects->faces.end());
		for(std::list<ivec3>::iterator itface=this->mergeVerticesOperationObjects->faces.begin();itface!=enditfaces;itface++)
		{
			this->faces.push_back(*itface);
		}
		this->mergeVerticesOperationObjects->faces.clear();

	}


	BaseTriangulator::~BaseTriangulator()
	{


	}
	void ProcessEdgeCollapse(std::vector<std::list<std::size_t> >& verticesToface,const std::vector<vec3>& vertices,std::vector<ivec3>& faces,const std::size_t& from, const std::size_t& to,std::vector<bool>* destroyedFacets,std::size_t& destroyedFacetsCounter)
	{
		std::list<std::size_t>& originVerticeToFace(verticesToface[from]);
		std::list<std::size_t>& destinationVerticeToFace(verticesToface[to]);
		//////
		// Cr�ation de la liste fusionn� des faces des deux points
		std::list<std::size_t> unionFaces(originVerticeToFace);
		unionFaces.insert(unionFaces.begin(),destinationVerticeToFace.begin(),destinationVerticeToFace.end());
		unionFaces.sort();
		unionFaces.unique();

		destinationVerticeToFace.clear();


		for(std::list<std::size_t>::const_iterator itFace=unionFaces.begin(); itFace!=unionFaces.end(); itFace++)
		{
			ivec3 fromFace(faces[*itFace]);

			bool translation(false);
			bool destinationAtOneVertice(false);
			if(fromFace[0]==to || fromFace[1]==to || fromFace[2]==to)
				destinationAtOneVertice=true;
			if(fromFace[0]==from)
			{
				fromFace[0]=to;
				translation=true;
			}else if(fromFace[1]==from)
			{
				fromFace[1]=to;
				translation=true;
			}else if(fromFace[2]==from)
			{
				fromFace[2]=to;
				translation=true;
			}

			if(!(destinationAtOneVertice && translation)) //si le triangle a 3 sommets diff�rends
			{
				faces[*itFace]=fromFace;
				destinationVerticeToFace.push_back(*itFace);
			}else{
				//Cette surface a �t� supprim� il faut supprimer les liens dans ses sommets
				if(destroyedFacets)
					(*destroyedFacets)[*itFace]=true;
				destroyedFacetsCounter++;
				verticesToface[faces[*itFace].a].remove(*itFace);
				verticesToface[faces[*itFace].b].remove(*itFace);
				verticesToface[faces[*itFace].c].remove(*itFace);
			}
		}
	}
	decimal GetTriangleQuality(const vec3& A,const vec3& B,const vec3& C)
	{
		vec3 h((decimal)A.distance(B),(decimal)C.distance(B),(decimal)A.distance(C)); //Longueur des cot�s
		decimal aireTri=GetAireTriangle(A,B,C);
		//Bank, Randolph E., PLTMG: A Software Package for Solving Elliptic Partial Differential Equations, User's Guide 6.0, Society for Industrial and Applied Mathematics, Philadelphia, PA, 1990.
		//http://www.mathworks.com/access/helpdesk/help/toolbox/pde/ug/pdetriq.html
		return (aireTri*6.92820323f)/(h[0]*h[0]+h[1]*h[1]+h[2]*h[2]);
	}

	/**
	 * L'effet de cette op�ration est �valu�, si elle n'est pas souhaitable cette fonction retourne faux
	 */
	bool IsInterestingEdgeCollapse(const std::vector<vec3>& vertices,const std::vector<ivec3>& faces,const std::size_t& from,const std::list<std::size_t>& originVerticeToFace, const std::size_t& to,const std::list<std::size_t>& destinationVerticeToFace, const decimal& quality_constraint,const decimal& merge_normal_constraint)
	{
		//////
		// Une fois fusionn� les triangles restants doivent avoir la m�me normal,ainsi que de ne pas avoir de mauvais ratio plus petit cot�, plus grand cot� (afin d'�viter les faces allong�s que tetgen n'aime pas)

		for(std::list<std::size_t>::const_iterator itFace=originVerticeToFace.begin(); itFace!=originVerticeToFace.end(); itFace++)
		{
			const ivec3& oldfromFace(faces[*itFace]);
			ivec3 fromFace(faces[*itFace]);
			bool translation(false);
			bool destinationAtOneVertice(false);
			if(fromFace[0]==to || fromFace[1]==to || fromFace[2]==to)
				destinationAtOneVertice=true;
			if(fromFace[0]==from)
			{
				fromFace[0]=to;
				translation=true;
			}else if(fromFace[1]==from)
			{
				fromFace[1]=to;
				translation=true;
			}else if(fromFace[2]==from)
			{
				fromFace[2]=to;
				translation=true;
			}

			if(!(destinationAtOneVertice && translation)) //si le triangle a 3 sommets diff�rends
			{
				const vec3 A(vertices[fromFace.a]),B(vertices[fromFace.b]),C(vertices[fromFace.c]);
				vec3 oldfaceNormal=core_mathlib::FaceNormal(vertices[oldfromFace.a],vertices[oldfromFace.b],vertices[oldfromFace.c]);
				vec3 newfaceNormal=core_mathlib::FaceNormal(A,B,C);
				if(oldfaceNormal.dot(newfaceNormal)<merge_normal_constraint)
					return false;
				//Evaluation de la qualit� du nouveau triangle
				decimal quality=GetTriangleQuality(A,B,C);

				if(quality<quality_constraint)
				{
					decimal oldQuality=GetTriangleQuality(vertices[oldfromFace.a],vertices[oldfromFace.b],vertices[oldfromFace.c]);
					if(quality<oldQuality)
						return false;
				}
			}
		}

		return true;
	}

	void MakeEdgeCollapseListByQuality(const std::vector<ivec3>& faces,const std::list<std::size_t>& originVerticeToFace,std::size_t& exception,std::list<std::size_t>& toList)
	{
		for(std::list<std::size_t>::const_iterator itFace=originVerticeToFace.begin(); itFace!=originVerticeToFace.end(); itFace++)
		{
			const ivec3& fromFace(faces[*itFace]);
			if(fromFace.a!=exception)
				toList.push_back(fromFace.a);
			if(fromFace.b!=exception)
				toList.push_back(fromFace.b);
			if(fromFace.c!=exception)
				toList.push_back(fromFace.c);
		}
		toList.sort();
		toList.unique();
	}
	void BaseTriangulator::PolygonReduction(progressOperation& mainProcess)
	{
		//Construction des structures d'optimisation de recherche des segments pouvant �tre r�duit en un point
		std::size_t verticeCount(this->vertices.size());
		std::size_t destroyedFacetsCounter(0);
		std::vector<std::list<std::size_t> > verticeToFace(verticeCount);
		std::vector<bool> destroyedFacets(this->faces.size(),false);
		std::size_t idface(0);
		std::list<std::size_t> verticesAlive;
		for(std::size_t i=0;i<verticeCount;i++)
			verticesAlive.push_back(i);
		for(std::vector<ivec3>::iterator itface=this->faces.begin();itface!=this->faces.end();itface++)
		{
			verticeToFace[itface->a].push_back(idface);
			verticeToFace[itface->b].push_back(idface);
			verticeToFace[itface->c].push_back(idface);
			//outModel.modelFaces.push_back(*itface);
			idface++;
		}
		//////////////////////////////////////////////////////////////////////////////
		// Boucle principale de fusion des segments
		bool continueWithCollapsing(true);
		while(continueWithCollapsing)
		{
			if(verbose)
				std::cout<<"Face count :"<<this->faces.size()-destroyedFacetsCounter<<std::endl;
			continueWithCollapsing=false;
			for(std::list<std::size_t>::iterator itvert=verticesAlive.begin();itvert!=verticesAlive.end();)
			{
				std::size_t from=*itvert;

				const std::list<std::size_t>& originVerticeToFace(verticeToFace[from]);

				// Recherche d'un sommet voisin au sommet from
				std::list<std::size_t> toList;
				for(std::list<std::size_t>::const_iterator itFace=originVerticeToFace.begin(); itFace!=originVerticeToFace.end(); itFace++)
				{
					const ivec3& fromFace(faces[*itFace]);
					if(fromFace.a!=from)
						toList.push_back(fromFace.a);
					if(fromFace.b!=from)
						toList.push_back(fromFace.b);
					if(fromFace.c!=from)
						toList.push_back(fromFace.c);
				}
				toList.sort();
				toList.unique();
				bool interstingEdgeFound(false);
				std::size_t to;
				for(std::list<std::size_t>::iterator toIt=toList.begin();toIt!=toList.end();toIt++)
				{
					to=*toIt;
					const std::list<std::size_t>& destinationVerticeToFace(verticeToFace[to]);
					if(IsInterestingEdgeCollapse(vertices,faces,from,originVerticeToFace,to,destinationVerticeToFace,quality_constraint,merge_normal_constraint))
					{
						interstingEdgeFound=true;
						break;
					}
				}
				if(interstingEdgeFound)
				{
					ProcessEdgeCollapse(verticeToFace,vertices,faces,from,to,&destroyedFacets,destroyedFacetsCounter);
					continueWithCollapsing=true;
					std::list<std::size_t>::iterator itVertToDestroy(itvert);
					itvert++;
					verticesAlive.erase(itVertToDestroy);
				}else{
					itvert++;
				}
			}
		}

		///////////////////////
		// Reconstruction des listes de faces
		verticeToFace.clear();
		verticesAlive.clear();
		std::vector<vec3> old_vertices(vertices);
		std::vector<ivec3> old_faces(faces);
		BeginFeedingFaces();
		idface=0;
		for(std::vector<ivec3>::iterator itface=old_faces.begin();itface!=old_faces.end();itface++)
		{
			if(!destroyedFacets[idface])
				AddFace(old_vertices[itface->a],old_vertices[itface->b],old_vertices[itface->c]);
			idface++;
		}
		FinishFeedingFaces();
	}

	void BaseTriangulator::InvertFacesNormal()
	{
		for(std::vector<ivec3>::iterator itface=this->faces.begin();itface!=this->faces.end();itface++)
			*itface=ivec3(itface->c,itface->b,itface->a);
	}

	bool BaseTriangulator::SaveAs(const std::string& filename)
	{
		using namespace formatRPLY;
		t_model outModel;
		for(auto & vertice : this->vertices)
		{
			outModel.modelVertices.push_back(vertice);
		}
		for(auto & face : this->faces)
		{
			outModel.modelFaces.emplace_back(face);
		}
		return CPly::ExportPly(outModel, filename);
	}

	BaseTriangulator::BaseTriangulator()
		: quality_constraint(static_cast<decimal>(0.1)), merge_normal_constraint(static_cast<decimal>(0.999)),
		  verbose(false),
		  mergeVerticesOperationObjects(PTR<MergeVerticesOperationObjects_t>(new MergeVerticesOperationObjects_t)) {
	}

	void GetTriNeigh(const std::vector<std::list<std::size_t> > &verticeToFace, const std::vector<ivec3> &faces,
	                 std::list<std::size_t> &tri_lst, const std::size_t &startVertice, unsigned short deep) {
		const auto endIterator(verticeToFace[startVertice].end());
		for (auto faceIterator = verticeToFace[startVertice].begin(); faceIterator != endIterator; ++faceIterator) {
			tri_lst.push_back(*faceIterator);
			if (deep > 1) {
				GetTriNeigh(verticeToFace, faces, tri_lst, faces[*faceIterator].a, deep - 1);
				GetTriNeigh(verticeToFace, faces, tri_lst, faces[*faceIterator].b, deep - 1);
				GetTriNeigh(verticeToFace, faces, tri_lst, faces[*faceIterator].c, deep - 1);
			}
		}
	}

	bool InVec(const int& iTest,const ivec3& Vtest)
	{
		if(iTest==Vtest.a || iTest==Vtest.b ||iTest==Vtest.c)
			return true;
		else
			return false;
	}

	bool BaseTriangulator::TestCollision( std::list<std::size_t>& facesToTest, const ivec3& faceTest, const std::size_t& exceptVertice )
	{
		const vec3& vertA(vertices[faceTest.a]),vertB(vertices[faceTest.b]),vertC(vertices[faceTest.c]);
		for(std::list<std::size_t>::iterator itTestFace=facesToTest.begin();itTestFace!=facesToTest.end();itTestFace++)
		{

			const ivec3& curFace(faces[*itTestFace]);
			if(curFace.a!=exceptVertice && curFace.b!=exceptVertice && curFace.c!=exceptVertice)
			{
				short sharedVertices(0);
				if(InVec(curFace.a,faceTest))
					sharedVertices++;
				if(InVec(curFace.b,faceTest))
					sharedVertices++;
				if(InVec(curFace.c,faceTest))
					sharedVertices++;
				if(sharedVertices<2)
				{
					const vec3& faceA(vertices[curFace.a]),faceB(vertices[curFace.b]),faceC(vertices[curFace.c]);
					if(CollisionAlgo::IntersectionTriTri(faceA,faceB,faceC,
						vertA,vertB,vertC))
					{
						#ifdef SAVE_FOUND_COLLISION
						vec3 faceNormal(FaceNormal(faceA,faceB,faceC));
						stlFile<<"  facet normal "<<faceNormal.x<<" "<<faceNormal.y<<" "<<faceNormal.z<<std::endl;
						stlFile<<"    outer loop"<<std::endl;
						stlFile<<"      vertex  "<<faceA.x<<" "<<faceA.y<<" "<<faceA.z<<std::endl;
						stlFile<<"      vertex  "<<faceB.x<<" "<<faceB.y<<" "<<faceB.z<<std::endl;
						stlFile<<"      vertex  "<<faceC.x<<" "<<faceC.y<<" "<<faceC.z<<std::endl;
						stlFile<<"    end loop"<<std::endl;
						stlFile<<"  end facet"<<std::endl;
						faceNormal=FaceNormal(vertA,vertB,vertC);
						stlFile<<"  facet normal "<<faceNormal.x<<" "<<faceNormal.y<<" "<<faceNormal.z<<std::endl;
						stlFile<<"    outer loop"<<std::endl;
						stlFile<<"      vertex  "<<vertA.x<<" "<<vertA.y<<" "<<vertA.z<<std::endl;
						stlFile<<"      vertex  "<<vertB.x<<" "<<vertB.y<<" "<<vertB.z<<std::endl;
						stlFile<<"      vertex  "<<vertC.x<<" "<<vertC.y<<" "<<vertC.z<<std::endl;
						stlFile<<"    end loop"<<std::endl;
						stlFile<<"  end facet"<<std::endl;
						#endif
						return true;
					}
				}
			}
		}
		return false;
	}

	bool BaseTriangulator::RefineVertice(vec3* itvert,const std::size_t& vertId,std::vector<std::list<std::size_t> >& verticeToFace,AdaptativeMeshGuide& refinementGuide,const decimal& cellSize)
	{
		decimal maxDist(cellSize*(decimal)5.); //sqrt(2^2+2^2+2^2) , correspond � la distance maximal possible d'une paroie par rapport au maillage

		const decimal aproximation=cellSize*(decimal).15;
		ClosestCandidates_t ClosestCandidates;
		//Met en place la liste des faces voisines
		std::list<std::size_t> tri_lst;
		//Todo recherche des triangles proches susceptible d'etre en collision (pas obligatoirement les voisins)
		GetTriNeigh(verticeToFace,faces,tri_lst,vertId,3);
		tri_lst.sort();
		tri_lst.unique();
		//Recherche les points les plus proches du noeud
		bool translationFound(false);
		if(refinementGuide.GetClosestPoint(*itvert,maxDist,ClosestCandidates))
		{
			refinementGuide.SortByDistance(ClosestCandidates);
			vec3 oldPosition(*itvert);
			for(ClosestCandidates_t::iterator itRes=ClosestCandidates.begin();itRes!=ClosestCandidates.end();itRes++)
			{
				const vec3& closestPoint(itRes->second);
				vec3 translationVec=closestPoint-*itvert;
				translationVec.normalize();
				*itvert=closestPoint-(translationVec*aproximation);

				///////////////////
				// Test de validit� de la translation
				bool isCollision(false);
				std::list<std::size_t>::iterator endit(verticeToFace[vertId].end());
				for(std::list<std::size_t>::iterator itface=verticeToFace[vertId].begin();itface!=endit;itface++)
				{
					const ivec3& curFace(faces[*itface]);
					if(TestCollision(tri_lst, curFace, vertId ))
					{
						isCollision=true;
						break;
					}
				}
				if(!isCollision)
				{
					translationFound=true;
					break;
				}else{
					*itvert=oldPosition; //cette translation a occasionn� une collision avec le maillage, dans ce cas on revient � la position d'origine
				}
			}
		}
		return translationFound;
		/*
		if(!translationFound)
		{
			//Supprimer le noeud (raccorder les faces)
			// si la translation a �t� rejet�e
			//std::list<std::size_t> toList;
			//MakeEdgeCollapseListByQuality(faces,verticeToFace[vertId],vertId,toList);
			//ProcessEdgeCollapse(verticeToFace,vertices,faces,vertId,*toList.begin(),NULL,destroyedFacetsCounter);
		}
		*/

	}
	void BaseTriangulator::RefineTriangulationGrid(AdaptativeMeshGuide& refinementGuide,const decimal& cellSize, progressOperation& mainProcess,progressionInfo& progressInfo)
	{

		/////DEBUG///////
		#ifdef SAVE_FOUND_COLLISION
		stlFile.open("collisionFaces.stl");
		stlFile<<"solid vcg"<<std::endl;
		#endif
		//////////
		std::size_t destroyedFacetsCounter(0);
		std::size_t verticeCount(this->vertices.size());
		std::vector<std::list<std::size_t> > verticeToFace(verticeCount);
		std::size_t idface(0);
		for(std::vector<ivec3>::iterator itface=this->faces.begin();itface!=this->faces.end();itface++)
		{
			verticeToFace[itface->a].push_back(idface);
			verticeToFace[itface->b].push_back(idface);
			verticeToFace[itface->c].push_back(idface);
			idface++;
		}
		vec3 translationVec;
		vec3 faceNormal;

		std::size_t vertId(0);

		progressOperation beginLoopVertices(&mainProcess,this->vertices.size());

		std::vector<vec3> originalPosition(this->vertices);

		//////////////
		// Pour chaque noeud du maillage
		PTR<std::list<std::size_t> > collisionVertices(new std::list<std::size_t>());
		for(std::vector<vec3>::iterator itvert=this->vertices.begin();itvert!=this->vertices.end();itvert++)
		{
			//Increment % calcul
			progressOperation itProgressVertice(&beginLoopVertices);
			progressInfo.OutputCurrentProgression();

			if(!RefineVertice(&(*itvert),vertId,verticeToFace,refinementGuide,cellSize))
				collisionVertices->push_back(vertId);
			vertId++;
		}

		bool collisionResolutionGetBetter(true);
		PTR<std::list<std::size_t> > nextVerticeTranslations(new std::list<std::size_t>());
		PTR<std::list<std::size_t> > curVerticeTranslation=collisionVertices;
		while(collisionResolutionGetBetter)
		{
			std::cout<<"Solve collision with "<<curVerticeTranslation->size()<<" vertices."<<std::endl;
			collisionResolutionGetBetter=false;
			for(std::list<std::size_t>::iterator itvert=curVerticeTranslation->begin();itvert!=curVerticeTranslation->end();itvert++)
			{
				if(RefineVertice(&(this->vertices[*itvert]),*itvert,verticeToFace,refinementGuide,cellSize))
				{
					collisionResolutionGetBetter=true;
				}else{
					nextVerticeTranslations->push_back(*itvert);
				}
			}
			curVerticeTranslation=nextVerticeTranslations;
			nextVerticeTranslations=PTR<std::list<std::size_t> >(new std::list<std::size_t>());
		}


		//Todo supprimer les noeuds restants dans nextVerticeTranslations
		#ifdef SAVE_FOUND_COLLISION
		stlFile<<"endsolid vcg"<<std::endl;
		stlFile.close();
		#endif
		//SaveXYZCollision();
	}
	void BaseTriangulator::SaveXYZCollision()
	{
		std::ofstream xyzFile;
		xyzFile.open("collision.XYZ",std::ios_base::app);
		if(!xyzFile)
			return;

		for(std::list<vec3>::iterator itc=collision_Debug.begin();itc!=collision_Debug.end();itc++)
		{
			xyzFile<<(*itc).x<<" "<<(*itc).y<<" "<<(*itc).z<<std::endl;
		}

		xyzFile.close();

	}

}
