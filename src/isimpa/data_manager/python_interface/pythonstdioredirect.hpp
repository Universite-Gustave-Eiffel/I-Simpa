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

#include "boost_python_header.h"
#include <wx/string.h>
#include <list>
/**
 * Cette classe va rediriger la sortie de python vers la zone qui lui est réservé dans l'interface
 */

#ifdef USE_PYTHON
#ifndef __PYTHON_STDIO_REDIRECT__
#define __PYTHON_STDIO_REDIRECT__

class PythonStdIoRedirect {
public:
	PythonStdIoRedirect();
    typedef std::list<wxString> ContainerType;
    void Write( std::wstring const& str );
    void WriteWx( const wxString& str );
	void flush();
	boost::python::str ReadLine(  );
	//Lit une ligne de code
	//Si il n'y a aucun code il faut faire attendre
	bool HasOutput();
    bool GetOutput(wxString& strOut);
protected:
    ContainerType m_outputs;
};

#endif
#endif