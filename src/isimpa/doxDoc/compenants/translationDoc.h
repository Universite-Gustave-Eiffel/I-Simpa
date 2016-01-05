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


/*! \page traductionTuto Note sur la gestion de la traduction
 * 
 * Ce logiciel est destiné à être traduit dans plusieurs langues.
 *
 * Le système de traduction utilisé est un des plus commun et est surtout natif à WxWidgets
 *
 * \section addTraduction Traduire un texte
 * Il n'y a qu'une seule opération à faire pour traduire un texte.
 *
 * Voici une chaine non traduite :
 * \dontinclude demos/demo_translation_1.h
 *
 * \skipline MARKERWRONG
 *
 * A chaque fois qu'une chaine de texte sera visible par l'utilisateur il faut l'entourer d'une méthode globale comme ceci :
 *
 * \skipline MARKEROK
 *
 * \section warningtraductionTuto Attention
 * 
 * Le language de base de l'application est en francais. Ensuite il y a un certains nombre de dictionnaires associés.
 *
 * Le dictionnaire francais est là pour corriger les textes de bases.
 * @warning Si le texte de base est modifié alors toutes les traductions ne seront plus effectuées jusqu'à la mise à jour manuel de chaque dictionnaire. Il faut donc uniquement faire les corrections dans le dictionnaire francais !
 *
 *
 * \section specialtraductionTuto Cas spéciaux
 * 
 * Certaines méthodes traduisent elles mêmes les chaines de caractères pour une bonne raison. Dans la plupart des cas cette raison est de ne pas inclure de chaines traduites au sein du fichier de projet.
 * 
 * Voici une liste non exhaustive de ces méthodes :
 *	- @ref Element::Element() "Libellé de l'élément"
 *	- @ref addprop
 *
 * Dans la documentation d'une méthode il est indiqué dans un bloc d'avertissement si un paramètre ne doit pas être traduit au préalable.
 * \section dictionnarytraductionTuto Modifier ou ajouter un dictionnaire
 * L'ajout ou la modification de dictionnaire est simplifié par l'utilisation du logiciel poEdit.
 * @see http://www.poedit.net
 * 
 * Une fois le logiciel installé se placer dans le dossier de langue.Se placer dans /fr/ pour francais.
 *
 * Le fichier internat.po correspond au fichier de configuration ascii qui peut être lu par poEdit.
 *
 * Ouvrez ce fichier avec poEdit.
 *
 * Pour actualiser les chaines de traductions depuis le code source du projet :
 *	- Ouvrez le menu "Catalogue"
 *	- Cliquez sur "Mise à jour depuis les sources"
 *
 * Vous obtenez un rapport sur les modifications et les nouvelles chaines sont visibles dans la liste.
 * 
 * Double-cliquez sur la ligne à modifier et entrez le texte traduit.
 *
 * Enregistrez le fichier. Aprés enregistrement le fichier internat.mo sera créé. C'est la version binaire du fichier de traduction. La seule compréhensible par notre application.
 */