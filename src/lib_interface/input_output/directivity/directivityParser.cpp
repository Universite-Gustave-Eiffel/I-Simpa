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

#include <input_output/directivity/directivityParser.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#ifdef WIN32
#include "input_output/pugixml/src/pugixml.hpp"
#endif // WIN32

using namespace std;

bool xhn_DirectivityParser::parse(std::string filePath, t_DirectivityBalloon *balloon)
{
	#ifdef WIN32
		ifstream file(pugi::as_wide(filePath), ifstream::in);
	#else
		ifstream file(filePath, ifstream::in);
	#endif // WIN
	if (!file.is_open()) {
		cout << "DirectivityBalloon : File not open" << endl;
		return false;
	}
	else
	{
		string line;
		double currentFrequency{ 0.0 };
		double theta_min_value, theta_max_value; // regardless of phi, thetha for 0 and 180� must be constant
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
						if (0 <= phi && phi <= 360 && fmod(phi, xhn_DirectivityParser::ANGLE_INCREMENT) == 0)
						{
							for (auto i = 1; i < tokens.size(); i++)
							{
								double theta = (i - 1) * xhn_DirectivityParser::ANGLE_INCREMENT;
								double value = stod(tokens[i]);

								if (theta == 0) {
									if (thetaBoundDefined == false) {
										theta_min_value = value;
									}
									balloon->setValue(currentFrequency, phi, theta, theta_min_value);
								}
								else if (theta == 180) {
									if (thetaBoundDefined == false) {
										theta_max_value = value;
										thetaBoundDefined = true;
									}
									balloon->setValue(currentFrequency, phi, theta, theta_max_value);
								}
								else {
									balloon->setValue(currentFrequency, phi, theta, value);
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
	balloon->setAngleIncrement(xhn_DirectivityParser::ANGLE_INCREMENT);
	return true;
}

