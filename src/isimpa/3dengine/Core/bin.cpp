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

#include "bin.h"
#include "Material.h"
#include "last_cpp_include.hpp"

extern CMaterial *materials;
extern CTexture *textures;

namespace formatBIN
{
 const int majorVersionFormat=1;
 const int minorVersionFormat=2;
CformatBIN::CformatBIN()
{

}

CformatBIN::~CformatBIN()
{

}

bool CformatBIN::ProcessNodeTexCoords(std::fstream &binFile,t3DModel *pModel)
{
	binaryTexCoords elementNode={0};
	binaryTexCoord vertexElement;
	if(!binFile.is_open() || binFile.eof())
		return false;
	binFile.read((char*)&elementNode, sizeof (binaryTexCoords));
	pModel->numOfTexCoords=elementNode.nbTexCoords;
	pModel->pTexVerts = new vec2[elementNode.nbTexCoords];
	for(unsigned long v=0;v<elementNode.nbTexCoords;v++)
	{
		binFile.read((char*)&vertexElement, sizeof (binaryTexCoord));
		pModel->pTexVerts[v].set(vertexElement.x,vertexElement.y);
	}
	return true;
}

bool CformatBIN::ProcessNodeVertices(std::fstream &binFile,t3DModel *pModel)
{
	binaryVertices elementNode={0};
	binaryVertex vertexElement;
	if(!binFile.is_open() || binFile.eof())
		return false;
	binFile.read((char*)&elementNode, sizeof (binaryVertices));
	pModel->numOfVerts=elementNode.nbVertex;
	pModel->pVerts = new vec3[elementNode.nbVertex];
	for(unsigned long v=0;v<elementNode.nbVertex;v++)
	{
		binFile.read((char*)&vertexElement, sizeof (binaryVertex));
		pModel->pVerts[v].set(vertexElement.x,vertexElement.z,-vertexElement.y);
	}
	return true;
}

bool CformatBIN::ProcessNodeMaterialv100(std::fstream &binFile,t3DModel *pModel)
{
	binaryMaterialv100 elementNode;
	binFile.read((char*)&elementNode, sizeof (binaryMaterialv100));
	if(elementNode.shine<0 || elementNode.shine>255)
		return false;

	pModel->pMaterials.push_back(tMaterialInfo());
	tMaterialInfo* nvmatiere=&pModel->pMaterials[pModel->pMaterials.size()-1];
	strcpy(nvmatiere->strName,elementNode.materialName);
	memset(nvmatiere->strFile,0,sizeof(nvmatiere->strFile));
	nvmatiere->colorA=GetVectorFromColor(elementNode.colorA);
	nvmatiere->colorD=GetVectorFromColor(elementNode.colorB);
	nvmatiere->colorS=GetVectorFromColor(elementNode.colorC);
	nvmatiere->Shininess=elementNode.shine;
	nvmatiere->Transparency=elementNode.transparency;
	return true;

}
bool CformatBIN::ProcessNodeMaterial(std::fstream &binFile,t3DModel *pModel)
{
	binaryMaterial elementNode;
	binFile.read((char*)&elementNode, sizeof (binaryMaterial));
	if(elementNode.shine<0 || elementNode.shine>255)
		return false;

	pModel->pMaterials.push_back(tMaterialInfo());
	tMaterialInfo* nvmatiere=&pModel->pMaterials[pModel->pMaterials.size()-1];
	strcpy(nvmatiere->strName,elementNode.materialName);
	strcpy(nvmatiere->strFile,elementNode.materialFileName);
	nvmatiere->colorA=GetVectorFromColor(elementNode.colorA);
	nvmatiere->colorD=GetVectorFromColor(elementNode.colorB);
	nvmatiere->colorS=GetVectorFromColor(elementNode.colorC);
	nvmatiere->Shininess=elementNode.shine;
	nvmatiere->Transparency=elementNode.transparency;
	return true;
}

bool CformatBIN::ProcessNodeGroup(std::fstream &binFile,t3DModel *pModel)
{
	binaryGroup elementNode;
	binaryFace faceElement;
	if(!binFile.is_open() || binFile.eof())
		return false;
	binFile.read((char*)&elementNode, sizeof (binaryGroup));
	t3DObject nvObjet={0};
	strcpy(nvObjet.strName,elementNode.groupName);
	nvObjet.numOfFaces=elementNode.nbFace;
	nvObjet.pFaces=new tFace[elementNode.nbFace];

	for(unsigned long f=0;f<elementNode.nbFace;f++)
	{
		binFile.read((char*)&faceElement, sizeof (binaryFace));
		nvObjet.pFaces[f].vertIndex[0]=faceElement.a;
		nvObjet.pFaces[f].vertIndex[1]=faceElement.b;
		nvObjet.pFaces[f].vertIndex[2]=faceElement.c;
		nvObjet.pFaces[f].diff[0] =faceElement.ab;
		nvObjet.pFaces[f].diff[1] =faceElement.bc;
		nvObjet.pFaces[f].diff[2] =faceElement.ca;
		nvObjet.pFaces[f].materialID=faceElement.idMaterial;
		nvObjet.pFaces[f].vertexTex=faceElement.coordtex;
		nvObjet.pFaces[f].internalFace=faceElement.internalFace;
	}
	pModel->pObject.push_back(nvObjet);
	return true;
}

bool CformatBIN::ProcessNodeGroupV110(std::fstream &binFile,t3DModel *pModel)
{
	binaryGroup elementNode;
	binaryFace faceElement;
	if(!binFile.is_open() || binFile.eof())
		return false;
	binFile.read((char*)&elementNode, sizeof (binaryGroup));
	t3DObject nvObjet={0};
	strcpy(nvObjet.strName,elementNode.groupName);
	nvObjet.numOfFaces=elementNode.nbFace;
	nvObjet.pFaces=new tFace[elementNode.nbFace];

	for(unsigned long f=0;f<elementNode.nbFace;f++)
	{
		binFile.read((char*)&faceElement, sizeof (binaryFace110));
		nvObjet.pFaces[f].vertIndex[0]=faceElement.a;
		nvObjet.pFaces[f].vertIndex[1]=faceElement.b;
		nvObjet.pFaces[f].vertIndex[2]=faceElement.c;
		nvObjet.pFaces[f].diff[0] =faceElement.ab;
		nvObjet.pFaces[f].diff[1] =faceElement.bc;
		nvObjet.pFaces[f].diff[2] =faceElement.ca;
		nvObjet.pFaces[f].materialID=faceElement.idMaterial;
		nvObjet.pFaces[f].vertexTex=faceElement.coordtex;
	}
	pModel->pObject.push_back(nvObjet);
	return true;
}
bool CformatBIN::ProcessNodeGroupv100(std::fstream &binFile,t3DModel *pModel)
{
	binaryGroup elementNode;
	binaryFace100 faceElement;
	if(!binFile.is_open() || binFile.eof())
		return false;
	binFile.read((char*)&elementNode, sizeof (binaryGroup));
	t3DObject nvObjet={0};
	strcpy(nvObjet.strName,elementNode.groupName);
	nvObjet.numOfFaces=elementNode.nbFace;
	nvObjet.pFaces=new tFace[elementNode.nbFace];

	for(unsigned long f=0;f<elementNode.nbFace;f++)
	{
		binFile.read((char*)&faceElement, sizeof (binaryFace100));
		nvObjet.pFaces[f].vertIndex[0]=faceElement.a;
		nvObjet.pFaces[f].vertIndex[1]=faceElement.b;
		nvObjet.pFaces[f].vertIndex[2]=faceElement.c;
		nvObjet.pFaces[f].diff[0] =faceElement.ab;
		nvObjet.pFaces[f].diff[1] =faceElement.bc;
		nvObjet.pFaces[f].diff[2] =faceElement.ca;
		nvObjet.pFaces[f].materialID=faceElement.idMaterial;
	}
	pModel->pObject.push_back(nvObjet);
	return true;
}
bool CformatBIN::ProcessNode(std::fstream &binFile,t3DModel *pModel,const binaryHeader& fileHeader)
{ //Lecture d'un noeud du fichier binaire
	binaryNode elementNode;
	elementNode.nextBrother=1;
	while(!binFile.eof() && elementNode.nextBrother != 0l)
	{
		binFile.read((char*)&elementNode, sizeof (binaryNode));
		switch(elementNode.nodeType)
		{
			case NODE_TYPE_VERTICES:
				this->ProcessNodeVertices(binFile,pModel);
				break;
			case NODE_TYPE_TEXCOORDS:
				this->ProcessNodeTexCoords(binFile,pModel);
				break;
			case NODE_TYPE_MATERIAL:
				if(fileHeader.majorVersion==1 && fileHeader.minorVersion==0)
					this->ProcessNodeMaterialv100(binFile,pModel);
				else
					this->ProcessNodeMaterial(binFile,pModel);
				break;
			case NODE_TYPE_GROUP:
				if(fileHeader.majorVersion==1 && fileHeader.minorVersion==1)
					this->ProcessNodeGroupV110(binFile,pModel);
				else if(fileHeader.majorVersion==1 && fileHeader.minorVersion==0)
					this->ProcessNodeGroupv100(binFile,pModel);
				else
					this->ProcessNodeGroup(binFile,pModel);
				break;
			default:
				return false;
				break;
		}
		if(elementNode.nextBrother != 0)
			binFile.seekg(elementNode.nextBrother);
	}

	return true;
}

bool CformatBIN::ImportBIN(t3DModel *pModel, const std::string& strFileName)
{
	using namespace std;
    fstream binFile (strFileName, ios::in | ios::binary);
    if(!binFile.is_open())
		return false;

	binaryHeader enteteFichier;
	binFile.read((char*)&enteteFichier, sizeof (binaryHeader));
	//La version du format se trouve dans enteteFichier
	this->ProcessNode(binFile,pModel,enteteFichier);

	binFile.close();
	ComputeNormals(pModel);
    Clean();
    return true;
}

void CformatBIN::writeNode(std::fstream &binFile,bShort nodeType,bLong nodeHeadSize,bLong nodeSize)
{
	binaryNode currentNode;
	currentNode.nodeType=nodeType;
	currentNode.firtSon= (bLong) binFile.tellp() + nodeHeadSize;
	currentNode.nextBrother= (bLong) binFile.tellp() + nodeSize;
	if(nodeHeadSize==0)
		currentNode.firtSon=0;
	if(nodeSize==0)
		currentNode.nextBrother=0;
	binFile.write((char*)&currentNode,sizeof(binaryNode));
}

binaryColor CformatBIN::GetColorFromVector(vec4 colorInfo)
{
	binaryColor bColor={colorInfo.x,colorInfo.y,colorInfo.z,colorInfo.w};
	return bColor;
}

vec4 CformatBIN::GetVectorFromColor(binaryColor colorInfo)
{
	vec4 vColor(colorInfo.red,colorInfo.green,colorInfo.blue,colorInfo.transparency);
	return vColor;
}

bool CformatBIN::ExportBIN(const std::string& strFileName,vec4 UnitizeVar,std::vector<vec3> &_pVertices,std::vector<vec2> &_pTexCoords,std::vector<SGroup3D> &_pGroups)
{
	//Debug, test size des structures, pertes d'octets a cause d'alignement de type
	//unsigned long binaryFaceSize=sizeof(binaryFace);
	//unsigned long binaryFaceSizeOther=sizeof(bLong)+3*sizeof(bInt)+3*sizeof(bBool)+sizeof(bShort);

	//Namespace
	using namespace std;
	//Initialisations
	unsigned long nbVertex=_pVertices.size();

	//Declarations
	unsigned long sizeVERTICES=(nbVertex*sizeof(binaryVertex))+sizeof(binaryNode)+sizeof(binaryVertices);
	unsigned long sizeTEX=(_pTexCoords.size()*sizeof(binaryTexCoord))+sizeof(binaryNode)+sizeof(binaryTexCoords);
	vector<TextureInfo> v_textures(textures->GetTextures());
	vector<SMaterial> v_materials(materials->GetMaterials());
	//Sauvegarde du modèle 3D
	fstream binFile (strFileName, ios::out | ios::binary);

	//*************************
	//Ecriture de l'entete du fichier
	binaryHeader fileHeader;
	fileHeader.majorVersion=majorVersionFormat;
	fileHeader.minorVersion=minorVersionFormat;
	binFile.write((char*)&fileHeader,sizeof(binaryHeader));

	//*************************
	//Ecriture du noeud VERTICE
	binaryVertices verticeGroup={0};
	//Compte le nombre de vertex
	verticeGroup.nbVertex=_pVertices.size();
	this->writeNode(binFile,NODE_TYPE_VERTICES,0,sizeVERTICES);
	binFile.write((char*)&verticeGroup,sizeof(binaryVertices));
	//Ecriture des vertices
	for(unsigned long v=0; v < _pVertices.size(); v++)
	{
		vec3 realCoords=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[v]);
		binaryVertex currentVertex={realCoords.x,realCoords.y,realCoords.z};
		binFile.write((char*)&currentVertex,sizeof(binaryVertex));
	}

	//*************************
	//Ecriture du noeud TEXCOORDS
	binaryTexCoords txGroup={0};
	//Compte le nombre de vertex
	txGroup.nbTexCoords=_pTexCoords.size();
	this->writeNode(binFile,NODE_TYPE_TEXCOORDS,0,sizeTEX);
	binFile.write((char*)&txGroup,sizeof(binaryTexCoords));
	//Ecriture des coordonnées de textures
	for(unsigned long v=0; v < _pTexCoords.size(); v++)
	{
		binaryTexCoord currentCoord={_pTexCoords[v].x,_pTexCoords[v].y};
		binFile.write((char*)&currentCoord,sizeof(binaryTexCoord));
	}
	//*************************
	//Ecriture du noeud GROUP
	//for(int g=0; g < _pGroups.size(); g++)
	int g=0;
	for(std::vector<SGroup3D>::iterator itgroup=_pGroups.begin();itgroup!=_pGroups.end();itgroup++)
	{
		binaryGroup groupNode;
		//ZeroMemory(groupNode.groupName,255);
		memset(groupNode.groupName,0,sizeof(char)*255);
		strcpy(groupNode.groupName,(*itgroup).Name.substr(0,254).c_str());
		groupNode.nbFace= (bInt) (*itgroup).pFaces.size();
		//Calcul de la taille du groupe en octets
		unsigned long sizeGROUP=0;
		if(v_materials.size()>0 || g < _pGroups.size() - 1 )
			sizeGROUP=sizeof(binaryNode)+sizeof(binaryGroup)+(groupNode.nbFace*sizeof(binaryFace));
		//Ecriture dans le fichier
		this->writeNode(binFile,NODE_TYPE_GROUP,0,sizeGROUP);
		binFile.write((char*)&groupNode,sizeof(binaryGroup));
		//for(long f=0; f < (*itgroup).pFaces.size() ;f++)
		for(std::vector<SFace3D>::iterator itface=(*itgroup).pFaces.begin();itface!=(*itgroup).pFaces.end();itface++)
		{
			binaryFace faceElement={
				(*itface).Vertices.a,
				(*itface).Vertices.b,
				(*itface).Vertices.c,
                (bShort) (*itface).idMaterial,
				(*itface).Diff[0],
				(*itface).Diff[1],
				(*itface).Diff[2],
				(*itface).internalFace,
				{(*itface).TexCoords.a,
				(*itface).TexCoords.b,
				(*itface).TexCoords.c
				}
			};
			binFile.write((char*)&faceElement,sizeof(binaryFace));
		}
		g++;
	}

	//*************************
	//Ecriture du noeud MATERIAL

	for(int m=0;m<v_materials.size();m++)
	{
		//Ecriture d'une matière
		binaryMaterial materialNode={};
		//memset(materialNode.materialName,0,sizeof(materialNode.materialName));
		//strcpy_s(materialNode.materialName,255,v_materials[m].Name.c_str());
		strcpy(materialNode.materialName,v_materials[m].Name.c_str());
		memset(materialNode.materialFileName,0,sizeof(materialNode.materialFileName));
		if(v_materials[m].idTex!=-1)
			strcpy(materialNode.materialFileName,v_textures[v_materials[m].idTex].name.c_str());
		materialNode.colorA=GetColorFromVector(v_materials[m].ColorA);
		materialNode.colorB=GetColorFromVector(v_materials[m].ColorD);
		materialNode.colorC=GetColorFromVector(v_materials[m].ColorS);
		materialNode.shine=v_materials[m].Shininess;
		materialNode.transparency=v_materials[m].Transparency;
		bLong nextMaterial=0;
		if(m<v_materials.size()-1)
			nextMaterial=sizeof(binaryNode)+sizeof(binaryMaterial);
		this->writeNode(binFile,NODE_TYPE_MATERIAL,0,nextMaterial);
		binFile.write((char*)&materialNode,sizeof(binaryMaterial));
	}

	binFile.close();

	return true;
}


void CformatBIN::Clean()
{

}


void CformatBIN::ComputeNormals(t3DModel *pModel)
{
	this->ObjCalcNormals(pModel);
}

void CformatBIN::ObjCalcNormals(t3DModel *p_model)
{
	int i;
	vec3 l_vect1,l_vect2,l_vect3,vVector1,vVector2,l_normal;  //Some local vectors
	smart_ptr_ar<int> l_connections_qty= new int[p_model->numOfVerts]; //Number of poligons around each vertex

    // Resetting connection array
	//ZeroMemory(&(*l_connections_qty),sizeof(int) * p_model->numOfVerts);
	memset(l_connections_qty.get(),0,sizeof(int) * p_model->numOfVerts);

    p_model->pNormals = new vec3 [p_model->numOfVerts];

	for(int o=0;o<p_model->pObject.size();o++)
	{
		t3DObject* p_object=&p_model->pObject[o];
		for (i=0; i<p_object->numOfFaces; i++)
		{
			int a=p_object->pFaces[i].vertIndex[0];
			int b=p_object->pFaces[i].vertIndex[1];
			int c=p_object->pFaces[i].vertIndex[2];

			l_vect1 = p_model->pVerts[a];
			l_vect2 = p_model->pVerts[b];
			l_vect3 = p_model->pVerts[c];

			Vector(l_vect1, l_vect2,vVector1);
			Vector(l_vect2, l_vect3,vVector2);

			Cross(vVector1, vVector2,l_normal);

			//face normal
			p_object->pFaces[i].faceNormals=l_normal;
			p_object->pFaces[i].faceNormals.normalize();

			l_connections_qty[a]+=1; // For each vertex shared by this polygon we increase the number of connections
			l_connections_qty[b]+=1;
			l_connections_qty[c]+=1;

			p_model->pNormals[a].x+=l_normal.x;
			p_model->pNormals[a].y+=l_normal.y;
			p_model->pNormals[a].z+=l_normal.z;
			p_model->pNormals[b].x+=l_normal.x;
			p_model->pNormals[b].y+=l_normal.y;
			p_model->pNormals[b].z+=l_normal.z;
			p_model->pNormals[c].x+=l_normal.x;
			p_model->pNormals[c].y+=l_normal.y;
			p_model->pNormals[c].z+=l_normal.z;

		}
	}
	for (i=0; i<p_model->numOfVerts; i++)
	{
		if (l_connections_qty[i]>0)
		{
			p_model->pNormals[i].x /= l_connections_qty[i]; // Let's now average the polygons' normals to obtain the vertex normal!
			p_model->pNormals[i].y /= l_connections_qty[i];
			p_model->pNormals[i].z /= l_connections_qty[i];
			p_model->pNormals[i].normalize();
		}
	}
}










} //namespace
