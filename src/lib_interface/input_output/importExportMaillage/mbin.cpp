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

#include "mbin.h"
#include <fstream>
#include "Core/mathlib.h"
#include "coreTypes.h"

namespace formatMBIN
{


	/**
	 * @brief Structure d'entete du fichier
	 */
	struct t_FileHeader
	{
		Longb quantTetra;
		Longb quantNodes;
	};
trimeshmodel CMBIN::LoadMesh(const char *strFileName)
{
	trimeshmodel modelret;
	bintetrahedre *tabTetra=NULL;
	t_binNode *tabNodes=NULL;
	unsigned int sizeTetra=0;
	unsigned int sizeNodes=0;
	if(ImportBIN(strFileName,&tabTetra,&tabNodes,sizeTetra,sizeNodes))
	{
		modelret.nodes.reserve(sizeNodes);
		for( unsigned int idnode=0;idnode<sizeNodes;idnode++)
			modelret.nodes.push_back(tabNodes[idnode]);
		modelret.tetrahedres.reserve(sizeTetra);
		for( unsigned int idtetra=0;idtetra<sizeTetra;idtetra++)
			modelret.tetrahedres.push_back(tabTetra[idtetra]);
	}
	delete[] tabTetra;
	delete[] tabNodes;
	return modelret;
}

bool CMBIN::SaveMesh(const char *strFileName,trimeshmodel& trimesh)
{
	unsigned int sizeTetra=trimesh.tetrahedres.size();
	unsigned int sizeNodes=trimesh.nodes.size();
	bintetrahedre *tabTetra=new bintetrahedre[sizeTetra];
	t_binNode *tabNodes=new t_binNode[sizeNodes];

	for( unsigned int idnode=0;idnode<sizeNodes;idnode++)
		tabNodes[idnode]=trimesh.nodes[idnode];
	for( unsigned int idtetra=0;idtetra<sizeTetra;idtetra++)
		tabTetra[idtetra]=trimesh.tetrahedres[idtetra];

	bool result = ExportBIN(strFileName,tabTetra,tabNodes,sizeTetra,sizeNodes);

	delete[] tabTetra;
	delete[] tabNodes;

    return result;
}

bool CMBIN::ExportBIN(const char *strFileName,const bintetrahedre* tabTetra,const t_binNode* tabNodes,unsigned int sizeTetra,unsigned int sizeNodes)
{
	//Namespace
	using namespace std;

	//Declarations

	//Sauvegarde du mod�le 3D
	fstream binFile (strFileName, ios::out | ios::binary);

	//*************************
	//Ecriture de l'entete du fichier
	t_FileHeader fileHeader;
	fileHeader.quantNodes=sizeNodes;
	fileHeader.quantTetra=sizeTetra;
	binFile.write((char*)&fileHeader.quantTetra,sizeof(Longb));
	binFile.write((char*)&fileHeader.quantNodes, sizeof(Longb));
	//*************************
	//Ecriture des noeuds
	for (int idNode = 0; idNode<sizeNodes; idNode++) {
		binFile.write((char*)&(tabNodes[idNode]).node[0], sizeof(Floatb));
		binFile.write((char*)&(tabNodes[idNode]).node[1], sizeof(Floatb));
		binFile.write((char*)&(tabNodes[idNode]).node[2], sizeof(Floatb));		
	}

    // Check tetrahedra
    for(int idTetra = 0; idTetra < sizeTetra; idTetra++) {
        const bintetrahedre& tetra = tabTetra[idTetra];
        for (int idvertLeft = 0; idvertLeft < 4; idvertLeft++) {
            for (int idvertRight = 0; idvertRight < 4; idvertRight++) {
                if (idvertLeft != idvertRight) {
                    if (tetra.vertices[idvertLeft] == tetra.vertices[idvertRight]) {
                        fprintf(stderr, _("Error in input mesh, a tetrahedra have at least the same two vertices idTetra:%i vertices:%li %li %li %li"),
                            idTetra, tetra.vertices[0], tetra.vertices[1], tetra.vertices[2], tetra.vertices[3]);
                        return false;
                    }
                }
            }
        }
    }


    //*************************
	//Write tetrahedra
	for (int idTetra = 0; idTetra < sizeTetra; idTetra++) {
		const bintetrahedre& tetra = tabTetra[idTetra];
		binFile.write((char*)&tetra.vertices[0], sizeof(Intb));
		binFile.write((char*)&tetra.vertices[1], sizeof(Intb));
		binFile.write((char*)&tetra.vertices[2], sizeof(Intb));
		binFile.write((char*)&tetra.vertices[3], sizeof(Intb));
		binFile.write((char*)&tetra.idVolume, sizeof(Intb));
		for (int idFace = 0; idFace < 4; idFace++) {
			const bintetraface& face = tetra.tetrafaces[idFace];
			binFile.write((char*)&face.vertices[0], sizeof(Intb));
			binFile.write((char*)&face.vertices[1], sizeof(Intb));
			binFile.write((char*)&face.vertices[2], sizeof(Intb));
			binFile.write((char*)&face.marker, sizeof(Intb));
			binFile.write((char*)&face.neighbor, sizeof(Intb));
		}
	}
	binFile.close();
	return true;
}

bool  CMBIN::ImportBIN(const char *strFileName,bintetrahedre **tabTetra,t_binNode **tabNodes,unsigned int &sizeTetra,unsigned int &sizeNodes)
{
	//Namespace
	using namespace std;

	//Declarations

	//Sauvegarde du mod�le 3D
    fstream binFile (strFileName, ios::in | ios::binary);

	if(binFile.is_open())
	{
		//*************************
		//Lecture de l'entete du fichier
		t_FileHeader fileHeader;
		binFile.read((char*)&fileHeader.quantTetra,sizeof(Longb));
		binFile.read((char*)&fileHeader.quantNodes, sizeof(Longb));
		sizeNodes=fileHeader.quantNodes;
		sizeTetra=fileHeader.quantTetra;

		//*************************
		// Read nodes
		*tabNodes=new t_binNode[sizeNodes];
		for(int idNode=0; idNode<sizeNodes; idNode++) {
			binFile.read((char*)&((*tabNodes)[idNode].node[0]), sizeof(Floatb));
			binFile.read((char*)&((*tabNodes)[idNode].node[1]), sizeof(Floatb));
			binFile.read((char*)&((*tabNodes)[idNode].node[2]), sizeof(Floatb));
		}		
		//*************************
		// Read tetrahedras
		*tabTetra=new bintetrahedre[sizeTetra];
		for(int idTetra = 0; idTetra < sizeTetra; idTetra++) {
			bintetrahedre& tetra = (*tabTetra)[idTetra];
			binFile.read((char*)&(tetra.vertices[0]), sizeof(Intb));
			binFile.read((char*)&(tetra.vertices[1]), sizeof(Intb));
			binFile.read((char*)&(tetra.vertices[2]), sizeof(Intb));
			binFile.read((char*)&(tetra.vertices[3]), sizeof(Intb));
			binFile.read((char*)&(tetra.idVolume), sizeof(Intb));
			for(int idFace = 0; idFace < 4; idFace++) {
				bintetraface& face = tetra.tetrafaces[idFace];
				binFile.read((char*)&(face.vertices[0]), sizeof(Intb));
				binFile.read((char*)&(face.vertices[1]), sizeof(Intb));
				binFile.read((char*)&(face.vertices[2]), sizeof(Intb));
				binFile.read((char*)&(face.marker), sizeof(Intb));
				binFile.read((char*)&(face.neighbor), sizeof(Intb));				
			}
		}
		binFile.close();
		return true;
	}else{
		return false;
	}
}

 double CMBIN::ComputeVolume(trimeshmodel& trimesh)
 {
	double res=0;
	std::size_t sizeTetra=trimesh.tetrahedres.size();
	for( std::size_t idtetra=0;idtetra<sizeTetra;idtetra++)
	{
		ivec4 isommets((const long *)(trimesh.tetrahedres[idtetra].vertices));
		vec3 sommets[4];
		for(short i=0;i<4;i++)
			sommets[i]=trimesh.nodes[isommets[i]].node;
		res+=CalcTetraVolume(sommets[0],sommets[1],sommets[2],sommets[3]);
	}
	return res;
 }
} //fin namespace
