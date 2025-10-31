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
#include <vector>
/**
 * @file GlBitmapSurface.h
 * @brief Gestion d'affichage de bitmap au premier plan d'une vue en 3D
 */

#ifndef __LEGEND_RENDERER__
#define __LEGEND_RENDERER__


/**
 * @class ForeGroundGlBitmap
 * @brief Gestion d'affichage de bitmap au premier plan d'une vue en 3D
 */

namespace legendRendering
{

	class ForeGroundGlBitmap
	{
	public:

		ForeGroundGlBitmap();
		ForeGroundGlBitmap(ForeGroundGlBitmap& cpyFrom);
		~ForeGroundGlBitmap();

		/**
		 * Rendu OpenGl de la liste des objets chargée
		 * @param view_width Largeur de la fenêtre
		 * @param view_height Hauteur de la fenêtre
		 * @param renewTex Si vrai la méthode redéclare les textures associées aux légendes, ralentit le processus mais permet de dessiner sur un autre contexte de rendu sans avoir à recopier toutes les légendes.
		 */
		void Draw(int view_width,int view_height);

		/**
		 * Ajoute un élément de légende. Il sera supprimé par cet objet lors de sa destruction.
		 */
		void Push(legendObject* newLegend);
		void Delete(legendObject* oldLegend);

		/**
		 * Recupere la liste des objets de légendes.
		 */
		const std::vector<legendObject*>* GetLegendObjects();

		/**
		 * Efface les objets de légendes.
		 */
		void Clear();
	private:
		std::vector<legendObject*> legendsList;
	};

}

#endif