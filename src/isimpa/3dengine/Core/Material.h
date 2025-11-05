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

#ifndef _HMaterial
#define _HMaterial

/*! \file Material.h
    \brief Gestion des matériaux OpenGl
    
    Les informations de rendu des faces du modèle(scène) dans la vue 3D au niveau couleur et reflexion lumineuse sont stockées ici
*/

#include <string.h>
#include "Mathlib.h"
#include "Texture.h"
 
/*!
    \brief Structure de données d'un matériau
*/

struct SMaterial
{
	std::string Name;		/*!< Nom de la texture (Nom du fichier)*/
	vec4 ColorA;	/*!< Couleur ambiante */
	vec4 ColorD;	/*!< Couleur diffuse */
	vec4 ColorS;	/*!< Couleur spéculaire */
	float Transparency;	/*!< Transparence [0;1] */
	float Shininess;	/*!< Brillance [0;1] */
	long idTex;			/*!< Indice de la texture */
};

/**
 *\class CMaterial 
 *	\brief Classe de gestion des matériaux OpenGl (de la scène)
 *
 *	Cette classe stocke et execute les commandes OpenGl de rendu de matériaux OpenGL
 *	@see CObjet3D::_RenderGroupTexture()
 *	
 */

class CMaterial 
{
public:

	/**
	 * Constructeur
	 */
	CMaterial() 
	{
	}

	/**
	 * Destructeur
	 */
	~CMaterial() 
	{	
		//for(long i=0;i<this->_pMaterial.size();i++)
	    //delete this->_pMaterial[i].Name;
		this->_pMaterial.empty();
	}
	/**
	 * Cette méthode permet d'acceder à la liste des matériaux utilisés. Utile pour sauvegarder celles-ci.
	 * @see CformatBIN::ExportBIN
	 * @return Un tableau de matériaux
	 */
	const std::vector<SMaterial>& GetMaterials(){ return _pMaterial; }
	/**
	 * Créer un matériau à partir d'informations de base (stockage dans la classe de gestion de matériau)
	 * @param name		Libellé du matériau
	 * @param colorA	Valeur RGB de la couche Ambiante
	 * @param colorD	Valeur RGB de la couche Diffuse	
	 * @param colorS	Valeur RGB de la couche Spéculaire
	 * @return L'identifiant du matériau créé
	 */
	unsigned long Create(char *name,const vec4 &colorA, vec4 &colorD, vec4 &colorS);	
	/**
	 * Créer un matériau à partir de l'objet matériau (stockage dans la classe de gestion de matériau)
	 * @param strm	Matériau
	 * @return L'identifiant du matériau inseré
	 */
	unsigned long Create(const SMaterial *strm);
	/**
	 * A partir du libellé du matériau retourne son identifiant
	 * @param name	Libellé du matériau
	 * @return L'identifiant du matériau (0-Nb matériau), si introuvable 0 (matériau par défaut)
	 */
	unsigned long GetId(const std::string& name);
	/**
	 * Cette méthode execute les commandes OpenGl correspondant au matériau
	 * @param m	Identifiant du matériau
	 * @param forcetransparency	Force la transparence du matériau, -1 par défaut
	 * @return L'identifiant du matériau si existant, 0 si inexistant
	 */
	unsigned long SetColor(unsigned long m,float forcetransparency=-1);
	/**
	 * Cette méthode execute les commandes OpenGl correspondant au matériau avec la gestion des textures
	 * @param m	Identifiant du matériau
	 * @return L'identifiant du matériau si existant, 0 si inexistant
	 */
	unsigned long SetMat(unsigned long m);
	/**
	 * Vrai si ce matériau a une texture
	 */
	bool HasTexture(unsigned long m);
	/**
	 * Retourne idTex
	 */
	long GetTextureId(unsigned long m);
private:
	std::vector<SMaterial> _pMaterial;
};

#endif  //_HMaterial
