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

#include "vol_splitter.hpp"
#include "Core/mathlib.h"
#include <set>
#include <tools/octree44_dotElement.hpp>
#include <tools/octree44_sphereElement.hpp>

namespace volumes_splitter
{

#define SET_FACE_STATE(tetraface) if(tetraface.marker!=-1 && !faces[tetraface.marker].processed)\
			{\
				faces[tetraface.marker].volid_norm=(*ittetra).idVolume;\
				faces[tetraface.marker].processed=true;\
				if(tetraface.neighbor>=0 && (std::size_t)tetraface.neighbor<quanttetra)\
				{\
					faces[tetraface.marker].volid_invnorm=tetramodel.tetrahedres[tetraface.neighbor].idVolume;\
					faces[tetraface.marker].isInternalFace=true;\
				}\
			}

#define GET_SOMMET_COORDS(idsommet) tetramodel.nodes[(*ittetra).vertices[idsommet]].node
	class TetraFinder
	{
	public:
		TetraFinder(vec3 _dotTest,const formatMBIN::trimeshmodel& _tetramodel)
			:dotTest(_dotTest),
			tetramodel(_tetramodel),
			idtetra(-1)
		{}
		bool OnNewTetraCandidate(const octreeTool::elementSize& staticCandidateIndex)
		{
			//ref http://steve.hollasch.net/cgindex/geometry/ptintet.html
			//ref http://jgt.akpeters.com/papers/GuigueDevillers03/addendum.html
			const formatMBIN::bintetrahedre& tettest=tetramodel.tetrahedres[staticCandidateIndex];

			if(DotInTetra(dotTest,tetramodel.nodes[tettest.vertices[0]].node,tetramodel.nodes[tettest.vertices[1]].node,tetramodel.nodes[tettest.vertices[2]].node,tetramodel.nodes[tettest.vertices[3]].node))
			{
				idtetra=staticCandidateIndex;
				return false;
			}else{
				return true;
			}

			return true;

		}
		int idtetra;
	protected:
		const formatMBIN::trimeshmodel& tetramodel;
		vec3 dotTest;

	};
	VolumesSplitter::VolumesSplitter()
		:tetraOctree(NULL)
	{

	}

	VolumesSplitter::~VolumesSplitter()
	{
		delete tetraOctree;
	}
	void VolumesSplitter::FindOrCreateVol(const int& idvol,int& lastvolid_norm,vol_t** pt_vol)
	{
		lastvolid_norm=idvol;
		int curel=0;
		for(std::list<vol_t>::iterator it=vols.begin();it!=vols.end();it++)
		{
			if((*it).volId==idvol)
			{
				*pt_vol=&(*it);
				return;
			}
			curel++;
		}
		//Le volume n'existe pas
		vols.push_back(vol_t());

		*pt_vol=&vols.back();
		vols.back().volId=idvol;
	}

	void VolumesSplitter::LoadOctree(const formatMBIN::trimeshmodel& tetramodel)
	{
		std::size_t quanttetra=tetramodel.tetrahedres.size();
		std::vector<octreeTool::sphereElement> tmpTetraContainer(quanttetra);
		octreeTool::spaceElementContainer spcontainer;
		spcontainer.reserve(quanttetra);

		std::size_t idtet=0;
		for(std::vector<formatMBIN::bintetrahedre>::const_iterator ittetra=tetramodel.tetrahedres.begin();ittetra!=tetramodel.tetrahedres.end();ittetra++)
		{
			vec3 gtetra=GetGTetra(GET_SOMMET_COORDS(0),
												GET_SOMMET_COORDS(1),
												GET_SOMMET_COORDS(2),
												GET_SOMMET_COORDS(3));
			tmpTetraContainer[idtet].Set(gtetra);
			float radius=gtetra.distance(GET_SOMMET_COORDS(0));
			radius=MAX(radius,gtetra.distance(GET_SOMMET_COORDS(1)));
			radius=MAX(radius,gtetra.distance(GET_SOMMET_COORDS(2)));
			radius=MAX(radius,gtetra.distance(GET_SOMMET_COORDS(3)));
			tmpTetraContainer[idtet].SetRadius(radius);
			spcontainer.push_back(&tmpTetraContainer[idtet]);
			idtet++;
		}
		delete tetraOctree;
		tetraOctree=new octreeTool::Octree44(spcontainer,5);
	}
	void VolumesSplitter::LoadDomain(const formatCoreBIN::ioModel& model,const formatMBIN::trimeshmodel& tetramodel)
	{
		faces.clear();
		vols.clear();
		faces.insert(faces.end(),model.faces.size(),facemodel_t());

		std::size_t quanttetra=tetramodel.tetrahedres.size();
		int lastvolid=-2;
		vol_t* lastvol=NULL;
		for(std::vector<formatMBIN::bintetrahedre>::const_iterator ittetra=tetramodel.tetrahedres.begin();ittetra!=tetramodel.tetrahedres.end();ittetra++)
		{
			//Mise à jour Lien, face<->volume
			SET_FACE_STATE((*ittetra).tetrafaces[0])
			SET_FACE_STATE((*ittetra).tetrafaces[1])
			SET_FACE_STATE((*ittetra).tetrafaces[2])
			SET_FACE_STATE((*ittetra).tetrafaces[3])
			//Mise à jour du volume
			if((*ittetra).idVolume!=lastvolid)
				FindOrCreateVol((*ittetra).idVolume,lastvolid,&lastvol);

			lastvol->volume+=CalcTetraVolume(GET_SOMMET_COORDS(0),
												GET_SOMMET_COORDS(1),
												GET_SOMMET_COORDS(2),
												GET_SOMMET_COORDS(3));
		}
		int lastvolid_norm=-2;
		vol_t* lastvol_norm=NULL;
		int lastvolid_invnorm=-2;
		vol_t* lastvol_invnorm=NULL;
		std::size_t idface=0;

		for(std::vector<facemodel_t>::iterator it=faces.begin();it!=faces.end();it++)
		{
			if((*it).volid_norm!=-1)
			{
				if((*it).volid_norm!=lastvolid_norm)
					FindOrCreateVol((*it).volid_norm,lastvolid_norm,&lastvol_norm);
				lastvol_norm->faceIds.push_back(idface);
			}
			if((*it).volid_invnorm!=-1)
			{
				if((*it).volid_invnorm!=lastvolid_invnorm)
					FindOrCreateVol((*it).volid_invnorm,lastvolid_invnorm,&lastvol_invnorm);
				lastvol_invnorm->faceIds.push_back(idface);
			}
			idface++;
		}
	}

	int VolumesSplitter::GetVolumeXmlId(const int& idel)
	{
		int curel=0;
		for(std::list<vol_t>::iterator it=vols.begin();it!=vols.end();it++)
		{
			if(curel==idel)
			{
				return (*it).volId;
			}
			curel++;
		}
		return -1;
	}
	void VolumesSplitter::GetVolumeInformation(const int& idel,int& volumeId,double& volume,std::vector<std::size_t>& faceIds,std::vector<std::size_t>& volNeigh)
	{
		int curel=0;
		std::set<std::size_t> uniqueNeigh;
		for(std::list<vol_t>::iterator it=vols.begin();it!=vols.end();it++)
		{
			if(curel==idel)
			{
				volumeId=(*it).volId;
				volume=(*it).volume;
				faceIds.reserve((*it).faceIds.size());
				for(std::list<size_t>::iterator itface=(*it).faceIds.begin();itface!=(*it).faceIds.end();itface++)
				{
					faceIds.push_back(*itface);
					if(faces[*itface].isInternalFace)
					{
						if(faces[*itface].volid_invnorm!=volumeId)
							uniqueNeigh.insert(faces[*itface].volid_invnorm);
						else
							uniqueNeigh.insert(faces[*itface].volid_norm);
					}
				}
				volNeigh.reserve(uniqueNeigh.size());
				for(std::set<size_t>::iterator itneigh=uniqueNeigh.begin();itneigh!=uniqueNeigh.end();itneigh++)
				{
					volNeigh.push_back(*itneigh);
				}
				return;
			}
			curel++;
		}
		throw "This volume doesn't exist.";
	}
	void VolumesSplitter::GetInternalFaces(std::vector<std::size_t>& internalFaceIds,const int& volumeXmlIdFilter)
	{
		internalFaceIds.reserve(faces.size());
		std::size_t idface=0;
		for(std::vector<facemodel_t>::iterator it=faces.begin();it!=faces.end();it++)
		{
			if((*it).isInternalFace && (volumeXmlIdFilter==-1 ||(*it).volid_invnorm==volumeXmlIdFilter || (*it).volid_norm==volumeXmlIdFilter))
				internalFaceIds.push_back(idface);
			idface++;
		}
	}
    /**
	 * Classe de test pour les collisions
	 */
	class AllCandidatesReceiver
	{
	public:
		AllCandidatesReceiver() {}
		bool ClientMethodReceiver(const octreeTool::elementSize& staticCandidateIndex)
		{
			candidatesLst.push_back(staticCandidateIndex);
			return true;
		}
		void fillVector(std::vector<octreeTool::elementSize>& vectorToFeed)
		{
			candidatesLst.sort();
			candidatesLst.unique();
			vectorToFeed.reserve(candidatesLst.size());
			for(std::list<octreeTool::elementSize>::iterator itcand=candidatesLst.begin();itcand!=candidatesLst.end();itcand++)
			{
				vectorToFeed.push_back(*itcand);
			}
		}
	protected:
		std::list<octreeTool::elementSize> candidatesLst;
	};

	int VolumesSplitter::GetTetraByPosition(const formatMBIN::trimeshmodel& tetramodel, const float& x,const float& y,const float& z,const bool& reloadTetraModel)
	{
		if(!tetraOctree || reloadTetraModel)
			LoadOctree(tetramodel);
		if(tetraOctree)
		{
			TetraFinder tetraTest(vec3(x,y,z),tetramodel);
			octreeTool::dotElement dottest(vec3(x,y,z));
			octreeTool::spaceElement* dot_root=&dottest;
			tetraOctree->GetDynamicCollisionCandidates<TetraFinder>(*dot_root,
			&tetraTest,
			&TetraFinder::OnNewTetraCandidate);

			return tetraTest.idtetra;

		}
		return -1;
	}
};
