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
#include "data_manager/appconfig.h"
#include <wx/log.h>
#ifndef __ELEMENT_FILENAME_PARTVISU__
#define __ELEMENT_FILENAME_PARTVISU__



/** \file e_report_partvisualisation.h
 *  \brief Classe représentant un ensemble d'enregistrements de positions de particules
 */

/**
 *	\brief Classe représentant un ensemble d'enregistrements de positions de particules
 */
class E_Report_ParticulesVisualisation: public E_Report_File
{
private:
	unsigned int userChoiceFreq;
public:
	/**
	 *	Constructeur de création
	 * @param parent Element parent
	 * @param Nom			Nom de l'élément (libellé dans l'arbre du projet). @ref traductiontuto "Ne pas traduire le libellé".
	 * @param Path Chemin d'accès au fichier depuis l'executable de l'interface
	 */
	E_Report_ParticulesVisualisation(Element* parent,wxString Nom,wxString Path)
		:E_Report_File(parent,"Visualisation particules",Path,ELEMENT_TYPE_REPORT_PARTVISUALISATION)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_PARTICLE);

	}

	/**
	 *	Constructeur XML
	 * @param parent Element parent
	 * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
	 */
	E_Report_ParticulesVisualisation(Element* parent,wxXmlNode* nodeElement)
		:E_Report_File(parent,"Visualisation particules","",ELEMENT_TYPE_REPORT_PARTVISUALISATION,nodeElement)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_PARTICLE);
		_("Particles display");
	}




	void OnRightClic( wxMenu* leMenu )
	{
		
		wxMenu* subMenuLoadSimu = new wxMenu;
		subMenuLoadSimu->Append(GetMenuItem(subMenuLoadSimu,IDEVENT_LOAD_PARTICLE_SIMULATION,_("Particles")));
		subMenuLoadSimu->Append(GetMenuItem(subMenuLoadSimu,IDEVENT_LOAD_PARTICLE_SIMULATION_PATH,_("Rays")));
		leMenu->Append(GetMenuItem(leMenu,-1,_("Load animation"),subMenuLoadSimu));
		E_Report_File::OnRightClic(leMenu);
	}

	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Report_File::SaveXMLDoc(NoeudParent);
		thisNode->SetName("partfile"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
};




#endif
