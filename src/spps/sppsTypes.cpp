#include "sppsTypes.h"


#ifdef __USE_BOOST_RANDOM_GENERATOR__

#include <boost/random/lagged_fibonacci.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>


//base_generator_type correspond au générateur aléatoire choisi
typedef boost::lagged_fibonacci607 base_generator_type;

base_generator_type generator;
// Define a uniform random number distribution which produces "double"
// values between 0 and 1 (0 inclusive, 1 exclusive).
boost::uniform_real<> uni_dist(0,1);
boost::variate_generator<base_generator_type&, boost::uniform_real<> > uni(generator, uni_dist);

float GetRandValue()
{
	return uni();
}

void SetRandSeed(unsigned long seed) {
	generator.seed(seed);
}


#endif