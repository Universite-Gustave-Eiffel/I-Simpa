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

#include "first_header_include.hpp"

/**
 * @file recepteursurf_difference.hpp
 * @brief Méthode de calcul de calcul d'attenuation entre deux fichiers de récepteur de surface
 *
 * Cet ensemble de méthode permet de calculer l'attenuation à partir de plusieurs enregistrements.
 */
#include <vector>
#include <wx/string.h>
#include <Core/mathlib.h>
#include <input_output/exportRecepteurSurf/rsbin.h>

#ifndef __RSURF_ATTENUATION_COMPUTATION__
#define __RSURF_ATTENUATION_COMPUTATION__
/**
 * @brief Méthode de calcul de calcul d'attenuation entre deux fichiers de récepteur de surface
 *
 * Cet ensemble de méthode permet de calculer l'attenuation à partir de plusieurs enregistrements.
 */
class RecepteurSurfAttenuationComputation
{
public:

	RecepteurSurfAttenuationComputation(wxString rsurfReferenceFile);

	bool SaveAttenuationRecepteurSurfFile(wxString rsurfSecondFile, wxString destinationResultFile );
private:
	struct t_rs_face
	{
		std::vector<wxInt32> timeStepToIndex; //-1 indique que ce pas de temps correspond a une énérgie nulle
	};
	struct t_r_surfprocess
	{
		std::vector<t_rs_face> faces;
	};
	struct t_process_data
	{
		formatRSBIN::t_ExchangeData rawData;
		std::vector<t_r_surfprocess> recepteurSurf;
	};
	t_process_data referenceRS;
	bool LoadRs(wxString pathRsFile,t_process_data* dataDestination);
	bool SaveRs(wxString pathRsFile,t_process_data& dataSource);

};

#endif