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


/*! \page addElement Ajout d'un �l�ment au sein du projet
 * 
 * Voici la marche � suivre pour ajouter un nouveau type de donn�es de projet dans un des trois arbres (scene,core ou resultats)
 *
 * \section addElementstepone Cr�er le fichier d'ent�te
 * Il faut cr�er le fichier vide ".h" dans le dossier correspondant � l'arbre o� l'�l�ment sera pr�sent :
 *	- data_manager\\tree_scene\\ Pour les �l�ments du premier onglet
 *	- data_manager\\tree_core\\ Pour les �l�ments du deuxi�me onglet
 *	- data_manager\\tree_rapport\\ Pour les �l�ments du troisi�me onglet
 *
 * Le nom du fichier doit donner une indication sur sa place dans la hi�rarchie, il suffit de se r�f�rer aux noms des autres �l�ments.
 *
 * \section addElementsteptwo Constructeurs de base d'un �l�ment
 * Il faut directement h�riter de Element pour un �l�ment n'ayant pas de comportement sp�cial (un dossier ou un �l�ment de propri�t�)
 *
 * \include demos/demos_add_element.h
 * Le premier constructeur est le constructeur XML. Il est appel� lors du chargement d'un projet existant.
 *
 * Le deuxi�me constructeur est le constructeur de cr�ation. Il est appel� lors de la cr�ation d'un noueau projet ou en cours d'execution du programme.
 * La m�thode SaveXMLDoc() est la surcharge permettant de sauvegarder les donn�es propres � l'�l�ment pour une r�cuperation ult�rieure dans le constructeur XML.
 *
 * @warning Il faut donner un indice � ce nouvel �l�ment en rajoutant une ligne � la fin de l'�numeration des �l�ments pr�sent dans element.h, voir Element::ELEMENT_TYPE .
 *
 * \section addElementstepthree Instanciation de la classe
 * Cette classe n'est pour l'instant pas utilis�. Pour qu'elle le soit il faut ajouter le code d'instanciation de notre nouvelle classe.
 * Pour cela il faut ouvrir le fichier d'entete de l'�l�ment parent.
 * Voici un exemple afin de cr�er toujours l'�l�ment demo � la cr�ation de E_Scene_Projet et charger le ou les �l�ment Demo au chargement d'un projet.
 * \include demos/demos_add_element_parent.h
 * Comme vous pouvez le voir il y a 3 modifications :
 *	- Ajout du fichier d'entete de la classe 
 *	  \skipline e_scene_projet_demo
 *	- Les deux lignes de test lors du chargement du fichier XML (Test d'�galit� des indices)
 *	  \skip MARKERBEGIN
 *	  \until MARKEREND
 *	- La ligne de cr�ation dans le deuxi�me constructeur.
 *	  \skipline MARKERAPPEND
 *
 * Compilez, executez le projet et cliquez sur "Nouveau projet". Vous pouvez voir votre �l�ment dans l'arbre du projet. Cependant l'int�raction avec cet �l�ment est tr�s limit�e.
 *
 * \section addElementstepfour Ajout de propri�t�s (facultatif)
 * La classe Element dispose de plusieurs m�thodes afin de contenir des propri�t�s tel qu'un champ texte et un champ de couleur (@ref addprop "Ensemble de m�thodes"):
 * Reprenons notre exemple. Nous allons ajouter un champ de texte et un champ de couleur.
 * 
 * \include demos/demos_add_element_withprop.h
 * 
 * On a seulement ajout� ces lignes :
 * \skip MARKERBEGIN
 * \until MARKEREND
 *
 * Vous vous demandez peut �tre quel est l'utilit� des lignes comportant uniquement @ref traductionTuto "la m�thode de traduction" ?
 *
 * En fait la raison est simple. Le libell� que vous renseignez dans l'appel de la m�thode d'ajout de propri�t� sera sauvegard� dans le fichier de projet comme �tant une chaine non traduite.
 * Si jamais vous utilisez la m�thode de traduction wxGetTranslation( ) dans ses param�tres alors les libell� des propri�t�s d'un projet qui sera �chang� entre un anglais et un francais resteront en anglais.
 * Donc utiliser la m�thode de traduction en dehors des propri�t�s permet au champ d'�tre traduit uniquement lors de l'affichage du libell� et que cette chaine � traduire soit reconnue par le programme de mise � jour des chaines � traduire.
 *
 * Vous pouvez ensuite utiliser \ref getprop "les accesseurs de propri�t�s" pour lire ses donn�es. Consultez \ref gestionpropelement "la documentation relative" pour plus de d�tails.
 *
 * \section addElementstepfive Rendu 3D de l'�l�ment
 *
 * Nous allons modifier notre �l�ment de d�monstration pour le dessiner sur la vue 3D.
 * \include demos/demos_add_element_drawable.h
 *
 * En premier lieu il ne faut plus inclure directement element.h mais drawable_element.h
 * \skipline drawable_element
 *
 * Nous n'allons plus h�riter directement de Element. On va remplacer la classe d'h�ritage par E_Drawable, cette classe permet justement le rendu 3D d'un �l�ment.
 * \skipline MARKER0
 * Il en va de m�me pour les deux constructeurs.
 *
 * Le constructeur XML
 * \skip MARKER1
 * \until {
 * Le constructeur de cr�ation
 * \skip MARKER2
 * \until {
 * 
 * La modification consiste �galement � la surcharge de la m�thode E_Drawable::DrawItem() afin de commander les m�thodes de dessins adapt�es � notre nouvel �l�ment.
 * \skip DrawItem
 * \until }
 * 
 * Le libell� de l'�l�ment est d�s � pr�sent visible dans la vue 3D. Les modifications des propri�t�s sont visibles imm�diatement dans la vue 3D.
 */
