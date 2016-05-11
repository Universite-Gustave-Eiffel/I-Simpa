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

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>

using namespace std;

/**
* @brief Modèle d'un ballon de directivité
*/
class t_DirectivityBalloon
{
public:
	t_DirectivityBalloon(string filePath);
	~t_DirectivityBalloon();

	bool asDataForFrequency(int frequency);
	float getInterpolatedValue(int freq, float phi, float theta);

private:
	// magnitude values as attenuations[frequency][phi][theta] = value
	std::map<int, std::map<int, std::map<int, float>>> attenuations;
};

//--------------------------------------------------------------------------------------------------------------------\\

t_DirectivityBalloon::t_DirectivityBalloon(string filePath)
{
	ifstream file(filePath, ifstream::in);
	if (!file.is_open()) {
		cerr << "File not open" << endl;
	}

	string line;
	int currentFrequency {0};
	vector< string > tokens;
	boost::regex re("[^0-9]");

	while (std::getline(file, line))
	{
		if (!line.empty() && line[0] != ';') // not a comment
		{
			boost::split(tokens, line, boost::is_any_of(",")); // tokenize, 60% of cpu time
			if (!tokens.empty())
			{
				// frequency
				if (tokens.size() == 3 && boost::equals(tokens[0], "\"Frequency\"")) // use iequals() for case insensitivity
				{
					currentFrequency = stoi(tokens[1]);
				}
				// data
				else if (tokens.size() == 38 && currentFrequency > 0)
				{
					// boost::trim(tokens[0]);
					boost::erase_all_regex(tokens[0], re);
					int phi = stoi( tokens[0] );

					for (auto i = 1; i < tokens.size(); i++)
					{
						int theta = (i-1) * 5;
						float value = stol(tokens[i]);
						attenuations[currentFrequency][phi][theta] = value;
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

bool t_DirectivityBalloon::asDataForFrequency(int frequency)
{
	return !(attenuations.find(frequency) == attenuations.end());
}

float t_DirectivityBalloon::getInterpolatedValue(int freq, float phi, float theta)
{
	// TODO
	return 0.0;
}
