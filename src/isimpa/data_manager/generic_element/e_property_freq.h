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
#include "data_manager/appconfig.h"

#ifndef __PROPERTYFREQ__
#define __PROPERTYFREQ__

/** \file e_property_freq.h
   \brief Element représentant la propriété de spectre d'une source sonore (liste déroulante de la source suivi du spectre)
*/

/**
   \brief Element représentant la propriété de spectre d'une source sonore (liste déroulante de la source suivi du spectre)
   @see E_Scene_Sources_Source
*/
class E_Property_Freq: public Element
{
protected:
	int idSpectre;
	ELEMENT_TYPE typeSpectre;
	std::vector<ApplicationConfiguration::t_lstSpec> lastLst;
	bool ignoreUpdate;
	bool askReload;
	static bool orderSpectrums(Element* lEl,Element* rEl);
	void CreateGlobalVal(float valDefault);
	/**
	 * Met à jour la colonne Lw depuis la base de données
	 */
	void LoadLwFromBdd();
	void CreateExtendedBFreq();
	/**
	 * Calcul du niveau global à partir du niveau par bande
	 */
	void CalcNiveauSonoreGlobal(const wxString& typeDb);
	void SetGlobalLevel( const wxString& typeDb, wxFloat32 lvl );
public:
	E_Property_Freq( wxXmlNode* noeudCourant ,  Element* parent);

	/**
	 * Cette méthode a été surchargée afin d'ajouter aux propriétés le spectre correspondant au spectre sélectionné dans la liste déroulante
	 */
	E_Property_Freq( Element* parent, wxString label="Spectre");
	void InitProp(); //Appelé par la grille de propriété avant l'affichage
	void Modified(Element* eModif);
	bool IsAskPropReload();
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent);
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);
	/**
	 * Retourne la valeur courante global
	 */
	float GetGlobaldBVal(const wxString& dbCol);
};
#endif