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

#ifndef __E_DIRECTIVITY__
#define __E_DIRECTIVITY__

/** \file e_directivity.h
\brief Directivity database node implementation
*/

/**
\brief Directivity database node implementation
*/

class E_Directivity : public Element
{
protected:
	int idDirectivity;
	void Modified(Element* eModif);
	void InitProperties();

public:
	E_Directivity(Element* parent, wxString Nom, ELEMENT_TYPE _type, wxXmlNode* nodeElement = NULL);
	virtual ~E_Directivity();

	int GetIdDirectivity();
	wxFileName GetAssociatedFile();
	virtual void InitProp();

	Element::ELEMENT_TYPE GetTypeDireciticity();
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);
	virtual wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent);
};
#endif