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


#include <wx/hashmap.h>


#ifndef __INSTANCE_MANAGER__
#define __INSTANCE_MANAGER__


#define VALHASHEL void*

WX_DECLARE_HASH_MAP( int, VALHASHEL , wxIntegerHash, wxIntegerEqual, t_HashPspsObject );

enum OBJECT_REF_TYPE
{
	OBJECT_REF_TYPE_ELEMENT,	/*!< Tout les élément du projet référencé par t_elementInfo::xmlIdElement */
	OBJECT_REF_TYPE_PROJECT,	/*!< Projet chargé dans le logiciel*/
	OBJECT_REF_TYPE_CAMERA,
	OBJECT_REF_TYPE_SIZE		/*!< Nombre de type d'objet référencé */
};
/**
 * Lors de l'instanciation d'un objet dans le code python cette classe est utilisé afin d'obtenir 
 * la référence de l'objet de PSPS précédement déclaré.
 *
 * Ainsi les modules python ont une source de données communes pour obtenir les références de la variable this désirée
 */

class InstanceManager
{
protected:
	struct t_HashObjectArray
	{
		t_HashPspsObject ptr_hash[OBJECT_REF_TYPE_SIZE];
	};
	static t_HashObjectArray HashObjectArray;
public:

	static void AppendReference(const OBJECT_REF_TYPE& refType, const int& index, VALHASHEL ref);
	static void RemoveReference(const OBJECT_REF_TYPE& refType, const int& index);
	/**
	 * Retourne la référence de l'objet souhaité
	 * @param refType Type de l'élément à extraire
	 * @param index Indice de l'élément
	 * @param[out] Pointeur où placer l'adresse de l'élément
	 * @return Vrai si un élément correspond à cet indice
	 */
	static bool GetReference(const OBJECT_REF_TYPE& refType, const int& index,VALHASHEL* pt_ref);

	static size_t GetElementCount();

};


#endif
