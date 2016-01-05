#include "sppsTypes.h"	//les types de données sont déclaré ici
						// Il est déconseillé d'utiliser d'autre types que ceux déclaré dans ce fichier dans le programme
#include "input_output/reportmanager.h"
#include "data_manager/core_configuration.h"
#include <list>
#ifndef __CALC_CORE__
#define __CALC_CORE__

/**
 * @file CalculationCore.h
 * @brief Moteur de calcul basé sur le calcul particule par particule
 */


void printVec(vec3 inf);
/**
 * @brief Moteur de calcul basé sur le calcul particule par particule
 *
 * Ce moteur de calcul utilise le maillage du modèle afin d'optimiser le temps de recherche de collision.
 */
class CalculationCore
{
public:
	/**
	 * @brief Paramètres du calcul de propagation acoustique
	 *
	 * Structure de données donnant les informations sur les paramètres globaux de calcul
	 */
	struct CONF_CALCULATION
	{
		uentier nbPasTemps;	/*!< Nombre de pas de temps */
		decimal pasTemps;	/*!< Pas de temps (s) */
		std::list<CONF_PARTICULE> duplicatedParticles; /*!< Particules à calculer par la suite */
	};

private:
t_Mesh *sceneMesh;
t_TetraMesh *sceneTetraMesh;
ReportManager *reportTool;
Core_Configuration *configurationTool;
CONF_CALCULATION& confEnv;

public:
	
	/**
	 * @brief Constructeur du moteur de calcul.
	 * 
	 * @param _pVertices Tableau de points du modèle
	 * @param _pGroups Tableau de faces du modèle
	 * @param intersTool Outils de calcul d'intersection
	 * @param confEnv Configuration générale de l'environnement
	 * @param _reportTool Classe d'enregistrements des résultats de calculs.
	 * @param _configurationTool Classe de gestion de configuration
	 */
	CalculationCore(t_Mesh& _sceneMesh,t_TetraMesh& _sceneTetraMesh,CONF_CALCULATION &confEnv, Core_Configuration &_configurationTool,ReportManager* _reportTool);
	/**
	 * Execute le calcul pour une particule
	 * @param configurationP Configuration de la particule
	 * @return Vrai si le calcul c'est effectué avec succès 
	 */
	bool Run(CONF_PARTICULE configurationP);

private:
	void Mouvement(CONF_PARTICULE &configurationP);
	inline decimal GetDistance(CONF_PARTICULE &configurationP);
	bool CollisionTest(CONF_PARTICULE &configurationP,uentier &faceIndex,INTERSECTION_INFO &infoIntersection, float &factDistance);
	void SetNextParticleCollision(CONF_PARTICULE &configurationP);
	/**
	 * Réévalution de la distance entre la particule et un élément présent dans l'encombrement.(par tirage aléatoire)
	 */
	void SetNextParticleCollisionWithObstructionElement(CONF_PARTICULE &configurationP);
	/**
	 * Translation d'une particule dans un milieu libre de collisions avec la scène
	 */
	void FreeParticleTranslation(CONF_PARTICULE &configurationP,const vec3 &translationVector);
	/**
	 * Permet de connaitre la collision de la particule sur la face du tetrahèdre courant
	 * @return Indice de la face [1-4] (-1 si la particule n'est pas dans un tetrahèdre)
	 * @param t facteur de translationVector afin d'arrive a cette face
	 */
	entier_court GetTetraFaceCollision(CONF_PARTICULE &configurationP,vec3 &translationVector,float &t);

	void OnChangeCelerite(CONF_PARTICULE &configurationP, t_Tetra* tetra2);
	void TraverserTetra(CONF_PARTICULE &configurationP, bool& collisionResolution);
};

#endif