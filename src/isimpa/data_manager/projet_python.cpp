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

/**
 * @file projet_python.cpp
 * @brief Specification des méthodes de ProjectManager ayant un rapport avec le module python
 */
#include "python_interface/pythonshell.hpp"
#include "projet.h"
#include "last_cpp_include.hpp"


#ifdef USE_PYTHON

extern ProjectManager *projetCourant;

using namespace boost::python;

void SendPythonCommand(const wxString& cmd)
{
	projetCourant->RunPythonCmd(cmd);
}
bool ins_pyelement(boost::python::object& py_el,const wxInt32& wxid, const wxString& module,const wxString& _pyclass)
{
  PythonShell* pyshell=projetCourant->GetPythonShell();
  if(pyshell)
  {
	return pyshell->ins_pyelement(py_el,wxid,module,_pyclass);
  }
  return false;
}
bool RunPyManagedMenu(const int& elementType,const int& elementId, wxMenu& menu)
{
	return projetCourant->GetPythonManagedMenu(elementType,elementId,menu);
}
bool GetFormulaEvaluation(const wxString& formula,double& evalResult)
{
  PythonShell* pyshell=projetCourant->GetPythonShell();
  if(pyshell)
  {
    try
	{
      	evalResult=extract_or_throw<double>(pyshell->eval(formula),WXSTRINGTOSTDWSTRING(wxGetTranslation("The formula result must be a number")));
	}catch( error_already_set ) {
		wxLogError(wxGetTranslation("Unable to solve the user's defined formula !"));
		return false;
    }
	catch( elementException elexc)
	{

		wxLogError(wxGetTranslation("Unable to solve the user's defined formula :\n")+elexc.msg());
		return false;
	}
	//return pyshell->ins_pyelement(py_el,wxid,module,_pyclass);
	return true;
  }
  return false;
}
void ProjectManager::InitPythonEngine()
{
	if(pyShell==NULL)
	{
		PythonShell::load_uictrl();

		Py_Initialize();
		
		pyShell = new PythonShell(shellControl);

		pyShell->Init();
		
	}
}

void GetListItems(boost::python::list& pymenu, wxMenu& menu);
boost::python::list GetMenuItems(wxMenu* menuToFill);

void ProjectManager::PyProjectLoadingEvt()
{
	if(pyShell)
	{
		pyShell->run_startupscript(L"UserScript/",L"__project_loading__.py");
	}

}

bool ProjectManager::GetPythonManagedMenu(const int& elementType,const int& elementId, wxMenu& menu)
{
	try
	{
		//1ere étape transformer le menu en objet python
		boost::python::list pymenu=GetMenuItems(&menu);
		//2eme étape fournir à pyshell les éléments
		if(pyShell)
		{
			if(pyShell->GetPythonManagedMenu(elementType,elementId,pymenu))
			{
				//Supprime l'ancien menu
				wxMenuItemList menuitems=menu.GetMenuItems();
				wxMenuItemList::iterator iter;
				for (iter = menuitems.begin(); iter != menuitems.end(); ++iter)
				{
					wxMenuItem *current = *iter;
					menu.Destroy(current);
				}
				//Ajoute les anciens et les nouveaux éléments de menu
				GetListItems(pymenu,menu);
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	} catch( error_already_set ) {
		if (PyErr_Occurred())
		{
			PyErr_Print();
		}
	}
	return false;
}
void ProjectManager::OnClearShell()
{
	shellControl->Clear();
}
void ProjectManager::UnloadPyShell()
{
	delete pyShell;
	pyShell=NULL;
	Py_Finalize();
}
void ProjectManager::OnPushPythonCommand(wxCommandEvent& eventCmdPython)
{
	RunPythonCmd(eventCmdPython.GetString());
}
void ProjectManager::RunPythonCmd(const wxString& cmd)
{
	//wxCriticalSectionLocker locker(m_critsectWork);
	if(pyShell)
		pyShell->ExecLineCommand(cmd);
}
void ProjectManager::AppendBatchParameters(const wxString& name,const wxString& value)
{
	this->pyenventparams[name]=value;
}
void ProjectManager::ClearBatchParameters()
{
	this->pyenventparams.clear();
}
bool ProjectManager::GetParameter(const wxString& name,wxString& value)
{
	const auto it=this->pyenventparams.find(name);
	if(it != this->pyenventparams.end())
	{
		value=it->second;
		return true;
	}
	wxLogDebug(wxGetTranslation("Python(TM) plugin event parameter %s not found"),name);
	return false;
}

PythonShell* ProjectManager::GetPythonShell()
{
	return pyShell;
}


void ProjectManager::SendPythonEvent(const int& eventid,const int& elementid)
{
	pyShell->call_event(eventid,elementid);
}

#else
bool GetFormulaEvaluation(const wxString& formula,double& evalResult)
{
	return false;
}
void SendPythonCommand(const wxString& cmd)
{
}

bool RunPyManagedMenu(const int& elementType,const int& elementId, wxMenu& menu)
{
	return false;
}

void ProjectManager::InitPythonEngine()
{
}

void ProjectManager::PyProjectLoadingEvt()
{
}

bool ProjectManager::GetPythonManagedMenu(const int& elementType,const int& elementId, wxMenu& menu)
{
	return false;
}
void ProjectManager::OnClearShell()
{
	
}
void ProjectManager::UnloadPyShell()
{
	
}
void ProjectManager::OnPushPythonCommand(wxCommandEvent& eventCmdPython)
{

}
void ProjectManager::RunPythonCmd(const wxString& cmd)
{

}
void ProjectManager::AppendBatchParameters(const wxString& name,const wxString& value)
{
	
}
void ProjectManager::ClearBatchParameters()
{
	
}
bool ProjectManager::GetParameter(const wxString& name,wxString& value)
{
	return false;
}

PythonShell* ProjectManager::GetPythonShell()
{
	return NULL;
}


void ProjectManager::SendPythonEvent(const int& eventid,const int& elementid)
{
	
}
#endif
