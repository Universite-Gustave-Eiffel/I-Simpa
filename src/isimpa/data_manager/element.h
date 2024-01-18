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

#include <wx/treectrl.h>
#include <wx/filename.h>
#include <vector>
#include <list>
#include <wx/xml/xml.h>
#include <wx/menu.h> //menu de popup
#include <atomic>
#include "3dengine/Core/Mathlib.h"
#include "manager/stringTools.h"
#include "manager/lifetimeWitness.hpp"
#include "UtfConverter.h"
/**  \file element.h
 *   \brief Element de base définissant une partie du projet ouvert.
 */


#ifndef __ELEMENT_H__
#define __ELEMENT_H__

/**
 @brief Element de base définissant une partie du projet courant.

 La classe élément est destinée a être surchargée afin de répondre à des exigeances spécifiques.
 De base un élément est sauvegardé dans le fichier XML du projet,
 il est visible dans l'arbre du projet dans l'interface,
 Ces données peuvent être exporté dans le presse-papier
 Il peut contenir des propriétés et elles peuvent être affichées dans un grille.
 @see E_Data
*/
class Element {
private:

    static std::atomic_long nbElement;

    void SaveChildren(wxXmlNode *NoeudCourant);

    wxXmlNode *GenericSaveXmlDoc(wxXmlNode *NoeudParent);

    void SetXmlId();

protected:
    wxTreeCtrl *treeCtrl; /*!< Contrôle lié à cet élément */


    void initPropLabel(Element *confCore, const wxString &propName, const wxString &propLabel);

    void initPropListItem(Element *confCore, const wxString &propName, int itemId, const wxString &propLabel);

public :

    /** @defgroup Element Noyau élémentaire composant le projet
     *
     *	Toutes les informations du projet qui sont visibles dans l'arbre du projet et dans la grille des propriétés héritent de la classe Element
     *  @see Element
     *  @{
     */
    /** @defgroup Types Types propre à un élément.
     *
     *	La classe Element contient plusieurs types et structures de données donnant des informations sur l'élément lui-même
     *  @see Element
     *  @{
     */
    /**
     *  Enumeration de tout les types d'élément existant, à la création d'un nouveau type d'élément, ajouter une ligne à cette énumération
     *  \warning Ne pas changer l'ordre de ces éléments énumérés sous peine de ne plus pouvoir ouvrir les projet *.proj des anciennes versions de l'interface. Ajouter les identifiants des nouveaux éléments à la FIN de cette enumeration.
     *  \warning Après modification executer generate_enum_def.py pour exporter cette énumeration vers python
     */
    enum ELEMENT_TYPE {
        ELEMENT_TYPE_CORE_ROOT, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_RESULT_ROOT, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_ROOT, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_GROUPESURFACES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR_PROPRIETES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR_RENDU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSS, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_PROPRIETES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR_RENDU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_SOURCES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_SOURCES_SOURCE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PUISSANCE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_SOURCES_SOURCE_RENDU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_TEXT, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_COLOR, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_POSITION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_LIST, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_INTEGER, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_FLOAT, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_BOOL, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_RENDU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_RENDU_ORIGINE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_RENDU_MODEL, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_SPECTRUMS, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_USER, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_SPECTRUMS_APP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_GROUP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_APP_MATERIAU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_GROUP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_USER_MATERIAU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_PROPMATERIAU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_CATMATERIAL, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_MATERIAU_APP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_MATERIAU_USER, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_GAMMEFREQ_APP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_GAMMEFREQ_USER, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_PROPERTY_FREQ = 49, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_ROW, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_ROW_BFREQ, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_ROW_MATERIAU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_ENCOMBREMENTS, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT,  /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_GAMMEABSORPTION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_CORE_SPPS, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_CORE_CORE_CONFIG, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_USERCONFIGURATION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_RENDU_PARTICULES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_DONNEES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_ENVIRONNEMENTCONF, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_DRAWABLE, /*!< \~french Element pouvant être représenté dans la vue 3D \~english 3D object element */
        ELEMENT_TYPE_CORE_CORE_BFREQSELECTION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_BOOL_BFREQ, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_FOLDER, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_PARTVISUALISATION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_GABE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_GABE_RECP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_TREE_LIST, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_CORE_TC, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_PROJET_INFORMATION, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_BDD_MATERIAUX_MATERIAU_RENDER, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_FONT, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_CORE_TLM, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_GABE_GAP, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_UNKNOWN, /*!< Fichier inconnu par PSPS mais connu par le système d'exploitation */
        ELEMENT_TYPE_CORE_SPPS_OCTREE,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_RPI,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_VOLUMES,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_VOLUMES_VOLUME,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_RENDU,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_PROPRIETES,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_PYTHON_EXTENSION,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_ELEMENT,            /*!< Lors de la déclaration d'un élément utilisateur, ce type permet d'exprimer le fait que l'élément hérite directement de l'élément de base*/
        ELEMENT_TYPE_CORE_CORE,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_FILE,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_STANDART,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_GAIN,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_TR,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_EDT,/*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_PROPRIETES, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEURCOUPE_RENDU, /*!< \~french Type d'élément \~english Built-in element type */
        ELEMENT_TYPE_USER_PREFERENCE_NODE,                     /*!< \~french élément de noeud des préférences utilisateurs \~english User preference node, in the user prefrence tree*/
        ELEMENT_TYPE_USER_PREFERENCE_ITEM,                    /*!< \~french élément des préférences utilisateurs \~english User preference item, in the user prefrence tree*/
        ELEMENT_TYPE_USER_PREFERENCE_ITEM_ISOTEMPLATE,        /*!< \~french élément des préférences utilisateurs qui liste les palettes de couleurs iso \~english User preference item, in the user prefrence tree where user can choose iso palette.*/
        ELEMENT_TYPE_ROW_EXTBFREQ,                            /*!< \~french élément ligne de frequence contenant également une attenuation \~english Extanded freq row property */
        ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES,
        ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_APP,
        ELEMENT_TYPE_SCENE_BDD_DIRECTIVITIES_USER,
        ELEMENT_TYPE_DIRECTIVITIES_APP,
        ELEMENT_TYPE_DIRECTIVITIES_USER,
        ELEMENT_TYPE_FILE,
        ELEMENT_TYPE_REPORT_GABE_RECPS, /*!< Receiver sound level by source */
    };
    /**
     * Indice des états des images
     */
    enum GRAPH_STATE {
        GRAPH_STATE_NORMAL,
        GRAPH_STATE_SELECTED,
        GRAPH_STATE_EXPANDED,
        GRAPH_STATE_SELECTED_EXPANDED,
        GRAPH_STATE_ALL
    };
    /**
     * Indice des images déclaré dans uiTreeCtrl
     */
    enum GRAPH {
        GRAPH_FOLDER,                        /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_ITEM,                            /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_FOLDER_OPEN,                    /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_FITTINGS_OPEN,                /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_FITTINGS_CLOSE,                /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_FITTING_OPEN,                    /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_FITTING_CLOSE,                /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_PUNCTUAL_RECEIVERS_OPEN,        /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_PUNCTUAL_RECEIVERS_CLOSE,        /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_SURFACE_RECEIVERS_OPEN,        /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_SURFACE_RECEIVERS_CLOSE,        /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_SOUND_SOURCES_OPEN,            /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_SOUND_SOURCES_CLOSE,            /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_SURFACES_OPEN,                /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_SURFACES_CLOSE,                /*!< \~french Indice d'une image \~english Built-in picture declaration */
        GRAPH_VOLUMES_CLOSE,
        GRAPH_VOLUMES_OPEN,
        GRAPH_PROJECT_OPEN,
        GRAPH_PROJECT_CLOSE,
        GRAPH_DATA_CLOSE,
        GRAPH_DATA_OPEN,
        GRAPH_USER_MATERIALS_CLOSE,
        GRAPH_USER_MATERIALS_OPEN,
        GRAPH_APPLICATION_MATERIALS_CLOSE,
        GRAPH_APPLICATION_MATERIALS_OPEN,
        GRAPH_DATABASE_CLOSE,
        GRAPH_DATABASE_OPEN,
        GRAPH_USER_SPECTRUMS_CLOSE,
        GRAPH_USER_SPECTRUMS_OPEN,
        GRAPH_APPLICATION_SPECTRUMS_CLOSE,
        GRAPH_APPLICATION_SPECTRUMS_OPEN,
        GRAPH_MATERIAL_CLOSE,
        GRAPH_MATERIAL_OPEN,
        GRAPH_STANDARTCORE_CLOSE,
        GRAPH_STANDARTCORE_OPEN,
        GRAPH_CORES_CLOSE,
        GRAPH_CORES_OPEN,
        GRAPH_SPPSCORE_CLOSE,
        GRAPH_SPPSCORE_OPEN,
        GRAPH_DISK_FOLDER_OPEN,
        GRAPH_DISK_FOLDER_CLOSE,
        GRAPH_ENVIRONMENT,
        GRAPH_PROJECT_AUTHOR,
        GRAPH_INFORMATION,
        GRAPH_DISK_DEFAULT_FILE,
        GRAPH_DISK_GABE,
        GRAPH_DISK_RS,
        GRAPH_DISK_PARTICLE,
        GRAPH_EL_CONFIGURATION,
        GRAPH_EL_3D_DISPLAY,
        GRAPH_EL_POSITION,
        GRAPH_EL_TRIANGLE,
        GRAPH_SPECTRUM,
        GRAPH_ORIGIN,
        GRAPH_TETMESH_PARAMETERS,
        GRAPH_RENDERING_FOLDER_CLOSE,
        GRAPH_RENDERING_FOLDER_OPEN,
        GRAPH_ROOT_MATERIALS_OPEN,
        GRAPH_ROOT_MATERIALS_CLOSE,
        GRAPH_ROOT_SPECTRUMS_OPEN,
        GRAPH_ROOT_SPECTRUMS_CLOSE,
        GRAPH_PUNCTUAL_RECEIVER_OPEN,
        GRAPH_PUNCTUAL_RECEIVER_CLOSE,
        GRAPH_SURFACE_RECEIVER_OPEN,
        GRAPH_SURFACE_RECEIVER_CLOSE,
        GRAPH_SOUND_SOURCE_OPEN,
        GRAPH_SOUND_SOURCE_CLOSE,
        GRAPH_VOLUME_OPEN,
        GRAPH_VOLUME_CLOSE,
        GRAPH_PREF_ANIMATION,
        GRAPH_PREF_GENERAL,
        GRAPH_PREF_LEGEND,
        GRAPH_PREF_NOISE_MAP,
        GRAPH_PREF_PARTICLES,
        GRAPH_USER_PREF_ROOT_CLOSE,
        GRAPH_USER_PREF_ROOT_OPEN,
        GRAPH_USER_PREF_MAINCONFIGURATION_OPEN,
        GRAPH_USER_PREF_MAINCONFIGURATION_CLOSE,
        GRAPH_USER_PREF_HISTORY,
        GRAPH_DIRECTIVITY,
        GRAPH_USER_DIRECTIVITY_OPEN,
        GRAPH_USER_DIRECTIVITY_CLOSE,
        GRAPH_LAST_STATIC_GRAPH        /*!< \~french Dernier icône \~english Last graph id */
    };
    /**
     * Indice des événements générés par les élément
     *  @see ProjectManager::ElementEvent
      *  \warning Après modification executer generate_enum_def.py pour exporter cette énumeration vers python
     */
    enum IDEVENT {
        IDEVENT_DELETE_ELEMENT, /*!< \~french Supprime un element \~english Element delete */
        IDEVENT_RENAME_ELEMENT, /*!< @param  {"name": "new label"} \~french Renommer un élément \~english Element rename  */
        IDEVENT_COPIER,                                /*!< \~french Evenement \~english built-in event */
        IDEVENT_COLLER,                                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_SURFACE_GROUP,                    /*!< \~french Evenement \~english built-in event */
        IDEVENT_GETPROPERTIES,                        /*!< \~french Evenement \~english built-in event */
        IDEVENT_SELECT_TREE_ITEM,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_RECEPTEUR_P,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_SELECT_POSITION,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_SOURCE,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_RECEPTEUR_S,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_USERFREQ,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_ENCOMBREMENT,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_ENCOMBREMENT_CUBOIDE,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_USERMAT,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_NEW_MATERIAL_GROUP,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_RUN_CALCULATION,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_IMPORT_MATERIAL,                /*!< @param {"path": "material file path"} \~french Importer un fichier de matériaux catt ou odeon \~english Import material file from odeon or Catt */
        IDEVENT_LOAD_PARTICLE_SIMULATION,            /*!< \~french Evenement \~english built-in event */
        IDEVENT_LOAD_PARTICLE_SIMULATION_PATH,            /*!< \~french Evenement \~english built-in event */
        IDEVENT_RELOAD_FOLDER,                    /*!< \~french Evenement \~english built-in event */
        IDEVENT_DELETE_FOLDER,                    /*!< \~french Evenement \~english built-in event */
        IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP,        /*!< \~french Evenement \~english built-in event */
        IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP_SUM,    /*!< \~french Evenement \~english built-in event */
        IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM,                /*!< \~french Evenement \~english built-in event */
        IDEVENT_INVERT_FACE_ORIENTATION,        /*!< \~english built-in event \~french Evenement inverser l'orientation des faces sélectionnées */
        IDEVENT_EMPTY_POINTER_VERTEX_GROUP,        /*!< \~english built-in event \~french Evenement vider un pointeur de groupe de surface*/
        IDEVENT_RECP_COMPUTE_ACOUSTIC_PARAMETERS, /*!< \~english built-in event \~french Evenement calculer les paramètre acoustique d'un recepteur ponctuel @param {"C": "Clarté (ms)","D":"Définition (ms)","TR":"Décroissance pour le calcul du TR"} */
        IDEVENT_RECEPTEURS_COMPUTE_TR, /*!< \~english built-in event \~french Evenement calcul de la cartographie du tr */
        IDEVENT_RECEPTEURS_COMPUTE_EDT, /*!< \~english built-in event \~french Evenement calcul de la cartographie de l'edt */
        IDEVENT_RECEPTEURS_COMPUTE_CLARITY, /*!< \~english built-in event \~french Evenement calcul de la cartographie */
        IDEVENT_RECEPTEURS_COMPUTE_DEFINITION, /*!< \~english built-in event \~french Evenement calcul de la cartographie */
        IDEVENT_RECEPTEURS_COMPUTE_TS, /*!< \~english built-in event \~french Evenement calcul de la cartographie */
        IDEVENT_RECEPTEURS_COMPUTE_ST, /*!< \~english built-in event \~french Evenement calcul de la cartographie */
        IDEVENT_REPORT_PARTICULES_MAKE_GABE, /*!< \~english built-in event \~french Evenement création du graph niveau sonore en fonction du temps*/
        IDEVENT_BFREQ_PRESELECTION_NONE, /*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
        IDEVENT_BFREQ_PRESELECTION_THIRD_BAND, /*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
        IDEVENT_BFREQ_PRESELECTION_BAND, /*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
        IDEVENT_BFREQ_PRESELECTION_BUILDING_THIRD_BAND, /*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
        IDEVENT_BFREQ_PRESELECTION_BUILDING_BAND, /*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
        IDEVENT_RECP_COMPUTE_ADVANCED_ACOUSTIC_PARAMETERS, /*!< \~english built-in event \~french Evenement calculer les paramètre acoustique d'un recepteur ponctuel @param  {"LF": "Fraction d'énergie latérale précoce LF (ms)","LFC":"Fraction d'énergie latérale précoce LFC (ms)","LG":"Tenue acoustique latérale LG(ms)","G":"Tenue acoustique G (ms)"} */
        IDEVENT_NEW_RECEPTEURP_GROUP, /*!< \~english built-in event \~french Nouveau groupe de récepteur */
        IDEVENT_NEW_SOURCE_GROUP, /*!< \~english built-in event \~french Nouveau groupe de sources */
        IDEVENT_NEW_SURFACE_GROUP_FROM_SELECTION, /*!< \~english built-in event \~french Nouveau groupe de surface à partir de la sélection */
        IDEVENT_LOAD_RECEPTEURSP_SIMULATION,        /*!< \~english built-in event \~french Chargement de l'animation des récepteurs ponctuels */
        IDEVENT_BUILD_VOLUMES_FROM_TRIMESH,            /*!< \~english built-in event \~french Déclaration des volumes à partir du maillage */
        IDEVENT_NEW_VOLUME,                            /*!< \~english built-in event \~french Ajout d'un volume*/
        IDEVENT_OPEN_FOLDER,                        /*!< \~english built-in event \~french Ouverture d'un dossier */
        IDEVENT_CONVERT_VOL_TO_FITTING,                /*!< \~english built-in event \~french Converti le volume en encombrement scène */
        IDEVENT_NEW_RECEPTEUR_S_COUPE,                /*!< \~english Add a cutting plan receiver event \~french Ajouter une récepteur en coupe */
        IDEVENT_BFREQ_PRESELECTION_ALL,                /*!< \~english built-in event \~french Séléction automatique des bandes de fréquences */
        IDEVENT_NEW_USERDIRECTIV,
		IDEVENT_RECEPTEURS_COMPUTE_STI,
        IDEVENT_LAST_FIXED /* !! This event must be the last, those before are "built-in" and those after are send to python */
    };
    /**
     * @brief Structure de données de base d'un élément
     * Structure de données correspondant aux informations communes à tout les éléments
     */
    struct t_elementInfo {
        ELEMENT_TYPE typeElement;        /*!< Type de l'élément */
        wxTreeItemId idElement;        /*!< Identifiant de l'élément dans l'arbre du projet */
        wxTreeItemId idParent;        /*!< Identifiant de l'élément parent dans l'arbre du projet */
        wxString libelleElement;        /*!< libellé de l'élément dans l'arbre du projet */
        wxInt32 graphElement;            /*!< Indice du graphique associé à l'élément */
        wxInt32 graphElementStateOpen;/*!< Indice du graphique associé à l'élément */
        wxInt32 xmlIdElement;            /*!< Identifiant unique de l'élément*/
        bool expanded;                /*!< Indique si l'élément affiche ses enfants dans l'arbre du projet */
        bool exportLblToCore;            /*!< Indique si le libellé de l'élément sera visible dans le code xml d'exportation vers le coeur de calcul */
        bool hidden;                            /*< Indique si l'élément est invisible pour l'utilisateur */
        bool userDestroyable;            /*!< Indique sur l'utilisateur à le droit de supprimer cet élément (faux par défaut)*/
    };
    /** @} */
    /**
     * Constructeur
     * @param parent		Element parent
     * @param name			Nom de l'élément (libellé dans l'arbre du projet). @ref traductiontuto "Ne pas traduire le libellé".
     * @param _type			Type de l'élément
     * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
     */
    Element(Element *parent, const wxString &name, ELEMENT_TYPE _type, wxXmlNode *nodeElement = NULL);

    /**
     * Destructeur
     * Cet élément se supprime de l'arbre dans lequel il était lié
     * Il supprime également chacun de ses fils.
     */
    virtual ~Element();

    /** @defgroup MethodesEvenement Evénement affectant un élément
     *
     *  Actions de l'utilisateur sur l'élément.
     *  Méthodes virtuelles effectuant un traitement particulier en fonction des actions de l'utilisateur
     *  @see uiTreeCtrl
     *  @see MainPropGrid
     *  @{
     */
    /**
     *  Après avoir sélectionné l'élément dans la fenêtre d'arbre du projet et cliquer-glisser de l'élément
     *  Par défaut l'élément refuse ce type d'opération. La méthode doit être surchargée afin d'accepter le "Drag" de l'élément
     *  @see uiTreeCtrl
     *  @see EndDrag
     *  @param treeEvent Objet lié à l'événement du "Drag"
     *  @param tree Contrôle à l'origine du "Drag"
     */
    virtual void BeginDrag(wxTreeEvent &, wxTreeCtrl *);

    /**
     *  L'utilisateur a sélectionné un ou plusieurs éléments et a relâché ces éléments sur cet élément (Drag&Drop)
     *
     *  Par défaut l'élément refuse ce type d'opération. La méthode doit être surchargée afin d'accepter le "Drop" d'un(des) élément(s)
     *  @see uiTreeCtrl
     *  @see BeginDrag
     *  @param treeEvent Objet lié à l'événement du "Drop"
     *  @param tree Contrôle à l'origine du "Drop"
     *  @param dragLst Cette liste contient les élément issues du glisser "Drag"
     */
    virtual void EndDrag(wxTreeEvent &, wxTreeCtrl *, std::vector<Element *>);

    /**
     * Double clique dans le contrôle de l'arbre du projet
     *  Il n'y a pas de comportement par défaut, toutefois au niveau du contrôle l'élément est déplié et les propriétés sont affiché si celui-ci en possèdes.
     *  @param treeEvent Spécification de l'événement
     *  @see uiTreeCtrl
     *  @see OnExpanding
     *  @see OnCollapsing
     */
    virtual void OnDoubleClic(wxTreeEvent &);

    /**
     *  Clique droit dans le contrôle de l'arbre du projet
     *  Par défaut un menu "popup" est affiché avec l'option Copier et Propriétés (si hasProperties à vrai)
     *  Surcharger cette méthode pour :
     *		- Ne pas afficher de menu
     *		- Ajouter une nouvelle ligne dans le menu (renseigner wxMenu avant d'appeler Element::OnRightClic)
     *
     *  @param leMenu Menu affiché par Element::OnRightClic
     *  @see uiTreeCtrl::OnRightClic()
     */
    virtual void OnRightClic(wxMenu *leMenu);

    /**
     * Retourne un élément de menu en fonction des paramètres
     * @param parent Menu à alimenter (apppend n'est pas appelé)
     * @param id Indice de l'évenement du menu
     * @param label Libellé du menu
     * @param menuIcon Chemin de l'image de l'icone (bmp,png,ico). Chaine vide si aucune icone.
     */
    static wxMenuItem *
    GetMenuItem(wxMenu *parent, int id, const wxString &label, const wxString &menuIcon = wxString());

    static wxMenuItem *
    GetMenuItem(wxMenu *parent, int id, const wxString &label, wxMenu *subMenu, const wxString &menuIcon = wxString());

    /**
     * Rename XML attributes using the path "name" attribute. Usefull for xml upgrade
     * @param root Root XML node
     * @param path Path of navigation (name attribute)
     * @param attributeName Attribute to change value
     * @param newValue New value of the attribute
     * @return root node
     */
	static wxXmlNode* renameAttribute(wxXmlNode* root, const std::vector<wxString>& path, const wxString& attributeName, const wxString& newValue);
    /**
     *  Séléction de l'élément dans le contrôle de l'arbre du projet
     *  Par défaut il n'y a aucun traitement au niveau local de l'élément
     *  @param treeEvent Spécification de l'événement
     *  @see uiTreeCtrl::OnSelectTreeItem()
     */
    virtual void OnSelectTreeItem(wxTreeEvent &);

    /**
     *  L'utilisateur a sélectionné puis cliqué sur le libellé de l'élément ou l'appel de la méthode wxTreeCtrl::EditLabel
     *  Par défaut l'opération d'édition du libellé est refusé. Il faut surcharger cette méthode afin d'accepter l'édition.
     *  @param treeEvent Spécification de l'événement
     *  @see OnEndLabelEdit()
     */
    virtual void OnBeginLabelEdit(wxTreeEvent &);

    /**
     *  L'utilisateur a terminé l'édition du libellé de l'élément
     *  Par défaut l'opération n'a aucun effet sur l'élément. Il faut surcharger cette méthode afin d'utiliser le libellé pour mettre à jour des informations.
     *  @param treeEvent Spécification de l'événement
     *  @see OnBeginLabelEdit()
     */
    virtual void OnEndLabelEdit(wxTreeEvent &);

    /**
     *  L'élément va être replié dans l'arbre du projet
     *  Par défaut l'état de la variable elementInfo est mis à jour (expanded) ainsi que l'image correspondante du contrôle.
     *  @param treeEvent Spécification de l'événement
     *  @see uiTreeCtrl::OnCollapsing()
     *  @see OnCollapsing()
     */
    virtual void OnCollapsing(wxTreeEvent &);

    /**
     *  L'élément va être déplié dans l'arbre du projet
     *  Par défaut l'état de la variable elementInfo est mis à jour (expanded) ainsi que l'image correspondante du contrôle.
     *  @param treeEvent Spécification de l'événement
     *  @see uiTreeCtrl::OnExpanding()
     *  @see OnExpanding()
     */
    virtual void OnExpanding(wxTreeEvent &);

    /**
     *  Appel suite à l'événement Coller (IDEVENT_COLLER::IDEVENT_COLLER)
     *  Par défaut il n'y a aucun traitement. Surcharger cette méthode afin de traiter le noeud XML.
     *  @param nodeElement Noeud XML de l'élément à coller
     *  @see ProjectManager::OnMenuCopy()
     *  @see ProjectManager::OnMenuPaste()
     */
    virtual void OnPaste(wxXmlNode *nodeElement);

    /**
     * Lors-ce que l'élément est supprimé du projet par l'utilisateur cette méthode est appelée.
     * @
     */
    virtual bool OnElementRemoved();
    /** @} */


    /** @defgroup Getxxx Accesseurs Element
     *
     *  Ces fonctions permettent d'obtenir des informations sur l'élément
     *  @{
     */
    /**
     *  Retourne la liste des sous élément propre à cet élément
     *  Surcharger cet élément pour contrôler la liste des propriétés qui seront affiché à partir de cet élément.
     *  Il est par exemple possible de joindre les fils d'un autre élément.
     *  @return Pointeur vers une liste d'éléments
     *  @warning Le pointeur vers le tableau doit être supprimé (delete) sinon une fuite mémoire sera généré.
     */
    virtual void GetChildElement(std::list<Element *> &childs);

    /**
     *  Cette méthode retourne les informations générique de l'élément.
     *  @return Structure de données correspondant aux données de l'élément.
     **/
    const t_elementInfo &GetElementInfos() const;
    /**
     * Indique si cet élément peut être séléctionné en même temps que d'autre élément du même type
     */
    bool IsAllowMultipleSelection() const { return this->allow_multiple_selection; }

    /**
     *  Cette méthode retourne l'indice de l'élément
     *  @return Indice de l'élément
     **/
    wxInt32 GetXmlId() const;

    /**
     * Cet méthode définit l'opérateur de comparaison (inférieur,supérieur) avec un autre élément. Il est utilisé pour trier les éléments dans l'arbre et dans la feuille de propriétés.
     * @param _rEl Element à droite de l'opérateur
     * @return Vrai si cet élément est supérieur
     **/
    virtual bool operator>(Element &_rEl);

    /**
     * Cet méthode définit l'opérateur de comparaison (inférieur,supérieur) avec un autre élément. Il est utilisé pour trier les éléments dans l'arbre et dans la feuille de propriétés.
     * @param _rEl Element à droite de l'opérateur
     * @return Vrai si cet élément est inférieur
     **/
    virtual bool operator<(Element &_rEl);

    /**
     * Que ce soit un élément de l'arbre ou une propriété cette permet permet de cacher ou d'afficher l'objet.
     * @param visible Nouvel état de visibilité
     */
    virtual void Hide(bool visible = false);

    /**
     * Ajoute dans une liste tout les éléments du type passé en paramètre de façon récursive
     * @param typeElement Type recherché
     * @param[out] eleList Liste d'élément qui sera alimentée
     */
    void GetAllElementByType(ELEMENT_TYPE typeElement, std::vector<Element *> &eleList);

    /**
     * Retourne le premier élément trouvé de ce type à tous les niveaux inférieur de cet élément , si non trouvé NULL
     * @warning Cette méthode peut être coûteuse en temps de calcul si il y a beaucoup d'élément dans l'arbre du projet. Il faut limiter le nombre d'appel à cette méthode
     * @param typeElement Type recherché
     * @return Pointeur vers l'élément si trouvé, NULL si non trouvé (toujours vérifier)
     **/
    Element *GetElementByType(ELEMENT_TYPE typeElement);

    /**
     * Retourne le premier élément trouvé ayant cet identifiant à tous les niveaux inférieur de cet élément , si non trouvé NULL
     * wxTreeItemId est l'identifiant procuré par le contrôle en arbre du projet, les éléments de données E_Data n'ont pas cette valeur d'initialisée.
     * @warning Cette méthode peut être coûteuse en temps de calcul si il y a beaucoup d'élément dans l'arbre du projet. Il faut limiter le nombre d'appel à cette méthode
     * @param idElement Indice recherché. Récupérable via le contrôle uiTreeCtrl
     * @return Pointeur vers l'élément si trouvé, NULL si non trouvé (toujours vérifier)
     **/
    Element *GetElementByTreeId(wxTreeItemId idElement);

    /**
     * Retourne le premier élément trouvé ayant cet identifiant à tous les niveaux inférieur de cet élément , si non trouvé NULL
     * Cet identifiant est modifié à chaque chargement du fichier XML du projet.
     * @warning Cette méthode peut être coûteuse en temps de calcul si il y a beaucoup d'élément dans l'arbre du projet. Il faut limiter le nombre d'appel à cette méthode
     * @param _xmlId Indice recherché.
     * @return Pointeur vers l'élément si trouvé, NULL si non trouvé (toujours vérifier)
     **/
    Element *GetElementByXMLId(int _xmlId);

    /**
     * Retourne le premier élément trouvé ayant le libellé (non traduit) correspondant au paramètre. Et ce de facon récursive.
     * @warning Il est préférable d'utiliser GetElementByXMLId voir GetElementByType si possible.
     * @param libelle Libellé recherché
     * @return Pointeur vers l'élément si trouvé, NULL si non trouvé (toujours vérifier)
     **/
    Element *GetElementByLibelle(wxString libelle);

    /**
     * Ajout un élément LifeTimeWitness afin d'effectuer des opération sur un contrôle exterieur quand cet élément est supprimé.
     * @param newWitness Objet à ajouter
     */
    void AppendWitness(const LifeTimeWitness &newWitness);

    /**
     * Ajout un élément LifeTimeWitness afin d'effectuer des opération sur un contrôle exterieur quand cet élément est supprimé.
     * @param newEventCatcher Objet à ajouter
     */
    void AppendEventCatcher(const EventCatcher &newEventCatcher);
    /**
     * Cette méthode permet de connaître si l'élément a besoin d'être sauvegardé dans fichier de projet.
     * @return Vrai si l'élément a été modifié (ou un de ses descendants) depuis la dernière sauvegarde, faux dans le cas contraire.
     **/
    bool IsAskUpdate();

    /**
     * Cette méthode permet de savoir si cet élément a des enfants qui ne seront affichées que si l'utilisateur désire déplier cet élément.
     * A destination de treeCtrl, la vrai question est, les enfants seront t'il généré qu'à l'ouverture dans l'arbre ?
     * @return Vrai si l'élément a des enfants TreeItem cachés
     **/
    virtual bool HasChildren();

    /**
     * Retourne le libellé a utiliser pour trier cet élément par ordre alphabétique
     * @return Libellé de l'élément
     */
    virtual const wxString GetElementNameForOrder() const;

    /**
     * Cette méthode retourne vrai si cet objet a un de ses fils qui héritent de E_Data
     * @return Vrai si l'élément est de type propriété.
     */
    virtual bool IsContainsProperties();

    /**
     * Cette méthode permet de sélectionner les éléments selon un ensemble de type hiérarchisé
     * @param typeTest Type de l'élément
     */
    virtual bool IsFittingWithThisType(const ELEMENT_TYPE &typeTest);

    /**
     * Cette méthode permet de récuperer le père de l'élément.
     * @return Adresse du père si existant. NULL si il n'y a pas de père.
     */
    Element *GetElementParent();
    /** @} */

    /** @defgroup gestionpropelement Gestion des propriétés d'un élément
     *
     *  Ces fonctions permettent de créer, d'obtenir ou de mettre à jour des propriétés
     *  @{
     */

    /** @defgroup majprop Mise à jour des propriétés
     *
     *  Ces fonctions permettent de mettre à jour des propriétés
     *  @{
     */
    /**
     * Met à jour la valeur de la propriété
     * @param name Nom de la propriété
     * @param newValue Nouvelle valeur de la propriété
     * @return Vrai si la propriété existe et a été mis à jour. Faux dans le cas contraire.
     */
    bool UpdateStringConfig(wxString name, wxString newValue);
    /**
     * Met à jour la valeur de la propriété
     * @param name Nom de la propriété
     * @param newValue Nouvelle valeur de la propriété
     * @return Vrai si la propriété existe et a été mis à jour. Faux dans le cas contraire.
     */
    bool UpdateDecimalConfig(wxString name, float newValue);
    /**
     * Met à jour la valeur de la propriété
     * @param name Nom de la propriété
     * @param newIndex Indice de la valeur dans la liste.
     * @return Vrai si la propriété existe et a été mis à jour. Faux dans le cas contraire.
     */
    bool UpdateListConfig(wxString name, int newIndex);
    /**
     * Met à jour la valeur de la propriété
     * @param name Nom de la propriété
     * @param newIndex Indice de la valeur dans la liste.
     * @return Vrai si la propriété existe et a été mis à jour. Faux dans le cas contraire.
     */
    bool UpdateEntierConfig(wxString name, int newValue);
    /**
     * Met à jour la valeur de la propriété
     * @param name Nom de la propriété
     * @param newValue Nouvelle valeur de la propriété
     * @return Vrai si la propriété existe et a été mis à jour. Faux dans le cas contraire.
     */
    bool UpdatePositionConfig(const wxString &name, const vec3 &newValue);
    /**
     * Met à jour la valeur de la propriété
     * @param name Nom de la propriété
     * @param newValue Nouvelle valeur de la propriété
     * @return Vrai si la propriété existe et a été mis à jour. Faux dans le cas contraire.
     */
    bool UpdateBoolConfig(wxString name, bool newValue);
    /** @} */
    /** @defgroup getprop Accesseurs de propriétés
     *  Ces fonctions permettent d'obtenir les valeurs des propriétés. Si le champ n'existe pas un message Debug sera affiché.
     *  @see Element::IsPropertyExist
     *  @{
     */
    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété, chaîne vide si inexistant
     */
    wxString GetStringConfig(wxString name);

    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété, vecteur 0,0,0 si inexistant
     */
    vec3 GetPositionConfig(wxString name);

    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété, 0.0 si inexistant
     */
    float GetDecimalConfig(wxString name);
    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété, faux si inexistant
     */
    bool GetBoolConfig(wxString name);

    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété, 0 si inexistant
     */
    int GetIntegerConfig(wxString name);

    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété,vecteur 0,0,0 (noir) si inexistant
     */
    vec3 GetColorConfig(wxString name);

    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété,-1 si inexistant
     */
    int GetListConfig(wxString name);

    /**
     * Obtient la valeur courante de la propriété
     * @param name Nom de la propriété
     * @return Valeur de la propriété
     */
    wxFont GetFontConfig(const wxString &name);

    /**
    * Obtient la valeur courante de la propriété
    * @param name Nom de la propriété
    * @return Valeur de la propriété
    */
    wxFileName GetFileConfig(wxString name);
    /** @} */

    /** @defgroup addprop Ajout de propriétés
     *  Ces fonctions permettent d'ajouter des propriétés à cet élément
     *  @warning Le libellé en paramètre de ses méthodes @ref traductiontuto "ne dois pas être traduit".
     *  @{
     */
    /**
     * Ajoute un champ de texte dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création, Méthode _("texte de base").Il sera traduit par la suite automatiquement à l'affichage.
     * @param propertyDefaultValue Valeur par défaut du champ. C'est la valeur qu'il y aura lors de la création d'un nouveau projet
     * @param readOnly Lecture seule, à vrai le champ ne pourra pas être modifié par l'utilisateur
     * @param exportToCore A vrai cette propriété sera exporté dans les propriétés du noeud supérieur lors de la génération du code XML à destination des coeurs de calculs.
     * @return Pointeur vers l'élément ajouté, NULL si la propriété existait déjà
     * @see E_Data_Text
     */
    Element *AppendPropertyText(wxString propertyName, wxString propertyLabel, wxString propertyDefaultValue,
                                bool readOnly = false, bool exportToCore = false);

    /**
     * Ajoute un sous élément de position comme fils de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractère alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @param propertyDefaultValue Valeur par défaut du champ. C'est la valeur qu'il y aura lors de la création d'un nouveau projet
     * @param exportToCore A vrai les propriétés x,y,z de cet élément seront exporté dans les propriétés du noeud supérieur lors de la génération du code XML à destination des coeurs de calculs.
     * @see E_Position
     */
    Element *AppendPropertyPosition(wxString propertyName, wxString propertyLabel, vec3 propertyDefaultValue,
                                    bool exportToCore = false);

    /**
     * Ajoute un champ liste dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création,  Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @param defaultValue Valeur par défaut du champ. C'est la valeur qu'il y aura lors de la création d'un nouveau projet
     * @param values Tableau de chaîne de caractères contenant les valeurs visibles de la liste.
     * @param asTitle Ce champ doit-il être visible en premier dans la liste des propriétés
     * @param hSize Combien de cellules ce champ va occuper
     * @param indiceValues Liste des indices correspondant aux valeurs.
     * @see E_Data_List
     */
    Element *
    AppendPropertyList(wxString propertyName, wxString propertyLabel, const std::vector<wxString> &values, long defaultValue,
                       bool asTitle = false, int hSize = 1, const std::vector<int> indiceValues = std::vector<int>(),
                       bool exportToCore = false);

    /**
     * Ajoute un champ de choix de couleur dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création,  Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @param defaultRed Composante de couleur par défaut
     * @param defaultGreen Composante de couleur par défaut
     * @param defaultBlue Composante de couleur par défaut
     * @see E_Data_Color
     */
    Element *AppendPropertyColor(wxString propertyName, wxString propertyLabel, long defaultRed, long defaultGreen,
                                 long defaultBlue);

    /**
     * Ajoute un champ de choix de fichier dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création,  Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @see E_Data_File
     */
    Element *
    AppendPropertyFile(wxString propertyName, wxString propertyLabel, wxString storageFolder, wxString _dialogTitle,
                       wxString _fileExtension);

    /**
     * Ajoute un champ de saisie de nombre dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création,  Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @param propertyDefaultValue Valeur par défaut du champ. C'est la valeur qu'il y aura lors de la création d'un nouveau projet
     * @param exportToCore A vrai cette propriété sera exporté dans les propriétés du noeud supérieur lors de la génération du code XML à destination des coeurs de calculs.
     * @see E_Data_Integer
     */
    Element *AppendPropertyInteger(wxString propertyName, wxString propertyLabel, int propertyDefaultValue,
                                   bool exportToCore = false, bool isMaxValue = false, bool isMinValue = false,
                                   int maxValue = 0, int minValue = 0);

    /**
     * Ajoute un champ de saisie de nombre décimal dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création,  Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @param propertyDefaultValue Valeur par défaut du champ. C'est la valeur qu'il y aura lors de la création d'un nouveau projet
     * @param readOnly Lecture seule, à vrai le champ ne pourra pas être modifié par l'utilisateur
     * @param precision Précision d'affichage au niveau du nombre de chiffres Après la virgule.
     * @param isMaxValue A vrai si une contrainte de valeur maximale est à appliquer
     * @param isMinValue A vrai si une contrainte de valeur minimale est à appliquer
     * @param maxValue Correspond à la valeur maximale de la valeur (si isMaxValue à Vrai )
     * @param minValue Correspond à la valeur minimale de la valeur (si isMinValue à Vrai )
     * @see E_Data_Float
     */
    Element *AppendPropertyDecimal(wxString propertyName, wxString propertyLabel, float propertyDefaultValue,
                                   bool readOnly = false, int precision = 4, bool isMaxValue = false,
                                   bool isMinValue = false, float maxValue = 0, float minValue = 0,
                                   bool exportToCore = false);

    /**
     * Ajoute un champ de coche (CheckBox ) dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création,  Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @param propertyDefaultValue Valeur par défaut du champ. C'est la valeur qu'il y aura lors de la création d'un nouveau projet
     * @see E_Data_Bool
     */
    Element *AppendPropertyBool(wxString propertyName, wxString propertyLabel, bool propertyDefaultValue,
                                bool exportToCore = false);

    /**
     * Ajoute un champ de sélection de police de caractère dans la feuille de propriété de cet élément
     * @param propertyName Nom de la propriété, non visible par l'utilisateur, de préférence se limiter aux caractères alphabétique sans accents
     * @param propertyLabel Libellé du champ, le libellé ne doit PAS être passé par la méthode de traduction pendant sa création,  Méthode _("") .Il sera traduit par la suite automatiquement à l'affichage.
     * @param propertyDefaultValue Valeur par défaut du champ. C'est la valeur qu'il y aura lors de la création d'un nouveau projet
     * @see E_Data_Font
     */
    Element *
    AppendPropertyFont(wxString propertyName, wxString propertyLabel, wxFont propertyDefaultValue = *wxNORMAL_FONT);
    /** @} */

    /** @defgroup propother Autres méthodes des propriétés
     *  Ces fonctions permettent d'autres opérations avec les propriétés de cet élément
     *  @{
     */


    /**
     * Appellé par la fenêtre de propriété avant l'ouverture de l'élément.
     * Surchargé afin d'effectuer des opérations à ce moment de l'exécution
     * @see MainPropGrid::LoadElement()
     */
    virtual void InitProp();

    /**
     * Lu par MainPropGrid Après la modification d'un champ. Si cette méthode retourne vrai alors tout les champs seront actualisés.
     * @return Vrai si tous les champs doivent être réactualisés.
     */
    virtual bool IsAskPropReload();

    /**
     * Active/Désactive la possibilité de modifier le champ pour l'utilisateur pour un champ donné
     * @param name Nom du champ
     * @param readOnly Nouvel état du champ
     * @param col Pour les champ ayant plusieurs colonnes tel que E_Data_Row indique quel colonne sera affecté par la lecture seule.
     */
    void SetReadOnlyConfig(const wxString &name, bool readOnly = true, int col = 0);

    /**
     * Lit la possibilité de modifier le champ pour l'utilisateur pour un champ donné
     * @param name Nom du champ
     * @param col Pour les champ ayant plusieurs colonnes tel que E_Data_Row indique quelle colonne lire
     */
    bool IsReadOnlyConfig(const wxString &name, int col = -1);

    /**
     * Active/Désactive la possibilité de modifier tout les champs pour l'utilisateur pour une colonne donnée
     * @param readOnly Nouvel état des champs
     * @param col Pour les champ ayant plusieurs colonnes tel que E_Data_Row indique quel colonne sera affectée par la lecture seule.
     */
    void SetReadOnlyAllConfig(bool readOnly = true, int col = 0);

    /**
     * Retourne le libellé de la colonne pour ce champ.
     * @param col Pour les champs ayant plusieurs colonnes tel que E_Data_Row indique quel colonne nous désirons connaître le libellé
     * @return Libellé de la colonne
     */
    virtual wxString GetColTitle(int col);

    /**
     * Lie un élément de propriété avec un menu. De tel sorte que la mise à jour du champ mette également à jour le menu.
     * @param name Nom du champ
     * @param menuItem Pointeur vers l'élément du menu
     */
    void LinkMenuWithPropBool(wxString name, wxMenuItem *menuItem);
    /**
     * Permet de rechercher parmis les propriétés de cet élément si une propriété porte le nom en paramètre
     * @param propertyName Nom du champ
     * @return Vrai si ce champ est parmis les propriétés de cet élément
     */
    bool IsPropertyExist(wxString propertyName, Element **found = NULL);

    /** @} */
    /** @} */

    /** @defgroup elemxmlmethode Méthodes en rapport avec le traitement XML
     *  Ces méthode permettent l'interaction entre les éléments et les fichiers XML
     *  @{
     */
    /**
     * @brief Sauvegarde XML de l'élément
     *
     * Met à jour ou ajoute les informations de cet élément et celles de ces enfants dans le noeud Xml NoeudParent
     * @param NoeudParent Noeud parent de l'élément
     * @return NoeudCourant XML de l'élément.
     */
    virtual wxXmlNode *SaveXMLDoc(wxXmlNode *NoeudParent);

    /**
     * Met à jour ou ajoute les informations de cet élément uniquement dans le noeud Xml NoeudParent
     * @param NoeudParent Noeud parent de l'élément
     * @return NoeudCourant XML de l'élément.
     */
    virtual wxXmlNode *SaveXMLDocWithoutChildren(wxXmlNode *NoeudParent);

    /**
     * Ajoute les informations de cet élément destinées au coeur de calcul.
     * @param NoeudParent Noeud parent de l'élément
     * @return NoeudCourant XML de l'élément.
     */
    virtual wxXmlNode *SaveXMLCoreDoc(wxXmlNode *NoeudParent);

    /**
     * Ajoute les informations de cet élément et celles de ses fils au noeud XML. Pour l'instant ces informations sont envoyés au presse papier.
     * @param NoeudParent Noeud parent de l'élément
     * @return NoeudCourant XML de l'élément.
     */
    wxXmlNode *ExportToXMLDoc(wxXmlNode *NoeudParent);

    /** @} */


    /** @defgroup elemtreectrlmethode Méthodes en rapport avec le traitement du contrôle en arbre
     *  Ces méthode permettent l'interaction entre les éléments et le contrôle uiTreeCtrl
     *  @{
     */
    /**
     * Ajoute cet élément ainsi que ces fils dans le contrôle passé en paramètre.
     * @param treeToFeed Pointeur vers le contrôle à renseigner. Si non spécifié le pointeur local sera utilisé (si assigné).
     **/
    virtual void FillWxTree(wxTreeCtrl *treeToFeed = NULL, const wxTreeItemId &parentId = wxTreeItemId());

    /**
     * Affecte l'icone de l'élément dans la vue en arbre
     * @param iconState Etat de l'icone
     * @param staticGraphId Indice de l'icone
     */
    void SetIcon(const Element::GRAPH_STATE &iconState, const Element::GRAPH &staticGraphId);

    /**
     * Supprime un élément descendant à cet élément ayant l'identifiant égal au paramètre.
     * @return Vrai si un élément a été supprimé.
     */
    virtual bool DeleteElementByTreeId(wxTreeItemId idElement);

    /**
     * Supprime un élément fils ayant cet identifiant. La suppression s'arrête au niveau fils seulement.
     */
    virtual bool DeleteElementByXmlId(const wxInt32 &xmlIdElement, bool setModification = true);

    /**
     * Réinitialise l'identifiant de cet élément. A partir de cet élément il ne sera pas possible de retrouver son équivalent dans uiTreeCtrl
     */
    void UnsetTreeId();

    /**
     * Initialise l'identifiant de cet élément.
     * @param _idElement Indice de cet élément dans le contrôle uiTreeCtrl
     */
    void SetTreeItemId(wxTreeItemId _idElement);

    /**
     * Pour l'affichage dans l'arbre retourne le libellé de l'élément
     */
    virtual wxString GetTreeLabel() const { return elementInfo.libelleElement; }
    /** @} */

    /** @defgroup elemtoelementmethode Méthodes en rapport avec les autres élément du projet
     *  Ces méthode permettent l'interaction entre cet éléments et les autres éléments
     *  @{
     */

    /**
     * Compte le nombre de fils de cet élément
     * @return Nombre de fils
     */
    int CountNbChildrens() { return fils.size(); }

    /**
     * Supprime tout les fils immédiat ayant un type égal au paramètre
     * @param typeElement Type des éléments à supprimer
     */
    void DeleteAllElementByType(ELEMENT_TYPE typeElement);

    /**
     * Supprime tout les fils et descendant des fils ayant un type égal au paramètre
     * @param typeElementToDelete Type des éléments à supprimer
     */
    virtual void DeleteAllElementByTypeR(ELEMENT_TYPE typeElementToDelete);

    /**
     * Lors de la mise à jour de cet élément ou d'un de ses enfants, cet élément sera redessiné dans la vue 3D.
     * @return Vrai si cet élément (ou un de ses parents) est visible dans la vue 3D
     * @see E_Drawable
     */
    bool IsDrawable();

    /**
     * Lors de la mise à jour de cet élément ou d'un de ses enfants, cet élément sera redessiné dans la vue 3D.
     * @see E_Drawable
     * @see IsDrawable()
     */
    void SetDrawable();

    /**
     * Cette méthode doit être appelé si une variable de votre classe a été mis à jour et que vous désirez que la méthode SaveXMLDoc() soit appelée pour sauvegarder cette variable.
     */
    virtual void Modified(Element *elementUpdated); //Si cet objet a besoin d'etre mis a jour
    /**
     * Appel la méthode Modified() pour cet élément et pour les éléments descendants.
     */
    void ForceBottomModified();

    /**
     * Retourne l'indice courant représentant le nombre d'élément (XmlId le plus grand du projet)
     * @return Indice
     */
    static long GetElementCount();

    /**
     * Increment the value and return the old count
     * @warning La valeur est automatiquement incrémenté à chaque ajout d'un élément. Cette méthode peut être appelé uniquement si aucun élément n'a la valeur xmlIdElement > au paramètre indiqué !
     * @param newValue Nouvelle valeur de la variable statique nbElement de la classe Element.
     */
    static long IncElementCount();

    /*
    * Set the element count, called only by project.cpp when starting a new project
    */
    static void SetElementCount(long elCount);

    /**
     * Supprime le lien Pere(this)->Enfant(ChildToSeparate)
     * @warning Ce lien permet notamment de libérer l'espace mémoire lors de la fermeture du projet. Vous devez absolument relier cet objet a un autre père sinon cela générera une fuite mémoire ! De préférence utilisez la méthode Reparent() .
     * @param ChildToSeparate Element fils qui doit être séparé du père
     * @see Reparent()
     */
    void BrokeLinkToChild(Element *ChildToSeparate);

    /**
     * Cette méthode effectue toutes les opérations nécessaires pour modifier le père de cet élément.
     * @param newParent Nouvel élément parent de l'objet.
     */
    void Reparent(Element *newParent);

    /**
     * @brief Ajouter un nouveau fils à cet élément
     *
     * Cet élément sera sauvegardé, toutefois si ce type d'élément n'est pas connue par l'élément père (n'est pas initialisé dans le constructeur XML de l'élément)
     * alors cet élément ne sera pas créé suite au chargement du projet.
     *
     */
    Element *AppendFils(Element *filsToAppend, bool appendToTree = false);

    /**
     * Cette fonction permet l'ajout d'un fils à partir d'une classe exterieure sans avoir
     * besoin de connaitre l'élément hérité ni l'élément à creer.
     * Cette fonction ne fonction qu'avec les éléments compatibles (ayant implémenté cette méthode)
     * @return L'élément ajouté ou NULL si aucun élément ajouté.
     */
    virtual Element *AppendFilsByType(ELEMENT_TYPE etypefils, const wxString &libelle = wxString());

    /** @} */

    /** @} */
protected:
    //Methodes
    /**
         * Active la multiséléction avec cet élément
         */
    void AllowMultipleSelection(const bool &allow = true);

    /**
     * Retourne le compteur nbElement. Synonyme de GetCompteur
     * @return Indice maximal
     **/
    int GetNbElement() { return nbElement; };

    /**
     * Cette méthode permet de connaître le nombre de noeuds fils du noeud NoeudCourant
     * @param NoeudCourant Noeud xml à tester
     * @return nombre de fils immédiat
     */
    static long CountXmlNodeChildren(wxXmlNode *NoeudCourant);

    bool ExportMode; /*!< Si cette variable est à vrai cela veut dire que l'element courant est en mode d'exportation, c'est à dire qu'il ne s'agit pas de la sauvegarde du projet (lors d'un copier sur un élément) */


    /**
     * En se référent à la propriété XmlId re cet élément et celles des noeuds fils XML, recherche le noeud correspondant à this, si il n'y a aucun noeud fils correspondant retourn NULL
     * @param noeudParent Noeud XML parent
     * @return Noeud XML courant
     */
    wxXmlNode *SeekParentNodeToThisNode(wxXmlNode *noeudParent);

    /**
     * Vrai si cet élément est visible dans la vue 3D
     * @see IsDrawable
     */
    bool drawable;
    /**
     * Données de l'élément
     * @see GetElementInfos
     */
    t_elementInfo elementInfo;
    /**
     * Pointeur vers l'élément parent. Initialiser à NULL
     * @see GetElementParent
     */
    Element *pere = NULL;
    /**
     * Liste des éléments fils
     *
     */
    std::list<Element *> fils;
    std::list<LifeTimeWitness> witnesses;    /** Cette liste contient les objets permettant**/
    std::list<EventCatcher> evt_catchlst;    /** Ces objet recoivent les évenement de cet objet tel que les mis à jour*/
    /**
     * Cetélément ou au moins un de ses fils a besoin d'être mis à jour dans le document XML
     */
    bool needUpdate;
    /**
     * A vrai le menu "Coller" est affiché
     */
    bool insertPasteMenu;
    /**
     * A vrai l'utilisateur peut séléctionner plusieurs éléments de ce type en même temps
     */
    bool allow_multiple_selection;
};


#endif

