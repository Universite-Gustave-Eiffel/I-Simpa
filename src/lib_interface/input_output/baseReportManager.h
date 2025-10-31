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

#include <fstream>
#include "coreTypes.h"
#include <vector>
#include "data_manager/base_core_configuration.h"
/**
 * @file baseReportmanager.h
 * @brief Implémentation du gestionnaire de fichiers de rapports
 *
 * Une instance de BaseReportManager permet de gérer un récepteur surfacique pour une bande de fréquence sur le nombre de pas de temps indiqué en paramètre.
 */


#ifndef __BASE_REPORT_MANAGER__
#define __BASE_REPORT_MANAGER__

typedef CoreString stringClass;

/**
 * @brief Gestionnaire de fichier de sortie à déstination de l'interface PSPS
 *
 * Cette classe permet l'export de tout les fichiers de resultat de calcul.
 */
class BaseReportManager
{
public:
	/**
	 * Constructeur de la classe
	 */
	BaseReportManager();
	virtual ~BaseReportManager();
	/**
	 * Sauvegarde le fichier contenant les recepteurs surfaciques associés aux tétraèdre
	 * @param divBySurface A vrai divise le niveau sonore par l'aire de chaque surface
	 */
	static void SauveRecepteursSurfaciques(stringClass rootFolder,uentier idfreq,std::vector<r_Surf*>& tabRecepteurS,t_TetraMesh& sceneMesh,decimal timeStep=1.f,bool divBySurface=true);
		/**
	 * Sauvegarde le fichier contenant les recepteurs surfaciques associés aux faces du modèle
	 * @param divBySurface A vrai divise le niveau sonore par l'aire de chaque surface
	 */
	static void SauveRecepteursSurfaciquesCoupe(stringClass rootFolder,std::vector<r_SurfCut*> tabRecepteurS,decimal timeStep=1.f,bool divBySurface=true,bool allfreq=true,uentier idfreq=0);
	/**
	 * Sauvegarde le fichier contenant les recepteurs surfaciques associés aux faces du modèle
	 * @param divBySurface A vrai divise le niveau sonore par l'aire de chaque surface
	 */
	static void SauveRecepteursSurfaciques(stringClass rootFolder,uentier idfreq,std::vector<r_Surf*>& tabRecepteurS,t_Mesh& sceneModel,decimal timeStep=1.f,bool divBySurface=true);

	/**
	 * Sauvegarde un récepteur ponctuel
	 */
	static void SauveRecepteurPonctuel(const stringClass& fileName, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& timeStepLst,t_Recepteur_P* tabRecepteurP);

	/**
	 * Sauvegarde des récepteurs ponctuels, créé les dossiers
	 */
	static void SauveRecepteursPonctuels(stringClass rootFolder,const stringClass& fileName, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& timeStepLst,std::vector<t_Recepteur_P*>* tabRecepteurP);
	/**
	 * Sauvegarde le fichier toute bandes contenant les recepteurs surfaciques associés aux tétraèdre
	 * @param divBySurface A vrai divise le niveau sonore par l'aire de chaque surface
	 */
	static void SauveGlobalRecepteursSurfaciques(stringClass rootFolder,std::vector<r_Surf*>& tabRecepteurS,t_TetraMesh& sceneMesh,decimal timeStep=1.f,bool divBySurface=true);
	/**
	 * Sauvegarde le fichier toute bandes contenant les recepteurs surfaciques aux faces du modèle
	 * @param divBySurface A vrai divise le niveau sonore par l'aire de chaque surface
	 */
	static void SauveGlobalRecepteursSurfaciques(stringClass rootFolder,std::vector<r_Surf*>& tabRecepteurS,t_Mesh& sceneModel,decimal timeStep=1.f,bool divBySurface=true);
	/**
	 * Init empty arrays with labels used by *.recp files
	 * @param[in] configManager 
	 * @param[out] lblFreq 
	 * @param[out] lblTime 
	 */
	static void InitHeaderArrays(Base_Core_Configuration& configManager, std::vector<CoreString>& lblFreq, std::vector<CoreString>& lblTime);
};

#endif
