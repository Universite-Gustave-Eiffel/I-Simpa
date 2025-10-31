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

#include "3ds.h"
#include "last_cpp_include.hpp"
#include <string.h>

namespace format3DS
{
#define  _PRIMARY				0x4D4D	/**< Codage Hexadecimal correspondant à un type de noeud */


#define  _OBJECTINFO			0x3D3D	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _VERSION3DS			0x0002	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _EDITKEYFRAME			0xB000	/**< Codage Hexadecimal correspondant à un type de noeud */


#define  _MATERIAL				0xAFFF	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _OBJECT				0x4000 	/**< Codage Hexadecimal correspondant à un type de noeud */


#define  _MATNAME				0xA000	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _MATAMBIENT			0xA010	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _MATDIFFUSE			0xA020	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _MATSPECULAR			0xA030	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _MATSHININESS			0xA040	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _MATTRANSPARENCY		0xA050	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _MATMAP				0xA200	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _MATMAPFILE			0xA300	/**< Codage Hexadecimal correspondant à un type de noeud */

#define  _OBJECT_MESH			0x4100	/**< Codage Hexadecimal correspondant à un type de noeud */

#define  _OBJECT_VERTICES		0x4110	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _OBJECT_FACES			0x4120	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _OBJECT_MATERIAL		0x4130	/**< Codage Hexadecimal correspondant à un type de noeud */
#define  _OBJECT_UV				0x4140	/**< Codage Hexadecimal correspondant à un type de noeud */

CLoad3DS::CLoad3DS()
{
    m_TempChunk = new tChunk;
}

CLoad3DS::~CLoad3DS()
{
	delete m_TempChunk;
}

bool CLoad3DS::Import3DS(t3DModel *pModel, const char *strFileName)
{
    char strMessage[255] = {0};
    m_FilePointer=fopen(strFileName, "rb");
    if(!m_FilePointer)
		return false;
	tChunk *m_CurrentChunk = new tChunk;
    ReadChunk(m_CurrentChunk);
    if (m_CurrentChunk->ID !=  _PRIMARY)
		return false;

	printf("First chunk %i\n",m_CurrentChunk->ID);
    ProcessNextChunk(pModel, m_CurrentChunk);

	delete m_CurrentChunk;
    ComputeNormals(pModel);
    Clean();
    return true;
}

void CLoad3DS::Clean()
{
    fclose(m_FilePointer);
}



void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
    t3DObject newObject;
    tMaterialInfo newTexture;
    unsigned int version = 0;
    int buffer[50000] = {0};
    tChunk *m_CurrentChunk = new tChunk;
    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
		printf("new chunk %i\n",m_CurrentChunk->ID);
        ReadChunk(m_CurrentChunk);
        switch (m_CurrentChunk->ID)
        {
        case  _VERSION3DS:
            m_CurrentChunk->bytesRead += fread(&version, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
            break;

        case  _OBJECTINFO:
			/*
            ReadChunk(m_TempChunk);
			fseek(m_FilePointer,m_TempChunk->length - m_TempChunk->bytesRead,SEEK_CUR);
			m_TempChunk->bytesRead +=m_TempChunk->length - m_TempChunk->bytesRead;
            //m_TempChunk->bytesRead += fread(buffer, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
            m_CurrentChunk->bytesRead += m_TempChunk->bytesRead;
			*/
            ProcessNextChunk(pModel, m_CurrentChunk);
            break;

        case  _MATERIAL:
            pModel->numOfMaterials++;
            pModel->pMaterials.push_back(newTexture);

            ProcessNextMaterialChunk(pModel, m_CurrentChunk);
            break;

        case  _OBJECT:
            pModel->numOfObjects++;
            pModel->pObject.push_back(newObject);
            memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));
            m_CurrentChunk->bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
            ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrentChunk);
            break;

        case  _EDITKEYFRAME:
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
            break;

        default:
            m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
            break;
        }
        pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
    }
    delete m_CurrentChunk;
}

void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
    int buffer[50000] = {0};
    tChunk *m_CurrentChunk = new tChunk;

    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        ReadChunk(m_CurrentChunk);
        switch (m_CurrentChunk->ID)
        {
        case  _OBJECT_MESH:
            ProcessNextObjectChunk(pModel, pObject, m_CurrentChunk);
            break;

        case  _OBJECT_VERTICES:
            ReadVertices(pObject, m_CurrentChunk);
            break;

        case  _OBJECT_FACES:
            ReadVertexIndices(pObject, m_CurrentChunk);
            break;

        case  _OBJECT_MATERIAL:
            ReadObjectMaterial(pModel, pObject, m_CurrentChunk);
            break;

        case  _OBJECT_UV:
            ReadUVCoordinates(pObject, m_CurrentChunk);
            break;
        default:
			if(m_CurrentChunk->length>6)
			{
				fseek(m_FilePointer,m_CurrentChunk->length - m_CurrentChunk->bytesRead,SEEK_CUR);
				m_CurrentChunk->bytesRead += m_CurrentChunk->length - m_CurrentChunk->bytesRead;
			}
            break;
        }
        pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
    }
    delete m_CurrentChunk;
}

void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
    int buffer[50000] = {0};
    tChunk *m_CurrentChunk = new tChunk;

    while (pPreviousChunk->bytesRead < pPreviousChunk->length)
    {
        ReadChunk(m_CurrentChunk);
        switch (m_CurrentChunk->ID)
        {
        case  _MATNAME:
            m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
            break;

        case  _MATAMBIENT:
            ReadColorChunk((pModel->pMaterials[pModel->numOfMaterials - 1].colorA), m_CurrentChunk);
			break;

		case  _MATDIFFUSE:
            ReadColorChunk((pModel->pMaterials[pModel->numOfMaterials - 1].colorD), m_CurrentChunk);
			break;

		case  _MATSPECULAR:
            ReadColorChunk((pModel->pMaterials[pModel->numOfMaterials - 1].colorS), m_CurrentChunk);
			break;

		case  _MATSHININESS:
            ReadColorChunk((pModel->pMaterials[pModel->numOfMaterials - 1].Shininess), m_CurrentChunk);
			break;

		case  _MATTRANSPARENCY:
            ReadColorChunk((pModel->pMaterials[pModel->numOfMaterials - 1].Transparency), m_CurrentChunk);
			break;

        case  _MATMAP:
            ProcessNextMaterialChunk(pModel, m_CurrentChunk);
            break;

        case  _MATMAPFILE:
            m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
            break;

        default:
            m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
            break;
        }
        pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
    }
    delete m_CurrentChunk;
}

void CLoad3DS::ReadChunk(tChunk *pChunk)
{
    pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);
    pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

int CLoad3DS::GetString(char *pBuffer)
{
    int index = 0;
    fread(pBuffer, 1, 1, m_FilePointer);
    while (*(pBuffer + index++) != 0)
	{
        fread(pBuffer + index, 1, 1, m_FilePointer);
    }
    return strlen(pBuffer) + 1;
}



void CLoad3DS::ReadColorChunk(unsigned char *Color, tChunk *pChunk)
{
    ReadChunk(m_TempChunk);

    m_TempChunk->bytesRead += fread(Color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
    pChunk->bytesRead += m_TempChunk->bytesRead;
}

void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
    unsigned short index = 0;
	int j;
    pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);
    pObject->pFaces = new tFace [pObject->numOfFaces];
    memset(&*pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);
    for(int i = 0; i < pObject->numOfFaces; i++)
    {
        for(j = 0; j < 4; j++)
        {
            pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

            if(j < 3)
            {
                pObject->pFaces[i].vertIndex[j] = index;
			}else{
				//Lecture de Diff AB BC CA

				index=index & 0x000F;
				pObject->pFaces[i].diff[0]=(index & 0x0004) >> 2;
				pObject->pFaces[i].diff[1]=(index & 0x0002) >> 1;
				pObject->pFaces[i].diff[2]=(index & 0x0001);
			}
        }
    }
}


void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
    pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);
    pObject->pTexVerts = new vec2 [pObject->numTexVertex];
    pPreviousChunk->bytesRead += fread(*pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{

    pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);
    pObject->pVerts = new vec3 [pObject->numOfVerts];
    memset(*pObject->pVerts, 0, sizeof(vec3) * pObject->numOfVerts);
    pPreviousChunk->bytesRead += fread(*pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
    for(int i = 0; i < pObject->numOfVerts; i++)
    {

        float fTempY = pObject->pVerts[i].y;
        pObject->pVerts[i].y = pObject->pVerts[i].z;
        pObject->pVerts[i].z = -fTempY;

    }
}

void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
    char strMaterial[255] = {0};
    int buffer[50000] = {0};
	short nbrFace;
	short idFace;
    pPreviousChunk->bytesRead += GetString(strMaterial);
    for(int i = 0; i < pModel->numOfMaterials; i++)
    {
        if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
        {
			//Pour chaque materiel, on y trouve la liste des faces qui ont cette matière
			pPreviousChunk->bytesRead += fread(&nbrFace, 1, sizeof(nbrFace), m_FilePointer);
			for(short f=0;f<nbrFace;f++)
			{
				pPreviousChunk->bytesRead += fread(&idFace, 1, sizeof(idFace), m_FilePointer);
				if(idFace>=0)
					pObject->pFaces[idFace].materialID=i;
			}
			if(!pObject->bHasmaterialID)
			{
				pObject->materialID = i;
				pObject->bHasmaterialID = true;
			}
            if(strlen(pModel->pMaterials[i].strFile) > 0)
			{
                pObject->bHasTexture = true;
            }
            break;
        }
        else if(!pObject->bHasmaterialID)
        {
            pObject->materialID = -1;
        }
    }
    pPreviousChunk->bytesRead += fread(buffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
    vec3 vVector1, vVector2, vNormal, vPoly[3];
	int i,j;

    if(pModel->numOfObjects <= 0)
        return;
    for(int index = 0; index < pModel->numOfObjects; index++)
    {
		this->ObjCalcNormals(&(pModel->pObject[index]));
    }
}




void CLoad3DS::ObjCalcNormals(t3DObject *p_object)
{
	int i;
	vec3 l_vect1,l_vect2,l_vect3,vVector1,vVector2,l_normal;  //Some local vectors
	smart_ptr_ar<int> l_connections_qty= new int[p_object->numOfVerts]; //Number of poligons around each vertex

    // Resetting connection array
	//ZeroMemory(&(*l_connections_qty),sizeof(int) * p_object->numOfVerts);
	memset(&(*l_connections_qty),0,sizeof(int) * p_object->numOfVerts);

    p_object->pNormals = new vec3 [p_object->numOfVerts];

	for (i=0; i<p_object->numOfFaces; i++)
	{
		int a=p_object->pFaces[i].vertIndex[0];
		int b=p_object->pFaces[i].vertIndex[1];
		int c=p_object->pFaces[i].vertIndex[2];

		l_vect1 = p_object->pVerts[a];
        l_vect2 = p_object->pVerts[b];
        l_vect3 = p_object->pVerts[c];

        Vector(l_vect1, l_vect2,vVector1);
        Vector(l_vect2, l_vect3,vVector2);

        Cross(vVector1, vVector2,l_normal);

		//face normal
		p_object->pFaces[i].faceNormals=l_normal;
		p_object->pFaces[i].faceNormals.normalize();

		l_connections_qty[a]+=1; // For each vertex shared by this polygon we increase the number of connections
		l_connections_qty[b]+=1;
		l_connections_qty[c]+=1;

		p_object->pNormals[a].x+=l_normal.x;
		p_object->pNormals[a].y+=l_normal.y;
		p_object->pNormals[a].z+=l_normal.z;
		p_object->pNormals[b].x+=l_normal.x;
		p_object->pNormals[b].y+=l_normal.y;
		p_object->pNormals[b].z+=l_normal.z;
		p_object->pNormals[c].x+=l_normal.x;
		p_object->pNormals[c].y+=l_normal.y;
		p_object->pNormals[c].z+=l_normal.z;

	}

    for (i=0; i<p_object->numOfVerts; i++)
	{
		if (l_connections_qty[i]>0)
		{
			p_object->pNormals[i].x /= l_connections_qty[i]; // Let's now average the polygons' normals to obtain the vertex normal!
			p_object->pNormals[i].y /= l_connections_qty[i];
			p_object->pNormals[i].z /= l_connections_qty[i];
			p_object->pNormals[i].normalize();
		}
	}
}










} //namespace
