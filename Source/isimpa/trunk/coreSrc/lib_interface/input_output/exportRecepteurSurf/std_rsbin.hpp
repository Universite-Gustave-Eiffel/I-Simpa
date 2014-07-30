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

#include <vector>
#include <list>
#include <string>
/*! \file std_rsbin.hpp
    \brief Import export de l'animation des recepteurs surfaciques
*/

/**
 * \brief Import export de l'animation des recepteurs surfaciques
 */
namespace formatRSBIN
{

struct t_ExchangeData;

class rsurf_io;
typedef float t_pos[3];
typedef std::size_t t_index[3];
struct t_iso_contouring_data;
struct t_curve //std::pair accepté par py++ ???
{
	t_pos A;
	t_pos B;
};
/**
 * @brief Contient les données d'un fichier de résultat de récepteur de surfaces
 *
 * Permet de créer et de lire les données
 */
class rsurf_data
{
public:

	rsurf_data();
	~rsurf_data();

	/**
	 * Affecte directement les données à cette classe à partir de la structure correspondance.
	 * @warning Cette classe supprimera ces données, à moins que NULL soit passé en parémètre avant la suppression de rsurf_data
	 * @python Non disponible en python
	 */
	void SetRsBinData(t_ExchangeData* rsData);
	void SetRsBinData(t_ExchangeData& rsData);
	/**
	 * @return le nombre de récepteurs surfaciques.
	 */
	std::size_t GetRsCount();
	/**
	 * 1ere étape Création, Mise en place des récepteurs et des propriétés
	 * @param[in] tabNodesSize Nombre de sommets de triangles
	 * @param[in] tabRsSize Nombre de récepteurs de surfaces
	 * @param[in] nbtimestep Nombre de pas de temps maximum
	 * @param[in] timestep Pas de temps en secondes
	 * @param[in] record_type Type d'enregistrement. Valeurs possibles "SPL_STANDART" "SPL_GAIN" "TR" "EDT"
	 */
	void Make(const std::size_t& tabNodesSize,const std::size_t& tabRsSize,const int& nbtimestep,const float& timestep,const std::string& record_type="SPL_STANDART");
	/**
	 * 2eme étape Création, Affectation des propriétés d'un récepteur
	 * @param[in] rsIndex Indice du récepteur surfacique
	 * @param[in] nbfacesconst Nombre de faces dans ce récepteur de surfaces.
	 * @param[in] rs_name Nom du récepteur de surface
	 * @param[in] xmlid Indice XML du récepteur de surface
	 */
	void MakeRs(const std::size_t& rsIndex, const std::size_t& nbfacesconst, const std::string& rs_name, const int& xmlid);
	/**
	 * 2eme étape Création, Affectation de la valeur d'un noeud
	 * @param[in] nodeIndex Indice du sommet
	 * @param[in] x Position X (m)
	 * @param[in] y Position Y (m)
	 * @param[in] z Position Z (m)
	 */
	void SetNodeValue(const std::size_t& nodeIndex, const float& x, const float& y, const float& z);
	/**
	 * 3eme étape Création, Affectation des propriétés d'une face
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[in] faceIndex Indice de la face
	 * @param[in] vertexA Indice du sommet
	 * @param[in] vertexB Indice du sommet
	 * @param[in] vertexC Indice du sommet
	 * @param[in] recordCount Nombre d'enregistrements de niveaux (energy!=0)
	 */
	void SetFaceInfo(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& vertexA,const std::size_t& vertexB,const std::size_t& vertexC,const std::size_t& recordCount);
	/**
	 * 4eme étape Création, Affectation de la valeur de pression d'un pas de temps sur une face
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[in] faceIndex Indice de la face
	 * @param[in] recordIndex Indice de l'enregistrement
	 * @param[in] idstep xeme pas de temps ( x=y sec/timestep )
	 * @param[in] energy Niveau de pression (SPL) ou temps (TR EDT)
	 */
	void SetFaceEnergy(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& recordIndex,const std::size_t& idstep,const float& energy);

	/**
	 * Accesseur propriétés du fichier
	 * @param[out] rs_size Nombre de sommets de triangles
	 * @param[out] nodes_size Nombre de récepteurs de surfaces
	 * @param[out] nbtimestep Nombre de pas de temps maximum
	 * @param[out] timestep Pas de temps en secondes
	 * @param[out] record_type Type d'enregistrement. Valeurs possibles "SPL_STANDART" "SPL_GAIN" "TR" "EDT"
	 * @python Return a tuple containing (rs_size,nodes_size,nbtimestep,timestep,recordType)
	 */
	void GetFileInfos(std::size_t& rs_size,std::size_t& nodes_size,std::size_t& nbtimestep,float& timestep,std::string& recordType) const;
	/**
	 * Accesseur valeur d'un noeud
	 * @param[in] nodeIndex Indice du sommet
	 * @param[out] x X
	 * @param[out] y Y
	 * @param[out] z Z
	 * @python Return a tuple containing (x,y,z)
	 */
	void GetNodePositionValue(const std::size_t& nodeIndex, float& x, float& y, float& z) const;
	/**
	 * Accesseur Information sur un récepteur surfacique
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[out] nbfaces Nombre de surfaces
	 * @param[out] rs_name Libellé du récepteur de surface
	 * @param[out] xmlid Indice xml du récepteur
	 * @python Return a tuple containing (nbfaces,rs_name,xmlid)
	 */
	void GetRsInfo(const std::size_t& rsIndex, std::size_t& nbfaces, std::string& rs_name, int& xmlid) const;
	/**
	 * Accesseur Information sur une face
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[in] faceIndex Indice de la surface
	 * @param[out] vertexA Indice du sommet
	 * @param[out] vertexB Indice du sommet
	 * @param[out] vertexC Indice du sommet
	 * @param[out] recordCount Nombre d'enregistrement
	 * @python Return a tuple containing ( vertexA,vertexB,vertexC,recordCount)
	 */
	void GetFaceInfo(const std::size_t& rsIndex,const std::size_t& faceIndex,std::size_t& vertexA,std::size_t& vertexB,std::size_t& vertexC,std::size_t& recordCount) const;
	/**
	 * Calcul l'aire d'une surface
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[in] faceIndex Indice de la surface
	 * @return Aire de la surface en m²
	 */
	float ComputeFaceArea(const std::size_t& rsIndex,const std::size_t& faceIndex);
	/**
	 * Accesseur Information sur un enregistrement d'une face
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[in] faceIndex Indice de la surface
	 * @param[in] recordIndex Indice de l'enregistrement
	 * @param[out] idstep Compteur du pas de temps
	 * @param[out] energy Energie(w) ou temps(s)
	 * @python Return a tuple containing (idstep,energy)
	 */
	void GetFaceEnergy(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& recordIndex,std::size_t& idstep,float& energy) const;

	/**
	 * Accesseur Information sur un enregistrement d'une face
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[in] faceIndex Indice de la surface
	 * @return Energie cumulé (w) ou temps (s)
	 */
	float GetFaceSumEnergy(const std::size_t& rsIndex,const std::size_t& faceIndex);
protected:
	friend class rsurf_io;
	void SetStruct(t_ExchangeData* extData);
	t_ExchangeData* GetStruct();
private:
	bool manageDelete;
	t_ExchangeData* localData;
	void CheckNodeParameter(const std::size_t& nodeIndex) const;
	void CheckRsParameter(const std::size_t& rsIndex) const;
	void CheckFaceParameter(const std::size_t& rsIndex,const std::size_t& faceIndex) const;
	void CheckRecordParameter(const std::size_t& rsIndex,const std::size_t& faceIndex,const std::size_t& recordIndex) const;
};


class rsurf_io
{
	public:
	rsurf_io();

	static bool Load(const std::string& fileName,rsurf_data& data);
	static bool Save(const std::string& fileName,rsurf_data& data);

};


class rsurf_iso_contouring
{
public:

	/**
	 * /~english
	 * Iso Contouring initialisation step.
	 *
	 */
	rsurf_iso_contouring(const rsurf_data& data,const std::size_t& rsIndex);
	~rsurf_iso_contouring();
	/**
	 * Retourne les valeurs min,max
	 *
	 */
	void GetMinMax(float& min,float& max);
	/**
	 * Afin de lisser les isocourbes on peut multiplier le nombre de triangle et interpoler la valeur des nouveaux sommets.
	 * @param smoothcount Nombre de subdivision de la grille.
	 */
	void SmoothGrid(const int& smoothcount);
	/**
	 * Calcul la position de segments
	 */
	void GetIsoLevelCurves(const float& isolvl, std::list<t_curve>& curvlst);
private:
	t_iso_contouring_data* iso_contouring_data;
};

};
