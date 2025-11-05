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

#include <wx/dynarray.h>
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/aui/aui.h>
#include <wx/toolbar.h>
#include <wx/progdlg.h>

#include <vector>

/**
 * @file nc_curves.cpp
 * @brief Contains sellected NC curves what allows to introduce background noise to STI calculation.
 * See:https://www.engineeringtoolbox.com/nc-noise-criterion-d_725.html
 */
class nc_curves
{
private:
	int NC_data[13][8];
public:
	nc_curves(){
		int tmp[13][8]={
					{47,36,29,22,17,14,12,11},
					{51,40,33,26,22,19,17,16},
					{54,44,37,31,27,24,22,21},
					{57,48,41,35,31,29,28,27},
					{60,52,45,40,36,34,33,32},
					{64,56,50,45,41,39,38,37},
					{67,60,54,49,46,44,43,42},
					{71,64,58,54,51,49,48,47},
					{74,67,62,58,56,54,53,52},
					{77,67,62,58,56,54,53,52},
					{80,75,71,68,66,64,63,62},
					{83,79,75,72,71,70,69,68},
					{0,0,0,0,0,0,0,0}
					};
		for(int i=0;i<13;i++){
			for(int j=0;j<8;j++){
				NC_data[i][j]=tmp[i][j];
			}
		}
	}

	int* get_nc(wxFloat32 idx)
	{
		switch((int)idx)
		{
			case 15:
				return NC_data[0];
				break;
			case 20:
				return NC_data[1];
				break;
			case 25:
				return NC_data[2];
				break;
			case 30:
				return NC_data[3];
				break;
			case 35:
				return NC_data[4];
				break;
			case 40:
				return NC_data[5];
				break;
			case 45:
				return NC_data[6];
				break;
			case 50:
				return NC_data[7];
				break;
			case 55:
				return NC_data[8];
				break;
			case 60:
				return NC_data[9];
				break;
			case 65:
				return NC_data[10];
				break;
			case 70:
				return NC_data[11];
				break;
			default:
				return NC_data[12];
				break;
		}
	}
};