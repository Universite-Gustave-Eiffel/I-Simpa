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

#include "pythonshell.hpp"
#include <wx/dir.h>
#include <wx/log.h>
#include "UtfConverter.h"
#include "last_cpp_include.hpp"

#ifdef USE_PYTHON

using namespace boost::python;

void export_application_class();
void export_project_class();
void export_stdioclass()
{

}
BOOST_PYTHON_MODULE(uictrl){
	//export_stdioclass();
	export_project_class();
	export_application_class();
}

PythonShell::PythonShell(PyConsole* pyCtrl)
{
	promptNewCmd=">>> ";
	promptMore="... ";
	m_py_ctrl=pyCtrl;
	outputRedirectOut=NULL;
	outputRedirectErr=NULL;
	outputRedirectIn=NULL;

	main_module = import("__main__");
	inspect_module = import("inspect");
	main_namespace = main_module.attr("__dict__");

	//Enregistrement des modules
	PyImport_AppendInittab("uictrl", inituictrl);
    //throw std::runtime_error(_("Impossible d'importer le module de psps dans l'interpreteur python.").t_str());

	//Python va copier les objets passés en paramètre

	main_namespace["PythonStdIoRedirect"] = class_<PythonStdIoRedirect>("PythonStdIoRedirect", no_init)
	.def("write", &PythonStdIoRedirect::Write)
	.def("readline", &PythonStdIoRedirect::ReadLine);
	object sys=boost::python::import("sys");

	PythonStdIoRedirect outputRedirectFrame;
	sys.attr("stderr") = outputRedirectFrame;
	sys.attr("stdout") = outputRedirectFrame;
	sys.attr("stdin") = outputRedirectFrame;

	//De notre coté on va récuperer ses objets et les stocker

	outputRedirectOut=extract<PythonStdIoRedirect*>(sys.attr("stdout"));
	outputRedirectErr=extract<PythonStdIoRedirect*>(sys.attr("stderr"));
	outputRedirectIn=extract<PythonStdIoRedirect*>(sys.attr("stdin"));

	//Ajout du dossier de script
	boost::python::import("site").attr("addsitedir")("UserScript");
	boost::python::import("site").attr("addsitedir")("SystemScript");
}
PythonShell::~PythonShell()
{

}
void PythonShell::GetIndentation(const wxString& command,wxString* indentationBlock)
{
	wxChar fracTab(' ');
	wxUint32 sizeofstring(command.size());
	wxInt32 idc=0;
	while(idc<sizeofstring && command.GetChar(idc)==fracTab)
		idc++;
	if(idc>0)
		*indentationBlock=wxString(fracTab,idc);
	else
		indentationBlock->clear();
}
wxString PythonShell::GetStr(const wxString& strPos)
{
	char const* c_str = extract<char const*>(main_namespace[strPos]);
	return c_str;
}
bool PythonShell::ShowMsgStack()
{
	bool showing_something=false;
	if(outputRedirectErr && outputRedirectOut)
	{
		if( outputRedirectErr->HasOutput() || outputRedirectOut->HasOutput())
			showing_something=true;
		wxString msg;
		while(outputRedirectErr->GetOutput(msg))
			m_py_ctrl->AddPythonError(msg);
		while(outputRedirectOut->GetOutput(msg))
			m_py_ctrl->AddPythonMsg(msg);
	}
	return showing_something;
}

void PythonShell::Init()
{
	RunRawCmd("import code");
	RunRawCmd("import __main__");
	RunRawCmd("import sys");
	RunRawCmd("import uictrl as ui");
	RunRawCmd(_("print 'Python(TM)',sys.version,'on',sys.platform"));
	run_startupscript("UserScript/","__ui_startup__.py");
	run_startupscript("SystemScript/","__ui_startup__.py");
	//RunRawCmd("emulationDict=dict()");
	RunRawCmd("consoleEmulation=code.InteractiveInterpreter(__main__.__dict__)"); //emulationDict
	wxString promptNewCmd(">>> ");
	wxString promptMore("... ");
	wxString indentation="";
	m_py_ctrl->SetPromptSize(promptNewCmd.Length());
	ShowMsgStack();
	m_py_ctrl->AddPrompt(promptNewCmd);

}

boost::python::object PythonShell::eval(const wxString& code)
{
	RunRawCmd("res="+code);
	return main_namespace["res"];
}
void PythonShell::ExecLineCommand(const wxString& newcommand)
{
	if(outputRedirectIn)
	{
		wxString indentation;
		outputRedirectIn->WriteWx(newcommand);
		int currentIndentation=0;
		while(outputRedirectIn->HasOutput())
		{
			outputRedirectIn->GetOutput(cmd);
			//Si commande avec seulement des indentations on supprime les indentations pour executer la commande
			if(cmd.Freq(' ')==cmd.size())
				cmd.clear();
			cmd.Replace("\\n","\n");
			//Pour déduire la prochaine indentation a utiliser
			GetIndentation(cmd,&indentation);
			cmd=oldcmd+cmd;
			main_module.attr("cmd")=std::wstring(cmd.c_str());
			RunRawCmd("res=consoleEmulation.runsource(cmd)");
			bool res=extract<bool>(main_namespace["res"]); //on recupere la valeur de retour
			ShowMsgStack();
			if(res)
			{ //La commande nécessite une ligne de code supplémentaire
				oldcmd=cmd+wxString("\n");
				m_py_ctrl->AddPrompt(promptMore);

				if(cmd.Right(1)==":") //Ajout d'indentation
					indentation+="    ";
				m_py_ctrl->AddCmd(indentation);
			}else{
				//La commande est complète
			   oldcmd.clear();
			   indentation.clear();
			   m_py_ctrl->AddPrompt(promptNewCmd);
			}

		}
		ShowMsgStack();
	}else{
		RunRawCmd(newcommand);
		m_py_ctrl->AddPrompt(promptNewCmd);
	}
}

void PythonShell::RunRawCmd(const wxString& command)
{
	if(handle_exception(boost::bind(exec, WXSTRINGTOCHARPTR(command),main_namespace,main_namespace)))
	{
		if (PyErr_Occurred())
		{
			PyErr_Print();
		}
		else
		{
			if(outputRedirectErr)
				this->outputRedirectErr->WriteWx(_("Python(TM) implementation error can't be produced"));
		}
	}
	ShowMsgStack();
}

/*
void register_menu_manager(const wxString& element_typename, boost::python::object& manager)
{


}
*/
void PythonShell::register_menu_manager(const int& element_typeid, boost::python::object& manager)
{
	if(hasattr(manager,"getmenu"))
	{
		if(extract_or_throw<bool>(this->menu_managers.attr("has_key")(element_typeid)))
		{
			this->menu_managers[element_typeid].attr("append")(manager);
		}else{
			list lstmanagers;
			lstmanagers.append(manager);
			this->menu_managers[element_typeid]=lstmanagers;
		}
	}else{
		ThrowPyException(_("A menu manager must contain the following method: bool getmenu(eltype,elid,listmenu) "));
	}
}
int PythonShell::register_event(boost::python::object& func)
{
	event_lst.append(func);
	return GetCountEventTable()-1;
}

void PythonShell::call_event(const int& eventid,const int& elementid)
{
	if(eventid>=0 && GetCountEventTable()>eventid)
	{	try
		{
			//Il faut définir si la méthode attent 1 ou 2 argument
			//On utilise le module inspect afin d'obtenir le nombre d'argument
			boost::python::object event_function=event_lst[eventid];
			boost::python::list arglist=extract_or_throw<boost::python::list>(inspect_module.attr("getargspec")(event_function).attr("args"));
			boost::python::ssize_t argCount=len(arglist);
			//Self ne doit pas être pris en compte dans le nombre d'argument
			if(extract_or_throw<bool>(arglist.attr("__contains__")("self")))
				argCount--;
			if(argCount==2)
				event_function(elementid,eventid);
			else
				event_function(elementid);
		} catch( error_already_set ) {
			if (PyErr_Occurred())
			{
				PyErr_Print();
			}
		}
		if(ShowMsgStack())
		   m_py_ctrl->AddPrompt(promptNewCmd);
	}else{
		ThrowPyException(_("This event doesn't exist !"));
	}
}

int PythonShell::GetCountEventTable()
{
	return extract_or_throw<int>(event_lst.attr("__len__")());
}
bool PythonShell::GetPythonManagedMenu(const int& element_type,const int& elementId,boost::python::list& pymenu)
{
	if(extract_or_throw<bool>(this->menu_managers.attr("has_key")(element_type)))
	{
		bool modified=false;
		std::vector<object> lstManagers;
		try
		{
			if(extract_array<object>(this->menu_managers[element_type],&lstManagers))
			{
				size_t managercounts(lstManagers.size());
				for(size_t i=0;i<managercounts;i++)
				{
					try
					{
						if(extract_or_throw<bool>(lstManagers[i].attr("getmenu")(element_type,elementId,pymenu)))
							modified=true;
					} catch( error_already_set ) {
						if (PyErr_Occurred())
						{
							PyErr_Print();
						}
						ShowMsgStack();
					}
				}
				return modified;
			}else{
				return false;
			}
		} catch( error_already_set )
		{
			if (PyErr_Occurred())
			{
				PyErr_Print();
			}
			if(ShowMsgStack())
			   m_py_ctrl->AddPrompt(promptNewCmd);
			return false;
		}
		catch( const char * msg)
		{
			wxLogError(msg);
		}
	}else{
		return false;
	}
}
bool PythonShell::ins_pyelement(boost::python::object& py_el,const wxInt32& wxid, const wxString& module,const wxString& _pyclass)
{
	bool isOk=false;
	/*
	main_module = import("__main__");
	main_namespace = main_module.attr("__dict__");
	*/
  // Define the derived class in Python.
	try
	{
		boost::python::object user_module = import(WXSTRINGTOCHARPTR(module));
		py_el = user_module.attr(WXSTRINGTOCHARPTR(_pyclass))(wxid);
		isOk=true;
	}
	catch( error_already_set )
	{
		if (PyErr_Occurred())
		{
			PyErr_Print();
			wxLogError(_("Unable to init the Python(TM) element %s of the %s module"),_pyclass,module);
		}
	}

	return isOk;
}
void PythonShell::run_startupscript(const wxString& scriptPath,const wxString& pyfilename)
{
	//recherche des fichier se nommant __ui_startup__.py
	wxArrayString files_found;
	if(!wxDirExists(scriptPath))
		wxMkDir(scriptPath,0777);
	if(wxDir::GetAllFiles(scriptPath,&files_found,pyfilename))
	{
		for(size_t ifile=0;ifile<files_found.size();ifile++)
		{

			wxString scriptFile=files_found[ifile];
			try
			{

				boost::python::exec_file(WXSTRINGTOCHARPTR(scriptFile),main_namespace,main_namespace);

			}catch( error_already_set )
			{
				if (PyErr_Occurred())
				{
					PyErr_Print();
				}
			}
			if(ShowMsgStack())
			   m_py_ctrl->AddPrompt(promptNewCmd);
		}
	}
}

#endif
