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

#include "gabe.h"
#include <vector>
#include <string>

#ifndef __STD_GABE__
#define __STD_GABE__



/**
 * @brief Fichier d'échange générique
 */
namespace gabe_io
{
	using namespace std;
	typedef formatGABE::GABE_OBJECTTYPE COLTYPE;

	typedef vector<string> stringarray;
	typedef vector<int> intarray;
	typedef vector<float> floatarray;
	typedef vector<COLTYPE> typearray;


	/**
	 * Gestion d'écriture et de lecture du format GABE lu par l'interface
	 * La première colonne correspond aux libellées des lignes.
	 * Chaque colonne peut être de 3 types différends, chaine de caractère, nombre à décimale ou nombre entier.
	 *
	 */
	class Gabe_rw : protected formatGABE::GABE
	{
	public:
		/**
		 * @param sizeHint A la génération d'un tableau ce paramètre permet une plus grand rapiditée d'ajout des colonnes
		 */
		Gabe_rw(const size_t& sizeHint);

		Gabe_rw();
		/**
		 * Chargement du tableau
		 * @param strFileName Chemin du fichier
		 */
		bool Load(const string& fileName);
		/**
		 * Sauvegarde du tableau
		 * @param strFileName Chemin du fichier
		 */
		bool Save(const string& fileName);

		/**
		 * Ajoute une colonne de chaine de caractères à la fin du tableau
		 */
		void AppendStrCol( const stringarray& coldata, const string& coltitle );
		/**
		 * Ajoute une colonne d'entier à la fin du tableau
		 */
		void AppendIntCol( const intarray& coldata, const string& coltitle );
		/**
		 * Ajoute une colonne de nombres flottants à la fin du tableau
		 */
		void AppendFloatCol( const floatarray& coldata, const string& coltitle, const int& precision=12 );
		/**
		 * Lecture du titre d'une colonne
		 */
		string GetColTitle(const size_t& idcol);
		/**
		 * Retourne les types de données sur les colonnes
		 */
		typearray GetTabTypes();

		/*
		 * Lecture d'une colonne sous forme de chaine de caractère, n'echoue jamais
		 * @python The coldata paramemeter is returned by this function.
		 */
		void ReadColStr(const size_t& idcol,stringarray& coldata);
		/*
		 * Lecture d'une colonne sous forme de chaine de caractère
		 * @warning Si cet indice ne correspond pas à une colonne de ce type alors cette méthode n'affectera pas le paramètre en entrée et ne lévera pas d'erreur.
		 * @python The coldata paramemeter is returned by this function.
		 */
		void ReadColInt(const size_t& idcol,intarray& coldata);
		/*
		 * Lecture d'une colonne sous forme de chaine de caractère
		 * @warning Si cet indice ne correspond pas à une colonne de ce type alors cette méthode n'affectera pas le paramètre en entrée et ne lévera pas d'erreur.
		 * @python The coldata paramemeter is returned by this function.
		 */
		void ReadColFloat(const size_t& idcol,floatarray& coldata);


		/**
		 * Les données sont elles en lecture seule pour l'utilisateur.
		 */
		bool IsReadOnly();

		/**
		 * Change le mode d'édition par l'utilisateur dans l'interface
		 */
		void SetReadOnly(const bool& ReadOnly=true);

		/**
		 * @return Nombre de colonnes
		 * @python Use the len function to give the number of cols.
		 */
		size_t size() const;
		/**
		 * Lire une colonne en tant que chaine de caractères (compatible avec n'importe quel type de colonne)
		 */
		stringarray operator[](size_t indicecol);
	private:
		size_t filledcols;
		/*
		 * @return Indice de la première colonne non affecté, -1 si aucun
		 */
		int GetFirstEmptyCol();
		/**
		 * Ajoute une colonne à la fin du tableau
		 */
		void AppendCol(formatGABE::GABE_Object* objtoappend);
	};

	static void impl_vectors()
	{

		stringarray tabchaine;
		intarray tabentier;
		floatarray tabflottant;
	}
};

#endif
