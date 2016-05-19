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
#include "Particules.h"
#include <wx/log.h>
#include <wx/progdlg.h>
#include "manager/stringTools.h"
#include "data_manager/appconfig.h"
#include <fstream>
#include <wx/filename.h>
#include <wx/utils.h>
#include <input_output/part_binary.h>
#include "last_cpp_include.hpp"
#define CHAR_TAB 9
#define CHAR_RETURN_WIN 13
#define CHAR_RETURN_UNIX 10
#define CHAR_FINCHAINE 0
#define TIMESTEP_LIMITATION 1000000

ParticulesControler::ParticulesControler()
: Animator(),tabInfoParticles(NULL)
{
	this->SetRendererName("particules");
	nbStep=0;
	nbParticles=0;
	legendRenderer=NULL;
}


wxString ParticulesControler::GetRendererLabel()
{
	return _("Particles");
}
ParticulesControler::~ParticulesControler()
{
	delete[] tabInfoParticles;
}

int ParticulesControler::GetNbTimeStep()
{
	return nbStep;
}

void ParticulesControler::SetTimeStep(const int& _timeStp)
{
	if(this->p_legends.currentTimeStep)
	{
		if(this->nbStep>0)
		{
			this->p_legends.currentTimeStep->InitText(Convertor::ToString(timeStep*(_timeStp+1))+" s");
		}
	}
}
int ParticulesControler::GetSizeTabParticle()
{
	return this->nbParticles;
}
void ParticulesControler::Init(const bool& resetLoadingTime)
{
	delete[] tabInfoParticles;
	this->tabInfoParticles = NULL;
	this->nbParticles=0;
	nbStep=0;
	this->timeStep=0.f;
	if(legendRenderer)
	{
		if(this->p_legends.currentTimeStep)
		{
			legendRenderer->Delete(this->p_legends.currentTimeStep);
			this->p_legends.currentTimeStep=NULL;
		}
		if(this->p_legends.currentFile)
		{
			legendRenderer->Delete(this->p_legends.currentFile);
			this->p_legends.currentFile=NULL;
		}
	}
	Animator::Init(resetLoadingTime);
}

void ParticulesControler::InitLegend(legendRendering::ForeGroundGlBitmap& _legendRenderer)
{
	legendRenderer=&_legendRenderer;
}


void ParticulesControler::SetTextForegroundColor(const ivec4& foregroundColor)
{
	this->text_foreground_color.Set(foregroundColor.red,foregroundColor.green,foregroundColor.blue,foregroundColor.alpha);
}
void ParticulesControler::SetTextBackgroundColor(const ivec4& backgroundColor)
{
	this->text_background_color.Set(backgroundColor.red,backgroundColor.green,backgroundColor.blue,backgroundColor.alpha);
}
void ParticulesControler::SetParticleColor(const ivec3& _particleColor)
{
	particleColor.set(_particleColor.a/255.f,_particleColor.b/255.f,_particleColor.c/255.f,1.f);
}
void ParticulesControler::RefreshAnimationRendering()
{
	loadingTime=wxDateTime::Now();
}
void ParticulesControler::SetTextFont(const wxFont& textFont)
{
	this->legendFont=textFont;
}


void ParticulesControler::SaveToPBin(wxString fileName)
{
	using namespace std;
	fstream particleFile (WXSTRINGTOCHARPTR(fileName), ios::out | ios::binary);

	//*************************
	//Ecriture de l'entete du fichier
	/*
	t_Header fileHeader={0};
	fileHeader.nbParticules=this->nbParticles;
	fileHeader.nbPasDeTemps=this->nbStep;
	binFile.write((char*)&fileHeader,sizeof(t_Header));
	*/
	binaryFHeader enteteSortie;
	enteteSortie.nbParticles=this->nbParticles;
	enteteSortie.nbTimeStepMax=this->nbStep;
	enteteSortie.particleInfoLength=sizeof(binaryPTimeStep);
	enteteSortie.particleHeaderInfoLength=sizeof(binaryPHeader);
	enteteSortie.formatVersion=PARTICLE_BINARY_VERSION_INFORMATION;
	enteteSortie.fileInfoLength=sizeof(binaryFHeader);
	enteteSortie.timeStep=this->timeStep;
	particleFile.write((char*)&enteteSortie,sizeof(binaryFHeader));


	//*************************
	//Ecriture de la position des particules
	for(unsigned long pIndex=0;pIndex<this->nbParticles;pIndex++)
	{
		if(!tabInfoParticles[pIndex].tabSteps)
			return;
		unsigned long nbStepPart=tabInfoParticles[pIndex].nbStep;
		particleFile.write((char*)&binaryPHeader(nbStepPart,tabInfoParticles[pIndex].firstStep),sizeof(binaryPHeader));
		for(unsigned long pTimeIndex=0;pTimeIndex<nbStepPart;pTimeIndex++)
		{
		    vec3 poststep(tabInfoParticles[pIndex].tabSteps[pTimeIndex].pos);
		    binaryPTimeStep stepdata(poststep,tabInfoParticles[pIndex].tabSteps[pTimeIndex].energie);
			particleFile.write( (char*)&stepdata,sizeof(binaryPTimeStep));
		}
	}

	particleFile.close();

}


void ParticulesControler::LoadPBin(wxString fileName, bool doCoordsTransformation, vec4 UnitizeVal)
{

	Init(false);
	using namespace std;
	fstream binFile (WXSTRINGTOCHARPTR(fileName), ios::in | ios::binary);
    if(!binFile.is_open())
		return;

	wxFileName filePath=fileName;
	filePath.MakeRelativeTo(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_REPORT_FOLDER_PATH);
	if(filePath.GetDirCount()>2)
	{
		filePath.RemoveLastDir();
		filePath.RemoveLastDir();
	}
	filePath.ClearExt();
	filePath.SetName("");
	ShortcutPath=filePath.GetFullPath();
	//Création popup de progression
	wxProgressDialog progDialog(_("Loading particles file"),_("Loading particles file"),100,NULL,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE );
	progDialog.Update(0);


	binaryFHeader enteteFichier;
	memset(&enteteFichier,0,sizeof(binaryFHeader));
	unsigned long curPosHFile=binFile.tellp();
	binFile.read((char*)&enteteFichier, sizeof (binaryFHeader));
	this->nbParticles=enteteFichier.nbParticles;
	this->nbStep=enteteFichier.nbTimeStepMax;
	this->timeStep=enteteFichier.timeStep;

	//Calcul de la mémoire nécessaire
	wxMemorySize freemem=wxGetFreeMemory();
	//2.71 facteur d'occupation mémoire dû à la précompilation opengl en mode particule. Facteur mis à 4 afin d'avoir une marge de mémoire
	wxMemorySize particleCount(enteteFichier.nbParticles),
		stepcount(enteteFichier.nbTimeStepMax),
		headerpart(sizeof(binaryPHeader)),
		stepheader(sizeof(binaryPTimeStep));
	wxMemorySize memrequire=(wxMemorySize)4*(headerpart*particleCount+stepheader*particleCount*stepcount);
	if(freemem<memrequire && freemem!=-1)
	{
		wxLogError(_("Insufficent memory available, %u memory required and %u memory available"),(unsigned int)(memrequire/1.e6).ToLong(),(unsigned int)(freemem/1.e6).ToLong());
		binFile.close();
		Init();
		return;
	}
	bool FileVersionDifferent=false;
	if(enteteFichier.formatVersion!=PARTICLE_BINARY_VERSION_INFORMATION)
		FileVersionDifferent=true;

	if(FileVersionDifferent)
		binFile.seekp(curPosHFile+enteteFichier.fileInfoLength);

	wxLogDebug(_("Particles file contains %i particles and %i time step"),this->nbParticles,this->nbStep);
	if(this->nbParticles>0)
	{
		delete[] tabInfoParticles;
		tabInfoParticles = new t_ParticuleInfo[this->nbParticles];
		int percProgression=0;
		int ansPercProgression=0;
		for(unsigned long pIndex=0;pIndex<this->nbParticles;pIndex++)
		{
			percProgression=((float)pIndex/this->nbParticles)*100;
			if(ansPercProgression!=percProgression && percProgression<100)
			{
				ansPercProgression=percProgression;
				if(!progDialog.Update(percProgression))
				{
					//Annulation du chargement du fichier par l'utilisateur
					Init();
					wxLogInfo(_("Cancel loading particles files"));
					binFile.close();
					return;
				}
			}
			unsigned long curPosH=binFile.tellp();
			binaryPHeader headerPart;
			memset(&headerPart,0,sizeof(binaryPHeader));
			binFile.read((char*)&headerPart, sizeof (binaryPHeader));
			tabInfoParticles[pIndex].nbStep=headerPart.nbTimeStep;
			tabInfoParticles[pIndex].firstStep=headerPart.firstTimeStep;
			if(headerPart.nbTimeStep>0 && headerPart.nbTimeStep<TIMESTEP_LIMITATION)
			{
				tabInfoParticles[pIndex].tabSteps = new t_Particule[headerPart.nbTimeStep];

				if(FileVersionDifferent)
					if((unsigned long)binFile.tellp()!=curPosH+enteteFichier.particleHeaderInfoLength)
						binFile.seekp(curPosH+enteteFichier.particleHeaderInfoLength);
				for(unsigned long pTimeIndex=0;pTimeIndex<headerPart.nbTimeStep;pTimeIndex++)
				{
					unsigned long curPosP=binFile.tellp();
					binaryPTimeStep partTimestep;
					binFile.read((char*)&partTimestep, sizeof (binaryPTimeStep));
					if(doCoordsTransformation)
						partTimestep.position=coordsOperation::CommonCoordsToGlCoords(UnitizeVal,partTimestep.position);

					tabInfoParticles[pIndex].tabSteps[pTimeIndex].pos[0]=partTimestep.position[0];
					tabInfoParticles[pIndex].tabSteps[pTimeIndex].pos[1]=partTimestep.position[1];
					tabInfoParticles[pIndex].tabSteps[pTimeIndex].pos[2]=partTimestep.position[2];
					tabInfoParticles[pIndex].tabSteps[pTimeIndex].energie=partTimestep.energy;
					if(FileVersionDifferent)
						if((unsigned long)binFile.tellp()!=curPosP+enteteFichier.particleInfoLength)
							binFile.seekp(curPosP+enteteFichier.particleInfoLength);
				}
			}
		}
	}else{
		this->nbStep=0;
		this->timeStep=0;
	}
	binFile.close();


	//Chargement et mis à jour des légendes
	RedrawLegend();
	loadingTime=wxDateTime::Now();
}

bool  ParticulesControler::LoadParticleFile(const char *mfilename, vec4 UnitizeVal, Element* elConf, Element::IDEVENT loadingMethod)
{
	renderMethod=loadingMethod;
	this->Init(false);
    if (!wxFileExists(mfilename))
    {
		wxLogError(_("Particles file does not exist!\n %s"),mfilename);
        return false;
    }

	try
	{
		LoadPBin(mfilename,true,UnitizeVal);
	}
	catch( ... ) {
		wxLogError(_("Unknown error when reading particles file"));
		return false;
	}
	return true;
}

void ParticulesControler::RedrawLegend()
{
	if(!this->tabInfoParticles || !this->IsRenderingEnable())
		return;
	using namespace legendRendering;
	if(legendRenderer)
	{

		legendText* fileLegend;


		if(!this->p_legends.currentFile)
		{
			fileLegend=new legendText();
		}else{
			fileLegend=this->p_legends.currentFile;
		}
		fileLegend->SetFont( this->legendFont );
		fileLegend->SetTextForeground(this->text_foreground_color);
		fileLegend->SetTextBackground(this->text_background_color);
		fileLegend->InitText(_("Particles:")+ShortcutPath,35);


		if(!this->p_legends.currentFile)
		{
			legendCfg currentObjConf=fileLegend->GetCfg();
			currentObjConf.alignementV=ALIGNEMENT_V_TOP;
			currentObjConf.alignementH=ALIGNEMENT_H_LEFT;
			fileLegend->SetCfg(currentObjConf);
			this->p_legends.currentFile=fileLegend;
			legendRenderer->Push(this->p_legends.currentFile);
		}
	}
	if(legendRenderer)
	{

		legendText* fileLegend;


		if(!this->p_legends.currentTimeStep)
		{
			fileLegend=new legendText();
		}else{
			fileLegend=this->p_legends.currentTimeStep;
		}
		fileLegend->SetFont( this->legendFont );
		fileLegend->SetTextForeground(this->text_foreground_color);
		fileLegend->SetTextBackground(this->text_background_color);
		fileLegend->InitText("0,000 s",8);


		if(!this->p_legends.currentTimeStep)
		{
			legendCfg currentObjConf=fileLegend->GetCfg();
			currentObjConf.alignementV=ALIGNEMENT_V_TOP;
			currentObjConf.alignementH=ALIGNEMENT_H_LEFT;
			fileLegend->SetCfg(currentObjConf);
			this->p_legends.currentTimeStep=fileLegend;
			legendRenderer->Push(this->p_legends.currentTimeStep);
		}
	}
}
void ParticulesControler::Render(const int& timeStp)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);	//glEnable(GL_BLEND);
	glPointSize(2);
	//Antialiasing sur les points
	glDisable (GL_POINT_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	//Gestion de la transparence
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(renderMethod==Element::IDEVENT_LOAD_PARTICLE_SIMULATION)
	{
		glBegin(GL_POINTS);
		glColor4f(particleColor[0], particleColor[1], particleColor[2], 1.);

		#define __PARTICLE_COLOR__ 1
		#if __PARTICLE_COLOR__
		float particle_pivot;
		bool pivot_set = false;
		#endif

		for(unsigned int i=0;i<nbParticles;i++)
		{
			if(tabInfoParticles[i].nbStep>timeStp-tabInfoParticles[i].firstStep && tabInfoParticles[i].tabSteps)
			{
				#if __PARTICLE_COLOR__
				// color mod in function of energy
				float particle_energy = tabInfoParticles[i].tabSteps[timeStp - tabInfoParticles[i].firstStep].energie;
				if (pivot_set)
				{
					if (particle_energy > particle_pivot)
					{
						glColor4f(1.0, 0.5, 0.0, 1.0);
					}
					else {
						glColor4f(0.0, 0.5, 1.0, 1.0);
					}
					
				}
				else
				{
					particle_pivot = particle_energy;
					pivot_set = true;
				}
				#endif
				
				glVertex3fv(tabInfoParticles[i].tabSteps[timeStp-tabInfoParticles[i].firstStep].pos);
			}
		}
		glEnd();
	}else if(renderMethod==Element::IDEVENT_LOAD_PARTICLE_SIMULATION_PATH){
		//On fait le tracé entre deux positions pas de temps -1 vers pas de temps demandé
		glBegin(GL_LINES);
		glColor4f(particleColor[0],particleColor[1],particleColor[2],1.);
		for(unsigned int i=0;i<nbParticles;i++)
		{
			if(tabInfoParticles[i].nbStep>timeStp-tabInfoParticles[i].firstStep && tabInfoParticles[i].tabSteps && int(timeStp)-int(tabInfoParticles[i].firstStep)>0)
			{
				glVertex3fv(tabInfoParticles[i].tabSteps[timeStp-tabInfoParticles[i].firstStep-1].pos);
				glVertex3fv(tabInfoParticles[i].tabSteps[timeStp-tabInfoParticles[i].firstStep].pos);
			}
		}
		glEnd();
	}
}
