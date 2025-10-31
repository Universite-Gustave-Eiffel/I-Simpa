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

#include "GL/opengl_inc.h"
#include "Recepteurs_ponctuel_intensity.hpp"
#include "data_manager/appconfig.h"
#include <input_output/gabe/stdgabe.h>
#include <wx/filename.h>
#include <wx/math.h>
#include "last_cpp_include.hpp"

RecepteursPControler::RecepteursPControler()
:Animator()
{
	nbtimestep=0;
	legendRenderer=NULL;
	this->SetRendererName("intensite");

}
RecepteursPControler::~RecepteursPControler()
{


}
wxString RecepteursPControler::GetRendererLabel()
{
	return wxGetTranslation("Intensity");
}

void RecepteursPControler::RedrawLegend()
{
	if(this->recepteurs.size()==0 || !this->IsRenderingEnable())
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
		fileLegend->InitText(wxGetTranslation("Intensity : ")+pathLoadedFile,35);


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
void RecepteursPControler::Init(const bool& resetLoadingTime)
{
	nbtimestep=0;
	recepteurs.clear();
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
void RecepteursPControler::SetTimeStep(const int& timeStp)
{
	if(this->p_legends.currentTimeStep)
	{
		if(this->nbtimestep>0)
		{
			this->p_legends.currentTimeStep->InitText(wxString::Format("%.3f s",(timestep*(timeStp+1))));
		}
	}
}


bool RecepteursPControler::LoadRecepteursPFile(const wxString& path, const vec4& UnitizeVal)
{
	Init(false);
	const float p_0=1/pow((float)(20*pow(10.f,(int)-6)),(int)2);
	recepteurs.clear();
	using namespace gabe_io;
	Gabe_rw reader;
	if(reader.Load(std::string(path.utf8_str())))
	{
		wxFileName filePath=path;
		filePath.MakeRelativeTo(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_REPORT_FOLDER_PATH);
		filePath.ClearExt();
		filePath.SetName("");
		pathLoadedFile=filePath.GetFullPath();
		intarray header_parameter;
		floatarray header_parameter_f;
		reader.ReadColInt(0,header_parameter);
		reader.ReadColFloat(1,header_parameter_f);
		recepteurs.insert(recepteurs.begin(),header_parameter[0],RecepteurPData_t());
		nbtimestep=header_parameter[1];
		int nbcolbyrp=header_parameter[2];
		timestep=header_parameter_f[0];
		int idrecp=0;
		for(int idcol=header_parameter[3];idcol<reader.size();idcol+=nbcolbyrp)
		{
			floatarray col_x,col_y,col_z;
			reader.ReadColFloat(idcol,col_x);
			reader.ReadColFloat(idcol+1,col_y);
			reader.ReadColFloat(idcol+2,col_z);
			if(col_x.size()-1>nbtimestep && col_y.size()-1>nbtimestep && col_z.size()-1>nbtimestep)
			{
				RecepteurPData_t* curData=&recepteurs[idrecp];
				curData->position=coordsOperation::CommonCoordsToGlCoords(UnitizeVal,vec3(col_x[0],col_y[0],col_z[0]));
				curData->vecDir.reserve(nbtimestep);
				bool minmax_assigned=false;
				for(unsigned int idstep=0;idstep<nbtimestep;idstep++)
				{
					vec3 vecdir(col_x[idstep+1],col_y[idstep+1],col_z[idstep+1]);
					float normVec=vecdir.length();
					float energy=10*log10(normVec*p_0);
					if(wxFinite(energy))
					{
						if(!minmax_assigned)
						{
							curData->min=energy;
							curData->max=energy;
							minmax_assigned=true;
						}else{
							if(curData->min>energy)
								curData->min=energy;
							if(curData->max<energy)
								curData->max=energy;
						}
					}
				}
				for(unsigned int idstep=0;idstep<nbtimestep;idstep++)
				{
					vec3 vecdir(col_x[idstep+1],col_y[idstep+1],col_z[idstep+1]);
					float normVec=vecdir.length();
					if(normVec>0)
					{
						float energy=10*log10(normVec*p_0);
						vecdir.set(vecdir.x,vecdir.z,-vecdir.y);
						vecdir*=(1/normVec)*.15;

						vecdir*=(energy-curData->min)/(curData->max-curData->min);
						if(wxFinite(energy))
						{
							curData->vecDir.push_back(vecdir);
						}else{
							curData->vecDir.push_back(vec3());
						}
					}else{
						curData->vecDir.push_back(vec3());
					}
				}
			}
			idrecp++;
		}
		//Chargement et mis à jour des légendes
		RedrawLegend();
		loadingTime=wxDateTime::UNow();
	}

	return true;
}

void RecepteursPControler::Render(const int& timeStp)
{

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glLineWidth(3);
	glColor3f(.3f,.3f,1.f);
	glBegin(GL_LINES);
	for(std::size_t idrecp=0;idrecp<recepteurs.size();idrecp++)
	{
		RecepteurPData_t* currp=&recepteurs[idrecp];
		if(currp->vecDir.size()>timeStp)
		{
			glVertex3fv(currp->position);
			glVertex3fv(currp->position+currp->vecDir[timeStp]);
		}
	}
	glEnd();
}

int RecepteursPControler::GetNbTimeStep()
{
	return nbtimestep;
}

void RecepteursPControler::InitLegend(legendRendering::ForeGroundGlBitmap& _legendRenderer)
{
	legendRenderer=&_legendRenderer;
}
