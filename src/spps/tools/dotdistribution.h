/**
 * @file dotdistribution.h
 * @brief Classe de distribution de particules sur une source sonore
 */
#include "sppsTypes.h"

#ifndef __PARTICLE_DISTRIBUTION__
#define __PARTICLE_DISTRIBUTION__

/**
 * @brief Classe de distribution de particules
 *
 * Cette classe permet de générer une distribution aléatoire ou non de particules dans un espace en 3 dimensions
 */
class ParticleDistribution
{
public:
	/**
	 * Ajouter à la liste de particules
	 */
	static void GenSphereDistribution(struct CONF_PARTICULE& particle,decimal vecNorm);
	static void GenXYDistribution(struct CONF_PARTICULE& particle,decimal vecNorm);
	static void GenXZDistribution(struct CONF_PARTICULE& particle,decimal vecNorm);
	static void GenYZDistribution(struct CONF_PARTICULE& particle,decimal vecNorm);
};

#endif
