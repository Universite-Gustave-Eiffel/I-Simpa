#include "coreTypes.h"
#include "sppsTypes.h"


/**
 * @brief Cette classe regroupe les méthodes de réfléxion de particules sur les surfaces
 *
 * Les vecteurs de réflexion retournés sont normalisés car une perte de précision se fait ressentir quand à la norme de ces vecteurs
 */
class ReflectionLaws
{
public:
	/**
	 * En fonction du matériau appel la méthode de réflexion associé. Si la méthode est inconnue la réflexion est spéculaire.
	 * @param vectorDirection Vecteur incident
	 * @param materialInfo Matériau de la face
	 * @param faceInfo Informations de la face en collision
	 * @param AB Vecteur perpendiculaire à la normal de la face
	 * @return Vecteur réflexion normalisé
	 */
	static vec3 SolveReflection(vec3 &vectorDirection,t_Material_BFreq &materialInfo,vec3& faceNormal)
	{
		switch(materialInfo.reflectionLaw)
		{
			case REFLECTION_LAW_SPECULAR:
				return SpecularReflection(vectorDirection,faceNormal);
				break;
			case REFLECTION_LAW_LAMBERT:
				return BaseWnReflection(vectorDirection,faceNormal,1);
				break;
			case REFLECTION_LAW_UNIFORM:
				return BaseWnReflection(vectorDirection,faceNormal,0);
				break;
			case REFLECTION_LAW_W2:
				return BaseWnReflection(vectorDirection,faceNormal,2);
				break;
			case REFLECTION_LAW_W3:
				return BaseWnReflection(vectorDirection,faceNormal,3);
				break;
			case REFLECTION_LAW_W4:
				return BaseWnReflection(vectorDirection,faceNormal,4);
				break;
			default:
				return SpecularReflection(vectorDirection,faceNormal);
		};
	}
	/**
	 * Reflection spéculaire
	 * @param vecteurVitesse Vecteur de translation de la particule
	 * @param Normal de la face.
	 * @return Vecteur vitesse réfléchie normalisé
	 */
	static vec3 SpecularReflection(vec3 &vecteurVitesse,vec3 &faceNormal)
	{
		vec3 retVal=( vecteurVitesse	-(faceNormal*(vecteurVitesse.dot(faceNormal))*2));
		return retVal/retVal.length();
	}

	/**
	 * Calcul de base pour lambert et wn
	 * @param vecteurVitesse Vecteur de translation de la particule
	 * @param Normal de la face.
	 * @param expo Exposant de phi
	 * @return Vecteur vitesse réfléchie normalisé
	 */
	static vec3 BaseWnReflection(vec3 &vecteurVitesse,vec3 &faceNormal,decimal expo)
	{
		decimal theta=GetRandValue() * M_2PI;
		decimal phi=acos(pow((float)1-GetRandValue(),(float)(1./(expo+1.))));//pow((float)acos(1-GetRandValue()),(float)(1./(expo+1.)));
		return BaseUniformReflection(vecteurVitesse,faceNormal,theta,phi);
	}

	/**
	 * Calcul experimental de reflection dans un mileu encombré
	 */

	static vec3 FittingUniformReflection(vec3 &vecteurVitesse)
	{
		decimal theta=GetRandValue() * M_2PI;
		decimal phi=acos((float)1-sqrtf(4*GetRandValue()));
		return BaseUniformReflection(vecteurVitesse,vecteurVitesse/vecteurVitesse.length(),theta,phi);
	}


	/**
	 * Calcul experimental de reflection dans un mileu encombré
	 */
	static vec3 FittingLambertReflection(vec3 &vecteurVitesse)
	{
		decimal theta=GetRandValue() * M_2PI;
		decimal u;
		decimal phi;
		decimal rejection;
		do{
			phi=GetRandValue() * M_PI;
			u=GetRandValue();
			//rejection=((2.f*(sinf(phi)-phi*cosf(phi)))/(12*M_PI));
			rejection=((sinf(phi)-phi*cosf(phi))/(4.f));
		}while(u>rejection);
		return BaseUniformReflection(vecteurVitesse,vecteurVitesse/vecteurVitesse.length(),theta,phi);
	}
private:


	/**
	 * Calcul de base ayant attrait à la reflection de type uniforme
	 * @param vecteurVitesse Vecteur de translation de la particule
	 * @param Normal de la face.
	 * @param theta angle (radians) de rotation dans le plan de la surface (de 0 à 2*pi)
	 * @param phi  angle (radians) de rotation par rapport à la normale (de 0 à pi/2)
	 * @return Vecteur vitesse réfléchie
	 */
	static vec3 BaseUniformReflection(const vec3& vecteurVitesse,const vec3 &faceNormal, decimal theta,decimal phi)
	{
		vec3 retVal;
		if(vec3(faceNormal.x,faceNormal.y,0).length()>EPSILON)
		{
			retVal.set(faceNormal.y,-faceNormal.x,0);
		}else{
			retVal.set(1,0,0);
		}
		retVal=faceNormal.Rotation(retVal,phi);
		retVal=retVal.Rotation(faceNormal,theta);
		return retVal / retVal.length();
	}
};