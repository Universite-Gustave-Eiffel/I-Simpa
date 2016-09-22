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

// sppsNantes.cpp�: d�finit le point d'entr�e pour l'application console.
//


#include "coreinitialisation.h"
#include "tools/collision.h"


using namespace std;

entier_court GetTetraFaceWithNormal(t_Tetra* volumeTest,const vec3& faceNormal)
{
	for(entier idface=0;idface<4;idface++)
	{
		if(volumeTest->faces[idface].normal.barelyEqual(faceNormal))
			return idface;
	}
	return 0;
}

entier GetRecepteurSurfacique(entier xmlIdRs,std::vector<r_Surf*>& recepteur_s_List)
{
	for(std::size_t idRs=0;idRs<recepteur_s_List.size();idRs++)
	{
		if(recepteur_s_List[idRs]->idRecepteurS==xmlIdRs)
			return idRs;
	}
	return -1;
}

void genprintVec(vec3 inf)
{
	using namespace std;
	std::string retStr;
	cout<<"["<<inf.x<<";"<<inf.y<<";"<<inf.z<<"]";
}


/**
 * Initialise la propri�t� de t�tra�dre courant des sources
 * @param tetraMesh Maillage t�tra�drique de la sc�ne
 * @param lstSources Liste des sources
 */
void InitSourcesTetraLocalisation(t_TetraMesh* tetraMesh,std::vector<t_Source*>* lstSources)
{
	for(std::size_t idSrc=0;idSrc<lstSources->size();idSrc++)
	{
		t_Source* currentSource=(*lstSources)[idSrc];
		uentier idtetra=0;
		while(!(*currentSource).currentVolume && idtetra<tetraMesh->tetraedresSize)
		{
			bool inside=true;
			for(int idFace=0;idFace<4;idFace++)
			{
				t_Tetra_Faces* currentFace=&tetraMesh->tetraedres[idtetra].faces[idFace];
				//Calcul du vecteur entre la position de la source et le premier sommet de la face du t�trah�dre
				vec3 pSrcA=tetraMesh->nodes[currentFace->indiceSommets.a]-(*currentSource).Position;
				if(pSrcA.dot(currentFace->normal)>0)//si normal de la face va vers la position de la source
				{
					inside=false;
					break;
				}
			}
			if(inside)
				(*currentSource).currentVolume=&tetraMesh->tetraedres[idtetra];
			idtetra++;
		}
	}
}

/**
 * Retourne la longueur entre la position Z du tetra�dre en param�tre et la position Z du centre de la face la plus haute
 */
decimal GetHeight( t_TetraMesh* tetraMesh, t_Tetra* currentTetra )
{
	vec3 optimal_z_normal(0,0,1);
	vec3 origin=currentTetra->g;
	decimal z_min=origin.z;
	t_Tetra* z_min_neigh=NULL;
	t_cFace* z_min_face=NULL;
	while(currentTetra!=NULL && z_min_face==NULL)
	{
		decimal z_angle_min=180;
		z_min_neigh=NULL;
		for(entier idneigh=0;idneigh<4;idneigh++)
		{
			decimal angle_z_zf=currentTetra->faces[idneigh].normal.angle(optimal_z_normal);
			if(angle_z_zf<z_angle_min)
			{
				vec3 gTri=GetGTriangle(tetraMesh->nodes[currentTetra->faces[idneigh].indiceSommets.a], tetraMesh->nodes[currentTetra->faces[idneigh].indiceSommets.b], tetraMesh->nodes[currentTetra->faces[idneigh].indiceSommets.c]);
				z_angle_min=angle_z_zf;
				z_min=gTri.z;
				z_min_face=currentTetra->faces[idneigh].face_scene;
				z_min_neigh=currentTetra->voisins[idneigh];
			}
		}
		currentTetra=z_min_neigh;
		if(currentTetra && currentTetra->z!=-1) //Si ce t�tra�dre a d�j� �t� test�
		{
			z_min=currentTetra->g.z-currentTetra->z;
			break;
		}
	}
	return origin.z-z_min;
}
void InitTetraCelerite(t_TetraMesh* tetraMesh,Base_Core_Configuration& configManager)
{
	decimal alog=*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_ALOG);
	decimal blin=*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_BLIN);
	if(!(alog==0 && blin==0)) //si milieu non homog�ne
	{
		std::cout<<"Compute of z ground inside each tetra."<<std::endl;
		for(uentier idtetra=0;idtetra<tetraMesh->tetraedresSize;idtetra++)
		{
			t_Tetra* currentTetra=&tetraMesh->tetraedres[idtetra];
			if(currentTetra->z==-1)
			{
				currentTetra->z=GetHeight(tetraMesh,currentTetra);
			}
		}
		std::cout<<"Calculation of z ground inside each tetra has been done."<<std::endl;
	}
}
void InitEncombrementTetraLocalisation(t_TetraMesh* tetraMesh,Base_Core_Configuration& configManager)
{
	t_Encombrement* lastEncombrementUsedPt=NULL;
	entier lastEncombrementUsedOutsideIndex=0;
	for(uentier idtetra=0;idtetra<tetraMesh->tetraedresSize;idtetra++)
	{

		if(tetraMesh->tetraedres[idtetra].idVolume!=lastEncombrementUsedOutsideIndex && tetraMesh->tetraedres[idtetra].idVolume!=0)
		{
			lastEncombrementUsedOutsideIndex=tetraMesh->tetraedres[idtetra].idVolume;
			lastEncombrementUsedPt=configManager.GetEncombrementByOutsideIndex(lastEncombrementUsedOutsideIndex);
		}
		if(tetraMesh->tetraedres[idtetra].idVolume!=0)
		{
			tetraMesh->tetraedres[idtetra].volumeEncombrement=lastEncombrementUsedPt;
			//Sur chaque face du t�trah�dre associer � l'encombrement
			for(int idface=0;idface<4;idface++)
			{
				if(tetraMesh->tetraedres[idtetra].faces[idface].face_scene)
				{
					tetraMesh->tetraedres[idtetra].faces[idface].face_scene->faceEncombrement=lastEncombrementUsedPt;
				}
			}
		}
	}
}

void InitRecepteurPTetraLocalisation(t_TetraMesh* tetraMesh,std::vector<t_Recepteur_P*>* lstRecepteurP,Base_Core_Configuration& configManager)
{
	for(std::size_t idRp=0;idRp<lstRecepteurP->size();idRp++)
	{
		t_Recepteur_P* currentRecepteurp=(*lstRecepteurP)[idRp];

		for(uentier idtetra=0;idtetra<tetraMesh->tetraedresSize;idtetra++)
		{
			bool inside=true;
			for(int idFace=0;idFace<4;idFace++)
			{
				t_Tetra_Faces* currentFace=&tetraMesh->tetraedres[idtetra].faces[idFace];
				//Calcul du vecteur entre la position de la source et le premier sommet de la face du t�trah�dre
				vec3 pRcpA=tetraMesh->nodes[currentFace->indiceSommets.a]-(*currentRecepteurp).position;
				if(pRcpA.dot(currentFace->normal)>0)//si normal de la face va vers la position du recepteur ponctuel
				{
					inside=false;
					break;
				}
			}
			if(inside)
			{
				if(!tetraMesh->tetraedres[idtetra].linkedRecepteurP)
					tetraMesh->tetraedres[idtetra].makeTabRecp();
				tetraMesh->tetraedres[idtetra].linkedRecepteurP->push_back(currentRecepteurp);
				//Calcul de c*Dt/V_maille
				//ivec4 sommetsTetra=tetraMesh->tetraedres[idtetra].sommets;
				//decimal volTetra=CalcTetraVolume( tetraMesh->nodes[sommetsTetra.a],tetraMesh->nodes[sommetsTetra.b],tetraMesh->nodes[sommetsTetra.c],tetraMesh->nodes[sommetsTetra.d]);
				//currentRecepteurp->cdt_vol=CCalculsGenerauxThermodynamique::masse_vol(*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_PRESSION_ATMOSPHERIQUE),*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_TEMPERATURE)+273.15)*(*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_CELERITE))*(*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_CELERITE))/volTetra;
				currentRecepteurp->cdt_vol=*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_CELERITE)*(*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_RHO));
				currentRecepteurp->indexTetra=idtetra;
				break;
			}
		}
	}
}

void InitCutMapTetraLocalisation(t_TetraMesh* tetraMesh,std::vector<r_SurfCut*>* lstCutMap,Base_Core_Configuration& configManager)
{
	for(std::vector<r_SurfCut*>::iterator itrs=lstCutMap->begin();itrs!=lstCutMap->end();itrs++)
	{
		vec3& planeA((*itrs)->Bvert);
		vec3& planeB((*itrs)->Avert);
		vec3& planeC((*itrs)->Cvert);

		for(uentier idtetra=0;idtetra<tetraMesh->tetraedresSize;idtetra++)
		{
			t_Tetra& curTetra(tetraMesh->tetraedres[idtetra]);
			const ivec4& sommetsTetra(curTetra.sommets);
			if(collision_manager::tetrahedron_intersect_parallelogram(tetraMesh->nodes[sommetsTetra.a],
				tetraMesh->nodes[sommetsTetra.b],
				tetraMesh->nodes[sommetsTetra.c],
				tetraMesh->nodes[sommetsTetra.d],
				planeA,
				planeB,
				planeC))
			{
				curTetra.pushCutReceiver(*itrs);
			}
		}
	}
}
/**
 * R�serve l'espace m�moire correspondant � une bande de fr�quence.
 */
void InitRecepteurSBfreq(std::vector<r_Surf*>& recepteursSurfaciques,uentier idFreq,uentier nbTimeStep)
{
	for(std::size_t idr=0;idr<recepteursSurfaciques.size();idr++)
	{
		for(uentier idface=0;idface<recepteursSurfaciques[idr]->nbFaces;idface++)
		{
			recepteursSurfaciques[idr]->faces[idface].InitFreq(idFreq,nbTimeStep);
		}
	}
}
void InitRecepteurSCut(std::vector<r_SurfCut*>& recepteursSurfaciques,Base_Core_Configuration& configManager)
{

	///////////////////////////////////////////
	// Instanciation des tableaux du recepteurs surfacique
	std::vector<bool> usage(configManager.freqList.size(),false);

	for(std::size_t idfreq=0;idfreq<configManager.freqList.size();idfreq++)
	{
		usage[idfreq]=configManager.freqList[idfreq]->doCalculation;
	}
	for(std::size_t idr=0;idr<recepteursSurfaciques.size();idr++)
	{
		recepteursSurfaciques[idr]->Init(configManager.freqList.size(),usage,recepteursSurfaciques[idr]->NbCellU,recepteursSurfaciques[idr]->NbCellV,*configManager.FastGetConfigValue(Base_Core_Configuration::IPROP_QUANT_TIMESTEP));
	}


}
void InitRecepteurS(t_TetraMesh* tetraModel,std::vector<r_Surf*>& recepteursSurfaciques,Base_Core_Configuration& configManager)
{

	///////////////////////////////////////////////
	// Chargement des recepteurs surfacique, assignation du nombre de faces

	std::vector<t_Tetra_Faces*> tetraFaceRsOtherSide;
	std::vector<t_Tetra_Faces*> tetraFaceRs;
	std::vector<uentier> tetraFaceRsIndex;
	std::vector<uentier> RsIndex;
	uentier_long nbTetras=tetraModel->tetraedresSize;
	int last_localIdRecept=-1;
	int last_xmlIdRecept=-1;

	for(uentier_long idtetra=0;idtetra<nbTetras;idtetra++)
	{
		for(int i=0;i<4;i++)
		{
			if(tetraModel->tetraedres[idtetra].faces[i].face_scene)
			{
				int idRs=tetraModel->tetraedres[idtetra].faces[i].face_scene->idRecepteurS;
				if(idRs!=-1 && tetraModel->tetraedres[idtetra].faces[i].face_scene->normal.barelyEqual(-tetraModel->tetraedres[idtetra].faces[i].normal)) //added  && tetraModel->tetraedres[idtetra].faces[i].face_scene->normal==tetraModel->tetraedres[idtetra].faces[i].normal 21 aout 2008 (pour les recepteurs de surface plac� entre deux t�tra�dres, associer uniquement � la face ayant la m�me normal que la face de tetra�dre)
				{
					int localIdRecept;
					if(last_xmlIdRecept!=idRs)
					{
						last_localIdRecept=GetRecepteurSurfacique(idRs,recepteursSurfaciques);
						last_xmlIdRecept=idRs;
					}
					localIdRecept=last_localIdRecept;
					tetraFaceRsIndex.push_back(recepteursSurfaciques[localIdRecept]->nbFaces);
					RsIndex.push_back(localIdRecept);
					tetraFaceRs.push_back(&tetraModel->tetraedres[idtetra].faces[i]);
					recepteursSurfaciques[localIdRecept]->nbFaces++;
					//De l'autre cot�, c'est � dire sur l'autre t�tra�dre il faut lier sa face au r�cepteur surfacique
					if(tetraModel->tetraedres[idtetra].voisins[i])
					{
						entier_court ifacemirror=GetTetraFaceWithNormal(tetraModel->tetraedres[idtetra].voisins[i],tetraModel->tetraedres[idtetra].faces[i].normal*-1);
						tetraFaceRsOtherSide.push_back(&(tetraModel->tetraedres[idtetra].voisins[i]->faces[ifacemirror]));
					}else{
						tetraFaceRsOtherSide.push_back(NULL);
					}
				}
			}
		}
	}

	///////////////////////////////////////////
	// Instanciation des tableaux du recepteurs surfacique

	for(std::size_t idr=0;idr<recepteursSurfaciques.size();idr++)
	{
		recepteursSurfaciques[idr]->InitFaces(recepteursSurfaciques[idr]->nbFaces,configManager.freqList.size());
	}


	///////////////////////////////////////////////
	// Assignation des pointeurs vers les recepteurs surfaciques, et assignation des sommets des recepteurs surfacique
	for(std::size_t idfrs=0;idfrs<tetraFaceRs.size();idfrs++)
	{

		tetraFaceRs[idfrs]->recepteurS=&recepteursSurfaciques[RsIndex[idfrs]]->faces[tetraFaceRsIndex[idfrs]];
		if(tetraFaceRsOtherSide[idfrs])
			tetraFaceRsOtherSide[idfrs]->recepteurS=tetraFaceRs[idfrs]->recepteurS;
		recepteursSurfaciques[RsIndex[idfrs]]->faces[tetraFaceRsIndex[idfrs]].indiceSommets=tetraFaceRs[idfrs]->indiceSommets;
		recepteursSurfaciques[RsIndex[idfrs]]->faces[tetraFaceRsIndex[idfrs]].sommets[0]=tetraModel->nodes[tetraFaceRs[idfrs]->indiceSommets.a];
		recepteursSurfaciques[RsIndex[idfrs]]->faces[tetraFaceRsIndex[idfrs]].sommets[1]=tetraModel->nodes[tetraFaceRs[idfrs]->indiceSommets.b];
		recepteursSurfaciques[RsIndex[idfrs]]->faces[tetraFaceRsIndex[idfrs]].sommets[2]=tetraModel->nodes[tetraFaceRs[idfrs]->indiceSommets.c];
	}
}

void InitRecepteurS(t_Mesh* sceneMesh,std::vector<r_Surf*>& recepteursSurfaciques,Base_Core_Configuration& configManager)
{

	///////////////////////////////////////////////
	// Chargement des recepteurs surfacique, assignation du nombre de faces
	std::vector<t_cFace*> faceRs;
	std::vector<uentier> faceRsIndex;
	std::vector<uentier> RsIndex;
	uentier_long nbFaces=sceneMesh->pface_size;
	for(uentier_long idface=0;idface<nbFaces;idface++)
	{
		if(sceneMesh->pfaces[idface].idRecepteurS!=-1)
		{
			//On charge le recepteur
			int idRs=GetRecepteurSurfacique(sceneMesh->pfaces[idface].idRecepteurS,recepteursSurfaciques);
			faceRs.push_back(&sceneMesh->pfaces[idface]);
			faceRsIndex.push_back(recepteursSurfaciques[idRs]->nbFaces);
			RsIndex.push_back(idRs);
			recepteursSurfaciques[idRs]->nbFaces++;
		}
	}

	///////////////////////////////////////////
	// Instanciation des tableaux du recepteurs surfacique

	for(std::size_t idr=0;idr<recepteursSurfaciques.size();idr++)
	{
		recepteursSurfaciques[idr]->InitFaces(recepteursSurfaciques[idr]->nbFaces,configManager.freqList.size());
	}


	///////////////////////////////////////////////
	// Assignation des pointeurs vers les recepteurs surfaciques, et assignation des sommets des recepteurs surfacique
	for(std::size_t idfrs=0;idfrs<faceRs.size();idfrs++)
	{
		faceRs[idfrs]->recepteurS=&recepteursSurfaciques[RsIndex[idfrs]]->faces[faceRsIndex[idfrs]];
		recepteursSurfaciques[RsIndex[idfrs]]->faces[faceRsIndex[idfrs]].indiceSommets=faceRs[idfrs]->sommets;
		recepteursSurfaciques[RsIndex[idfrs]]->faces[faceRsIndex[idfrs]].sommets[0]=sceneMesh->pvertices[faceRs[idfrs]->sommets.a];
		recepteursSurfaciques[RsIndex[idfrs]]->faces[faceRsIndex[idfrs]].sommets[1]=sceneMesh->pvertices[faceRs[idfrs]->sommets.b];
		recepteursSurfaciques[RsIndex[idfrs]]->faces[faceRsIndex[idfrs]].sommets[2]=sceneMesh->pvertices[faceRs[idfrs]->sommets.c];
	}
}
/**
 * Chargement des faces dans les structures appropri�es
 */
bool initMesh(t_Mesh &sceneMesh,CoreString workingDir,CoreString sceneMeshPath,Base_Core_Configuration& configManager, bool verbose_mode)
{
	formatCoreBIN::ioModel modelEntree;
	//**************************************************
	// 1: Lire le fichier de mod�le en entr�e
	if (verbose_mode) { cout << "Scene mesh is currently loading .." << endl; }
	formatCoreBIN::CformatBIN binImporter;
	if(!binImporter.ImportBIN(modelEntree,sceneMeshPath.c_str()) || modelEntree.vertices.size()==0)
	{
		cout<<"Unable to read the scene mesh file :"<<endl<<sceneMeshPath<<endl;
		return false;
	}else{
		if (verbose_mode) { cout << "Opening " << sceneMeshPath << endl << "Load of the scene mesh file has been completed successfully." << endl; }
	}
	//**************************************************
	// Chargement des faces dans les structures appropri�es
	sceneMesh.pvert_size=modelEntree.vertices.size();
	sceneMesh.pvertices=new vec3[sceneMesh.pvert_size];
	for(unsigned int i=0;i<modelEntree.vertices.size();i++)
		sceneMesh.pvertices[i]=(float*)&modelEntree.vertices[i];
	sceneMesh.pface_size=modelEntree.faces.size();
	sceneMesh.pfaces=new t_cFace[sceneMesh.pface_size];
	//Materiau
	t_Material* lastMaterialUsedPt=configManager.GetMaterialByOutsideIndex(0);
	uentier lastMaterialUsedOutsideIndex=0;
	//Encombrement
	t_Encombrement* lastEncombrementUsedPt=NULL;
	entier lastEncombrementUsedOutsideIndex=-1;
	for(uentier idface=0;idface<sceneMesh.pface_size;idface++)
	{
		sceneMesh.pfaces[idface].sommets.a=modelEntree.faces[idface].a;
		sceneMesh.pfaces[idface].sommets.b=modelEntree.faces[idface].b;
		sceneMesh.pfaces[idface].sommets.c=modelEntree.faces[idface].c;
		sceneMesh.pfaces[idface].idRecepteurS=modelEntree.faces[idface].idRs;
		sceneMesh.pfaces[idface].idencombrement=modelEntree.faces[idface].idEn;
		sceneMesh.pfaces[idface].normal=FaceNormal(sceneMesh.pvertices[sceneMesh.pfaces[idface].sommets.a],sceneMesh.pvertices[sceneMesh.pfaces[idface].sommets.b],sceneMesh.pvertices[sceneMesh.pfaces[idface].sommets.c]);
		///////////////////////////////////////////////
		// Association mat�riau, face
		if(modelEntree.faces[idface].idMat!=lastMaterialUsedOutsideIndex)
		{
			lastMaterialUsedOutsideIndex=modelEntree.faces[idface].idMat;
			lastMaterialUsedPt=configManager.GetMaterialByOutsideIndex(lastMaterialUsedOutsideIndex);
			if(!lastMaterialUsedPt) //Si une face n'a pas de mat�riaux associ�
				configManager.materialList[0];
		}
		sceneMesh.pfaces[idface].faceMaterial=lastMaterialUsedPt;
		///////////////////////////////////////////////
		// Association encombrement, face
		if(modelEntree.faces[idface].idEn!=lastEncombrementUsedOutsideIndex && modelEntree.faces[idface].idEn!=-1)
		{
			lastEncombrementUsedOutsideIndex=modelEntree.faces[idface].idEn;
			lastEncombrementUsedPt=configManager.GetEncombrementByOutsideIndex(lastEncombrementUsedOutsideIndex);
		}
		if(modelEntree.faces[idface].idEn!=-1)
			sceneMesh.pfaces[idface].faceEncombrement=lastEncombrementUsedPt;
		else
			sceneMesh.pfaces[idface].faceEncombrement=NULL;
	}
	return true;
}

bool initTetraMesh(CoreString tetraMeshFilePath,t_Mesh &sceneMesh,int nbFreq,t_TetraMesh& sceneTetraMesh,Base_Core_Configuration& configManager, bool verbose_mode)
{
	//**************************************************
	// Chargement du maillage
	if(!sceneTetraMesh.LoadFile(tetraMeshFilePath.c_str(),sceneMesh,nbFreq))
	{
		cout<<"Unable to read the tetrahedalization of the scene mesh file, calculation canceled."<<endl;
		return false;
	}else{
		if (verbose_mode) { cout << "Load of the tetrahedalization scene mesh file has been completed successfully." << endl; }
	}
	//**************************************************
	// Initialisation des Liens avec le maillage et leurs param�tres (pr�-calcul)
	InitSourcesTetraLocalisation(&sceneTetraMesh,&configManager.srcList);
	InitRecepteurPTetraLocalisation(&sceneTetraMesh,&configManager.recepteur_p_List,configManager);
	InitCutMapTetraLocalisation(&sceneTetraMesh,&configManager.recepteur_scut_List,configManager);
	InitEncombrementTetraLocalisation(&sceneTetraMesh,configManager);
	InitRecepteurS(&sceneTetraMesh,configManager.recepteur_s_List,configManager);
	InitRecepteurSCut(configManager.recepteur_scut_List,configManager);
	InitTetraCelerite(&sceneTetraMesh,configManager);
	return true;
}

void reportCompilation(Base_Core_Configuration& configManager,CoreString workingDir)
{
	//**************************************************
	// 6: Compilation des fichiers de resultats
	cout<<"Output results files."<<endl;
	//Instanciation du tableau des libell� des champs de fr�quences
	std::vector<CoreString> reportFreqLbl;
	//Instanciation du tableau des libell� des pas de temps
	std::vector<CoreString> reportStepLbl;
	BaseReportManager::InitHeaderArrays(configManager, reportFreqLbl, reportStepLbl);
	CoreString rootRaw=workingDir;
	CoreString rootRp=rootRaw+CoreString(*configManager.FastGetConfigValue(Base_Core_Configuration::SPROP_PONCTUAL_RECEIVER_FOLDER_PATH)+st_path_separator());
	st_mkdir(rootRaw);
	st_mkdir(rootRp);
	BaseReportManager::SauveRecepteursPonctuels(rootRp,*configManager.FastGetConfigValue(Base_Core_Configuration::SPROP_PONCTUAL_RECEIVER_FILE_PATH),reportFreqLbl,reportStepLbl,&configManager.recepteur_p_List);
}
