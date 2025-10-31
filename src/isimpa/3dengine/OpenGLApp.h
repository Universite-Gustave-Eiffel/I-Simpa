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


#ifndef _OpenGLApp_h
#define _OpenGLApp_h
/*! \file OpenGLApp.h
    \brief Implémentation de la classe OpenGLApp
*/

#include "Core/Objet3D.h"
#include "Core/Animator.hpp"
#include "Camera.h"
#include <string.h>
#include <wx/image.h>
#include <list>
#include "GL/opengl_inc.h"
/**
 * Contient les information d'un animateur
 */
struct AnimatorManager
{
	AnimatorManager();
	void SetAnimator(Animator* _animator);
	/**
	 * Libére les données du tableau gl_compilation_array
	 */
	void FreeGlCompilationArray();
	~AnimatorManager();
	/**
	 * Supprime les données du tableau gl_compilation_array et l'initialise avec le nombre de pas de temps contenu dans l'animation
	 */
	void ResetCompilationArray();
	Animator* ctrl_animator;
	unsigned int* gl_compilation_array;
	unsigned int gl_compilation_array_size;
	wxDateTime lastCompilationTime;
};
typedef smart_ptr<AnimatorManager> ptAnimatorManager;
typedef std::list<ptAnimatorManager>::iterator AnimatorManagerIterator;
/**
 * @brief
 *
 * Classe de contrôle de la classe CObjet3D, intermediaire entre la fenêtre 3D et Le rendu des objets
 * Cette classe définit l'environnement autour de la scène et fait appel aux méthode de CObjet3D en fonction des parametres de rendu
 * Il gère également l'accélération du rendu grâce à la précompilation des listes d'instructions de rendu 3D.
 */
class OpenGLApp
{
public:
	/**
	 * Enumeration des paramètres de rendu 3D
	 */
	enum t_renderMode
	{
		renderModelFaces,
		renderModelLines,
		renderModelLinesAndConstruction, //lignes de contours et de construction
		renderModelInside,
		renderMaillageFaces,
		renderMaillageLines,
		renderSelection,
		renderModelMaterialFaces,
		GL_APP_QUANT_RENDER_TYPE
	};
private:
	bool renderMode[GL_APP_QUANT_RENDER_TYPE];
public:
	/**
	 * Constructeur
	 */
	OpenGLApp();
	/**
	 * Destructeur
	 */
	~OpenGLApp() { Destroy();ClearAnimatorLst();}

	/**
	 * Charge l'objet passé en parametre comme objet de rendu.
	 * @param im_Object Objet de rendu 3D
	 */
	void Init(CObjet3D *im_Object);

	/**
	 * Actualise l'affichage à partir des listes précompilées
	 * @param hDC Drawing Context, instance de rendu de la scène (Provenement d'une fenêtre)
	 */
	//void Display(HDC hDC);
	void Display();
	/**
	 * Redimensionne le rendu 3D.
	 * @param w Nouvelle largeur
	 * @param h Nouvelle hauteur
	 */
	void ChangeWindow(int w, int h);

	/**
	 * Décrémente le pas de temps
	 */
	void DecreaseTimeStep();

	/**
	 * Incrémentation du pas de temps du rendu 3D des particules. Reviens au premier pas de temps si le pas de temps courant est le dernier.
	 */
	void Tic();
	/**
	 * Reprend au premier pas de temps du rendu des animations
	 */
	void RestartAnimationRendering();
	/**
	 * A partir des coordonnées sur la fenêtre rend l'indice de la face correspondant
	 * @param x Cordonnées en 2D relative à l'origine de la zone de rendu 3D.
	 * @param y Cordonnées en 2D relative à l'origine de la zone de rendu 3D.
	 * @return Le numéro de face et de groupe retourné est à -1 si aucune face ne se trouve sous ces coordonnées.
	 */
	t_faceIndex SelectVertex(unsigned int x, unsigned int y);
	/**
	 * Detection de collision entre la position de rendu et la scène 3D.
	 * @param x Cordonnées en 2D relative à l'origine de la zone de rendu 3D.
	 * @param y Cordonnées en 2D relative à l'origine de la zone de rendu 3D.
	 * @return Position x,y,z de collision avec une face par rapport à la vue courante
	 */
	vec3 SelectPosition(unsigned int x, unsigned int y);
	/**
	 * Réinitialisation de la scène, détache l'objet de rendu du rendu 3D.
	 */
	void ResetAll();
	/**
	 * Transforme le pourcentage de plan de coupe en valeur correspondante dans la scène 3D.
	 * @param valeur Valeur en pourcentage [0;1]
	 * @param axe Axe concerné x:0 y:1 z:2
	 */
	void SetPlaneValue(float valeur,int axe);
	/**
	 * Permet de savoir si un objet de rendu a été passé dans la scène
	 * @return Vrai si la scène a été initialisée
	 * @see Init()
	 */
	bool IsObjetLoaded(void) {return m_IsObjetLoaded;}

	float GetRotX(void) {return m_Rot.x;}
	float GetRotY(void) {return m_Rot.y;}
	float GetRotZ(void) {return m_Rot.z;}

	float GetTransX(void) {return m_Trans.x;}
	float GetTransY(void) {return m_Trans.y;}
	float GetTransZ(void) {return m_Trans.z;}

	float GetFocalX(void) {return m_Focal.x;}
	float GetFocalY(void) {return m_Focal.y;}
	float GetFocalZ(void) {return m_Focal.z;}

	/**
	 * Retourne un pointeur vers le gestionnaire de caméra
	 * @return Gestionnaire de caméra
	 */
	CameraControler* GetUserCam(void);
	/**
	 * Recompile la liste d'instruction correspondant au modèle de la scène
	 */
	void UpdateGlList();
	/**
	 * Recompile la liste d'instruction correspondant aux éléments E_Drawable
	 * @param useLists Vrai si on recompile, faux si on doit faire directement le rendu sans compiler les instructions openGL.
	 */
	void UpdateGlElementList(bool useLists=true);

	/**
	 * Recompile la liste d'instruction correspondant à la sélection des faces
	 * @param useLists Vrai si on recompile, faux si on doit faire directement le rendu sans compiler les instructions openGL.
	 */
	void UpdateGlSelectionList(bool useLists=true);

	/**
	 * Recompile la liste d'instruction correspondant au maillage de la scène
	 */
	void UpdateGlMaillageList();
	/**
	 * Paramètres du plan de coupe du maillage
	 */
	t_cutPlane cutPlane;

	/**
	 * Permet de modifier les paramètres de rendu
	 * @param idRenderMode Identifiant du paramètre (Utiliser les constantes et non un entier)
	 * @param newValue Nouvelle valeur du paramètre
	 * @see GetCurrentRenderMode()
	 */
	void ChangeRenderMode(t_renderMode idRenderMode,bool newValue);
	/**
	 * Accesseur des paramètres des rendu
	 * @param idRenderMode Identifiant du paramètre (Utiliser les constantes et non un entier)
	 * @return Retourne la valeur d'un paramètres des rendu
	 * @see ChangeRenderMode()
	 */
	bool GetCurrentRenderMode(t_renderMode idRenderMode);

	/**
	 * Charge l'image passé en paramètre par le rendu 3D courant.
	 * @param[out] aimage Image à renseigner
	 * @return 1 si l'opération a réussi
	 */
	int GetImage(wxImage& aimage);

	/**
	 * Execute le rendu total de la scène 3D
	 * @param useLists Vrai si on utilise les instructions précompilé, faux si on doit faire directement le rendu (beaucoup plus lent).
	 */
	void RunGlCommands(bool useLists=true);
	/**
	 * Execution des instructions de rendu 3D d'environnement de la scène, tel que la luminosité et la couleur de fond du rendu.
	 */
	void InitGl();

	/**
	 * Efface la liste des animateurs contenu dans la classe
	 */
	void ClearAnimatorLst();
	/**
	 * Ajoute un animateur
	 * @param[in] newAnimator Animateur a ajouter
	 */
	void AddAnimator(Animator* newAnimator);
	/**
	 * Réinitialise les animateurs contenu dans la classe
	 */
	void InitAnimatorLst();
private:
	std::vector<ptAnimatorManager> animators;
	void SetTimeStep(int idTimeStep);

	void InitLst(int index);
	void DeleteLst(int index);
	float Ratio;

	void InitCutPlane();

	void LoadGlMaillageList(bool useLists=true);
	void LoadGlModelList(bool useLists=true);
	void Destroy();
	void LoadAnimatorLst(ptAnimatorManager& managerToCompile);
	CObjet3D *m_Object = nullptr;
	char m_filename[256];

	bool m_IsObjetLoaded;

	CameraControler userCamera;

	vec2 m_Abs;
	vec3 m_DirectionLight, m_Rot, m_Trans, m_Focal, m_RotLight,  m_Model_Rot, m_Model_Trans;
	vec4 m_AmbientLight, m_DiffuseLight, m_SpecularLight, m_PosLight;
	float m_SpotAngle;

	/**
	 *	m_list Indice instructions openGl :
	 *		0: Scène 3D
	 *		1: Maillage
	 *		2: Elements (E_Drawable)
	 *		3: Sélection
	*/
	GLuint m_list[4] {0, 0, 0, 0};
	bool m_list_isinit[4] {false, false, false, false};
	int CurrentTimeStep;
	int winHeight,winWidth;
};

#endif //	_OpenGLApp_h
