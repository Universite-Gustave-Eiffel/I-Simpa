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

#include "data_manager/element.h"
#include <wx/filename.h>
#ifndef __ELEMENT_FILE__
#define __ELEMENT_FILE__

/** \file e_report_file.h
 *  \brief Classe virtuelle de base qui représente un fichier ou un dossier dans le dossier de résultat de calcul
 */

/**
 *	\brief Classe virtuelle de base qui représente un fichier ou un dossier dans le dossier de résultat de calcul
 */
class E_Report_File: public Element
{
private:
	void SaveProperties(wxXmlNode* thisNode);
protected:
	bool blockUpdateNotification;	
	wxString filePath;
public:
	/**
	 * Structure décrivant le type
	 */
	/**
	 *	Constructeur
	 * @param parent Element parent
	 * @param Nom			Nom de l'élément (libellé dans l'arbre du projet). @ref traductiontuto "Ne pas traduire le libellé".
	 * @param Path Chemin d'accès au fichier depuis l'executable de l'interface
	 * @param _type			Type de l'élément
	 * @param _graph		Indice de l'élément graphique courant de l'élément
	 * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
	 */
	E_Report_File(Element* parent,wxString Nom,wxString Path,ELEMENT_TYPE _type,wxXmlNode* nodeElement=NULL);


	/**
	 * Cette méthode va lire le dossier lié à cet élément et actualiser le ou les fichiers de façon récursive.
	 */
	virtual void RefreshFolderContents();

	/**
	 * Supprime le dossier associé à cet élément
	 */
	virtual void DeleteFolder();

	/**
	 * Cette méthode renseigne le tableau en paramètre avec le ou les noms de fichiers associés à cet élément.
	 * Cette méthode n'est pas récursive et renseigne donc que sur le dossier courant de l'élément.
	 */
	void PushElementPath(wxString& prefix,wxArrayString& tabPath);

	/**
	 * Vérifie si le fichier associé a cet élément existe
	 * @param fullPath Chemin du dossier père
	 * @see BuildFullPath()
	 */
	bool IsExist(wxString& fullPath);
	/**
	 * Alimente le tableau en paramètre avec le chemin des fichiers contenues dans le dossier renseigné
	 * @param[in] rootFolder Dossier à lire
	 * @param[out] folderDir Tableau a renseigner
	 */
	static void GetAllFolderItems(wxString rootFolder,wxArrayString& folderDir);

	void OnRightClic( wxMenu* leMenu );

	/**
	 * Méthode ascendante ( du fils vers le dernier père e_report_file ). Construit le chemin d'accès à cet élément.
	 * @param[in,out] fullPath Chemin d'accès
	 */
	void BuildFullPath(wxString& fullPath);

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);
	wxXmlNode* SaveXMLDocWithoutChildren(wxXmlNode* NoeudParent);

	virtual bool OnElementRemoved();
	void OnBeginLabelEdit(wxTreeEvent& treeEvent); 
	virtual void OnEndLabelEdit(wxTreeEvent& evt);
	void Modified(Element* eModif);
	virtual bool IsFittingWithThisType( const ELEMENT_TYPE& typeTest )
	{
		if(typeTest==ELEMENT_TYPE_REPORT_FILE)
			return true;
		else
			return Element::IsFittingWithThisType(typeTest);
	}
};




#endif