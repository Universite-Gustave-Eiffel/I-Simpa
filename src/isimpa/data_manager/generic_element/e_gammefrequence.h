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

#include "data_manager/element.h"

#ifndef __E_GAMMEFREQ__
#define __E_GAMMEFREQ__

/** \file e_gammefrequence.h
   \brief Classe correspondant au spectre d'une source

   Il calcul automatiquement la valeur du Niveau sonore global à chaque modification d'un de ses membres.
*/

/**
   \brief Classe correspondant au spectre d'une source

    Il calcul automatiquement la valeur du Niveau sonore global à chaque modification d'un de ses membres.
	@see E_Data_Row_BandeFreq
	@see E_Scene_Bdd_Spectrums_Application
	@see E_Scene_Bdd_Spectrums_User
*/
class E_GammeFrequence: public Element
{
protected:
	bool ignoreNextUpdate;
	int idSpectre; /*!< Indice du spectre sélectionné*/
	/**
	 * Cette méthode met à jour le champ "Global" à partir des valeurs par bande
	 * @param[in] typeDb "dB" ou "dBA" indique quel colonne mettre à jour
	 */
	void CalcNiveauSonoreGlobal(const wxString& typeDb);

	void Modified(Element* eModif);
	/**
	 * Ajoute les éléments de propriétées du spectre
	 */
	void InitProperties();

public:
	E_GammeFrequence(Element* parent,wxString Nom,ELEMENT_TYPE _type,wxXmlNode* nodeElement=NULL);
	virtual ~E_GammeFrequence();
	/**
	 * Renseigne l'identifiant du spectre
	 * @return Identifiant du spectre
	 * @see ApplicationConfiguration::AddSpectrum()
	 */
	int GetIdSpectre();
	/**
	 * Initialisation des propriétés avant affiche dans la grille de propriétés
	 **/
	virtual void InitProp();

	/**
	 * Retourne le type de spectre (synonyme de GetElementInfos().typeElement)
	 * @return Le type de l'élément
	 **/
	Element::ELEMENT_TYPE GetTypeSpectre();
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);
	virtual wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent);

	/**
	 * Initialisation des valeurs par bande à partir d'un niveau global
	 * @param[in] typeDb "dB" ou "dBA" indique quel colonne mettre à jour
	 * @param[in] lvl Niveau sonore en dB.
	 */
	void SetGlobalLevel( const wxString& typeDb, wxFloat32 lvl );
	/**
	 * Initialisation des valeurs par bande à partir d'un niveau global
	 * @param[in] typeDb "dB" ou "dBA" indique quel colonne mettre à jour
	 * @return Niveau sonore global en dB ou dbA selon typeDb.
	 */
	wxFloat32 GetGlobalLevel( const wxString& typeDb);

};

#endif