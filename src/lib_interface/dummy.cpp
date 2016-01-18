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

#include "coreinitialisation.h"

#include "tools/octree44.hpp"
#include "tools/octree44_dotElement.hpp"



class testAlgo
{
public:
	testAlgo(){}
	bool sendCollisionCandidatesMethod(const octreeTool::elementSize& staticCandidateIndex)
	{
		return false;
	}


};




int main(int argc, char* argv[])
{
	/*
	using namespace octreeTool;
	spaceElementContainer myContainer;
	Octree44 MyOctree(myContainer,8);


	dotElement dotTest;
	testAlgo testOctr;
	spaceElement* elementATester=&dotTest;




	typedef testAlgo CallClass;
	CALLBACK_METHOD = NULL;
	sendCollisionCandidatesMethod=&testAlgo::sendCollisionCandidatesMethod;

	/*
	MyOctree.GetDynamicCollisionCandidates<testAlgo>(
		*elementATester,
		&testOctr,
		sendCollisionCandidatesMethod
		);
	MyOctree.GetDynamicCollisionCandidates<testAlgo>(
		*elementATester,
		&testOctr,
		&testAlgo::sendCollisionCandidatesMethod
		);
		*/

	return 0;
}
