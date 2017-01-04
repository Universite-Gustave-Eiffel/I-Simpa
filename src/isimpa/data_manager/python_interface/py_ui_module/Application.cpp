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

#include "Application.hpp"
#include "data_manager/python_interface/instanceManager.hpp"
#include "data_manager/tree_rapport/e_report_gabe.h"
#include "IHM/customdialog/customdlg.h"
#include "../pythonshell.hpp"
#include "UtfConverter.h"
#include <wx/filename.h>
#include "last_cpp_include.hpp"

#ifdef USE_PYTHON

using namespace boost::python;
using namespace uictrl;

/**
 * Definition des paramètres par défaut
 * @ref http://www.boost.org/doc/libs/1_37_0/libs/python/doc/v2/overloads.html
 */

BOOST_PYTHON_FUNCTION_OVERLOADS(importscene_overloads,application::importscene,1,5)
BOOST_PYTHON_FUNCTION_OVERLOADS(sendevent_overloads,application::sendevent,2,3)
BOOST_PYTHON_FUNCTION_OVERLOADS(saveproject_overloads,application::saveproject,0,1)

const char* TranslateBoost(const std::wstring& string)
{
	return _(string);
}

/**
 * @ref http://www.boost.org/doc/libs/1_37_0/libs/python/doc/tutorial/doc/html/python/techniques.html#python.reducing_compiling_time
 */
void export_application_class()
{

	//,args("String"),_("Traduction de la chaine dans le language locale")
 def("_",TranslateBoost,args("String"),"Use the I-SIMPA translation dictionary and return the translated version of the parameter.");
 class_<application>("application",no_init)
	 .def("clearlogdata", &application::clearlogdata, "Clear the text in the logging window.")
	 .def("clearshell", &application::clearshellhisto, "Clear the python console content.")
	 .staticmethod("clearshell")
	 .def("newproject", &application::newproject, "Close the current project, then make an empty project.")
	 .def("getrootscene", &application::getrootscene, "Return the element id of the scene tree root.")
	 .def("getrootcore", &application::getrootcore, "Return the element id of the core tree root.")
	 .def("getrootreport", &application::getrootreport, "Return the element id of the core report root.")
	 .def("getrootpreference", &application::getrootpreference, "Return the element id of the user option tree root (project independent).")
	 .def("getdataarray", &application::getdataarray, "Return the data of a grid as shown to the user. Works with element with properties and element of GABE data type.",args("element"))
	 .def("loadproject",&application::loadproject, "Close the current project without saving then load the I-SIMPA project file from the path parameter.",arg("path"))
	 .def("savelog",&application::savelog, "Save the log window into a text file.",arg("path"))
	 .def("saveshell",&application::saveshell, "Save the python shell content into a file.",arg("path"))
	 .def("getuserinput", &application::getuserinput, "Show a dialog with a grid, the user can update the values, then apply or cancel. Rows is a dictionnary and the dictionnary value can be a string (for simple text entry) or list for a combox box entry.",args("title,msg,rows"))
	 .def("register_menu_manager", &application::register_menu_manager, "Register an object to manage the menu popup with the element_type.", args("element_type","manager"))
	 .def("register_event",&application::register_event,"Register a function and return the event_id to set when the menu manager put the associated menu item.",args("object"))
	 .def("saveproject",&application::saveproject, saveproject_overloads("Save the project to the current location if there is no argument.",arg("path=u''")))
	 .def("sendevent", &application::sendevent,sendevent_overloads( "Run the specified event on the argument element.",args("element,eventid,parameters")))
	 .def("importscene", &application::importscene,
		importscene_overloads(
								"Import 3D model from a file.",
								args("modelpath","keepexistinglinkwithgroup","domodelcorrection","domeshsurface","paramTetgen")
							)
		)
	 .def("getlocale", &application::getlocale, "Return the user selected language corresponding to the canonical form of current locale name. Canonical form is the one that is used on UNIX systems: it is a two- or five-letter string in xx or xx_YY format, where xx is ISO 639 code of language and YY is ISO 3166 code of the country. Examples are \"en\", \"en_GB\", \"en_US\" or \"fr_FR\".")
	 .def("reloadgroupsfrommodel",&application::reloadgroupsfrommodel,"Destroy old group then make new group as specified in the 3D model.")
	 .def("getapplicationpath",&application::getapplicationpath,"Return a dictionnary with main application paths.")
	 .def("dropselectionto", &application::dropselectionto, "Drag and Drop selected tree items to the element in argument.",args("element"))
	 .def("setselection", &application::setselection, "Unselect previous selection, then select all the element passed as parameter",args("int_list"))
	 .staticmethod("reloadgroupsfrommodel")
	 .staticmethod("getdataarray")
	 .staticmethod("getrootreport")
	 .staticmethod("clearlogdata")
	 .staticmethod("newproject")
	 .staticmethod("importscene")
	 .staticmethod("getrootscene")
	 .staticmethod("getrootcore")
	 .staticmethod("getrootpreference")
	 .staticmethod("sendevent")
	 .staticmethod("saveproject")
	 .staticmethod("loadproject")
	 .staticmethod("savelog")
	 .staticmethod("saveshell")
	 .staticmethod("getuserinput")
	 .staticmethod("register_menu_manager")
	 .staticmethod("register_event")
	 .staticmethod("getlocale")
	 .staticmethod("getapplicationpath")
	 .staticmethod("dropselectionto")
	 .staticmethod("setselection")
	 ;

}

Element* GetElement(const wxInt32& xmlId);

ProjectManager* GetManager()
{
	void *obj;
	if(InstanceManager::GetReference(OBJECT_REF_TYPE_PROJECT,0,&obj))
	{
		return (ProjectManager*)obj;
	}else{
		throw boost::python::error_already_set();
		return NULL;
	}
}

/**
 * Permet de revenir en mode interface même en cas de crash
 */
class pyeventlocker
{
public:
	pyeventlocker(bool* _marker)
		:marker(_marker)
	{
		*marker=true;
	}
	~pyeventlocker()
	{
		*marker=false;
	}
private:
	bool* marker;
};

namespace uictrl
{
	void application::clearlogdata()
	{
		ProjectManager* self=GetManager();
		self->OnClearConsole();
	}

	void application::clearshellhisto()
	{
		ProjectManager* self=GetManager();
		self->OnClearShell();
	}

	void application::newproject()
	{
		ProjectManager* self=GetManager();
		pyeventlocker lckUi(&self->pyeventmode);
		self->NewProject();
	}

	void application::saveproject(const std::wstring& path)
	{
		ProjectManager* self=GetManager();
		pyeventlocker lckUi(&self->pyeventmode);
		if(path=="")
		{
			self->Save();
		}else{
			self->SaveTo(path,false);
		}
	}

	void application::savelog(const std::wstring& path)
	{
		ProjectManager* self=GetManager();
		pyeventlocker lckUi(&self->pyeventmode);
		self->OnSaveShellToFile(path);
	}
	void application::saveshell(const std::wstring& path)
	{
		ProjectManager* self=GetManager();
		pyeventlocker lckUi(&self->pyeventmode);
		self->OnSaveConsoleToFile(path);

	}
	void application::loadproject(const std::wstring& path)
	{
		ProjectManager* self=GetManager();
		pyeventlocker lckUi(&self->pyeventmode);
		self->Open(path);
	}

	void application::sendevent(const element& pyel, const int& idevent,boost::python::dict parameters)
	{
		ProjectManager* self=GetManager();
		wxInt32 wxid=pyel.getindice();
		//Il faut trouver dans quel contrôle se trouve l'élément puis le séléctionner
		wxCommandEvent eventElement;
		eventElement.SetId(idevent);
		ProjectManager::eventCtrl fromCtrl;
		Element* el=GetElement(wxid);

		if(self->rootResult->GetElementByXMLId(wxid)!=NULL)
		{
			fromCtrl=ProjectManager::EVENT_CTRL_REPORT;
			//self->treeResult->UnselectAll();
			//self->treeResult->SelectItem(el->GetElementInfos().idElement);
		}else if(self->rootCore->GetElementByXMLId(wxid)!=NULL){
			fromCtrl=ProjectManager::EVENT_CTRL_CORE;
			//self->treeCore->UnselectAll();
			//self->treeCore->SelectItem(el->GetElementInfos().idElement);
		}else if(self->rootScene->GetElementByXMLId(wxid)!=NULL){
			fromCtrl=ProjectManager::EVENT_CTRL_SCENE;
			//self->treeScene->UnselectAll();
			//self->treeScene->SelectItem(el->GetElementInfos().idElement);
		}else{
			return;
		}
		self->ClearBatchParameters();
		if(extract_or_throw<int>(parameters.attr("__len__")())!=0)
		{
			std::vector<std::wstring> values;
			std::vector<std::wstring> keys;
			extract_array<std::wstring>(parameters.values(),&values);
			extract_array<std::wstring>(parameters.keys(),&keys);
			std::size_t sizedict=keys.size();
			if(sizedict==values.size())
			{
				for(std::size_t idk=0;idk<sizedict;idk++)
				{
					self->AppendBatchParameters(keys[idk],values[idk]);
				}
			}else{
				wxLogError(_("Incorrect data types. Values and index must be characters"));
			}
		}
		pyeventlocker lckUi(&self->pyeventmode);
		self->ElementEvent(eventElement,fromCtrl,el);
	}
	bool application::importscene(const std::wstring& path,bool keepexistingfacegroup,bool docorrection,bool domeshsurface,const std::wstring& paramTetgen)
	{
		ProjectManager* self=GetManager();
		ProjectManager::t_param_load_model paramLoading;
		paramLoading.pathModel=path;
		paramLoading.keepexistingfacegroup=keepexistingfacegroup;
		paramLoading.docorrection=docorrection;
		paramLoading.domeshsurface=domeshsurface;
		paramLoading.glueSurfaces=true;
		paramLoading.paramTetgen=paramTetgen;
		return self->LoadScene(paramLoading);
	}

	int application::getrootscene()
	{
		ProjectManager* self=GetManager();
		return self->rootScene->GetXmlId();
	}
	int application::getrootcore()
	{
		ProjectManager* self=GetManager();
		return self->rootCore->GetXmlId();
	}
	int application::getrootreport()
	{
		ProjectManager* self=GetManager();
		return self->rootResult->GetXmlId();
	}

	int application::getrootpreference()
	{
		ProjectManager* self=GetManager();
		return self->rootUserConfig->GetXmlId();
	}
	/*
	boost::python::list GetPropertyListFromElement(Element* eltoread)
	{
		list retlst;

		eltoread->InitProp();

		std::vector<Element*> filsNoeud;
		eltoread->GetChildElement(filsNoeud);

		for(long i=0;i<filsNoeud.size();i++)
		{
			if(!filsNoeud[i]->GetElementInfos().hidden)
			{
				E_Data* elementFils=dynamic_cast<E_Data*>(filsNoeud[i]);
				if(elementFils) //Si il s'agit d'un objet dérivé de data
				{

				}
			}
		}
		return retlst;
	}
	*/
	boost::python::list application::getdataarray(const element& pyel)
	{
		ProjectManager* self=GetManager();
		list retlst;
		Element* el=GetElement(pyel.getindice());
		if(el)
		{
			if(el->IsContainsProperties())
			{
				self->OpenPropertyElement(el);
				list collbllst;
				collbllst.append(std::wstring());
				for(int idcol=0;idcol<self->propFrame->GetNumberCols();idcol++)
					collbllst.append(WXSTRINGTOSTDWSTRING(self->propFrame->GetColLabelValue(idcol)));
				retlst.append(collbllst);
				for(int idrow=0;idrow<self->propFrame->GetNumberRows();idrow++)
				{
					list rowlbllst;
					rowlbllst.append(WXSTRINGTOSTDWSTRING(self->propFrame->GetRowLabelValue(idrow)));
					for(int idcol=0;idcol<self->propFrame->GetNumberCols();idcol++)
						rowlbllst.append(WXSTRINGTOSTDWSTRING(self->propFrame->GetCellValue(idrow,idcol)));
					retlst.append(rowlbllst);
				}
			}else if(el->IsFittingWithThisType(Element::ELEMENT_TYPE_REPORT_GABE))
			{
				E_Report_Gabe* gabeEl=dynamic_cast<E_Report_Gabe*>(el);
				if(gabeEl)
				{
					wxString arrayTitle;
					std::vector<wxString> lblCols;
					std::vector<wxString> lblRows;
					std::vector<wxString> cells;
					std::vector<float> cellsValue;
					gabeEl->GetArrayData(NULL,arrayTitle,lblCols,lblRows,cells,cellsValue);
					wxInt32 nbrows(lblRows.size());
					wxInt32 nbcols(lblCols.size());
					list collbllst;
					collbllst.append(std::wstring());
					for(int idcol=0;idcol<nbcols;idcol++)
						collbllst.append(WXSTRINGTOSTDWSTRING(lblCols.at(idcol)));
					retlst.append(collbllst);
					bool loadfloatval=((nbrows*nbcols)==cellsValue.size());
					for(int idrow=0;idrow<nbrows;idrow++)
					{
						list rowlbllst;
						rowlbllst.append(WXSTRINGTOSTDWSTRING(lblRows.at(idrow)));
						if(loadfloatval)
						{
							for(int idcol=0;idcol<nbcols;idcol++)
								rowlbllst.append(cellsValue.at((idcol*nbrows)+idrow));
						}else{
							for(int idcol=0;idcol<nbcols;idcol++)
								rowlbllst.append(WXSTRINGTOSTDWSTRING(cells.at((idcol*nbrows)+idrow)));
						}
						retlst.append(rowlbllst);
					}
				}
			}
		}

		return retlst;
	}

	boost::python::tuple application::getuserinput(const std::wstring& title,const std::wstring& msg,boost::python::dict rows)
	{
		ProjectManager* self=GetManager();
		wxCustomEntryDialog parametersDialog(self->mainFrame,_(wxString(msg)),_(wxString(title)));
		std::vector<object> arrayKeys;
		//Ne pas traduire ici les libellés des champs
		int numItems = boost::python::len(rows.keys());
		object iter = rows.iteritems();
		for(int idItem = 0;idItem<numItems;idItem++) {
			tuple item = extract_or_throw<tuple>(iter.attr("next")());
			arrayKeys.push_back(item[0]);
			std::string object_classname = boost::python::extract<std::string>(item[1].attr("__class__").attr("__name__"));
			if(object_classname == "str") {
				// String cell
				parametersDialog.AddTextControl(extract_wxstring(item[0]), extract_wxstring(item[1]));
			
			} else if(object_classname == "list") {
				//choice cell
				std::list<std::wstring> cArrValues;
				if (extract_array<std::wstring, std::list<std::wstring> >(item[1], &cArrValues))
				{
					wxArrayString arrayValues;
					arrayValues.reserve(cArrValues.size());
					for (std::list<std::wstring>::iterator itvals = cArrValues.begin(); itvals != cArrValues.end(); itvals++)
					{
						arrayValues.push_back(*itvals);
					}
					wxString defaultValue;
					if (!cArrValues.empty())
						defaultValue = arrayValues[0];
					parametersDialog.AddListBox(extract_wxstring(item[0]), defaultValue, arrayValues);
				}
			}
		}
		if (parametersDialog.ShowModal() == wxID_OK)
		{
			dict values;
			std::vector<wxString> fieldsValue;
			parametersDialog.GetValues(fieldsValue);
			for(int fieldId=0;fieldId<fieldsValue.size();fieldId++)
			{
				values[arrayKeys[fieldId]] = fieldsValue.at(fieldId).ToStdWstring();
			}
			return boost::python::make_tuple(true,values);
		}else{
			return boost::python::make_tuple(false,dict());
		}
	}


	void  application::register_menu_manager(const int& element_typeid, boost::python::object& manager)
	{
		ProjectManager* self=GetManager();
		self->GetPythonShell()->register_menu_manager(element_typeid,manager);
	}
	int application::register_event(boost::python::object& func)
	{
		ProjectManager* self=GetManager();
		return Element::IDEVENT_LAST_FIXED+self->GetPythonShell()->register_event(func);
	}
	void application::reloadgroupsfrommodel()
	{
		ProjectManager* self=GetManager();
		self->LoadFacesFromModel();
	}

	boost::python::dict application::getapplicationpath()
	{
		ProjectManager* self=GetManager();
		boost::python::dict retdict;
		retdict["lastcalculationpath"]=WXSTRINGTOSTDWSTRING(self->LastComputationFolder);
		retdict["cachedir"]=WXSTRINGTOSTDWSTRING(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath);
		wxFileName userscript("UserScript/");
		userscript.MakeAbsolute();
		retdict["userscript"]=WXSTRINGTOSTDWSTRING(userscript.GetFullPath());
		return retdict;
	}
	std::wstring application::getlocale()
	{
		return WXSTRINGTOSTDWSTRING(wxGetLocale()->GetCanonicalName());
	}

	uiTreeCtrl* application::GetTreeFromElement(const wxTreeItemId& treeItemId)
	{
		ProjectManager* self=GetManager();
		if(treeItemId.IsOk())
		{
			if(self->treeScene->GetElement(treeItemId))
				return self->treeScene;
			else if(self->treeCore->GetElement(treeItemId))
				return self->treeCore;
			else if(self->treeResult->GetElement(treeItemId))
				return self->treeResult;
			else if(self->treeUserPref->GetElement(treeItemId))
				return self->treeUserPref;
		}
		return NULL;
	}
	void application::dropselectionto(const element& pyel)
	{
		ProjectManager* self=GetManager();
		Element* el=GetElement(pyel.getindice());
		if(el)
		{
			wxTreeItemId iddrop=el->GetElementInfos().idElement;
			if(iddrop.IsOk())
			{
				wxTreeCtrl* droptree=GetTreeFromElement(iddrop);
				//Drag
				wxTreeEvent dragevt(wxEVT_COMMAND_TREE_BEGIN_DRAG,droptree);
				wxPostEvent(droptree,dragevt);
				//& drop
				wxTreeEvent dropevt(wxEVT_COMMAND_TREE_END_DRAG,droptree,iddrop);
				wxPostEvent(droptree,dropevt);
			}
		}
	}

	void application::setselection( const boost::python::list& intlst)
	{
		ProjectManager* self=GetManager();
		std::vector<int> elementLst;
		//Convert python list to std:vector<int> list
		elementLst.reserve(GetPythonArraySize(intlst));
		if(extract_array<int,std::vector<int> >(intlst,&elementLst))
		{
			//If there is at least one element
			if(!elementLst.empty())
			{
				//Initialisation of treeitemid list
				std::vector<wxTreeItemId> selectionLst;
				selectionLst.reserve(elementLst.size());
				for(std::vector<int>::iterator itv=elementLst.begin();itv!=elementLst.end();itv++)
				{
					Element* el=GetElement(*itv);
					if(el)
						selectionLst.push_back(el->GetElementInfos().idElement);
				}
				//Get the first element to find the associated tree control
				if(!selectionLst.empty())
				{
					uiTreeCtrl* actiontree=GetTreeFromElement(selectionLst[0]);
					actiontree->UnselectAll();
					for(std::vector<wxTreeItemId>::iterator its=selectionLst.begin();its!=selectionLst.end();its++)
					{
						if(actiontree==GetTreeFromElement(*its))
							actiontree->ToggleItemSelection(*its);
					}
				}
			}
		}
	}
}

#endif
