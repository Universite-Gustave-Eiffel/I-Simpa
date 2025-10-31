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

#include "scalar_field_creator.hpp"
#ifdef _DEBUG
#include <iostream>
#endif
#include <fstream>
#include <list>
#include <utility>
#include <stdio.h>
#include <string.h>
namespace ScalarFieldBuilders
{

	inline SpatialDiscretization::cell_id_t At(const SpatialDiscretization::cell_id_t& X,const SpatialDiscretization::cell_id_t& Y, const SpatialDiscretization::cell_id_t& Size)
	{
		return X+Y*Size;
	}

	#ifndef MIN
		#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
	#endif
	#ifndef MAX
		#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
	#endif

	vec3 CellIdToCenterCoordinate( const ivec3& cell_id, const decimal& cellSize, const vec3& zeroCellCenter)
	{
		return  zeroCellCenter+vec3(cellSize*cell_id.x,cellSize*cell_id.y,cellSize*cell_id.z);
	}

	ScalarFieldCreator::ScalarFieldCreator(const decimal& _resolution)
		:resolution(_resolution)
	{



	}
	ScalarFieldCreator::~ScalarFieldCreator()
	{
	}
	void ScalarFieldCreator::ComputeMatrixParams(const vec3& boxMin,const vec3& boxMax, const decimal& minResolution, mainVolumeConstruction_t& computedVolumeInfo)
	{
		vec3 boxsize=boxMax-boxMin;
		ivec3 cellCount=ivec3((long)ceil(boxsize.x/minResolution),(long)ceil(boxsize.y/minResolution),(long)ceil(boxsize.z/minResolution));

		computedVolumeInfo.boxMin=boxMin;
		computedVolumeInfo.boxMax=boxMax;
		computedVolumeInfo.cellCount=MAX(MAX(cellCount.x,cellCount.y),cellCount.z);
		computedVolumeInfo.mainVolumeCenter=(boxMax+boxMin)/2.f;
		computedVolumeInfo.cellSize=MAX(MAX(boxsize.x,boxsize.y),boxsize.z)/computedVolumeInfo.cellCount;
		computedVolumeInfo.cellHalfSize=vec3(computedVolumeInfo.cellSize,computedVolumeInfo.cellSize,computedVolumeInfo.cellSize)/2;
		computedVolumeInfo.zeroCellCenter=computedVolumeInfo.mainVolumeCenter-vec3(computedVolumeInfo.cellSize,computedVolumeInfo.cellSize,computedVolumeInfo.cellSize)*(decimal)(computedVolumeInfo.cellCount/2);
	}
	void ScalarFieldCreator::FirstStep_Params(const vec3& boxMin,const vec3& boxMax)
	{
		//Calcul de la boite englobante
		vec3 cellCubeSize(resolution,resolution,resolution);
		cellCubeSize*=2;
		ComputeMatrixParams(boxMin-cellCubeSize,boxMax+cellCubeSize,resolution,this->volumeInfo);
		//Cr�ation de la matrice

		domainInformation.domainSize=this->volumeInfo.cellCount;
		domainInformation.weight=0;
		fieldData=PTR<SpatialDiscretization::weight_matrix>(new SpatialDiscretization::weight_matrix(domainInformation));
	}
	unsigned int ScalarFieldCreator::count()
	{
		unsigned int cpt(0);
		(*fieldData).Count(cpt);
		return cpt;
	}
	std::size_t ScalarFieldCreator::GetDomainSize()
	{
		return this->volumeInfo.cellCount;
	}

	void ScalarFieldCreator::GetMinMax(vec3& minBox,vec3& maxBox)
	{
		minBox=this->volumeInfo.boxMin;
		maxBox=this->volumeInfo.boxMax;
	}

	decimal ScalarFieldCreator::GetVolumeValue(const SpatialDiscretization::weight_t& volId)
	{
		if(volId<this->volumeInfo.volumeValue.size())
		{
			return (decimal)this->volumeInfo.volumeValue[volId];
		}else{
			return 0.f;
		}
	}
	SpatialDiscretization::weight_t ScalarFieldCreator::GetVolumeCount()
	{
		return this->volumeInfo.volumeCount;
	}
	decimal GetVolumeValue(const SpatialDiscretization::weight_t& volId);
	void ScalarFieldCreator::MakeXYZ(const std::string& filename,const decimal& minVol)
	{
		using namespace SpatialDiscretization;
		std::ofstream xyzFile(filename.c_str());
		xyzFile.close();
		for(SpatialDiscretization::weight_t idvol=3;idvol<this->volumeInfo.volumeCount;idvol++)
		{
			if(this->volumeInfo.volumeValue[idvol]>=minVol)
				MakeXYZ(filename,idvol);
		}
	}
	bool ScalarFieldCreator::IsContainsVol( const ivec2& xyCell, SpatialDiscretization::weight_t& volId)
	{
		using namespace SpatialDiscretization;
		zcell* currentCell=&((*this->fieldData)[xyCell.x][xyCell.y]);
		while(currentCell->Next(&currentCell))
		{
			if(currentCell->GetData()==volId)
				return true;
		}
		return false;
	}
	void ScalarFieldCreator::GetMinMaxOnZ( const ivec2& xyCell, SpatialDiscretization::weight_t& minVolId,SpatialDiscretization::weight_t& maxVolId)
	{
		using namespace SpatialDiscretization;
		zcell* currentCell=&((*this->fieldData)[xyCell.x][xyCell.y]);
		minVolId=currentCell->GetData();
		maxVolId=minVolId;
		while(currentCell->Next(&currentCell))
		{
			minVolId=MIN(minVolId,currentCell->GetData());
			maxVolId=MAX(maxVolId,currentCell->GetData());
		}
	}
	typedef std::pair<SpatialDiscretization::weight_t,decimal> listValue_t;
	int sortFunc(const listValue_t& left,const listValue_t& right)
	{
		return left.second>right.second;
	}
	void ScalarFieldCreator::ExportVolsStats(const std::string& fileName, const std::string& volsLabelsFileName)
	{
		std::ofstream statVolsFile;
		statVolsFile.open(fileName.c_str(),std::ios_base::out);
		if(!statVolsFile)
			return;

		statVolsFile<<"Volume id"<<";value(m^3)"<<std::endl;
		//On va trier par ordre d�croissant de volume
		std::list<listValue_t> volumeList;
		for(int i=2;i<GetVolumeCount();i++)
		{
			volumeList.push_back(listValue_t(i,GetVolumeValue(i)));
		}
		volumeList.sort(&sortFunc);
		for(std::list<listValue_t>::iterator itvol=volumeList.begin();itvol!=volumeList.end();itvol++)
		{
			statVolsFile<<itvol->first<<";"<<itvol->second<<std::endl;
		}
		statVolsFile.close();

	}
	void ScalarFieldCreator::MakeXYZ(const std::string& filename,const SpatialDiscretization::weight_t& idVol)
	{
		std::ofstream xyzFile;
		xyzFile.open(filename.c_str(),std::ios_base::app);
		if(!xyzFile)
			return;

		using namespace SpatialDiscretization;
		cell_id_t cell_z=0;
		for(cell_id_t cell_x=0;cell_x<volumeInfo.cellCount;cell_x++)
		{
			for(cell_id_t cell_y=0;cell_y<volumeInfo.cellCount;cell_y++)
			{
				cell_z=0;
				zcell* currentCell=&((*this->fieldData)[cell_x][cell_y]);
				while(currentCell)
				{
					if((*currentCell).GetData()==idVol)
					{
						for(cell_id_t cell_z_offset=0;cell_z_offset<currentCell->GetSize();cell_z_offset++)
						{
							vec3 cellCenter=CellIdToCenterCoordinate(ivec3(cell_x,cell_y,cell_z+cell_z_offset),this->volumeInfo.cellSize,this->volumeInfo.zeroCellCenter);
							xyzFile<<cellCenter.x<<" "<<cellCenter.y<<" "<<cellCenter.z<<std::endl;
						}
					}
					cell_z+=currentCell->GetSize();
					currentCell->Next(&currentCell);
				}
			}
		}
		xyzFile.close();

	}

	void ScalarFieldCreator::InitExteriorVolumeId()
	{
		using namespace SpatialDiscretization;
		//Pour chaque s�rie Z
		// La premi�re et derni�re s�rie appartiendra au volume 2
		cell_id_t cell_z=0;
		for(cell_id_t cell_x=0;cell_x<volumeInfo.cellCount;cell_x++)
		{
			for(cell_id_t cell_y=0;cell_y<volumeInfo.cellCount;cell_y++)
			{
				zcell* currentCell=&((*this->fieldData)[cell_x][cell_y]);

				if(currentCell->GetData()==0)
					currentCell->SetData(weight_t(2));
				do
				{
					if(!currentCell->IsNextCell() && currentCell->GetData()==0)
						currentCell->SetData(weight_t(2));
				}while(currentCell->Next(&currentCell));

			}
		}

	}
	bool ScalarFieldCreator::CellToCellVolumePropagation(const ivec2& destinationPropa,const ivec2& sourcePropa,const SpatialDiscretization::weight_t& volumeId)
	{
		using namespace SpatialDiscretization;
		bool modification(false);
		zcell* sourceCell=&((*this->fieldData)[sourcePropa.x][sourcePropa.y]);
		zcell* destinationCell=&((*this->fieldData)[destinationPropa.x][destinationPropa.y]);
		cell_id_t sourceZ(0),destinationZ(0);
		do
		{
			if(sourceCell->GetData()==volumeId)
			{
				//On a trouv� une s�rie de Z correspondant � la valeur � �tendre
				//On navigue jusqu'� la position de la source
				while(destinationZ+destinationCell->GetSize()<sourceZ) //while(destinationZ<sourceZ)
				{
					destinationZ+=destinationCell->GetSize();
					if(!destinationCell->Next(&destinationCell))
						return modification;
				}
				//On affecte � partir de sourceZ jusqu'a sourceZ+sourceCell->GetSize() les noeuds dont la valeur est 0
				//while(destinationZ>= sourceZ || destinationZ+destinationCell->GetSize()<=sourceZ+sourceCell->GetSize())
				while(true)
				{
					if(destinationCell->GetData()==0)
					{
						destinationCell->SetData(volumeId);
						modification=true;
					}
					//On avance si notre pointer est encore dans la cellule source
					if(sourceZ+sourceCell->GetSize()>destinationZ+destinationCell->GetSize())	{

						destinationZ+=destinationCell->GetSize();
						if(!destinationCell->Next(&destinationCell))
							return modification;
					}else{
						break;
					}
				}
			}
			sourceZ+=sourceCell->GetSize();
		}while(sourceCell->Next(&sourceCell));
		return modification;
	}

	void ScalarFieldCreator::ComputeVolumesValue(std::vector<decimal>& volumeValue)
	{
		using namespace SpatialDiscretization;
		volumeValue=std::vector<decimal>(this->volumeInfo.volumeCount,0.);
		decimal cellVolume=pow((decimal)this->volumeInfo.cellSize,(decimal)3.);
		for(cell_id_t cell_x=0;cell_x<volumeInfo.cellCount;cell_x++)
		{
			for(cell_id_t cell_y=0;cell_y<volumeInfo.cellCount;cell_y++)
			{
				zcell* currentCell=&((*this->fieldData)[cell_x][cell_y]);
				do
				{
					volumeValue[(*currentCell).GetData()]+=cellVolume*currentCell->GetSize();
				}while(currentCell->Next(&currentCell));
			}
		}
	}
	ivec3 ScalarFieldCreator::GetFirstCellByWeight(const SpatialDiscretization::weight_t& weight,SpatialDiscretization::zcell** foundCell )
	{
		using namespace SpatialDiscretization;
		cell_id_t cell_z;
		for(cell_id_t cell_x=0;cell_x<volumeInfo.cellCount;cell_x++)
		{
			for(cell_id_t cell_y=0;cell_y<volumeInfo.cellCount;cell_y++)
			{
				cell_z=0;
				zcell* currentCell=&((*this->fieldData)[cell_x][cell_y]);
				while(currentCell)
				{
					if((*currentCell).GetData()==weight)
					{
						*foundCell=currentCell;
						return ivec3(cell_x,cell_y,cell_z);
					}
					cell_z+=currentCell->GetSize();
					currentCell->Next(&currentCell);
				}
			}
		}
		*foundCell=NULL;
		return ivec3();
	}

	ivec3 ScalarFieldCreator::GetCellIdByCoord(const vec3& position)
	{
		vec3 tmpvec=((position-this->volumeInfo.mainVolumeCenter)/this->volumeInfo.cellSize);
		ivec3 halfCellCount(this->volumeInfo.cellCount/2,this->volumeInfo.cellCount/2,this->volumeInfo.cellCount/2);
		return ivec3((long)floor(tmpvec.x),(long)floor(tmpvec.y),(long)floor(tmpvec.z))+halfCellCount;
	}
	void ScalarFieldCreator::ExtandVolume(const SpatialDiscretization::weight_t& volumeId)
	{
		long neighLink[4][2]={{0,1},
							  {1,0},
		                      {0,-1},
			                  {-1,0} };

		using namespace SpatialDiscretization;
		std::size_t sizeOfMatrixXY(this->volumeInfo.cellCount*this->volumeInfo.cellCount);
		PTR_ARR<bool> cellsToCheck(new bool[sizeOfMatrixXY]); //Matrice X,Y de bool�en indiquant les cellules � v�rifier dans le cycle courant
		PTR_ARR<bool> NextCellsToCheck(new bool[sizeOfMatrixXY]); //Matrice X,Y de bool�en indiquant les cellules � v�rifier lors du prochain cycle.
		memset(cellsToCheck.get(),1,sizeof(bool)*sizeOfMatrixXY);
		memset(NextCellsToCheck.get(),0,sizeof(bool)*sizeOfMatrixXY);
		bool moreCellsToCheck(true); //Si sur un cycle de test aucune cellule ne s'est vu modifi� alors le volume volumeId est compl�tement d�fini
		/*
		#ifdef _DEBUG
		std::size_t loopPropa(0);
		#endif
		*/
		do
		{
			/*
			#ifdef _DEBUG
			loopPropa++;
			std::cout<<"Boucle propagation #"<<loopPropa<<std::endl;
			std::size_t cellchecked(0);
			#endif
			*/
			moreCellsToCheck=false;
			cell_id_t cellXY;
			for(cell_id_t cell_x=0;cell_x<volumeInfo.cellCount;cell_x++)
			{
				for(cell_id_t cell_y=0;cell_y<volumeInfo.cellCount;cell_y++)
				{

					cellXY=At(cell_x,cell_y,this->volumeInfo.cellCount);
					if(cellsToCheck[cellXY]==true)
					{
						/*
						#ifdef _DEBUG
							cellchecked++;
						#endif
						*/
						ivec2 currentid(cell_x,cell_y);
						/////////////////////////////
						// En fonction des [2-4] voisins nous allons propager les identifiants de la s�rie.
						bool destCellUpdated(false);
						for(unsigned short neigh=0;neigh<4;neigh++)
						{
							ivec2 FromXY=ivec2(neighLink[neigh])+currentid;
							if((unsigned int)FromXY.x>=0 && (unsigned int)FromXY.x<this->volumeInfo.cellCount && (unsigned int)FromXY.y>=0 && (unsigned int)FromXY.y<this->volumeInfo.cellCount)
							{
								cell_id_t neighXY=At(FromXY.x,FromXY.y,this->volumeInfo.cellCount);
								if(CellToCellVolumePropagation(currentid,FromXY,volumeId))
									destCellUpdated=true;
							}
						}

						if(destCellUpdated) //Cellule de destination modifi�, toute les cellule voisines doivent �tre marqu� comme � subir une propagation
						{
							moreCellsToCheck=true;
							//NextCellsToCheck[cellXY]=true;
							for(unsigned short neigh=0;neigh<4;neigh++)
							{
								ivec2 FromXY=ivec2(neighLink[neigh])+currentid;
								if((unsigned int)FromXY.x>=0 && (unsigned int)FromXY.x<this->volumeInfo.cellCount && (unsigned int)FromXY.y>=0 && (unsigned int)FromXY.y<this->volumeInfo.cellCount)
								{
									cell_id_t neighXY=At(FromXY.x,FromXY.y,this->volumeInfo.cellCount);
									NextCellsToCheck[neighXY]=true;
								}
							}
						}
					}
				}
			}
			memcpy(cellsToCheck.get(),NextCellsToCheck.get(),sizeof(bool)*sizeOfMatrixXY);
			memset(NextCellsToCheck.get(),0,sizeof(bool)*sizeOfMatrixXY);
			/*
			#ifdef _DEBUG
			std::cout<<"Cells checked="<<cellchecked<<std::endl;
			#endif
			*/
		}while(moreCellsToCheck);
	}

	vec3 ScalarFieldCreator::GetCenterCellCoordinates( const ivec3& cell_id) const
	{
		return CellIdToCenterCoordinate(cell_id,this->volumeInfo.cellSize, this->volumeInfo.zeroCellCenter);
	}

	SpatialDiscretization::weight_t ScalarFieldCreator::GetMatrixValue(const ivec3& index)
	{
		return (*this->fieldData)[index.x][index.y][index.z];
	}

	void ScalarFieldCreator::ThirdStep_VolumesCreator()
	{
		using namespace SpatialDiscretization;
		//Initialisation du volume 2 (exterieur)
		InitExteriorVolumeId();
		ExtandVolume(SpatialDiscretization::weight_t(2));
		zcell* foundCell(NULL);
		GetFirstCellByWeight(weight_t(0),&foundCell);
		weight_t volId(3);
		while(foundCell!=NULL)
		{

			#ifdef _DEBUG
			std::cout<<"Propagation of volume id="<<volId<<std::endl;
			#endif
			//Initialisation du volume volId
			foundCell->SetData(volId);
			ExtandVolume(volId);
			//Passage au prochain volume

			volId++;
			GetFirstCellByWeight(weight_t(0),&foundCell);
		}
		volumeInfo.volumeCount=volId;
		ComputeVolumesValue(this->volumeInfo.volumeValue);
	}
}
