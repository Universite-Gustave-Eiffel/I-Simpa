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

#include "m_report_bin.h"
#include <fstream>
#include <string.h>

namespace formatMRENDERBIN
{

bool REPORT_MBIN::ExportBIN(const char *strFileName,t_reportbinexchange& reportData)
{
	//Namespace
	using namespace std;

	//Declarations

	//Sauvegarde du modèle 3D
	fstream binFile (strFileName, ios::out | ios::binary);

	//*************************
	//Ecriture de l'entete du fichier
	binFile.write((char*)&reportData.data_infos,sizeof(t_FileHeader));
	//*************************
	//Ecriture des noeuds
	binFile.write((char*)reportData.nodes,sizeof(t_binNode)*reportData.data_infos.quantNodes);
	//*************************
	//Ecriture des tetrahèdres
	binFile.write((char*)reportData.tetras,sizeof(bintetrahedre)*reportData.data_infos.quantTetra);

	binFile.close();
	return true;
}

bool  REPORT_MBIN::ImportBIN(const char *strFileName,t_reportbinexchange& reportData)
{
	//Namespace
	using namespace std;

	//Declarations

	//Sauvegarde du modèle 3D
    fstream binFile (strFileName, ios::in | ios::binary);
	if(binFile.is_open())
	{
		//*************************
		//Lecture de l'entete du fichier
		memset(&reportData.data_infos,0,sizeof(t_FileHeader));
		binFile.read((char*)&reportData.data_infos,sizeof(t_FileHeader));

		if(reportData.data_infos.quantNodes>0 && reportData.data_infos.quantTetra>0)
		{
			delete[] reportData.nodes;
			delete[] reportData.tetras;
			reportData.nodes = new t_binNode[reportData.data_infos.quantNodes];
			reportData.tetras = new bintetrahedre[reportData.data_infos.quantTetra];
			//*************************
			//Lecture des noeuds
			binFile.read((char*)reportData.nodes,sizeof(t_binNode)*reportData.data_infos.quantNodes);
			//*************************
			//Lecture des tetrahèdres
			binFile.read((char*)reportData.tetras,sizeof(bintetrahedre)*reportData.data_infos.quantTetra);
			binFile.close();
		}else{
			binFile.close();
			return false;
		}
		return true;
	}else{
		return false;
	}
}

} //fin namespace
