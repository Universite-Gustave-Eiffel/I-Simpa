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

/*************************************
* Class Particules.h
*
* Contrôle les particules
*************************************/

/*! \file Particules.h
    \brief Gestionnaire de particules
*/
#include <vector>
#include <iostream>
#include <fstream>

#include "Mathlib.h"
#include "Objet3D.h"

#include "data_manager/element.h"

#include "../GlBitmapSurface.h" //rendu des légendes
#include "../legendFonts.h"
#include "Animator.hpp"

/**
 * \brief Gestionnaire de particules
 *
 * Cette classe permet de lire et écrire les fichiers de positions de particules et d'effectuer le rendu 3D des particules.
 */
class ParticulesControler : public Animator
{
public:
 /**
  * Constructeur
  */
	ParticulesControler();
 /**
  * Desctructeur
  */
~ParticulesControler();
/**
 * Initialisation. Vide le tableau des particules en mémoire
 */
virtual void Init(const bool& resetLoadingTime=true);

/**
 * @param mfilename Chemin et nom du fichier de particules
 * @param UnitizeVal Vecteur de transformation du modèle (Voir coordsOperation)
 * @param 
 */
bool LoadParticleFile(const char *mfilename, vec4 UnitizeVal, Element* elConf=NULL, Element::IDEVENT loadingMethod=Element::IDEVENT_LOAD_PARTICLE_SIMULATION);

/**
 * A vrai les pas de temps sont rendu de pas de temps = 0 à pas de temps = pas de temps courant
 * Cette méthode est utilisé pour le tracé des trajectoires des particules
 */
bool DoLoopFromZero() { return renderMethod==Element::IDEVENT_LOAD_PARTICLE_SIMULATION_PATH; }
/**
 * Execute les commandes OpenGL de rendu de particules
 */
virtual void Render(const int& timeStep=-1);
/**
 * Permet de connaître le nombre de particules
 * @return Dimensions du tableau de particules
 */
int GetSizeTabParticle(); 
/**
 * Informations sur le fichier de particules chargé
 * @return Nombre de pas de temps
 */
int GetNbTimeStep();
virtual void SetTimeStep(const int& timeStep);
void SaveToPBin(wxString fileName);

/**
 * Charge un fichier Binaire en mémoire. Préférer ce format plutot que les fichiers texte. La rapidité de chargement d'un fichier binaire est au moins 60 fois plus rapide que celle d'un fichier texte.
 * @param fileName Chemin du fichier
 * @param doCoordsTransformation A vrai le système de coordonnées sera modifié par rapport au fichier en entrée
 * @param UnitizeVal Modification du système de coordonnées
 */
void LoadPBin(wxString fileName, bool doCoordsTransformation=false, vec4 UnitizeVal=vec4(0,0,0,1));

/**
 * Initialise l'objet de rendu de légendes
 * @param drawingContext Contexte de rendu 
 */

void InitLegend(legendRendering::ForeGroundGlBitmap& _legendRenderer);
virtual wxString GetRendererLabel();

/**
 * Met à jour la couleur du texte de la légende
 * @param foregroundColor Nouvelle couleur [r,g,b]
 */
void SetTextForegroundColor(const ivec4& foregroundColor);
/**
 * Met à jour la couleur de fond du texte de la légende
 * @param backgroundColor Nouvelle couleur [r,g,b]
 */
void SetTextBackgroundColor(const ivec4& backgroundColor);
/**
 * Met à jour la couleur des particules
 * @param particleColor Nouvelle couleur [r,g,b]
 */
void SetParticleColor(const ivec3& _particleColor);
/**
 * Met à jour la police du texte de la légende
 * @param textFont Nouvelle police
 */
void SetTextFont(const wxFont& textFont);
/**
 * Recompile l'animation
 */
void RefreshAnimationRendering();
virtual void RedrawLegend();
private:
typedef float t_PosParticule;
typedef float t_EnergyParticule;
struct t_Particule
{
	t_PosParticule pos[3];
	t_EnergyParticule energie;
};
struct t_ParticuleInfo
{
	unsigned int nbStep;
	unsigned int firstStep;
	t_Particule *tabSteps;
	t_ParticuleInfo() { tabSteps = NULL; }
	~t_ParticuleInfo() { delete[] tabSteps;}
};
vec4 particleColor;
float timeStep;
unsigned int nbStep;
wxString ShortcutPath;
Element::IDEVENT renderMethod;
unsigned long nbParticles;

t_ParticuleInfo *tabInfoParticles;
   struct t_p_legends
	{
		legendRendering::legendText* currentFile;
		legendRendering::legendText* currentTimeStep;
		t_p_legends()
		{
			currentFile=NULL;
			currentTimeStep=NULL;
		}
	} p_legends;
	legendRendering::ForeGroundGlBitmap* legendRenderer;
};
