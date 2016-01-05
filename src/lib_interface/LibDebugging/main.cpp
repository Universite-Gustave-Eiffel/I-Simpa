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

#include <input_output/bin.h>
#include <iostream>

int main(int argc, char* argv[])
{
  using namespace formatCoreBIN;
  int szint=sizeof(uint32_t);
  std::cout<<"uint32_t="<<szint<<std::endl;
  ioModel model;
  CformatBIN tool;
  if(tool.ImportBIN(model, "mesh.cbin"))
  {
    std::cout<<"Model load success"<<std::endl;
    std::cout<<"There are "<<model.faces.size()<<" faces"<<std::endl;
  }else{
    std::cout<<"Model load failed"<<std::endl;
  }
 return 0;
}
/*
#include <tools/vol_identifier.hpp>
#include <tools/vol_splitter.hpp>
#include <iostream>
int main(int argc, char* argv[])
{
	using namespace volume_identifier;
	using namespace formatMBIN;
	CMBIN triimporter;
	trimeshmodel modeltri=triimporter.LoadMesh("tetramesh.mbin");
	formatCoreBIN::CformatBIN importer;
	formatCoreBIN::ioModel model;
	importer.ImportBIN(model,"mesh.cbin");

	//VolumeIdentifier::IdentifyVolumes(modeltri);
	volumes_splitter::VolumesSplitter splitvol;
	splitvol.LoadDomain(model,modeltri);
	int volxid;
	double vol;
	std::vector<std::size_t> faces;
	std::vector<std::size_t> neigh;
	//splitvol.GetVolumeInformation(0,volxid,vol,faces,neigh);
	//0.598193" y="1.766407" z="0.750000
	int idtet=splitvol.GetTetraByPosition(modeltri,0.598193f,1.766407f,0.750000f);
	std::cout<<"Id tet:"<<idtet<<std::endl;
}

*/
/*
#include "tools/surf_merging.hpp"
int main(int argc, char* argv[])
{
	using namespace surf_merge;
	formatCoreBIN::ioModel model;
	formatCoreBIN::CformatBIN importer;
	importer.ImportBIN(model,"modeldebug.cbin");
	surf_merge::SurfaceMerging merger;
	merger.LoadGroups(model);
}
*/
/*
#include <input_output/exportRecepteurSurf/std_rsbin.hpp>
#include <Core/mathlib.h>

int main(int argc, char* argv[])
{
	using namespace formatRSBIN;
	rsurf_data datars;
	rsurf_io::Load("rs_cut.csbin",datars);

	rsurf_iso_contouring contouringTool(datars,0);
	contouringTool.SmoothGrid(1);
	float dbcurve=13.27f;
	std::list<t_curve> curvlst;
	//powf(10.f,dbcurve/10)*powf(10.f,-12.f)
	contouringTool.GetIsoLevelCurves(dbcurve,curvlst);
	return 0;
}
*/
