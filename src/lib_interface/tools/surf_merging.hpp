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
#include <vector>
#include <list>

/**
 * @file surf_merging.hpp
 * Permet de regrouper les faces selon la même norme et le même matériau
 */
/**
 * Permet de regrouper les faces selon la même norme et le même matériau
 *
 */
namespace surf_merge
{
/**
 * @class SurfaceMerging
 * Regrouper les faces selon la même norme et le même matériau
 *
 * Permet de regrouper les faces selon la même norme et le même matériau
 *
 */
	class SurfaceMerging
	{
	private:
		struct SelfData_t;
		SelfData_t* self;
	public:
		SurfaceMerging();
		virtual ~SurfaceMerging();
		/**
		 * Lectures des structures de données à la recherche des faces coplanaires
		 * @param model Modèle de la scène
		 */
		void LoadGroups(const formatCoreBIN::ioModel& model);
		/**
		 * Retourne le nombre de groupes de surfaces détectés
		 */
		int GetGroups();
		/**
		 * Donne les données pour un groupe
		 * @param[in] idel Indice du groupe [0-GetGroups()[
		 * @param[out] faceIds Faces du groupe
		 * @python Return faceIds vector, use list(SurfaceMerging::GetGroupInformation(idel)) python built-in function to translate into an integer list.
		 */
		void GetGroupInformation(const unsigned int& idel,std::vector<std::size_t>& faceIds);

		/**
		 * Sauvegarde les triangles fusionnés dans le format de fichier .poly
		 * @param strFileName Nom et chemin du fichier de sortie
		 */
		void SavePolyToPolyFile(const char *strFileName);
	};
};
