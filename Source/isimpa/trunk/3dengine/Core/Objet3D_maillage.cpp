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

#include "GL/opengl_inc.h"
#include "wx/log.h"
#include "Objet3D.h"
#include "ply.h"	// gestion du format PLY stanford
#include "var.h"
#include "data_manager/appconfig.h"
#include "coreSrc/preprocess/input_output/poly.h" // gestion du format POLY tetgen
#include "data_manager/drawable_element.h"
#include <wx/filename.h>
#include "data_manager/python_interface/instanceManager.hpp"
#include <tools/vol_splitter.hpp>
//#include <tools/collision.h>
#include "last_cpp_include.hpp"

inline bool Contains(long &index,ivec3 &rightIndex)
{
	return ((index==rightIndex.a)||(index==rightIndex.b)||(index==rightIndex.c));
}

bool Vec3Contains(ivec3 &leftIndex,ivec3 &rightIndex)
{
	return (Contains(leftIndex.a,rightIndex) && Contains(leftIndex.b,rightIndex) && Contains(leftIndex.c,rightIndex));
}

bool LoadNodeFile(vec4 UnitizeVar, vec3** tabNodes, unsigned long &nodesMaillageSize, const std::wstring& nodeFilePath)
{
	using namespace std;
	using namespace formatPLY;

	if(!wxFileExists(nodeFilePath))
		return false;

	FILE *infile;
	char mefilename[250];
	char buffer[250];
	infile = fopen(STDWSTRINGTOCONSTCHAR(nodeFilePath), "r+");

	//Lecture de l'entête
	char snbnode[20];
	char sdim[5];
	fscanf(infile,"%20s %5s %5s %5s\n",snbnode,sdim,buffer,buffer);
	unsigned int nbNodes=Convertor::ToInt(snbnode);
	if(nbNodes>0)
	{
		delete[] *tabNodes;
		*tabNodes = new vec3[nbNodes];
		nodesMaillageSize=nbNodes;

		unsigned int idNode=0;
		char sidNode[12];
		char sx[25];
		char sy[25];
		char sz[25];
		while(idNode<nbNodes && !feof(infile))
		{
			fscanf(infile,"%12s %25s %25s %25s\n",sidNode,sx,sy,sz);
			idNode=Convertor::ToInt(sidNode);
			vec3 position(Convertor::ToFloat(wxString(sx)),Convertor::ToFloat(wxString(sy)),Convertor::ToFloat(wxString(sz)));
			position=coordsOperation::CommonCoordsToGlCoords(UnitizeVar,position);
			if(idNode>0 && idNode<=nbNodes)
			{
				(*tabNodes)[idNode-1]=position;
			}
		}
	}
    fclose(infile);
	if(nbNodes>0)
	{
		return true;
	}else{
		return false;
	}
}

bool LoadEleFile(tetrahedre** tabTetra, unsigned long &tabTetraSize, wxString tetraFilePath, vec3* tabNodes,vec4 UnitizeVar)
{
			//Mise à jour du volume total
	ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.volScene=0;
	using namespace std;
	using namespace formatPLY;

	if(!wxFileExists(tetraFilePath))
		return false;

	FILE *infile;
	char mefilename[250];
	char buffer[250];
	infile = fopen(tetraFilePath, "r+");

	//Lecture de l'entête
	char snbnode[20];
	char sdim[5];
	fscanf(infile,"%20s %5s %5s\n",snbnode,sdim,buffer);
	unsigned int nbTetra=Convertor::ToInt(snbnode);
	if(nbTetra>0)
	{
		delete[] *tabTetra;
		*tabTetra = new tetrahedre[nbTetra];
		tabTetraSize=nbTetra;

		unsigned int idTetra=0;
		char sidTetra[12];
		char sa[12];
		char sb[12];
		char sc[12];
		char sd[12];
		char svolume[12];
		while(idTetra<nbTetra && !feof(infile))
		{
			fscanf(infile,"%12s %12s %12s %12s %12s    %12s\n",sidTetra,sa,sb,sc,sd,svolume);

			idTetra=Convertor::ToInt(sidTetra);
	    	(*tabTetra)[idTetra-1].idVolume=Convertor::ToInt(svolume);
			ivec4 sommets(Convertor::ToInt(sa)-1,Convertor::ToInt(sb)-1,Convertor::ToInt(sc)-1,Convertor::ToInt(sd)-1);
			if(idTetra>0 && idTetra<=nbTetra)
			{
				////////////////
				// Sommets du tetrahedre
				//    D
				//   /|\
				//  / | \
				// A\ | /C
				//   \|/
				//    B
				////////////////
				(*tabTetra)[idTetra-1].sommets=sommets;
						//Mise à jour du volume total
				ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.volScene+=CalcTetraVolume(tabNodes[sommets.a]/UnitizeVar.w,tabNodes[sommets.b]/UnitizeVar.w,tabNodes[sommets.c]/UnitizeVar.w,tabNodes[sommets.d]/UnitizeVar.w);
				(*tabTetra)[idTetra-1].tetrafaces[0].sommets.set(sommets.b,sommets.d,sommets.c); //neighbors.a
				(*tabTetra)[idTetra-1].tetrafaces[1].sommets.set(sommets.c,sommets.d,sommets.a); //neighbors.b
				(*tabTetra)[idTetra-1].tetrafaces[2].sommets.set(sommets.a,sommets.d,sommets.b); //neighbors.c
				(*tabTetra)[idTetra-1].tetrafaces[3].sommets.set(sommets.b,sommets.c,sommets.a); //neighbors.d
				(*tabTetra)[idTetra-1].tetrafaces[0].marker=-1;
				(*tabTetra)[idTetra-1].tetrafaces[1].marker=-1;
				(*tabTetra)[idTetra-1].tetrafaces[2].marker=-1;
				(*tabTetra)[idTetra-1].tetrafaces[3].marker=-1;
			}
		}
	}
    fclose(infile);
	return true;
}

struct t_sommet
{
	std::vector<tetrahedre*> linkedTetra;
};
bool LoadFaceListe(std::vector<ivec3>& tabFace, wxString faceFilePath)
{
	using namespace std;
	using namespace formatPLY;

	if(!wxFileExists(faceFilePath))
		return false;

	FILE *infile;
	char mefilename[250];
	char buffer[250];
	infile = fopen(faceFilePath, "r+");

	//Lecture de l'entête
	char snbnode[20];
	int ismarker;
	fscanf(infile,"%20s %i\n",snbnode,&ismarker);
	unsigned int nbFaceTetra=Convertor::ToInt(snbnode);
	if(nbFaceTetra>0)
	{

		unsigned int idFaceTetra=0;
		char sidFace[12];
		char sa[12];
		char sb[12];
		char sc[12];
		char smarker[12];
		tabFace.reserve(nbFaceTetra);
		while(idFaceTetra<nbFaceTetra && !feof(infile))
		{
			if(ismarker==1)
				fscanf(infile,"%12s %12s %12s %12s %12s\n",sidFace,sa,sb,sc,smarker);
			else
				fscanf(infile,"%12s %12s %12s %12s\n",sidFace,sa,sb,sc);
			tabFace.push_back(ivec3(Convertor::ToInt(sc)-1,Convertor::ToInt(sb)-1,Convertor::ToInt(sa)-1));
			idFaceTetra++;
		}
	}
    fclose(infile);
	return true;


}
bool LoadFaceFile(tetrahedre** tabTetra, unsigned long &tabTetraSize, wxString faceFilePath,unsigned long nbNodes)
{
	using namespace std;
	using namespace formatPLY;

	if(!wxFileExists(faceFilePath))
		return false;

	//Construction d'une structure de données afin de lier 1 sommet à une liste de tetraèdres
	t_sommet* sommetsLst= new t_sommet[nbNodes];

	for(unsigned int idTetra=0;idTetra<tabTetraSize;idTetra++)
	{
		for(int idsommet=0;idsommet<4;idsommet++)
		{
			if(tabTetra+idTetra)
			{
				wxInt32 idnode=(*tabTetra)[idTetra].sommets[idsommet];
				if(idnode<nbNodes)
					sommetsLst[idnode].linkedTetra.push_back(&(*tabTetra)[idTetra]);
			}
		}
	}


	FILE *infile;
	char mefilename[250];
	char buffer[250];
	infile = fopen(faceFilePath, "r+");

	//Lecture de l'entête
	char snbnode[20];
	unsigned int  sdim;
	fscanf(infile,"%20s %i\n",snbnode,&sdim);
	unsigned int nbFaceTetra=Convertor::ToInt(snbnode);
	if(nbFaceTetra>0 && sdim==1)
	{

		unsigned int idFaceTetra=0;
		char sidFace[12];
		char sa[12];
		char sb[12];
		char sc[12];
		char smarker[12];
		while(idFaceTetra<nbFaceTetra && !feof(infile))
		{
			fscanf(infile,"%12s %12s %12s %12s %12s\n",sidFace,sa,sb,sc,smarker);

			ivec3 sommets(Convertor::ToInt(sa)-1,Convertor::ToInt(sb)-1,Convertor::ToInt(sc)-1);
			idFaceTetra=Convertor::ToInt(sidFace);
			unsigned int marker=Convertor::ToInt(smarker);
			std::vector<tetrahedre*>* vecTetras=&sommetsLst[sommets.a].linkedTetra;
			for(unsigned int idTetra=0;idTetra<vecTetras->size();idTetra++)
			{
				tetrahedre* tetratest=(*vecTetras)[idTetra];
				for(unsigned short idface=0;idface<4;idface++)
				{
					if(Vec3Contains(sommets,tetratest->tetrafaces[idface].sommets))
					{
						tetratest->tetrafaces[idface].marker=marker;
						break;
					}
				}
			}
		}
	}else{
		fclose(infile);
		delete[] sommetsLst;
		return false;
	}
    fclose(infile);
	delete[] sommetsLst;
	return true;
}

bool LoadNeighFile(tetrahedre** tabTetra, unsigned long &tabTetraSize, wxString faceFilePath)
{
	using namespace std;
	using namespace formatPLY;

	if(!wxFileExists(faceFilePath))
		return false;

	FILE *infile;
	char mefilename[250];
	char buffer[250];
	infile = fopen(faceFilePath, "r+");

	//Lecture de l'entête
	char snbnode[20];
	char sdim[5];
	fscanf(infile,"%20s %5s\n",snbnode,sdim);
	unsigned int nbTetra=Convertor::ToInt(snbnode);
	if(nbTetra>0)
	{
		unsigned int idTetra=0;
		char sidTetra[12];
		char sa[12];
		char sb[12];
		char sc[12];
		char sd[12];
		while(idTetra<nbTetra && !feof(infile))
		{
			fscanf(infile,"%12s %12s %12s %12s %12s\n",sidTetra,sa,sb,sc,sd);

			unsigned int idTetra=Convertor::ToInt(sidTetra)-1;

			ivec4 neighbors(Convertor::ToInt(sa)-1,Convertor::ToInt(sb)-1,Convertor::ToInt(sc)-1,Convertor::ToInt(sd)-1);

			if(idTetra>=0 && idTetra<tabTetraSize)
			{
				(*tabTetra)[idTetra].tetraNeighboor=neighbors;
			}
		}
	}
    fclose(infile);
	return true;
}

bool CObjet3D::LoadMaillage(const std::wstring& facePath,const std::wstring& elePath,const std::wstring& nodePath,const std::wstring& neighPath)
{
	if(!(this->GetNumFaces()>0))
		return false;
	////////////////////////////////////////
	// Chargement des noeuds des tetrahèdres
	////////////////////////////////////////
	if(!LoadNodeFile(UnitizeVar,&this->nodesMaillage,this->nodesMaillageSize,nodePath))
		return false;
	////////////////////////////////////////
	// Chargement des tetrahèdres
	////////////////////////////////////////
	if(!LoadEleFile(&this->tabVertexMaillage,this->tabVertexMaillageSize,elePath,this->nodesMaillage,UnitizeVar))
		return false;
	////////////////////////////////////////
	// Chargement des marqueurs (lien facetetra<->face scène)
	////////////////////////////////////////
	if(!LoadFaceFile(&this->tabVertexMaillage,this->tabVertexMaillageSize,facePath,this->nodesMaillageSize))
		return false;
	////////////////////////////////////////
	// Chargement des pointeurs de voisinage des tetrahèdre
	////////////////////////////////////////
	if(!LoadNeighFile(&this->tabVertexMaillage,this->tabVertexMaillageSize,neighPath))
		return false;
	formatMBIN::trimeshmodel trimodel;
	formatCoreBIN::ioModel model;
	this->GetTetraMesh(trimodel,true);
	this->ToCBINFormat(model);
	this->SetInternalFaceState(model,trimodel);
	return true;
}

void CObjet3D::SetInternalFaceState(const formatCoreBIN::ioModel& modelExport,const formatMBIN::trimeshmodel& tetramodel)
{
	if(modelExport.faces.size()!=this->GetNumFaces())
		return;

	for(int g=0; g < this->_pGroups.size(); g++)
	{
		int fGsize=this->_pGroups[g].pFaces.size();
		for(long f=0; f < fGsize ;f++)
		{
			this->_pGroups[g].pFaces[f].internalFace=false;
		}
	}
	volumes_splitter::VolumesSplitter internalFaceFinder;
	internalFaceFinder.LoadDomain(modelExport,tetramodel);
	std::vector<std::size_t> internFaces;
	internalFaceFinder.GetInternalFaces(internFaces);
	t_faceIndex faceIndex;
	bool propmodified=false;
	for(std::vector<std::size_t>::iterator it=internFaces.begin();it!=internFaces.end();it++)
	{
		 faceIndex=FindFaceGroupWithFaceIndex((*it));
		 if(!this->_pGroups[faceIndex.g].pFaces[faceIndex.f].internalFace==true)
		 {
			 this->_pGroups[faceIndex.g].pFaces[faceIndex.f].internalFace=true;
			 propmodified=true;
		 }
	}
	if(propmodified)
		this->Save(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME);
}

void CObjet3D::RenderMaillage(std::size_t modeRendu,t_cutPlane cutPlane)
{ //Execute les commandes openGl afin de donner le rendu du maillage

	if(modeRendu==GL_TRIANGLES)
	{
		this->_RenderMaillageTriangles(cutPlane);
	}else{
		this->_RenderMaillageLines(cutPlane);
	}

		//this->_RenderMaillageTriangles(cutPlane);
		//this->_RenderMaillageLines(cutPlane);
}

void CObjet3D::_RenderMaillageTriangles(t_cutPlane cutPlane)
{
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, 1.f);

	//Debug comptage du nombre de faces dans la scène
	unsigned int nbfaces=this->GetNumFaces();
	if(!this->tabVertexMaillage)
		return;
	//Rendu d'un tetrahedre
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT );
	glBegin(GL_TRIANGLES);
	long anstetranum=0;
	srand ( 0 );
	bool showTetra=true;
	int oldVol=-1;
	//////////////////////////////////////
	//Debug
	/*
	const vec3 planeB=coordsOperation::CommonCoordsToGlCoords(this->UnitizeVar,vec3(-16.81003189,-16.26114082,5.00000000));
	const vec3 planeA=coordsOperation::CommonCoordsToGlCoords(this->UnitizeVar,vec3(-16.81003189,16.29076767,5.00000000));
	const vec3 planeC=coordsOperation::CommonCoordsToGlCoords(this->UnitizeVar,vec3(27.29039383,16.29076767,5.00000000));
	*/
	for(long tetranum=0; tetranum < this->tabVertexMaillageSize ;tetranum++)
	{
		ivec4 sommetsTetra=this->tabVertexMaillage[tetranum].sommets;

		//Test des sommets du tetrahedre
		if(cutPlane.actif)
		{

			int quantInside=0;
			if(this->nodesMaillage[sommetsTetra.a][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(this->nodesMaillage[sommetsTetra.b][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(this->nodesMaillage[sommetsTetra.c][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(this->nodesMaillage[sommetsTetra.d][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(quantInside>0 && quantInside<4)
				showTetra=true;
			else
				showTetra=false;



			//////////////////////////////////////
			//Debug collision
			/*
			if(collision_manager::tetrahedron_intersect_parallelogram(this->nodesMaillage[sommetsTetra.a],
				this->nodesMaillage[sommetsTetra.b],
				this->nodesMaillage[sommetsTetra.c],
				this->nodesMaillage[sommetsTetra.d],
				planeA,
				planeB,
				planeC))
				showTetra=true;
			else
				showTetra=false;
			*/
		}

		if(showTetra)
		{
			if(oldVol!=this->tabVertexMaillage[tetranum].idVolume)
			{
				oldVol=this->tabVertexMaillage[tetranum].idVolume;

				//Coloration du maillage en fonction de la configuration
				void *obj;
				srand ( this->tabVertexMaillage[tetranum].idVolume*100 );
				float r=GetRandValue();
				float g=GetRandValue();
				float b=GetRandValue();
				if(this->tabVertexMaillage[tetranum].idVolume<=0)
				{
						r=g=b=1;
				}else{
					if(InstanceManager::GetReference(OBJECT_REF_TYPE_ELEMENT,oldVol,&obj))
					{
						Element* elobj=(Element*)obj;
						if(elobj)
						{
							if(elobj->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_CUBOIDE || elobj->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT)
							{
								Element* elprop=elobj->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_RENDU);
								if(elprop)
								{
									vec3 colvol=elprop->GetColorConfig("couleur")/255;
									r=colvol.r;
									g=colvol.g;
									b=colvol.b;
								}
							}else if(elobj->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME)
							{
								Element* elprop=elobj->GetElementByType(Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_RENDU);
								if(elprop)
								{
									vec3 colvol=elprop->GetColorConfig("volcolor")/255;
									r=colvol.r;
									g=colvol.g;
									b=colvol.b;
								}
							}
						}
					}
				}

				vec4 couleur_ambient(vec4(r, g, b, 1)*.75);
				vec4 couleur_diffuse(vec4(r, g, b, 1));
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, couleur_ambient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleur_diffuse);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,couleur_ambient);
			}

			for(int idface=0;idface<4;idface++)
			{
				ivec3 index=this->tabVertexMaillage[tetranum].tetrafaces[idface].sommets;
				bool colorChange=false;

				glNormal3fv(-FaceNormal(this->nodesMaillage[index.a],
										this->nodesMaillage[index.b],
										this->nodesMaillage[index.c]));

				glVertex3fv(this->nodesMaillage[index.a]);
				glVertex3fv(this->nodesMaillage[index.b]);
				glVertex3fv(this->nodesMaillage[index.c]);

			}
		}
	}
	glEnd();
	glEnable(GL_DEPTH_TEST);
}


void CObjet3D::_RenderMaillageLines(t_cutPlane cutPlane)
{
	if(!this->tabVertexMaillage)
		return;
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glLineWidth(1);
	glBegin(GL_LINES);
	vec4 couleur_ambient(.5, .5, .5, 1);

	glColor3fv(couleur_ambient);
	long anstetranum=0;
	bool showTetra=true;
	for(long tetranum=0; tetranum < this->tabVertexMaillageSize ;tetranum++)
	{
		//Test des sommets du tetrahedre
		if(cutPlane.actif)
		{
			int quantInside=0;
			if(this->nodesMaillage[this->tabVertexMaillage[tetranum].sommets.a][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(this->nodesMaillage[this->tabVertexMaillage[tetranum].sommets.b][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(this->nodesMaillage[this->tabVertexMaillage[tetranum].sommets.c][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(this->nodesMaillage[this->tabVertexMaillage[tetranum].sommets.d][cutPlane.plane]*cutPlane.sign>cutPlane.value)
				quantInside++;
			if(quantInside>0 && quantInside<4)
				showTetra=true;
			else
				showTetra=false;
		}

		if(showTetra)
		{
			for(int idface=0;idface<4;idface++)
			{


				ivec3 index=this->tabVertexMaillage[tetranum].tetrafaces[idface].sommets;
				glVertex3fv(this->nodesMaillage[index.a]);
				glVertex3fv(this->nodesMaillage[index.b]);
				glVertex3fv(this->nodesMaillage[index.b]);
				glVertex3fv(this->nodesMaillage[index.c]);
				glVertex3fv(this->nodesMaillage[index.c]);
				glVertex3fv(this->nodesMaillage[index.a]);
					/*
					vec3 g=GetGTriangle(this->nodesMaillage[index.a],this->nodesMaillage[index.b],this->nodesMaillage[index.c]);
					glEnd();
					glPrint(g,vec3(),"f:%i",this->tabVertexMaillage[tetranum].tetrafaces[idface].marker);
					glBegin(GL_LINES);
					*/
			}
		}
	}
	glEnd();
}

void CObjet3D::ToCBINFormat(formatCoreBIN::ioModel& modelExport)
{
	using namespace formatCoreBIN;

	/////////////////////////////////////////////////////////////////////
	//Pour les faces appartenant au modèle de base, association avec les matériaux, les récepteurs de surface, les encombrements (de type scène)
	std::vector<std::vector<ApplicationConfiguration::t_PropFace> > linkFacesAssociations(this->_pGroups.size());
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		std::vector<ApplicationConfiguration::t_PropFace> newVec(this->_pGroups[g].pFaces.size());
		linkFacesAssociations[g]=newVec;
	}
	ApplicationConfiguration::GetFaceLink(linkFacesAssociations);


	//////////////////////////////////////////////////////////////////////////////////////
	// Export du modèle 3D


	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			modelExport.faces.push_back(ioFace());
			ioFace *nvFace=&modelExport.faces[modelExport.faces.size()-1];
			(*nvFace).a=this->_pGroups[g].pFaces[f].Vertices.a;
			(*nvFace).b=this->_pGroups[g].pFaces[f].Vertices.b;
			(*nvFace).c=this->_pGroups[g].pFaces[f].Vertices.c;
			(*nvFace).idMat=linkFacesAssociations[g][f].idMaterial;
			(*nvFace).idRs=linkFacesAssociations[g][f].idRecepteurSurfacique;
			(*nvFace).idEn=linkFacesAssociations[g][f].idEncombrement;
		}
	}
	for(unsigned int i=0;i<this->_pVertices.size();i++)
	{
		vec3 vertex=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,this->_pVertices[i]);
		modelExport.vertices.push_back(t_pos(vertex.x,vertex.y,vertex.z));
	}



	/////////////////////////////////////////////////////////////////////
	// Chargement des encombrements dessinés par l'utilisateur
	t_HashElement::iterator it=ApplicationConfiguration::BeginRef(ApplicationConfiguration::ELEMENT_REF_TYPE_DRAWABLE);
	int eSize=GetDrawableElementSize();
	for(int navigateIndex=0;navigateIndex<eSize;navigateIndex++)
	{
		E_Drawable* eleToDraw=dynamic_cast<E_Drawable*>(it->second);
		if(eleToDraw)
		{
			std::vector<E_Drawable::t_triangle> consistentModel;
			eleToDraw->GetConsistentModel(consistentModel);
			if(consistentModel.size()>0)
			{


				for(int idface=0;idface<consistentModel.size();idface++)
				{
					modelExport.faces.push_back(ioFace());
					ioFace *nvFace=&modelExport.faces[modelExport.faces.size()-1];
					int indexVertice=modelExport.vertices.size();
					vec3 scaledVertice;
					scaledVertice=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,consistentModel[idface].a);
					modelExport.vertices.push_back(t_pos(scaledVertice.x,scaledVertice.y,scaledVertice.z));
					scaledVertice=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,consistentModel[idface].b);
					modelExport.vertices.push_back(t_pos(scaledVertice.x,scaledVertice.y,scaledVertice.z));
					scaledVertice=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,consistentModel[idface].c);
					modelExport.vertices.push_back(t_pos(scaledVertice.x,scaledVertice.y,scaledVertice.z));
					nvFace->a=indexVertice;
					nvFace->b=indexVertice+1;
					nvFace->c=indexVertice+2;
					nvFace->idMat=0;
					nvFace->idRs=-1;
					nvFace->idEn=eleToDraw->GetElementInfos().xmlIdElement;
				}
			}
		}
		it++;
	}


}
bool CObjet3D::_SaveCBIN(const char *filename)
{
	using namespace formatCoreBIN;
	CformatBIN classExport;
	ioModel modelExport;
	ToCBINFormat(modelExport);
	return classExport.ExportBIN(filename,modelExport);
}
void CObjet3D::GetTetraMesh(formatMBIN::trimeshmodel& trimesh,bool toRealCoords)
{
	using namespace formatMBIN;

	/////////////////
	// Instanciation des structures de données
	/////////////////
	trimesh.nodes.clear();
	trimesh.tetrahedres.clear();

	if(!tabVertexMaillage || !nodesMaillage)
		return;

	trimesh.nodes.reserve(nodesMaillageSize);

	if(!toRealCoords)
	{
		for(int idNode=0;idNode<nodesMaillageSize;idNode++)
		{
			trimesh.nodes.push_back(formatMBIN::t_binNode(nodesMaillage[idNode].x,nodesMaillage[idNode].y,nodesMaillage[idNode].z));
		}
	}else{
		for(int idNode=0;idNode<nodesMaillageSize;idNode++)
		{
			vec3 realNodeCoords=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,nodesMaillage[idNode]);
			trimesh.nodes.push_back(formatMBIN::t_binNode(realNodeCoords.x,realNodeCoords.y,realNodeCoords.z));
		}
	}
	trimesh.tetrahedres.insert(trimesh.tetrahedres.begin(),tabVertexMaillageSize,bintetrahedre());
	for(int idTetra=0;idTetra<tabVertexMaillageSize;idTetra++)
	{
		memcpy(&trimesh.tetrahedres[idTetra].sommets,&tabVertexMaillage[idTetra].sommets,sizeof(formatMBIN::Intb)*4);

		trimesh.tetrahedres[idTetra].idVolume=tabVertexMaillage[idTetra].idVolume;
		for(int idFace=0;idFace<4;idFace++)
		{
			trimesh.tetrahedres[idTetra].tetrafaces[idFace].marker=tabVertexMaillage[idTetra].tetrafaces[idFace].marker;
			memcpy(&trimesh.tetrahedres[idTetra].tetrafaces[idFace].sommets,&tabVertexMaillage[idTetra].tetrafaces[idFace].sommets,sizeof(ivec3));
			trimesh.tetrahedres[idTetra].tetrafaces[idFace].neighboor=tabVertexMaillage[idTetra].tetraNeighboor[idFace];
		}

	}
}
bool CObjet3D::SaveMaillage(const char *filename,bool toRealCoords)
{
	using namespace formatMBIN;
	trimeshmodel trimesh;
	GetTetraMesh(trimesh,toRealCoords);
	CMBIN binExporter;
	binExporter.SaveMesh(filename,trimesh);
	if(!wxFileExists(filename))
	{
		wxLogError(_("L'exportation du maillage de la scène n'a pu se faire."));
		return false;
	}
	return true;
}

bool CObjet3D::LoadMaillage(const char *filename)
{
	using namespace formatMBIN;
	bintetrahedre* tabtetra;
	t_binNode* tabNodes;

	CMBIN binImporter;
	unsigned int sizeTetra=0;
	unsigned int sizeNodes=0;

	binImporter.ImportBIN(filename,&tabtetra,&tabNodes,sizeTetra,sizeNodes);

	if(sizeTetra>0 && sizeNodes>0)
	{
		//Copie des noeuds
		delete[] nodesMaillage;
		nodesMaillage=new vec3[sizeNodes];
		nodesMaillageSize=sizeNodes;
		memcpy(nodesMaillage,tabNodes,sizeNodes);
		//Copie des tetrahèdres
		delete[] tabVertexMaillage;
		tabVertexMaillage=new tetrahedre[sizeTetra];
		tabVertexMaillageSize=sizeTetra;

		for(int idTetra=0;idTetra<sizeTetra;idTetra++)
		{
			memcpy(&tabVertexMaillage[idTetra].sommets,&tabtetra[idTetra].sommets,4);
			for(int idFace=0;idFace<4;idFace++)
			{
				tabVertexMaillage[idTetra].tetrafaces[idFace].marker=tabtetra[idTetra].tetrafaces[idFace].marker;
				memcpy(&tabVertexMaillage[idTetra].tetrafaces[idFace].sommets,&tabtetra[idTetra].tetrafaces[idFace].sommets,3);
				tabVertexMaillage[idTetra].tetraNeighboor[idFace]=tabtetra[idTetra].tetrafaces[idFace].neighboor;
			}

		}

		delete[] tabtetra;
		delete[] tabNodes;
		return true;
	}else{
		return true;
	}
}

bool CObjet3D::BuildVarConstraintFile(const char *filename, float faceAreaContraint)
{
	using namespace formatVAR;


	t_file constraintInfos;




	std::vector<std::vector<ApplicationConfiguration::t_PropFace> > linkFacesAssociations(this->_pGroups.size());
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		std::vector<ApplicationConfiguration::t_PropFace> newVec(this->_pGroups[g].pFaces.size());
		linkFacesAssociations[g]=newVec;
	}
	ApplicationConfiguration::GetFaceLink(linkFacesAssociations);
	int idFace=0;
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			if(linkFacesAssociations[g][f].idRecepteurSurfacique!=-1)
			{
				constraintInfos.facetsConstraints.push_back(t_facet_constraint(idFace,faceAreaContraint));
			}
			idFace++;
		}
	}

	CVar varExporter;



	return varExporter.BuildVar(constraintInfos,filename);
}

bool CObjet3D::_SavePOLY(const char *filename,bool exportUserModel,bool separateUserDefinedModel,bool saveFaceIndex,std::vector<t_faceIndex>* fg_to_ind,bool saveFaceIndexAsGroup)
{
	using namespace formatPOLY;
	CPoly polySaver;
	formatPOLY::t_model modelExport;
	modelExport.saveFaceIndex=saveFaceIndex;
	modelExport.modelVertices.reserve(this->_pVertices.size());
	for(unsigned int i=0;i<this->_pVertices.size();i++)
	{
		modelExport.modelVertices.push_back(coordsOperation::GlCoordsToCommonCoords(UnitizeVar,this->_pVertices[i]));
	}
	unsigned long faceIndex=0;
	///////////////////////////////////////
	// Sauvegarde modèle 3D
	modelExport.modelFaces.reserve(this->GetNumFaces());
	if(fg_to_ind)
		fg_to_ind->reserve(this->GetNumFaces());
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			formatPOLY::t_face nvFace;
			if(saveFaceIndexAsGroup)
				nvFace.faceIndex=g;
			else
				nvFace.faceIndex=faceIndex;
			nvFace.indicesSommets=this->_pGroups[g].pFaces[f].Vertices;
			modelExport.modelFaces.push_back(nvFace);
			if(fg_to_ind)
				fg_to_ind->push_back(t_faceIndex(f,g));
			faceIndex++;
		}
	}
	///////////////////////////////////////
	// Sauvegarde des éléments ajoutés par l'utilisateur
	if(exportUserModel)
	{
		t_HashElement::iterator it=ApplicationConfiguration::BeginRef(ApplicationConfiguration::ELEMENT_REF_TYPE_DRAWABLE);
		int eSize=GetDrawableElementSize();
		for(int navigateIndex=0;navigateIndex<eSize;navigateIndex++)
		{
			E_Drawable* eleToDraw=dynamic_cast<E_Drawable*>(it->second);
			if(eleToDraw)
			{
				std::vector<E_Drawable::t_triangle> consistentModel;
				eleToDraw->GetConsistentModel(consistentModel);
				if(consistentModel.size()>0)
				{
					for(int idface=0;idface<consistentModel.size();idface++)
					{
						int indexVertice=modelExport.modelVertices.size();
						modelExport.modelVertices.push_back(coordsOperation::GlCoordsToCommonCoords(UnitizeVar,consistentModel[idface].a));
						modelExport.modelVertices.push_back(coordsOperation::GlCoordsToCommonCoords(UnitizeVar,consistentModel[idface].b));
						modelExport.modelVertices.push_back(coordsOperation::GlCoordsToCommonCoords(UnitizeVar,consistentModel[idface].c));
						formatPOLY::t_face nvFace;
						nvFace.indicesSommets.set(indexVertice,indexVertice+1,indexVertice+2);
						nvFace.faceIndex=faceIndex;
						if(separateUserDefinedModel)
							modelExport.userDefinedFaces.push_back(nvFace);
						else
							modelExport.modelFaces.push_back(nvFace);
						if(fg_to_ind)
							fg_to_ind->push_back(t_faceIndex(idface,-1));
						faceIndex++;
					}
				}
				E_Drawable::t_el_vol_infos eleinfo;
				if(eleToDraw->GetMaillageVolumeInfos(eleinfo))
				{
					///////////////////////////////////////
					// Ajout des indices de volumes
					t_region elementRegion;
					elementRegion.regionIndex=eleToDraw->GetElementInfos().xmlIdElement;
					elementRegion.regionRefinement=eleinfo.volumeConstraint;

					if(eleinfo.firstFace.g==-1)
					{
						elementRegion.dotInRegion=eleinfo.dotInsideVol;
					}else{
						//On a l'indice de la première face composant le volume
						//Calcul  d'un point à l'interieur de se volume
						if(eleinfo.dotInsideVol==vec3())
						{
							vec3 translatedDot;
							SFace3D* firstFace=&this->_pGroups[eleinfo.firstFace.g].pFaces[eleinfo.firstFace.f];
							vec3 gTri=GetGTriangle(this->_pVertices[firstFace->Vertices.a],this->_pVertices[firstFace->Vertices.b],this->_pVertices[firstFace->Vertices.c]);
							translatedDot=gTri+firstFace->FaceNormals*0.05f;
							elementRegion.dotInRegion=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,translatedDot);
						}else{
							elementRegion.dotInRegion=eleinfo.dotInsideVol;
						}
					}
					modelExport.modelRegions.push_back(elementRegion);
				}
			}
			it++;
		}
	}

	return polySaver.ExportPOLY(modelExport,filename);
}




bool CObjet3D::_LoadFaceFile(const char *filename)
{
	wxFileName nodePath(filename);
	nodePath.SetExt("node");
	vec3* nodesFaces=NULL;
	unsigned long nodesSize;
	std::vector<ivec3> faces;
	////////////////////////////////////////
	// Chargement des noeuds des tetrahèdres
	////////////////////////////////////////
	this->_Destroy();
	if(LoadNodeFile(UnitizeVar,&nodesFaces,nodesSize,WXSTRINGTOSTDWSTRING(nodePath.GetFullPath())))
	{
		////////////////////////////////////////
		// Chargement des faces
		////////////////////////////////////////
		if(LoadFaceListe(faces,filename))
		{
			vec4 transfo=UnitizeVar;
			//Supprime le modèle ouvert
			//Transfert  de tabMaillage vers la scène
			//Transfert des vertices
			this->_pVertices.clear();
			if(nodesFaces)
			{
				for(int idvert=0;idvert<nodesSize;idvert++)
					this->_pVertices.push_back(nodesFaces[idvert]);
			}
			//Transfer des groupes
			SGroup3D grp;
			this->_pGroups.push_back(grp);
			int g=0;
			unsigned int nbfaces=faces.size();
			SFace3D nvface;
			this->_pGroups[g].Name="model";
			this->_pGroups[g].Material=-1;
			this->_pGroups[g].pFaces.reserve(nbfaces);
			for(long f=0;f<nbfaces;f++)
			{
				this->_pGroups[g].pFaces.push_back(nvface);
				this->_pGroups[g].pFaces[f].Vertices=faces[f];

				this->_pGroups[g].pFaces[f].Diff[0]=true;
				this->_pGroups[g].pFaces[f].Diff[1]=true;
				this->_pGroups[g].pFaces[f].Diff[2]=true;
				ivec3 sommetsTri=faces[f];
				if(nodesFaces)
					this->_pGroups[g].pFaces[f].FaceNormals=FaceNormal(this->_pVertices[sommetsTri.a],this->_pVertices[sommetsTri.b],this->_pVertices[sommetsTri.c]);
				this->_pGroups[g].pFaces[f].idMaterial=-1;
			}
			//Transfert des matières
			this->_nbMaterials=0;
			this->_pNormals.push_back(vec3());
			this->Unitize();
			delete IntersectionTool;
			IntersectionTool=new ui_tools::IntersectionSeeker(_pVertices,_pGroups);
			return true;
		}
	}
	return false;
}
