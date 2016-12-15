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

#include "first_header_include.hpp"
#include <wx/timer.h>
#include "manager/smart_ptr.h"
#include "interf_logger.hpp"
#include <wx/process.h>
#include <wx/intl.h>
#include <vector>

#ifndef  __PROCESS_MANAGER__
#define __PROCESS_MANAGER__
/** \file process.h
 *  \brief Gestionnaire d'execution d'un programme externe
 */

class processManager : public wxProcess
{
public:
	/**
	 * Constructeur
	 * @param parent Fenêtre parente
	 * @param cmd Chemin de l'exécutable
	 */
	processManager(wxFrame *parent, const wxString& cmd, const wxString& labelOutput, wxWindow* progressDialog)
		: wxProcess(reinterpret_cast<wxEvtHandler*>(parent)), m_cmd(cmd), labelOutput(labelOutput), m_timer(this), progressDialog(progressDialog), parent(parent), outputProgression(0)
	{
        Redirect();
		m_timer.StartOnce(150); // Message refresh time
    }


	~processManager() {
		if (m_timer.IsRunning())
		{
			m_timer.Stop();
		}
		HandleOutput();
	}
	
	/**
	* Process log messages from external exe
	*/
	void OnTimer(wxTimerEvent& WXUNUSED(event));
	/**
	* Add a logger object
	* @param logger Instance of log object
	*/
	void AddLogger(smart_ptr<InterfLogger> logger);

private:
	void HandleOutput();
	wxString m_cmd;
	wxTimer m_timer;
	wxWindow* progressDialog;
	wxFrame *parent;
	float outputProgression;
	const wxString labelOutput;
	std::vector<smart_ptr<InterfLogger> > outlogs;
	wxDECLARE_EVENT_TABLE();
};


bool uiRunExe(wxFrame* parent,const wxString& path,const wxString& labelOutput, wxWindow* progressDialog,smart_ptr<InterfLogger> extLogger=smart_ptr<InterfLogger>(NULL));

#endif
