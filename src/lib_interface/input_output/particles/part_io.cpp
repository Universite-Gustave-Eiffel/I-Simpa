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

#include "part_io.hpp"
#include "part_binary.h"
#include <string.h>
#ifdef WIN32
#include "input_output/pugixml/src/pugixml.hpp"
#endif // WIN32

namespace particleio
{
	using namespace std;

	ParticuleIO::ParticuleIO()
		:particleFile(NULL),readonly(true)
	{
		currentPartInfo.opened=false;
		fileInfo.maxstepcount=0;
		memset(&fileInfo,0,sizeof(fileInfo_t));
	}
	ParticuleIO::~ParticuleIO()
	{
		CloseAndClear();
	}
	void ParticuleIO::WriteHeader(bool returntopos)
	{
		if(IsOpenAndReadyForWrite())
		{
			CloseLastParticleHeader();

            unsigned long lastpos=particleFile->tellp();
            particleFile->seekp(lastParticuleFileHeaderInfo);

            binaryFHeader enteteSortie;
            enteteSortie.particleInfoLength=sizeof(binaryPTimeStep);
            enteteSortie.particleHeaderInfoLength=sizeof(binaryPHeader);
            enteteSortie.fileInfoLength=sizeof(binaryFHeader);
            enteteSortie.formatVersion=PARTICLE_BINARY_VERSION_INFORMATION;
            enteteSortie.timeStep=fileInfo.timeStep;
            enteteSortie.nbParticles=fileInfo.nbparticles;
            enteteSortie.nbTimeStepMax=fileInfo.maxstepcount;
            particleFile->write((char*)&enteteSortie,sizeof(binaryFHeader));
            if(returntopos)
                particleFile->seekp(lastpos);
		}
	}
	void  ParticuleIO::CloseAndClear()
	{
		if(particleFile)
		{
			if(particleFile->is_open())
			{
				if(IsOpenAndReadyForWrite())
					WriteHeader(false);
				particleFile->close();
			}
			fstream* tmp=particleFile;
			particleFile=NULL;
			delete tmp;
			readonly=true;
		}
	}

	void ParticuleIO::Close()
	{
		CloseAndClear();
	}
	bool  ParticuleIO::OpenForWrite(const std::string& fileName)
	{
		readonly=false;
		CloseAndClear();
		#ifdef WIN32
				particleFile = new fstream(pugi::as_wide(fileName), ios::out | ios::binary);
		#else
		particleFile = new fstream(fileName.c_str(), ios::out | ios::binary);
		#endif // WIN

		lastParticuleFileHeaderInfo=particleFile->tellp();
		currentPartInfo.opened=false;
		WriteHeader(false);
		return particleFile;
	}
	bool ParticuleIO::IsOpenAndReadyForWrite()
	{
		return (particleFile && particleFile->is_open( ) && !readonly);
	}
	bool ParticuleIO::IsOpenAndReadyForRead()
	{
		return (particleFile && particleFile->is_open( ) && readonly);
	}

	void ParticuleIO::SetHeaderData(const float& timeStep)
	{
		fileInfo.timeStep=timeStep;
	}
	void ParticuleIO::CloseLastParticleHeader()
	{
		if(currentPartInfo.opened)
		{
			unsigned long lastpos=particleFile->tellp();
			particleFile->seekp(lastParticuleHeaderInfo);
			binaryPHeader pHeaderInfo(currentPartInfo.nbtimestep,currentPartInfo.firsttimestep);
			if(fileInfo.maxstepcount<currentPartInfo.nbtimestep)
				fileInfo.maxstepcount=currentPartInfo.nbtimestep;
			particleFile->write((char*)&pHeaderInfo,sizeof(binaryPHeader));
			particleFile->seekp(lastpos);
			currentPartInfo.opened=false;
		}
	}
	void ParticuleIO::NewParticle(const unsigned long& firsttimestep)
	{
		if(IsOpenAndReadyForWrite())
		{
			CloseLastParticleHeader();
			fileInfo.nbparticles++;
			lastParticuleHeaderInfo=particleFile->tellp();
			currentPartInfo.firsttimestep=firsttimestep;
			binaryPHeader pHeaderInfo(1,firsttimestep);
			particleFile->write((char*)&pHeaderInfo,sizeof(binaryPHeader));
			currentPartInfo.opened=true;
			currentPartInfo.currentstep=firsttimestep;
			currentPartInfo.nbtimestep=0;
		}else{
			throw "You cannot write new particle if the file is not ready to write";
		}
	}

	void ParticuleIO::NewPositionParticle(const float& x,const float& y,const float& z,const float& energy)
	{
		if(IsOpenAndReadyForWrite())
		{
			if(currentPartInfo.opened)
			{
				currentPartInfo.currentstep++;
				currentPartInfo.nbtimestep++;
				binaryPTimeStep stepobj(vec3(x,y,z),energy);
				particleFile->write( (char*)&stepobj,sizeof(binaryPTimeStep));
			}else{
				throw 1; //"You must call NewParticle() before adding new particle position";
			}
		}else{
			throw 2; //"You cannot write new particle position if the file is not ready to write";
		}
	}

	void ParticuleIO::GetHeaderData(float& timeStep,unsigned long& nbParticles,unsigned long& nbStepMax)
	{
		timeStep=fileInfo.timeStep;
		nbParticles=fileInfo.nbparticles;
		nbStepMax=fileInfo.maxstepcount;
	}

	bool  ParticuleIO::OpenForRead(const std::string& fileName)
	{
		readonly=true;
		CloseAndClear();
		#ifdef WIN32
				particleFile = new fstream(pugi::as_wide(fileName), ios::in | ios::binary);
		#else
				particleFile = new fstream(fileName.c_str(), ios::in | ios::binary);
		#endif // WIN
		if(IsOpenAndReadyForRead())
		{
			binaryFHeader enteteFichier;
			memset(&enteteFichier,0,sizeof(binaryFHeader));
			lastParticuleFileHeaderInfo=particleFile->tellp();
			particleFile->read((char*)&enteteFichier, sizeof (binaryFHeader));
			fileInfo.nbparticles=enteteFichier.nbParticles;
			fileInfo.timeStep=enteteFichier.timeStep;
			fileInfo.fileInfoLength=enteteFichier.fileInfoLength;
			fileInfo.isoldversionfile=(PARTICLE_BINARY_VERSION_INFORMATION!=enteteFichier.formatVersion);
			fileInfo.particleHeaderInfoLength=enteteFichier.particleHeaderInfoLength;
			fileInfo.particleInfoLength=enteteFichier.particleInfoLength;
			fileInfo.maxstepcount=enteteFichier.nbTimeStepMax;
			currentPartInfo.currentstep=0;
			currentPartInfo.opened=false;
			currentPartInfo.nbtimestep=0;
			currentPartInfo.firsttimestep=0;
			currentPartInfo.idpart=0;
		}else{
			CloseAndClear();
		}
		return particleFile;
	}

	void ParticuleIO::ResetRead() {
		if (IsOpenAndReadyForRead())
		{
			particleFile->seekp(lastParticuleFileHeaderInfo + sizeof(binaryFHeader));
			currentPartInfo.currentstep = 0;
			currentPartInfo.opened = false;
			currentPartInfo.nbtimestep = 0;
			currentPartInfo.firsttimestep = 0;
			currentPartInfo.idpart = 0;
		}	
	}
	void ParticuleIO::NextParticle(unsigned long& firstTimeStep,unsigned long& nbTimeStep)
	{
		nbTimeStep=0;
		if(IsOpenAndReadyForRead())
		{
			if(currentPartInfo.idpart<fileInfo.nbparticles)
			{
				if(currentPartInfo.opened && currentPartInfo.currentstep<currentPartInfo.nbtimestep-1)
				{
					//On se d�place juste avant l'entete de la prochaine particule
					particleFile->seekp(lastParticuleHeaderInfo+fileInfo.particleHeaderInfoLength+fileInfo.particleInfoLength*currentPartInfo.nbtimestep);
				}
				lastParticuleHeaderInfo=particleFile->tellp();
				currentPartInfo.idpart++;
				currentPartInfo.opened=true;
				currentPartInfo.currentstep=0;
				binaryPHeader headerPart;
				memset(&headerPart,0,sizeof(binaryPHeader));
				particleFile->read((char*)&headerPart, sizeof (binaryPHeader));
				nbTimeStep=headerPart.nbTimeStep;
				firstTimeStep=headerPart.firstTimeStep;
				currentPartInfo.nbtimestep=nbTimeStep;
				currentPartInfo.firsttimestep=firstTimeStep;
				if(fileInfo.isoldversionfile)
				{
					particleFile->seekp(lastParticuleHeaderInfo+fileInfo.particleHeaderInfoLength);
				}
			}else{
				throw "There is no more particles in this file";
			}
		}else{
			throw "You must call OpenForRead() before calling NextParticle()";
		}
	}
	void ParticuleIO::NextTimeStep(float& x,float& y,float& z,float& energy)
	{
		if(IsOpenAndReadyForRead() && currentPartInfo.opened)
		{
			if(currentPartInfo.currentstep<currentPartInfo.nbtimestep)
			{
				currentPartInfo.currentstep++;
				binaryPTimeStep partTimestep;
				unsigned long filepos(0);
				if(fileInfo.isoldversionfile)
					filepos=particleFile->tellp();
				particleFile->read((char*)&partTimestep, sizeof (binaryPTimeStep));
				if(fileInfo.isoldversionfile)
				{
					particleFile->seekp(filepos+fileInfo.particleInfoLength);
				}
				x=partTimestep.position.x;
				y=partTimestep.position.y;
				z=partTimestep.position.z;
				energy=partTimestep.energy;
			}else{
				throw "There is no more time step for this particle";
			}
		}else{
			throw "You must call OpenForRead() and NextParticle() before calling NextTimeStep()";
		}


	}
};
