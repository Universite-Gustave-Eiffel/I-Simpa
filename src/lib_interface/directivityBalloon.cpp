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

#include <directivityBalloon.h>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

using namespace std;

t_DirectivityBalloon::t_DirectivityBalloon(string filePath)
{
	ifstream file(filePath, ifstream::in);
	if (!file.is_open()) {
		cout << "DirectivityBalloon : File not open" << endl;
	}
	else
	{
		string line;
		double currentFrequency{ 0.0 };
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
						double phi = stod(tokens[0]);

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

	if (attenuations[freq].upper_bound(phi) != attenuations[freq].end()) {
		phi_upBound = attenuations[freq].upper_bound(phi)->first;
		phi_lowBound = phi_upBound - t_DirectivityBalloon::ANGLE_INCREMENT;
	} else {
		phi_upBound = 0;
		phi_lowBound = 355;
	}
	theta_upBound = attenuations[freq][phi_lowBound].upper_bound(theta)->first;
	theta_lowBound = theta_upBound - t_DirectivityBalloon::ANGLE_INCREMENT;

	// Bilinear interpolation

	double u = (theta - theta_lowBound) / (theta_upBound - theta_lowBound);
	double t = (phi - phi_lowBound) / (phi_upBound - phi_lowBound);

	double va = (1 - u) * attenuations[freq][phi_lowBound][theta_lowBound] + u * attenuations[freq][phi_lowBound][theta_upBound];
	double vb = (1 - u) * attenuations[freq][phi_upBound][theta_lowBound] + u * attenuations[freq][phi_upBound][theta_upBound];

	return (1 - t) * va + t * vb;
}

tuple<double, double> t_DirectivityBalloon::cartesianToSpherical(const vec3 &v)
{
	double r = sqrt(pow(v.x, 2.0) + pow(v.y, 2.0) + pow(v.z, 2.0));
	double polar = acos(v.z / r);
	double azimuth = atan2(v.y, v.x);
	if (azimuth < 0)
	{
		azimuth = M_2PI + azimuth;
	}
	return make_tuple(azimuth, polar);
}

vec3 t_DirectivityBalloon::sphericalToCartesian(double radius, double phi, double theta)
{
	double x = radius * sin(theta) * cos(phi);
	double y = radius * sin(theta) * sin(phi);
	double z = radius * cos(theta);
	vec3 v(x, y, z);
	return v;
}

tuple<double, double> t_DirectivityBalloon::loudspeaker_coordinate(const vec3 &source_Direction, const vec3 &particle)
{
	vec3 intermediate_Direction(0, 1, 0); // front (y axis)

	auto source_Direction_sph = cartesianToSpherical(source_Direction);
	auto intermediate_Direction_sph = cartesianToSpherical(intermediate_Direction);

	double delta_Phi = get<0>(intermediate_Direction_sph) - get<0>(source_Direction_sph);
	double delta_Theta = get<1>(intermediate_Direction_sph) - get<1>(source_Direction_sph);

	vec3 xaxis(1, 0, 0);
	vec3 yaxis(0, 1, 0);
	vec3 zaxis(0, 0, 1);

	vec3 particle_spk = particle.Rotation(zaxis, delta_Phi);
	particle_spk = particle_spk.Rotation(xaxis, -delta_Theta);

	particle_spk = particle_spk.Rotation(yaxis, M_PI);
	particle_spk = particle_spk.Rotation(xaxis, M_PIDIV2);

	return cartesianToSpherical(particle_spk);
}
