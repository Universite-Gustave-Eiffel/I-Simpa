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

#include "first_header_include.hpp"

#include "legendObject.h"
#include <wx/font.h>
#include <wx/colour.h>
/**
 * @file legendFonts.h
 * Gestionnaire de création d'objet de légende à partir d'une chaine de caractère
 */


#ifndef __LEGEND_FONT__
#define __LEGEND_FONT__

namespace legendRendering
{
	/*
	 * Gestionnaire de création d'objet de légende à partir d'une chaine de caractère
	 */
	class legendText : public legendObject
	{
		public:
			legendText();


			/**
			 * Appliquer une couleur de texte. La couleur est noir par défaut.
			 * @param colour Nouvelle couleur. 
			 * @warning Il est préférable d'appeler cette méthode avant d'avoir initialisé le texte.
			 */
			void SetTextForeground(const wxColour& colour);
			/**
			 * Appliquer une couleur de l'arrière plan du texte. Il n'y a aucune couleur par défaut.
			 * @param colour Nouvelle couleur. 
			 * @warning Il est préférable d'appeler cette méthode avant d'avoir initialisé le texte.
			 */
			void SetTextBackground(const wxColour& colour);
			/**
			 *	Appliquer une police de caractère au texte.
			 *  @param font Police actuelle.
			 * @warning Il est préférable d'appeler cette méthode avant d'avoir initialisé le texte.
			 */
			void SetFont(const wxFont& font);

			/**
			 * Initialisation du texte. Vous devez appeler les méthodes de formatage du texte si vous ne voulez pas une police par défaut.
			 * @param text Libellé du texte
			 * @param maxLength Taille maximale du texte en nombre de caractères. Par défaut cette taille est celle du texte en paramètres.
			 */
			void InitText(const wxString& text,int maxLength=-1);

			/**
			 * Ajoute du texte dans la légende
			 */
			void AppendText(const wxString& text,int x,int y);


			/**
			 * Retourne la taille en pixel du texte en fonction des paramètres de l'objet
			 */
			wxSize GetTextSize(const wxString& text);
		private:
			wxColour textForeground;
			wxColour textBackground;
			wxFont textFont;
			wxString textValue;
			void DoAppendText(const wxString& text,int x,int y, bool overwrite=true,int maxLength=-1);
	};
}

#endif