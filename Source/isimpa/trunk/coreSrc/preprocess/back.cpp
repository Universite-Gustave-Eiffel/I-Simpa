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


					if(result)
					{
						//Il y a deux cas possible
						//Cas 1, deux triangles sont coplanaire (triangle overlapping)
						//Cas 2, Les deux triangles entrent en collision
						if(coplanar)
						{
							std::cout<<"Superposition de triangles, suppression du plus petit "<<std::endl;
							if(GetAireTriangle(*faceA,*faceB,*faceC)>GetAireTriangle(modelData.modelVertices[faceTestSommets->a],modelData.modelVertices[faceTestSommets->b],modelData.modelVertices[faceTestSommets->c]))
							{
								modelData.modelFaces.erase(modelData.modelFaces.begin()+idfaceTest);
							}else{
								modelData.modelFaces.erase(modelData.modelFaces.begin()+idface);
							}
						}else{
							//////////////////////////////////////////////////////
							//Recherche des indices de isecA et isecB
							int secAOwner=idface;
							int secBOwner=idface;
							int i_isecA(-1),i_isecB(-1);
							int idSommetCollision=GetIndiceByCoord(*faceA,*faceB,*faceC,isecA);
							if(idSommetCollision==-1)
							{
								idSommetCollision=GetIndiceByCoord(modelData.modelVertices[faceTestSommets->a],modelData.modelVertices[faceTestSommets->b],modelData.modelVertices[faceTestSommets->c],isecA);
								if(idSommetCollision!=-1)
								{
									i_isecA=modelData.modelFaces[idfaceTest].indicesSommets[idSommetCollision];
									secAOwner=idfaceTest;
								}
							}else{
								i_isecA=modelData.modelFaces[idface].indicesSommets[idSommetCollision];
							}
							if(isecA.distance(isecB)<EPSILON)
							{
								i_isecB=i_isecA;
							}else{
								idSommetCollision=GetIndiceByCoord(*faceA,*faceB,*faceC,isecB);
								if(idSommetCollision==-1)
								{
									idSommetCollision=GetIndiceByCoord(modelData.modelVertices[faceTestSommets->a],modelData.modelVertices[faceTestSommets->b],modelData.modelVertices[faceTestSommets->c],isecB);
									if(idSommetCollision!=-1)
									{
										i_isecB=modelData.modelFaces[idfaceTest].indicesSommets[idSommetCollision];
										secBOwner=idfaceTest;
									}
								}else{
									i_isecB=modelData.modelFaces[idface].indicesSommets[idSommetCollision];
								}
							}
							//////////////////////////////////////////////////////
							//Recherche locales des indices de isecA et isecB infructueuses, recherche globale
							if(i_isecA==-1)
							{
								i_isecA=FindIndexWithPosition(isecA);
								if(i_isecA==-1)
								{
									modelData.modelVertices.push_back(isecA);
									i_isecA=modelData.modelVertices.size()-1;
								}
							}
							if(i_isecB==-1)
							{
								i_isecB=FindIndexWithPosition(isecB);
								if(i_isecA==-1)
								{
									modelData.modelVertices.push_back(isecA);
									i_isecA=modelData.modelVertices.size()-1;
								}
							}

							unsigned int idFaceToSplit=idfaceTest;
							if(secAOwner==idfaceTest)
								idFaceToSplit=idface;

							int  newFaceOne, newFaceTwo;
							SplitTriangleByThree(modelData.modelFaces[idFaceToSplit],i_isecA,&newFaceOne,&newFaceTwo);

							//Gestion du point d'intersection B

						}
						return true;
						/*
						std::cout<<coplanar<<" Intersect f1:"<<idface;
						genprintVec(*faceSommets);
						std::cout<<" f2:"<<idfaceTest;
						genprintVec(*faceTestSommets);
						std::cout<<" ";
						genprintVec(isecA);
						std::cout<<" ";
						genprintVec(isecB);
						std::cout<<std::endl;
						*/
					}