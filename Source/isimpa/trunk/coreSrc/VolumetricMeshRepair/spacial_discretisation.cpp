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

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "spacial_discretisation.hpp"
#include <iostream>

/**
 * Cette méthode permet le tirage aléatoire d'un nombre décimal
 * @return Decimal de 0 à 1
 */
inline float GetRandValue()
{
	return ((float)rand()) / ((float)RAND_MAX+1);
}
inline int Range(const int& from,const int& to)
{
	int res=(int)floor(GetRandValue()*(to-from)+from);
	if(res<to)
		return res;
	else
		return from;
}



void TestAlgo()
{
	const unsigned int resolution(500);
	xcell_t mainArray(resolution); //Créé une discrétisation spaciale de 15m3  si la précision est de 1m
	/*
	mainArray.at(10).at(10).at(10).dotValue.set(3.14f,3.14f,3.14158f);
	mainArray.at(9).at(11).at(10)=mainArray[10][10][10];
	mainArray.at(10).at(10).at(10).dotValue.set(8.f,8.f,3.14158f);
	const vec3& dotval=mainArray[9][11][10].dotValue;
	std::cout<<dotval.x<<" "<<dotval.y<<" "<<dotval.z<<std::endl;
	*/

	unsigned int nbcell(0);
	/*
	for(int x=20;x<resolution-20;x++)
	{
		mainArray.at(x).at(12).at(13).dotValue.set(3.14f,3.14f,3.14158f);
	}
	mainArray.Count(nbcell);
	std::cout<<nbcell<<" "<<resolution*resolution*resolution<<std::endl;
*/
	/*
	for(int x=0;x<resolution;x++)
	{
		for(int y=0;y<resolution;y++)
		{
			for(int z=0;z<resolution;z++)
			{
				mainArray.at(x,resolution).at(y,resolution).at(z,resolution);
			}
		}
	}
	*/
	for(int i=0;i<300000;i++)
	{
		ivec3 position(Range(0,resolution),Range(0,resolution),Range(0,resolution));
		FindOrCreateCell(mainArray,resolution,position);
	}


	mainArray.Count(nbcell);
	std::cout<<nbcell<<" "<<resolution*resolution*resolution<<std::endl;
}
int main(int argc, char* argv[])
{
	TestAlgo();

	#ifdef _DEBUG
		_CrtDumpMemoryLeaks(); //Affiche les fuites mémoires
	#endif

	return 0;
}