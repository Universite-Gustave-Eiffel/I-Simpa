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

#ifndef _HVAR
#define _HVAR

/*! \file var.h
 *  \brief Exportation du fichier de contrainte tetgen
 */
#include "Mathlib.h"
#include <vector>

/*!
 *    \brief Exportation du fichier de contrainte tetgen
 */
namespace formatVAR
{

	/**
	 * @brief Structure de données de contrainte du maillage
	 */
	struct t_facet_constraint
	{
		unsigned int boundaryMarker;
		float areaConstraint;
		t_facet_constraint(){};
		t_facet_constraint(unsigned int _boundaryMarker , float _areaConstraint ):boundaryMarker(_boundaryMarker),areaConstraint(_areaConstraint){}

	};

	/**
	 * @brief Structure de données englobant toutes les contraintes
	 *
	 */
	struct t_file
	{
		std::vector<t_facet_constraint> facetsConstraints;
	};

/**
 *	\class Cvar  
 *	\brief Exportation du fichier de contrainte tetgen
 */
class CVar
{
public:

	/**
	 * Constructeur
	 */
	CVar();
	/**
	 * Méthode d'exportation d'un modèle 3D
	 */
	bool BuildVar(t_file& sceneconst,const char* mfilename);
private:
	enum {FILENAMESIZE = 1024};

};





} //Fin namespace
#endif