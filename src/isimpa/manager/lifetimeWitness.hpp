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

#include "first_header_include.hpp"

#include "manager/smart_ptr.h"
/*
 * @file lifetimeWitness.hpp
 * Cette classe permet de lier la fin d'un objet à une liste d'événements.
 *
 */
#ifndef __LIFETIME_WITNESS__
#define __LIFETIME_WITNESS__

struct t_outsideOperator
{
	virtual ~t_outsideOperator(){}
};

struct t_baseEvtCatcher
{
	virtual void OnModified(const int& elid){}
	virtual void OnNewChildElement(const int& childType,const int& parentid,const int& childid){}
	virtual ~t_baseEvtCatcher(){}
};
typedef smart_ptr<t_outsideOperator> LifeTimeWitness;
typedef smart_ptr<t_baseEvtCatcher> EventCatcher;

#endif