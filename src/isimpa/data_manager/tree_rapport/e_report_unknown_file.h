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

#include "e_report_file.h"

#ifndef __ELEMENT_FILE_UNKNOWN__
#define __ELEMENT_FILE_UNKNOWN__



/** \file e_report_unknown_file.h
 *  \brief Classe représentant un fichier non reconnu par PSPS mais reconnu par le système d'exploitation
 */

/**
 *	\brief Classe représentant un fichier non reconnu par PSPS mais reconnu par le système d'exploitation
 */
class E_Report_Unknown: public E_Report_File
{
public:
	/**
	 * Structure décrivant le type
	 */

	/**
	 *	Constructeur de création
	 * @param parent Element parent
	 * @param Nom			Nom de l'élément (libellé dans l'arbre du projet). @ref traductiontuto "Ne pas traduire le libellé".
	 * @param Path Chemin d'accès au fichier depuis l'executable de l'interface
	 */
	E_Report_Unknown(Element* parent,wxString Nom,wxString Path);

	/**
	 *	Constructeur XML
	 * @param parent Element parent
	 * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
	 */
	E_Report_Unknown(Element* parent,wxXmlNode* nodeElement);

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);
	/**
	 * Test l'extension du fichier passé en paramètre afin de déterminer si le système d'exploitation courant connait un logiciel compatible avec cette extension
	 *
	 * @return Vrai si un élément E_Report_Unknown peut etre créé avec ce fichier.
	 */
	static bool ExtKnownByCurrentOs(const wxString& fileName);

	virtual void FillWxTree(wxTreeCtrl* treeToFeed=NULL,const wxTreeItemId& parentId=wxTreeItemId());

	void OpenFile();
	
};




#endif