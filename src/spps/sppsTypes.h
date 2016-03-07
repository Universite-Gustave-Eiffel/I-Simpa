#include "coreTypes.h"

#ifndef SPPS_TYPES
#define SPPS_TYPES



#define SPPS_VERSION "Spps 2.1.4 version december 13 2013"
#define __USE_BOOST_RANDOM_GENERATOR__
#define UTILISER_MAILLAGE_OPTIMISATION




#ifdef __USE_BOOST_RANDOM_GENERATOR__
	/**
	 * Cette méthode permet le tirage aléatoire d'un nombre décimal
	 * @return Decimal de 0 à 1
	 */
	float GetRandValue();
	void SetRandSeed(unsigned long seed);
#else
	/**
	 * Cette méthode permet le tirage aléatoire d'un nombre décimal
	 * @return Decimal de 0 à 1
	 */
	inline float GetRandValue()
	{
		return ((float)rand()) / (float)RAND_MAX;
	}
	inline void SetRandSeed(unsigned long seed) {
		srand(seed);
	}
#endif

/**
 * @brief Informations de collision
 *
 * Structure de données donnant les informations sur l'evenement de collision avec une face ( maillage ou modèle )
 */

struct INTERSECTION_INFO
{
	vec3 collisionPosition;				/*!< Coordonnées de collision*/
	entier idface;						/*!< Indice de la face concernée (face scène ou face du tétraèdre courant) */
};

/**
 * Etat de la particule, utilisé uniquement pour les statistiques à la fin du calcul
 */
enum PARTICULE_STATE
{
	PARTICULE_STATE_ALIVE,				/*!< Active, état initial */
	PARTICULE_STATE_LOST,				/*!< Supprimé car impossible de situer le tétraèdre */
	PARTICULE_STATE_ABS_ATMO,			/*!< Supprimé car absorbé par l'atmosphère */
	PARTICULE_STATE_ABS_SURF,			/*!< Supprimé car absorbé par une surface */
	PARTICULE_STATE_LOOP,				/*!< Supprimé car impossible de résoudre un pas de temps (boucle infinie) */
	PARTICULE_STATE_ABS_ENCOMBREMENT	/*!< Supprimé car absorbé par une surface */
};
/**
 * @brief Informations propres à une particule
 *
 * Structure de données donnant les informations sur une particule
 */
struct CONF_PARTICULE
{
	bool outputToParticleFile;					/*!< A vrai cette particule sera visible dans la vue 3D	*/
	vec3 position;								/*!< Position x,y,z (m) */
	vec3 direction;								/*!< Mvmt x,y,z (m/s), la norme correspond à la célérité */
	t_freq frequenceIndex;						/*!< Indice de la frequence de la particule (de 0 à n fréquences) */
	t_Tetra* currentTetra;						/*!< MailleCourante de la particule */
	uentier_court pasCourant;					/*!< Pas de temps courant */
	uentier reflectionOrder;					/*!< Ordre de reflexion, utilisé uniquement si outputToParticleFile à vrai*/
	INTERSECTION_INFO nextModelIntersection;	/*!< Prochaine intersection avec le modèle */
	l_decimal energie;							/*!< Energie de la particule (0: la particule a disparu) */
	decimal distanceToNextEncombrementEle;		/*!< Distance par rapport à la prochaine collision */
	PARTICULE_STATE stateParticule;				/*!< Etat de la particule, utilisé uniquement pour les statistiques à la fin du calcul */
	l_decimal energie_epsilon;					/*!< Limite d'energie pour laquelle où dû a la transmission la duplication de la particule est impossible (=Energie initiale*Paramètre de limite de transmission) */
	decimal elapsedTime;						/*!< Temps écoulé depuis le début du pas de temps actuel pasCourant*/
	unsigned int sourceid;						/*!< Indice de la source d'où cette particule a été tirée */
	CONF_PARTICULE(){sourceid=0;currentTetra=NULL;distanceToNextEncombrementEle=0.f;stateParticule=PARTICULE_STATE_ALIVE;elapsedTime=0.;reflectionOrder=0;}
};



#endif
