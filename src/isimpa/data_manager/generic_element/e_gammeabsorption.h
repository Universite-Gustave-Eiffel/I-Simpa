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

#ifndef __E_GAMMEABS__
#define __E_GAMMEABS__

/** \file e_gammeabsorption.h
   \brief Classe correspondant au spectre d'un encombrement

   Il calcul automatiquement la valeur du Niveau sonore global moyen à chaque modification d'un de ses membres.
*/

/**
   \brief Classe correspondant au spectre d'un encombrement

   Il calcul automatiquement la valeur du Niveau sonore global moyen à chaque modification d'un de ses membres.	
	@see E_Scene_Encombrements_Encombrement_Model
	@see E_Scene_Encombrements_Encombrement_Cuboide
*/
class E_GammeAbsorption: public Element
{
private:

	void CalcNiveauSonoreGlobalMoyen(wxString typeDb);
	void InitProperties();
	void CopyAverageToEachRow(wxString typeS);
	bool ignoreNextUpdate;
protected:
	void Modified(Element* eModif);
public:
	E_GammeAbsorption(wxXmlNode* nodeElement,Element* parent);
	E_GammeAbsorption(Element* parent);
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);
	virtual wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent);
};

#endif