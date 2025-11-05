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

#include <string>


#ifndef __CORE_STRING__
#define __CORE_STRING__
/**
 * @file coreString.h
 * Surcharge de la classe std::string
 */

#define entier int						/*!< Type de données pour les entier signé */
#define uentier_court unsigned short	/*!< Type de données pour les entier non signé court ( 65000 maximum )*/
#define entier_court short				/*!< Type de données pour les entier court ( 32000 maximum )*/
#define uentier unsigned int			/*!< Type de données pour les entier non signé */
#define decimal float					/*!< Type de données pour les chiffres à décimal */
#define l_decimal double				/*!< Type de données pour les chiffres à décimal long, a utiliser pour les cumuls d'énergie */
#define t_freq unsigned int				/*!< Type de données pour les fréquences */
#define uentier_long unsigned long		/*!< Type de données entier long */

/**
 * @brief Surcharge de la classe std::string
 *
 * Afin de simplifier l'écriture de certaines opérations, tel que la concaténation cette surcharge a été effectué.
 */

class CoreString : public std::string
{
public:
	/**
	 * Constructeur à partir d'une chaine de caractères complète
	 * @param ch[in] Chaine de caractères
	 */
	CoreString(const char* ch);

	CoreString(std::string outputstr);
	/**
	 * Constructeur à partir d'une fraction d'une chaine de caractères
	 * @param ch[in] Chaine de caractères
	 * @param length[in] Longueur de la chaine de caractères
	 */
	CoreString(const char* ch, unsigned int length);

	/**
	 * Constructeur d'une chaine vide
	 */

	CoreString( );

	static std::string FromInt(const entier& valInt);

	static std::string FromFloat(decimal nombreDecimal,entier digits=3);

	static entier ToInt(const CoreString& strtoconv);
	static decimal ToFloat(const CoreString& strtoconv);

	entier ToInt() const;

	decimal ToFloat() const;
};

#endif
