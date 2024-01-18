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

#include <memory>
#include <string.h>
#include <stdint.h>

#ifndef __RSBIN__
#define __RSBIN__

/*! \file rsbin.h
    \brief Import export de l'animation des recepteurs surfaciques
*/

/**
 * \brief Import export de l'animation des recepteurs surfaciques
 */
namespace formatRSBIN
{

typedef unsigned short bCourt;
typedef int32_t Intb;
typedef float Floatb;
typedef uint32_t Longb ;
typedef bool Boolb ;
typedef char Charb;

const uint32_t VERSION=3; //Version à incrémenter à chaque modification de la structure, les anciennes version du logiciels et les nouvelles pourront toujours lire les fichiers
const uint32_t STRING_SIZE=255;
//Types Import/Export




/**
 * @brief Type d'enregistrement de ce récepteur de surface
 */
enum RECEPTEURS_RECORD_TYPE
{
	RECEPTEURS_RECORD_TYPE_SPL_STANDART,		/*!< Enregistrement standart (dB)*/
	RECEPTEURS_RECORD_TYPE_SPL_GAIN,			/*!< Différence entre deux enregistrement standart (dB)*/
	RECEPTEURS_RECORD_TYPE_TR,					/*!< Cartographie du temps de réverbération (secondes)*/
	RECEPTEURS_RECORD_TYPE_EDT,					/*!< Cartographie du Temps de décroissance sur les 10 premiers dB (secondes*/
	RECEPTEURS_RECORD_TYPE_PRESSURE,			/*!< Cartographie de pression négatif permit (pascal)*/
	RECEPTEURS_RECORD_TYPE_CLARITY,				/*!< Cartographie du paramètre C(dB)*/
	RECEPTEURS_RECORD_TYPE_DEFINITION,			/*!< Cartographie du paramètre D(%)*/
	RECEPTEURS_RECORD_TYPE_TS,					/*!< Cartographie du paramètre TS(secondes)*/
	RECEPTEURS_RECORD_TYPE_ST,					/*!< Cartographie du paramètre ST(dB)*/
	RECEPTEURS_RECORD_TYPE_STI					/*!< Cartographie du paramètre STI*/
};



/**
 * @brief Structure de données d'un noeud
 */
struct t_nodesPosition
{
	Floatb node[3];
};

/**
 * @brief Structure d'un récepteur de surface
 */
struct t_RecepteurS
{
	Intb xmlIndex;
	Intb quantFaces;
	Charb recepteurSName[STRING_SIZE];
};

/**
 * Structure de données d'une face
 */
struct t_FaceRS
{
	Intb sommetsIndex[3];
	Intb nbRecords; /*!< Nombre de pas de temps où il y a eu un enregistrement de collision */
};



/**
 * @brief Structure d'une valeur
 *
 */
struct t_faceValue
{
	t_faceValue():energy(0){}
	t_faceValue( 	bCourt _timeStep, Floatb _energy):timeStep(_timeStep),energy(_energy){}
	bCourt timeStep;
	Floatb energy;
};

/**
 * Structures d'echange avec l'exterieur
 */



struct t_ExchangeData_Face
{
	t_FaceRS dataFace;
	t_faceValue* tabTimeStep;

	t_ExchangeData_Face(){ tabTimeStep=NULL; }
	~t_ExchangeData_Face(){ delete[] tabTimeStep; };

};


struct t_ExchangeData_Recepteurs
{
	t_RecepteurS dataRec;
	t_ExchangeData_Face* dataFaces;

	t_ExchangeData_Recepteurs(){ dataFaces=NULL; memset(dataRec.recepteurSName,0,sizeof(Charb)*STRING_SIZE);}
	~t_ExchangeData_Recepteurs(){ delete[] dataFaces; };
};
/**
 *
 */
struct t_ExchangeData
{
	RECEPTEURS_RECORD_TYPE recordType; /*!< Type d'enregistrement de ce récepteur de surface */
	t_nodesPosition* tabNodes;
	Intb tabNodesSize;
	t_ExchangeData_Recepteurs* tabRs;
	Intb tabRsSize;
	Intb nbTimeStep;
	Floatb timeStep;
	t_ExchangeData(){ tabNodes=NULL;tabRs=NULL; recordType=RECEPTEURS_RECORD_TYPE_SPL_STANDART;}
	~t_ExchangeData(){ delete[] tabNodes;delete[] tabRs; };
};

/**
 *	\brief Classe de sauvegarde et de chargement du maillage tétrahedrique
 */
class RSBIN
{
public:
	/** Constructeur
	 */
	RSBIN(){}
	/** Destructeur
	 */
	~RSBIN(){}

	/**
	 * Lit uniquement l'entête du fichier
	 * @param strFileName Chemin du fichier
	 * @param mainData Données du fichier
	 * @return Vrai si l'opération a réussi
	 */
    bool ReadHeaderBIN(const char *strFileName,t_ExchangeData& mainData);

	/**
	 * Méthode d'importation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @param mainData Données du fichier
	 * @return Vrai si l'opération a réussi
	 */
    bool ImportBIN(const char *strFileName,t_ExchangeData& mainData);
	/**
	 * Méthode d'exportation d'un modèle 3D
	 * @param strFileName Chemin du fichier
	 * @param mainData Données du fichier
	 * @return Vrai si l'opération a réussi
	 */
	bool ExportBIN(const char *strFileName,t_ExchangeData& mainData);

};


} //namespace
#endif
