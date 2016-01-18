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

/*
 * Code source écrit dans le cadre du projet OPALHA contrat ADEME n°06.04.C.0070 (2007)
 */



#include <cassert>
#include <vector> //todo suppr dépendance
#include <list>
#include <string.h>

#ifndef __OCTREE44_H__
#define __OCTREE44_H__

/**
 * @file octree44.hpp
 * @brief Optimisation de la détéction de collision
 *
 * L'espace est fragmenté en 8 espace et ce sur plusieurs itérations
 *
 */

namespace octreeTool
{
	typedef unsigned short sommetIndex;
	typedef unsigned short sizeChild;
	typedef short depthSize;
	typedef float coordPrecision;
	/**
	 * Argument correspondant à la méthode d'envoie des candidats à la collision positive
	 * @return Faux si le test de collision doit se terminer
	 */
	#define CALLBACK_METHOD bool(CallClass::*sendCollisionCandidatesMethod)(const octreeTool::elementSize& staticCandidateIndex)
	#define CALLER_DEF CallClass* caller
	#define CALL_CALLBACK (caller->*sendCollisionCandidatesMethod)


	#define SETVEC(source,destinationVec) \
		destinationVec[0]=source;	 \
		destinationVec[1]=source;	 \
		destinationVec[2]=source;
	#define COPYVEC(source,destination) \
		destination[0]=source[0];	 \
		destination[1]=source[1];	 \
		destination[2]=source[2];

	#ifndef MIN
		#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
	#endif
	#ifndef MAX
		#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
	#endif


	inline static void SetMinMax(const coordPrecision& coordTest,coordPrecision& coordMin,coordPrecision& coordMax)
	{
		if(coordTest<coordMin)
			coordMin=coordTest;
		else if(coordTest>coordMax)
			coordMax=coordTest;
	}
	inline static void SetMinMaxVec(coordPrecision coordTest[3],coordPrecision coordMin[3],coordPrecision coordMax[3])
	{
		SetMinMax(coordTest[0],coordMin[0],coordMax[0]);
		SetMinMax(coordTest[1],coordMin[1],coordMax[1]);
		SetMinMax(coordTest[2],coordMin[2],coordMax[2]);
	}
	/**
	 * @brief Elément positionné dans l'espace 3d
	 *
	 * Cette classe dite virtuelle correspond à un élément positionné dans l'espace et notament dans un ou plusieurs noeuds de l'octree.
	 * Pour utiliser la classe Octree vous devez utiliser une classe dérivant de celle-ci. Vous pouvez utiliser par exemple triangleElement ou dotElement.
	 * @see triangleElement
	 * @see dotElement
	 */
	class spaceElement
	{
		public:
			spaceElement() {};
			virtual ~spaceElement() {};
			virtual bool IsOverlap(coordPrecision boxcenter[3],const coordPrecision& boxhalfsize)=0;
			virtual void GetMinMax(coordPrecision xyzMin[3],coordPrecision xyzMax[3])=0;
	};

	/**
	 * @brief Définition d'une boite.
	 */
	struct t_box
	{
		coordPrecision boxcenter[3];
		coordPrecision boxhalfsize;
	};

	typedef std::vector<spaceElement*> spaceElementContainer;
	typedef spaceElementContainer::size_type elementSize;
	typedef std::list<elementSize> elementIndexContainer; //Todo remplacer par linked list class
	typedef std::list<elementSize>::iterator elementIndexContainerIterator;
	typedef std::vector<t_box> boxContainer;



	/**
	 * Offset de translation des sous volumes
	 */
	static coordPrecision octNodeOffset[8][3] =
                {
                        {-0.5, -0.5, -0.5},
                        {+0.5, -0.5, -0.5},
                        {-0.5, +0.5, -0.5},
                        {+0.5, +0.5, -0.5},
                        {-0.5, -0.5, +0.5},
                        {+0.5, -0.5, +0.5},
                        {-0.5, +0.5, +0.5},
                        {+0.5, +0.5, +0.5}
                };

	static void GetCubeCenterCoord(const coordPrecision& parentHalfSize,coordPrecision parentCenterCoord[3], const sizeChild& octantIndex,coordPrecision octCenterCoord[3])
	{
		coordPrecision radius[3];
		radius[0]= parentHalfSize * octNodeOffset[octantIndex][0];
		radius[1]= parentHalfSize * octNodeOffset[octantIndex][1];
		radius[2]= parentHalfSize * octNodeOffset[octantIndex][2];
		octCenterCoord[0]=radius[0]+parentCenterCoord[0];
		octCenterCoord[1]=radius[1]+parentCenterCoord[1];
		octCenterCoord[2]=radius[2]+parentCenterCoord[2];

	}
	/**
	 * @brief Cette classe correspond à une subdivision de l'espace
	 *
	 * Cette classe est locale à octree et ne dois pas être utilisé à l'exterieur de octree44.hpp
	 * Cette espace ne sera pas subdivisé si la taille de elementLst est inférieur ou égal à 8 ou si maxDepth est atteint
	 */
      
	class t_Node
	{
		public:
			t_Node* subNode[8];

			template<class CallClass> bool DynamicTestElement( spaceElement& testElement, CALLER_DEF, CALLBACK_METHOD )
			{
				//On test cet élément par rapport a notre volume et si c'est concluant on appel la méthode pour chacun de nos éléments
				if(testElement.IsOverlap(boxcenter,boxhalfsize))
				{
					for( elementIndexContainerIterator elementIt=elementIndexLst.begin();elementIt!=elementIndexLst.end();elementIt++)
					{
						if(!CALL_CALLBACK((*elementIt))) //On continu a envoyer les candidats tant que l'objet qui a demandé le test le désire
							return false;
					}
					for(sizeChild idchild=0;idchild<8;idchild++)
					{
						if(HasChild(idchild))
						{
							if(!subNode[idchild]->DynamicTestElement<CallClass>(testElement,caller,sendCollisionCandidatesMethod))
								return false;
						}
					}
				}
				return true;
			}



			void SetPositionCenterX(const coordPrecision& _x){boxcenter[0]=_x;}
			void SetPositionCenterY(const coordPrecision& _y){boxcenter[1]=_y;}
			void SetPositionCenterZ(const coordPrecision& _z){boxcenter[2]=_z;}
			void SetHalfSize(const coordPrecision& hSize){boxhalfsize=hSize;}
			bool HasChild(sizeChild idchild) { return subNode[idchild]!=NULL; }
			/**
			 * Ce noeud a t'il au moins un noeud fils
			 */
			bool HasChildNodes()
			{
				for(sizeChild idchild=0;idchild<8;idchild++)
					if(HasChild(idchild))
						return true;
				return false;
			}

                        t_Node( )
                                :sizeofel(0)
                        {
                                memset(subNode,0,sizeof(subNode));
                        }

                        ~t_Node()
                        {
                                for(sizeChild idchild=0;idchild<8;idchild++)
                                        delete subNode[idchild];
                        }

                        bool Build(spaceElementContainer& staticElementsLst,t_Node* parent,const depthSize& maxDepth,const depthSize& minDepth)
                                {
                                        if(!parent)
                                        {
                                                Clear();
                                                //Noeud principal
                                                //for(elementSize elTest=0;elTest<staticElementsLst.size();elTest++)
                                                //	PushElementToArray(elTest);
                                        }else{
                                                Clear();
                                                if(!parent->elementIndexLst.empty())
                                                {
                                                    for( elementIndexContainerIterator elementIt=parent->elementIndexLst.begin();elementIt!=parent->elementIndexLst.end();elementIt++)
                                                    {
                                                            //Si collision entre cette élément et ce noeud(cube)
                                                            if(staticElementsLst[*elementIt]->IsOverlap(boxcenter,boxhalfsize))
                                                                    PushElementToArray(*elementIt);
                                                    }
                                                }else{
                                                    for(elementSize elTest=0;elTest<staticElementsLst.size();elTest++)
                                                    {
                                                            //Si collision entre cette élément et ce noeud(cube)
                                                            if(staticElementsLst[elTest]->IsOverlap(boxcenter,boxhalfsize))
                                                                    PushElementToArray(elTest);
                                                    }
                                                }
                                                if(Size()==0)
                                                        return false;
                                        }
                                        if((maxDepth>0 && (Size()>8 || !parent))||minDepth>0)
                                        {
                                                //Si cet espace doit être de nouveau subdivisé on créé les 8 enfants
                                                for(sizeChild idchild=0;idchild<8;idchild++)
                                                {
                                                        subNode[idchild]=new t_Node();
                                                        coordPrecision subboxcenter[3];
                                                        GetCubeCenterCoord(boxhalfsize,boxcenter,idchild,subboxcenter);
                                                        subNode[idchild]->SetPositionCenterX(subboxcenter[0]);
                                                        subNode[idchild]->SetPositionCenterY(subboxcenter[1]);
                                                        subNode[idchild]->SetPositionCenterZ(subboxcenter[2]);
                                                        subNode[idchild]->SetHalfSize(boxhalfsize*.5f);
                                                        if(!subNode[idchild]->Build(staticElementsLst,this,maxDepth-1,minDepth-1))
                                                        {
                                                                delete subNode[idchild];
                                                                subNode[idchild]=NULL;
                                                        }
                                                }
                                                //Comme cet espace a été subdivisé on vide notre liste d'éléments
                                                Clear();
                                        }
                                        return true;
                                }

                                bool Append(spaceElement* newElement,const elementSize& elementIndex ,const depthSize& maxDepth)
                                {
                                        if(newElement->IsOverlap(boxcenter,boxhalfsize))
                                        {
                                                //On test les sous volumes si on ne dépasse pas la profondeur max
                                                // et que l'on a au moins atteint la profondeur min
                                                if(maxDepth>0)
                                                {
                                                        for(sizeChild idchild=0;idchild<8;idchild++)
                                                        {
                                                                if(!HasChild(idchild))
                                                                {
                                                                        subNode[idchild]=new t_Node();
                                                                        coordPrecision subboxcenter[3];
                                                                        GetCubeCenterCoord(boxhalfsize,boxcenter,idchild,subboxcenter);
                                                                        subNode[idchild]->SetPositionCenterX(subboxcenter[0]);
                                                                        subNode[idchild]->SetPositionCenterY(subboxcenter[1]);
                                                                        subNode[idchild]->SetPositionCenterZ(subboxcenter[2]);
                                                                        subNode[idchild]->SetHalfSize(boxhalfsize*.5f);
                                                                }
                                                                subNode[idchild]->Append(newElement,elementIndex,maxDepth-1);
                                                        }
                                                }else{
                                                        this->PushElementToArray(elementIndex);
                                                }
                                                return true;
                                        }else{
                                                return false;
                                        }
                                }

                                void Remove(spaceElement* oldElement,const elementSize& oldElementIndex)
                                {
                                        if(oldElement->IsOverlap(boxcenter,boxhalfsize))
                                        {
                                                if(Size()>0)
                                                {
                                                        RemoveElIndexFromArray(oldElementIndex);
                                                }
                                                for(sizeChild idchild=0;idchild<8;idchild++)
                                                {
                                                        if(HasChild(idchild))
                                                        {
                                                                subNode[idchild]->Remove(oldElement,oldElementIndex);
                                                        }
                                                }
                                        }
                                }

                                void Remove(const elementSize& oldElementIndex)
                                {
                                        if(Size()>0)
                                        {
                                                RemoveElIndexFromArray(oldElementIndex);
                                        }
                                        for(sizeChild idchild=0;idchild<8;idchild++)
                                        {
                                                if(HasChild(idchild))
                                                {
                                                        subNode[idchild]->Remove(oldElementIndex);
                                                }
                                        }
                                }
                                void Clear()
                                {
                                    elementIndexLst.clear();
                                    sizeofel=0;
                                }

                                void RemoveElIndexFromArray( const elementSize& indexToRemove)
                                {
                                        for(elementIndexContainer::iterator iter=elementIndexLst.begin();iter!=elementIndexLst.end();iter++)
                                        {
                                                if(*iter==indexToRemove)
                                                {
                                                        sizeofel--;
                                                        elementIndexLst.erase(iter);
                                                        break;
                                                }
                                        }
                                }
                                void PushElementToArray(const elementSize& newElementIndex)
                                {
                                        sizeofel++;
                                        elementIndexLst.push_back(newElementIndex);
                                }

                                elementSize Size()
                                {
                                        return sizeofel;
                                }

                                void GetTotalNodes(elementSize& totalNodes)
                                {
                                        totalNodes++;
                                        for(sizeChild idchild=0;idchild<8;idchild++)
                                        {
                                                if(HasChild(idchild))
                                                {
                                                        subNode[idchild]->GetTotalNodes(totalNodes);
                                                }
                                        }

                                }

                                void GetTotalElementCount(elementSize& totalElementIndices)
                                {
                                        totalElementIndices+=Size();
                                        for(sizeChild idchild=0;idchild<8;idchild++)
                                        {
                                                if(HasChild(idchild))
                                                {
                                                        subNode[idchild]->GetTotalElementCount(totalElementIndices);
                                                }
                                        }

                                }
                                void PushBoxInfos(boxContainer& boxLst)
                                {
                                        if(HasChildNodes() || Size()>0)
                                        {
                                                t_box infoBox;
                                                infoBox.boxhalfsize=boxhalfsize;
                                                COPYVEC(boxcenter,infoBox.boxcenter);
                                                boxLst.push_back(infoBox);
                                        }
                                                for(sizeChild idchild=0;idchild<8;idchild++)
                                                {
                                                        if(HasChild(idchild))
                                                        {
                                                                subNode[idchild]->PushBoxInfos(boxLst);
                                                        }
                                        }
                                }

                                bool IsOverlap(spaceElement& newElement)
                                {
                                        if(newElement.IsOverlap(boxcenter,boxhalfsize))
                                                return true;
                                        else
                                                return false;
                                }
                    protected:
			coordPrecision boxcenter[3];
			coordPrecision boxhalfsize;
			std::size_t sizeofel;
			//friend class t_Node; //elementLst est visible par un objet de type t_Node exterieur au notre.

		private:
			elementIndexContainer elementIndexLst;
	};

	/**
	 * @brief Optimisation de la détéction de collision sur une liste fixe d'élément
	 *
	 * @ref http://en.wikipedia.org/wiki/Octree
	 *
	 * Cette classe permet la détection de collision entre un éléments et une liste d'élément placé dans un espace tridimensionnel.
	 * Cet élément peut être de n'importe quel type. Du moment qu'un test de collision existe entre un cube et cet élément.
	 * @see spaceElement::IsOverlap()
	 */
	class Octree44
	{
		public:
			/**
			 * Constructeur
			 * @warning _staticElements Il faut conserver l'ordre des éléments du tableau passé en paramètre avant que Octree44 ne soit détruit, sinon évidament les indices retournés lors des recherches ne correspondent plus. Il faut également penser à détruire ce tableau quand il n'est plus utilisé (seul les indices des éléments sont conservés en interne et le tableau peut donc être immédiatement effacé peut donc etre immédiatement effacé).
			 *
			 */
			Octree44(spaceElementContainer& _staticElements,const depthSize& maxDepth,const depthSize& minDepth=0 );

			Octree44( const coordPrecision _boundingBoxMin[3], const coordPrecision _boundingBoxMax[3]);
			/**
			 * Ajoute un nouvel élément dans l'arbre octree
			 * Cette méthode est plus couteuse car les tableaux sont redimensionnés dans chaque noeud affecté par l'ajout de cet indice
			 */
                        void PushNewSpaceElement(spaceElement* newElement,const elementSize& elementIndex, const depthSize& maxDepth);


			/**
			 * Cette méthode permet la suppression d'un élément via un élément de test, cette méthode est plus rapide que son homologue @see RemoveSpaceElementByElementIndex
			 * @param oldElement Element dans le même volume que l'ancien élément
			 * @param elementIndex Indice de l'ancien élément à supprimer
			 */
			void RemoveSpaceElementByElement(spaceElement* oldElement,const elementSize& elementIndex);
			/**
			 * Cette méthode permet la suppression d'un élément via un élément de test, cette méthode est plus lente que son homologue @see RemoveSpaceElementByElement
			 * @param elementIndex Indice de l'ancien élément à supprimer
			 */
			void RemoveSpaceElementByElementIndex(const elementSize& elementIndex);
			/**
			 * Lance un test de rapprochement
			 * @param testElement Element qui va subir les test de position de volumes
			 * @param sendCollisionCandidatesMethod Méthode d'une classe non statique qui sera appelé pour chaque élément se trouvant dans le même cube que testElement. Tant que votre méthode retournera vrai alors un nouveau candidat sera passé.
			 * @warning Le même indice d'élément peut être retourné plusieurs fois.
			 * Utilisation :
			 * MyOctree.GetDynamicCollisionCandidates<MyClass>( testElement, this, &MyClass::MyPushCandidatesMethod );
			 */
			template<class CallClass>
			void GetDynamicCollisionCandidates( spaceElement& testElement, CALLER_DEF, CALLBACK_METHOD  )
			{
				rootNode.DynamicTestElement<CallClass>(testElement,caller,sendCollisionCandidatesMethod);
			}
			/**
			 * Retourne tout les élement proche de testElement.
			 * Il n'y a pas de double sur les indices retournés.
			 */
			void GetAllCollisionCandidates( spaceElement& testElement, std::vector<elementSize>& candidates );
			/**
			 * Retourne le nombre de noeud existant
			 */
			elementSize GetNodesSize();
			/**
			 * Retourne le nombre d'éléments inséré (avec doublons si plusieurs espaces occupé par l'élément)
			 */
			elementSize GetElementLstSize();
			/**
			 * Alimente un vecteur avec les informations des noeuds ayant un fils ou des éléments à l'intérieur.
			 */
			void PushBoxInfos(boxContainer& boxLst);

			/**
			 * Effectue un test afin de savoir si l'élément passé en paramètre est à l'interieur du volume principal
			 */
			bool InsideOctree(spaceElement& testElement);
		protected:
			void InitFromBoundingBox();
			coordPrecision boundingBoxMin[3];
			coordPrecision boundingBoxMax[3];
			elementSize sizeOfStaticElements;
			t_Node rootNode;

	};
}

#endif
