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

//--------------------------------------------------------------------------------------------------------------------\\

t_DirectivityBalloon::t_DirectivityBalloon(string filePath)
{
	ifstream file(filePath, ifstream::in);
	if (!file.is_open()) {
		cerr << "File not open" << endl;
	}

	string line;
	double currentFrequency {0.0};
	double theta_min_value, theta_max_value; // regardless of phi, thetha for 0 and 180° must be constant
	bool thetaBoundDefined = false;
	vector< string > tokens;
	boost::regex Re_notNumber("[^0-9]");

	while (std::getline(file, line))
	{
		if (!line.empty() && line[0] != ';') // not a comment
		{
			boost::split(tokens, line, boost::is_any_of(",")); // split, 60% of cpu time
			if (!tokens.empty())
			{
				// frequency
				if (boost::equals(tokens[0], "\"Frequency\"") && tokens.size() == 3) // use iequals() for case insensitivity
				{
					currentFrequency = stod(tokens[1]);
				}
				// data, no missing value allowed, 40% of cpu time
				else if (tokens.size() == 38 && currentFrequency > 0)
				{
					boost::erase_all_regex(tokens[0], Re_notNumber);
					double phi = stod( tokens[0] );

					// verify if phi is in bound
					if (0 <= phi && phi <= 360 && fmod(phi, t_DirectivityBalloon::ANGLE_INCREMENT) == 0) 
					{
						for (auto i = 1; i < tokens.size(); i++)
						{
							double theta = (i - 1) * t_DirectivityBalloon::ANGLE_INCREMENT;
							double value = stod(tokens[i]);

							if (theta == 0) {
								if (thetaBoundDefined == false) {
									theta_min_value = value;
								}
								attenuations[currentFrequency][phi][theta] = theta_min_value;
							}
							else if (theta == 180) {
								if (thetaBoundDefined == false) {
									theta_max_value = value;
									thetaBoundDefined = true;
								}
								attenuations[currentFrequency][phi][theta] = theta_max_value;
							}
							else {
								attenuations[currentFrequency][phi][theta] = value;
							}
						}
					}
				}
			}
		}
		tokens.clear();
	}

	file.close();

}

t_DirectivityBalloon::~t_DirectivityBalloon()
{
}

bool t_DirectivityBalloon::asDataForFrequency(double frequency)
{
	return !(attenuations.find(frequency) == attenuations.end());
}

bool t_DirectivityBalloon::asValue(double freq, double phi, double theta)
{
	if (!this->asDataForFrequency(freq))
		return false;
	else if (attenuations[freq].find(phi) == attenuations[freq].end())
		return false;
	else if (attenuations[freq][phi].find(theta) == attenuations[freq][phi].end())
		return false;
	else
		return true;
}

bool t_DirectivityBalloon::asInterpolatedValue(double freq, double phi, double theta)
{
	if (!this->asDataForFrequency(freq))
		return false;
	else
		return 0 <= phi && phi <= 360 && 0 <= theta && theta <= 180;
}

double t_DirectivityBalloon::getValue(double freq, double phi, double theta)
{
	return attenuations[freq][phi][theta];
}

double t_DirectivityBalloon::getInterpolatedValue(double freq, double phi, double theta)
{
	if (this->asValue(freq, phi, theta))
	{
		return this->getValue(freq, phi, theta);
	}

	double phi_lowBound, phi_upBound, theta_lowBound, theta_upBound;

	phi_upBound = attenuations[freq].upper_bound(phi)->first;
	phi_lowBound = phi_upBound - t_DirectivityBalloon::ANGLE_INCREMENT;
	theta_upBound = attenuations[freq][phi_lowBound].upper_bound(theta)->first;
	theta_lowBound = theta_upBound - t_DirectivityBalloon::ANGLE_INCREMENT;

	// Bilinear interpolation (but we're on an sphere so it's probably a mistake, or not ?)

	double u = (theta - theta_lowBound) / (theta_upBound - theta_lowBound);
	double t = (phi - phi_lowBound) / (phi_upBound - phi_lowBound);

	double va = (1-u) * attenuations[freq][phi_lowBound][theta_lowBound] + u * attenuations[freq][phi_lowBound][theta_upBound];
	double vb = (1-u) * attenuations[freq][phi_upBound][theta_lowBound] + u * attenuations[freq][phi_upBound][theta_upBound];

	return (1-t) * va + t * vb;
}
