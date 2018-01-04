#include <coreTypes.h>

#ifndef __TC_TYPES__
#define __TC_TYPES__

#define NB_THEORIES 2
#define NB_COLS_BY_THEORIE 3

/**
 * Enumération des théories implémentées
 */
enum MODE_CALCUL
{
	MODE_CALCUL_SABINE,
	MODE_CALCUL_EYRING
};
/**
 * Structure de données des propriétés de calcul pour une fréquence et une théorie
 */
struct  t_calculation_property
{
	decimal AireAbsorptionEquivalente;
	decimal TR;
	decimal NiveauSonoreChampReverbereLineaire;
	decimal NiveauSonoreChampReverbere;
};

/**
 * Structure de données des propriétés de calcul pour une théorie
 */
struct  t_calculation_mode
{	
	t_calculation_property modeCalcul[NB_THEORIES];
};

/**
 * Classe contenant les données des propriétés de calcul général
 */
class t_mainData
{
public:
	t_calculation_mode* frequencyDependentValues;
	entier nbFreq;
	decimal sceneVolume;
	t_mainData(int _nbFreq)
		:nbFreq(_nbFreq)
	{
		frequencyDependentValues=new t_calculation_mode[nbFreq];
	}
	~t_mainData()
	{
		delete[] frequencyDependentValues;
	}

};

/**
 * Structure de données désignant l'environnement de calcul en cours.
 */
struct t_calculationParameters
{
	int id_freq;
	MODE_CALCUL typeCalcul;	
};

/**
 * Structure de données des valeurs de champ total propre à une théorie de calcul
 */
struct t_rp_calculation_property
{
	decimal ChampTotal;
};

/**
 * Structure de données propre à une fréquence pour un récepteur ponctuel
 */
struct t_rp_freq
{
	t_rp_calculation_property tabDataByTheorie[NB_THEORIES];
	decimal ChampDirect;
	decimal ChampDirectLineaire;
};


/**
 * Structure de données propre à un récepteur ponctuel
 */
class t_TC_RecepteurPonctuel
{
public:
	t_Recepteur_P* linkedRecepteurP;
	t_rp_freq* tabDataByFreq;
	entier nbFreq;
	t_TC_RecepteurPonctuel()
	{
		linkedRecepteurP=NULL;
		tabDataByFreq=NULL;
		nbFreq=0;
	}
	void init(int _nbFreq)
	{
		nbFreq=_nbFreq;
		linkedRecepteurP=NULL;
		delete[] tabDataByFreq;
		tabDataByFreq=new t_rp_freq[nbFreq];
	}
	~t_TC_RecepteurPonctuel()
	{
		delete[] tabDataByFreq;
	}
};

/*
class t_rs_calculation_property
{
public:
	decimal** ChampTotal;
	int nbFace;
	t_rs_calculation_property( int _nbFace)
		:nbFace(_nbFace)
	{
		ChampTotal = new decimal*[_nbFace];
		memset(ChampTotal,0,sizeof(decimal*)*_nbFace);
	}
	~t_rs_calculation_property()
	{
		delete[] ChampTotal;
	}
};

struct t_rs_freq
{
	 tabDataByTheorie[NB_THEORIES];
	decimal ChampDirect;
};


struct t_RecepteurSurfacique
{
	t_rp_freq tabDataByFreq[NB_FREQ];
};
*/

#endif