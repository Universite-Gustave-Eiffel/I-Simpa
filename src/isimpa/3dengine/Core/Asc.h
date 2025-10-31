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

#ifndef _HASC
#define _HASC

#include "Mathlib.h"
#include <vector>
#include "manager/smart_ptr.h"
#include "manager/stringTools.h"
#include "Model3D.h"

/*! \brief Implémentation de l'interpréteur de fichier modèle 3D Ascii (*.asc)
*/
namespace formatASC
{

/*! \file Asc.h
    \brief Implémentation de l'interpréteur de fichier modèle 3D Ascii (*.asc)
*/

	
/*! 
	Cette structure est utilisée pour l'échange entre la classe Asc.h et CObjet3D
*/
struct tIndices {
    unsigned short a, b, c, bVisible;      
};

/*! 
	Cette structure est utilisée pour l'échange entre la classe Asc.h et CObjet3D
*/
struct tFace {
    int vertIndex[3];	/**< Indice des vertex dans _pVertices */  
	vec3 faceNormals;	/**< Indice des vecteurs normales dans _pNormals */  
    int coordIndex[3];	/**< Coordonnées de l'application de la texture sur la face */ 
};

/*! 
	Cette structure est utilisée pour l'échange entre la classe Asc.h et CObjet3D
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
	Cette structure est utilisée pour l'échange entre la classe Asc.h et CObjet3D
*/
struct t3DObject 
{
    int  numOfVerts;				/**< Nombre de points dans cet objet (groupe) */            
    int  numOfFaces;				/**< Nombre de face dans cet objet (groupe) */               
    int  numTexVertex;         
    int  materialID;				/**< Numéro de matériau associé au groupe */              
    bool bHasTexture;				/**< Indique si le groupe a une texture associée */ 	
	bool bHasmaterialID;			/**< Indique si le groupe a un matériau associée */ 
    char strName[255];				/**< Nom du groupe */ 
    smart_ptr_ar<vec3> pVerts;	/**< Liste des points */ 
    smart_ptr_ar<vec3> pNormals;	/**< Liste des Normales */        
    smart_ptr_ar<vec2> pTexVerts;	/**< Liste des position de textures */       
    smart_ptr_ar<tFace> pFaces;	/**< Liste des faces */             
};

/*! 
	Cette structure est utilisée pour l'échange entre la classe Asc.h et CObjet3D
*/
struct t3DModel 
{
    int numOfObjects;                  
    int numOfMaterials; 
	int numOfLights;						/**< Nombre de lumiéres associée à la scène */ 
    std::vector<tMaterialInfo> pMaterials;	/**< Liste de matériaux dans ce modèle*/ 
	std::vector<t3DObject> pObject;			/**< Liste des objets composant la scène */   
};

/**
 *	\class CAsc 
 *	\brief Classe de chargement de fichier ASC
 */
class CAsc
{
public:


	/**
	 * Constructeur
	 */
	CAsc();
	
	/**
	 * Méthode d'exportation d'un modèle 3D
	 * @param mfilename Chemin du fichier
	 * @param UnitizeValue Vecteur 4d redimensionnement et de translation du modèle
	 * @param Vertices Tableau de points
	 * @param pModel Tableau de groupes
	 * @param nbVertices Nombre de points
	 * @param nbFaces Nombre de faces
	 */
	void ExportASC(vec4 UnitizeValue,std::vector<SGroup3D> &pModel, std::vector<vec3> &Vertices, const char *mfilename,long nbVertices, long nbFaces);
	/**
		Exportation du maillage, non utilisé
	*/
	void ExportTetraASC(vec4 UnitizeValue,triangleFace *tabVertexMaillage, const char *mfilename, long nbFaces);
	/**
		Importation du maillage, non utilisé
	*/
	void ImportTetraASC(triangleFace *tabVertexMaillage, long nbFaces, const char *strFileName);
	
	/**
	 * Méthode d'importation d'un fichier asc
	 * @param pModel Structure de données où stocker le modèle à charger
	 * @param strFileName Chemin du fichier
	 * @return Vrai si l'opération a réussi
	 */
	bool ImportASC(t3DModel *pModel, const char *strFileName);
private:
	enum {FILENAMESIZE = 1024};

};





} //Fin namespace
#endif