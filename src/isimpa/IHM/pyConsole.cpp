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


#include "pyConsole.hpp"
#include "data_manager/appconfig.h"
#include <wx/log.h>
#include "last_cpp_include.hpp"
#include <wx/sizer.h>
#include <wx/splitter.h>

#include "wx/osx/stattext.h"

PyConsole::PyConsole(wxWindow* parent) : wxPanel(parent)
{
	waitingForNextPrompt=true;
	// Create the splitter window
	auto* splitter = new wxSplitterWindow(this, wxID_ANY);

	// Top control: read-only output display
	m_outputCtrl = new wxTextCtrl(splitter, wxID_ANY, wxEmptyString,
	   wxDefaultPosition, wxDefaultSize,
	   wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2 | wxVSCROLL);

	// Bottom panel: contains prompt label and input control
	auto* inputPanel = new wxPanel(splitter, wxID_ANY);
	auto* inputSizer = new wxBoxSizer(wxHORIZONTAL);

	// Create prompt label
	m_promptLabel = new wxStaticText(inputPanel, wxID_ANY, ">>> ");

	// Create input control
	m_inputCtrl = new wxTextCtrl(inputPanel, wxID_ANY, wxEmptyString,
	   wxDefaultPosition, wxDefaultSize,
	   wxTE_PROCESS_ENTER);

	// Set the label background to match the input control
	m_promptLabel->SetBackgroundColour(m_inputCtrl->GetBackgroundColour());
	inputPanel->SetBackgroundColour(m_inputCtrl->GetBackgroundColour());

	// Add to horizontal sizer
	inputSizer->Add(m_promptLabel, 0, wxALIGN_TOP, 5);
	inputSizer->Add(m_inputCtrl, 1, wxEXPAND, 5);

	inputPanel->SetSizer(inputSizer);

	// Split horizontally with initial sash position at 70% of height
	splitter->SplitHorizontally(m_outputCtrl, inputPanel, -100);
	splitter->SetMinimumPaneSize(50); // Prevent collapsing too small
	splitter->SetSashGravity(0.7); // Keep 70% for output when resizing window

	// Layout
	auto* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(splitter, 1, wxEXPAND);
	SetSizer(sizer);

	// Bind enter key event
	m_inputCtrl->Bind(wxEVT_TEXT_ENTER, &PyConsole::OnCommandEnter, this);
}

void PyConsole::OnCommandEnter(wxCommandEvent& event)
{
	wxCommandEvent PostPythonCodeUpdate( wxEVT_PSPS_MAIN_EVENT, GetId() );
	PostPythonCodeUpdate.SetInt(ApplicationConfiguration::MAIN_EVENT_POST_PYTHON_COMMAND);
	wxString command=event.GetString();
	command.Replace("\n","\\n");
	PostPythonCodeUpdate.SetString(command);
	wxPostEvent(this, PostPythonCodeUpdate);
	waitingForNextPrompt=true;
	m_inputCtrl->Clear();
}

void PyConsole::AppendOutput(const wxString& text)
{
	m_outputCtrl->AppendText(text);
}

void PyConsole::AddPythonMsg(const wxString& pyMsg)
{
	m_outputCtrl->SetDefaultStyle(*wxBLUE);
	m_outputCtrl->AppendText(pyMsg);
}

void PyConsole::AddPythonError(const wxString& pyErr)
{
	m_outputCtrl->SetDefaultStyle(*wxRED);
	m_outputCtrl->AppendText(pyErr);
}

void PyConsole::SetPrompt(const wxString& pyPrompt) {
	m_promptLabel->SetLabel(pyPrompt);
}

void PyConsole::Clear() {
	m_outputCtrl->Clear();
}

void PyConsole::AddCmd(const wxString& pyCmd, bool atInsertionPoint) {
	m_outputCtrl->SetDefaultStyle(*wxBLACK);
	m_outputCtrl->AppendText(pyCmd);
}




//
// void PyConsole::AddCmd(const wxString& pyCmd,bool atInsertionPoint)
// {
// 	SetDefaultStyle(*wxBLACK);
// 	if(atInsertionPoint)
// 		WriteText(pyCmd);
// 	else
// 		AppendText(pyCmd);
// }
//
// void PyConsole::Clear()
// {
// 	lastPromptPosition=0;
// 	wxTextCtrl::Clear();
// }
// void PyConsole::AddPrompt(const wxString& pyPrompt)
// {
// 	SetDefaultStyle(*wxBLACK);
// 	AppendText(pyPrompt);
// 	wxYieldIfNeeded();
// 	lastPromptPosition = GetLastPosition();
// 	waitingForNextPrompt=false;
// }
// void PyConsole::AddPythonMsg(const wxString& pyMsg)
// {
// 	SetDefaultStyle(*wxBLUE);
// 	AppendText(pyMsg);
// }
//
// void PyConsole::AddPythonError(const wxString& pyErr)
// {
// 	SetDefaultStyle(*wxRED);
// 	AppendText(pyErr);
// }
// void PyConsole::OnTextEnter(wxCommandEvent& txtEvent)
// {
// 	wxCommandEvent PostPythonCodeUpdate( wxEVT_PSPS_MAIN_EVENT, GetId() );
// 	PostPythonCodeUpdate.SetInt(ApplicationConfiguration::MAIN_EVENT_POST_PYTHON_COMMAND);
// 	wxString command=this->GetRange(lastPromptPosition,GetLastPosition());
// 	command.Replace("\n","\\n");
// 	PostPythonCodeUpdate.SetString(command);
// 	wxPostEvent(this, PostPythonCodeUpdate);
// 	AppendText("\n");
// 	waitingForNextPrompt=true;
// }
// void PyConsole::OnKeyDown(wxKeyEvent& txtEvent)
// {
// 	int keycod=txtEvent.GetKeyCode();
// 	long pos=GetInsertionPoint();
// 	long x,y;
// 	PositionToXY(pos,&x,&y);
//
// 	if(keycod==WXK_TAB)
// 	{
// 		AddCmd("    ",true); //On remplace les tabulations par des espaces
// 	}else if(keycod==WXK_BACK){
// 		if(pos>lastPromptPosition) //On conserve le nombre de caractère minimal sur une ligne
// 		{
// 			long posBefore=pos-4;
// 			if(posBefore>0)
// 			{
// 				wxString line=GetRange(posBefore,pos);
// 				if(line=="    ") //on supprime une tabulation complète
// 				{
// 					this->Remove(posBefore,pos);
// 				}else{
// 					txtEvent.Skip();
// 				}
// 			}
// 		}
// 	}else if(keycod==WXK_RETURN || keycod==WXK_NUMPAD_ENTER){
// 		if(pos<lastPromptPosition-promptSize)
// 		{
// 			//On copie la ligne courante a la toute fin de la console
// 			long debutLigne=XYToPosition(0,y);
//
// 			wxString line=GetRange(debutLigne+promptSize,debutLigne+GetLineLength(y));
// 			AddCmd(line);
// 		}else if(!waitingForNextPrompt){
// 			SetInsertionPointEnd();
// 			txtEvent.Skip();
// 		}
// 	}else if(keycod==WXK_HOME)
// 		{
// 		if(y==GetNumberOfLines()-1)
// 		{
// 			SetInsertionPoint(lastPromptPosition);
// 		}else{
// 			txtEvent.Skip();
// 		}
//
// 	}else{
// 		//(txtEvent.ControlDown() && keycod==67) pour autoriser ctrl-c avant >>>
// 		if((!waitingForNextPrompt && pos>=lastPromptPosition )|| keycod==WXK_UP  || keycod==WXK_DOWN  || keycod==WXK_LEFT  || keycod==WXK_RIGHT  || keycod==WXK_HOME  || keycod==WXK_PAGEUP  || keycod==WXK_PAGEDOWN  || keycod==WXK_END || (txtEvent.ControlDown() && keycod==67) ) //On permet la modification du texte seulement sur la derniere ligne
// 		{
// 			txtEvent.Skip(); //on autorise la saisie
// 		}
// 	}
//}