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

#ifndef __PROGINFO_
#define __PROGINFO_
#include <iostream>
#include <math.h>

/**
 * @file progressionInfo.h
 * @brief Gestionnaire d'affichage de progression du calcul compatible avec le multithreading (à condition de ne pas détruire deux objets en même temps)
 */


/**
 * Représente une opération de calcul
 */
class progressOperation
{
public:
	/**
	 * Constructeur
	 */
	progressOperation(progressOperation* parentOperation,unsigned int sonEstimation=1)
		:parent(parentOperation),
		sizeOf(0)
		,sizeOfEstimation(sonEstimation)
	{

	}

	~progressOperation()
	{
		if(parent)
		{
			parent->PushProgression(1-(sizeOf/sizeOfEstimation));
		}
	}

	void GetState(float* thisLvlProg)
	{
		*thisLvlProg=sizeOf/sizeOfEstimation;
	}

	void Next()
	{
		PushProgression(1.f);
	}
protected:
	//friend class progressOperation;

	/**
	 *  Un des fils a changé d'état incProg constitue la différence entre la dernière mise à jour de sa progression et sa nouvelle progression
	 * @param incProg ]0;1]
	 */
	void PushProgression(float incProg)
	{
		if(sizeOf+incProg<=sizeOfEstimation)
		{
			if(parent && sizeOf>0)
				parent->PushProgression(-(sizeOf/sizeOfEstimation));
			sizeOf+=incProg;
			if(parent)
				parent->PushProgression(sizeOf/sizeOfEstimation);
		}
	}
private:
	progressOperation* parent;
	unsigned int sizeOfEstimation; //Nombre de fils estimé
	float sizeOf;				   //Nombre de fils détruits
};







/**
 * @brief Gestionnaire d'affichage de progression du calcul compatible avec le multithreading
 */
class progressionInfo
{
private:
	progressOperation mainOperation;
	float lastOutputProgression;
public:
	/**
	 * Constructeur
	 * @param firstLevelEstimation Nombre de tache au premier niveau hiérarchique de taches
	 */
	progressionInfo(int firstLevelEstimation)
		:mainOperation(NULL,firstLevelEstimation)
	{
		lastOutputProgression=0;
	}

	virtual ~progressionInfo()
	{
	}
	/**
	 * @return L'operation principale.
	 */
	progressOperation* GetMainOperation()
	{
		return &mainOperation;
	}

	/**
	 * Affiche la progression si celle-ci a changé
	 */
	void OutputCurrentProgression()
	{
		using namespace std;
		float currentProgression;
		mainOperation.GetState(&currentProgression);
		currentProgression*=100;
		if ((int)(currentProgression * 100)>(int)(lastOutputProgression * 100) && currentProgression<100)
		{
			lastOutputProgression=currentProgression;
			DoShowProgression(currentProgression);
		}
	}
	float GetCurrentProgression()
	{
		float currentProgression;
		mainOperation.GetState(&currentProgression);
		currentProgression*=100;
		return currentProgression;
	}

protected:
	virtual void DoShowProgression(float newProg)
	{
		std::cout.precision(4);
		std::cout<<"#"<<newProg<<std::endl;
	}
};


#endif
