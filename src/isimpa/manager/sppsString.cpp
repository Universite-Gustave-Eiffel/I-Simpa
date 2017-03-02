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

#include "stringTools.h"
#include <wx/tokenzr.h>
#include <wx/log.h> 
#include <wx/intl.h> 
#include <sstream>
#include "last_cpp_include.hpp"

wxString Convertor::fromConvDecimal=' ';
wxString Convertor::toConvDecimal=' ';

double Convertor::ToFloat( const wxString& sval )
{
	return ToFloat(sval.ToStdString());
}

double Convertor::ToFloat(const std::string& sval) {
	std::istringstream iss(sval);
	iss.imbue(std::locale::classic());

	float value(0.f);
	if (iss >> value) {
		return value;
	}
	else {
		wxLogError(_("Cannot convert \"%s\" to decimal value"), sval);
		return strtod(sval.c_str(), NULL);
	}
}
char Convertor::getConvFrom()
{
	if(fromConvDecimal==' ')
		updateDecimalChar();
	return fromConvDecimal.GetChar(0);
}


void SplitString(wxString src,wxString delimiter,wxArrayString& dst)
{
	wxStringTokenizer tkz(src, delimiter);
	while ( tkz.HasMoreTokens() )
	{
		dst.Add(tkz.GetNextToken());
	}
}
void SplitString(wxString src,wxString delimiter,std::vector<wxFloat32>& dst)
{
	wxStringTokenizer tkz(src, delimiter);
	while ( tkz.HasMoreTokens() )
	{
		dst.push_back(Convertor::ToFloat(tkz.GetNextToken()));
	}
}
char Convertor::getConvTo()
{
	if(toConvDecimal==' ')
		updateDecimalChar();
	return toConvDecimal.GetChar(0);
}
std::string Convertor::ToString( double fval , int precision)
{
		std::ostringstream oss;
		oss.imbue(std::locale::classic());
		oss.precision(precision);
		oss << fval;
		return oss.str();
}

std::string Convertor::ToString( int ival)
{
	std::ostringstream oss;
	oss.imbue(std::locale::classic());
	oss << ival;
	return oss.str();
}
int Convertor::ToInt( const wxString& sval )
{
		long rval;
		sval.ToLong(&rval);
		return rval;
}

long long Convertor::ToLong( wxString sval )
{
		long long rval;
		sval.ToLongLong(&rval);
		return rval;
}

void Convertor::updateDecimalChar()
{
	struct lconv locale_structure;
	struct lconv *locale_ptr=&locale_structure;
	locale_ptr=localeconv();
	locale_ptr->decimal_point;

	if(*locale_ptr->decimal_point=='.')
	{
		fromConvDecimal=',';
		toConvDecimal='.';
	}else{
		fromConvDecimal='.';
		toConvDecimal=',';
	}
}
