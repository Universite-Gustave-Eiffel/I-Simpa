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

#include "pythonstdioredirect.hpp"
#include "UtfConverter.h"
#include "last_cpp_include.hpp"

#ifdef USE_PYTHON
using namespace boost::python;


PythonStdIoRedirect::PythonStdIoRedirect()
{
	
}
void PythonStdIoRedirect::Write( std::wstring const& str ) {
    m_outputs.push_back(str);
}
void PythonStdIoRedirect::WriteWx( const wxString& str ) {
    m_outputs.push_back(str);
}

void PythonStdIoRedirect::flush() {

}

boost::python::str PythonStdIoRedirect::ReadLine( )
{
	wxString retStr;
	GetOutput(retStr);
	return WXSTRINGTOCHARPTR(retStr);
}
bool PythonStdIoRedirect::HasOutput()
{
	return m_outputs.size()>0;
}
bool PythonStdIoRedirect::GetOutput(wxString& strOut)
{
	if(m_outputs.size()>0)
	{
		strOut=m_outputs.front();
		m_outputs.pop_front();
		return true;
	}else{
		return false;
	}
}

#endif