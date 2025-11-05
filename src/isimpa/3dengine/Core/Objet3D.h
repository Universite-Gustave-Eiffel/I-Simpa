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

#ifndef _Objet3D_H
#define _Objet3D_H

/*! \file Objet3D.h
    \brief Déclaration de la classe CObjet3D
    
    Voir la classe CObjet3D
	@see CObjet3D()
*/

#ifdef _WINDOWS
#define _WINSOCKAPI_
#include <windows.h>
#endif
#include "Material.h"
#include "Texture.h"
#include "Mathlib.h"
#include <wx/string.h>

#include <input_output/bin.h> // gestion du format binaire pour le coeur de calcul spps
#include <input_output/importExportMaillage/mbin.h> //gestion de l'exportation,importation du maillage

#include "Model3D.h"
#include "3dengine/tools/extruder.h"

/**
 *  \class CObjet3D 
 * 	\brief Classe de rendu 3D de la scène
 *	
 *  Cette classe contient l'objet représentant la scène 3D (Le modèle).
 *  Cette classe contient des méthodes permettant d'effectuer des opérations sur cette objet et à obtenir des informations à son sujet.
 *	L'instanciation peut se faire de deux facons, soit en lui donnant le nom d'un fichier de modèle ayant un format compatible
 *	soit en indiquant les dimensions de la scène cuboïde.
 *	@see OpenGLApp()
 *	
 */
class CObjet3D 
{
public:

	/**
	 * Constructeur
	 */
	CObjet3D(); 
	/**
	 * Destructeur
	 */
	~CObjet3D();
	
	/**
	 * Les listes d'insctruction OpenGL doivent elles êtres initialisées
	 * @return Vrai si la méthode BuildFont doit être appelé avant d'afficher le texte dans la fentre openGL
	 */
	bool IsFontNeedBuilding();
	/**
	 * Initialise les listes OpenGL avec la police de caractère "Courier New" (non paramétrable pour l'instant)
	 * @param vhDC Windows Only, Drawing context HDC
	 */
	void BuildFont(void* vhDC=NULL);
	/**
	 * Décharge les listes OpenGL de génération de caractères
	 */
	void KillFont();
	/**
	 * Execute les instructions OpenGL afin de générer une chaîne de caractère dans la vue 3D
	 * @param pos Position x,y,z de la chaîne de caractère (position du premier caractère en partant de la gauche)
	 * @param color Couleur de rendu des caractère
	 * @param fmt Chaîne de caractères, (compréhension des formalisations tel la méthode printf)
	 */
	void glPrint(vec3 pos,vec3 color,const char *fmt, ...);
	/**
	 * Supprimer le modèle de la scène courante et charge le fichier de modèle
	 * @param filename Chemin et nom du fichier de modèle compatible
	 * @return Vrai si le chargement du fichier s'est déroulé avec succès
	 */
	bool Load(const std::string& filename, double scale);

	/**
	 * Charge le fichier de modèle en conservant les propriété de groupe et de matériaux du modèle déjà chargé
	 * @param filename Chemin et nom du fichier de modèle compatible
	 */
	void LoadPolyWithoutLostCurrentModelGroupAndMaterials(const std::string& filename);
	/**
	 * Supprimer le modèle de la scène courante et construire un cuboïde comme nouvelle scène
	 * @param debCuboide Position x,y,z du coin inférieur du volume
	 * @param finCuboide Position x,y,z du coin supérieur du volume
	 */
	bool BuildModel(vec3 debCuboide,vec3 finCuboide);
	/**
	 * Exporte la scène vers un fichier de modèle
	 * @param filename Chemin et nom du fichier de modèle (le format est défini par l'extension)
	 * @return Vrai si l'exportation du modèle s'est déroulée avec succès
	 */
	bool Save(const wxString& filename);
	/**
	 * Exporte la scène vers le format de structure de données CBIN
	 * @param[out] modelExport Structure de donnée de CBIN
	 */	
	void ToCBINFormat(formatCoreBIN::ioModel& modelExport);
	/**
	 * Construit le fichier .var de contrainte des faces. A partir des faces associées aux récepteur surfacique définit une surface de tetrahèdre maximal en contact.
	 * @param filename Chemin et nom du fichier de maillage
	 * @param faceAreaContraint Contrainte de surface des tetrahèdres en m²
	 */
	bool BuildVarConstraintFile(const wxString& filename, float faceAreaContraint);
	/**
	 * Exporte le maillage vers un fichier au format MBIN
	 * @param filename Chemin et nom du fichier de maillage
	 * @param toRealCoords A vrai converti les coordonées opengl vers les coordonées de la scène
	 * @return Vrai si l'exportation s'est déroulée avec succès
	 */
	bool SaveMaillage(const wxString& filename,bool toRealCoords=false);
	/**
	 * Retourne le maillage en mémoire sous la structure de données MBIN
	 */
	void GetTetraMesh(formatMBIN::trimeshmodel& trimesh,bool toRealCoords);

	/**
	 * Redimensionne et recentre le modèle courant pour un rendu 3D correct quelque soit les coordonnées du modèle
	 * @see coordsOperation::CommonCoordsToGlCoords()
	 * @see coordsOperation::GlCoordsToCommonCoords()
	 * @return Le facteur de redimensionnement du modèle
	 */
	int Unitize();
	/**
	 *	Execute les commandes OpenGl afin de donner un rendu du modèle 3D
	 * @param modeRendu Mode de rendu des faces (filaire ou en facettes)
	 * @param blendRendering Méthode de rendu des faces,Réflexion de la lumiére par vertex (false) ou par face (true)
	 * @param hideLines	Vrai si seul les traits du contour du modèle doivent être dessinées, utile seulement si modeRendu==GL_LINES
	 */
	long RenderModel(std::size_t modeRendu, bool blendRendering,bool hideLines);
	/**
	 *	Permet de connaître le nombre total de points dans la scène
	 * @return Le nombre de points (vertices) dans le modèle
	 */
	long GetNumVertices() { return this->_pVertices.size(); } 
	/**
	 *	Permet de connaître le nombre de vecteur normal dans la scène (normallement autant qu'il y a de points)
	 * @return Le nombre de normales dans le modèle
	 */
	long GetNumNormals() { return this->_pNormals.size(); } 
	/**
	 *	Permet de connaître le nombre de coordonnées de textures dans le modèle
	 * @return Le nombre de coordonnées de texture dans le modèle
	 */
	long GetNumTexCoords() { return this->_pTexCoords.size(); } 
	/**
	 *	Permet de connaître le nombre de groupes de faces dans le modèle
	 * @return Le nombre de groupes de faces dans le modèle
	 */
	long GetNumGroups() { return this->_pGroups.size(); }

	/**
	 *	Permet de connaître le nom d'un groupe de faces
	 * @param g Identifiant du groupe
	 * @return Le nom d'un groupe de faces
	 */
	std::string GetNameGroup(long g);
	/**
	 *	Permet de connaître le nombre de matériaux dans le modèle
	 * @return Le nombre de matériaux dans le modèle
	 */
	long GetNumMaterials() { return this->_nbMaterials; }
	/**
	 *	Permet de connaître le nombre de facse dans le modèle,groupe
	 * @param g Correspond au groupe de face concerné
	 * @return Le nombre de faces dans le groupe (si g>-1) sinon le nombre de faces tout groupes confondus
	 */
	long GetNumFaces(long g=-1);
	/**
	 * L'objet contient-elle une texture
	 * @return Vrai si le modèle contient une texture
	 */
	bool HasTexture() {return this->_hasTexture;}
	/**
	 * A partir du numéro de face retourne son groupe et son numéro au sein du groupe
	 * @param face Indice de la face (indice de son groupe + cumul du nombre de face des groupes 0 à NumGroupe)
	 * @return La composante Numéro de groupe, Numéro de face
	 */
	t_faceIndex FindFaceGroupWithFaceIndex(long face);
	/**
	 * Désélectionne toute les faces
	 */
	void UnselectVertex();
	/**
	 * Désélectionne la face dont l'indice est passé en paramètre
	 * @see FindFaceGroupWithFaceIndex()
	 */
	void UnselectVertex(long face);
	/**
	 * Désélectionne la face dont l'indice est passé en paramètre
	 * @see FindFaceGroupWithFaceIndex()
	 */
	void UnselectVertex(long face, long group);
	/**
	 * Désélectionne la face dont l'indice est passé en paramètre
	 * @see FindFaceGroupWithFaceIndex()
	 */
	void UnselectVertex(t_faceIndex faceid);
	/**
	 * Séléctionne la face dont l'indice est passé en paramètre
	 * @see FindFaceGroupWithFaceIndex()
	 */
	void SelectVertex(long face);
	/**
	 * Séléctionne la face dont l'indice est passé en paramètre
	 * @see FindFaceGroupWithFaceIndex()
	 */
	void SelectVertex(long face, long group);
	/**
	 * Séléctionne la face dont l'indice est passé en paramètre
	 * @see FindFaceGroupWithFaceIndex()
	 */
	bool IsSelectedFace(long face, long group);
	/**
	 * Séléctionne la face dont l'indice est passé en paramètre
	 * @see FindFaceGroupWithFaceIndex()
	 */
	void SelectVertex(const t_faceIndex& faceid);
	/**
	 * Retourne les données d'une face
	 */
	const SFace3D& GetFace(t_faceIndex faceid);
	/**
	 * Retourne les données d'un sommet
	 */
	void GetRealVertice(const unsigned long verticeId,vec3* outVert);
	/**
	 * Retourne les coordonnées du centre du triangle
	 * @param[out] center Centre de la face 
	 */
	void GetRealCenterFace(const t_faceIndex& faceid,vec3* center);

	/**
	 * Séléctionne la face qui comprend le point x,y,z sur sa surface
	 * @param x Position du point
	 * @param y Position du point
	 * @param z Position du point
	 * @return Indice de la face si trouvé. Si en dehors d'une surface retourne le couple (-1,-1)
	 */
	t_faceIndex SelectVertex(float x,float y, float z);
	/**
	 * Retourne l'indice de toutes les faces sur le même plan et ayant un sommet en commun
	 * @param firstFace Face de référence pour le test
	 * @param[out] dstLst Adresse du tableau à alimenter
	 */
	void GetAllCoplanarFaces( t_faceIndex firstFace,std::vector<t_faceIndex> &dstLst);
	/**
	 * Modifie le rendu des contours en mode filaire de facon à n'afficher que les surfaces et non la décomposition en triangles
	 * @param ResetFaceOrientation Si deux faces ayant un sommet en commun on des normal inverse alors affecter la normal du premier triangle au deuxième
	 */
	void ComputeFacesBorders(bool ResetFaceOrientation=false ); 
	/**
	 * Execute les commandes OpenGL de rendu de la sélection
	 */
	void RenderSelection();
	/**
	 * Maillage de la scène 3D
	 * @param[in] params Valeur de configuration du mailleur
	 * @return Vrai si le maillage s'est executé avec succès
	 */
	bool LoadMaillage(const std::string& facePath,const std::string& elePath,const std::string& nodePath,const std::string& neighPath);
	/**
	 * Execute les commandes OpenGL de rendu du maillage
	 * @param modeRendu GL_LINES ou GL_TRIANGLES, rendu des tétrahedres
	 * @param cutPlane Valeur du plan de coupe du maillage
	 */
	void RenderMaillage(std::size_t modeRendu,t_cutPlane cutPlane);

	//Méthodes et fonctions deséléments pouvant etre dessiné en 3D
	/**
	 *	Execute les commandes OpenGl afin de donner un rendu des éléments de la scène 3D
	 * @see E_Drawable()
	 */
	void RenderDrawableElement( int eIndex , bool blendRendering);

	int GetDrawableElementSize();

	vec4 UnitizeVar;		/**< Transformation appliqué au modèle 3D pour l'afficher correctement a l'ecran*/
	vec3 LowVar;			/**< x,y,z min avant transformation*/
	vec3 HightVar;			/**< x,y,z max avant transformation*/
	bool selectionChange;	/**< vrai si la selection a changé depuis son dernier rendu3D*/
	bool modelChange;		/**< Vrai force rechargement du modèle*/
	bool backgroundChange;	/**< Vrai redessine le fond de la scène */
	/**
	 * Calcul de la somme des triangles sélectionnés
	 * @return Surface de sélection (m²)
	 */
	float GetSelectionArea( ); 
	
	/**
	 * Calcul de l'aire de la scène
	 * @return Surface (m²)
	 */
	float GetAireScene( );

	
	void FlipSelectedFaces( );
	
	/**
	 * Effectue une projection perpandiculaire d'un rayon entre le point et la face en paramètre.
	 * @param[out] exactPosition La position exacte du point de collision.
	 * @return Vrai si il existe un point de collision.
	 */
	bool GetExactCollisionPosition( t_faceIndex faceCollision, vec3 aproximateCollisionPosition,vec3* exactPosition );
	/**
	 * Chargement du fichier face provenant de tetgen en tant que modèle
	 */
	bool _LoadFaceFile(const std::string& filename);
	bool _LoadFaceListeWithMarkers(std::vector<ivec4>& tabFace, const std::string& faceFilePath);
	/**
	 * Export au format poly supporté par tetgen avec des paramètres spéciaux
	 * @param exportUserModel A vrai les objets ajoutés par l'utilisateurs ( faces dynamiques) sont exportés dans ce fichier
	 * @param separateUserDefinedModel A vrai les objets ajoutés par l'utilisateurs ( faces dynamiques) sont exportés à la fin de ce fichier ( interprété par preprocess mais pas par tetgen )
	 * @param saveFaceIndex A vrai les indices de faces sont sauvegardé ( marqueur sur les faces des tétrahèdres par la suite )
	 * @param[out] fg_to_ind Correspondance entre les indices de faces (face,groupe) du modèle et l'indice interprété par tetgen
	 * @param saveFaceIndexAsGroup Les marqeurs correspondent aux groupes de surfaces
	 * @see http://tetgen.berlios.de/fformats.poly.html
	 */
	bool _SavePOLY(const wxString& filename, bool exportUserModel=false, bool separateUserDefinedModel=false, bool saveFaceIndex=false, std::vector<t_faceIndex>* fg_to_ind=NULL, bool saveFaceIndexAsGroup=false);



	/**
	 * Alimente un vecteur avec les sommets dans le système de coordonnées d'entrée ( pas celui de rendu)
	 */
	void FillVectorWithRealVertices(std::vector<vec3>* vectorToFeed);
	/**
	 * Met à jour la couleur de séléction des surfaces
	 * @param newSelectionColor Nouvelle couleur [0-1;0-1;0-1]
	 */
	void SetColorSelection(const vec3& newSelectionColor)
	{
		if(newSelectionColor!=colorSelection)
		{
			colorSelection=newSelectionColor;
			selectionChange=true;			
		}
	}

	/**
	 * Met à jour la couleur des lignes du modèle
	 * @param newModelLineColor Nouvelle couleur [0-1;0-1;0-1]
	 */
	void SetColorModelLines(const vec3& newModelLineColor)
	{
		if(newModelLineColor!=colorModelLines)
		{
			colorModelLines=newModelLineColor;
			modelChange=true;
		}
	}
	/**
	 * Met à jour la couleur de fond de la scène 3D
	 * @param newModelBackgroundColor Nouvelle couleur [0-1;0-1;0-1]
	 */
	void SetColorModelBackground(const vec3& newModelBackgroundColor)
	{
		if(newModelBackgroundColor!=colorModelBackground)
		{
			colorModelBackground=newModelBackgroundColor;
			backgroundChange=true;
		}
	}
	/**
	 * Met à jour la couleur des surfaces qui n'ont pas de couleur associée.
	 * @param newModelDefaultColor Nouvelle couleur [0-1;0-1;0-1]
	 */
	void SetDefaultModelColor(const vec3& newModelDefaultColor)
	{
		if(newModelDefaultColor!=colorModelDefault)
		{
			colorModelDefault=newModelDefaultColor;
			this->modelChange=true;
		}
	}
	/**
	 * Retourne la couleur de fond de la scène
	 */
	vec3 GetBackgroundColor()
	{
		return this->colorModelBackground;
	}
private:
	/**
	 * Les surfaces ayant un volume de chaque coté sont affichés quel que soit l'orientation, contrairement au surfaces externes. Ceci permet de voir les objets à l'intérieur du modèle.
	 */
	void SetInternalFaceState(const formatCoreBIN::ioModel& modelExport,const formatMBIN::trimeshmodel& tetramodel);
	void _RenderMaillageLines(t_cutPlane cutPlane);
	bool _SaveMESH(const wxString& filename);
	/**
	 * Save model.mat.ply , model with project defined groups
	 */
	bool _SavePLYProjectGroup(const wxString& filename);
	bool _SavePLY(const wxString& filename);
	bool _SaveASC(const wxString& filename);
	bool _SaveNFF(const wxString& filename);
	bool _SaveBIN(const wxString& filename);
	bool _SaveCBIN(const wxString& filename);
	bool faceConnecte(t_faceIndex _f);
	bool _SaveNFFMaillage(const std::string& filename);

	size_t _RenderGroupTexture(long g, bool blendRendering);
	long _RenderGroupLines(long g,bool hideLines);
	bool _Load3DS(const std::string& filename);
	bool _LoadBIN(const std::string& filename);
	bool _LoadPLY(const std::string& filename);
	bool _LoadPOLY(const std::string& filename);
	bool _LoadSTL(const std::string& filename);
	void _Destroy(void); 
	long _nbMaterials;
	bool _hasTexture;
	void _RenderMaillageTriangles(t_cutPlane cutPlane);
	vec3 colorSelection;
	vec3 colorModelLines;
	vec3 colorModelBackground;
	vec3 colorModelDefault;
	tetrahedre *tabVertexMaillage = nullptr;			//Tableau de faces du maillage
	unsigned long tabVertexMaillageSize;	// Nombre de tetrahedre
	vec3* nodesMaillage;					//Tableau de noeud 
	unsigned long nodesMaillageSize;		//Nombre de noeuds

	std::vector<vec3> _pNormals,_pVertices;
	std::vector<vec2> _pTexCoords;
	std::vector<SGroup3D> _pGroups;
	ui_tools::IntersectionSeeker* IntersectionTool;
	vec3 selectedDot;
	std::size_t	base;				// Base Display List For The Font Set 
	bool fontInitialized;
};



#endif  // _Objet3D_H


