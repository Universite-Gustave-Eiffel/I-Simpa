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

#include "processManager.h"
#include <wx/txtstrm.h>
#include <wx/log.h>
#include "manager/stringTools.h"
#include <wx/progdlg.h>
#include <wx/frame.h>
#include "last_cpp_include.hpp"

void processManager::OnTerminate(int pid, int status)
{
	run=false;
}
void processManager::AddLogger(smart_ptr<InterfLogger> logger)
{
	this->outlogs.push_back(logger);
}
void processManager::LogOutput(bool &hasOutput, const wxString &label,float *outputProgression)
{
   hasOutput = false;

    if ( IsInputAvailable() )
    {
        wxTextInputStream tis(*GetInputStream());

        // this assumes that the output is always line buffered
        wxString msg;
		wxString output= tis.ReadLine();
		if(!this->outlogs.empty())
		{
			for(std::vector<smart_ptr<InterfLogger> >::iterator itlogs=this->outlogs.begin();itlogs!=this->outlogs.end();itlogs++)
			{
				(*(*itlogs)).LogMessage(output);
			}
		}
		if(outputProgression==NULL || output.Left(1)!="#")
		{
			msg << label << output;
			msg.Replace("%","%%"); //si il y a un seul % alors un bug apparait wxString attend un format du type %s ou %i par exemple
			if(output.Left(1)=="!")
			{
				wxLogWarning(msg);
			}else{
				wxLogMessage(msg);
			}
		}else{
			wxString prog=output.Right(output.Len()-1).Strip();
			*outputProgression=Convertor::ToFloat(prog);
		}

        hasOutput = true;
    }

    while ( IsErrorAvailable() )
    {
        wxTextInputStream tis(*GetErrorStream());
		const wxString& errMsg(tis.ReadLine());
		if(!this->outlogs.empty())
		{
			for(std::vector<smart_ptr<InterfLogger> >::iterator itlogs=this->outlogs.begin();itlogs!=this->outlogs.end();itlogs++)
			{
				(*(*itlogs)).LogError(errMsg);
			}
		}
        // this assumes that the output is always line buffered
        wxString msg;
		msg << _("Calculation code error:") << errMsg;
		msg.Replace("%","%%"); //si il y a un seul % alors un bug apparait wxString attend un format du type %s ou %i par exemple
		wxLogError(msg);

        hasOutput = true;
    }
}
bool processManager::IsRunning()
{
	return run;
}


bool uiRunExe(wxFrame* parent,const wxString& path,const wxString& labelOutput, wxWindow* progressDialog,smart_ptr<InterfLogger> extLogger)
{
    _("Elapsed time : ");
	_("Remaining time : ");
	_("Close");
	_("Cancel");
	_("Unknown");
	wxProgressDialog * progDialog=wxDynamicCast(progressDialog,wxProgressDialog);
	bool hasOutput=true;
	processManager* process = new processManager(parent,path);
	if(extLogger.get()!=NULL)
		process->AddLogger(extLogger);

	wxLogInfo(_("External code execution"));
	wxLogInfo(path);

	int processId=wxExecute(path,wxEXEC_ASYNC,process);
	if(!processId)
	{
		wxLogInfo("Program execution failed");
		delete process;
		return false;
	}
	float percFinish=0;
	wxDateTime lastProgShow=wxDateTime::UNow();
	while(process->IsRunning())
	{
		hasOutput=true;
		wxMilliSleep(50);
		while(hasOutput) //&& (!progDialog || progDialog->Update(percFinish*100))
		{
			process->LogOutput(hasOutput,labelOutput,&percFinish);
		}
		if(wxDateTime::UNow().GetValue()-lastProgShow.GetValue()>500)
		{
			lastProgShow=wxDateTime::UNow();
			if(parent)
				parent->Update();
			if(percFinish>99.f)
				percFinish=99;
			if(progDialog && !progDialog->Update(percFinish*100))
			{
				wxKillError killerror=wxProcess::Kill(processId,wxSIGKILL);
				wxLogInfo(_("External code execution canceled"));
				wxLogInfo(_("Process answer:"));
				switch(killerror)
				{
				case wxKILL_OK:              // no error
					wxLogInfo(_("No error"));
					break;
				case wxKILL_BAD_SIGNAL:      // no such signal
					wxLogError(_("Signal doesn't exist"));
					break;
				case wxKILL_ACCESS_DENIED:   // permission denied
					wxLogError(_("Unauthorized closing process"));
					break;
				case wxKILL_NO_PROCESS:      // no such process
					wxLogError(_("Process doesn't exist"));
					break;
				case wxKILL_ERROR :           // another, unspecified error
					wxLogError(_("Non-specified process output"));
					break;
				default:
					wxLogError(_("Unknown output process"));
					break;
				}
				//Si on supprime le processus maintenant on aura une erreur
				//Si on ne le supprime pas il y a une fuite mémoire
				//delete process;
				return false;
			}
		}
	}
	// On récupère les derniers messages
	wxMilliSleep(150);
	process->LogOutput(hasOutput,labelOutput,&percFinish);
	while(hasOutput)
		process->LogOutput(hasOutput,labelOutput,&percFinish);
	delete process;
	return true;
}
