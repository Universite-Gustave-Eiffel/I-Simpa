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

/*! \page addelement Ajout d'un élément au sein du projet
 * 
 * Un élément est un noeud ou une feuille visible dans un des trois arbres (scene,core ou resultats). Ce sont ces éléments qui servent à organiser les données du projet.
 *
 * \section addElementstepone Créer le fichier d'entête
 * Il faut créer le fichier vide ".h" dans le dossier correspondant à l'arbre où l'élément sera présent :
 *	- "data_manager\\tree_scene\\"		Pour les éléments du premier onglet
 *	- "data_manager\\tree_core\\"		Pour les éléments du deuxième onglet
 *	- "data_manager\\tree_rapport\\"	Pour les éléments du troisième onglet
 *
 * Le nom du fichier doit donner une indication sur sa place dans la hiérarchie, il suffit de se référer aux noms des autres éléments.
 *
 * \section addElementsteptwo Constructeurs de base d'un élément
 * Il est impératif d'hériter directement ou indirectement de la classe Element pour pouvoir être affiché dans l'arbre du projet.
 *
 * \include demos/demos_add_element.h
 * Le premier constructeur est le constructeur XML. Il est appelé lors du chargement d'un projet existant.
 *
 * Le deuxième constructeur est le constructeur de création. Il est appelé lors de la création d'un nouveau projet ou en cours d'exécution du programme.
 * La méthode SaveXMLDoc() est la surcharge permettant de sauvegarder les données propres à l'élément pour une récupération ultérieure dans le constructeur XML.
 *
 * @warning Il faut donner un indice à ce nouvel élément en rajoutant une ligne à la fin de l'énumération des éléments présent dans element.h, voir Element::ELEMENT_TYPE .
 *
 * \section addElementstepthree Instanciation de la classe
 * Cette classe n'est pour l'instant pas utilisée. Pour qu'elle le soit, il faut ajouter le code d'instanciation de notre nouvelle classe.
 * Pour cela il faut ouvrir le fichier d'entête de l'élément parent.
 * Voici un exemple afin de créer l'élément demo aprés la création de E_Scene_Projet au moment de la création d'un nouveau projet.
 * \include demos/demos_add_element_parent.h
 * Comme vous pouvez le voir il y a 3 modifications :
 *	- Ajout du fichier d'entete de la classe ;
 *	  \skipline e_scene_projet_demo
 *	- Les deux lignes de test lors du chargement du fichier XML (Test d'égalité des indices);
 *	  \skip ELEMENT_TYPE_SCENE_PROJET_DEMO
 *	  \until E_Scene_Projet_Demo
 *	- La ligne de création dans le deuxième constructeur;
 *	  \skipline E_Scene_Projet_Demo
 *
 * Compilez, exécutez le projet et cliquez sur "Nouveau projet". Vous pouvez voir votre élément dans l'arbre du projet. Cependant l'interaction avec cet élément est très limitée.
 *
 * \section addElementstepfour Ajout de propriétés (facultatif)
 * La classe Element dispose de plusieurs méthodes afin de contenir des propriétés tel qu'un champ texte et un champ de couleur (@ref addprop "Ensemble de méthodes"):
 *
 * Reprenons notre exemple; nous allons ajouter un champ de texte et un champ de couleur.
 * 
 * \include demos/demos_add_element_withprop.h
 * 
 * On a seulement ajouté ces lignes :
 * \skip AppendPropertyText
 * \until monstration");
 *
 * L'utilité des lignes comportant uniquement @ref traductiontuto "la méthode de traduction" est la suivante :
 *
 * Le libellé que vous renseignez dans l'appel de la méthode d'ajout de propriété sera sauvegardé dans le fichier de projet comme étant une chaîne non traduite.
 * Si jamais vous utilisez la méthode de traduction wxGetTranslation( ) dans ses paramètres, alors les libellés des propriétés d'un projet qui sera échangé entre un anglais et un français resteront en anglais.
 *
 * Donc le fait d'utiliser la méthode de traduction en dehors des propriétés permet au champ d'être traduit uniquement lors de l'affichage du libellé. Cette chaîne à traduire est alors reconnue par le programme de mise à jour des chaînes à traduire.
 *
 * Vous pouvez ensuite utiliser \ref getprop "les accesseurs de propriétés" pour lire ses données. Consultez \ref gestionpropelement "la documentation relative" (Gestion des propriétés des éléments) pour plus de détails.
 *
 * \section addElementstepfive Rendu 3D de l'élément
 *
 * Nous allons modifier notre élément de démonstration pour le dessiner sur la vue 3D.
 * \include demos/demos_add_element_drawable.h
 *
 * En premier lieu il ne faut plus inclure directement element.h mais drawable_element.h
 * \skipline drawable_element
 *
 * Ce nouvel élément ne va plus hériter directement de Element mais de E_Drawable, cette classe permet le rendu 3D d'un élément.
 * \skipline public E_Drawable
 * Il en va de même pour les deux constructeurs.
 *
 * Le constructeur XML :
 * \skip E_Scene_Projet_Demo( wxXmlNode
 * \until {
 * Le constructeur de création :
 * \skip E_Scene_Projet_Demo( Element
 * \until {
 * 
 * La modification consiste également à la surcharge de la méthode E_Drawable::DrawItem() afin de commander les méthodes de dessin adaptées à notre nouvel élément.
 * \skip DrawItem
 * \until }
 * 
 * Le libellé de l'élément est dès à présent visible dans la vue 3D. Les modifications des propriétés sont visibles immédiatement dans la vue 3D.
 */
