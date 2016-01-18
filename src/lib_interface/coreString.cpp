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

#include "coreString.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

CoreString::CoreString(const char* ch):std::string( ch )
{

}

CoreString::CoreString(std::string outputstr):std::string( outputstr )
{

}

CoreString::CoreString(const char* ch, unsigned int length):std::string( ch, length )
{

}

CoreString::CoreString( ):std::string( )
{

}

entier CoreString::ToInt(const CoreString& strtoconv)
{
	return strtoconv.ToInt();
}
decimal CoreString::ToFloat(const CoreString& strtoconv)
{
	return strtoconv.ToFloat();
}
std::string CoreString::FromInt(const entier& valInt)
{
	char buffer[255];
	//itoa(entierLong,buffer,10);
	sprintf(buffer,"%i",valInt);
	std::string fromStr=buffer;
	return fromStr;
}

std::string CoreString::FromFloat(decimal nombreDecimal,entier digits)
{
	char buffer[256];
	//gcvt(nombreDecimal,digits,buffer);
	//std::string fromStr=buffer;
    std::string form;
    std::stringstream form_stream;
    form_stream<<"%"<<digits<<"f";
    form_stream>>form;
    sprintf(buffer,form.c_str(),nombreDecimal);
    std::string fromStr=buffer;
	return fromStr;
}

entier CoreString::ToInt() const
{
	return atoi(this->c_str());
}

decimal CoreString::ToFloat() const
{
	if(this)
	{
		size_type sepPos=this->find(',');
		if(sepPos<this->size())
		{
			CoreString dotnb=*this;
			dotnb.replace(sepPos,1,".");
			return (decimal)atof(dotnb.c_str());
		}else{
			return (decimal)atof(this->c_str());
		}
	}else{
		return 0.f;
	}
}
