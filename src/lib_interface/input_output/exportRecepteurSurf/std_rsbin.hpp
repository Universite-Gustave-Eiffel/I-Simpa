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

#include <vector>
#include <list>
#include <string>
#include "Core/mathlib.h"
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
struct t_iso_contouring_data;
struct t_curve
{
	vec3 A;
	vec3 B;
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
	 */;
	void SetRsBinData(t_ExchangeData& rsData);
	/**
	 * @return le nombre de récepteurs surfaciques.
	 */
	std::size_t GetRsCount() const;
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
	 * @return The number of vertices
	 */
	std::size_t GetNodesCount() const;
	/**
	 * @return the number of time steps
	 */
	std::size_t GetTimeStepCount() const;

	/**
	 * @return The timestep value in second
	 */
	float GetTimeStep() const;
	/**
	 * @return record type "SPL_STANDART" "SPL_GAIN" "TR" "EDT"
	 */
	std::string getRecordType() const;

	/**
	 * Accesseur valeur d'un noeud
	 * @param[in] nodeIndex Indice du sommet
	 * @return node coordinate
	 */
	vec3 GetNodePositionValue(const std::size_t& nodeIndex) const;
	
	/**
	 * @param[in] rsIndex Receiver index
	 * @return Surface count 
	 */
	std::size_t GetRsFaceCount(const std::size_t& rsIndex) const;

	/**
	* @param[in] rsIndex Receiver index
	* @return Surface receiver name
	*/
	std::string GetRsName(const std::size_t& rsIndex) const;

	/**
	* @param[in] rsIndex Receiver index
	* @return Surface receiver xmlId
	*/
	int GetRsXmlId(const std::size_t& rsIndex) const;

	/**
	* @param[in] rsIndex Receiver index
	* @param[in] faceIndex Surface index
	* @return Triangle vertices index
	*/
	ivec3 GetFaceVertices(const std::size_t& rsIndex, const std::size_t& faceIndex) const;


	/**
	* @param[in] rsIndex Receiver index
	* @param[in] faceIndex Surface index
	* @return Triangle record count
	*/
	std::size_t GetFaceRecordCount(const std::size_t& rsIndex, const std::size_t& faceIndex) const;


	/**
	 * Calcul l'aire d'une surface
	 * @param[in] rsIndex Indice du récepteur de surface
	 * @param[in] faceIndex Indice de la surface
	 * @return Aire de la surface en m²
	 */
	float ComputeFaceArea(const std::size_t& rsIndex,const std::size_t& faceIndex);
	
	/**
	* @param[in] rsIndex Receiver index
	* @param[in] faceIndex Surface index
	* @param[in] recordIndex Record index
	* @return Triangle record energy
	*/
	float GetFaceEnergy(const std::size_t& rsIndex, const std::size_t& faceIndex, const std::size_t& recordIndex) const;

	/**
	* @param[in] rsIndex Receiver index
	* @param[in] faceIndex Surface index
	* @param[in] recordIndex Record index
	* @return Triangle record time step.To compute in seconds multiply by #GetTimeStep()
	*/
	std::size_t GetFaceTimeStep(const std::size_t& rsIndex, const std::size_t& faceIndex, const std::size_t& recordIndex) const;
	
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
