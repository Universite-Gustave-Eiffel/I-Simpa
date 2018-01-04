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

#include "baseReportManager.h"
#include "exportRecepteurSurf/rsbin.h"
#include "gabe/gabe.h"
#include <boost/filesystem.hpp>

using namespace std;

void ExportTouteBande(formatRSBIN::t_ExchangeData& mainData,std::vector<r_Surf*>& tabRecepteurS,bool divBySurface)
{
	using namespace formatRSBIN;
	for(int idRs=0;idRs<mainData.tabRsSize;idRs++)
	{
		mainData.tabRs[idRs].dataRec.xmlIndex=tabRecepteurS[idRs]->idRecepteurS;
		mainData.tabRs[idRs].dataRec.quantFaces=tabRecepteurS[idRs]->nbFaces;
		tabRecepteurS[idRs]->name.copy(mainData.tabRs[idRs].dataRec.recepteurSName,STRING_SIZE-1);
		mainData.tabRs[idRs].dataFaces = new t_ExchangeData_Face[mainData.tabRs[idRs].dataRec.quantFaces];
		//Lecture des faces
		for(int idFace=0;idFace<mainData.tabRs[idRs].dataRec.quantFaces;idFace++)
		{
			decimal aireSubRecepteurS=1.f;
			if(divBySurface)
				aireSubRecepteurS=GetAireTriangle(tabRecepteurS[idRs]->faces[idFace].sommets[0],tabRecepteurS[idRs]->faces[idFace].sommets[1],tabRecepteurS[idRs]->faces[idFace].sommets[2]);
			mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords=0;
			memcpy(&mainData.tabRs[idRs].dataFaces[idFace].dataFace.sommetsIndex,&tabRecepteurS[idRs]->faces[idFace].indiceSommets,sizeof(ivec3));
			for(int idTimeStep=0;idTimeStep<mainData.nbTimeStep;idTimeStep++)
			{
				bool somethingRecordedAtThisTimeStep=false;
				//On enregistre uniquement les pas de temps o� un niveau sonore a �t� enregistr� afin de limiter les donn�es � enregistrer
				//Calcul du nombre de pas de temps a enregistrer
				for(uentier idfreq=0;idfreq<tabRecepteurS[idRs]->faces[idFace].nbfreq && !somethingRecordedAtThisTimeStep;idfreq++)
				{
					if(tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq]!=NULL)
					{
						if(tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq][idTimeStep]!=0)
						{
							somethingRecordedAtThisTimeStep=true;
						}
					}
				}

				if(somethingRecordedAtThisTimeStep)
					mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords++;
			}
			mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep = new t_faceValue[mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords];

			int idCol=0;
			for(int idTimeStep=0;idTimeStep<mainData.nbTimeStep;idTimeStep++)
			{
				bool incrementTimeStep=false;
				for(uentier idfreq=0;idfreq<tabRecepteurS[idRs]->faces[idFace].nbfreq;idfreq++)
				{
					if(tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq]!=NULL)
					{
						if(tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq][idTimeStep]!=0)
						{
							incrementTimeStep=true;
							mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idCol].timeStep=idTimeStep;

							mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idCol].energy+=tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq][idTimeStep]/aireSubRecepteurS;
						}
					}
				}
				if(incrementTimeStep)
					idCol++;
				if(idCol>mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords)
					idCol --;
			}
		}
	}
}

void ExportSimpleBande(formatRSBIN::t_ExchangeData& mainData,std::vector<r_Surf*>& tabRecepteurS, const uentier& idfreq,bool divBySurface)
{
	using namespace formatRSBIN;
	for(int idRs=0;idRs<mainData.tabRsSize;idRs++)
	{
		mainData.tabRs[idRs].dataRec.xmlIndex=tabRecepteurS[idRs]->idRecepteurS;
		mainData.tabRs[idRs].dataRec.quantFaces=tabRecepteurS[idRs]->nbFaces;
		tabRecepteurS[idRs]->name.copy(mainData.tabRs[idRs].dataRec.recepteurSName,STRING_SIZE-1);
		mainData.tabRs[idRs].dataFaces = new t_ExchangeData_Face[mainData.tabRs[idRs].dataRec.quantFaces];
		//Lecture des faces
		for(int idFace=0;idFace<mainData.tabRs[idRs].dataRec.quantFaces;idFace++)
		{
			decimal aireSubRecepteurS=1.f;
			if(divBySurface)
				aireSubRecepteurS=GetAireTriangle(tabRecepteurS[idRs]->faces[idFace].sommets[0],tabRecepteurS[idRs]->faces[idFace].sommets[1],tabRecepteurS[idRs]->faces[idFace].sommets[2]);
			mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords=0;
			memcpy(&mainData.tabRs[idRs].dataFaces[idFace].dataFace.sommetsIndex,&tabRecepteurS[idRs]->faces[idFace].indiceSommets,sizeof(ivec3));
			for(int idTimeStep=0;idTimeStep<mainData.nbTimeStep;idTimeStep++)
			{
				if(tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq][idTimeStep]!=0)
					mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords++;
			}
			mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep = new t_faceValue[mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords];

			unsigned int idCol=0;
			for(int idTimeStep=0;idTimeStep<mainData.nbTimeStep;idTimeStep++)
			{
				if(tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq]!=NULL)
				{
					if(tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq][idTimeStep]!=0)
					{
						mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idCol].timeStep=idTimeStep;
						mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idCol].energy=tabRecepteurS[idRs]->faces[idFace].energieRecu[idfreq][idTimeStep]/aireSubRecepteurS;
						idCol++;
					}
				}
			}
		}
	}
}


BaseReportManager::BaseReportManager()
{
}


BaseReportManager::~BaseReportManager()
{
}

void BaseReportManager::SauveRecepteurPonctuel(const stringClass& recpFolder, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& timeStepLst,t_Recepteur_P* currentRecP) {

	using namespace formatGABE;
	entier nbFreq=freqLst.size();
	entier nbTimeStep=timeStepLst.size();
	entier nbFreqUsed=0;
	for(int idfreq=0;idfreq<nbFreq;idfreq++)
		if(currentRecP->energy_sum[idfreq])
			nbFreqUsed++;

	GABE binExporter(nbFreqUsed+1); //+1 pour le libell�
	binExporter.LockData(); //L'utilisateur ne pourra pas modifier les donn�es
	//Ajout du libell�
	GABE_Data_ShortString* lblCol = new GABE_Data_ShortString(nbTimeStep);
	for(int idstep=0;idstep<nbTimeStep;idstep++)
		lblCol->SetString(idstep,timeStepLst[idstep].c_str());
	lblCol->SetLabel("SPL");
	binExporter.SetCol(0,lblCol);
	//Pour chaque bande de frequence utilis�
	int idcol=1;
	for(int idfreq=0;idfreq<nbFreq;idfreq++)
	{
		if(currentRecP->energy_sum[idfreq])
		{
			//////////////////////////////////
			// Copie des donn�es de densit� d'�nergie J
			GABE_Data_Float* freqCol = new GABE_Data_Float(nbTimeStep);
			(*freqCol).SetLabel(freqLst[idfreq].c_str());
			//Copie rapide des donn�es
			//(*freqCol).FastCopy(currentRecP->energy_sum[idfreq]);

			for(int timeStp=0;timeStp<nbTimeStep;timeStp++)
			{
				*((*freqCol)[timeStp])=currentRecP->energy_sum[idfreq][timeStp]*currentRecP->cdt_vol; // Conversion W -> J/m3
			}

			binExporter.SetCol(idcol,freqCol);
			idcol++;
		}
	}
	binExporter.Save(recpFolder.c_str());
}
void BaseReportManager::SauveRecepteursPonctuels(stringClass rootFolder,const stringClass& fileName, const std::vector<stringClass>& freqLst,const std::vector<stringClass>& timeStepLst,std::vector<t_Recepteur_P*>* tabRecepteurP)
{
	//Create a new file for each receiver
	for(std::size_t idrp=0;idrp<tabRecepteurP->size();idrp++) {
		t_Recepteur_P *currentRecP = (*tabRecepteurP)[idrp];
		boost::filesystem::path recpFolder;
		uentier counter = 0;
		recpFolder = (rootFolder + currentRecP->lblRp);
		// Rename path if receivers have the same names
        while (boost::filesystem::exists(recpFolder) && counter < 20) {
			recpFolder = (rootFolder + currentRecP->lblRp + stringClass::FromInt(counter++));
		}
		currentRecP->pathRp=recpFolder.string() + st_path_separator();
        st_mkdir(recpFolder.string());
		recpFolder=recpFolder.string() + st_path_separator() + fileName;
		SauveRecepteurPonctuel(recpFolder.string(), freqLst, timeStepLst, currentRecP); 
	}

}
void BaseReportManager::SauveRecepteursSurfaciquesCoupe(stringClass rootFolder,std::vector<r_SurfCut*> tabRecepteurS,decimal timeStep,bool divBySurface, bool allfreq,uentier idfreq)
{
	if(tabRecepteurS.size()==0)
		return;
	using namespace formatRSBIN;
	t_ExchangeData mainData;
	mainData.nbTimeStep=tabRecepteurS[0]->nbtimestep;
	mainData.timeStep=timeStep;
	mainData.tabNodesSize=0;
	for(std::vector<r_SurfCut*>::iterator itrs=tabRecepteurS.begin();itrs!=tabRecepteurS.end();itrs++)
	{
		mainData.tabNodesSize+=((**itrs).nbcols+1)*((**itrs).nbrows+1);
	}
	mainData.tabRsSize=tabRecepteurS.size();
	mainData.tabNodes=new t_nodesPosition[mainData.tabNodesSize];
	mainData.tabRs = new t_ExchangeData_Recepteurs[mainData.tabRsSize];
	//Ajout des noeuds et des recepteurs surfacique
	uentier nodecount=0;
	uentier idRs=0;
	for(std::vector<r_SurfCut*>::iterator itrs=tabRecepteurS.begin();itrs!=tabRecepteurS.end();itrs++)
	{
		vec3 Bvert=(**itrs).Bvert;
		vec3 BC=(**itrs).Cvert-Bvert;
		vec3 BA=(**itrs).Avert-Bvert;
		vec3 stepU=BC/BC.length()*(**itrs).UCellSize;
		vec3 stepV=BA/BA.length()*(**itrs).VCellSize;
		uentier nbvertrow=(**itrs).nbrows+1;
		uentier nbvertcol=(**itrs).nbcols+1;
		//Ajout des noeuds

		for(uentier idnoderow=0;idnoderow<nbvertrow;idnoderow++)
		{
			for(uentier idnodecol=0;idnodecol<nbvertcol;idnodecol++)
			{
				vec3 vertcoord(Bvert+(stepU*((decimal)idnoderow)+(stepV*((decimal)idnodecol))));
				memcpy(mainData.tabNodes[nodecount+(idnodecol)+(idnoderow*nbvertcol)].node,vertcoord,sizeof(vec3));
			}
		}

		//Ajout d'un r�cepteur surfacique
		mainData.tabRs[idRs].dataRec.xmlIndex=(**itrs).idRecepteurS;
		mainData.tabRs[idRs].dataRec.quantFaces=(**itrs).nbrows*(**itrs).nbcols*2;
		tabRecepteurS[idRs]->name.copy(mainData.tabRs[idRs].dataRec.recepteurSName,STRING_SIZE-1);
		mainData.tabRs[idRs].dataFaces = new t_ExchangeData_Face[mainData.tabRs[idRs].dataRec.quantFaces];
		decimal tricell_area=2;
		if(divBySurface)
			tricell_area=GetAireTriangle(Bvert+stepV,Bvert,Bvert+stepU)*2;
		for(uentier idrow=0;idrow<(*itrs)->nbrows;idrow++)
		{
			for(uentier idcol=0;idcol<(*itrs)->nbcols;idcol++)
			{
				//Affectation des donn�es pour la surface, divis� en 2 faces
				int idFace=(idcol+(idrow*(*itrs)->nbcols))*2;
				mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords=0;
				//Indices de sommets
				ivec3 sommetstri1(nodecount+(idcol+1+(idrow*nbvertcol)),nodecount+(idcol+(idrow*nbvertcol)),nodecount+(idcol+((idrow+1)*nbvertcol)));
				memcpy(mainData.tabRs[idRs].dataFaces[idFace].dataFace.sommetsIndex,sommetstri1,sizeof(ivec3));
				sommetstri1.set(sommetstri1.a,sommetstri1.c,nodecount+(idcol+1+((idrow+1)*nbvertcol)));
				memcpy(mainData.tabRs[idRs].dataFaces[idFace+1].dataFace.sommetsIndex,sommetstri1,sizeof(ivec3));
				//Valeurs de la surface
				for(int idTimeStep=0;idTimeStep<mainData.nbTimeStep;idTimeStep++)
				{
					if(allfreq)
					{
						bool somethingrecorded=false;
						for(uentier ifreq=0;ifreq<(*itrs)->nbfreq;ifreq++)
						{
							if(!(*itrs)->data[ifreq].empty())
							{
								if((*itrs)->data[ifreq][idrow][idcol][idTimeStep]!=0)
								{
									somethingrecorded=true;
									break;
								}
							}
						}
						if(somethingrecorded)
							mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords++;
					}else{
						if((*itrs)->data[idfreq][idrow][idcol][idTimeStep]!=0)
							mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords++;
					}
				}
				mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep = new t_faceValue[mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords];
				mainData.tabRs[idRs].dataFaces[idFace+1].tabTimeStep = new t_faceValue[mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords];
				mainData.tabRs[idRs].dataFaces[idFace+1].dataFace.nbRecords=mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords;

				uentier record_count=0;
				for(int idTimeStep=0;idTimeStep<mainData.nbTimeStep;idTimeStep++)
				{
					t_cell cellData=0.;
					if(allfreq)
					{
						for(uentier ifreq=0;ifreq<(*itrs)->nbfreq;ifreq++)
						{
							if(!(*itrs)->data[ifreq].empty())
								cellData+=((*itrs)->data[ifreq][idrow][idcol][idTimeStep]);
						}
					}else{
						cellData=((*itrs)->data[idfreq][idrow][idcol][idTimeStep]);
					}
					if(cellData!=0)
					{
						mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[record_count].timeStep=idTimeStep;
						mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[record_count].energy=cellData/tricell_area;
						record_count++;
					}
				}
				memcpy(mainData.tabRs[idRs].dataFaces[idFace+1].tabTimeStep,mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep,sizeof(formatRSBIN::t_faceValue)*mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords);
			}
		}

		nodecount+=((**itrs).nbcols+1)*((**itrs).nbrows+1);
		idRs++;
	}

	//Enregistrement
	RSBIN exporter;
	exporter.ExportBIN(rootFolder.c_str(),mainData);
}
void BaseReportManager::SauveRecepteursSurfaciques(stringClass rootFolder,uentier idfreq,std::vector<r_Surf*>& tabRecepteurS,t_TetraMesh& sceneMesh,decimal timeStep,bool divBySurface)
{
	if(tabRecepteurS.size()==0 || tabRecepteurS[0]->nbFaces==0)
		return;
	using namespace formatRSBIN;

	t_ExchangeData mainData;
	mainData.nbTimeStep=tabRecepteurS[0]->faces[0].nbtimestep;
	mainData.timeStep=timeStep;

	mainData.tabNodesSize=sceneMesh.nodesSize;

	mainData.tabRsSize=tabRecepteurS.size();


	mainData.tabNodes=new t_nodesPosition[mainData.tabNodesSize];
	mainData.tabRs = new t_ExchangeData_Recepteurs[mainData.tabRsSize];

	//Ajout des noeuds
	memcpy(mainData.tabNodes,sceneMesh.nodes,sizeof(vec3)*mainData.tabNodesSize);

	//Ajout des recepteurs surfacique
	ExportSimpleBande(mainData,tabRecepteurS,idfreq,divBySurface);


	RSBIN exporter;
	exporter.ExportBIN(rootFolder.c_str(),mainData);
}


void BaseReportManager::SauveRecepteursSurfaciques(stringClass rootFolder,uentier idfreq,std::vector<r_Surf*>& tabRecepteurS,t_Mesh& sceneModel,decimal timeStep,bool divBySurface)
{
	if(tabRecepteurS.size()==0 || tabRecepteurS[0]->nbFaces==0)
		return;
	using namespace formatRSBIN;

	t_ExchangeData mainData;
	mainData.nbTimeStep=tabRecepteurS[0]->faces[0].nbtimestep;
	mainData.timeStep=timeStep;


	mainData.tabNodesSize=sceneModel.pvert_size;

	mainData.tabRsSize=tabRecepteurS.size();


	mainData.tabNodes=new t_nodesPosition[mainData.tabNodesSize];
	mainData.tabRs = new t_ExchangeData_Recepteurs[mainData.tabRsSize];

	//Ajout des noeuds
	memcpy(mainData.tabNodes,sceneModel.pvertices,sizeof(vec3)*mainData.tabNodesSize);

	//Ajout des recepteurs surfacique
	ExportSimpleBande(mainData,tabRecepteurS,idfreq,divBySurface);


	RSBIN exporter;
	exporter.ExportBIN(rootFolder.c_str(),mainData);
}

void BaseReportManager::SauveGlobalRecepteursSurfaciques(stringClass rootFolder,std::vector<r_Surf*>& tabRecepteurS,t_TetraMesh& sceneMesh,decimal timeStep,bool divBySurface)
{
	if(tabRecepteurS.size()==0 || tabRecepteurS[0]->nbFaces==0)
		return;
	using namespace formatRSBIN;

	t_ExchangeData mainData;
	mainData.nbTimeStep=tabRecepteurS[0]->faces[0].nbtimestep;
	mainData.timeStep=timeStep;

	mainData.tabNodesSize=sceneMesh.nodesSize;

	mainData.tabRsSize=tabRecepteurS.size();


	mainData.tabNodes=new t_nodesPosition[mainData.tabNodesSize];
	mainData.tabRs = new t_ExchangeData_Recepteurs[mainData.tabRsSize];

	//Ajout des noeuds
	memcpy(mainData.tabNodes,sceneMesh.nodes,sizeof(vec3)*mainData.tabNodesSize);

	//Ajout des recepteurs surfacique
	ExportTouteBande(mainData,tabRecepteurS,divBySurface);

	RSBIN exporter;
	exporter.ExportBIN(rootFolder.c_str(),mainData);
}

void BaseReportManager::InitHeaderArrays(Base_Core_Configuration& configManager, std::vector<CoreString>& lblFreq, std::vector<CoreString>& lblTime) {

	//Instanciation du tableau des libell� des champs de fr�quences
	for(std::size_t idfreq=0;idfreq<configManager.freqList.size();idfreq++)
		lblFreq.push_back(CoreString::FromInt(configManager.freqList[idfreq]->freqValue)+" Hz");
	//Instanciation du tableau des libell� des pas de temps
	decimal pasdetemps=(*configManager.FastGetConfigValue(Base_Core_Configuration::FPROP_TIME_STEP));
	int nbpasdetemps=(*configManager.FastGetConfigValue(Base_Core_Configuration::IPROP_QUANT_TIMESTEP));
	for(entier idstep=0;idstep<nbpasdetemps;idstep++)
		lblTime.push_back(CoreString::FromFloat((pasdetemps*(float)(idstep+1)*1000.f), COMMA_PRECISION_TIME_MS)+" ms");
}
void BaseReportManager::SauveGlobalRecepteursSurfaciques(stringClass rootFolder,std::vector<r_Surf*>& tabRecepteurS,t_Mesh& sceneModel,decimal timeStep,bool divBySurface)
{
	if(tabRecepteurS.size()==0 || tabRecepteurS[0]->nbFaces==0)
		return;
	using namespace formatRSBIN;

	t_ExchangeData mainData;
	mainData.nbTimeStep=tabRecepteurS[0]->faces[0].nbtimestep;
	mainData.timeStep=timeStep;

	mainData.tabNodesSize=sceneModel.pvert_size;

	mainData.tabRsSize=tabRecepteurS.size();


	mainData.tabNodes=new t_nodesPosition[mainData.tabNodesSize];
	mainData.tabRs = new t_ExchangeData_Recepteurs[mainData.tabRsSize];

	//Ajout des noeuds
	memcpy(mainData.tabNodes,sceneModel.pvertices,sizeof(vec3)*mainData.tabNodesSize);


	//Ajout des recepteurs surfacique
	ExportTouteBande(mainData,tabRecepteurS,divBySurface);

	RSBIN exporter;
	exporter.ExportBIN(rootFolder.c_str(),mainData);
}
