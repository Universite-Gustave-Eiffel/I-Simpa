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
/** \file e_scene_projet_configuration.h
   \brief Element représentant la configuration cachée du projet ( non renseigné par l'utilisateur )
*/

/**
   \brief Element représentant la configuration cachée du projet ( non renseigné par l'utilisateur )
*/
class E_Scene_Configuration: public Element
{
public:
	E_Scene_Configuration( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Configuration",Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		wxGetTranslation("Configuration");
		if(this->GetStringConfig("urlmodel")==wxString("current")+wxFileName::GetPathSeparator()+"sceneMesh.bin") //maj version 1.0.3
			this->UpdateStringConfig("urlmodel","sceneMesh.bin");
	}

	E_Scene_Configuration( Element* parent)
		:Element(parent,"Configuration",Element::ELEMENT_TYPE_SCENE_PROJET_CONFIGURATION)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_EL_CONFIGURATION);
		InitProperties();
	}
	/*
	 * Ajoute les éléments de configuration tel que l'emplacement du fichier de modèle et le chemin de sauvegarde par défaut du projet
	 */
	void InitProperties()
	{
		this->AppendPropertyText("urlmodel","Fichier de modèle","",true);
		wxGetTranslation("Scene file"); //Pour generer les fichiers de langues
		this->AppendPropertyText("urlsave","Fichier de sauvegarde","",true);
		wxGetTranslation("Backup file");
	}

	void FillWxTree(wxTreeCtrl* treeToFeed,const wxTreeItemId& parentId)
	{
		//Ne pas afficher cet élément dans l'arbre
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("Configuration"); // Nom de la balise xml ( pas d'espace autorise )

		return thisNode;
	}
};
