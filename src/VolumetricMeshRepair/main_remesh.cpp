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

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "triangle_feeder.hpp"
#include "triang_marching_cube.hpp"
#include "adaptative_mesh_guide.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <input_output/ply/rply_interface.hpp>
#include <vector>
#include <coreString.h>

/**
 * Cette m�thode permet le tirage al�atoire d'un nombre d�cimal
 * @return Decimal de 0 � 1
 */
inline decimal GetRandValue()
{
	return ((decimal)rand()) / ((decimal)RAND_MAX+1);
}
inline int Range(const int& from,const int& to)
{
	int res=(int)floor(GetRandValue()*(to-from)+from);
	if(res<to)
		return res;
	else
		return from;
}


#ifndef MIN
	#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
	#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef MINREF
	#define MINREF(a, b)  if(a>b) a=b;
#endif
#ifndef MAXREF
	#define MAXREF(a, b)  if(a<b) a=b;
#endif
#ifndef MINVEC
	#define MINVEC(av, bv)  MINREF(av[0],bv[0]);MINREF(av[1],bv[1]);MINREF(av[2],bv[2]);
#endif
#ifndef MAXVEC
	#define MAXVEC(av, bv)  MAXREF(av[0],bv[0]);MAXREF(av[1],bv[1]);MAXREF(av[2],bv[2]);
#endif
void PrintUsage(int argc, char* argv[])
{
	std::cout<<"Usage: "<< argv[0] <<" [-precDECIMAL] [-depthINTEGER] [-v] [-qDECIMAL] [-noadaptative] [-skipext] [-extonly] [-mqDECIMAL] [-volstatsFILENAME] [-ivINTEGER] -iFILENAME -oFILENAME"<<std::endl;
	std::cout<<" -prec : Absolute cell size."<<std::endl;
	std::cout<<" -depth : [5-10] Relative cell size, cell subdivision count will be 2^depth . Default 5."<<std::endl;
	std::cout<<" -v : Verbose mode. Give more details about remeshing."<<std::endl;
	std::cout<<" -q : [0.-0.62] Worst triangle quality. Default 0.1 . Higher is better."<<std::endl;
	std::cout<<" -noadaptative : Remove the adaptative mesh step."<<std::endl;
	std::cout<<" -skipext : Do not remesh the exterior volume."<<std::endl;
	std::cout<<" -extonly : Remesh only the exterior volume."<<std::endl;
	std::cout<<" -mq : [0.-1.] Merge quality contraint . Default 0.999 .Merge quality constraint during the triangle decimation step."<<std::endl;
	std::cout<<" -i : PLY input filename."<<std::endl;
	std::cout<<" -o : PLY output filename."<<std::endl;
	std::cout<<" -iv : [2-Volume Count] Write the remesh of this volume ID. You can add this parameter multiple times.You can use the option -volstats to find volumes ID."<<std::endl;
	std::cout<<" -volstats : Discretise with prec or depth and write volumes statistics to specified file then stop execution."<<std::endl;
	std::cout<<std::endl;
}

struct VolumeSelectionInfo_t
{
	VolumeSelectionInfo_t()
		:minimalVol(0.f),selectionFilter(FILTER_BY_VOLUME)
	{}
	enum FILTER
	{
		FILTER_EXT_ONLY,
		FILTER_SKIP_EXT,
		FILTER_BY_VOLUME,
		FILTER_BY_ID
	} selectionFilter;
	decimal minimalVol;
	std::list<SpatialDiscretization::cell_id_t> extractedVolumes;

};
int MainRemesh(int argc, char* argv[])
{
	using namespace ScalarFieldBuilders;

	VolumeSelectionInfo_t volumeSelectionInfo;
	decimal precision(0.f);
	unsigned long d(0);
	bool adaptative_step(true);

	decimal quality_constraint(-1);
	decimal mergeQualityContraint(-2);

	std::string fileInput;
	std::string fileOutput;
	std::string volStatsOutput;
	unsigned int depth(0); //R�solution relative 2^d sur chaque axe
	unsigned int iv_buffer(0); //buffer pour les indices des volumes pass�s par l'utilisateur.
	bool verbose(false);
	try
	{
		while (--argc)
		{
			if (sscanf(argv[argc], "-prec%g", &precision) == 1);
			else if (sscanf(argv[argc], "-depth%lu", &d) == 1);
			else if (sscanf(argv[argc], "-iv%u", &iv_buffer) == 1)
				volumeSelectionInfo.extractedVolumes.push_back(iv_buffer);
			else if (sscanf(argv[argc], "-minvol%g", &volumeSelectionInfo.minimalVol) == 1);
			else if (sscanf(argv[argc], "-q%g", &quality_constraint) == 1);
			else if (sscanf(argv[argc], "-mq%g", &mergeQualityContraint) == 1);
			else if (strncmp(argv[argc], "-volstats", 9) == 0)
			  volStatsOutput = std::string(argv[argc] + 9);
			else if (strncmp(argv[argc], "-v", 2) == 0) verbose=true;
			else if (strncmp(argv[argc], "-skipext", 8) == 0) volumeSelectionInfo.selectionFilter=VolumeSelectionInfo_t::FILTER_SKIP_EXT;
			else if (strncmp(argv[argc], "-extonly", 8) == 0) volumeSelectionInfo.selectionFilter=VolumeSelectionInfo_t::FILTER_EXT_ONLY;
			else if (strncmp(argv[argc], "-noadaptative", 13) == 0) adaptative_step=false;
			else if (strncmp(argv[argc], "-i", 2) == 0)
			  fileInput = std::string(argv[argc] + 2);
			else if (strncmp(argv[argc], "-o", 2) == 0)
			  fileOutput = std::string(argv[argc] + 2);
			else {
			  PrintUsage(argc,argv);
			  return -2;
			}
		}
	}catch(...)
	{
	  PrintUsage(argc,argv);
	  return -2;
	}
	if(fileInput.empty() || (fileOutput.empty() && volStatsOutput.empty()))
	{
	  PrintUsage(argc,argv);
	  return -2;
	}
	vec3 minBoundingBox,maxBoundingBox;


	//1�re passe Evaluation de la boite englobante
	std::cout<<"Open "<< fileInput<<std::endl;
	formatRPLY::t_model model3D;
	if(!formatRPLY::CPly::ImportPly(model3D,fileInput) || model3D.modelVertices.size()==0)
		return -2;

	minBoundingBox=(*model3D.modelVertices.begin());
	maxBoundingBox=(*model3D.modelVertices.begin());
	std::vector<vec3> vertices_vec;
	vertices_vec.reserve(model3D.modelVertices.size());
	for(std::list<vec3>::iterator itvert=model3D.modelVertices.begin();itvert!=model3D.modelVertices.end();itvert++)
	{
		MAXVEC(maxBoundingBox,(*itvert));
		MINVEC(minBoundingBox,(*itvert));
		vertices_vec.push_back(*itvert);
	}
	model3D.modelVertices.clear();



	if(precision==0 && d==0)
		d=5;
	if(d!=0)
	{
		vec3 axesPrecision=(maxBoundingBox-minBoundingBox)/vec3((decimal)pow((decimal)2,(int)d),(decimal)pow((decimal)2,(int)d),(decimal)pow((decimal)2,(int)d));
		precision=MAX(axesPrecision.x,MAX(axesPrecision.z,axesPrecision.y));
	}
	std::cout<<"Discretization of space with a "<<precision<<" m cell width"<<std::endl;

	ScalarFieldBuilders::TriangleScalarFieldCreator FromTriangleRemesh(precision);
	FromTriangleRemesh.FirstStep_Params(minBoundingBox,maxBoundingBox);

	PTR<Triangulators::AdaptativeMeshGuide> meshGuide;
	if(adaptative_step && volStatsOutput.empty())
		meshGuide=PTR<Triangulators::AdaptativeMeshGuide>(new Triangulators::AdaptativeMeshGuide);

	std::size_t domainSize(FromTriangleRemesh.GetDomainSize());
	if(verbose)
		std::cout<<"Matrix size "<<domainSize<<"x"<<domainSize<<"x"<<domainSize<<" = "<<pow((decimal)domainSize,3)<<" cells max("<<pow((decimal)domainSize,2)<<"min)"<<std::endl;


	/////////////////////////////////////////////////////////////
	//2 eme �tape Passe Renseignement des limites des domaines

	unsigned int idtri(0);
	std::size_t triCount(model3D.modelFaces.size());
	int lastprogression(0),progression(0);
	std::cout<<"Feeding matrix "<<std::endl;
	for(std::list<formatRPLY::t_face>::iterator itface=model3D.modelFaces.begin();itface!=model3D.modelFaces.end();itface++)
	{
		FromTriangleRemesh.SecondStep_PushTri(vertices_vec[(*itface).indicesSommets.a],
			vertices_vec[(*itface).indicesSommets.b],
			vertices_vec[(*itface).indicesSommets.c]);
		if(verbose)
		{
			idtri++;
			progression=int(((float)idtri/(float)triCount)*100);
			if(progression!=lastprogression)
			{
				std::cout<<"Feeding matrix "<<progression<<"% face"<<idtri<<"/"<<triCount<<std::endl;
				lastprogression=progression;
			}
		}
	}
	if(verbose)
	{
		std::cout<<"Finish feeding matrix with intersection nodes"<<std::endl;
		std::cout<<"There are "<<FromTriangleRemesh.count()<<" cells in the matrix"<<std::endl;
	}


	if(meshGuide.get())
	{
		for(std::list<formatRPLY::t_face>::iterator itface=model3D.modelFaces.begin();itface!=model3D.modelFaces.end();itface++)
		{

			meshGuide->AddTri(vertices_vec[(*itface).indicesSommets.a],
				vertices_vec[(*itface).indicesSommets.b],
				vertices_vec[(*itface).indicesSommets.c]);

		}

		meshGuide->EndTriangleFeeding((unsigned short)floor((log((decimal)domainSize)/log((decimal)2))));
	}
	vertices_vec.clear();
	model3D.modelFaces.clear();

	/////////////////////////////////////////////////////////////
	//3 eme �tape Discr�tisation spaciale


	FromTriangleRemesh.ThirdStep_VolumesCreator();
	if(verbose)
	{
		std::cout<<"Volumes defined :"<<std::endl;
		for(int i=2;i<FromTriangleRemesh.GetVolumeCount();i++)
		{
			std::cout<<"Volume id:"<<i<<" value:"<<FromTriangleRemesh.GetVolumeValue(i)<<" m^3"<<std::endl;
		}
	}else{
		std::cout<<"Volumes defined.."<<std::endl;
	}

	if(!(volStatsOutput.empty()))
	{
		FromTriangleRemesh.ExportVolsStats(volStatsOutput);
		return 0;
	}

	/////////////////////////////////////////////////////////////
	//4 eme �tape Marching Cubes
	progressionInfo progressInfo(1); //Marching Cube, Adaptative Mesh, Polygon decimation

	if(volumeSelectionInfo.extractedVolumes.empty())
	{
		for(int i=2;i<FromTriangleRemesh.GetVolumeCount();i++)
		{
			if(FromTriangleRemesh.GetVolumeValue(i)>volumeSelectionInfo.minimalVol && (!(volumeSelectionInfo.selectionFilter==VolumeSelectionInfo_t::FILTER_SKIP_EXT) || i!=2) && (!(volumeSelectionInfo.selectionFilter==VolumeSelectionInfo_t::FILTER_EXT_ONLY) || i==2))
			{
				volumeSelectionInfo.extractedVolumes.push_back(i);
			}
		}
	}
	std::size_t volumeCountToProcess(volumeSelectionInfo.extractedVolumes.size());

	progressOperation volumeOperation(progressInfo.GetMainOperation(),volumeCountToProcess);
	for(std::list<SpatialDiscretization::cell_id_t>::iterator itvol=volumeSelectionInfo.extractedVolumes.begin();itvol!=volumeSelectionInfo.extractedVolumes.end();itvol++)
	{
		int i(*itvol);
		//if(FromTriangleRemesh.GetVolumeValue(i)>volumeSelectionInfo.minimalVol && (!(volumeSelectionInfo.selectionFilter==VolumeSelectionInfo_t::FILTER_SKIP_EXT) || i!=2) && (!(volumeSelectionInfo.selectionFilter==VolumeSelectionInfo_t::FILTER_EXT_ONLY) || i==2))
		if(i>1 && i<FromTriangleRemesh.GetVolumeCount())
		{
			std::size_t lastPathSeparatorPosition(fileOutput.rfind(st_path_separator()));
			std::string basePath;
			std::string fileName;
			if(lastPathSeparatorPosition!=std::string::npos)
			{
				basePath=std::string(fileOutput.begin(),fileOutput.begin()+lastPathSeparatorPosition+1);
				fileName=std::string(fileOutput.begin()+lastPathSeparatorPosition+1,fileOutput.end());
			}else{
				fileName=fileOutput;
			}

			std::string saveto(basePath+CoreString::FromInt(i)+CoreString("_")+fileName);
			progressOperation ProcessVolumeOperation(&volumeOperation,3);
			std::cout<<"Running marching cube.."<<std::endl;
			Triangulators::MarchingCubeTriangulator marchCubeTrig;

			marchCubeTrig.Triangulate(FromTriangleRemesh,i,ProcessVolumeOperation);

			/////////////////////////////////////////////////////////////
			//5 eme �tape Rapprochement du maillage par rapport au mod�le d'origine

			progressOperation AdaptativeMeshOperation(&ProcessVolumeOperation,1);
			if(meshGuide.get())
			{
				std::cout<<"Running adaptative mesh operation.."<<std::endl;
				marchCubeTrig.RefineTriangulationGrid(*meshGuide.get(),precision,AdaptativeMeshOperation,progressInfo);
			}

			/////////////////////////////////////////////////////////////
			//6 eme �tape R�duction de triangles
			std::cout<<"Reducing polygons.."<<std::endl;
			if(quality_constraint!=-1)
				marchCubeTrig.SetQualityConstraint(quality_constraint);
			if(mergeQualityContraint!=-2)
				marchCubeTrig.SetMergeQualityConstraint(mergeQualityContraint);
			marchCubeTrig.SetVerbose(verbose);
			marchCubeTrig.PolygonReduction(ProcessVolumeOperation);




			if(i==2) //Les faces sont invers�s dans le maillage exterieur du mod�le
				marchCubeTrig.InvertFacesNormal();
			std::cout<<"Save model in "<<saveto<<std::endl;
			marchCubeTrig.SaveAs(saveto);
		}
	}
	return 0;
}
int main(int argc, char* argv[])
{
	int ret(MainRemesh(argc,argv));

    #ifdef _DEBUG
		_CrtDumpMemoryLeaks(); //Affiche les fuites m�moires
	#endif

	return ret;
}
