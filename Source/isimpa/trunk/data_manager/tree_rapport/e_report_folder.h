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

#ifndef __ELEMENT_FOLDER__
#define __ELEMENT_FOLDER__



/** \file e_report_folder.h
 *  \brief Classe représentant un dossier sans comportement spécifique
 */

/**
 *	\brief Classe représentant un dossier sans comportement spécifique
 */
class E_Report_Folder: public E_Report_File
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
	E_Report_Folder(Element* parent,wxString Nom,wxString Path)
		:E_Report_File(parent,Nom,Path,ELEMENT_TYPE_REPORT_FOLDER)
	{

		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_DISK_FOLDER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_DISK_FOLDER_CLOSE);

	}

	/**
	 *	Constructeur XML
	 * @param parent Element parent
	 * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
	 */
	E_Report_Folder(Element* parent,wxXmlNode* nodeElement)
		:E_Report_File(parent,"Unnamed Folder","",ELEMENT_TYPE_REPORT_FOLDER,nodeElement)
	{
		SetIcon(GRAPH_STATE_EXPANDED,GRAPH_DISK_FOLDER_OPEN);
		SetIcon(GRAPH_STATE_NORMAL,GRAPH_DISK_FOLDER_CLOSE);


	}
	
		
	void OnRightClic(wxMenu* leMenu)
	{
		leMenu->Append(GetMenuItem(leMenu,IDEVENT_OPEN_FOLDER, _("Ouvrir le dossier")));
		E_Report_File::OnRightClic(leMenu);
	}
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Report_File::SaveXMLDoc(NoeudParent);
		thisNode->SetName("folder"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
	void OpenFolder()
	{
		wxString fullpath;
		this->BuildFullPath(fullpath);
		wxString cmd;
		#ifdef __WXMSW__
		cmd=wxString::Format("explorer %s",fullpath);
		#elif __WXGTK__
		cmd=wxString::Format("nautilus %s",fullpath);
		#elif __WXMAC__ 
		cmd=wxString::Format("open %s",fullpath);
		#endif
		wxExecute(cmd);
	}
	virtual const wxString GetElementNameForOrder() const
	{
		//Les dossier d'abord, les elements ensuite
		return wxString("  ")+wxString(_(this->GetTreeLabel()));
	}
};




#endif