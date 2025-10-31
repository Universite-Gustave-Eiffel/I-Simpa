/* ----------------------------------------------------------------------
* I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
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
* For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or
* send an email to contact@noise-planet.org
*
* ----------------------------------------------------------------------*/

#include "spatial_discretization.hpp"
#include <vector>
#include <string>

#ifndef __SCALARFIELDBUILDERS__
#define __SCALARFIELDBUILDERS__

namespace ScalarFieldBuilders
{
	/**
	 * Retourne les coordonn�es du centre du cube correspondant � l'indice en param�tre
	 */
	vec3 CellIdToCenterCoordinate( const ivec3& cell_id, const decimal& cellSize, const vec3& zeroCellCenter);
	/**
	 * Cette classe permet de g�n�rer un espace discr�tis� en plusieurs volumes. C'est la premi�re �tape de la reconstruction du mod�le.
	 */
	class ScalarFieldCreator
	{
	protected:
		PTR<SpatialDiscretization::weight_matrix> fieldData; //Donn�es de la matrice X,Y,Z


		struct mainVolumeConstruction_t
		{
			decimal cellSize;
			unsigned int cellCount;
			vec3 mainVolumeCenter;
			vec3 cellHalfSize;
			vec3 zeroCellCenter;
			SpatialDiscretization::weight_t volumeCount;
			std::vector<decimal> volumeValue;
			vec3 boxMin;
			vec3 boxMax;
		} volumeInfo;
		SpatialDiscretization::domainInformation_t domainInformation;
		decimal resolution;
		static void ComputeMatrixParams(const vec3& boxMin,const vec3& boxMax, const decimal& minResolution, mainVolumeConstruction_t& computedVolumeInfo);
		/**
		 * Initialise les donn�es pour le volume ext�rieur
		 */
		void InitExteriorVolumeId();
		/**
		 * Propage les valeurs des identifiants de volume d'une cellule � l'autre
		 * Si la cellule cible est modifi� cette m�thode retourne vrai
		 */
		bool CellToCellVolumePropagation(const ivec2& destinationPropa,const ivec2& sourcePropa,const SpatialDiscretization::weight_t& volumeId);
		/**
		 * Propage un indice de volume dans toute la matrice
		 */
		void ExtandVolume(const SpatialDiscretization::weight_t& volumeId);
		/**
		 * Retourne la position de la premi�re cellule avec la valeur en param�tre
		 */
		ivec3 GetFirstCellByWeight(const SpatialDiscretization::weight_t& weight,SpatialDiscretization::zcell** foundCell);

		/**
		 * Calcul pour chaque volume sa valeur en m^3
		 * @param[out] volumeValue Un tableau de dimension �gale au nombre de volume dans le domaine. Dont la valeur est en m^3.
		 */
		void ComputeVolumesValue(std::vector<decimal>& volumeValue);

		/**
		 * Retourne l'indice de la cellule contenant le point pass� en param�tre
		 */
		ivec3 GetCellIdByCoord(const vec3& position);
	public:
		/**
		 * Constructeur
		 * @param _resolution Dimension d'une cellule qui composera la matrice. Plus la r�solution est �lev�e plus le model g�n�r� sera proche du mod�le en entr�e et plus de triangles seront g�n�r�s.
		 */
		ScalarFieldCreator(const decimal& _resolution);
		/**
		 * Initialisation de la matrice selon la r�solution et la boite englobante pass� en param�tre.
		 * @param boxMin Coordonn�es minimale des objets qui alimenteront la matrice
		 * @param boxMax Coordonn�es maximale des objets qui alimenteront la matrice
		 */
		void FirstStep_Params(const vec3& boxMin,const vec3& boxMax);
		virtual ~ScalarFieldCreator();

		/**
		 * Une fois toutes les primitives renseign�es. Cette m�thode doit �tre appel�e afin de d�tecter les volumes d�limit� par les limites.
		 */
		void ThirdStep_VolumesCreator();

		/**
		 * Retourne la valeur de la matrice selon les indices des cellules
		 * @param index Entier positif d�signant le n� de cellule.
		 * @see GetDomainSize()
		 */
		SpatialDiscretization::weight_t GetMatrixValue(const ivec3& index);


		vec3 GetCenterCellCoordinates( const ivec3& cell_id) const;


		/**
		 * Pour toutes les valeurs de Z pour un x,y donn�. Retourne la valeur min,max.
		 */
		void GetMinMaxOnZ( const ivec2& xyCell, SpatialDiscretization::weight_t& minVolId,SpatialDiscretization::weight_t& maxVolId);
		bool IsContainsVol( const ivec2& xyCell, SpatialDiscretization::weight_t& volId);

		SpatialDiscretization::weight_t GetVolumeCount();
		decimal GetVolumeValue(const SpatialDiscretization::weight_t& volId);

		void GetMinMax(vec3& minBox,vec3& maxBox);

		/**
		 * Exporte les indices et volumes des domaines
		 * @param fileName Nom et chemin du fichier de sortie
		 * @param volsLabelsFileName Nom et chemin du fichier d'entr�e des noms utilisateur de volume (fichier texte avec "x y z NomDuVolume" � chaque ligne)
		 */
		void ExportVolsStats(const std::string& fileName, const std::string& volsLabelsFileName=std::string());
		///////////////////////////////
		// Debug Functions
		unsigned int count();
		void MakeXYZ(const std::string& filename,const SpatialDiscretization::weight_t& idVol);
		void MakeXYZ(const std::string& filename,const decimal& minVol);
		std::size_t GetDomainSize();


	};

}

#endif
