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

#ifndef _HBIN
#define _HBIN

#include <fstream>
#include <vector>

#include "Mathlib.h"
#include "manager/smart_ptr.h"
#include "manager/stringTools.h"
#include "Model3D.h"
/**
 * \brief Implémentation de l'interpréteur de fichier modèle Bin (*.bin) 
 */
namespace formatBIN
{

/*! \file bin.h
    \brief Implentation de l'interpreteur de fichier Binaire SPPS
*/

/*! 
	Cette structure est utilisée pour l'échange entre la classe bin.h et CObjet3D
*/
struct tIndices {
    unsigned short a, b, c, bVisible;      
};


/*! 
	Cette structure est utilisée pour l'échange entre la classe bin.h et CObjet3D
*/
struct tFace {
    int vertIndex[3];	/**< Indice des vertex dans _pVertices */  
	vec3 faceNormals;	/**< Indice des vecteurs normales dans _pNormals */  
    int coordIndex[3];	/**< Coordonnées de l'application de la texture sur la face */  
	bool diff[3];		/**< Indique lesquels des trois coté ne pas afficher en mode filaire contour seulement */  
	bool internalFace;  /**< Si cette face est une face interne du modèle */
	short materialID;	/**< Numéro de matériau associé à la face */  
	ivec3 vertexTex;	/**< Coordonnées de textures*/
	tFace() { internalFace=false; }
};

/*! 
	Cette structure est utilisée pour l'échange entre la classe bin.h et CObjet3D
*/
struct tMaterialInfo 
{ 
    char  strName[255];			/**< Nom du matériau */        
    char  strFile[255];         /**< Nom du fichier de matériau */ 
    vec4 colorA;				/**< Couleur du composant A */
    vec4 colorD;				/**< Couleur du composant D */         
    vec4 colorS;				/**< Couleur du composant S */         
    float Shininess;			/**< Valeur de brillance */
	float Transparency;			/**< Valeur de transparance */
    unsigned int  textureId; 	/**< Identifiant de la texture */            
    float uTile;        		/**< Modification de coordonnées de texture */          
    float vTile;        		/**< Modification de coordonnées de texture */                
    float uOffset;        		/**< Modification de coordonnées de texture */               
    float vOffset;        		/**< Modification de coordonnées de texture */         
} ;


/*! 
	Cette structure est utilisée pour l'échange entre la classe bin.h et CObjet3D
*/
struct t3DObject 
{
    int  numOfFaces;    			/**< Nombre de face dans cet objet (groupe) */        
    int  materialID; 				/**< Numéro de matériau associé au groupe */           
    bool bHasTexture;				/**< Indique si le groupe a une texture associée */ 
	bool bHasmaterialID;			/**< Indique si le groupe a un matériau associée */ 
    char strName[255];				/**< Nom du groupe */ 
    smart_ptr_ar<tFace> pFaces;	/**< Liste des faces */ 
};

/*! 
	Cette structure est utilisée pour l'échange entre la classe bin.h et CObjet3D
*/
struct t3DModel 
{                
	int numOfLights;    					/**< Nombre de lumiéres associée à la scène */ 
	unsigned long numOfVerts;    			/**< Nombre de vertices présent dans la scène */ 
	unsigned long numOfTexCoords;    			/**< Nombre de coordonnées de textures présent dans la scène */ 
    std::vector<tMaterialInfo> pMaterials;	/**< Liste de matériaux dans ce modèle*/ 
	std::vector<t3DObject> pObject;			/**< Liste des objets composant la scène */   
    smart_ptr_ar<vec3> pVerts;			/**< Liste des coordonnées composant la scène */ 
    smart_ptr_ar<vec3> pNormals;			/**< Liste des normales composant la scène */ 
    smart_ptr_ar<vec2> pTexVerts;  		/**< Liste des coordonnées de textures composant la scène */       
};

	//Types du fichier
	typedef unsigned short bShort;
	typedef unsigned int bInt;
	typedef int bsInt;
	typedef char bString;
	typedef float bFloat;
	typedef long bLong;
	typedef long bsLong;
	typedef bool bBool;
/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryNode {
	bShort nodeType;	/**< Type de noeud */
	std::size_t firtSon;		/**< Adresse de fichier pour le premier fils du noeud */
	std::size_t  nextBrother;	/**< Adresse de fichier pour le prochain frère */
};

/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryVertices {
	bLong nbVertex;		/**< Nombre de vertices dans la liste */
};

/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryVertex {
	bFloat x;		/**< Valeur de position du vertex */
	bFloat y;		/**< Valeur de position du vertex */
	bFloat z;		/**< Valeur de position du vertex */
};

/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryTexCoords {
	bLong nbTexCoords;		/**< Nombre de coordonnées de textures dans la liste */
};
/*! 
	\brief Cette structure définit les coordonnées 2d des textures
*/
struct binaryTexCoord {
	bFloat x;		/**< Valeur de position du vertex */
	bFloat y;		/**< Valeur de position du vertex */
};
/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryGroup {
	bString groupName[255];		/**< Nom du groupe */
	bInt nbFace;				/**< Nombre de faces composant le groupe */
};
/*! 
	\brief Version 1.00 de la face d'un triangle
*/
struct binaryFace100 {
	bLong a;			/**< Indice du vertex d'un des points composant le triangle */
	bLong b;			/**< Indice du vertex d'un des points composant le triangle */
	bLong c;			/**< Indice du vertex d'un des points composant le triangle */
	bShort idMaterial;	/**< Indice du matériau de rendu 3D du triangle binaryMaterial */
	bBool ab;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bBool bc;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bBool ca;			/**< En mode filaire contour indique si ce coté doit être dessiné */
};

/*! 
	\brief Version 1.10 Cette structure définit une section de données du fichier Binaire
*/
struct binaryFace110 {
	bLong a;			/**< Indice du vertex d'un des points composant le triangle */
	bLong b;			/**< Indice du vertex d'un des points composant le triangle */
	bLong c;			/**< Indice du vertex d'un des points composant le triangle */
	bShort idMaterial;	/**< Indice du matériau de rendu 3D du triangle binaryMaterial */
	bBool ab;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bBool bc;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bBool ca;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bsLong coordtex[3];		/**< Coordonnées de textures au sommet */
};
/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryFace {
	bLong a;			/**< Indice du vertex d'un des points composant le triangle */
	bLong b;			/**< Indice du vertex d'un des points composant le triangle */
	bLong c;			/**< Indice du vertex d'un des points composant le triangle */
	bShort idMaterial;	/**< Indice du matériau de rendu 3D du triangle binaryMaterial */
	bBool ab;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bBool bc;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bBool ca;			/**< En mode filaire contour indique si ce coté doit être dessiné */
	bBool internalFace;	/**< Si cette face est une face interne du modèle */
	bsLong coordtex[3];	/**< Coordonnées de textures au sommet */
};

/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryHeader{
	bInt majorVersion; 	/**< Version du format */
	bInt minorVersion; 	/**< Version du format */
};

/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryColor {
	bFloat red;		 		/**< Valeur de la composante de couleur */
	bFloat green;	 		/**< Valeur de la composante de couleur */
	bFloat blue;	 		/**< Valeur de la composante de couleur */
	bFloat transparency;	/**< Valeur de transparence de couleur */
};

/*! 
	\brief Cette structure définit une section de données du fichier Binaire de la version 1.00
*/
struct binaryMaterialv100 {
	bString materialName[255];		/**< Nom du matériau */
	bFloat shine;		 			/**< Valeur de brillance */
	bFloat transparency; 			/**< Valeur de transparence */
	binaryColor colorA;	 			/**< Valeur de la composante de couleur AMBIANT */
	binaryColor colorB;	 			/**< Valeur de la composante de couleur DIFFUSE*/
	binaryColor colorC;	 			/**< Valeur de la composante de couleur SPECULAR*/
};
/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryMaterial {
	bString materialName[255];		/**< Nom du matériau */
	bFloat shine;		 			/**< Valeur de brillance */
	bFloat transparency; 			/**< Valeur de transparence */
	binaryColor colorA;	 			/**< Valeur de la composante de couleur AMBIANT */
	binaryColor colorB;	 			/**< Valeur de la composante de couleur DIFFUSE*/
	binaryColor colorC;	 			/**< Valeur de la composante de couleur SPECULAR*/
	bString materialFileName[255];	/**< Nom du fichier de texture du matériau */
};

/*! 
	\brief Cette structure définit une section de données du fichier Binaire
*/
struct binaryTexture {
	bString textureFilename; 		/**< Chemin du fichier de texture */
};

/**
 *	\class CformatBIN 
 *	\brief Classe de chargement de fichier binaire de modèle SPPS
 *	@see CObjet3D::Save()
 *	@see CObjet3D::Load()
 */
class CformatBIN 
{
public: 
	
	/**
	 * Constructeur
	 */
    CformatBIN();
	/**
	 * Destructeur
	 */
	~CformatBIN();
	/**
	 * Méthode d'importation d'un fichier binaire
	 * @param pModel Structure de données où stocker le modèle à charger
	 * @param strFileName Chemin du fichier
	 * @return Vrai si l'opération a réussi
	 */
    bool ImportBIN(t3DModel *pModel, const char *strFileName);
	/**
	 * Méthode d'exportation d'un modèle 3D
	 * @param strFileName Chemin du fichier
	 * @param UnitizeVar Vecteur 4d redimensionnement et de translation du modèle
	 * @param _pVertices Tableau de points
	 * @param _pTexCoords Tableau de vecteurs de textures
	 * @param _pGroups Tableau de groupes
	 * @return Vrai si l'opération a réussi
	 */
	bool ExportBIN(const char *strFileName,vec4 UnitizeVar,std::vector<vec3> &_pVertices,std::vector<vec2> &_pTexCoords,std::vector<SGroup3D> &_pGroups);
private:
	enum NODE_TYPE{
		NODE_TYPE_VERTICES,
		NODE_TYPE_GROUP,
		NODE_TYPE_MATERIAL,
		NODE_TYPE_TEXTURE,
		NODE_TYPE_TEXCOORDS
	};
	/**
	 * Lecture d'un noeud Binaire
	 */
	bool ProcessNode(std::fstream &binFile,t3DModel *pModel,const binaryHeader& fileHeader);
	/**
	 * Lecture d'un noeud Binaire
	 */
	bool ProcessNodeVertices(std::fstream &binFile,t3DModel *pModel);
	/**
	 * Lecture d'un noeud Binaire
	 */
	bool ProcessNodeTexCoords(std::fstream &binFile,t3DModel *pModel);
	/**
	 * Lecture d'un noeud Binaire
	 */
	bool ProcessNodeMaterial(std::fstream &binFile,t3DModel *pModel);
	/**
	 * Lecture d'un noeud Binaire v100
	 */
	bool ProcessNodeMaterialv100(std::fstream &binFile,t3DModel *pModel);
	/**
	 * Lecture d'un noeud Binaire
	 */
	bool ProcessNodeGroup(std::fstream &binFile,t3DModel *pModel); 
	/**
	 * Lecture d'un noeud Binaire version 1.00
	 */
	bool ProcessNodeGroupv100(std::fstream &binFile,t3DModel *pModel); 

	/**
	 * Lecture d'un noeud Binaire version 1.00
	 */
	bool ProcessNodeGroupV110(std::fstream &binFile,t3DModel *pModel); 
	/**
	 * Ecriture d'un noeud Binaire
	 */
	void writeNode(std::fstream &binFile,bShort nodeType,bLong nodeHeadSize,bLong nodeSize);

	vec4 GetVectorFromColor(binaryColor colorInfo);
	binaryColor GetColorFromVector(vec4 colorInfo);
	/**
	 * Calcul des vecteurs de normales du modèle
	 */
	void ObjCalcNormals(t3DModel *p_object);
    void ComputeNormals(t3DModel *pModel);
	void Clean();
};


} //namespace
#endif // _H3DS
