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
    processManager(wxFrame *parent, const wxString& cmd)
        : wxProcess((wxEvtHandler*)parent), m_cmd(cmd)
    {
        m_parent = parent;
        Redirect();
		run=true;
    }

    // instead of overriding this virtual function we might as well process the
    // event from it in the frame class - this might be more convenient in some
    // cases
	/**
	 * Utilise les messages de la pile de sortie de l'executable externe afin de les afficher vers la sortie wxLog
	 * @param[out] hasOutput Passe à vrai si il reste encore des lignes dans la pile mémoire.
	 * @param[in] label Libellé du message qui précede le texte de sortie
	 * @param[out] Si une des sortie est de la forme "#entier" alors affecte cet entier dans cette variable
	 */
	void LogOutput(bool &hasOutput, const wxString &label=wxString(_("Calculation output:")),float *outputProgression=NULL);
	/**
	 * Permet de connaître l'etat de l'execution
	 * @return Vrai si l'application est en cours d'exécution
	 */
	bool IsRunning();
    /**
	 * Add a logger object
	 * @param logger Instance of log object
	 */
	void AddLogger(smart_ptr<InterfLogger> logger);
private:
	/**
	 * Fin de l'execution de l'application externe
	 */
    virtual void OnTerminate(int pid, int status);
	bool run;
    wxString m_cmd;
	wxFrame* m_parent;
	std::vector<smart_ptr<InterfLogger> > outlogs;
};


bool uiRunExe(wxFrame* parent,const wxString& path,const wxString& labelOutput, wxWindow* progressDialog,smart_ptr<InterfLogger> extLogger=smart_ptr<InterfLogger>(NULL));

#endif
