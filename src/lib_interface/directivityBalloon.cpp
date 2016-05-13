# include <directivityBalloon.h>

t_DirectivityBalloon::t_DirectivityBalloon(string filePath)
{
	ifstream file(filePath, ifstream::in);
	if (!file.is_open()) {
		cerr << "File not open" << endl;
	}

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

	double va = (1 - u) * attenuations[freq][phi_lowBound][theta_lowBound] + u * attenuations[freq][phi_lowBound][theta_upBound];
	double vb = (1 - u) * attenuations[freq][phi_upBound][theta_lowBound] + u * attenuations[freq][phi_upBound][theta_upBound];

	return (1 - t) * va + t * vb;
}
