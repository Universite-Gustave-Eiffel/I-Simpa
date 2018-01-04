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

/*! \file Recepteurs_surfacique.h
    \brief Gestionnaire de rendu des recepteurs surfaciques
	
	Rendu des résultat du calcul concernant les recepteurs surfaciques
*/

#include "Mathlib.h"
#include "Objet3D.h"
#include "data_manager/element.h"
#include "../GlBitmapSurface.h" //rendu des légendes
#include "../legendFonts.h"
#include "Animator.hpp"
#include <list>
#include <input_output/exportRecepteurSurf/std_rsbin.hpp>
/**
 * @brief Gestionnaire de rendu des recepteurs surfaciques
 *
 * Rendu des résultat du calcul concernant les recepteurs surfaciques
 */
class RecepteursSControler : public Animator
{
public:
 /**
  * Constructeur
  */
	RecepteursSControler();
 /**
  * Desctructeur
  */
	~RecepteursSControler();
/**
 * Initialisation. Vide le tableau des surfaces en mémoire
 */
virtual void Init(const bool& resetLoadingTime=true);
/**
 * Met à jour les paramètres de configuration des faces
 * @see ApplicationConfiguration
 */
void UpdateRecepteurSSConfig();
/**
 * Passe au pas de temps passé en paramètre (met à jour la légende)
 */
void SetTimeStep(const int& timeStep);

/**
 * @param mfilename Chemin et nom du fichier de faces
 * @param UnitizeVal Vecteur de transformation du modèle (Voir coordsOperation)
 * @param 
 */
bool LoadRecepteursSFile(wxArrayString& recepteurSFiles, vec4 UnitizeVal, Element* elConf=NULL,Element::IDEVENT loadingMethod=Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP);
/**
 * Charge la palette en tant que couleur de rendu des récepteurs de surface
 */
bool LoadGplFile(const wxString& gplFileName);
/**
 * Actualise l'échelle de valeur de rendu
 * @param elConf Element de configuration du récepteur de surface
 */
void Refresh(Element* elConf,bool reloadAnimation=true);
/**
 * Execute les commandes OpenGL de rendu des recepteurs surfaciques
 */
void Render(const int& timeStep=-1);

/**
 * Informations sur le fichier de recepteurs surfacique chargé
 * @return Nombre de pas de temps
 */
int GetNbTimeStep();
virtual void RedrawLegend();
/**
 * Initialise l'objet de rendu de légendes
 * @param drawingContext Contexte de rendu 
 */

void InitLegend(legendRendering::ForeGroundGlBitmap& legendRenderer);
/**
 * Une fois l'outil de récuperation de valeur sélectionné, si l'utilisteur désire connaitre le niveau acoustique cette méthode est appelée.
 */
bool UserWantToKnowAcousticLevelAtPosition( vec3 extractPosition );
void SetIsoLineColor(const ivec3& isolineColor);
virtual wxString GetRendererLabel();
private:
	float Ponderate(const float& val);
	vec4 GetColor(const float& val);
	std::vector<vec3> colorPalette;
	struct t_RssLegend
	{
		wxString maxValue;
		wxString avgValue;
		wxString minValue;
		wxString path;
		wxString currentTime;
		wxString labelEl;
		wxFloat32 v_min;
		wxFloat32 v_max;
		wxString unit;
	} libellLegends;
	int xmlIdFile;				//Numéro de l'élément chargé en dernier
	float opacity;				//Valeur d'opacité
	bool renderFaceOne;			//Rendu sur la normal
	bool renderFaceTwo;			//Rendu inverse normal
	bool smoothColour;			// Couleur sur les noeuds et non par face
	bool renderIsoCurves;		//Rendu des courbes iso
	ivec3 isolinecolor;

	int RecordRecepteurSurfType; /*<! Type de récepteur de surface @see formatRSBIN::RECEPTEURS_RECORD_TYPE */
	float timeStep;
	unsigned int idStep;
	unsigned int nbStep;
	wxFloat32 ComputeBestIsoInterval(wxFloat32 vmin, wxFloat32 vmax);
	struct t_rs_face
	{	
		std::vector<float> energy; //Cette valeur est la véritable valeur enregistré et est utilisé quand l'utilisateur désire connaitre le niveau acoustique à un point donnée
		ivec3 sommets;
		bool render;
		t_rs_face(){render=true;}
		t_rs_face(ivec3 _sommets):sommets(_sommets),render(true){}
	};
	struct t_nodeS
	{
		t_nodeS() {quantFacesLinked=0;}
		std::vector<float> energy;
		int quantFacesLinked;
		vec3 position;
	};
	struct t_isocurves
	{
		float isolvl;
		std::list<formatRSBIN::t_curve> segments;
	};
	struct t_recepteurS
	{
		std::vector<t_rs_face> faces;
		std::list<t_isocurves> iso_curves;
	};
	std::vector<t_recepteurS> recepteursS;
	std::vector<t_nodeS> nodesData;
	struct t_rs_legends
	{
		legendRendering::legendText* graduationBar;
		legendRendering::legendText* currentFile;
		legendRendering::legendText* currentTimeStep;
		t_rs_legends()
		{
			graduationBar=NULL;
			currentFile=NULL;
			currentTimeStep=NULL;
		}
	} rs_legends;
	legendRendering::ForeGroundGlBitmap* legendRenderer;
	enum const_size_legend
	{
		LevelGraphBpp=3,
		LevelGraphWidth=80,
		LevelGraphHeight=340
	};
	unsigned char LevelGraphbytes[LevelGraphHeight*LevelGraphWidth*LevelGraphBpp];
	t_rs_face* GetFaceByCoord(vec3 insideDotCoords);
};