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

#include "processManager.h"
#include <wx/txtstrm.h>
#include <wx/log.h>
#include "manager/stringTools.h"
#include <wx/progdlg.h>
#include <wx/frame.h>
#include "last_cpp_include.hpp"


wxBEGIN_EVENT_TABLE(processManager, wxProcess)
	EVT_TIMER(wxID_ANY, processManager::OnTimer)
wxEND_EVENT_TABLE()


void processManager::HandleOutput() {
	wxString warningMessage;
	wxString message;
	wxString errorMessage;
	while (IsInputAvailable() || IsErrorAvailable())
	{
		if (IsInputAvailable()) {
			wxTextInputStream tis(*GetInputStream());

			// this assumes that the output is always line buffered
			wxString msg;
			wxString output = tis.ReadLine();
			if (!this->outlogs.empty())
			{
				for (std::vector<smart_ptr<InterfLogger> >::iterator itlogs = this->outlogs.begin(); itlogs != this->outlogs.end(); itlogs++)
				{
					itlogs->get()->LogMessage(output);
				}
			}
			if (output.Left(1) != "#")
			{
				msg << labelOutput << output;
				msg.Replace("%", "%%"); //wxLog expect this character used for parameters, then add another % to escape it
				if (output.Left(1) == "!")
				{
					warningMessage += msg + "\n";
				}
				else {
					message += msg + "\n";
				}
			}
			else {
				wxString prog = output.Right(output.Len() - 1).Strip();
				outputProgression = Convertor::ToFloat(prog);
			}
		}
		if (IsErrorAvailable()) {
			wxTextInputStream tis(*GetErrorStream());
			const wxString& errMsg(tis.ReadLine());
			// The IsErrorAvailable function seems to always return true.  I think there must be an empty line in the error stream. -- Larry Pyeatt
			if(errMsg.Len() > 0)
			  {
			if (!this->outlogs.empty())
			{
				for (std::vector<smart_ptr<InterfLogger> >::iterator itlogs = this->outlogs.begin(); itlogs != this->outlogs.end(); itlogs++)
				{
					(*(*itlogs)).LogError(errMsg);
				}
			}
			// this assumes that the output is always line buffered
			wxString msg;
			msg << wxGetTranslation("Calculation code error:") << errMsg;
			msg.Replace("%", "%%"); //wxLog expect this character used for parameters, then add another % to escape it
			errorMessage += msg + "\n";
		}
	}
	}
	if (!message.IsEmpty()) {
		wxLogMessage(message);
	}
	if (!warningMessage.IsEmpty()) {
		wxLogWarning(warningMessage);
	}
	if (!errorMessage.IsEmpty()) {
		wxLogError(errorMessage);
	}
}

void processManager::OnTimer(wxTimerEvent& WXUNUSED(event)) {
	HandleOutput();
	if (parent)
		parent->Update();
	if (outputProgression>99.f)
		outputProgression = 99;
	wxProgressDialog * progDialog = wxDynamicCast(progressDialog, wxProgressDialog);
	if (progDialog && !progDialog->Update(outputProgression * 100))
	{
		int pid = GetPid();
		if(pid > 0) {
			wxKillError killerror = wxProcess::Kill(pid, wxSIGKILL);
			wxLogMessage(wxGetTranslation("External code execution canceled"));
			wxLogMessage(wxGetTranslation("Process answer:"));
			switch (killerror)
			{
			case wxKILL_OK:              // no error
				wxLogMessage(wxGetTranslation("No error"));
				break;
			case wxKILL_BAD_SIGNAL:      // no such signal
				wxLogError(wxGetTranslation("Signal doesn't exist"));
				break;
			case wxKILL_ACCESS_DENIED:   // permission denied
				wxLogError(wxGetTranslation("Unauthorized closing process"));
				break;
			case wxKILL_NO_PROCESS:      // no such process
				wxLogError(wxGetTranslation("Process doesn't exist"));
				break;
			case wxKILL_ERROR:           // another, unspecified error
				wxLogError(wxGetTranslation("Non-specified process output"));
				break;
			default:
				wxLogError(wxGetTranslation("Unknown output process"));
				break;
			}
		}
	}
	m_timer.StartOnce(150); // Message refresh time
}

void processManager::AddLogger(smart_ptr<InterfLogger> logger)
{
	this->outlogs.push_back(logger);
}

bool uiRunExe(wxFrame* parent,const wxString& path,const wxString& labelOutput, wxWindow* progressDialog,smart_ptr<InterfLogger> extLogger)
{
    wxGetTranslation("Elapsed time: ");
	wxGetTranslation("Remaining time: ");
	wxGetTranslation("Close");
	wxGetTranslation("Cancel");
	wxGetTranslation("Unknown");
	wxProgressDialog * progDialog=wxDynamicCast(progressDialog,wxProgressDialog);

	processManager process(parent, path, labelOutput, progDialog);
	if (extLogger.get() != NULL)
		process.AddLogger(extLogger);

	wxLogMessage(wxGetTranslation("External code execution"));
	wxLogMessage(path);

	int processId = wxExecute(path, wxEXEC_SYNC, &process);
	if (processId < 0)
	{
		wxLogMessage("Program execution failed");
		return false;
	}
	else {
		return true;
	}
}
