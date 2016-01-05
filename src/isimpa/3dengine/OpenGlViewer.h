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

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "OpenGLApp.h"
#include "GlBitmapSurface.h" //rendu des légendes
//#include <string> 
#define PARAM_BOUND_ON_SELECT_FACES (const std::vector<t_faceIndex>& facelst,const bool& dragging)

/*! \file OpenGlViewer.h
    \brief Implémentation de la classe OpenGlViewer
*/
/**
 * @brief Fenêtre du rendu 3D
 * 
 * Les actions utilisateur sur la fenêtre 3D sont géré ici. Cette classe fait office d'interface entre la classe de rendu de la scène OpenGLApp et l'interface utilisateur.
 */
class OpenGlViewer: public wxGLCanvas
{
public:
	/**
	 * Enumeration des modes de caméra
	 */
	enum CAMERA_MODE {CAMERA_FIRST_PERSON = 0,CAMERA_ROTATION=1};
	/**
	 * Enumeration des outils associés au curseur
	 */
	enum TOOL_MODE {TOOL_MODE_CAMERA ,TOOL_MODE_SELECTION,TOOL_MODE_RECEPTEURS_SOUNDLEVEL_EXTRACT };
	/**
	 * Constructeur
	 * @param parent Fenêtre parent
	 * @param id Identifiant de la fenêtre
	 * @param pos Position relative de la fenêtre
	 * @param size Dimensions de la fenêtre
	 * @param style Style de la fenêtre
	 * @param name Libellé de la fenêtre
	 * @param gl_attrib Attributs openGl (Requis par wxGLCanvas)
	 */
    OpenGlViewer(wxWindow *parent, wxWindowID id = wxID_ANY,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize, long style = 0,
    const wxString& name = _T("OpenGlViewer"), int *gl_attrib = NULL);

	/*
	 * Surcharge de la méthode d'actualisation
	void Refresh(bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL);
	 */

	/**
	 * Modifie l'outil courant associé à la souris de l'utilisateur
	 * @param newMode Nouvel outil à utiliser
	 */
	void SetCurrentTool(TOOL_MODE newMode);
	/**
	 * Retourne l'outil courant associé à la souris de l'utilisateur
	 * @return Outil utilisé
	 */
	TOOL_MODE GetCurrentTool();
	/**
	 * Copie le rendu de la scène vers un objet image.
	 * @param[out] aimage Image à renseigner
	 * @param awidth Largeur en pixels du rendu à effectuer.
	 * @param aheight Hauteur en pixels du rendu à effectuer.
	 * @return 1 si l'opération a réussi
	 */
	int GetImage(wxImage& aimage, const int awidth, const int aheight);
	/**
	 * Copie le rendu de la scène vers un objet image.
	 * @param[out] aimage Image à renseigner
	 * @return 1 si l'opération a réussi
	 */
	int GetImage(wxImage& aimage);
	/**
	 * Destructeur
	 */
    ~OpenGlViewer();
	/**
	 * Un partie de l'ecran a besoin d'être redessiner, cette méthode rafraichie le rendu
	 * @param event Objet associé à l'événement
	 */
    void OnPaint(wxPaintEvent& event);

	/**
	 * La fenêtre a été redimensionné par l'utilisateur
	 * @param event Objet associé à l'événement
	 */
    void OnSize(wxSizeEvent& event);
	/**
	 * Cette surcharge a été effectué afin d'eviter la fenêtre de scintiller
	 * @param event Objet associé à l'événement
	 */
    void OnEraseBackground(wxEraseEvent& event);
	/**
	 * L'utilisateur a effectué une opération avec la souris
	 * @param event Objet associé à l'événement
	 */
	void OnMouseEvent(wxMouseEvent& event);
	/**
	 * Charge le model 3D dans la fenêtre
	 * @param m_Object Objet3D
	 */
	void OpenModel(CObjet3D *m_Object);
	/**
	 * Actualise les parametres de rendu de l'animation
	 * @param refreshrate Interval d'affichage de l'animation en ms
	 */
	void SetSimulationRefreshRate(const int& refreshrate);
	/**
	 * Redefinie le plan de coupe
	 */
	void SetCutPlane(bool actif,int axe,int signe, float valeur);
	/**
	 * Change le mode de caméra à utiliser
	 * @param cameraMode Nouveau mode de caméra
	 * @see CAMERA_MODE
	 */
	void SetCameraMode(int cameraMode);
	/**
	 * Permet de connaître le mode courant de caméra pour cette affichage
	 * @return L'indice du mode de caméra
	 * @see CAMERA_MODE
	 */
	int GetCameraMode();
	/**
	 * Cette méthode permet de connaître le prochain mode de caméra par rapport au mode de caméra courant
	 * @return Le mode de camera suivant
	 */
	OpenGlViewer::CAMERA_MODE NextCameraMode();
	/**
	 * Cette méthode permet de réinitialiser la vue 3D
	 */
	void ResetCamera();

	/**
	 * Démmarre la simulation du rendu de particule
	 */

	void RunSimulation();
	
	/**
	 * Met en pause la simulation du rendu de particule
	 */
	void PauseSimulation();
	
	/**
	 * Arrete et réinitialise la simulation du rendu de particule
	 */
	void StopSimulation();
	
	/**
	 * Affiche le prochain pas de rendu de particule
	 */
	void NextStepSimulation();
	
	/**
	 * Affiche le précédent pas de rendu de particule
	 */
	void PreviousStepSimulation();
	/**
	 * Actualise le rendu des éléments E_Drawable
	 */
	void RefreshElementDraw();
	
	/**
	 *  À vrai un clic dans la vue 3D va appeler dans le gestionnaire de projet la méthode ProjectManager::OnSelectPosition()
	 */
	bool modeSelectionPoint;

	/**
	 * Associer la méthode d'événement de sélection d'une face.
	 * @param fevent Méthode a lier
	 */
	void bindSelectionEvent(void (*fevent)PARAM_BOUND_ON_SELECT_FACES);
	/**
	 * Associer la méthode d'événement de sélection d'un point.
	 * @param fevent Méthode a lier
	 */
	void bindSelectionPositionEvent(void (*fevent)(const vec3));

	/**
	 * Permet de modifier les paramètres de rendu
	 * @param idRenderMode Identifiant du paramètre (Utiliser les constantes et non un entier)
	 * @param newValue Nouvelle valeur du paramètre
	 * @see GetCurrentRenderMode()
	 */
	void ChangeRenderMode(OpenGLApp::t_renderMode idRenderMode,bool newValue);
	/**
	 * Accesseur des paramètres des rendu
	 * @param idRenderMode Identifiant du paramètre (Utiliser les constantes et non un entier)
	 * @return Retourne la valeur d'un paramètres des rendu
	 * @see ChangeRenderMode()
	 */
	bool GetCurrentRenderMode(OpenGLApp::t_renderMode idRenderMode);

	void OnKeyDown(wxKeyEvent& keyEvt);
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

	legendRendering::ForeGroundGlBitmap legendDrawer;

	DECLARE_EVENT_TABLE()
private:
	//Rendu des légendes
	void DrawingBitmaps(wxDC* drawingContext);
	

	TOOL_MODE currentTool;
	void (*pointeurFonctionEventSelection)PARAM_BOUND_ON_SELECT_FACES; // groupe,face todo const std::vector<t_faceIndex>&
	void (*pointeurFonctionEventSelectionPosition)(const vec3); // groupe,face
	void OnMouseDoubleClick(t_faceIndex selection);
	bool eventBinded;
	bool eventPositionBinded; 
	bool cutPlaneToUpdate;
	bool ElementDrawToUpdate;
	bool doScreenRefresh;
	OpenGLApp *m_GLApp;
	GLboolean smooth;
	GLenum modeaff;
	void OnTimer( wxTimerEvent& event );
	bool appLoaded;
	bool simulationIsRunning;
	int particlesTimeStep;
	wxDateTime LastAnimationTimeStep; //nombre de ms depuis le dernier rafraichissement des particules
	int minimalTimeStep; //Taux de raffraichissement Maximal, pas de temps en ms
    wxTimer m_Timer;
	CObjet3D *CurrentObject;
	void Display();
};