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

#include "stringTools.h"
#ifndef __STRING_PARSER_H_
#define __STRING_PARSER_H_


#define SP_CHAR_a 97
#define SP_CHAR_A 65
#define SP_CHAR_z 122
#define SP_CHAR_Z 90
#define SP_CHAR_0 48
#define SP_CHAR_9 57
#define SP_CHAR_DOT 46
#define SP_CHAR_COMMA 44
#define SP_CHAR_MINUS 45

/*! \file stringParser.h
    \brief Classe permettant le découpage d'une chaîne
*/

/**
 * @brief Classe permettant le découpage d'une chaîne
 *
 */
class stringParser
{
public:
	/**
	 * Structure désignant le type d'un élément de la chaîne
	 */
	enum t_ele
	{
		ELE_STRING,		/*!< chaîne alpha numerique */
		ELE_INTEGER,	/*!< entier */
		ELE_FLOAT,		/*!< Chiffre a decimal */
		ELE_SEPARATOR	/*!< caractere separateur ( autre que alphanumerique sauf " ") */
	};
private:
	wxString localStr;

public:
	/**
	 * Constructeur
	 */
	stringParser()
	{
		localStr="";
	}

	/**
	 * Charge la chaîne à découper
	 * @param strToParse chaîne à découper
	 */
	void Parse( wxString &strToParse)
	{
		if(strToParse.Len()==0)
			return;
		localStr=strToParse;
	}

	/**
	 *
	 * @return Vrai si aucune chaîne est chargé ou si la chaîne chargé est vide ( tout a été découpé)
	 **/
	bool IsEmpty(  )
	{
		return localStr.empty();
	}

	/**
	 * Identifie le caractère
	 * @param theChar Paramètre à identifier
	 * @return Vrai si le caractère est de type séparateur
	 **/
	bool IsSeparatorChar(const wxChar& theChar)
	{
		if((theChar>=SP_CHAR_0 && theChar<=SP_CHAR_9) || (theChar>=SP_CHAR_A && theChar<=SP_CHAR_Z) || (theChar>=SP_CHAR_a && theChar<=SP_CHAR_z))
			return false;
		else
			return true;
	}
	/**
	 * Identifie le caractère
	 * @param theChar Paramètre à identifier
	 * @return Vrai si le caractère est de type séparateur
	 **/
	bool IsSeparatorChar(const wxUniCharRef& theChar)
	{
		if((theChar>=SP_CHAR_0 && theChar<=SP_CHAR_9) || (theChar>=SP_CHAR_A && theChar<=SP_CHAR_Z) || (theChar>=SP_CHAR_a && theChar<=SP_CHAR_z))
			return false;
		else
			return true;
	}
	/**
	 * Parmi la chaîne chargé, retourne le prochain mot
	 * @return Prochain mot de gauche à droite
	 */
	wxString GetNextStringWord()
	{
		wxString retBuffer;
		if(localStr.IsEmpty())
			return retBuffer;
		localStr.Trim(false);
		//On boucle jusqu'a rencontrer autre chose
		while(!localStr.IsEmpty() && !IsSeparatorChar(localStr[0]))
		{
			retBuffer<<localStr[0];
			localStr.Remove(0,1);
		}
		return retBuffer;
	}
	/**
	 * Retourne le prochain séparateur
	 * @return Chaîne contenant le prochain séparateur
	 */
	wxString GetNextSeparator()
	{
		wxString retBuffer;
		if(localStr.IsEmpty())
			return retBuffer;
		localStr.Trim(false);
		//On boucle jusqu'a rencontrer autre chose
		while(!localStr.IsEmpty() && IsSeparatorChar(localStr[0]))
		{
			retBuffer<<localStr[0];
			localStr.Remove(0,1);
		}
		return retBuffer;
	}

	/**
	 * Retourne la chaîne restante
	 * @return Reste de la chaîne
	 */
	wxString GetNextStringLine()
	{
		return localStr;
	}


	/**
	 * Retourne le prochain entier
	 * @return Prochain entier dans la chaîne chargée
	 */
	int GetNextInteger()
	{
		wxString retBuffer;
		if(localStr.IsEmpty())
			return 0;
		localStr.Trim(false);
		while(!localStr.IsEmpty() && ((localStr[0]>=SP_CHAR_0 && localStr[0]<=SP_CHAR_9) || localStr[0]==SP_CHAR_MINUS))
		{
			retBuffer<<localStr[0];
			localStr.Remove(0,1);
		}
		return Convertor::ToLong(retBuffer);
	}

	/**
	 * Retourne le prochaine valeur à décimale
	 * @return Prochaine valeur à décimale
	 */
	float GetNextFloat()
	{
		wxString retBuffer;
		if(localStr.IsEmpty())
			return 0;
		localStr.Trim(false);
		while(!localStr.IsEmpty() && ((localStr[0]>=SP_CHAR_0 && localStr[0]<=SP_CHAR_9) || localStr[0]==SP_CHAR_DOT || localStr[0]==SP_CHAR_MINUS || localStr[0]==SP_CHAR_COMMA))
		{
			retBuffer<<localStr[0];
			localStr.Remove(0,1);
		}
		return Convertor::ToFloat(retBuffer);
	}


	/**
	 * Retourne le prochain caractère, sans modifier la chaîne chargée
	 * @return Prochain caractère
	 */
	wxChar GetNextChar()
	{
		return localStr[0];
	}

};


#endif
