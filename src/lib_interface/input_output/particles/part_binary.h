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

#include "Core/mathlib.h"
/*!
	@file part_binary.h
	\brief Cette structure définit l'entete de fichier de particules
*/
#ifndef __PARTICLE_BINARY__
#define __PARTICLE_BINARY__

#define PARTICLE_BINARY_VERSION_INFORMATION 1 /*!< Version du format de fichier de particules incrementer la version en cas de modification des structures */
#define bpLong unsigned long
#define bpFloat float
#define bpUShort unsigned short

/*!
	\brief Cette structure définit l'entete de fichier de particules
*/
struct binaryFHeader {
	bpLong nbParticles;					/*!< Nombre de particules dans le fichier */
	bpLong formatVersion;				/*!< Version du format */
	bpLong fileInfoLength;				/*!< Taille en octet de l'information du fichier d'entete*/
	bpLong particleInfoLength;			/*!< Taille en octet d'une information de pas de temps */
	bpLong particleHeaderInfoLength;	/*!< Taille en octet d'une information de particule */
	bpLong nbTimeStepMax;				/*!< Nombre de pas de temps maximum, utilisé comme indice pour la réservation d'espace mémoire lors du chargement du fichier */
	bpFloat timeStep;					/*!< Pas de temps de la simulation */
};

/*!
	\brief Cette structure définit l'entete de fichier de particules
*/
struct binaryPHeader {
	binaryPHeader(){};
	binaryPHeader(const bpLong &_nbTimeStep,const bpUShort& _firstTimeStep) : nbTimeStep(_nbTimeStep),firstTimeStep(_firstTimeStep) {}
	bpLong nbTimeStep;			/*!< Nombre de pas de temps effectif de la particule (nombre de lignes pour cette particule) */
	bpUShort firstTimeStep; /*!< Numero du premier pas de temps */
};


/*!
	\brief Cette structure définit l'entete de fichier de particules
*/
struct binaryPTimeStep {
	binaryPTimeStep(){}
	binaryPTimeStep(const vec3& _pos,const bpFloat &_energy) : position(_pos),energy(_energy) {}
	vec3 position;			/*!< Position de la particule */
	bpFloat energy;			/*!< Energie de la particule */
};
#endif
