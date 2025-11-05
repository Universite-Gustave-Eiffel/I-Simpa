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

/*! \mainpage Manuel du développeur I-SIMPA

\section intro_sec Introduction

Voici le manuel développeur de l'application "Interface de SIMulation de Propagation Acoustique" (I-SIMPA)

Ce document est décomposé en trois parties :
	- Les explications nécessaires à la compilation des sources ;
	- Des tutoriels afin d'ajouter rapidement les fonctionnalités les plus communes ;
	- Une spécification complète des classes et des membres publiques de celles-ci.

\section install_sec Installation
 Après avoir installé Visual C++ 2005 Express (ou supérieur), il est nécessaire d'installer la bibliothèque WxWidgets.
 Dans le fichier de configuration de WxWidgets "setup.h", il faut activer les paramètres suivants (ils sont à 0 par défaut) :
		- \#define wxUSE_GLCANVAS       1
		- \#define wxUSE_REGEX       1

 Pour le mode de distribution (compilation release) il faut modifier un paramètre pour chaque module de WxWidgets utilisé par I-SIMPA
 Une fois le projet I-SIMPA chargé, dans l'explorateur de solutions sélectionnez tout les projets autres que I-SIMPA. Cliquez bouton droit puis dans le menu popup sélectionnez "Propriétés". (ils doivent être rechargés au préalable)
 Dans l'arbre à gauche :
	- Propriétés de configuration
		- C/C++
			- Génération de code
 Modifier la valeur de "Bibliothèque runtime" pour MultiThread (//MT)

\see http://www.wxwidgets.org/wiki/index.php/Microsoft_Visual_CPP_Guide

Liste des tutoriels :
	- \subpage traductiontuto ;
	- \subpage addelement ;
	- \subpage doxygentuto ;


*/
