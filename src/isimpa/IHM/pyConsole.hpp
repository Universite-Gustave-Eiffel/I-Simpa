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

#include "first_header_include.hpp"

#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/stattext.h>

#ifndef __PY_CONSOLE_CTRL_
#define __PY_CONSOLE_CTRL_
/**
 * @brief Console Python
 *
 *  Python GUI panel with the two controls
 */
class PyConsole : public wxPanel
{
protected:

	wxTextCtrl* m_outputCtrl;
	wxTextCtrl* m_inputCtrl;
	wxStaticText* m_promptLabel;
	void OnCommandEnter(wxCommandEvent& event);
	void AppendOutput(const wxString& text);

	bool waitingForNextPrompt; /*!< The user cannot type commands if Python has not given us control. */

public:
	PyConsole(wxWindow* parent);

	wxTextCtrl* GetOutputControl() { return m_outputCtrl; }
	/**
	 * Ajoute le resulat interpreté par python  dans la console
	 */
	void AddPythonMsg(const wxString& pyMsg);
	/**
	 * Ajoute le message d'erreur dans la console
	 */
	void AddPythonError(const wxString& pyErr);
	/**
	 * Ajoute les caractère de ligne de commande (>>> , ... ou autre)
	 */
	void SetPrompt(const wxString& pyPrompt);
	
    virtual void Clear();
	/**
	 * Ajoute des caractère de la même couleur que le texte tapé par l'utilisateur
	 */
	void AddCmd(const wxString& pyCmd,bool atInsertionPoint=false);
};

#endif