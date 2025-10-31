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

#include "coreTypes.h"
#include "input_output/importExportMaillage/mbin.h"
#include <iostream>
#include <assert.h>

//////////////////////////////////////////
// t_Recepteur_P

t_Recepteur_P::t_Recepteur_P(uentier _nbfreq,	uentier _nbtimestep)
	:nbfreq(_nbfreq),nbtimestep(_nbtimestep)
{

	bruit_spectre=new t_Bruit_Freq[nbfreq];
	energy_sum=new l_decimal*[nbfreq];
	memset(energy_sum,0,sizeof(l_decimal*)*nbfreq);
	memset(bruit_spectre,0,sizeof(t_Bruit_Freq)*nbfreq);
};
t_Recepteur_P::~t_Recepteur_P()
{
	for(unsigned int idfreq=0;idfreq<nbfreq;idfreq++)
		delete[] energy_sum[idfreq];
	delete[] energy_sum;
	delete[] bruit_spectre;
}
void t_Recepteur_P::InitFreq(uentier idfreq)
{
	energy_sum[idfreq]=new l_decimal[nbtimestep];
	memset(energy_sum[idfreq],0,sizeof(l_decimal)*nbtimestep);
}

//////////////////////////////////////////
// r_Surf_Face


r_Surf_Face::r_Surf_Face()
{
	energieRecu=NULL;
	nbfreq=0;
	nbtimestep=0;
}

r_Surf_Face::~r_Surf_Face()
{
	deleteData();
}

void r_Surf_Face::deleteData()
{
	if(energieRecu && nbfreq>0)
	{
		for(uentier idfreq=0;idfreq<nbfreq;idfreq++)
			delete[] energieRecu[idfreq];
	}
	delete[] energieRecu;
	energieRecu=NULL;
}
void r_Surf_Face::InitTabFreq(uentier _nbfreq)
{
	deleteData();
	nbfreq=_nbfreq;
	energieRecu=new decimal*[nbfreq];
	memset(energieRecu,0,sizeof( decimal*)*nbfreq);
}
void r_Surf_Face::InitFreq(uentier idfreq, uentier _nbtimestep)
{
	nbtimestep=_nbtimestep;
	if(nbfreq>idfreq)
	{
		delete[] energieRecu[idfreq];
		energieRecu[idfreq]=new decimal[_nbtimestep];
		memset(energieRecu[idfreq],0,sizeof(decimal)*_nbtimestep);
	}
}

void r_Surf_Face::operator=(const r_Surf_Face& copyFrom)
{
	this->indiceSommets=copyFrom.indiceSommets;
	memcpy(this->sommets,copyFrom.sommets,sizeof(vec3)*3);
	this->InitTabFreq(copyFrom.nbfreq);
	for(uentier idfreq=0;idfreq<nbfreq;idfreq++)
	{
		if(copyFrom.energieRecu[idfreq])
		{
			this->InitFreq(idfreq,copyFrom.nbtimestep);
			memcpy(energieRecu[idfreq],copyFrom.energieRecu[idfreq],sizeof(decimal)*copyFrom.nbtimestep);
		}
	}
}
//////////////////////////////////////////
// r_Surf

r_Surf::r_Surf()
{
	faces=NULL;
	nbFaces=0;
	idRecepteurS=-1;
}


r_Surf::r_Surf(r_Surf& copyFrom)
{
	idRecepteurS=copyFrom.idRecepteurS;
	nbFaces=copyFrom.nbFaces;

	//Create from a copy
	if(copyFrom.nbFaces==0)
	{
		faces=NULL;
	}else{
		faces=new r_Surf_Face[nbFaces];
		for(uentier idface=0;idface<nbFaces;idface++)
		{
			faces[idface]=copyFrom.faces[idface];
		}
	}
}

r_Surf::~r_Surf()
{
	delete[] faces;
}

void r_Surf::InitFaces(uentier _nbFaces, uentier _nbfreq)
{
	nbFaces=_nbFaces;
	delete[] faces;
	faces=new r_Surf_Face[nbFaces];
	for(uentier idface=0;idface<nbFaces;idface++)
	{
		faces[idface].InitTabFreq(_nbfreq);
	}
}




//////////////////////////////////////////
// t_TetraMesh


bool t_TetraMesh::LoadFile(const char * fileName, t_Mesh &sceneMesh,uentier nbFreq)
{

	delete[] nodes;
	delete[] tetraedres;
	nodes=NULL;
	tetraedres=NULL;

	using namespace formatMBIN;
	bintetrahedre* tabtetra;
	t_binNode* tabNodes;

	CMBIN binImporter;
	unsigned int binsizeTetra=0;
	unsigned int binsizeNodes=0;

	if(!binImporter.ImportBIN(fileName,&tabtetra,&tabNodes,binsizeTetra,binsizeNodes)) {
		return false;
	}

	if(binsizeTetra>0 && binsizeNodes>0)
	{
		this->nodesSize=binsizeNodes;
		// Copy vertex
		nodes=new vec3[nodesSize];
		memcpy(nodes,tabNodes,sizeof(vec3)*nodesSize);
		// Copy tetrahedrons
		tetraedres=new t_Tetra[binsizeTetra];
		tetraedresSize=binsizeTetra;

		for(unsigned int idTetra=0;idTetra<binsizeTetra;idTetra++)
		{
			tetraedres[idTetra].index=idTetra;
			tetraedres[idTetra].idVolume=tabtetra[idTetra].idVolume;
			//Copy vertex
            tetraedres[idTetra].sommets.set(tabtetra[idTetra].vertices[0], tabtetra[idTetra].vertices[1],
                                                    tabtetra[idTetra].vertices[2], tabtetra[idTetra].vertices[3]);
			assert(tetraedres[idTetra].sommets.a < nodesSize &&
                           tetraedres[idTetra].sommets.b < nodesSize &&
                           tetraedres[idTetra].sommets.c < nodesSize &&
                           tetraedres[idTetra].sommets.d < nodesSize);
            for(int idvertLeft = 0; idvertLeft < 4; idvertLeft++) {
                for (int idvertRight = 0; idvertRight < 4; idvertRight++) {
                    if(idvertLeft != idvertRight) {
                        if(tabtetra[idTetra].vertices[idvertLeft] == tabtetra[idTetra].vertices[idvertRight]) {
                            fprintf(stderr, _("Error in input mesh, a tetrahedra have at least the same two vertices idTetra:%i vertices:%li %li %li %li"),
                                idTetra, tetraedres[idTetra].sommets.a, tetraedres[idTetra].sommets.b, tetraedres[idTetra].sommets.c, tetraedres[idTetra].sommets.d);
                            exit(1);
                        }
                    }
                }
            }
			tetraedres[idTetra].g=GetGTetra(nodes[tetraedres[idTetra].sommets.a],nodes[tetraedres[idTetra].sommets.b],nodes[tetraedres[idTetra].sommets.c],nodes[tetraedres[idTetra].sommets.d]);
			for(int idFace=0;idFace<4;idFace++)
			{
				if(tabtetra[idTetra].tetrafaces[idFace].marker>=0)
				{
					tetraedres[idTetra].faces[idFace].face_scene=&sceneMesh.pfaces[tabtetra[idTetra].tetrafaces[idFace].marker];
				}
                tetraedres[idTetra].faces[idFace].indiceSommets.set(tabtetra[idTetra].tetrafaces[idFace].vertices[0],
                                                                    tabtetra[idTetra].tetrafaces[idFace].vertices[1],
                                                                    tabtetra[idTetra].tetrafaces[idFace].vertices[2]);
				if(tabtetra[idTetra].tetrafaces[idFace].neighbor>=0)
					tetraedres[idTetra].voisins[idFace]=&tetraedres[tabtetra[idTetra].tetrafaces[idFace].neighbor];
				ivec3 cFaceSommets=tetraedres[idTetra].faces[idFace].indiceSommets;
				tetraedres[idTetra].faces[idFace].normal=FaceNormal(nodes[cFaceSommets.a],nodes[cFaceSommets.b],nodes[cFaceSommets.c]);
			}
		}

		delete[] tabtetra;
		delete[] tabNodes;
		return true;
	}else{
		std::cout<<fileName<<std::endl<<_("Tetrahedron file is empty, the calculation can't be done !")<<std::endl;
		return false;
	}
}




const char* translate( const char* strToTranslate)
{

	return strToTranslate;
}
