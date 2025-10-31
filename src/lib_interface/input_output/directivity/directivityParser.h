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

#include <iostream>
#include <input_output/directivity/directivityBalloon.h>

#ifndef DIRECTIVITY_PARSER
#define DIRECTIVITY_PARSER

using namespace std;

/**
* @brief Parsing strategy for directivities files
*/
class DirectivityParser 
{
public:
	virtual ~DirectivityParser() {};
	virtual bool parse(std::string filePath, t_DirectivityBalloon *balloon) = 0;
	static const int ANGLE_INCREMENT = 0;
};

/**
* @brief .xhn (ease import) directivity file parser
*/
class xhn_DirectivityParser : public DirectivityParser 
{
public:
	static const int ANGLE_INCREMENT = 5;
	virtual bool parse(std::string filePath, t_DirectivityBalloon *balloon);
};

#endif
