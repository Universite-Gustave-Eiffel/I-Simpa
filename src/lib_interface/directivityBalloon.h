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

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

#ifndef DIRECTIVITY_BALLOON
#define DIRECTIVITY_BALLOON

using namespace std;

/**
* @brief Modèle d'un ballon de directivité
*/
class t_DirectivityBalloon
{
public:
	/** 
	* Constructor using a directivity file
	* @param filePath File to parse
	*/
	t_DirectivityBalloon(string filePath);
	~t_DirectivityBalloon();

	bool asDataForFrequency(double frequency);
	bool asValue(double freq, double phi, double theta);
	bool asInterpolatedValue(double freq, double phi, double theta);

	/**
	* Get a value at precise coordinate
	* @pre With same parameter, asValue() return true
	* @see asValue()
	*
	* @param freq Frequency
	* @param phi Phi, azimuth angle (0 to 360°)
	* @param theta Theta, polar/inclination angle (0 to 180°)
	*/
	double getValue(double freq, double phi, double theta);

	/**
	* Get a value at valid coordinate
	* @pre With same parameter, asInterpolatedValue() return true
	* @see asInterpolatedValue()
	*
	* @param freq Frequency
	* @param phi Phi, azimuth angle (0 to 360°)
	* @param theta Theta, polar/inclination angle (0 to 180°)
	* @return A value wich is the bilinear interpolation ok the 4 nearest.
	*/
	double getInterpolatedValue(double freq, double phi, double theta);

private:
	/** @brief store magnitude values as attenuations[frequency][phi][theta] = value */
	std::map<double, std::map<double, std::map<double, double>>> attenuations;

	static const int ANGLE_INCREMENT = 5;
};

#endif
