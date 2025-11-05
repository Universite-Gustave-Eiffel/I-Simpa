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

#include "vol_identifier.hpp"
#include <list>

namespace volume_identifier
{
	void VolumeIdentifier::IdentifyVolumes(formatMBIN::trimeshmodel& tetramodel)
	{
		using namespace formatMBIN;
		for(std::vector<bintetrahedre>::iterator it=tetramodel.tetrahedres.begin();it!=tetramodel.tetrahedres.end();it++)
		{
			(*it).idVolume=-1;
		}
		std::size_t start_tetra=0;
		std::size_t nbtetra=tetramodel.tetrahedres.size();
		std::size_t volcount=0;
		while(start_tetra<nbtetra)
		{
			std::list<std::size_t> vol_arb;
			std::list<short> vol_neigh;
			vol_arb.push_back(start_tetra);
			vol_neigh.push_back(-1);

			while(vol_arb.size()>0)
			{
				vol_neigh.back()++;
				bintetrahedre& cur_tetra=tetramodel.tetrahedres[vol_arb.back()];
				cur_tetra.idVolume=volcount;
				if(vol_neigh.back()<4)
				{
					if(cur_tetra.tetrafaces[vol_neigh.back()].marker==-1)
					{
						const Intb& neigh_id=cur_tetra.tetrafaces[vol_neigh.back()].neighbor;
						if(neigh_id!=-2)
						{
							//Ce voisin n'est pas coup� avec nous par une face du mod�le et il y en a un
							if(tetramodel.tetrahedres[neigh_id].idVolume==-1) //Si le voisin n'a pas �t� trait�
							{
								//On va traiter le voisin
								vol_arb.push_back(neigh_id);
								vol_neigh.push_back(-1);
							}
						}
					}
				}else{
					//On va traiter le prochain fils du p�re
					vol_neigh.pop_back();
					vol_arb.pop_back();
				}
			}
			while( start_tetra<nbtetra && tetramodel.tetrahedres[start_tetra].idVolume!=-1)
				start_tetra++;
			volcount++;
		}
		//Tout les t�tra�dres ont �t� marqu�s
	}
};
