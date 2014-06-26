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

#include "data_manager/data_calculation/calculsPropagation.h"
#include <iostream>


#include "input_output/bin.h"
#include "input_output/progressionInfo.h"
#include "data_manager/base_core_configuration.h"
#include "input_output/baseReportManager.h"
/**
 * @file coreInitialisation.h
 * Déclaration
 */
entier GetRecepteurSurfacique(entier xmlIdRs,std::vector<r_Surf*>& recepteur_s_List);

void genprintVec(vec3 inf);


/**
 * Initialise la propriété de tétraèdre courant des sources
 * @param tetraMesh Maillage tétraèdrique de la scène
 * @param lstSources Liste des sources
 */
void InitSourcesTetraLocalisation(t_TetraMesh* tetraMesh,std::vector<t_Source*>* lstSources);


/**
 * Initialisation de la célérité du son dans chaque volume du maillage. La célérité dépend de la hauteur des volumes, de la rugosité Z0, et de C0
 */
void InitTetraCelerite(t_TetraMesh* tetraMesh,Base_Core_Configuration& configManager);
/**
 * Initialise la propriété des tétraèdres lié aux recepteurs ponctuels
 * @param tetraMesh Maillage tétraèdrique de la scène
 * @param lstRecepteurP Liste des recepteurs ponctuels
 */
void InitRecepteurPTetraLocalisation(t_TetraMesh* tetraMesh,std::vector<t_Recepteur_P*>* lstRecepteurP,Base_Core_Configuration& configManager);

/**
 * Réserve l'espace mémoire correspondant à une bande de fréquence.
 */
void InitRecepteurSBfreq(std::vector<r_Surf*>& recepteursSurfaciques,uentier idFreq,uentier nbTimeStep);

/**
 * Initialise les récepteurs surfaciques. Ajoute les faces correspondant aux recepteur composé de plusieurs surfaces du maillage
 * @param tetraMesh Maillage tétraèdrique de la scène
 * @param recepteursSurfaciques Liste des recepteurs surfaciques
 * @param configManager Configuration du projet
 */
void InitRecepteurS(t_TetraMesh* tetraModel,std::vector<r_Surf*>& recepteursSurfaciques,Base_Core_Configuration& configManager);

/**
 * Initialise les récepteurs surfaciques. Réserve l'espace mémoire pour le stockage des résultats
 * @param recepteursSurfaciques Liste des recepteurs surfaciques
 * @param configManager Configuration du projet
 */
void InitRecepteurSCut(std::vector<r_SurfCut*>& recepteursSurfaciques,Base_Core_Configuration& configManager);
/**
 * Initialise les récepteurs surfaciques. Ajoute les faces correspondantes aux recepteurs composés de plusieurs surfaces du modèle
 * @param tetraMesh Maillage tétraèdrique de la scène
 * @param lstRecepteurP Liste des recepteurs ponctuels
 */
void InitRecepteurS(t_Mesh* sceneMesh,std::vector<r_Surf*>& recepteursSurfaciques,Base_Core_Configuration& configManager);

/**
 * Chargement des faces dans les structures appropriées
 */
bool initMesh(t_Mesh &sceneMesh,CoreString workingDir,CoreString sceneMeshPath,Base_Core_Configuration& configManager);
/**
 * Chargement du fichier de maillage et initialisation de l'environnement
 */
bool initTetraMesh(CoreString tetraMeshFilePath,t_Mesh &sceneMesh,int nbFreq,t_TetraMesh& sceneTetraMesh,Base_Core_Configuration& configManager);
/**
 * Enregistrement des données récoltées
 */
void reportCompilation(Base_Core_Configuration& configManager,CoreString workingDir);
