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

// Preprocess.cpp : définit le point d'entrée pour l'application console.
//
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

#include "computations.hpp"
#include <string>
#include <iostream>
#include "coreString.h"
#define MESH_CORRECTION_MAX 100

int MainProcess(int argc, char* argv[])
{
	progressionInfo progressManager(3);
	using namespace std;
	//**************************************************
	//Verification des arguments
	string pathFichier;
	if(argc>1)
	{
		pathFichier.append(argv[1]);
		for(int idarg=2;idarg<argc;idarg++)
		{
			pathFichier.append(" ");
			pathFichier.append(argv[idarg]);
		}
	}else{
		cout<<"The path of the mesh file must be specified !"<<endl;
		return 1;
	}


	meshOperation operationClass(pathFichier.c_str());
	if(!operationClass.IsOk())
	{
		cout<<"The mesh file cant be found !"<<endl;
		return 1;
	}
	int repairTries=0;

	////////////////////////////////////////////
	// Fusion des sommets proches
	progressOperation* progressOp=new progressOperation(progressManager.GetMainOperation());
	operationClass.DestroyNoAreaFaces();
	operationClass.mergeVertices();
	progressManager.OutputCurrentProgression();
	delete progressOp;
	//////////////////////////////////////////
	// Suppression des faces coplanaires
	while(operationClass.MeshDestroyCoplanarFaces() && repairTries<=MESH_CORRECTION_MAX)
	{
		repairTries++;
	}
	

	if(repairTries<MESH_CORRECTION_MAX)
	{
		operationClass.TransferUserFaceToGlobalFaces();
	}

	std::cout<<"Coplanar correction step and vertices merging step has finished."<<std::endl;
	repairTries=0;



	while(operationClass.MeshReconstruction() && repairTries<=MESH_CORRECTION_MAX)
	{
		repairTries++;
	}
    
	if(repairTries<MESH_CORRECTION_MAX)
	{
	    operationClass.ShowStats();
		operationClass.Save(pathFichier.c_str());
	}else{
				std::cout<<"Mesh reparation has been aborted. The algorithm enter into an infinite loop. Try to stick coplanar faces or destroy manually."<<std::endl;
	}

	return 0;
}


int main(int argc, char* argv[])
{
	MainProcess(argc,argv);

	#ifdef _DEBUG
		_CrtDumpMemoryLeaks(); //Affiche les fuites mémoires
	#endif

	return 0;
}