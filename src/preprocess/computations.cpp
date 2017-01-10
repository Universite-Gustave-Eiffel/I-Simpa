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

#include "computations.hpp"
#include <iostream>
#include "tools/intersection_algo.h"
#include "tools/intersection_tri.h"
#include <tools/octree44_dotElement.hpp>
#include <map>
#include <utility>

#ifdef _DEBUG
	struct t_duplicata
	{
		t_duplicata(unsigned int _firstOccurance,unsigned int _duplicata): firstOccurance(_firstOccurance),duplicata(_duplicata) {}
		unsigned int firstOccurance;
		unsigned int duplicata;
	};
#endif

#define equal_arr( a, arrB )  (a==arrB[0] || a==arrB[1] || a==arrB[2])
/*
bool SeperatedTri( const ivec3& firstTri ,const ivec3& secondTri )
{
	if(equal_arr(firstTri.a,secondTri) ||  equal_arr(firstTri.b,secondTri) || equal_arr(firstTri.c,secondTri))
		return false;
	else
		return true;
}
*/

bool SeperatedTri( const ivec3& firstTri ,const ivec3& secondTri,int* commonVertices )
{
	*commonVertices=0;
	if(equal_arr(firstTri.a,secondTri))
		(*commonVertices)++;
	if(equal_arr(firstTri.b,secondTri))
		(*commonVertices)++;
	if(equal_arr(firstTri.c,secondTri))
		(*commonVertices)++;
	if(	*commonVertices>=2)
		return false;
	else
		return true;
}

int GetIndiceByCoord( const dvec3& a,const dvec3& b,const dvec3& c, const dvec3& testCoord)
{
	if(a.barelyEqual(testCoord))
		return 0;
	else if(b.barelyEqual(testCoord))
		return 1;
	else if(c.barelyEqual(testCoord))
		return 2;
	else
		return -1;
}
bool IsDotInTriangle( const dvec3& a,const dvec3& b,const dvec3& c, const dvec3& testCoord)
{
	if(GetIndiceByCoord(a,b,c,testCoord)==-1)
	{
		return DotIsInVertex(testCoord,a,b,c);
	}else{
		return true;
	}
}
void genprintVec(dvec3 inf)
{
	using namespace std;
	std::string retStr;
	cout<<"["<<inf.x<<";"<<inf.y<<";"<<inf.z<<"]";
}
void genprintVec(ivec3 inf)
{
	using namespace std;
	std::string retStr;
	cout<<"["<<inf.a+1<<";"<<inf.b+1<<";"<<inf.c+1<<"]";
}


class OctreeDotTest
{
	public:
		OctreeDotTest(octreeTool::elementSize _exception,formatPOLY::t_model* _lstFaces):exception(_exception),lstFaces(_lstFaces){};
	bool DotDotCorrespondanceTest(const octreeTool::elementSize& staticCandidateIndex)
	{
		//Ajouter chaque sommet en indice
		ivec3 sommets=lstFaces->modelFaces[staticCandidateIndex].indicesSommets;
		for(short i=0;i<3;i++)
		{
			if(sommets[i]!=exception)
				results.push_back(sommets[i]);
		}
		return true;
	}
	std::vector<octreeTool::elementSize> results;
	octreeTool::elementSize exception;
	formatPOLY::t_model* lstFaces;
};
meshOperation::meshOperation(const char* polyFileName)
: stat_NbFaceSplitting(0),
  stat_DestroyedFaces(0),
  stat_NbPointsMerged(0),
  modelOctree(NULL)
{
	//Chargement du fichier
	using namespace formatPOLY;

	CPoly polyImporter;

	if(polyImporter.ImportPOLY(modelData,polyFileName))
	{
		modelData.modelFaces.reserve(modelData.modelFaces.size()*2);
		ReBuildOctree();
	}
	
}
void meshOperation::ReBuildOctree()
{
	delete modelOctree;
	lstFaces.clear();
	using namespace octreeTool;
	unsigned int sizeOfFace=modelData.modelFaces.size();
	lstFaces.insert(lstFaces.begin(),sizeOfFace,NULL);
	ivec3 sommets;
	for(elementSize idface=0;idface<sizeOfFace;idface++)
	{
		sommets=modelData.modelFaces[idface].indicesSommets;
		lstFaces[idface]=new triangleElement(dvec3_to_vec3(modelData.modelVertices[sommets.a]), dvec3_to_vec3(modelData.modelVertices[sommets.b]), dvec3_to_vec3(modelData.modelVertices[sommets.c]));
	}
	modelOctree = new octreeTool::Octree44(lstFaces,6);
}
meshOperation::~meshOperation()
{
	using namespace octreeTool;
	if(modelOctree)
	{
		for(elementSize idface=0;idface<lstFaces.size();idface++)
		{
			delete lstFaces[idface];
		}
	}
	delete modelOctree;
}


bool meshOperation::IsOk()
{
	return modelOctree!=NULL;
}
int meshOperation::FindIndexWithPosition( const dvec3& position )
{

	unsigned int sizeOf=modelData.modelVertices.size();

	for(int idVertice=sizeOf-1;idVertice>=0;idVertice--)
	{
		if(modelData.modelVertices[idVertice].barelyEqual(position))
			return idVertice;
	}
	return -1;
}
int meshOperation::FindFaceWithSommet( const ivec3& sommetsSearch )
{

	unsigned int sizeOf=modelData.modelFaces.size();

	for(int idFace=sizeOf-1;idFace>=0;idFace--)
	{
		if(modelData.modelFaces[idFace].indicesSommets==sommetsSearch)
			return idFace;
	}
	return -1;
}
void meshOperation::TransferUserFaceToGlobalFaces()
{
	unsigned int sizeOfUserFace=modelData.userDefinedFaces.size();
	for(int idface=0;idface<sizeOfUserFace;idface++)
	{
		PushNewFace(modelData.userDefinedFaces[idface]);
	}
	modelData.userDefinedFaces.clear();
}

bool meshOperation::MeshDestroyCoplanarFaces()
{	using namespace formatPOLY;
	int result;
	unsigned int sizeOfUserFace=modelData.userDefinedFaces.size();
	unsigned int sizeOfFace=modelData.modelFaces.size();

	for(int idface=0;idface<sizeOfUserFace;idface++)
	{
		ivec3* faceSommets=&modelData.userDefinedFaces[idface].indicesSommets;
		dvec3* faceA=&modelData.modelVertices[faceSommets->a];
		dvec3* faceB=&modelData.modelVertices[faceSommets->b];
		dvec3* faceC=&modelData.modelVertices[faceSommets->c];
		int coplanar=0;
		dvec3 isecA;
		dvec3 isecB;
		dvec3 face_1g=GetGTriangle(*faceA,*faceB,*faceC);
		for(int idfaceTest=0;idfaceTest<sizeOfFace;idfaceTest++)
		{
			result=0;
			coplanar=0;
			ivec3* faceTestSommets=&modelData.modelFaces[idfaceTest].indicesSommets;
			dvec3* faceA_Test=&modelData.modelVertices[faceTestSommets->a];
			dvec3* faceB_Test=&modelData.modelVertices[faceTestSommets->b];
			dvec3* faceC_Test=&modelData.modelVertices[faceTestSommets->c];
			/*
			result=tri_tri_intersect_with_isectline(*faceA,*faceB,*faceC,
					modelData.modelVertices[faceTestSommets->a],modelData.modelVertices[faceTestSommets->b],modelData.modelVertices[faceTestSommets->c],
					&coplanar,
					isecA
					,isecB);
			*/
			dvec3 face_2g=GetGTriangle(*faceA_Test,*faceB_Test,*faceC_Test);
			float ecart1;
			float ecart2;
			bool face1_in_face2=DotIsInVertex(face_1g,*faceA_Test,*faceB_Test,*faceC_Test,&ecart1);
			bool face2_in_face1=DotIsInVertex(face_2g,*faceA,*faceB,*faceC,&ecart2);
			face1_in_face2=ecart1<BARELY_EPSILON;
			face2_in_face1=ecart1<BARELY_EPSILON;
			if(face1_in_face2 || face2_in_face1)
			{	
				result=1;
				coplanar=1;
			}
			//Test des points de collision, si ces points de collisions sont simplement les points des sommets en commun alors on passe aux prochains triangles
			if(result)
			{
				if(coplanar)
				{
					#ifdef _DEBUG
						std::cout<<"Superposition de triangles, suppression du plus petit "<<std::endl;
					#endif
					stat_DestroyedFaces++;
					modelData.userDefinedFaces.erase(modelData.userDefinedFaces.begin()+idface);
					return true;
				}
			}
		}
	}
	return false;
}

void meshOperation::DestroyNoAreaFaces()
{
	using namespace formatPOLY;
	std::vector<t_face> newTabFace;
	int sizeOfFace=modelData.modelFaces.size();
	newTabFace.reserve(sizeOfFace);
	for(int idface=0;idface<sizeOfFace;idface++)
	{
		ivec3* faceSommets=&modelData.modelFaces[idface].indicesSommets;
		dvec3* faceA=&modelData.modelVertices[faceSommets->a];
		dvec3* faceB=&modelData.modelVertices[faceSommets->b];
		dvec3* faceC=&modelData.modelVertices[faceSommets->c];
		float aireTri=GetAireTriangle(*faceA,*faceB,*faceC);
		if(aireTri>BARELY_EPSILON)
			newTabFace.push_back(modelData.modelFaces[idface]);
		else
			stat_DestroyedFaces++;
	}
	modelData.modelFaces=newTabFace;
	ReBuildOctree();
}
bool meshOperation::MeshDestroyIntersectingTriangles()
{
	using namespace formatPOLY;
	int result;
	unsigned int sizeOfFace=modelData.modelFaces.size();

	for(int idface=0;idface<sizeOfFace;idface++)
	{
		ivec3* faceSommets=&modelData.modelFaces[idface].indicesSommets;
		dvec3* faceA=&modelData.modelVertices[faceSommets->a];
		dvec3* faceB=&modelData.modelVertices[faceSommets->b];
		dvec3* faceC=&modelData.modelVertices[faceSommets->c];
		int coplanar=0;
		dvec3 isecA;
		dvec3 isecB;
		for(int idfaceTest=idface+1;idfaceTest<sizeOfFace;idfaceTest++)
		{
			ivec3* faceTestSommets=&modelData.modelFaces[idfaceTest].indicesSommets;
			//Si le deux faces n'ont pas de sommets en commun
			int common_vertices=0;
			dvec3* faceA_Test=&modelData.modelVertices[faceTestSommets->a];
			dvec3* faceB_Test=&modelData.modelVertices[faceTestSommets->b];
			dvec3* faceC_Test=&modelData.modelVertices[faceTestSommets->c];
			//modelData.modelFaces[idface].indicesSommets[sommet]=indexCorrespondance[modelData.modelFaces[idface].indicesSommets[sommet]];
			result=tri_tri_intersect_with_isectline(*faceA,*faceB,*faceC,
					*faceA_Test,*faceB_Test,*faceC_Test,
					&coplanar,
					isecA
					,isecB);
			//Test des points de collision, si ces points de collisions sont simplement les points des sommets en commun alors on passe aux prochains triangles
			if(result)
			{

				if(!coplanar)
				{
					bool collision_in_face2= IsDotInTriangle(*faceA_Test,*faceB_Test,*faceC_Test,isecA);
					bool collision_in_face1= IsDotInTriangle(*faceA,*faceB,*faceC,isecA);

					if(collision_in_face2 && collision_in_face1)
					{
						if(!OnCollisionDetectedDestroyIt(modelData.modelFaces[idface],modelData.modelFaces[idfaceTest],isecA))
						{
							if(!isecA.barelyEqual(isecB))
							{
								collision_in_face2=IsDotInTriangle(*faceA_Test,*faceB_Test,*faceC_Test,isecB);
								collision_in_face1= IsDotInTriangle(*faceA,*faceB,*faceC,isecB);
								if(collision_in_face2 && collision_in_face1)
								{
									if(OnCollisionDetectedDestroyIt(modelData.modelFaces[idface],modelData.modelFaces[idfaceTest],isecB))
									{
										stat_DestroyedFaces++;
										std::cout<<"Supprime face n°"<<idfaceTest<<std::endl;
										modelData.modelFaces.erase(modelData.modelFaces.begin()+idfaceTest);
										return true;
									}
								}
							}
						}else{
							stat_DestroyedFaces++;
							std::cout<<"Supprime face n°"<<idface<<std::endl;
							modelData.modelFaces.erase(modelData.modelFaces.begin()+idface);
							return true;
						}
					}	
				}
			}
		}
	}
	return false;
}
bool meshOperation::coplanarIntersection(int idface,int idfaceTest)
{
	ivec3* faceSommets=&modelData.modelFaces[idface].indicesSommets;
	ivec3* faceTestSommets=&modelData.modelFaces[idfaceTest].indicesSommets;
	//Process only not overlaping coplanar face
	//Get coplanar intersection point other than common vertices
	//                    face |faceTest
	short commonVertices[]={-1,-1,-1};
	short commonVerticesCount=0;
	for(short i=0;i<3;i++)
	{
		for(short j=0;j<3;j++)
		{
			if(faceSommets->i[i]==faceTestSommets->i[j])
			{
				commonVertices[i]=j;
				commonVerticesCount++;
				break;
			}
		}
		if(commonVerticesCount==2)
			break;
	}
	//dvec3::closestPointOnSegment
	if(commonVerticesCount!=2)
	{
		for(short idcommonvert=0;idcommonvert<3;idcommonvert++)
		{
			dvec3& faceVert=modelData.modelVertices.at(faceSommets->i[idcommonvert]);
			if(commonVertices[idcommonvert]==-1)
			{
				//On fait une projection sur chaque segment de l'autre triangle
				//Si le point projeté correspond au sommet alors il s'agit du point d'intersection
				for(short side=0;side<3;side++)
				{
					ivec2 side_vertices=faceTestSommets->sideVertices(side);
					dvec3 closest=faceVert.closestPointOnSegment(modelData.modelVertices.at(side_vertices.a),modelData.modelVertices.at(side_vertices.b));
					if(closest.barelyEqual(faceVert))
					{
						//std::cout<<"Intersection At ["<<closest.x<<","<<closest.y<<","<<closest.z<<"]"<<" (#"<<idface<<"& #"<<idfaceTest<<") "<<commonVerticesCount<<std::endl;
						return OnCollisionDetectedSplitIt(modelData.modelFaces[idface],modelData.modelFaces[idfaceTest],closest);
					}
				}
			}
		}
		
	}
	return false;
}
bool meshOperation::MeshReconstruction()
{
	bool sceneSplited=false;
	using namespace formatPOLY;
	int result;
	unsigned int sizeOfFace=modelData.modelFaces.size();
	//Construction de l'octree
	

	for(int idface=0;idface<sizeOfFace;idface++)
	{
		ivec3* faceSommets=&modelData.modelFaces[idface].indicesSommets;
		dvec3* faceA=&modelData.modelVertices[faceSommets->a];
		dvec3* faceB=&modelData.modelVertices[faceSommets->b];
		dvec3* faceC=&modelData.modelVertices[faceSommets->c];
		int coplanar=0;
		//Test octree et récuperation des résultats
		
		using namespace octreeTool;
		OctreeResult instanceCalcul(idface);
		modelOctree->GetDynamicCollisionCandidates(*lstFaces[idface],&instanceCalcul,&OctreeResult::DotTriangleCollisionTest);
		dvec3 rootFace=FaceNormal(*faceA,*faceB,*faceC);
		for(elementSize resultIndex=0;resultIndex<instanceCalcul.results.size();resultIndex++)
		{
			int idfaceTest=instanceCalcul.results[resultIndex];
			
	
			ivec3* faceTestSommets=&modelData.modelFaces[idfaceTest].indicesSommets;
			//Si le deux faces n'ont pas de sommets en commun
			int common_vertices=0;
			dvec3* faceA_Test=&modelData.modelVertices[faceTestSommets->a];
			dvec3* faceB_Test=&modelData.modelVertices[faceTestSommets->b];
			dvec3* faceC_Test=&modelData.modelVertices[faceTestSommets->c];
			dvec3 isecA;
			dvec3 isecB;
			//if(CollisionAlgo::IntersectionTriTri(*faceA,*faceB,*faceC,
			//		*faceA_Test,*faceB_Test,*faceC_Test,&isecA,&isecB))
			result=tri_tri_intersect_with_isectline(*faceA,*faceB,*faceC,
					*faceA_Test,*faceB_Test,*faceC_Test,
					&coplanar,
					isecA
					,isecB);
			if(result)
			{
				//If not coplanar
				//if(!FaceNormal(*faceA_Test,*faceB_Test,*faceC_Test).barelyEqual(rootFace))
				if(!coplanar)
				{
					bool collision_in_face2= IsDotInTriangle(*faceA_Test,*faceB_Test,*faceC_Test,isecA);
					bool collision_in_face1= IsDotInTriangle(*faceA,*faceB,*faceC,isecA);

					if(collision_in_face2 && collision_in_face1)
					{
						if(!OnCollisionDetectedSplitIt(modelData.modelFaces[idface],modelData.modelFaces[idfaceTest],isecA))
						{
							if(!isecA.barelyEqual(isecB))
							{
								collision_in_face2=IsDotInTriangle(*faceA_Test,*faceB_Test,*faceC_Test,isecB);
								collision_in_face1= IsDotInTriangle(*faceA,*faceB,*faceC,isecB);
								if(collision_in_face2 && collision_in_face1)
								{
									if(OnCollisionDetectedSplitIt(modelData.modelFaces[idface],modelData.modelFaces[idfaceTest],isecB))
									{
										sceneSplited=true;
										break;//return true;
									}
								}
							}
						}else{
							sceneSplited=true;
							break; //return true;
						}
					}
				}else{
					if(coplanarIntersection(idface,idfaceTest))
						return true;
					if(coplanarIntersection(idfaceTest,idface))
						return true;
				}
			}
		}
	}
	return sceneSplited;
}
/*
bool meshOperation::MeshReconstruction()
{
	bool sceneSplited=false;
	using namespace formatPOLY;
	int result;
	unsigned int sizeOfFace=modelData.modelFaces.size();
	//Construction de l'octree
	

	for(int idface=0;idface<sizeOfFace;idface++)
	{
		ivec3* faceSommets=&modelData.modelFaces[idface].indicesSommets;
		dvec3* faceA=&modelData.modelVertices[faceSommets->a];
		dvec3* faceB=&modelData.modelVertices[faceSommets->b];
		dvec3* faceC=&modelData.modelVertices[faceSommets->c];
		int coplanar=0;
		dvec3 isecA;
		dvec3 isecB;
		//Test octree et récuperation des résultats
		
		using namespace octreeTool;
		OctreeResult instanceCalcul(idface);
		modelOctree->GetDynamicCollisionCandidates(*lstFaces[idface],&instanceCalcul,&OctreeResult::DotTriangleCollisionTest);
		for(elementSize resultIndex=0;resultIndex<instanceCalcul.results.size();resultIndex++)
		{
			int idfaceTest=instanceCalcul.results[resultIndex];
			
			
			ivec3* faceTestSommets=&modelData.modelFaces[idfaceTest].indicesSommets;
			//Si le deux faces n'ont pas de sommets en commun
			int common_vertices=0;
			dvec3* faceA_Test=&modelData.modelVertices[faceTestSommets->a];
			dvec3* faceB_Test=&modelData.modelVertices[faceTestSommets->b];
			dvec3* faceC_Test=&modelData.modelVertices[faceTestSommets->c];
			result=tri_tri_intersect_with_isectline(*faceA,*faceB,*faceC,
					*faceA_Test,*faceB_Test,*faceC_Test,
					&coplanar,
					isecA
					,isecB);
			//Test des points de collision, si ces points de collisions sont simplement les points des sommets en commun alors on passe aux prochains triangles
			if(result)
			{

				if(!coplanar)
				{
					bool collision_in_face2= IsDotInTriangle(*faceA_Test,*faceB_Test,*faceC_Test,isecA);
					bool collision_in_face1= IsDotInTriangle(*faceA,*faceB,*faceC,isecA);

					if(collision_in_face2 && collision_in_face1)
					{
						if(!OnCollisionDetectedSplitIt(modelData.modelFaces[idface],modelData.modelFaces[idfaceTest],isecA))
						{
							if(!isecA.barelyEqual(isecB))
							{
								collision_in_face2=IsDotInTriangle(*faceA_Test,*faceB_Test,*faceC_Test,isecB);
								collision_in_face1= IsDotInTriangle(*faceA,*faceB,*faceC,isecB);
								if(collision_in_face2 && collision_in_face1)
								{
									if(OnCollisionDetectedSplitIt(modelData.modelFaces[idface],modelData.modelFaces[idfaceTest],isecB))
									{
										sceneSplited=true;
										break;//return true;
									}
								}
							}
						}else{
							sceneSplited=true;
							break; //return true;
						}
					}	
				}
			}
		}
	}
	return sceneSplited;
}
*/
bool meshOperation::OnCollisionDetectedDestroyIt( formatPOLY::t_face& triangleOne, formatPOLY::t_face& triangleTwo, const dvec3& position )
{
	using namespace formatPOLY;
	dvec3* nodeA_triangleOne=&modelData.modelVertices[triangleOne.indicesSommets.a];
	dvec3* nodeB_triangleOne=&modelData.modelVertices[triangleOne.indicesSommets.b];
	dvec3* nodeC_triangleOne=&modelData.modelVertices[triangleOne.indicesSommets.c];
	dvec3* nodeA_triangleTwo=&modelData.modelVertices[triangleTwo.indicesSommets.a];
	dvec3* nodeB_triangleTwo=&modelData.modelVertices[triangleTwo.indicesSommets.b];
	dvec3* nodeC_triangleTwo=&modelData.modelVertices[triangleTwo.indicesSommets.c];

	int idSommet_triangleOne=GetIndiceByCoord(*nodeA_triangleOne,*nodeB_triangleOne,*nodeC_triangleOne,position);
	int idSommet_triangleTwo=GetIndiceByCoord(*nodeA_triangleTwo,*nodeB_triangleTwo,*nodeC_triangleTwo,position);

	//Deux triangles se recontrent en un point, si ce point appartient au sommet des deux triangles on ne fait rien
	if(idSommet_triangleOne!=-1 && idSommet_triangleTwo!=-1)
		return false;

	int i_intersec;
	int addFaceOne,addFaceTwo;

	t_face* triangleToSplit=&triangleOne;
	if(idSommet_triangleTwo!=-1)
	{
		i_intersec=triangleTwo.indicesSommets[idSommet_triangleTwo];
	}else if(idSommet_triangleOne!=-1){		
		i_intersec=triangleOne.indicesSommets[idSommet_triangleOne];
		triangleToSplit=&triangleTwo;
	}else{
		//Ce point est inconnu des deux triangles on créé ce nouveau point
		i_intersec=this->FindIndexWithPosition(position);
		if(i_intersec==-1)
		{
			modelData.modelVertices.push_back(position);
			i_intersec=modelData.modelVertices.size()-1;
		}
	}

	//Destruction d'une des deux faces.

	return true;
}
bool meshOperation::OnCollisionDetectedSplitIt( formatPOLY::t_face& triangleOne, formatPOLY::t_face& triangleTwo, const dvec3& position )
{
	using namespace formatPOLY;
	dvec3* nodeA_triangleOne=&modelData.modelVertices[triangleOne.indicesSommets.a];
	dvec3* nodeB_triangleOne=&modelData.modelVertices[triangleOne.indicesSommets.b];
	dvec3* nodeC_triangleOne=&modelData.modelVertices[triangleOne.indicesSommets.c];
	dvec3* nodeA_triangleTwo=&modelData.modelVertices[triangleTwo.indicesSommets.a];
	dvec3* nodeB_triangleTwo=&modelData.modelVertices[triangleTwo.indicesSommets.b];
	dvec3* nodeC_triangleTwo=&modelData.modelVertices[triangleTwo.indicesSommets.c];

	int idSommet_triangleOne=GetIndiceByCoord(*nodeA_triangleOne,*nodeB_triangleOne,*nodeC_triangleOne,position);
	int idSommet_triangleTwo=GetIndiceByCoord(*nodeA_triangleTwo,*nodeB_triangleTwo,*nodeC_triangleTwo,position);

	//Deux triangles se recontrent en un point, si ce point appartient au sommet des deux triangles on ne fait rien
	if(idSommet_triangleOne!=-1 && idSommet_triangleTwo!=-1)
		return false;

	int i_intersec;
	int addFaceOne,addFaceTwo;

	t_face* triangleToSplit=&triangleOne;
	if(idSommet_triangleTwo!=-1)
	{
		i_intersec=triangleTwo.indicesSommets[idSommet_triangleTwo];
	}else if(idSommet_triangleOne!=-1){		
		i_intersec=triangleOne.indicesSommets[idSommet_triangleOne];
		triangleToSplit=&triangleTwo;
	}else{
		//Ce point est inconnu des deux triangles on créé ce nouveau point
		i_intersec=this->FindIndexWithPosition(position);
		if(i_intersec==-1)
		{
			modelData.modelVertices.push_back(position);
			i_intersec=modelData.modelVertices.size()-1;
		}
	}

	return SplitTriangleByThree(*triangleToSplit,i_intersec,&addFaceOne,&addFaceTwo);
}

void meshOperation::ShowStats()
{
	using namespace std;
	
	cout<<"Remesh status : "<<endl;
	cout<<"Vertices merged : "<<stat_NbPointsMerged<<endl;
	cout<<"Coplanar Faces destroyed : "<<stat_DestroyedFaces<<endl;
	cout<<"Face splitted : "<<stat_NbFaceSplitting<<endl;
}

bool meshOperation::SplitTriangleByThree( formatPOLY::t_face& triangleToSplit , const int& splitCentre,int* addedFaceOne,int* addedFaceTwo  )
{
	if(equal_arr(splitCentre,triangleToSplit.indicesSommets))
		return false;
	stat_NbFaceSplitting++;
	using namespace formatPOLY;
	ivec3 splitSommets=triangleToSplit.indicesSommets;

	dvec3 A_tri=modelData.modelVertices[splitSommets.a];
	dvec3 B_tri=modelData.modelVertices[splitSommets.b];
	dvec3 C_tri=modelData.modelVertices[splitSommets.c];
	dvec3 I_tri=modelData.modelVertices[splitCentre];
	
	std::cout<<"Split Triangle for the "<<stat_NbFaceSplitting<<" times. ["<<I_tri.x<<";"<<I_tri.y<<";"<<I_tri.z<<"]"<<std::endl;

	bool lie_on_AB=colinear(A_tri,I_tri,B_tri,BARELY_EPSILON);
	bool lie_on_BC=colinear(B_tri,I_tri,C_tri,BARELY_EPSILON);
	bool lie_on_CA=colinear(C_tri,I_tri,A_tri,BARELY_EPSILON);
	

	if(lie_on_AB)
		triangleToSplit.indicesSommets.b=splitCentre;
	else
		triangleToSplit.indicesSommets.c=splitCentre;

	t_face newFaceOne;
	newFaceOne.faceIndex=triangleToSplit.faceIndex;
	if(lie_on_BC)
		newFaceOne.indicesSommets=ivec3(splitCentre,splitSommets.c,splitSommets.a);		
	else
		newFaceOne.indicesSommets=ivec3(splitCentre,splitSommets.b,splitSommets.c);
	if(!lie_on_AB && !lie_on_BC && !lie_on_CA)
	{
		t_face newFaceTwo;
		newFaceTwo.faceIndex=triangleToSplit.faceIndex;
		newFaceTwo.indicesSommets=ivec3(splitSommets.a,splitCentre,splitSommets.c);
		if(FindFaceWithSommet(newFaceTwo.indicesSommets)==-1)
		{
			PushNewFace(newFaceTwo);
			*addedFaceTwo=modelData.modelFaces.size()-1;
		}
	}


	if(FindFaceWithSommet(newFaceOne.indicesSommets)==-1)
	{
		PushNewFace(newFaceOne);
		*addedFaceOne=modelData.modelFaces.size()-1;
	}

	return true;
}
void meshOperation::PushNewFace(const formatPOLY::t_face& newFace)
{
	modelData.modelFaces.push_back(newFace);
	lstFaces.push_back(new octreeTool::triangleElement(dvec3_to_vec3(modelData.modelVertices[newFace.indicesSommets.a]),dvec3_to_vec3(modelData.modelVertices[newFace.indicesSommets.b]),dvec3_to_vec3(modelData.modelVertices[newFace.indicesSommets.c])));
	modelOctree->PushNewSpaceElement(lstFaces[lstFaces.size()-1],lstFaces.size()-1,6);
}
void meshOperation::Save(const char* polyFileName)
{
	using namespace formatPOLY;

	CPoly polyImporter;

	polyImporter.ExportPOLY(modelData,polyFileName);

}

void meshOperation::mergeVertices()
{
	using namespace std;
	unsigned int sizeOf=modelData.modelVertices.size();
	std::vector<std::size_t> indexCorrespondance(sizeOf);
	std::multimap<double,std::pair<std::size_t,dvec3>> mergedVertices;
	typedef std::multimap<double,std::pair<std::size_t,dvec3>>::iterator mapIterator;
	typedef std::pair<double,std::pair<std::size_t,dvec3>> mapValue;

	for(std::size_t idVertice=0;idVertice<sizeOf;idVertice++)
	{
		const dvec3& vecValue(modelData.modelVertices[idVertice]);
		double vecLength(vecValue.length());
		bool foundDuplicate(false);
		for(mapIterator itfound=mergedVertices.find(vecLength);itfound!=mergedVertices.end() && itfound->first==vecLength;itfound++)
		{
			if(itfound->second.second.barelyEqual(vecValue))
			{
				indexCorrespondance[idVertice]=itfound->second.first;
				foundDuplicate=true;
				stat_NbPointsMerged++;
				break;
			}
		}
		if(!foundDuplicate)
		{
			indexCorrespondance[idVertice]=mergedVertices.size();
			mergedVertices.insert(mapValue(vecLength,std::pair<std::size_t,dvec3>(indexCorrespondance[idVertice],vecValue)));
		}
	}

	for(mapIterator mapit=mergedVertices.begin();mapit!=mergedVertices.end();mapit++)
	{
		modelData.modelVertices[mapit->second.first]=mapit->second.second;
	}
	modelData.modelVertices.resize(mergedVertices.size());
	mergedVertices.clear();
	std::size_t sizeOfFace=modelData.modelFaces.size();
	for(int idface=0;idface<sizeOfFace;idface++)
	{
		formatPOLY::t_face& curFace(modelData.modelFaces[idface]);
		curFace.indicesSommets[0]=indexCorrespondance[curFace.indicesSommets[0]];
		curFace.indicesSommets[1]=indexCorrespondance[curFace.indicesSommets[1]];
		curFace.indicesSommets[2]=indexCorrespondance[curFace.indicesSommets[2]];
	}
	unsigned int sizeOfUserFace=modelData.userDefinedFaces.size();
	for(int idface=0;idface<sizeOfUserFace;idface++)
	{
		formatPOLY::t_face& curFace(modelData.userDefinedFaces[idface]);
		curFace.indicesSommets[0]=indexCorrespondance[curFace.indicesSommets[0]];
		curFace.indicesSommets[1]=indexCorrespondance[curFace.indicesSommets[1]];
		curFace.indicesSommets[2]=indexCorrespondance[curFace.indicesSommets[2]];
	}
}