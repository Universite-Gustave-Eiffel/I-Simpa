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

#ifndef __PY_CONSOLE_CTRL_
#define __PY_CONSOLE_CTRL_
/**
 * @brief Console Python
 *
 * C'est le contrôle texte qui permet d'emuler python si il est utilisé avec PythonShellThread
 */
class PyConsole : public wxTextCtrl
{
protected:
	void OnTextEnter(wxCommandEvent& txtEvent);
	void OnKeyDown(wxKeyEvent& txtEvent);
	int promptSize; /*!< Nombre de caractère de la ligne de commande */
	long lastpromptpos;
	bool waitingForNextPrompt; /*!< L'utilisateur ne peut taper de command si python ne nous a pas laissé la main */
public:

	PyConsole(wxWindow* parent);
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
	void AddPrompt(const wxString& pyPrompt);
	
    virtual void Clear();
	/**
	 * Ajoute des caractère de la même couleur que le texte tapé par l'utilisateur
	 */
	void AddCmd(const wxString& pyCmd,bool atInsertionPoint=false);
	/**
	 * C'est le nombre de caractère minimal dans une ligne.
	 */
	void SetPromptSize(const wxInt32& lengthPrompt)
	{
		promptSize=lengthPrompt;
	}
	DECLARE_EVENT_TABLE();
};

#endif