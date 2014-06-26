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


#include "pythonstdioredirect.hpp"
#include "IHM/pyConsole.hpp"
#include "boost_python_header.h"

#ifndef __PYTHON_SHELL__
#define __PYTHON_SHELL__


#ifdef USE_PYTHON
/**
 * @brief Thread d'execution de python
 *
 * Simule l'execution d'une console python et communique avec le controle de texte associé
 */
class PythonShell
{
public:
    PythonShell(PyConsole* pyCtrl);
	virtual ~PythonShell();
	/**
	 * Donne le bloc d'indentation de la commande passé en paramètre
	 */
	void GetIndentation(const wxString& command,wxString* indentationBlock);
	wxString GetStr(const wxString& strPos);
	bool ShowMsgStack();
	void ExecLineCommand(const wxString& cmd);
	/**
	 * Evaluate the code argument within the python vm. Then return the value of the evaluation
	 * The code will be run as "res=eval(code)"
	 */
	boost::python::object eval(const wxString& code);
	void Init();
	/**
	 * Enregistre un gestionnaire de menu associé au clic droit sur un type d'élément
	 * Ce gestionnaire ira ensuite alimenter ou non le menu
	 */
	void register_menu_manager(const int& element_typeid, boost::python::object& manager);
	/**
	 * Ajoute un événement dans la table des evenements
	 * @return L'indice de l'événement lié
	 * @see call_event
	 */
	int register_event(boost::python::object& func); 
	/**
	 * Appel la méthode python ayant comme indice celui passé en paramètre
	 * @param eventid Indice de l'événement
	 */
	void call_event(const int& eventid,const int& elementid);



	/**
	 * Cette méthode va parcourir les sous dossier à la recherche des fichier se nommant pyfilename
	 * Puis il les executes
	 */
	void run_startupscript(const wxString& scriptPath,const wxString& pyfilename); 
	int GetCountEventTable();
	/**
	 * Modifie le menu au cas où un gestionnaire de menu python est associé à pythonshell
	 * @return vrai si le menu a été modifié
	 */
	bool GetPythonManagedMenu(const int& element_type,const int& elementId,boost::python::list& pymenu);
	
	bool ins_pyelement(boost::python::object& py_el,const wxInt32& wxid, const wxString& module,const wxString& _pyclass);
	
protected:
	boost::python::object main_module;
	boost::python::object main_namespace;
	boost::python::object inspect_module;
	wxString cmd;
	wxString oldcmd;
	wxString promptNewCmd;
	wxString promptMore;
	void RunRawCmd(const wxString& command);
    PyConsole* m_py_ctrl;
	boost::python::list event_lst; /*!< Cette liste contient les événements supplémentaires implémentés sous python */
	boost::python::dict menu_managers; /*!< Ce dictionnaire contient les générateurs de menu, chacun étant associé à un type d'élément du projet */
	PythonStdIoRedirect* outputRedirectOut;
	PythonStdIoRedirect* outputRedirectErr;
	PythonStdIoRedirect* outputRedirectIn;

};

#endif
#endif