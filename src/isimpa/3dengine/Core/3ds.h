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
#include <stdio.h>
#include "Mathlib.h"
#include <vector>
#include "manager/smart_ptr.h"

#ifndef _H3DS
#define _H3DS


/*! \file 3ds.h
    \brief Implémentation de l'interpréteur de fichier modèle 3D Studio (*.3ds)
*/


/*! \brief Implémentation de l'interpréteur de fichier modèle 3D Studio (*.3ds)
*/
namespace format3DS
{


/*!
	Cette structure est utilisée pour l'échange entre la classe 3ds.h et CObjet3D
*/
struct tIndices {
    unsigned short a, b, c, bVisible;
};


/*!
	Cette structure est utilisée pour la lecture du fichier 3DS
*/
struct tChunk {
    unsigned short int ID;
    unsigned int length;
    unsigned int bytesRead;
};


/*!
	Cette structure est utilisée pour l'échange entre la classe 3ds.h et CObjet3D
*/
struct tFace {
    int vertIndex[3];  	/**< Indice des vertex dans _pVertices */
	vec3 faceNormals;	/**< Indice des vecteurs normales dans _pNormals */
    int coordIndex[3];	/**< Coordonnées de l'application de la texture sur la face */
	bool diff[3];		/**< Affichage ou non des cotées du triangles */
	short materialID; 	/**< Numéro de matériau associé à la face */
};

/*!
	Cette structure est utilisée pour l'échange entre la classe 3ds.h et CObjet3D
*/
struct tMaterialInfo
{
    char  strName[255];				/**< Nom du matériau */
    char  strFile[255];				/**< Nom du fichier de matériau */
    unsigned char colorA[3];		/**< Couleur du composant A */
    unsigned char colorD[3];		/**< Couleur du composant D */
    unsigned char colorS[3];		/**< Couleur du composant S */
    unsigned char Shininess[1];		/**< Valeur de brillance */
	unsigned char Transparency[1];	/**< Valeur de transparance */
    unsigned int  texureId;  		/**< Identifiant de la texture */
    float uTile;        			/**< Modification de coordonnées de texture */
    float vTile;        			/**< Modification de coordonnées de texture */
    float uOffset;        			/**< Modification de coordonnées de texture */
    float vOffset;        			/**< Modification de coordonnées de texture */
} ;

/*!
	Cette structure est utilisée pour l'échange entre la classe 3ds.h et CObjet3D
*/
struct t3DObject
{
    int  numOfVerts;				/**< Nombre de points dans cet objet (groupe) */
    int  numOfFaces;				/**< Nombre de face dans cet objet (groupe) */
    int  numTexVertex;				/**< Nombre d'indice de textures dans cet objet (groupe) */
    int  materialID;				/**< Numéro de matériau associé au groupe */
    bool bHasTexture;				/**< Indique si le groupe a une texture associée */
	bool bHasmaterialID;			/**< Indique si le groupe a un matériau associée */
    char strName[255];				/**< Nom du groupe */
    smart_ptr_ar<vec3> pVerts;		/**< Liste des points */
    smart_ptr_ar<vec3> pNormals;	/**< Liste des Normales */
    smart_ptr_ar<vec2> pTexVerts;	/**< Liste des position de textures */
    smart_ptr_ar<tFace> pFaces;		/**< Liste des faces */
};

/*!
	Cette structure est utilisée pour l'échange entre la classe 3ds.h et CObjet3D
*/
struct t3DModel
{
	t3DModel()
	{
		numOfObjects=0;
		numOfMaterials=0;
		numOfLights=0;
	}
    int numOfObjects;						/**< Nombre d'objets associée à la scène (pObject->size()) */
    int numOfMaterials;						/**< Nombre de matériaux associée à la scène */
	int numOfLights;						/**< Nombre de lumiéres associée à la scène */
    std::vector<tMaterialInfo> pMaterials;	/**< Liste de matériaux dans ce modèle*/
	std::vector<t3DObject> pObject;			/**< Liste des objets composant la scène */
};


/**
 *	\class CLoad3DS
 *	\brief Classe de chargement de fichier 3DS
 */
class CLoad3DS
{
public:
	/**
	 * Constructeur
	 */
    CLoad3DS();
	/**
	 * Destructeur
	 */
	~CLoad3DS();
	/**
	 * Méthode d'importation d'un fichier 3ds
	 * @param pModel Structure de données où stocker le modèle à charger
	 * @param strFileName Chemin du fichier
	 * @return Vrai si l'opération a réussi
	 */
    bool Import3DS(t3DModel *pModel, const char *strFileName);

private:
    int GetString(char *);
    void ReadChunk(tChunk *);
    void ProcessNextChunk(t3DModel *pModel, tChunk *);
    void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);
    void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);
    void ReadColorChunk(unsigned char *Color, tChunk *pChunk);
	void ObjCalcNormals(t3DObject *p_object);
    void ReadVertices(t3DObject *pObject, tChunk *);
    void ReadVertexIndices(t3DObject *pObject, tChunk *);
    void ReadUVCoordinates(t3DObject *pObject, tChunk *);
    void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);
    void ComputeNormals(t3DModel *pModel);
	void Clean();

    FILE *m_FilePointer;
    //tChunk *m_CurrentChunk;
    tChunk *m_TempChunk;
};


} //namespace
#endif // _H3DS
