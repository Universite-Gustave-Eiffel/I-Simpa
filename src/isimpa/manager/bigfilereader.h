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

#include <wx/file.h>
#ifndef __BIG_FILE_READER__
#define __BIG_FILE_READER__

/*! \file bigfilereader.h
    \brief Implémentation de la classe BigFileReader
*/

/**
 *\class BigFileReader 
 *	\brief Classe de lecture de fichiers volumineux
 *	
 *	Cette classe permet de specifier la taille du cache utilisé pour lire le fichier
 *	L'evolution sera d'implementer des algorithme d'optimisation dédié aux fichier ASCII
 *	@see stringParser()
 *	
 */
class BigFileReader
{
private:
	wxFile oFile;		/**< Contient l'instance de la classe de lecture de fichier de wxWidgets */ 
	long bufferPos;		/**< Entier indiquant la position du curseur de lecture dans le buffer mémoire */ 
	char* fBuffer;		/**< Buffer mémoire de taille variable (défini dans le constructeur) */ 
	long cacheSize;		/**< Taille du buffer mémoire fBuffer */ 
	/**
	 * Lit le fichier et place les données dans le buffer mémoire
	 * @return Vrai si il reste des données à lire dans le fichier
	 */
	bool FillBuffer();
public :
	/**
	 * Constructeur de la classe BigFileReader
	 * @param file Chemin et nom du fichier à lire
	 * @param cacheSize Taille en octets à placer en mémoire
	 * @see ~BigFileReader()
	 */
	BigFileReader(wxString file,unsigned long cacheSize);
	/**
	 * Destructeur de la classe BigFileReader
	 * @see BigFileReader()
	 */
	~BigFileReader();
	/**
	 * Rapport octets lu sur la taille du fichier
	 * @return Pourcentage lu du fichier de 0 à 1
	 */
	float getPercentRead();
	/**
	 * Permet de connaître l'etat du fichier
	 * @return Vrai si le fichier est ouvert et pret a etre lu, faux si fin de fichier ou fichier impossible a ouvrir
	 */
	bool IsOk();
	/**
	 * Lit le prochain octets du fichier
	 * @param outputChar Adresse où écrire la valeur du prochain octet
	 * @return Vrai si l'octet a été lu correctement
	 */
	bool GetNext(char* outputChar);
	/**
	 * Lit le prochain octets du fichier
	 * @param fl Adresse où écrire la valeur de la prochaine valeur décimale dans le fichier
	 * @return Vrai si l'octet a été lu correctement
	 */
	bool GetDecimal(double *fl);
	/**
	 * Lit les octets jusqu'au prochain retour chariot
	 * @return La chaîne de caractère correspondant à la ligne
	 */
	wxString GetLine();
};

#endif