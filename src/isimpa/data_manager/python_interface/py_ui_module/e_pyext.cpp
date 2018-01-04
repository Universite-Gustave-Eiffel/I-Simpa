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

#include "e_pyext.hpp"
#include <wx/log.h>
#include "data_manager/tree_core/e_core_core.h"
#include "data_manager/drawable_element.h"
#include "data_manager/tree_userpref/e_userprefnode.hpp"
#include "last_cpp_include.hpp"

#define MAKE_ELEMENT_PYTHON_DECLARATION(TYPE_EL) newel=(E_Python_Extension<Element>*)new E_Python_Extension<TYPE_EL>(parent,baseType,module,pyclass,noeudCourant);
												

#ifdef USE_PYTHON

/**
 * C'est ici qu'il faut ajouter les déclaration pour les éléments pouvant être surchargé par python.
 *
 **/

Element* SwitchNewElement(const Element::ELEMENT_TYPE& baseType,Element* parent,const wxString& module,const wxString& pyclass="pyel",bp::object* pyobj=NULL,wxXmlNode* noeudCourant=NULL)
{
	E_Python_Extension<Element>* newel;
	switch(baseType)
	{
		case Element::ELEMENT_TYPE_CORE_CORE:
			MAKE_ELEMENT_PYTHON_DECLARATION(E_Core_Core);	
			break;
		case Element::ELEMENT_TYPE_DRAWABLE:
			MAKE_ELEMENT_PYTHON_DECLARATION(E_Drawable);	
			break;
		case Element::ELEMENT_TYPE_USER_PREFERENCE_ITEM:
			MAKE_ELEMENT_PYTHON_DECLARATION(E_UserPreferenceItem);	
		case Element::ELEMENT_TYPE_USER_PREFERENCE_NODE:
			MAKE_ELEMENT_PYTHON_DECLARATION(E_UserPreferenceNode);
		default:
			MAKE_ELEMENT_PYTHON_DECLARATION(Element);	
	}
	if(pyobj)
		*pyobj=newel->GetPyObj();
	return newel;
}




Element* New_E_Python_Extension( const Element::ELEMENT_TYPE& baseType,Element* parent,const wxString& module,const wxString& pyclass)
{
	return SwitchNewElement(baseType,parent,module,pyclass);
}


Element* New_E_Python_Extension( const Element::ELEMENT_TYPE& baseType,Element* parent,const wxString& module,const wxString& pyclass,bp::object& pyobj)
{
	return SwitchNewElement(baseType,parent,module,pyclass,&pyobj);
}
Element* New_E_Python_Extension( Element* parent,wxXmlNode* noeudCourant)
{
	wxString propValue;
	noeudCourant->GetAttribute("beid",&propValue);
	
	Element::ELEMENT_TYPE baseType=(Element::ELEMENT_TYPE)Convertor::ToInt(propValue);
	return SwitchNewElement(baseType,parent,"","",NULL,noeudCourant);
}

Element::ELEMENT_TYPE GetPythonExtBaseType( Element* pythonEl)
{
	E_Python_Extension<Element>* castedEl=static_cast<E_Python_Extension<Element>*>(pythonEl);
	if(castedEl)
	{
		return castedEl->GetBaseElementId();
	}else{
		return Element::ELEMENT_TYPE_ELEMENT;
	}
}

#endif