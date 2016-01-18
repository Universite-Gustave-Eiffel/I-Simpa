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


#include "pyConsole.hpp"
#include "data_manager/appconfig.h"
#include <wx/log.h>
#include "last_cpp_include.hpp"

BEGIN_EVENT_TABLE( PyConsole, wxTextCtrl )
	EVT_TEXT_ENTER(wxID_ANY, PyConsole::OnTextEnter)
	EVT_KEY_DOWN (PyConsole::OnKeyDown)
END_EVENT_TABLE()

PyConsole::PyConsole(wxWindow* parent)
: wxTextCtrl(parent,-1, "",wxDefaultPosition, wxSize(200,150), wxTE_RICH | wxNO_BORDER | wxTE_MULTILINE | wxTE_PROCESS_TAB   )
{
	waitingForNextPrompt=true;
	promptSize=0;
	wxFont pythonFont(this->GetFont());
	if(pythonFont.SetFaceName("Courier"))
	{
		pythonFont.SetPointSize(10);
		this->SetFont(pythonFont);
	}
}


void PyConsole::AddCmd(const wxString& pyCmd,bool atInsertionPoint)
{
	SetDefaultStyle(*wxBLACK);
	if(atInsertionPoint)
		WriteText(pyCmd);
	else
		AppendText(pyCmd);
}

void PyConsole::Clear()
{
	lastpromptpos=0;
	wxTextCtrl::Clear();
}
void PyConsole::AddPrompt(const wxString& pyPrompt)
{
	SetDefaultStyle(*wxBLACK);
	AppendText(pyPrompt);
	lastpromptpos=this->GetInsertionPoint();
	waitingForNextPrompt=false;
}
void PyConsole::AddPythonMsg(const wxString& pyMsg)
{
	SetDefaultStyle(*wxBLUE);
	AppendText(pyMsg);
}

void PyConsole::AddPythonError(const wxString& pyErr)
{
	SetDefaultStyle(*wxRED);
	AppendText(pyErr);
}
void PyConsole::OnTextEnter(wxCommandEvent& txtEvent)
{
	wxCommandEvent PostPythonCodeUpdate( wxEVT_PSPS_MAIN_EVENT, GetId() );
	PostPythonCodeUpdate.SetInt(ApplicationConfiguration::MAIN_EVENT_POST_PYTHON_COMMAND);
	wxString command=this->GetRange(lastpromptpos,GetLastPosition());
	command.Replace("\n","\\n");
	PostPythonCodeUpdate.SetString(command);
	wxPostEvent(this, PostPythonCodeUpdate);
	waitingForNextPrompt=true;
	/*
	wxArrayString commands;
	SplitString(command,"\n",commands);
	for(size_t i=0;i<commands.size();i++)
	{
		PostPythonCodeUpdate.SetString(commands[i]);
		wxPostEvent(this, PostPythonCodeUpdate);
		waitingForNextPrompt=true;
	}
	*/
}
void PyConsole::OnKeyDown(wxKeyEvent& txtEvent)
{
	int keycod=txtEvent.GetKeyCode();
	long pos=GetInsertionPoint();
	long x,y;
	PositionToXY(pos,&x,&y);

	if(keycod==WXK_TAB)
	{
		AddCmd("    ",true); //On remplace les tabulations par des espaces
	}else if(keycod==WXK_BACK){
		if(pos>lastpromptpos) //On conserve le nombre de caractère minimal sur une ligne
		{
			long posBefore=pos-4;
			if(posBefore>0)
			{
				wxString line=GetRange(posBefore,pos);
				if(line=="    ") //on supprime une tabulation complète
				{
					this->Remove(posBefore,pos);
				}else{
					txtEvent.Skip();
				}
			}
		}
	}else if(keycod==WXK_RETURN || keycod==WXK_NUMPAD_ENTER){
		if(pos<lastpromptpos-promptSize)
		{
			//On copie la ligne courante a la toute fin de la console
			long debutLigne=XYToPosition(0,y);
			
			wxString line=GetRange(debutLigne+promptSize,debutLigne+GetLineLength(y));
			AddCmd(line);
		}else if(!waitingForNextPrompt){
			SetInsertionPointEnd();
			txtEvent.Skip();
		}
	}else if(keycod==WXK_HOME)
		{
		if(y==GetNumberOfLines()-1)
		{
			SetInsertionPoint(lastpromptpos);
		}else{
			txtEvent.Skip();
		}
	
	}else{
		//(txtEvent.ControlDown() && keycod==67) pour autoriser ctrl-c avant >>>
		if((!waitingForNextPrompt && pos>=lastpromptpos )|| keycod==WXK_UP  || keycod==WXK_DOWN  || keycod==WXK_LEFT  || keycod==WXK_RIGHT  || keycod==WXK_HOME  || keycod==WXK_PAGEUP  || keycod==WXK_PAGEDOWN  || keycod==WXK_END || (txtEvent.ControlDown() && keycod==67) ) //On permet la modification du texte seulement sur la derniere ligne
		{
			txtEvent.Skip(); //on autorise la saisie
		}
	}
}