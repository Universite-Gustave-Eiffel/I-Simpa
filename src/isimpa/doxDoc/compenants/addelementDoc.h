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


/*! \page addElement Ajout d'un élément au sein du projet
 * 
 * Voici la marche à suivre pour ajouter un nouveau type de données de projet dans un des trois arbres (scene,core ou resultats)
 *
 * \section addElementstepone Créer le fichier d'entête
 * Il faut créer le fichier vide ".h" dans le dossier correspondant à l'arbre où l'élément sera présent :
 *	- data_manager\\tree_scene\\ Pour les éléments du premier onglet
 *	- data_manager\\tree_core\\ Pour les éléments du deuxième onglet
 *	- data_manager\\tree_rapport\\ Pour les éléments du troisième onglet
 *
 * Le nom du fichier doit donner une indication sur sa place dans la hiérarchie, il suffit de se référer aux noms des autres éléments.
 *
 * \section addElementsteptwo Constructeurs de base d'un élément
 * Il faut directement hériter de Element pour un élément n'ayant pas de comportement spécial (un dossier ou un élément de propriété)
 *
 * \include demos/demos_add_element.h
 * Le premier constructeur est le constructeur XML. Il est appelé lors du chargement d'un projet existant.
 *
 * Le deuxième constructeur est le constructeur de création. Il est appelé lors de la création d'un noueau projet ou en cours d'execution du programme.
 * La méthode SaveXMLDoc() est la surcharge permettant de sauvegarder les données propres à l'élément pour une récuperation ultérieure dans le constructeur XML.
 *
 * @warning Il faut donner un indice à ce nouvel élément en rajoutant une ligne à la fin de l'énumeration des éléments présent dans element.h, voir Element::ELEMENT_TYPE .
 *
 * \section addElementstepthree Instanciation de la classe
 * Cette classe n'est pour l'instant pas utilisé. Pour qu'elle le soit il faut ajouter le code d'instanciation de notre nouvelle classe.
 * Pour cela il faut ouvrir le fichier d'entete de l'élément parent.
 * Voici un exemple afin de créer toujours l'élément demo à la création de E_Scene_Projet et charger le ou les élément Demo au chargement d'un projet.
 * \include demos/demos_add_element_parent.h
 * Comme vous pouvez le voir il y a 3 modifications :
 *	- Ajout du fichier d'entete de la classe 
 *	  \skipline e_scene_projet_demo
 *	- Les deux lignes de test lors du chargement du fichier XML (Test d'égalité des indices)
 *	  \skip MARKERBEGIN
 *	  \until MARKEREND
 *	- La ligne de création dans le deuxième constructeur.
 *	  \skipline MARKERAPPEND
 *
 * Compilez, executez le projet et cliquez sur "Nouveau projet". Vous pouvez voir votre élément dans l'arbre du projet. Cependant l'intéraction avec cet élément est trés limitée.
 *
 * \section addElementstepfour Ajout de propriétés (facultatif)
 * La classe Element dispose de plusieurs méthodes afin de contenir des propriétés tel qu'un champ texte et un champ de couleur (@ref addprop "Ensemble de méthodes"):
 * Reprenons notre exemple. Nous allons ajouter un champ de texte et un champ de couleur.
 * 
 * \include demos/demos_add_element_withprop.h
 * 
 * On a seulement ajouté ces lignes :
 * \skip MARKERBEGIN
 * \until MARKEREND
 *
 * Vous vous demandez peut être quel est l'utilité des lignes comportant uniquement @ref traductionTuto "la méthode de traduction" ?
 *
 * En fait la raison est simple. Le libellé que vous renseignez dans l'appel de la méthode d'ajout de propriété sera sauvegardé dans le fichier de projet comme étant une chaine non traduite.
 * Si jamais vous utilisez la méthode de traduction _( ) dans ses paramètres alors les libellé des propriétés d'un projet qui sera échangé entre un anglais et un francais resteront en anglais.
 * Donc utiliser la méthode de traduction en dehors des propriétés permet au champ d'être traduit uniquement lors de l'affichage du libellé et que cette chaine à traduire soit reconnue par le programme de mise à jour des chaines à traduire.
 *
 * Vous pouvez ensuite utiliser \ref getprop "les accesseurs de propriétés" pour lire ses données. Consultez \ref gestionpropelement "la documentation relative" pour plus de détails.
 *
 * \section addElementstepfive Rendu 3D de l'élément
 *
 * Nous allons modifier notre élément de démonstration pour le dessiner sur la vue 3D.
 * \include demos/demos_add_element_drawable.h
 *
 * En premier lieu il ne faut plus inclure directement element.h mais drawable_element.h
 * \skipline drawable_element
 *
 * Nous n'allons plus hériter directement de Element. On va remplacer la classe d'héritage par E_Drawable, cette classe permet justement le rendu 3D d'un élément.
 * \skipline MARKER0
 * Il en va de même pour les deux constructeurs.
 *
 * Le constructeur XML
 * \skip MARKER1
 * \until {
 * Le constructeur de création
 * \skip MARKER2
 * \until {
 * 
 * La modification consiste également à la surcharge de la méthode E_Drawable::DrawItem() afin de commander les méthodes de dessins adaptées à notre nouvel élément.
 * \skip DrawItem
 * \until }
 * 
 * Le libellé de l'élément est dès à présent visible dans la vue 3D. Les modifications des propriétés sont visibles immédiatement dans la vue 3D.
 */
