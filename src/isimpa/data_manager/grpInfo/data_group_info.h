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

#include "first_header_include.hpp"

#ifndef __MBIN__
#define __MBIN__

/*! \file data_group_info.h
    \brief Import export d'une liste de face
*/

/**
 * \brief Import export d'une liste de face
 */
namespace formatGRPINFO
{

//Types Import/Export

typedef unsigned short bCourt;
typedef int Intb;
typedef float Floatb;
typedef unsigned long Longb ;
typedef bool Boolb ;
/**
 * @brief Structure d'entete du fichier
 */
struct t_FileHeader
{
	Longb quantFace;
};
/**
 * @brief Structure d'une face
 */
struct binface
{
	Intb group;
	Intb face;
};


/**
 * @brief Structure regroupant les informations du fichier
 */
struct exchangeMainData
{
	t_FileHeader header;
	binface* tabFaces;
	~exchangeMainData(){ delete[] tabFaces;}
	exchangeMainData(){ tabFaces=(binface*)0;}
};

/**
 *	\brief Classe de sauvegarde et de chargement des groupes de faces
 */
class DGIBIN
{
public: 
	/** Constructeur
	 */
	DGIBIN(){}
	/** Destructeur
	 */
	~DGIBIN(){}
	/**
	 * Méthode d'importation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @param exchangeMainData informations du fichier
	 * @return Vrai si l'opération a réussi
	 */
    bool ImportBIN(const char *strFileName,exchangeMainData &mainData);
	/**
	 * Méthode d'exportation d'un modèle 3D
	 * @param strFileName Chemin du fichier
	 * @param exchangeMainData informations du fichier
	 * @return Vrai si l'opération a réussi
	 */
	bool ExportBIN(const char *strFileName,exchangeMainData &mainData);

};


} //namespace
#endif
