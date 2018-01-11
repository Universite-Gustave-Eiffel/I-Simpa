#include "reportmanager.h"
#include "tools/collision.h"
#include <cmath>
#ifdef WIN32
#include "input_output/pugixml/src/pugixml.hpp"
#endif // WIN32

const l_decimal p_0=1/pow((float)(20*pow(10.f,(int)-6)),(int)2);

l_decimal to_deciBel(const l_decimal& wjVal,const l_decimal&  Rho)
{
	return 10*log10(Rho*wjVal*p_0);
}


	/**
	   Calculate the intersection of a ray and a sphere
	   The line segment is defined from p1 to p2
	   The sphere is of radius r and centered at sc
	   There are potentially two points of intersection given by
	   p = p1 + mu1 (p2 - p1)
	   p = p1 + mu2 (p2 - p1)
	   @return Faux if the ray doesn't intersect the sphere.
	   @see http://local.wasp.uwa.edu.au/~pbourke/geometry/sphereline/
	*/
	int RaySphere(const vec3& p1,const vec3& p2,const vec3& sc,double r,double *mu1,double *mu2)
	{
	   double a,b,c;
	   double bb4ac;
	   vec3 dp;

	   dp.x = p2.x - p1.x;
	   dp.y = p2.y - p1.y;
	   dp.z = p2.z - p1.z;
	   a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
	   b = 2 * (dp.x * (p1.x - sc.x) + dp.y * (p1.y - sc.y) + dp.z * (p1.z - sc.z));
	   c = sc.x * sc.x + sc.y * sc.y + sc.z * sc.z;
	   c += p1.x * p1.x + p1.y * p1.y + p1.z * p1.z;
	   c -= 2 * (sc.x * p1.x + sc.y * p1.y + sc.z * p1.z);
	   c -= r * r;
	   bb4ac = b * b - 4 * a * c;
	   if (abs(a) < EPSILON || bb4ac < 0) {
		  *mu1 = 0;
		  *mu2 = 0;
		  return false;
	   }

	   *mu1 = (-b + sqrt(bb4ac)) / (2 * a);
	   *mu2 = (-b - sqrt(bb4ac)) / (2 * a);

	   return true;
	}

using namespace std;

inline bool ContainsRP(t_Recepteur_P* recepteurTest, std::vector<t_Recepteur_P*>* rpList)
{
	for(std::size_t idRP=0;idRP<rpList->size();idRP++)
	{
		if(rpList->at(idRP)==recepteurTest)
			return true;
	}
	return false;
}

ReportManager::ReportManager(t_ParamReport& _paramReport)
:BaseReportManager()
{
	paramReport=_paramReport;
	nbSource = paramReport.configManager->srcList.size();
	timeStepInSourceOutput = (bool)*paramReport.configManager->FastGetConfigValue(Core_Configuration::I_PROP_OUTPUT_RECEIVER_BY_SOURCE);
	tabEnergyByTimeStep=new l_decimal[_paramReport.nbTimeStep];
	memset(tabEnergyByTimeStep,0,sizeof(l_decimal)*_paramReport.nbTimeStep);
	lst_rp_lef.insert(lst_rp_lef.begin(),_paramReport.configManager->recepteur_p_List.size(),t_rp_lef());
	for(uentier idrp=0;idrp<lst_rp_lef.size();idrp++)
	{
		lst_rp_lef[idrp].Init(_paramReport.nbTimeStep,nbSource, timeStepInSourceOutput);
	}

	particleFile = NULL;
	particleSurfaceCSVFile = NULL;
    particleReceiverCSVFile = NULL;
	lastParticuleFileHeaderInfo=0;

	if(paramReport.nbParticles!=0)
		writeParticleFile();
	
}

void ReportManager::writeParticleFile()
{
	if(particleFile)
		delete particleFile;
	if(particleSurfaceCSVFile)
		delete particleSurfaceCSVFile;
    if (particleReceiverCSVFile)
        delete particleReceiverCSVFile;
    // Create particules folder
	st_mkdir(paramReport._particlePath);
	stringClass freqFolder;
	freqFolder=paramReport._particlePath+stringClass::FromInt(paramReport.freqValue)+st_path_separator();

	st_mkdir(freqFolder);
	stringClass fileNamePath=freqFolder+paramReport._particleFileName;
	
	#ifdef WIN32
		particleFile = new fstream(pugi::as_wide(fileNamePath), ios::out | ios::binary);
	#else
		particleFile = new fstream(fileNamePath.c_str(), ios::out | ios::binary);
	#endif // WIN


	stringClass fileCSVNamePath=freqFolder+"particle_surface_collision_statistics.csv";
    stringClass fileReceiversCSVNamePath = freqFolder + "particle_receivers_collision_statistics.csv";
    if(*(this->paramReport.configManager->FastGetConfigValue(Core_Configuration::I_PROP_SAVE_SURFACE_INTERSECTION))) {

		#ifdef WIN32
		particleSurfaceCSVFile = new fstream(pugi::as_wide(fileCSVNamePath), ios::out);
		#else
		particleSurfaceCSVFile = new fstream(fileCSVNamePath.c_str(), ios::out);
		#endif // WIN
    }
    if (*(this->paramReport.configManager->FastGetConfigValue(Core_Configuration::I_PROP_SAVE_RECEIVER_INTERSECTION))) {

		#ifdef WIN32
		particleReceiverCSVFile = new fstream(pugi::as_wide(fileReceiversCSVNamePath), ios::out);
		#else
		particleReceiverCSVFile = new fstream(fileReceiversCSVNamePath.c_str(), ios::out);
		#endif // WIN
    }

	enteteSortie.nbParticles=paramReport.nbParticles;
	enteteSortie.nbTimeStepMax=paramReport.nbTimeStep;
	nbPasDeTempsMax=paramReport.nbTimeStep;
	enteteSortie.particleInfoLength=sizeof(binaryPTimeStep);
	enteteSortie.particleHeaderInfoLength=sizeof(binaryPHeader);
	enteteSortie.fileInfoLength=sizeof(binaryFHeader);
	enteteSortie.formatVersion=PARTICLE_BINARY_VERSION_INFORMATION;
	enteteSortie.timeStep=paramReport.timeStep;
	lastParticuleFileHeaderInfo=particleFile->tellp();
	particleFile->write((char*)&enteteSortie,sizeof(binaryFHeader));
	*particleSurfaceCSVFile<<"id,collision coordinate,face normal,reflection order,incident vector,energy"<<std::endl;
    *particleReceiverCSVFile << "time(s),receiver name,incident vector x,incident vector y,incident vector z,energy * dist" << std::endl;
	realNbParticle=0;

}


ReportManager::~ReportManager()
{
	this->SaveAndCloseParticleFile();
	delete[] tabEnergyByTimeStep;
}

void ReportManager::RecordTimeStep(CONF_PARTICULE& particleInfos)
{
	if(particleInfos.outputToParticleFile )
	{
		if(firstTimeStep==-1)
			firstTimeStep=particleInfos.pasCourant;
		positionsCurrentParticule.push_back(binaryPTimeStep(particleInfos.position,particleInfos.energie));
	}
	if(particleInfos.pasCourant<paramReport.nbTimeStep)
	{
		tabEnergyByTimeStep[particleInfos.pasCourant]+=particleInfos.energie;
	}
}

void ReportManager::ParticuleFreeTranslation(CONF_PARTICULE& particleInfos, const vec3& nextPosition)
{
	vec3 direction(nextPosition-particleInfos.position);
	//Si dans le mod�le un r�cepteur plan a �t� d�clar�


	if(particleInfos.currentTetra->linkedCutMap)
	{
		for(std::vector<r_SurfCut*>::iterator itrs=particleInfos.currentTetra->linkedCutMap->begin();itrs!=particleInfos.currentTetra->linkedCutMap->end();itrs++)
		{
			float t,u,v;
			if(collision_manager::intersect_parallelogram(particleInfos.position,direction,(*itrs)->Bvert,(*itrs)->Cvert,(*itrs)->Avert,&t,&u,&v) && t>=0 && t<=1)
			{
				uentier CellRow=(uentier)floorf(u*(*itrs)->NbCellU);
				uentier CellCol=(uentier)floorf(v*(*itrs)->NbCellV);
				vec3 normal=(*itrs)->planeNormal;
				if(particleInfos.direction.dot(normal)<0) //si la face du recepteur est orient�e dans l'autre direction on inverse la normal
					normal*=-1;
				if(*(this->paramReport.configManager->FastGetConfigValue(Core_Configuration::I_PROP_SURFACE_RECEIVER_MODE))==0)
					(*itrs)->data[particleInfos.frequenceIndex][CellRow][CellCol][particleInfos.pasCourant]+=particleInfos.energie*cosf(normal.angle(particleInfos.direction));
				else
					(*itrs)->data[particleInfos.frequenceIndex][CellRow][CellCol][particleInfos.pasCourant]+=particleInfos.energie;
			}
		}
	}

	//Si le prochain tetrah�dre contient un ou des recepteurs ponctuel
	if(particleInfos.currentTetra->linkedRecepteurP)
	{

		for(std::size_t idrecp=0;idrecp<particleInfos.currentTetra->linkedRecepteurP->size();idrecp++)
		{
			t_Recepteur_P* currentRecp=particleInfos.currentTetra->linkedRecepteurP->at(idrecp);
			vec3 closestPointDuringPropagation=currentRecp->position.closestPointOnSegment(particleInfos.position,nextPosition);
			if(closestPointDuringPropagation.distance(currentRecp->position)<*paramReport.configManager->FastGetConfigValue(Core_Configuration::FPROP_RAYON_RECEPTEURP))
			{
				//Calcul de la longueur de croisement
				l_decimal mu1,mu2;

				if(RaySphere(particleInfos.position,nextPosition,currentRecp->position,*paramReport.configManager->FastGetConfigValue(Core_Configuration::FPROP_RAYON_RECEPTEURP),&mu1,&mu2))
				{
					if(mu2<0)
						mu2=0;
					else if(mu2>1)
						mu2=1;
					if(mu1<0)
						mu1=0;
					else if(mu1>1)
						mu1=1;
					float norm_dir=(direction).length();
					l_decimal Lintersect=abs(mu2-mu1)*norm_dir;
					if(Lintersect>0)
					{
						l_decimal cosphi=cos(M_PIDIV2-direction.angle(currentRecp->orientation));
						const l_decimal energy=particleInfos.energie*Lintersect;
						currentRecp->energy_sum[particleInfos.frequenceIndex][particleInfos.pasCourant]+=energy;

						lst_rp_lef[currentRecp->idrp].Lf[particleInfos.pasCourant]+=energy*pow(cosphi,2);
						lst_rp_lef[currentRecp->idrp].Lfc[particleInfos.pasCourant]+=energy*fabs(cosphi);

						vec3 particleIntensity((particleInfos.direction/particleInfos.direction.length())*energy);
						lst_rp_lef[currentRecp->idrp].intensity[particleInfos.pasCourant]+=veci_t(particleIntensity.x,particleIntensity.y,particleIntensity.z);
						if(timeStepInSourceOutput) {
							lst_rp_lef[currentRecp->idrp].SrcContrib[particleInfos.pasCourant*nbSource + particleInfos.sourceid]+=energy;
						} else {
							lst_rp_lef[currentRecp->idrp].SrcContrib[particleInfos.sourceid]+=energy;
						}
                        if (particleInfos.outputToParticleFile && *(this->paramReport.configManager->FastGetConfigValue(Core_Configuration::I_PROP_SAVE_RECEIVER_INTERSECTION)))
                        {
                            //Add intersection to history
                            decimal time = particleInfos.pasCourant * *this->paramReport.configManager->FastGetConfigValue(Base_Core_Configuration::FPROP_TIME_STEP) + particleInfos.elapsedTime;
                            this->receiverCollisionHistory.push_back(t_receiver_collision_history(time, particleInfos.direction, energy * currentRecp->cdt_vol, currentRecp->idrp));
                        }
					}
				}
			}
		}
	}
}
void ReportManager::ParticuleGoToNextTetrahedra(CONF_PARTICULE& particleInfos,t_Tetra* nextTetra)
{

}


void ReportManager::ParticuleCollideWithSceneMesh(CONF_PARTICULE& particleInfos)
{
	if(particleInfos.nextModelIntersection.idface==-1)
		return;


	//Retrieve face data ptr
	#ifdef UTILISER_MAILLAGE_OPTIMISATION
	t_Tetra_Faces* face=&particleInfos.currentTetra->faces[particleInfos.nextModelIntersection.idface];
	//If the associated face is a surface receiver
	vec3& normal=face->face_scene->normal;
	if(particleInfos.outputToParticleFile && *(this->paramReport.configManager->FastGetConfigValue(Core_Configuration::I_PROP_SAVE_SURFACE_INTERSECTION)) && face->face_scene!=NULL)
	{
		particleInfos.reflectionOrder++;
		//Add intersection to history
		this->collisionHistory.push_back(t_collision_history(face->face_scene->normal,particleInfos.reflectionOrder,particleInfos.nextModelIntersection.collisionPosition,particleInfos.direction, particleInfos.energie));
	}
	#else
	t_cFace* face=&paramReport.sceneModel->pfaces[particleInfos.nextModelIntersection.idface];
	vec3& normal=face->normal;
	#endif

	if(face->recepteurS)
	{
		if(particleInfos.direction.dot(normal)<0) //If the receiver surface is on the other side then revert the normal
			normal*=-1;
		//Add particle energy to receiver
		if(*(this->paramReport.configManager->FastGetConfigValue(Core_Configuration::I_PROP_SURFACE_RECEIVER_MODE))==0)
			face->recepteurS->energieRecu[particleInfos.frequenceIndex][particleInfos.pasCourant]+=particleInfos.energie*cosf(normal.angle(particleInfos.direction));
		else
			face->recepteurS->energieRecu[particleInfos.frequenceIndex][particleInfos.pasCourant]+=particleInfos.energie;
	}
}

void ReportManager::SaveParticule()
{
	CloseLastParticleHeader();
}
void ReportManager::CloseLastParticleHeader()
{
	if(!positionsCurrentParticule.empty())
	{
		realNbParticle++;
		binaryPHeader pHeaderInfo(positionsCurrentParticule.size(),firstTimeStep);
		particleFile->write((char*)&pHeaderInfo,sizeof(binaryPHeader));
		for(uentier idtime=0;idtime<pHeaderInfo.nbTimeStep;idtime++)
			particleFile->write( (char*)&positionsCurrentParticule[idtime],sizeof(binaryPTimeStep));
		positionsCurrentParticule.clear();
	}
	if(!this->collisionHistory.empty())
	{
		//Update CSV file
		if(this->particleSurfaceCSVFile!=NULL)
		{
			while(!this->collisionHistory.empty())
			{
				t_collision_history& part_event=this->collisionHistory.front();
				*this->particleSurfaceCSVFile<<this->realNbParticle<<","
					<<part_event.collisionCoordinate.x<<" "<<part_event.collisionCoordinate.y<<" "<<part_event.collisionCoordinate.z<<","
					<<part_event.faceNormal.x<<" "<<part_event.faceNormal.y<<" "<<part_event.faceNormal.z<<","
					<<part_event.reflexionOrder<<","
					<<part_event.incidentVector.x<<" "<<part_event.incidentVector.y<<" "<<part_event.incidentVector.z<<","<<part_event.energy
					<<std::endl;
				this->collisionHistory.pop_front();
			}
		}
	}
    if (!this->receiverCollisionHistory.empty())
    {
        //Update CSV file
        if (this->particleReceiverCSVFile != NULL)
        {
            while (!this->receiverCollisionHistory.empty())
            {
                t_receiver_collision_history& part_event = this->receiverCollisionHistory.front();
                *this->particleReceiverCSVFile << part_event.time << "," << this->paramReport.configManager->recepteur_p_List.at(part_event.idrp)->lblRp << "," << part_event.incidentVector.x << "," << part_event.incidentVector.y << "," << part_event.incidentVector.z << "," << part_event.energy
                    << std::endl;
                this->receiverCollisionHistory.pop_front();
            }
        }
    }
}

void ReportManager::CloseLastParticleFileHeader()
{

    particleFile->seekp(lastParticuleFileHeaderInfo);
    enteteSortie.nbParticles=realNbParticle;
    particleFile->write((char*)&enteteSortie,sizeof(binaryFHeader));
}

formatGABE::GABE_Object* ReportManager::GetColStats()
{
	using namespace formatGABE;
	GABE_Data_Integer* statValues=new GABE_Data_Integer(7);
	statValues->SetLabel((CoreString::FromInt(paramReport.freqValue)+" Hz").c_str());
	statValues->Set(0,statReport.partAbsAtmo);
	statValues->Set(1,statReport.partAbsSurf);
	statValues->Set(2,statReport.partAbsEncombrement);
	statValues->Set(3,statReport.partLoop);
	statValues->Set(4,statReport.partLost);
	statValues->Set(5,statReport.partAlive);
	statValues->Set(6,statReport.partTotal);

	return statValues;
}
void ReportManager::FillWithLefData(t_sppsThreadParam& data)
{
	using namespace formatGABE;
	//For each punctual receiver fill the recorded data collected while propagating
	data.GabeSumEnergyCosPhi.reserve(lst_rp_lef.size());
	data.GabeSumEnergyCosSqrtPhi.reserve(lst_rp_lef.size());
	for(uentier idrecp=0;idrecp<lst_rp_lef.size();idrecp++)
	{
		float volRp=(pow(*this->paramReport.configManager->FastGetConfigValue(Core_Configuration::FPROP_RAYON_RECEPTEURP),3)*M_PI*4.)/3.;
		t_rp_lef* currentRp=&lst_rp_lef[idrecp];
		t_Recepteur_P* currentConfigRp=paramReport.configManager->recepteur_p_List[idrecp];
		GABE_Data_Float* energyCosSqrtPhi=new GABE_Data_Float(paramReport.nbTimeStep);
		GABE_Data_Float* energyCosPhi=new GABE_Data_Float(paramReport.nbTimeStep);
		GABE_Data_Float* GabeIntensity[3];
		GABE_Data_Float* energyBySrc=new GABE_Data_Float(paramReport.configManager->srcList.size());
		for(short dim=0;dim<3;dim++)
		{
			GabeIntensity[dim]=new GABE_Data_Float(paramReport.nbTimeStep+1); //+1 pour le cumul
		}
		CoreString lblcol=(CoreString::FromInt(paramReport.freqValue));
		GabeIntensity[0]->SetLabel((lblcol+" Hz\nx").c_str());
		GabeIntensity[1]->SetLabel((lblcol+" Hz\ny").c_str());
		GabeIntensity[2]->SetLabel((lblcol+" Hz\nz").c_str());
		energyBySrc->SetLabel((lblcol+" Hz").c_str());

		dvec3 cumulIntensity;
		for(uentier idstep=0;idstep<paramReport.nbTimeStep;idstep++)
		{
			energyCosSqrtPhi->Set(idstep,currentRp->Lfc[idstep]*currentConfigRp->cdt_vol);
			energyCosPhi->Set(idstep,currentRp->Lf[idstep]*currentConfigRp->cdt_vol);
			GabeIntensity[0]->Set(idstep,currentRp->intensity[idstep].x/volRp);
			GabeIntensity[1]->Set(idstep,currentRp->intensity[idstep].y/volRp);
			GabeIntensity[2]->Set(idstep,currentRp->intensity[idstep].z/volRp);
			cumulIntensity+=currentRp->intensity[idstep]/volRp;
		}
		GabeIntensity[0]->Set(paramReport.nbTimeStep,cumulIntensity[0]);
		GabeIntensity[1]->Set(paramReport.nbTimeStep,cumulIntensity[1]);
		GabeIntensity[2]->Set(paramReport.nbTimeStep,cumulIntensity[2]);
		for(uentier idsrc=0;idsrc<energyBySrc->GetSize();idsrc++)
		{
			l_decimal sum = 0;
			// If the energy was stored by timestep, sum all results
			if(this->timeStepInSourceOutput) {
				for(uentier idstep=0;idstep<paramReport.nbTimeStep;idstep++)
				{
					sum += currentRp->SrcContrib[idstep * nbSource + idsrc]; 	
				}
			} else {
				sum = currentRp->SrcContrib[idsrc];
			}
			energyBySrc->Set(idsrc,sum*currentConfigRp->cdt_vol);
		}
		data.GabeSumEnergyCosPhi.push_back(energyCosPhi);
		data.GabeSumEnergyCosSqrtPhi.push_back(energyCosSqrtPhi);
		data.GabeIntensity[0].push_back(GabeIntensity[0]);
		data.GabeIntensity[1].push_back(GabeIntensity[1]);
		data.GabeIntensity[2].push_back(GabeIntensity[2]);
		data.GabeSlPerSrc.push_back(energyBySrc);
		if(this->timeStepInSourceOutput) {
			l_decimal* srcContribCopy = new l_decimal[nbSource*paramReport.nbTimeStep];
			memcpy(srcContribCopy, currentRp->SrcContrib, sizeof(l_decimal) * nbSource * paramReport.nbTimeStep); 
			data.SrcContrib.push_back(srcContribCopy);
		}
	}
}

formatGABE::GABE_Data_Float* ReportManager::GetSumEnergy()
{
	l_decimal cdtRho=(*(paramReport.configManager->FastGetConfigValue(Base_Core_Configuration::FPROP_RHO)))*(*(paramReport.configManager->FastGetConfigValue(Base_Core_Configuration::FPROP_CELERITE)));
	using namespace formatGABE;
	GABE_Data_Float* statValues=new GABE_Data_Float(paramReport.nbTimeStep);
	statValues->SetLabel((CoreString::FromInt(paramReport.freqValue)+" Hz").c_str());

	for(uentier idstep=0;idstep<paramReport.nbTimeStep;idstep++)
	{
		statValues->Set(idstep,(Floatb)(tabEnergyByTimeStep[idstep]*cdtRho));
	}
	return statValues;
}
void ReportManager::SaveAndCloseParticleFile()
{
	//On compl�te l'entete du fichier
	if(particleFile!=NULL)
	{
		CloseLastParticleFileHeader();
		//Fermeture du fichier de particules
		if(particleFile)
		{
			particleFile->close();
			fstream* tmp=particleFile;
			particleFile=NULL;
			delete tmp;
		}
	}
	if(particleSurfaceCSVFile!=NULL)
	{
		particleSurfaceCSVFile->close();
		fstream* tmp=particleSurfaceCSVFile;
		particleSurfaceCSVFile=NULL;
		delete tmp;
	}
    if (particleReceiverCSVFile != NULL)
    {
        particleReceiverCSVFile->close();
        fstream* tmp = particleReceiverCSVFile;
        particleReceiverCSVFile = NULL;
        delete tmp;
    }
}

void ReportManager::NewParticule(CONF_PARTICULE& particleInfos)
{
	if(particleFile!=NULL && particleInfos.outputToParticleFile)
	{
		//Dump old data, save 
		positionsCurrentParticule.clear();
		positionsCurrentParticule.reserve(paramReport.nbTimeStep);
		firstTimeStep=-1;
	}
}


void ReportManager::SaveThreadsStats(const CoreString& filename,const CoreString& filenamedBLvl,std::vector<t_sppsThreadParam>& cols,const t_ParamReport& params)
{
	/////////////////////////////////////
	// 1: Sauvegarde des statistiques des �tats finaux des particules

	using namespace formatGABE;

	GABE_Data_ShortString* statLbl=new GABE_Data_ShortString(7);
	statLbl->SetString(0,_("Particles absorbed by the atmosphere"));
	statLbl->SetString(1,_("Particles absorbed by the materials"));
	statLbl->SetString(2,_("Particles absorbed by the fittings"));
	statLbl->SetString(3,_("Particles lost by infinite loops"));
	statLbl->SetString(4,_("Particles lost by meshing problems"));
	statLbl->SetString(5,_("Particles remaining at the end of the calculation"));
	statLbl->SetString(6,_("Total"));
	uentier nbfreqUsed=0;
	for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
	{
		if(cols[idfreq].GabeColData)
			nbfreqUsed++;
	}
	GABE exportTab(nbfreqUsed+1);
	exportTab.LockData();
	exportTab.SetCol(0,statLbl);

	uentier currentIndex=1;
	for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
	{
		if(cols[idfreq].GabeColData)
		{
			exportTab.SetCol(currentIndex,cols[idfreq].GabeColData);
			currentIndex++;
		}
	}

	exportTab.Save(filename.c_str());


	/////////////////////////////////////
	// 2: Sauvegarde des statistiques du niveau sonore en fonction du temps (m�me forme que pour un r�cepteur ponctuel)
	GABE exportdBLevelTab(nbfreqUsed+1); //+1 pour la colonne des libell�s
	//Cr�ation de la colonne des libell�s
	GABE_Data_ShortString* statdBLbl=new GABE_Data_ShortString(params.nbTimeStep);
	for(uentier idstep=0;idstep<params.nbTimeStep;idstep++)
		statdBLbl->SetString(idstep,(CoreString::FromInt((int)(params.timeStep*(idstep+1)*1000))+" ms").c_str());
	statdBLbl->SetLabel("SPL");
	exportdBLevelTab.SetCol(0,statdBLbl);
	currentIndex=1;
	for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
	{
		if(cols[idfreq].GabeSumEnergyFreq)
		{
			exportdBLevelTab.SetCol(currentIndex,cols[idfreq].GabeSumEnergyFreq);
			currentIndex++;
		}
	}
	exportdBLevelTab.Save(filenamedBLvl.c_str());
}
void ReportManager::SetPostProcessSurfaceReceiver(Core_Configuration& coreConfig ,const std::size_t& freqIndex,std::vector<r_Surf*>& surface_receiver_list,const float& timeStep)
{
	if(*coreConfig.FastGetConfigValue(Core_Configuration::I_PROP_SURFACE_RECEIVER_MODE)==1)
	{
		float i0_div_p0sqr=pow(10.f,-12.f)/pow((float)(20*pow(10.f,(int)-6)),(int)2)*(*coreConfig.FastGetConfigValue(Core_Configuration::FPROP_RHO))*(*coreConfig.FastGetConfigValue(Core_Configuration::FPROP_CELERITE));
		for(std::vector<r_Surf*>::iterator itrs=surface_receiver_list.begin();itrs!=surface_receiver_list.end();itrs++)
		{
			r_Surf& currentRs=**itrs;
			for(unsigned int idFace=0;idFace<currentRs.nbFaces;idFace++)
			{
				r_Surf_Face& curFace(currentRs.faces[idFace]);
				for(std::size_t idTimeStep=0;idTimeStep<curFace.nbtimestep;idTimeStep++)
					curFace.energieRecu[freqIndex][idTimeStep]*=i0_div_p0sqr;
			}
		}
	}
}
void ReportManager::SetPostProcessCutSurfaceReceiver(Core_Configuration& coreConfig ,const std::size_t& freqIndex,std::vector<r_SurfCut*>& surface_receiver_list,const float& timeStep)
{
	if(*coreConfig.FastGetConfigValue(Core_Configuration::I_PROP_SURFACE_RECEIVER_MODE)==1)
	{
		float i0_div_p0sqr(pow(10.f,-12.f)/pow((float)(20*pow(10.f,(int)-6)),(int)2)*(*coreConfig.FastGetConfigValue(Core_Configuration::FPROP_RHO))*(*coreConfig.FastGetConfigValue(Core_Configuration::FPROP_CELERITE)));
		for(std::vector<r_SurfCut*>::iterator itrs=surface_receiver_list.begin();itrs!=surface_receiver_list.end();itrs++)
		{
			r_SurfCut& currentRs(**itrs);
			//Ajout des noeuds
			for(uentier idrow=0;idrow<currentRs.nbrows;idrow++)
			{
				for(uentier idcol=0;idcol<currentRs.nbcols;idcol++)
				{
					for(unsigned int idTimeStep=0;idTimeStep<currentRs.nbtimestep;idTimeStep++)
					{
						currentRs.data[freqIndex][idrow][idcol][idTimeStep]*=i0_div_p0sqr;
					}
				}
			}
		}
	}
}
void ReportManager::SaveSoundLevelBySource(const CoreString& filename,std::vector<t_sppsThreadParam>& cols,const t_ParamReport& params)
{
	using namespace formatGABE;
	uentier nbfreqUsed=0;
	uentier nbfreqMax=params.configManager->freqList.size();
	for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
	{
		if(cols[idfreq].GabeColData)
			nbfreqUsed++;
	}

	////////////////////////////////////////////
	// Source labels
	////////////////////////////////////////////
	GABE_Data_ShortString collbl(params.configManager->srcList.size());
	for(uentier idsrc=0;idsrc<collbl.GetSize();idsrc++)
		collbl.SetString(idsrc,params.configManager->srcList[idsrc]->sourceName.c_str());
	collbl.SetLabel("SPL");

	//For each punctual receiver
	for(uentier idrecp=0;idrecp<params.configManager->recepteur_p_List.size();idrecp++)
	{
		t_Recepteur_P* currentRP=params.configManager->recepteur_p_List[idrecp];
		GABE recepteurPonctData(nbfreqUsed+1);
		recepteurPonctData.SetCol(0,collbl);
		int idcol=1;
		for(uentier idfreq=0;idfreq<nbfreqMax;idfreq++)
		{
			if(params.configManager->freqList[idfreq]->doCalculation)
			{
				recepteurPonctData.SetCol(idcol,*(cols[idfreq].GabeSlPerSrc[idrecp]));
				idcol++;
			}
		}
		recepteurPonctData.LockData();
		recepteurPonctData.Save((currentRP->pathRp+filename).c_str());
	}

	/////////////////////////////////////
	// Save one impulse response by source
	/////////////////////////////////////
	if(*params.configManager->FastGetConfigValue(Core_Configuration::I_PROP_OUTPUT_RECEIVER_BY_SOURCE) != 0) {
		int srcCount = params.configManager->srcList.size();
		//Instanciate the array of frequency labels
		std::vector<CoreString> reportFreqLbl;
		//Instanciate the array of time steps labels
		std::vector<CoreString> reportStepLbl;
		BaseReportManager::InitHeaderArrays(*params.configManager, reportFreqLbl, reportStepLbl);
		CoreString workingDir = *params.configManager->FastGetConfigValue(Core_Configuration::SPROP_CORE_WORKING_DIRECTORY);
		for(int idsource=0; idsource < srcCount; idsource++) {
			//For each punctual receiver
			for(uentier idrecp=0;idrecp<params.configManager->recepteur_p_List.size();idrecp++)
			{
				CoreString rootRp=workingDir+CoreString(*params.configManager->FastGetConfigValue(Base_Core_Configuration::SPROP_PONCTUAL_RECEIVER_FOLDER_PATH)+st_path_separator());
				t_Recepteur_P* currentRP=params.configManager->recepteur_p_List[idrecp];
				t_Recepteur_P receiverData(nbfreqMax,params.nbTimeStep);
				// Create the content
				// Sound level by timestep and frequency
				for(uentier idfreq=0;idfreq<nbfreqMax;idfreq++)
				{
					if(params.configManager->freqList[idfreq]->doCalculation)
					{
						receiverData.InitFreq(idfreq);
						for(uentier idStep = 0; idStep < params.nbTimeStep; idStep ++) {
							double energy = cols[idfreq].SrcContrib[idrecp][idStep * srcCount + idsource];
							receiverData.energy_sum[idfreq][idStep] = energy;
						}
					}
				}
				receiverData.lblRp = currentRP->lblRp;
				receiverData.cdt_vol = currentRP->cdt_vol;
				CoreString file = rootRp + receiverData.lblRp + "/" + params.configManager->srcList[idsource]->sourceName + st_path_separator();
				st_mkdir(file);
				file += *params.configManager->FastGetConfigValue(Base_Core_Configuration::SPROP_PONCTUAL_RECEIVER_FILE_PATH);
				BaseReportManager::SauveRecepteurPonctuel(file, reportFreqLbl, reportStepLbl, &receiverData); 
			}
		}
	}
}
void ReportManager::SaveRecpIntensity(const CoreString& filename,std::vector<t_sppsThreadParam>& cols,const t_ParamReport& params)
{
	using namespace formatGABE;

	// Dans le format GABE on doit pr�ciser le nombre de colonnes � la construction
	// et le nombre de colonne correspond au nombre de bande de fr�quence*3+1
	//
	uentier nbfreqUsed=0;
	uentier nbfreqMax=params.configManager->freqList.size();
	for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
	{
		if(cols[idfreq].GabeColData)
			nbfreqUsed++;
	}

	////////////////////////////////////////////
	// S�rie Libell� des pas de temps
	////////////////////////////////////////////
	GABE_Data_ShortString collbl(params.nbTimeStep+1); //+1 pour le cumul
	GABE_Data_ShortString* statdBLbl=&collbl;
	for(uentier idstep=0;idstep<params.nbTimeStep;idstep++)
		statdBLbl->SetString(idstep,(CoreString::FromInt((int)(params.timeStep*(idstep+1)*1000))+" ms").c_str());
	statdBLbl->SetString(params.nbTimeStep,"Sum");
	statdBLbl->SetLabel("Intensity");

	for(uentier idrecp=0;idrecp<params.configManager->recepteur_p_List.size();idrecp++)
	{
		t_Recepteur_P* currentRP=params.configManager->recepteur_p_List[idrecp];
		GABE recepteurPonctData(nbfreqUsed*3+1);

		recepteurPonctData.SetCol(0,collbl);

		int idcol=1;
		for(std::size_t idfreq=0;idfreq<params.configManager->freqList.size();idfreq++)
		{
			if(params.configManager->freqList[idfreq]->doCalculation)
			{
				recepteurPonctData.SetCol(idcol,*(cols[idfreq].GabeIntensity[0][idrecp])); //x
				idcol++;
				recepteurPonctData.SetCol(idcol,*(cols[idfreq].GabeIntensity[1][idrecp])); //y
				idcol++;
				recepteurPonctData.SetCol(idcol,*(cols[idfreq].GabeIntensity[2][idrecp])); //z
				idcol++;
			}
		}
		recepteurPonctData.LockData();
		recepteurPonctData.Save((currentRP->pathRp+filename).c_str());
	}
	//////////////////////////////////////////////////
	// Enregistrement des animations
	std::vector<GABE_Object*> gabe_cols;
	GABE_Data_Integer serie_int_parameter(4);
	GABE_Data_Float serie_float_parameter(1);
	gabe_cols.push_back(&serie_int_parameter);
	gabe_cols.push_back(&serie_float_parameter);
	serie_int_parameter.Set(0,params.configManager->recepteur_p_List.size());	//Nombre de r�cepteur ponctuel
	serie_int_parameter.Set(1,params.nbTimeStep);								//Nombre de pas de temps
	serie_int_parameter.Set(2,3);												//Nombre de colonne d�finissant un r�cepteur ponctuel
	serie_int_parameter.Set(3,gabe_cols.size());						    	//Num�ro de la colonne du premier r�cepteur ponctuel
	serie_float_parameter.Set(0,params.timeStep);								//Pas de temps (s)

	CoreString workpath=params.working_Path+_("Intensity animation")+"/";
	st_mkdir(workpath);
	//For each frequency band

	for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
	{
		if(params.configManager->freqList[idfreq]->doCalculation)
		{
			std::vector<GABE_Object*> freq_gabe_cols;
			CoreString freqdir=workpath+CoreString::FromInt(params.configManager->freqList[idfreq]->freqValue)+" Hz/";

			st_mkdir(freqdir);
			for(uentier idrecp=0;idrecp<params.configManager->recepteur_p_List.size();idrecp++)
			{
				t_Recepteur_P* currentRP=params.configManager->recepteur_p_List[idrecp];
				for(short dim=0;dim<3;dim++)
				{
					GABE_Data_Float* rp_data=cols[idfreq].GabeIntensity[dim][idrecp];
					unsigned int nbrecords=rp_data->GetSize();
					GABE_Data_Float* serie_pos=new GABE_Data_Float(nbrecords+1);
					freq_gabe_cols.push_back(serie_pos);
					//Position du r�cepteur
					serie_pos->Set(0,currentRP->position[dim]);
					for(unsigned int idstep=0;idstep<nbrecords;idstep++)
						serie_pos->Set(idstep+1,rp_data->GetValue(idstep));

				}
			}
			//Enregistrement du fichier
			std::size_t nbheaders_cols=gabe_cols.size();
			GABE recorder(nbheaders_cols+freq_gabe_cols.size());
			for(std::size_t idcol=0;idcol<nbheaders_cols;idcol++)
				recorder.SetCol(idcol,*gabe_cols[idcol]); //cr�� une copie
			for(std::size_t idcol=0;idcol<freq_gabe_cols.size();idcol++)
				recorder.SetCol(nbheaders_cols+idcol,freq_gabe_cols[idcol]);
			recorder.LockData();
			recorder.Save(CoreString(freqdir+"Intensity.rpi").c_str());
		}
	}

}
void ReportManager::SaveRecpAcousticParamsAdvance(const CoreString& filename,std::vector<t_sppsThreadParam>& cols,const t_ParamReport& params)
{
	using namespace formatGABE;
	uentier nbfreqUsed=0;
	uentier nbfreqMax=params.configManager->freqList.size();
	for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
	{
		if(cols[idfreq].GabeColData)
			nbfreqUsed++;
	}

	const uentier nbFixedCols=4;		//Columns Index,Parameters,Source,Frequencies
	const uentier nbRecpHeaderCols=1; //Column noise
	const uentier nbColsByFreq=3;		//Column energy,LF,LFC
	const uentier nbCols=nbFixedCols+nbRecpHeaderCols+nbfreqUsed*nbColsByFreq; //Number of columns in each .gap files

	////////////////////////////////////////////
	// Index and meta-data int [Col 0]
	////////////////////////////////////////////
	GABE_Data_Integer indexCol(8);
	indexCol.Set(0,1);								//Number of the column of the decimal parameter
	indexCol.Set(1,2);								//Number of the source column
	indexCol.Set(2,3);								//Number of the column of the frequencies values
	indexCol.Set(3,nbFixedCols);					//Number of the column of noise
	indexCol.Set(4,nbFixedCols+nbRecpHeaderCols);	//Number of the column of first frequency
	indexCol.Set(5,nbfreqUsed);						//Number of frequency bands [1-27]
	indexCol.Set(6,nbColsByFreq);					//Number of the column by frequency
	indexCol.Set(7,params.nbTimeStep);				//Number of time steps

	////////////////////////////////////////////
	// Decimal meta-data [Col 1]
	////////////////////////////////////////////
	GABE_Data_Float decimalParam(2);
	decimalParam.Set(0,params.timeStep);					//Time step in s
	decimalParam.Set(1,params.nbTimeStep*params.timeStep);	//Time length

	////////////////////////////////////////////
	// source [Col 2]
	////////////////////////////////////////////v
	GABE_Data_Float sourceCol(nbfreqUsed);
	////////////////////////////////////////////
	// frequencies [Col 3]
	////////////////////////////////////////////
	GABE_Data_Integer freqCol(nbfreqUsed);
	uentier idfreqligne=0;
	for(std::size_t idfreq=0;idfreq<params.configManager->freqList.size();idfreq++)
	{
		if(params.configManager->freqList[idfreq]->doCalculation)
		{
			//Pour cette bande de fr�quence cumul de l'�n�rgie de toute les sources actives
			decimal cumulFreqSources=0;
			for(uentier idsource=0;idsource<params.configManager->srcList.size();idsource++)
			{
				cumulFreqSources+=params.configManager->srcList[idsource]->bandeFreqSource[idfreq].w_j;
			}
			sourceCol.Set(idfreqligne,cumulFreqSources*(*params.configManager->FastGetConfigValue(Core_Configuration::FPROP_RHO))*(*params.configManager->FastGetConfigValue(Core_Configuration::FPROP_CELERITE)));
			freqCol.Set(idfreqligne,(Intb)params.configManager->freqList[idfreq]->freqValue);
			idfreqligne++;
		}
	}
	for(uentier idrecp=0;idrecp<params.configManager->recepteur_p_List.size();idrecp++)
	{
		t_Recepteur_P* currentRP=params.configManager->recepteur_p_List[idrecp];
		GABE recepteurPonctData(nbCols);
		//Copie colonnes 0,1,2,3
		recepteurPonctData.SetCol(0,indexCol);
		recepteurPonctData.SetCol(1,decimalParam);
		recepteurPonctData.SetCol(2,sourceCol);
		recepteurPonctData.SetCol(3,freqCol);

		////////////////////////////////////////////
		// S�rie bruit  [Col nbFixedCols]
		////////////////////////////////////////////
		GABE_Data_Float* bruitCol=new GABE_Data_Float(nbfreqUsed);
		idfreqligne=0;
		for(std::size_t idfreq=0;idfreq<params.configManager->freqList.size();idfreq++)
		{
			if(params.configManager->freqList[idfreq]->doCalculation)
			{
				bruitCol->Set(idfreqligne,currentRP->bruit_spectre[idfreq].db);
				idfreqligne++;
			}
		}
		recepteurPonctData.SetCol(nbFixedCols,bruitCol);
		uentier idfreqcol=0;
		for(std::size_t idfreq=0;idfreq<cols.size();idfreq++)
		{
			if(currentRP->energy_sum[idfreq])
			{
				////////////////////////////////////////////
				// Column received energy   [Col nbFixedCols+nbRecpHeaderCols+(idfreqcol*nbColsByFreq)]
				////////////////////////////////////////////
				GABE_Data_Float* e_col=new GABE_Data_Float(params.nbTimeStep);
				for(uentier idstep=0;idstep<params.nbTimeStep;idstep++)
				{
					e_col->Set(idstep,currentRP->energy_sum[idfreq][idstep]*currentRP->cdt_vol);
				}
				recepteurPonctData.SetCol(nbFixedCols+nbRecpHeaderCols+(idfreqcol*nbColsByFreq),e_col);
				////////////////////////////////////////////
				// Column received energy LF  [Col nbFixedCols+nbRecpHeaderCols+(idfreqcol*nbColsByFreq)+1]
				////////////////////////////////////////////
				// Copy column
				recepteurPonctData.SetCol(nbFixedCols+nbRecpHeaderCols+(idfreqcol*nbColsByFreq)+1,*cols[idfreq].GabeSumEnergyCosPhi[idrecp]);
				////////////////////////////////////////////
				// Column received energy LFC  [Col nbFixedCols+nbRecpHeaderCols+(idfreqcol*nbColsByFreq)+2]
				////////////////////////////////////////////
				// Copy column
				recepteurPonctData.SetCol(nbFixedCols+nbRecpHeaderCols+(idfreqcol*nbColsByFreq)+2,*cols[idfreq].GabeSumEnergyCosSqrtPhi[idrecp]);
				idfreqcol++;
			}
		}
		recepteurPonctData.Save((currentRP->pathRp+filename).c_str());
	}
}
