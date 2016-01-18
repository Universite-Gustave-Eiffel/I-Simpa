#include "CalculationCore.h"
#include "tools/collision.h"
#include "tools/dotreflection.h"
#include "tools/dotdistribution.h"
#include <iostream>
#ifndef INTSIGN
	#define INTSIGN(x) ((x < 0) ? -1 : 1 );
#endif
#define TetraFaceTest(idFace) sommetsFace=configurationP.currentTetra->faces[idFace].indiceSommets;\
			if(configurationP.currentTetra->faces[idFace].normal.dot(translationVector)<EPSILON)\
				if(collision_manager::intersect_triangle(posPart,dirPart,scenenodes[sommetsFace.a].v,scenenodes[sommetsFace.b].v,scenenodes[sommetsFace.c].v,&t,&u,&v)==1)\
					return idFace;

void printVec(vec3 inf)
{
	using namespace std;
	std::string retStr;
	cout<<"["<<inf.x<<";"<<inf.y<<";"<<inf.z<<"]";
}



CalculationCore::CalculationCore(t_Mesh& _sceneMesh,t_TetraMesh& _sceneTetraMesh,CONF_CALCULATION &_confEnv, Core_Configuration &_configurationTool,ReportManager* _reportTool)
:confEnv(_confEnv)
{
	sceneMesh=&_sceneMesh;
	configurationTool=&_configurationTool;
	sceneTetraMesh=&_sceneTetraMesh;
	reportTool=_reportTool;
}

void CalculationCore::SetNextParticleCollisionWithObstructionElement(CONF_PARTICULE &configurationP)
{
	if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_ENCOMBREMENT) && configurationP.currentTetra->volumeEncombrement)
	{
		//Tirage aléatoire de la distance
		configurationP.distanceToNextEncombrementEle=-configurationP.currentTetra->volumeEncombrement->encSpectrumProperty[configurationP.frequenceIndex].lambda*log((float)(1-GetRandValue()));
	}
}

bool CalculationCore::Run(CONF_PARTICULE configurationP)
{
	decimal densite_proba_absorption_atmospherique=configurationTool->freqList[configurationP.frequenceIndex]->densite_proba_absorption_atmospherique;
	SetNextParticleCollision(configurationP);						//1er test de collision
	SetNextParticleCollisionWithObstructionElement(configurationP);	// Test de collision avec objet virtuel encombrant
	//Au premier pas de temps il faut enregistrer l'energie de la particule dans la maille courante
	//configurationP.stateParticule=PARTICULE_STATE_ALIVE;
	reportTool->ParticuleGoToNextTetrahedra(configurationP,configurationP.currentTetra);
	while(configurationP.stateParticule==PARTICULE_STATE_ALIVE && configurationP.pasCourant<confEnv.nbPasTemps)
	{
		//Test d'absorption atmosphérique
		if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_ABS_ATMO))
		{
			//Test de méthode de calcul
			if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_ENERGY_CALCULATION_METHOD))
			{ //Energetique
				configurationP.energie*=densite_proba_absorption_atmospherique;
				if(configurationP.energie<=configurationP.energie_epsilon)
				{
					configurationP.stateParticule=PARTICULE_STATE_ABS_ATMO;
				}
			}else{
				//Aléatoire
				if(GetRandValue()>=densite_proba_absorption_atmospherique)
				{
					configurationP.energie=0;	// La particule est détruite
					configurationP.stateParticule=PARTICULE_STATE_ABS_ATMO;
				}
			}
		}

		if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
			Mouvement(configurationP);	//Effectue un mouvement sur la distance restante


		//Fin du pas de temps, la particule a effectué aucune ou plusieurs collisions
		if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
		{
			reportTool->RecordTimeStep(configurationP);
			if(configurationP.currentTetra->z!=-1)								//Si le milieu n'est pas homogène
			{
				OnChangeCelerite(configurationP,configurationP.currentTetra);	//On calcul le changement de direction dû au gradiant de célérité
				SetNextParticleCollision(configurationP);
			}
			configurationP.pasCourant++;
		}
	}
	switch(configurationP.stateParticule)
	{
		case PARTICULE_STATE_ALIVE:
			reportTool->statReport.partAlive++;
			break;
		case PARTICULE_STATE_ABS_SURF:
			reportTool->statReport.partAbsSurf++;
			break;
		case PARTICULE_STATE_ABS_ATMO:
			reportTool->statReport.partAbsAtmo++;
			break;
		case PARTICULE_STATE_ABS_ENCOMBREMENT:
			reportTool->statReport.partAbsEncombrement++;
			break;
		case PARTICULE_STATE_LOOP:
			reportTool->statReport.partLoop++;
			break;
		case PARTICULE_STATE_LOST:
			reportTool->statReport.partLost++;
			break;
	}
	reportTool->statReport.partTotal++;
	return true;
}

void CalculationCore::Mouvement(CONF_PARTICULE &configurationP)
{
	decimal deltaT=*configurationTool->FastGetConfigValue(Core_Configuration::FPROP_TIME_STEP) ;
	decimal distanceSurLePas=configurationP.direction.length();
	decimal celeriteLocal=distanceSurLePas/deltaT;
	decimal faceDirection;
	bool collisionResolution=true; //On test de nouveau la collision dans le pas de temps courant si cette valeur est à vrai
	int iteration=0;
	decimal distanceCollision=0.f;
	decimal distanceAParcourir=0.f;
	while(collisionResolution && configurationP.stateParticule==PARTICULE_STATE_ALIVE)
	{
		iteration++;
		collisionResolution=false;
		//Si il y a collision avec une face (avec prise en compte de la distance parcourue)
		distanceCollision=(configurationP.nextModelIntersection.collisionPosition-configurationP.position).length();
		distanceAParcourir=celeriteLocal*(deltaT-configurationP.tempsEcoule);

		//Test de collision avec un élément de l'encombrement entre la position de la particule et une face du tetrahèdre courant.
		if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_ENCOMBREMENT) && distanceAParcourir>=configurationP.distanceToNextEncombrementEle && distanceCollision>configurationP.distanceToNextEncombrementEle && configurationP.currentTetra->volumeEncombrement)
		{
			//Collision avec un élément virtuel de l'encombrement courant

			//Test d'absorption

			if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_ENERGY_CALCULATION_METHOD))
			{
				//Energétique
				configurationP.energie*=(1-configurationP.currentTetra->volumeEncombrement->encSpectrumProperty[configurationP.frequenceIndex].alpha);
				if(configurationP.energie<=configurationP.energie_epsilon)
				{
					configurationP.stateParticule=PARTICULE_STATE_ABS_ENCOMBREMENT;
					return;
				}
			}else{
				//Aléatoire
				if( GetRandValue()<=configurationP.currentTetra->volumeEncombrement->encSpectrumProperty[configurationP.frequenceIndex].alpha)
				{
					//Absorbé
					configurationP.energie=0.f;
					configurationP.stateParticule=PARTICULE_STATE_ABS_ENCOMBREMENT;
					return;
				}
			}
			//N'est pas absorbé

			//On incrémente le temps de parcourt entre la position avant et aprés collision avec l'encombrement virtuel
			configurationP.tempsEcoule+=configurationP.distanceToNextEncombrementEle/celeriteLocal;
			//On place la particule sur la position de collision
			FreeParticleTranslation(configurationP,(configurationP.direction/configurationP.direction.length())*configurationP.distanceToNextEncombrementEle);
			collisionResolution=true;
			//On change la direction de la particule en fonction de la loi de distribution
			vec3 newDir;
			switch(configurationP.currentTetra->volumeEncombrement->encSpectrumProperty[configurationP.frequenceIndex].law_diffusion)
			{
				case DIFFUSION_LAW_UNIFORM:
					ParticleDistribution::GenSphereDistribution(configurationP,configurationP.direction.length());
					break;
				case DIFFUSION_LAW_REFLEXION_UNIFORM:
					newDir=ReflectionLaws::FittingUniformReflection(configurationP.direction);
					newDir.normalize();
					configurationP.direction=newDir*configurationP.direction.length();
					break;
				case DIFFUSION_LAW_REFLEXION_LAMBERT:
					newDir=ReflectionLaws::FittingLambertReflection(configurationP.direction);
					newDir.normalize();
					configurationP.direction=newDir*configurationP.direction.length();
					break;
				
			};
			//Calcul du nouveau point de collision
			SetNextParticleCollision(configurationP);
			SetNextParticleCollisionWithObstructionElement(configurationP);
		}else if(distanceCollision<=distanceAParcourir) // && configurationP.nextModelIntersection.idface!=-1
		{
			//Enregistrement de l'énergie passé à la paroi
			reportTool->ParticuleCollideWithSceneMesh(configurationP);


			vec3 vecTranslation=configurationP.nextModelIntersection.collisionPosition-configurationP.position;
			//On incrémente le temps de parcourt entre la position avant et aprés collision
			configurationP.tempsEcoule+=(vecTranslation/configurationP.direction.length()).length()*deltaT;

			//On place la particule sur la position de collision
			FreeParticleTranslation(configurationP,vecTranslation);

			// Récuperation de l'information de la face
			t_cFace* faceInfo=NULL;

			#ifdef UTILISER_MAILLAGE_OPTIMISATION
			faceInfo=configurationP.currentTetra->faces[configurationP.nextModelIntersection.idface].face_scene;

			//test de passage d'un tétraèdre à un autre

			//Vrai si la paroi est anormalement orientée
			bool doInvertNormal(false);
			if(faceInfo)
			{
				faceDirection=configurationP.direction.dot(faceInfo->normal);
				doInvertNormal=(faceDirection<=-BARELY_EPSILON);
			}
			//On traverse la paroi du tetrahedre si (pas de résolution de collision si)
			//	- Ce n'est pas une surface du modèle
			//  - (ou) Elle n'est pas orientée vers nous et le matériau n'affecte les surfaces sur une orientation
			//  - (ou) Cette surface est un encombrement et qu'un autre volume nous attend derrière
			if(!faceInfo || ((faceInfo->faceEncombrement || (!(faceInfo->faceMaterial->doubleSidedMaterialEffect) && doInvertNormal)) && configurationP.currentTetra->voisins[configurationP.nextModelIntersection.idface]))
			{
				TraverserTetra(configurationP,collisionResolution);
			}else{
			#else
			faceInfo=&sceneMesh->pfaces[configurationP.nextModelIntersection.idface];
			///////////////////////////////////
			// Test de passage d'un milieu libre à un milieu encombré (et inversement)
			if(!faceInfo->faceEncombrement)
			{
			#endif

				//On stocke le materiau dans la variable materialInfo
				t_Material_BFreq* materialInfo=&(*faceInfo).faceMaterial->matSpectrumProperty[configurationP.frequenceIndex];


				bool transmission=false;
				//Tirage aléatoire pour le test d'absorption
				if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_CHAMP_DIRECT))
				{
					//Particule absorbée
					if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
						configurationP.stateParticule=PARTICULE_STATE_ABS_SURF;
					configurationP.energie=0.f;
					return;
				}else{
					if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_ENERGY_CALCULATION_METHOD))
					{
						//Methode énérgétique, particule en collision avec la paroi
						//Particule courante = (1-alpha)*epsilon
						//Si l'absorption est totale la particule est absorbée si tau=0
						if(materialInfo->absorption==1) //Pas de duplication possible de la particule (forcement non réfléchie)
						{
							if(!materialInfo->dotransmission || !(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_TRANSMISSION)))
							{
								if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
									configurationP.stateParticule=PARTICULE_STATE_ABS_SURF;
								configurationP.energie=0;
								return;
							}else{
								transmission=true;
								configurationP.energie*=materialInfo->tau;
							}
						}else{
							if(materialInfo->absorption!=0) //Pas de duplication possible de la particule (forcement réfléchie)
							{
								if(materialInfo->dotransmission && materialInfo->tau!=0 && configurationP.energie*materialInfo->tau>configurationP.energie_epsilon && (*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_TRANSMISSION)))
								{
									//On va dupliquer la particule
									CONF_PARTICULE configurationPTransmise=configurationP;
									configurationPTransmise.energie*=materialInfo->tau;
									bool localcolres;
									TraverserTetra(configurationPTransmise,localcolres);
									//configurationPTransmise.currentTetra=configurationP.currentTetra->voisins[configurationP.nextModelIntersection.idface];
									if(configurationPTransmise.energie>configurationPTransmise.energie_epsilon)
									{
										confEnv.duplicatedParticles.push_back(configurationPTransmise);
										/*
										reportTool->SaveParticule();
										reportTool->NewParticule(configurationPTransmise);
										Run(configurationPTransmise);
										reportTool->SaveParticule();
										reportTool->NewParticule(configurationP);
										*/
									}
								}
								configurationP.energie*=(1-materialInfo->absorption);
							} //else reflexion sans absorption
						}
					}else{
						//Test d'absorption en aléatoire
						if(GetRandValue()<=materialInfo->absorption)
						{
							// Particule non réfléchie
							if((*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_TRANSMISSION)) && materialInfo->dotransmission && configurationP.currentTetra->voisins[configurationP.nextModelIntersection.idface] && GetRandValue()*materialInfo->absorption<=materialInfo->tau)
							{
								transmission=true;
							}else{
								//Particule absorbée
								if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
									configurationP.stateParticule=PARTICULE_STATE_ABS_SURF;
								configurationP.energie=0.;
								return;
							}
						}
					}
				}
				if(configurationP.energie<=configurationP.energie_epsilon)
				{
					if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
						configurationP.stateParticule=PARTICULE_STATE_ABS_SURF;
					return;
				}
				//Si Transmission on traverse la paroi
				if(transmission)
				{
					TraverserTetra(configurationP,collisionResolution);
				}else{
					// Choix de la méthode de reflexion en fonction de la valeur de diffusion
					vec3 nouvDirection;
					if(materialInfo->diffusion==1 || GetRandValue()<materialInfo->diffusion)
					{
						vec3 faceDirection;
						if(!doInvertNormal)
							faceDirection=-faceInfo->normal;
						else
							faceDirection=faceInfo->normal;
						nouvDirection=ReflectionLaws::SolveReflection(configurationP.direction,*materialInfo,faceDirection,configurationP);
					}else{
						nouvDirection=ReflectionLaws::SpecularReflection(configurationP.direction,faceInfo->normal);
					}
					//Calcul de la nouvelle direction de réflexion (en reprenant la célérité de propagation du son)
					configurationP.direction=nouvDirection*distanceSurLePas;
					collisionResolution=true;
					SetNextParticleCollision(configurationP);
				}
			}
		}

		if(iteration>1000)
		{
			//Elle est détruite et l'utilisateur en sera informé
			if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
				configurationP.stateParticule=PARTICULE_STATE_LOOP;
			configurationP.energie=0;
			return;
		}
	}
	if(configurationP.tempsEcoule==0.f)
	{   //Aucune collision sur le pas de temps courant
		FreeParticleTranslation(configurationP,configurationP.direction);
	}else{
		//Il y a eu une ou plusieurs collisions sur le pas de temps courant
 		FreeParticleTranslation(configurationP,configurationP.direction*((deltaT-configurationP.tempsEcoule)/deltaT));
		configurationP.tempsEcoule=0; //remise du compteur à 0
	}
}


void CalculationCore::TraverserTetra(CONF_PARTICULE &configurationP, bool& collisionResolution)
{
	// Récuperation de l'information de la face
	t_cFace* faceInfo=NULL;
	faceInfo=configurationP.currentTetra->faces[configurationP.nextModelIntersection.idface].face_scene;
	//test de passage d'un tétraèdre à un autre
	//PASSAGE DE TETRAEDRE

	if(!configurationP.currentTetra->voisins[configurationP.nextModelIntersection.idface])
	{
		#ifdef _DEBUG
		std::cout<<"La particule va sortir du perimètre du volume car une face du domaine est mal orientée ou le maillage est incorrect. La particule a été supprimée";
		#endif
		configurationP.energie=0;
		if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
			configurationP.stateParticule=PARTICULE_STATE_LOST;
		return;
	}else{
		reportTool->ParticuleGoToNextTetrahedra(configurationP,configurationP.currentTetra->voisins[configurationP.nextModelIntersection.idface]);
		t_Tetra* oldTetra=configurationP.currentTetra;
		//Affectation du nouveau volume
		configurationP.currentTetra=configurationP.currentTetra->voisins[configurationP.nextModelIntersection.idface];
		//Calcul de la prochaine collision
		SetNextParticleCollision(configurationP);
		collisionResolution=true; //On refait un test de collision avec les tetrahedres

		//Si la particule passe d'un volume d'encombrement à un autre type de volume (faceInfo appartient à l'ancien volume)
		if(*configurationTool->FastGetConfigValue(Core_Configuration::IPROP_DO_CALC_ENCOMBREMENT) && faceInfo && oldTetra->volumeEncombrement!=configurationP.currentTetra->volumeEncombrement)
			SetNextParticleCollisionWithObstructionElement(configurationP);
	}
}

void CalculationCore::OnChangeCelerite(CONF_PARTICULE &configurationP, t_Tetra* tetra2)
{
	double c1=configurationP.direction.length();
	double c2=configurationTool->GetNormVecPart(configurationP.position,tetra2);
	double xy_length=vec3(configurationP.direction.x,configurationP.direction.y,0).length();
	double cos_gamma1=xy_length/c1;
	double cos_phi=configurationP.direction.x/xy_length;
	double sin_phi=configurationP.direction.y/xy_length;
	double cos_gamma2=(cos_gamma1*c2)/c1;

	short sgn=INTSIGN(configurationP.direction.z);
	if(cos_gamma2>1)
	{
		cos_gamma2=1-EPSILON;
		sgn=-sgn;
	}
	double sin_gamma2=sqrt(1-pow(cos_gamma2,2));

	configurationP.direction.x=c2*cos_gamma2*cos_phi;
	configurationP.direction.y=c2*cos_gamma2*sin_phi;
	configurationP.direction.z=sgn*c2*sin_gamma2;
}


entier_court  CalculationCore::GetTetraFaceCollision(CONF_PARTICULE &configurationP, vec3 &translationVector, float &t)
{
	vec3 dir=translationVector;
	float* posPart=configurationP.position;
	float* dirPart=dir;
	float u,v;
	vec3* scenenodes=sceneTetraMesh->nodes;
	ivec3 tmpsmt;
	ivec3& sommetsFace=tmpsmt;
	if(configurationP.currentTetra)
	{

		//////////////
		// Test de passage a un autre tétraèdre
		TetraFaceTest(0);
		TetraFaceTest(1);
		TetraFaceTest(2);
		TetraFaceTest(3);

		//Dû à une marge d'erreur aucune collision sur les faces du tétrahèdre n'a pu être trouvé
		// Cette marge d'erreur dépend du maillage du volumes et de la position d'origine des particules

		//configurationP.outputToParticleFile=true; //debug
		//On recherche parmis les tétraèdres voisins(ceux où la particule est également sur la surface du tétraèdre), lequel est capable de positionner la prochaine face
		//de collision.
		//Si il est toujours impossible de positionner la particule, celle-ci sera détruite
		t_Tetra* old_tetra=configurationP.currentTetra;
		for(unsigned short idtet=0;idtet<4;idtet++)
		{
			if(old_tetra->voisins[idtet]!=NULL)
			{
				configurationP.currentTetra=old_tetra->voisins[idtet];
				if(core_mathlib::DotInTetra(configurationP.position,this->sceneTetraMesh->nodes[configurationP.currentTetra->sommets.a],
					this->sceneTetraMesh->nodes[configurationP.currentTetra->sommets.b],
					this->sceneTetraMesh->nodes[configurationP.currentTetra->sommets.c],
					this->sceneTetraMesh->nodes[configurationP.currentTetra->sommets.d]))
				{
					TetraFaceTest(0);
					TetraFaceTest(1);
					TetraFaceTest(2);
					TetraFaceTest(3);
				}
			}
		}
		configurationP.energie=0;
		if(configurationP.stateParticule==PARTICULE_STATE_ALIVE)
			configurationP.stateParticule=PARTICULE_STATE_LOST;
	}
	return -1;
}

#ifndef UTILISER_MAILLAGE_OPTIMISATION
void CalculationCore::SetNextParticleCollision(CONF_PARTICULE &configurationP)
{
	INTERSECTION_INFO collisionInfos;
	float minDist=9999999999.f;
	for(uentier cface=0;cface<sceneMesh->pface_size;cface++)
	{
		float faceDist;
		if(CollisionTest(configurationP,cface,collisionInfos,faceDist))
		{
			if(faceDist<minDist)
			{
				float dotVal=configurationP.direction.dot(sceneMesh->pfaces[collisionInfos.idface].normal);
				if(dotVal>0) //si la face est orienté vers nous
				{
					configurationP.nextModelIntersection=collisionInfos;
					minDist=faceDist;
				}
			}
		}
	}
}

void CalculationCore::FreeParticleTranslation(CONF_PARTICULE &configurationP,const vec3 &translationVector)
{
	if(configurationP.currentTetra)
	{
		bool changeTetrahedra=true;
		float tMax=-1;
		while(changeTetrahedra) //On navigue de tetraedre a tetraedre jusqu'a ce que le point de collision soit plus loin que le vecteur de translation
		{
			changeTetrahedra=false;
			float t;
			entier_court idfacemaxdist=GetTetraFaceCollision(configurationP,translationVector,t);
			if(idfacemaxdist>=0 && t<1 && t>=-EPSILON)
			{
				if(configurationP.currentTetra->voisins[idfacemaxdist] && tMax<t) //
				{
					reportTool->ParticuleGoToNextTetrahedra(configurationP,configurationP.currentTetra->voisins[idfacemaxdist]);
					configurationP.currentTetra=configurationP.currentTetra->voisins[idfacemaxdist];
					changeTetrahedra=true;
					tMax=t;
				}
			}
		}
	}
	if(configurationP.currentTetra->linkedRecepteurP)
		reportTool->ParticuleFreeTranslation(configurationP,configurationP.position+translationVector);

	//////////////
	// Translation de la particule
	configurationP.position+=translationVector;
}

bool CalculationCore::CollisionTest(CONF_PARTICULE &configurationP,uentier &faceIndex,INTERSECTION_INFO &infoIntersection, float &factDistance)
{
	using namespace std;

	float t,u,v;

	vec3 *vert0=&sceneMesh->pvertices[sceneMesh->pfaces[faceIndex].sommets.a];
	vec3 *vert1=&sceneMesh->pvertices[sceneMesh->pfaces[faceIndex].sommets.b];
	vec3 *vert2=&sceneMesh->pvertices[sceneMesh->pfaces[faceIndex].sommets.c];


	if(collision_manager::intersect_triangle(configurationP.position,configurationP.direction,
		*vert0,
		*vert1,
		*vert2,
		&t,&u,&v)==1)
	{
		if(t>=-BARELY_EPSILON) //t correspond au nombre de pas de temps pour atteindre la collision
		{
			factDistance=t;
			infoIntersection.collisionPosition=configurationP.position+configurationP.direction*t;
			infoIntersection.idface=faceIndex;
			infoIntersection.percCollision=t;
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

#else
void CalculationCore::SetNextParticleCollision(CONF_PARTICULE &configurationP)
{
	INTERSECTION_INFO collisionInfos;
	float t;
	configurationP.nextModelIntersection.idface=GetTetraFaceCollision(configurationP,configurationP.direction,t);

	configurationP.nextModelIntersection.collisionPosition=configurationP.position+configurationP.direction*t;
}
void CalculationCore::FreeParticleTranslation(CONF_PARTICULE &configurationP,const vec3 &translationVector)
{
	reportTool->ParticuleFreeTranslation(configurationP,configurationP.position+translationVector);
	// On prend en compte le rapprochement vers l'encombrement virtuel
	if(configurationP.currentTetra->volumeEncombrement)
		configurationP.distanceToNextEncombrementEle-=translationVector.length();
	configurationP.position+=translationVector;
}
#endif

