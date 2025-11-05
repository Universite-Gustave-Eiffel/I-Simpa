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



#ifdef USE_PYTHON

#ifndef BOOST_PYTHON_STATIC_LIB
	#define BOOST_PYTHON_STATIC_LIB 1
#endif

//#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind/bind.hpp>
using namespace boost::placeholders;

#include <boost/python.hpp>
#include <boost/python/tuple.hpp>

#include <vector>
#include <wx/string.h>

#ifndef __UI_BOOST_PYTHON_HEADER_FUNCS_
#define __UI_BOOST_PYTHON_HEADER_FUNCS_

void ThrowPyException(const std::wstring& _msg);
void ThrowPyException(const char* _msg);

bool hasattr(const boost::python::object& obj, std::wstring const &attrName);
bool hasattr(const boost::python::object& obj, std::string const &attrName);

int GetPythonArraySize(const boost::python::object& pyArr);
void registerexcept();

struct elementException : std::exception
{
	elementException(const std::string& _msg)
	{
		msgData=_msg;
	}
	virtual ~elementException() throw() {};
	char const* msg() const { return msgData.c_str(); }
private:
	std::string msgData;
};

wxString extract_wxstring(const boost::python::object& from);

template <class T>
T extract_or_throw(const boost::python::object& from, const std::wstring& throwmsg=wxT("Unknown data type"))
{
	if(from.ptr())
	{
		boost::python::extract<T> getval(from);
		if (getval.check())
			return getval();
	}
	ThrowPyException(throwmsg);
	return T();
}
template <class T,class V>
bool extract_array(const boost::python::object& pyArr,V* cppArray)
{
	int size=GetPythonArraySize(pyArr);
    for ( int i = 0; i < size; i++ )
	{
		boost::python::extract<T> getval(pyArr[i]);
        if (getval.check())
			cppArray->push_back(getval());
		else
			return false;
	}
	return true;
}



#endif
#endif
