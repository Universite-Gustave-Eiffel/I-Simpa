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

#ifndef __E_MATERIAU__
#define __E_MATERIAU__

/** \file e_materiau.h
   \brief Classe correspondant au spectre d'un type de surface
*/

/**
   \brief Classe correspondant au spectre d'un type de surface

    Les calculs entre chaque ligne et la valeur global est spécifié ici
	@see E_Data_Row_BandeFreq
	@see E_Scene_Bdd_Materiaux_User_Materiau
	@see E_Scene_Bdd_Materiaux_App_Materiau
*/
class E_Materiau: public Element
{
protected:
	bool ignoreModification; /*!< A vrai, bloque la mis à jour du champ dB global et la propagation vers les éléments superieur*/
	int idMateriau; /*!< Identifiant du matériau (voir ApplicationConfiguration) */
	/** 
     * @brief Structure champ lecture seule
	 *
	 *    Lors de la construction des sous-élément, se tableau indique lequel de ses sous-éléments seront en lecture seule.
	 */
	struct t_idChamp
	{
		int row; /*!< Ligne */
		int col; /*!< Colonne */
	};
	/**
	 * Calcul du niveau Sonore Global
	 * @param[in] typeDb Nom de la colonne a calculer
	 */
	void CalcNiveauSonoreGlobal(wxString typeDb);
	void Modified(Element* eModif);
	void InitProperties();
public:
	E_Materiau(Element* parent,wxString Nom,ELEMENT_TYPE _type,wxXmlNode* nodeElement=NULL);
	virtual ~E_Materiau();
	/**
	 * Retourne l'identifiant du matériau
	 * @return Entier correspondant à l'identifiant
	 * @see ApplicationConfiguration
	 */
	int GetIdMateriau();
	/**
	 * Retourne l'identifiant du matériau
	 * @return Type du type du matériau (Utilisateur ou Application)
	 * @see ApplicationConfiguration
	 */
	Element::ELEMENT_TYPE GetTypeMateriau();
	virtual void InitProp();
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent);
};

#endif