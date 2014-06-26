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

#include "Core/mathlib.h"
#include "coreString.h"
#include <vector>

#ifndef CORE_TYPES
#define CORE_TYPES
/**
 * @file coreTypes.h
 * Structures décrivant le domaine de propagation.
 */

const double _REF_W0_=pow(10.,-12.);


/**
 * @brief Structure de données correspondant aux informations d'absorption atmosphérique
 */
#define t_AbsAtmo decimal /*!< Absorption totale en dB/m  */


/**
 * @brief Structure de données correspondant aux informations d'une frequence de la simulation
 */
struct t_FreqUsage
{
	bool doCalculation;								/*!<  Vrai si cette bande de fréquence doit être traité dans les calculs */
	uentier freqIndex;								/*!<  Indice de la frequence dans la liste des fréquence de l'environnement */
	t_freq freqValue;								/*!<  Frequence associée (Hz) */
	t_AbsAtmo absorption_atmospherique;				/*!<  Information d'absorption atmospherique */
	decimal densite_proba_absorption_atmospherique; /*!< Probabilité d'absorption atmospherique sur un pas de temps \f$f(d_0)=\exp(m\times c\Delta t)\f$*/
};


/**
 * @brief Cette structure représente la bande de fréquence d'une source
 */
struct t_Source_Freq
{
	decimal db;		/*!<  Puissance de la source sur cette bande de frequence */
	decimal w_j;	/*!< Puissance watt de la source j en linéaire */
};

/**
 * @brief Cette structure représente la bande de fréquence d'un récepteur ponctuel
 */
struct t_Bruit_Freq
{
	decimal db;		/*!< Puissance du bruit sur cette bande de fréquence*/
};

/**
 * Enumération des lois de réfléction
 */
enum REFLECTION_LAW
{
	REFLECTION_LAW_SPECULAR,	/*!<  Loi de reflexion spéculaire */
	REFLECTION_LAW_UNIFORM,		/*!<  Loi de reflexion uniforme (w1) */
	REFLECTION_LAW_LAMBERT,		/*!<  Loi de reflexion de Lambert */
	REFLECTION_LAW_W2,			/*!<  Loi de reflexion w2 */
	REFLECTION_LAW_W3,			/*!<  Loi de reflexion w3 */
	REFLECTION_LAW_W4,			/*!<  Loi de reflexion w4 */
	REFLECTION_LAW_SEMI_DIFFUS	/*!<  Loi de reflexion semis diffus */
};
/**
 * Enumération des types des sources
 */
enum SOURCE_TYPE
{
	SOURCE_TYPE_OMNIDIRECTION,
	SOURCE_TYPE_UNIDIRECTION,
	SOURCE_TYPE_XY,
	SOURCE_TYPE_YZ,
	SOURCE_TYPE_XZ
};
/**
 * Enumération des lois de diffusion dans un encombrement
 */
enum DIFFUSION_LAW
{
	DIFFUSION_LAW_UNIFORM,				/*!<  Loi de diffusion uniforme  */
	DIFFUSION_LAW_REFLEXION_UNIFORM,	/*!<  Loi de reflection uniforme*/
	DIFFUSION_LAW_REFLEXION_LAMBERT		/*!<  Loi de reflection lambert   */
};

/**
 * @brief Structure de données correspondant aux informations d'un materiau pour une bande de fréquence
 */
struct t_Material_BFreq
{
	REFLECTION_LAW reflectionLaw;			/*!<  Loi de reflexion associé à la surface */
	decimal affaiblissement;				/*!<  Taux d'affaiblissement de la surface */
	bool dotransmission;					/*!<  Effectuer ou non la transimission avec ce matériau */
	decimal tau;							/*!<  Tau calculé à partir du taux d'affaiblissement de la surface \f$\tau=10^{R_a/10}\f$ */
	decimal absorption;						/*!<  Taux d'absorption de la surface */
	decimal diffusion;						/*!<  Taux de diffusion de la surface */
};

/**
 * @brief Structure de données correspondant à un matériau
 */
struct t_Material
{
	t_Material_BFreq* matSpectrumProperty; /*!< Tableau C contenant les propriétés d'un matériau */
	uentier outsideMaterialIndice;		   /*!< Indice associé au matériau XML */
	bool doubleSidedMaterialEffect;		   /*!< Prise en compte du matériau de la surface des deux cotés de la propagation */
	~t_Material(){ if(matSpectrumProperty) delete[] matSpectrumProperty; }
	t_Material(){ matSpectrumProperty=NULL;doubleSidedMaterialEffect=true; }
};


/**
 * Structure de donnée désignant une fraction du recepteur surfacique (une zone du recepteur), sur une bande de fréquence
 */
class r_Surf_Face
{
public:
	ivec3 indiceSommets;  /*!< Indice des sommets de la face */
	vec3 sommets[3];
	decimal** energieRecu; /*!< Tableau de nombre decimal désignant l'énérgie recu sur le nombre de pas de temps */
	/**
	 *  Appelé par r_Surf::r_Surf(). Initilialise le tableau général
	 */
	void InitTabFreq(uentier _nbfreq);
	/**
	 *  Avant de renseigner une bande de fréquence appeler cette méthode pour allouer l'espace mémoire
	 */
	void InitFreq(uentier idfreq, uentier _nbtimestep);
	~r_Surf_Face();
	r_Surf_Face();
	void operator=(const r_Surf_Face& copyFrom);
	uentier nbfreq;
	uentier nbtimestep;
private:
	void deleteData();
};
/**
 * @brief Structure de stockage d'un récepteur surfaciques
 */
class r_Surf
{
public:
	CoreString name;	/*!< Nom du récepteur surfacique */
	entier idRecepteurS; /*!< Indice du recepteur surfacique (externe) */
	uentier nbFaces;
	r_Surf_Face* faces; /*!< La taille du tableau correspond au nombre de sous-élément composant le recepteur surfacique*/
	void InitFaces(uentier _nbFaces, uentier _nbfreq);
	r_Surf();
	r_Surf(r_Surf& copyFrom);
	~r_Surf();
};

/*
template<class t_el>
class minimal_array{
public:
	minimal_array()
		:data(NULL)
	{

	}
	minimal_array(const minimal_array<t_el>& cpyFrom)
		:data(NULL)
	{
		Init(cpyFrom.size());
		for(unsigned int icell=0;icell<this->size_tab;icell++)
			(*this)[icell]=t_el(cpyFrom[icell]);
	}
	~minimal_array()
	{
		this->Init(0);
	}
	unsigned int size() const { return this->size_tab; }
	void Init(const uentier& _size)
	{
		delete[] data;
		data=NULL;
		size_tab=_size;
		if(_size>0)
			data=new t_el[_size];
	}
	void Init(const uentier& _size,const t_el& defaultValue)
	{
		delete[] data;
		data=NULL;
		size_tab=_size;
		if(_size>0)
		{
			data=new t_el[_size];
			for(unsigned int icell=0;icell<this->size_tab;icell++)
				(*this)[icell]=defaultValue;
		}
	}
	bool empty()
	{
		return !data;
	}
	t_el &operator[](const uentier& _i)
	{
		#ifdef _DEBUG
		if(!data)
			throw("minimal_array uninitialised");
		if(!(_i<size_tab))
			throw("minimal_array [] access out range.");
		#endif
		return data[_i];
	}
	const t_el& operator[](const uentier& _i) const
	{
		#ifdef _DEBUG
		if(!data)
			throw("minimal_array uninitialised");
		if(!(_i<size_tab))
			throw("minimal_array [] access out range.");
		#endif
		return data[_i];
	}
private:
	t_el* data;
	unsigned int size_tab;
};
*/



typedef decimal t_cell;
typedef std::vector<t_cell> t_time_ar;
typedef std::vector<t_time_ar> t_col_ar;
typedef std::vector<t_col_ar> t_row_ar;
typedef std::vector<t_row_ar> t_freq_ar;
/**
 * @brief Structure de stockage d'un récepteur surfaciques en coupe
 */
struct r_SurfCut
{
	CoreString name;	/*!< Nom du récepteur surfacique */
	entier idRecepteurS; /*!< Indice du recepteur surfacique (xml) */
	entier NbCellU;	/*!< Nombre de cellules sur le segment BC */
	entier NbCellV; /*!< Nombre de cellules sur le segment BA */
	decimal UCellSize; /*!< Taille du coté u des cellules (m)*/
	decimal VCellSize; /*!< Taille du coté v des cellules (m)*/
	vec3 Avert;	/*!< Sommet A*/
	vec3 Bvert;	/*!< Sommet B*/
	vec3 Cvert;	/*!< Sommet C*/
	vec3 planeNormal; /*!< Normal du plan*/
	uentier nbfreq; /*!< Nombre de bande de fréquence stockées*/
	uentier nbrows; /*!< Nombre de cellules U stockés */
	uentier nbcols; /*!< Nombre de cellules V stockés */
	uentier nbtimestep;
	//[t_freq_ar][t_row_ar][t_col_ar][t_time_ar]
	void Init(const uentier& _nbfreq, std::vector<bool>& _frequsage,const uentier& _nbrows,const uentier& _nbcols, const uentier& _nbtimestep)
	{
		nbfreq=_nbfreq;
		nbrows=_nbrows;
		nbcols=_nbcols;
		nbtimestep=_nbtimestep;
		data.resize(_nbfreq);
		for(uentier idfreq=0;idfreq<_nbfreq;idfreq++)
		{
			if(_frequsage[idfreq])
			{
				t_row_ar& row_ar(data[idfreq]);
				row_ar.resize(_nbrows);
				for(uentier idrow=0;idrow<_nbrows;idrow++)
				{
					t_col_ar& col_ar(row_ar[idrow]);
					col_ar.resize(_nbcols);
					for(uentier idcol=0;idcol<_nbcols;idcol++)
					{
						col_ar[idcol].resize(_nbtimestep,0);
						//memset(col_ar[idcol].data,0,_nbtimestep*sizeof(t_cell));
					}
				}
			}
		}
	}
	t_freq_ar data;
	r_SurfCut()
		:
	NbCellU(0),
	NbCellV(0),
	UCellSize(0.f),
	VCellSize(0.f),
	nbfreq(0),
	nbrows(0),
	nbcols(0),
	nbtimestep(0)
	{
	}
};
/**
 * @brief Structure de stockage d'une bande de fréquence d'un encombrement
 */
struct t_Encombrement_BFreq
{
	decimal alpha; /*!< Coefficient d'absorption des objets diffusants */
	decimal lambda; /*!< Libre parcours moyen (distance moyenne entre deux objets) */
	DIFFUSION_LAW law_diffusion; /*!< Loi de diffusion au sein de l'encombrement */

};
/**
 * @brief Structure de stockage d'un encombrement
 */
struct t_Encombrement
{
	entier idEncombrement; /*!< Indice de l'encombrement (externe) */
	t_Encombrement_BFreq* encSpectrumProperty;
	t_Encombrement(entier nbFreq){encSpectrumProperty = new t_Encombrement_BFreq[nbFreq];}
	~t_Encombrement(){delete[] encSpectrumProperty;}
};



/**
 * @brief Structure d'une face dans le coeur de calcul
 */
struct t_cFace
{
	ivec3 sommets;							/*!< Indice des sommets */
	vec3 normal;							/*!< Vecteur de normal */
	t_Material* faceMaterial;				/*!< Pointeur vers le matériau associé (NULL si aucun) */
	t_Encombrement* faceEncombrement;		/*!< Pointeur vers l'encombrement associé (NULL si aucun) */
	entier idencombrement;					/*!< Indice de l'encombrement (-1 si aucun,indice externe )*/
	entier idRecepteurS;					/*!< Indice du recepteur surfacique (indice externe)*/
	r_Surf_Face* recepteurS;				/*!<  Pointeur vers la face du recepteur surfacique correspondant */
	t_cFace(){ faceMaterial=NULL;
	recepteurS=NULL;

	}
};







/**
 * Modèle de la scène
 */
struct t_Mesh
{
	vec3* pvertices;
	t_cFace* pfaces;
	uentier pvert_size;
	uentier pface_size;

	t_Mesh()
	{
		pvertices=NULL;pfaces=NULL;pvert_size=0;pface_size=0;
	}
	~t_Mesh()
	{
		delete[] pvertices;
		delete[] pfaces;
	}
};
/**
 * @brief Structure d'une face des tetraèdres
 * @see t_Tetra
 */
struct t_Tetra_Faces
{
	t_cFace* face_scene;					/*!< Pointeur vers la face de la scène en contact NULL si aucun */
	ivec3 indiceSommets;					/*!< Indices des 3 sommets du triangles */
	vec3 normal;							/*!< Normal de la face (orianté vers l'exterieur) */

	r_Surf_Face* recepteurS;						/*!< Pointeur vers la face du recepteur surfacique correspondant */

	t_Tetra_Faces(){
		face_scene=NULL;
		recepteurS=NULL;
	};
};


/**
 * @brief Données d'un recepteur ponctuel
 * @see t_TetraMesh
 */
class t_Recepteur_P
{
public:
	CoreString lblRp;  /*!< Nom du récepteur ponctuel */
	CoreString pathRp; /*!< Chemin du dossier contenant les fichiers de résultats relatifs à ce récepteur ponctuel @see BaseReportManager::SauveRecepteursPonctuels() */
	uentier index; /*!< Indice xml du recepteur ponctuel */
	uentier idrp; /*!< Indice dans le tableau des récepteurs ponctuels */
	decimal cdt_vol; /*! Facteur utilisé lors de l'enregistrement du fichier. Tel que le niveau sonore dB correspondent à 10log10(energy_sum*cdt_vol*(1/pow((float)(20*pow(10.f,(int)-6)),(int)2))). Valeur initialisé par InitRecepteurPTetraLocalisation à cdt_vol=Celerite*Rho */
	vec3 position; /*! Position du récepteur */
	vec3 orientation; /*!< Orientation du récepteur ponctuel,vecteur normalisé */
	l_decimal** energy_sum; /*!< Tableau (par bande de fréquence) de pointeurs vers le 1er élément d'un tableau (par pas de temps) contenant la puissance recue */
	t_Bruit_Freq* bruit_spectre; /*!< Spectre du bruit du récepteur ponctuel */
	t_Recepteur_P( uentier _nbfreq,	uentier _nbtimestep);
	~t_Recepteur_P();
	void InitFreq(uentier idfreq);
	uentier indexTetra; /*!< Indice du tétrahèdre contenant le recepteur ponctuel*/
private:
	uentier nbfreq;
	uentier nbtimestep;
};

/**
 * @brief Données d'un tetrahèdre
 * @see t_TetraMesh
 */
class t_Tetra
{
public:
	uentier index;	/*!< Indice du tetrahèdre */
	t_Tetra_Faces faces[4];
	t_Tetra* voisins[4]; /*!< Pointeurs vers les voisins du tetrahèdre, NULL si pas de voisin n*/
	std::vector<t_Recepteur_P*>* linkedRecepteurP;
	std::vector<r_SurfCut*>* linkedCutMap; /*!< Pointeur vers le(s) récepteur de coupe */
	t_Encombrement* volumeEncombrement; /*!< Pointeur vers l'encombrement */

	entier idVolume;	/*!< Indice du volume associé à ce tétraèdre 0 volume principal */
	ivec4 sommets;
	decimal z;	/*!< Hauteur du centre du tétraèdre par rapport à la surface la plus proche du tétraèdre */
	vec3 g; /*!< Position du centre du tétraèdre */
	t_Tetra()
		:linkedCutMap(NULL)
	{
		for(int i=0;i<4;i++)
			voisins[i]=NULL;

		linkedRecepteurP=NULL;

		volumeEncombrement=NULL;
		z=-1;
	}
	~t_Tetra(){ delete linkedRecepteurP;  //Supprime le tableau de pointeur
				delete linkedCutMap;
						linkedRecepteurP=NULL;
						}
	void makeTabRecp() { linkedRecepteurP=new std::vector<t_Recepteur_P*>; }
	void pushCutReceiver(r_SurfCut* newCR)
	{
		if(!linkedCutMap)
			linkedCutMap=new std::vector<r_SurfCut*>;
		linkedCutMap->push_back(newCR);
	}
};

/**
 * @brief Modèle du maillage de la scène
 */
class t_TetraMesh
{
public:
	vec3* nodes;
	t_Tetra* tetraedres;
	uentier nodesSize;
	uentier tetraedresSize;
	t_TetraMesh()
	{
		nodes=NULL;tetraedres=NULL;
		nodesSize=0;tetraedresSize=0;
	}
	~t_TetraMesh()
	{
		delete[] nodes;
		delete[] tetraedres;
	}
	//Chargement du fichier de maillage
	bool LoadFile(const char * fileName, t_Mesh &sceneMesh,uentier nbFreq);
};

/**
 * @brief Structure de données correspondant aux informations d'une source
 */
struct t_Source
{
	SOURCE_TYPE type;
	vec3 Direction;
	vec3 Position;
	t_Source_Freq* bandeFreqSource;
	t_Tetra* currentVolume;
	decimal sourceDelay;
	unsigned int idsource; /*!< Indice de la source dans le tableau BaseCoreConfiguration::srcList [0:n-1]*/
	CoreString sourceName; /*!< Libellé de la source */
	~t_Source(){ if(bandeFreqSource) delete[] bandeFreqSource; }
	t_Source(){ bandeFreqSource=NULL;currentVolume=NULL; }
};





/**
 * Méthode de conversion de langue
 */
const char* translate( const char* strToTranslate );

#ifndef _
#define _(s)                     translate(s)
#endif

#endif

