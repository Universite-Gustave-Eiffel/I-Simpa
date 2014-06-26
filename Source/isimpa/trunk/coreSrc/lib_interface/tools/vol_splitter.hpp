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

#include "../input_output/bin.h"
#include "../input_output/importExportMaillage/mbin.h"
#include <list>
#include <vector>

/**
 * @file vol_splitter.hpp
 * Permet de regrouper les faces par rapport aux volumes auxquels ils appartiennent
 */
namespace octreeTool{
	class Octree44;
};
namespace volumes_splitter
{
/**
 * @class VolumesSplitter
 * Séparation d'un modèle en volumes
 *
 * Cette classe permet de regrouper les faces par rapport aux volumes auxquels ils appartiennent
 *
 */
	class VolumesSplitter
	{
	private:
		struct vol_t
		{
			vol_t()
				:volId(0),volume(0.)
			{

			}
			int volId;
			double volume;
			std::list<std::size_t> faceIds;
		};
		std::list<vol_t> vols;
		struct facemodel_t
		{
			facemodel_t():isInternalFace(false),volid_norm(-1),volid_invnorm(-1),processed(false){}
			bool isInternalFace;
			bool processed;
			int volid_norm;
			int volid_invnorm;
		};
		std::vector<facemodel_t> faces; //faces du modèle
		octreeTool::Octree44* tetraOctree;
		void FindOrCreateVol(const int& idvol,int& lastvolid_norm,vol_t** pt_vol);
		void LoadOctree(const formatMBIN::trimeshmodel& tetramodel);
	public:
		VolumesSplitter();
		~VolumesSplitter();
		/**
		 * Lectures des structures de données à la recherche des volumes
		 * @param model Modèle de la scène
		 * @param tetramodel Maillage de la scène
		 */
		void LoadDomain(const formatCoreBIN::ioModel& model,const formatMBIN::trimeshmodel& tetramodel);
		/**
		 * Retourne le nombre de volumes détectés
		 * @python Read through len(VolumesSplitterInstance)
		 */
		int GetVolumes(){return vols.size();}
		/**
		 * Donne les données pour un volume
		 * @param[in] idel Indice du volume [0-GetVolumes()[
		 * @param[out] volumeXmlId Indice Xml du volume
		 * @param[out] volume Volume (m3)
		 * @param[out] faceIds Faces propre au volume
		 * @param[out] volNeigh Volumes voisins du volume idel
		 * @python Read through VolumesSplitterInstance[volid] and return a tuple containing (volumeXmlId,volume,faceIds,volNeigh)
		 */
		void GetVolumeInformation(const int& idel,int& volumeXmlId,double& volume,std::vector<std::size_t>& faceIds,std::vector<std::size_t>& volNeigh);
		/**
		 * Donne l'Indice Xml du volume
		 * @param[in] idel Indice du volume [0-GetVolumes()[
		 * @return Indice Xml du volume
		 */
		int GetVolumeXmlId(const int& idel);
		/**
		 * Retourne la liste des limites internes (faces ayant un volume de chaque coté
		 * @param[out] internalFaceIds Faces propre au volume
		 * @param[int] volumeXmlIdFilter Limiter les faces internes appartenant à ce volume (-1 aucun filtre)
		 * @python Return internalFaceIds
		 */
		void GetInternalFaces(std::vector<std::size_t>& internalFaceIds,const int& volumeXmlIdFilter=-1);

		/**
		 * \~english Find the tetrahedron that contains the point specified at parameters
		 * \warning You dont have to call LoadDomain before using this function but you must always use the same tetramodel with the same VolumesSplitter instance.
		 * @param tetramodel Scene tetra mesh.
		 * \param x Position in local coordinate system
		 * \param y Position in local coordinate system
		 * \param z Position in local coordinate system
		 * \param reloadTetraModel Reload the position test array from the first parameter.Set it to true if you have modified the first parameter between two call of GetTetraByPosition.
		 * \return The tetrahedron index if found or -1 if not found.
		 * \~french Cherche la tétraèdre contenant le point indiqué en paramètre.
		 * \warning Il n'est pas nécessaire d'appeler la méthode LoadDomain avant d'utiliser cette fonction
		 * @param tetramodel Maillage de la scène
		 * \param x Position
		 * \param y Position
		 * \param z Position
		 * \param reloadTetraModel Recharge le premier paramètre dans la table de test de position. Mettez le à vrai si vous avez modifié la structure tetramodel entre deux appel de GetTetraByPosition.
		 * \return L'indice du tetraèdre ou -1 si non trouvé.
		 */
		int GetTetraByPosition(const formatMBIN::trimeshmodel& tetramodel, const float& x,const float& y,const float& z,const bool& reloadTetraModel=false);
	};
};
