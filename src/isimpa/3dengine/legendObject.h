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

/**
 * @file legendObject.h
 * @brief Cette classe représente un élément de légende
 */
#ifndef __LEGEND_OBJECT__
#define __LEGEND_OBJECT__

namespace legendRendering
{


	typedef unsigned char FGL_BYTE;

	enum T_ALIGNEMENT_V
	{
		ALIGNEMENT_V_TOP,
		ALIGNEMENT_V_CENTER,
		ALIGNEMENT_V_BOTTOM
	};
	enum T_ALIGNEMENT_H
	{
		ALIGNEMENT_H_LEFT,
		ALIGNEMENT_H_CENTER,
		ALIGNEMENT_H_RIGHT
	};
	struct legendCfg
	{
		int width;
		int height;
		float u_text;
		float v_text;
		int idTex;
		T_ALIGNEMENT_V alignementV;
		T_ALIGNEMENT_H alignementH;
		int border_top;
		int border_bottom;
		int border_left;
		int border_right;
		bool redim;
		float redim_width_perc;
		float redim_height_perc;
		legendCfg();
	};
	/**
	 * Stocke les données correspondant à un élément de la légende 
	 */
	class legendObject
	{
	public:
		legendObject();
		legendObject(legendObject& cpyFrom);

		virtual ~legendObject();

		/**
		 * Initialisation des données
		 */
		void Init(int _width,int _height);
		void Clear();
		/**
		 * Retourne le premier élément des données de texture de taille width*height*4 (RGBA)
		 * @warning Retourne pointeur NULL(0) si la méthode Init n'a pas été appelé pour cet objet
		 * @return Pointeur vers le premier élément du tableau de pixel, NULL si non initialisé.
		 */
		FGL_BYTE* GetRawData();
		/**
		 * Retourne l'élément de configuration de la légende.
		 * @return Configuration en lecture seule.
		 */
		const legendCfg& GetCfg();
		/**
		 * Remplace l'élément de configuration par celui placé en paramètre.
		 * @param cpyFrom Nouveaux paramètres.
		 */
		void SetCfg(legendCfg& cpyFrom);
		/**
		 * Copie à partir d'une source de données. La méthode Init doit avoir été appelé au préalable sinon cette méthode ne fera rien.
		 * @param[in] RGB_src tableau RVB de taille width*height*3
		 * @param[in] A_src tableau valeur alpha de taille width*height. NULL si pas de couche Alpha
		 */
		void Copy(const FGL_BYTE* RGB_src,const FGL_BYTE* A_src,int _sourcewidth,int _sourceheight); 
		/**
		 * Copie à partir d'une source de données. La méthode Init doit avoir été appelé au préalable sinon cette méthode ne fera rien.
		 * @param[in] RGBA_src tableau RVB de taille width*height*4
		 */
		void Copy(const FGL_BYTE* RGBA_src,int _sourcewidth,int _sourceheight); 

		/**
		 * Redimensionne la texture en conservant les données précédentes
		 */
		void PreserveResize(int _newwidth,int _newheight);
	protected:
		void DoCopy(const FGL_BYTE* RGB_src,const FGL_BYTE* A_src, int srcWidth,int srcHeight, bool overwrite=true,int destCornerX=0,int destCornerY=0);
		legendCfg objConfig;
		FGL_BYTE* foreground_tex;

	};
}

#endif