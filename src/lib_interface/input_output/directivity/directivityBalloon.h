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

#include <map>
#include <string>
#include <Core/mathlib.h>

#ifndef DIRECTIVITY_BALLOON
#define DIRECTIVITY_BALLOON

/**
* @brief Data structure for Directivity Balloon 
*/
class t_DirectivityBalloon
{
private:
	/** @brief store magnitude values as attenuations[frequency][phi][theta] = value */
	std::map<double, std::map<double, std::map<double, double>>> attenuations;
	int AngleIncrement;

public:
	/** 
	* Constructor using a Parsing strategy
	* @param parsingStrategy a class implementing the DirectivityParser interface
	* @see DirectivityParser
	*/
	t_DirectivityBalloon();
	~t_DirectivityBalloon();

	void setValue(double freq, double phi, double theta, double value);
	void setAngleIncrement(int _AngleIncrement);

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

	/**
	* Convert cartesian coordinate in spherical coordinate
	* @return (phi, theta) <=> (azimuth, polar) in radian, with 0 < phi < 2PI and 0 <= theta <= PI
	*/
	static std::tuple<double, double> cartesianToSpherical(const vec3 &v);

	/**
	* Convert spherical coordinate in cartesian coordinate
	* @param phi, the azimuth angle in radian in the interval [0, 2pi]
	* @param theta, the polar angle in radian in the interval [0, pi]
	*/
	static vec3 sphericalToCartesian(double radius, double phi, double theta);

	/**
	* Convert a coordinate from the conventionnal coordinate system to the directivity one
	*
	* The conventionnal spherical coordinate system used in physics as :
	* - a polar angle (theta) which is the inclination from the Z direction
	* - an azimuth (phi) angle mesured from the X axis in the XY plane
	*
	* The directivity specification use a different spherical coordinate system where :
	* - the polar angle (theta) is the inclination from the Y direction
	* -	the azimuth (phi) angle is mesured from the opposite X axis in the XZ plane
	*/
	static std::tuple<double, double> loudspeaker_coordinate(const vec3 &source_Direction, const vec3 &particle);
};
#endif
