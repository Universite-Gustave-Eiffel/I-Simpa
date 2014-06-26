#include "dotdistribution.h"




void ParticleDistribution::GenSphereDistribution(struct CONF_PARTICULE& particle,decimal vecNorm)
{
	using namespace std;

	// Choose a random longitude for the point
	float phi = GetRandValue() * M_2PI;

	// The z axis goes straight through the sphere and is chosen in random.
	float z = GetRandValue() * 2.0f - 1.0f;

	// From that, we'll calculate the latitude this point will have on the
	// sphere's surface.
	float costheta = (float)sqrt(1.0f - z * z);

	// Calculate the final position of the point in world space
	particle.direction.set( vecNorm * costheta * cos(phi),
						vecNorm * costheta * (float)sin(phi),
						vecNorm * z	);
}

void ParticleDistribution::GenXYDistribution(struct CONF_PARTICULE& particle,decimal vecNorm)
{
	using namespace std;

	// Choose a random longitude for the point
	float phi = GetRandValue() * M_2PI;

	// From that, we'll calculate the latitude this point will have on the
	// sphere's surface.
	float theta = M_PIDIV2;

	// Calculate the final position of the point in world space
	particle.direction.set( vecNorm * cos(phi),
						vecNorm * (float)sin(phi),
						0 );
}


void ParticleDistribution::GenXZDistribution(struct CONF_PARTICULE& particle,decimal vecNorm)
{
	using namespace std;

	// From that, we'll calculate the latitude this point will have on the
	// sphere's surface.
	float theta = GetRandValue() * M_2PI;

	// Calculate the final position of the point in world space
	particle.direction.set( vecNorm * cos(theta),
						0,
						vecNorm * sin(theta) );
}


void ParticleDistribution::GenYZDistribution(struct CONF_PARTICULE& particle,decimal vecNorm)
{
	using namespace std;

	// From that, we'll calculate the latitude this point will have on the
	// sphere's surface.
	float theta = GetRandValue() * M_2PI;

	// Calculate the final position of the point in world space
	particle.direction.set( 0,
						vecNorm * cos(theta),
						vecNorm * sin(theta) );
}