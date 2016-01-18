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

/*! \file Camera.h
    \brief Implémentation de la classe CameraControler
*/
#include "manager/smart_ptr.h"
#include "Core/Mathlib.h"
#include "ArcBall.h"


#define  _CAMERA_FP 0
#define  _CAMERA_ELLIP 1

#define  _CAMERA_FP_SENSIBILITE 0.3
#define  _CAMERA_FP_SENSIBILITE_ZOOM 0.007

#define  _CAMERA_ELLIP_SENSIBILITE 0.01

class ArcBall_t;
/**
 *  \class CameraControler
 *	\brief Contrôle de la camera du visualisateur
 *	Differend type de camera, première personne et elliptique(autour et tourné vers un point)
 *	@see OpenGLApp()
 *
 */
class CameraControler
{
public:

	/**
	 *	\brief Structure représentant les valeurs qui définissent une caméra
	 */
	struct t_cameraConfig
	{
		vec3 Camera_Trans;
		vec3 Camera_Rot;
		vec3 Camera_Focal;
		vec3 Model_Trans;
		vec3 Model_Rot;
	};

	/**
	 * \brief Enumération des codes de touches
	 **/
	enum KEYCODE
	{
		KEYCODE_LEFT=314,
		KEYCODE_UP,
		KEYCODE_RIGHT,
		KEYCODE_DOWN
	};
	/**
	 *	Constructeur de CameraControler
	 */
	CameraControler();
	/**
	 * Initialisation
	 * @param cameratype Type de caméra à controler
	 * @param wWidth Largeur de la fenêtre OpenGL
	 * @param wHeight Hauteur de la fenêtre OpenGL
	 * @param trans Pointeur vers la valeur de translation OpenGL
	 * @param rot Pointeur vers la valeur de rotation OpenGL
	 * @param focal Pointeur vers la valeur de focal GLUT
	 * @param m_trans Pointeur vers la valeur de translation GLUT
	 * @param ml_rot Pointeur vers la valeur de rotation GLUT
	 */
	void Init(int cameratype,int *wWidth,int *wHeight,vec3 *trans,vec3 *rot,vec3 *focal,vec3 *m_trans,vec3 *ml_rot);
	/**
	* Réinitialisation de la position de la caméra
	*/
	void ResetCameraPos();
	/**
	* Méthode de mise à jour de la caméra en fonction de l'événement de souris
	* @param leftdown Vrai si ce bouton est appuyé
	* @param midledown Vrai si ce bouton est appuyé
	* @param rightdown Vrai si ce bouton est appuyé
	* @param x Position du curseur par rapport à l'origine de la fenêtre OpenGL (pixel)
	* @param y Position du curseur par rapport à l'origine de la fenêtre OpenGL (pixel)
	* @param rotation de la roulette de la souris
	*/
	bool MouseEvent(bool leftdown,bool midledown,bool rightdown, int x,int y,int rotation);
	/**
	 * Contrôle de la caméra par le clavier
	 * @param keyCode Code de la touche du clavier
	 * @return Vrai si la caméra a bougé
	 */
	bool KeyDownEvent(int keyCode);
	/**
	 * Méthode permettant de changer le mode de caméra pour cameraMode
	 * @param cameraMode Nouveau mode de caméra
	 * @see _CAMERA_FP
	 * @see _CAMERA_ELLIP
	 */
	void changeCameraMode(int cameraMode);
	/**
	 * A partir du mode courant de la caméra
	 * @return Le mode de la caméra
	 * @see _CAMERA_FP
	 * @see _CAMERA_ELLIP
	 */
	int GetCameraMode();
	/**
	 * Retourne les paramètres de la camera utilisée actuellement
	 * @return Structure de données de type t_cameraConfig
	 */
	const t_cameraConfig GetCameraConfiguration();
private:
	void FirstCameraMove(int ansx,int ansy,int nouvx,int nouvy);
	void SecondCameraMove(int ansx,int ansy,int nouvx,int nouvy);
	void ThirdCameraMove(int ansx,int ansy,int nouvx,int nouvy);
	void TranslationCameraFace(float distance);
	void TranslationCameraCote(float distance);
	void RotationY(float angle);
	void RotationZX(float angle);
	int camtype,*windowHeight,*windowWidth;
	vec3 *Camera_Trans;
	vec3 *Camera_Rot;
	vec3 *Camera_Focal;
	vec3 *Model_Trans;
	vec3 *Model_Rot;
	ArcBall_t arcBall;
};
