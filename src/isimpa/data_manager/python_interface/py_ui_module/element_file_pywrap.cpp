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

#include "element_file_pywrap.hpp"
#include "data_manager/tree_rapport/e_report_file.h"
#include "data_manager/python_interface/instanceManager.hpp"
#include "UtfConverter.h"
#include "last_cpp_include.hpp"

#ifdef USE_PYTHON
using namespace boost::python;
using namespace uictrl;

#ifndef REMTR
#define REMTR
#endif

void export_project_class_pyfilelement()
{
	class_<e_file, bases<element> >("e_file",init<int>())
		.def(init<e_file>())
		.def("buildfullpath", &e_file::buildfullpath, REMTR("Retourne le chemin vers ce dossier ou fichier."));
}

E_Report_File* GetFileElement(const wxInt32& xmlId)
{
	void *obj;
	if(InstanceManager::GetReference(OBJECT_REF_TYPE_ELEMENT,xmlId,&obj))
	{
		E_Report_File* el=dynamic_cast<E_Report_File*>( (Element*)obj);
		if(el)
		{
			return el;
		}else{
			ThrowPyException(_("This element doesn't represent a file or a folder"));
			return NULL;
		}
	}else{
		ThrowPyException(_("This element doesn't exist"));
		return NULL;
	}
}


namespace uictrl
{
e_file::e_file(const wxInt32& _xmlId)
:element(_xmlId)
{
	GetFileElement(xmlId);
}
e_file::e_file(const element& cpyFrom)
:element((const element&)cpyFrom)
{
	GetFileElement(xmlId);
}

std::wstring e_file::buildfullpath()
{
	E_Report_File* self=GetFileElement(xmlId);
	wxString path;
	self->BuildFullPath(path);
	return WXSTRINGTOSTDWSTRING(path);
}
}
#endif
