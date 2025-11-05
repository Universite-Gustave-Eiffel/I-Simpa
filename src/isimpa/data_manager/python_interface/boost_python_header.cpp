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

#include "boost_python_header.h"
#include "UtfConverter.h"
#include "last_cpp_include.hpp"

#ifdef USE_PYTHON
using namespace boost::python;


void translator(elementException const& x) {
    PyErr_SetString(PyExc_UserWarning, x.msg());
}
void ThrowPyException(const std::wstring& _msg)
{
	throw elementException(UtfConverter::ToUtf8(_msg));
}
void ThrowPyException(const char* _msg)
{
	throw elementException(_msg);
}


bool hasattr(const boost::python::object& obj, std::wstring const &attrName) {
     return PyObject_HasAttrString(obj.ptr(), UtfConverter::ToUtf8(attrName).c_str());
}

bool hasattr(const boost::python::object& obj, std::string const &attrName) {
     return PyObject_HasAttrString(obj.ptr(), attrName.c_str());
}
int GetPythonArraySize(const boost::python::object& pyArr)
{
	return extract_or_throw<int>(pyArr.attr("__len__")());
}



void registerexcept()
{
  register_exception_translator<elementException>(translator);
}

void impl_templates()
{
	/*
	extract_or_throw<int>(object());
	extract_or_throw<bool>(object());
	extract_or_throw<object>(object());
	extract_or_throw<list>(object());
	extract_or_throw<float>(object());
	extract_or_throw<std::wstring>(object());
	extract_or_throw<tuple>(object());
*/
	/*
	std::vector<object> impl_vobjects;
	std::vector<std::wstring> impl_vstring;
	const boost::python::list val;
	boost::python::dict parameters;

	extract_array<std::wstring>(parameters.keys(),&impl_vstring);
	extract_array<object>(val,&impl_vobjects);

	printf("%i",impl_vstring.size());
	*/
}
wxString extract_wxstring(const boost::python::object& from)
{
	std::string object_classname = boost::python::extract<std::string>(from.attr("__class__").attr("__name__"));
	if(object_classname == "str")
	{
		//standart string
		return extract_or_throw<std::wstring>(from);
	}else if(object_classname =="unicode")
	{
		//Unicode
		return extract_or_throw<std::wstring>(from);
	}
	//Unknown type format
	return extract_or_throw<std::string>(from);
}

#endif
