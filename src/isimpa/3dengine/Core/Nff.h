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

#ifndef _HNFF
#define _HNFF

/*! \file Nff.h
    \brief Implémentation de l'interpréteur de fichier modèle Nff (*.nff)
 */
#include "Mathlib.h"
#include <vector>
#include "manager/smart_ptr.h"
#include "Model3D.h"

/*! \brief Implémentation de l'interpréteur de fichier modèle Nff (*.nff)
 */
namespace formatNFF
{

/**
 *	\class CNff 
 *	\brief Classe de chargement de fichier modèle Nff (*.nff)
 */
class CNff
{
public:

	/**
	 * Constructeur
	 */
	CNff();
	/**
	 * Méthode d'exportation d'un modèle 3D
	 * @param mfilename Chemin du fichier
	 * @param UnitizeValue Vecteur 4d redimensionnement et de translation du modèle
	 * @param Vertices Tableau de points
	 * @param pModel Tableau de groupes
	 * @param nbVertices Nombre de points
	 * @param nbFaces Nombre de faces
	 */
	void ExportNFF(vec4 UnitizeValue,std::vector<SGroup3D> &pModel, std::vector<vec3> &Vertices, const char *mfilename,long nbVertices, long nbFaces);
	/**
	 * Méthode d'exportation du maillage 3D
	 * @param mfilename Chemin du fichier
	 * @param UnitizeValue Vecteur 4d redimensionnement et de translation du modèle
	 * @param tabVertexMaillage Tableau de triangles
	 * @param nbFaces Nombre de faces
	 */
	void ExportTetraNFF(vec4 UnitizeValue,triangleFace *tabVertexMaillage, const char *mfilename, long nbFaces);
private:
	enum {FILENAMESIZE = 1024};

};





} //Fin namespace
#endif