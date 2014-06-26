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

#include "e_report_file.h"


#ifndef __ELEMENT_GABE__
#define __ELEMENT_GABE__



/** \file e_report_gabe.h
 *  \brief Classe représentant un fichier au format gabe. Fichier binaire contenant un tableau.
 */

/**
 *  \brief Classe représentant un fichier au format gabe. Fichier binaire contenant un tableau.
 */
class E_Report_Gabe: public E_Report_File
{
public:
	/**
	 *	Constructeur de création
	 * @param parent Element parent
	 * @param Nom			Nom de l'élément (libellé dans l'arbre du projet). @ref traductiontuto "Ne pas traduire le libellé".
	 * @param Path Chemin d'accès au fichier depuis l'executable de l'interface
	 */
	E_Report_Gabe(Element* parent,wxString Nom,wxString Path,ELEMENT_TYPE etype=ELEMENT_TYPE_REPORT_GABE);

	/**
	 *	Constructeur XML
	 * @param parent Element parent
	 * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
	 */
	E_Report_Gabe(Element* parent,wxXmlNode* nodeElement,ELEMENT_TYPE etype=ELEMENT_TYPE_REPORT_GABE);

	/**
	 *	Initialise les données en paramètres avec des valeurs pouvant être soumis à un tableau
	 * @param[out] arrayTitle Titre du tableau
	 * @param[out] lblCols Libellé des colonnes
	 * @param[out] lblRows Libellé des lignes
	 * @param[out] cells Valeur texte des cellules
	 * @param[out] cellsValue Valeur en nombre décimal des cellules (0 si aucun)
	 */
	virtual bool GetArrayData(wxWindow* auiBookWin,wxString& arrayTitle,std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<wxString>& cells,std::vector<float>& cellsValue);

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);

	/**
	 *  Indique si la gestion des données doit être confié à cet élément (faux) ou si le fichier est ouvert par le contrôle de la grille directement.
	 * @see GetArrayData()
	 */
	virtual bool OpenFileInGrid();

	
	virtual bool IsFittingWithThisType( const ELEMENT_TYPE& typeTest );
};




#endif