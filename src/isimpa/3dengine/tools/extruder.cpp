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

#include "extruder.h"
#include "wx/log.h"
#include "last_cpp_include.hpp"

namespace ui_tools
{
	Extruder::Extruder()
	{

	}
	Extruder::~Extruder( )
	{


	}
	void Extruder::GetFaceToDelete(	std::vector<t_faceIndex> retFaces,std::vector<vec3> &_pVertices,std::vector<SGroup3D> &_pGroups,IntersectionSeeker& intersectionTool,std::vector<t_faceIndex> &volumeExtrusion)
	{
		t_faceIndex tmpIndex;
		if(volumeExtrusion.size()==0)
			return;

		tmpIndex=volumeExtrusion[0];
		vec3 Lo = _pVertices[_pGroups[tmpIndex.g].pFaces[tmpIndex.f].Vertices.a];
		vec3 Hi = _pVertices[_pGroups[tmpIndex.g].pFaces[tmpIndex.f].Vertices.a];
		//_________________________________________________________________
		//1ere étape on doit créer une boite autour de l'élément à extraire
		for(int i=0;i<volumeExtrusion.size();i++)
		{
			int vertices[3];
			vertices[0]=_pGroups[tmpIndex.g].pFaces[tmpIndex.f].Vertices.a;
			vertices[1]=_pGroups[tmpIndex.g].pFaces[tmpIndex.f].Vertices.b;
			vertices[2]=_pGroups[tmpIndex.g].pFaces[tmpIndex.f].Vertices.c;
			for(int vi = 0; vi < 3 ;vi++) 
			{
				int v=vertices[vi];
				if(_pVertices[v].x < Lo.x) Lo.x = _pVertices[v].x;
				if(_pVertices[v].y < Lo.y) Lo.y = _pVertices[v].y;
				if(_pVertices[v].z < Lo.z) Lo.z = _pVertices[v].z;
				if(_pVertices[v].x > Hi.x) Hi.x = _pVertices[v].x;
				if(_pVertices[v].y > Hi.y) Hi.y = _pVertices[v].y;
				if(_pVertices[v].z > Hi.z) Hi.z = _pVertices[v].z;
			}			
		}
		//_________________________________________________________________
		//2eme étape on lance le test de collision entre notre boite et le modèle 3D
		retFaces;
		intersectionTool.GetCollisionLst(Lo,Hi,retFaces);

		return;
	}

	bool Extruder::DoExtrusion(IntersectionSeeker& intersectionTool,
		std::vector<vec3> &_pVertices,				//Entree
		std::vector<SGroup3D> &_pGroups,			//Entree&Sortie (les faces sont supprime et ajouté ici)
		std::vector<t_faceIndex> &volumeExtrusion	//Entree contient indice des faces de l'extrusion
		)
	{
		//1ere étape on recupere la liste des éléments à supprimer (triangles en collision avec notre volume)
		std::vector<t_faceIndex> faceToDestroy;
		GetFaceToDelete(faceToDestroy,_pVertices,_pGroups,intersectionTool,volumeExtrusion);
		
		if(faceToDestroy.size()==0)
			return true;  // Finalement l'objet est flottant, ce n'est pas une extrusion
		//2 eme étape, on trace un chemin à partir des faces à supprimer
		std::vector<ligne> cheminDomaine;
		std::vector<ivec3> exBase;
		for(int i=0;i<faceToDestroy.size();i++)
			exBase.push_back(_pGroups[faceToDestroy[i].g].pFaces[faceToDestroy[i].f].Vertices);
		CreateWay(exBase,cheminDomaine);
		//3 eme étape, on trace un chemin à partir de la base 
		std::vector<ligne> cheminBaseExtrusion;

		//4 eme étape, on génére la nouvelle base du modèle

		//5 eme étape on supprime de _pGroups les faces à supprimer

		//6 eme étape on ajoute à _pGroups la nouvelle base



		return true;
	}

	bool Extruder::CreateWay(std::vector<ivec3> baseFaces,std::vector<ligne>& chemin)
	{

		//Liste les lignes des triangles
		std::vector<ligne> lignesCollision;
		ligne tmpLigne;
		for(int i=0;i<baseFaces.size();i++)
		{
			tmpLigne.a=baseFaces[i].a;
			tmpLigne.b=baseFaces[i].b;
			lignesCollision.push_back(tmpLigne);
			tmpLigne.a=baseFaces[i].b;
			tmpLigne.b=baseFaces[i].c;
			lignesCollision.push_back(tmpLigne);
			tmpLigne.a=baseFaces[i].c;
			tmpLigne.b=baseFaces[i].a;
			lignesCollision.push_back(tmpLigne);
		}
		//Retire les doublons (supprime les triangles qui n'ont plus de liens avec le model)
		std::vector<ligne> lignesContour;
		for(int i=0;i<lignesContour.size();i++)
		{
			bool seeDouble=false;
			for(int j=0;i<lignesContour.size();j++)
			{
				if(j!=i && isEqual(lignesContour[j],lignesContour[i]))
				{
					seeDouble=true;
					break;
				}
			}
			if(!seeDouble)
				lignesContour.push_back(lignesContour[i]);
		}
		//Construit un chemin à partir des lignes restantes, si aucun chemin ne peut être formé alors on retourne une erreur
		ligne found;
		std::vector<ligne> chemin;
		chemin.push_back(lignesContour[0]);
		lignesContour.erase(lignesContour.begin());
		int i=0;
		while(chemin[0].a!=chemin[chemin.size()-1].b)
		{
			bool isNextFound=false;
			for(int j=0;i<lignesContour.size();j++)
			{
				if(lignesContour[j].b==chemin[i].b)
				{
					t_point tmpDot=lignesContour[j].a;
					lignesContour[j].a=lignesContour[j].b;
					lignesContour[j].b=tmpDot;
				}
				if(lignesContour[j].a==chemin[i].b)
				{
					found=lignesContour[j];
					isNextFound=true;
					lignesContour.erase(lignesContour.begin()+j);
					break;
				}
			}
			if(isNextFound)
			{
				chemin.push_back(found);
				i++;
			}else{
				break;
			}
		}
		if(chemin[0].a!=chemin[chemin.size()-1].b) //si le chemin n'est pas bouclé
		{
			wxLogError("Impossible de construire un chemin");
			return false;
		}
		return true;
	}

	bool Extruder::isEqual(ligne lLigne,ligne rLigne)
	{
		if((lLigne.a==rLigne.a && lLigne.b==rLigne.b)||(lLigne.a==rLigne.b && lLigne.b==rLigne.a))
		{
			return true;
		}else{
			return false;
		}
	}
}

