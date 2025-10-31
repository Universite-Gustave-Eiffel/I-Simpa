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
#include <string>
#include <algorithm>
#include <vector>
#include <string.h>
#include "Mathlib.h"


#ifndef _Texture_H
#define _Texture_H


/*! \file Texture.h
    \brief Gestion des textures de surface de la vue 3D

    Stocke, retourne, et execute les commandes OpenGl ayant attrait au rendu des textures. (images appliquées sur les surfaces)
*/
/**
 *	Enregistrement d'une texture, contient les informations de la texture.
 */
struct TextureInfo
{
	TextureInfo(): id(0),width(0),height(0),scale(1,1) {}
	unsigned int id, width, height;
	std::string name;
	std::string path;
	int bpp;
	vec2 scale;
	vec2 translation;
};

/*! \brief Gestion des textures de surface de la vue 3D

    Stocke, retourne, et execute les commandes OpenGl ayant attrait au rendu des textures. (images appliquées sur les surfaces)
*/

class CTexture
{
public:
	/**
		Constructeur
	*/
	CTexture();
	/**
		Destructeur
	*/
	~CTexture();
	/**
		Charge une texture dans la liste selon le nom du fichier d'image
		@param filename Chemin et nom du fichier image (filename devient le nom de la texture)
		@return Identifiant de la texture
	*/
	long Load(const std::string& filename);
	/**
		Insère la texture dans le moteur OpenGL
	*/
	bool SetTex(long id);
	/**
	 * Return the 
	 */
	void GetTransformation(const long& id,vec2& scale,vec2& translation);
	/**
		A partir du nom de la texture, retourne son identifiant
	*/
	long GetId(const std::string& name);
	/**
		Retourne la liste des textures
		@see CformatBIN::ExportBIN()
	*/
	const std::vector<TextureInfo>& GetTextures(){ return _pTexInf; }

	void ReloadAllTextures();
	void Destroy();
private:
	void FllipVerticale();
	std::vector<TextureInfo> _pTexInf;

};



#endif  // _Texture_H
