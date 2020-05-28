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

#include "element_pywrap.hpp"
#include <wx/intl.h>
#include "data_manager/python_interface/instanceManager.hpp"
#include "UtfConverter.h"
#include <wx/log.h>
#include "last_cpp_include.hpp"
#include "IHM/customItem/uimenuitem.hpp"

#ifdef USE_PYTHON
using namespace boost::python;
using namespace uictrl;

Element* New_E_Python_Extension( const Element::ELEMENT_TYPE& baseType, Element* parent,const wxString& module,const wxString& pyclass,boost::python::object& pyobj);


void GetListItems(boost::python::list& pymenu, wxMenu& menu)
{
	int size=GetPythonArraySize(pymenu);
    for ( int i = 0; i < size; i++ )
	{
		boost::python::tuple menutuple=extract_or_throw<boost::python::tuple>(pymenu[i]);
		int menuListSize=GetPythonArraySize(menutuple);
		if(menuListSize==0)
		{
			//séparateur
			menu.AppendSeparator();
		}else
		{
			//Si le deuxieme paramètre a une longueur alors il s'agit d'un sous menu
			if(hasattr(extract_or_throw<boost::python::object>(menutuple[1]),"__len__"))
			{

				wxMenu* submenu=new wxMenu();
				//Sous menu
				boost::python::list pytLst=extract_or_throw<boost::python::list>(menutuple[1]);
				GetListItems(pytLst,*submenu);
				menu.Append(Element::GetMenuItem(&menu,-1,_(extract_wxstring(menutuple[0])),submenu));
			}else{
				wxUiMenuItem* newMenu;
				if(menuListSize>=3) //avec ou sans icone
					newMenu=wxDynamicCast(Element::GetMenuItem(&menu,extract_or_throw<int>(menutuple[1]),extract_wxstring(menutuple[0]),extract_wxstring(menutuple[2])),wxUiMenuItem);
				else
					newMenu=wxDynamicCast(Element::GetMenuItem(&menu,extract_or_throw<int>(menutuple[1]),extract_wxstring(menutuple[0])),wxUiMenuItem);
				if(menuListSize==4) //Ajout de paramètres utilisateurs
				{

				}
				menu.Append(newMenu);
			}
		}
	}
}

/**
 * Traduit un menu en une imbrication de liste python
 */

boost::python::list GetMenuItems(wxMenu* menuToFill)
{
	boost::python::list retLst;
	const wxMenuItemList& menuitems=menuToFill->GetMenuItems();

	wxwxMenuItemListNode* curNode=menuitems.GetFirst();

	while(curNode)
	{
		wxMenuItem* curItem=dynamic_cast<wxMenuItem*>(curNode->GetData());
		if(curItem)
		{
			if(curItem->IsSubMenu())
			{
				retLst.append(boost::python::make_tuple(WXSTRINGTOSTDWSTRING(curItem->GetItemLabel()),GetMenuItems(curItem->GetSubMenu())));
			}else{
				if(curItem->IsEnabled())
				{
					if(!curItem->IsSeparator())
					{
						wxUiMenuItem* curUiItem=dynamic_cast<wxUiMenuItem*>(curItem);
						if(curUiItem)
						{
							retLst.append(boost::python::make_tuple(WXSTRINGTOSTDWSTRING(curUiItem->GetItemLabel()),curUiItem->GetId(),WXSTRINGTOSTDWSTRING(curUiItem->GetBitmapPath())));
						}else{
							wxLogDebug("Unable to translate menu with wxUiMenuItem class");
						}
					}else{
						retLst.append(boost::python::make_tuple());
					}
				}
			}
		}else{
			wxLogDebug("Unable to translate menu with wxMenuItem class");
		}
		curNode=curNode->GetNext();
	}
	return retLst;
}



/**
 * Definition des paramètres par défaut
 * @ref http://www.boost.org/doc/libs/1_37_0/libs/python/doc/v2/overloads.html
 */
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertytext_overloads,element::appendpropertytext,3,5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertyposition_overloads,element::appendpropertyposition,3,4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertylist_overloads,element::appendpropertylist,4,7)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertycolor_overloads,element::appendpropertycolor,2,5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertyentier_overloads,element::appendpropertyentier,2,8)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertydecimal_overloads,element::appendpropertydecimal,2,10)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertybool_overloads,element::appendpropertybool,3,4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendpropertyfont_overloads,element::appendpropertyfont,2,3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(appendfilsbytype_overloads,element::appendfilsbytype,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(hide_overloads,element::hide,0,1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(setreadonlyconfig_overloads,element::setreadonlyconfig,1,3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(setreadonlyallconfig_overloads,element::setreadonlyallconfig,0,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(deleteelementbyxmlid_overloads,element::deleteelementbyxmlid,1,2)

void export_enum();
void export_project_class_pyfilelement();
void export_E_Python_Extension();
/**
 * @ref http://www.boost.org/doc/libs/1_37_0/libs/python/doc/tutorial/doc/html/python/techniques.html#python.reducing_compiling_time
 */


#ifndef REMTR
#define REMTR
#endif

void export_project_class()
{

  export_enum();
  class_<element>("element",init<int>())
	  .def(init<element>())
	  .def("childs", &element::childs,REMTR("Returns the indices of the elements contained in the object.\nEach element is defined by three values:\n - the first is the index of the element allowing to create the object\n - the second is its type\n - the third sound labeled\n \nThe index is unique for all elements of the project, however it is constant that on the execution instance of the software and is modified when loading the project file\nThe type corresponds to a set of similar element, this type is constant even for the next update of the software.\nThe label corresponds to the one visible in the tree of the project and is thus very variable. Only the elements of properties have a fixed name and can be used safely."))
	  .def("updatestringconfig", &element::updatestringconfig,REMTR("Update a data field"),args("propertyname","propertynewvalue"))
	  .def("updatedecimalconfig", &element::updatedecimalconfig,REMTR("Update a data field"),args("propertyname","propertynewvalue"))
	  .def("updatelistconfig", &element::updatelistconfig,REMTR("Update a data field"),args("propertyname","propertynewindex"))
	  .def("updateentierconfig", &element::updateentierconfig,REMTR("Update a data field"),args("propertyname","propertynewvalue"))
	  .def("updateboolconfig", &element::updateboolconfig,REMTR("Update a data field"),args("propertyname","propertynewvalue"))
	  .def("updatepositionconfig", &element::updatepositionconfig,REMTR("Update a data field"),args("propertyname","propertynewvalue"))
	  .def("getstringconfig", &element::getstringconfig,REMTR("Returns the value of a data field"),args("propertyname"))
	  .def("getpositionconfig", &element::getpositionconfig,REMTR("Returns the value of a data field"),args("propertyname"))
	  .def("getdecimalconfig", &element::getdecimalconfig,REMTR("Returns the value of a data field"),args("propertyname"))
      .def("getboolconfig", &element::getboolconfig,REMTR("Returns the value of a data field"),args("propertyname"))
	  .def("getentierconfig", &element::getentierconfig,REMTR("Returns the value of a data field"),args("propertyname"))
      .def("getcolorconfig", &element::getcolorconfig,REMTR("Returns the value of a data field"),args("propertyname"))
      .def("getlistconfig", &element::getlistconfig,REMTR("Returns the value of a data field"),args("propertyname"))
	  .def("appendpropertytext", &element::appendpropertytext, appendpropertytext_overloads(REMTR("Adds a text field in the element."),args("propertyname","propertylabel","propertydefaultvalue","readonly","exporttocore")))
  	  .def("appendpropertyposition", &element::appendpropertyposition, appendpropertyposition_overloads(REMTR("Adds a data field to the element."),args("propertyname","propertylabel","propertydefaultvalue","exporttocore")))
   	  .def("appendpropertylist", &element::appendpropertylist, appendpropertylist_overloads(REMTR("Adds a data field to the element."),args("propertyName","propertyLabel","values","defaultValue","asTitle","hSize","exportToCore")))
	  .def("appendpropertycolor", &element::appendpropertycolor, appendpropertycolor_overloads(REMTR("Adds a data field to the element."), args("propertyname","propertylabel","defaultred","defaultgreen","defaultblue")))
	  .def("appendpropertyentier", &element::appendpropertyentier, appendpropertyentier_overloads(REMTR("Adds a data field to the element."), args("propertyname","propertylabel","propertydefaultvalue","exporttocore","ismaxvalue","isminvalue","maxvalue","minvalue")))
	  .def ("appendpropertydecimal", &element::appendpropertydecimal, appendpropertydecimal_overloads(REMTR("Adds a data field to the element."), args("propertyname","propertylabel","propertydefaultvalue","readonly","precision","ismaxvalue","isminvalue","maxvalue","minvalue","exporttocore")))
	  .def ("appendpropertybool", &element::appendpropertybool, appendpropertybool_overloads(REMTR("Adds a data field to the element."), args("propertyname","propertylabel","propertydefaultvalue","exporttocore")))
	  .def ("appendpropertyfont", &element::appendpropertyfont, appendpropertyfont_overloads(REMTR("Adds a data field to the element."), args("propertyname","propertylabel","propertydefaultvalue")))
	  .def("getmenu", &element::getmenu,REMTR("Returns the possible actions to perform on the element."))
	  .def("getallelementbytype", &element::getallelementbytype,REMTR("Returns all the elements having as type that passed in parameter."),args("elementtype"))
	  .def("getelementbytype", &element::getelementbytype,REMTR("Returns the first element encountered of this type in child elements and recursively"),args("elementtype"))
	  .def("getelementbylibelle", &element::getelementbylibelle,REMTR("Returns the first element encountered with this label.For the property elements this will be the name and not the label."),args("elementlabel"))
	  .def ("getinfos", &element::getinfos,REMTR("Returns the information of this element such as its label, type, etc.."))
	  .def ("appenduserelement", &element::appenduserelement,REMTR("Adds an element to the project from a source external to the program\n This external class must inherit from uictrl::element and be in the folder UserScript / @return The instance of the python class"), args("modulename","classname"))
	  .def ("appendfilsbytype", &element::appendfilsbytype, appendfilsbytype_overloads(REMTR("This function allows the addition of a child from its type. This function only works with compatible elements (having a parent / child link)\n @ return The index of the element added or -1 if no element added."), args("element_type","element_label")))
	  .def ("modified", &element::modified,REMTR("Mark the item as updated and will be saved in the xml file."))
	  .def ("hasproperty", &element::hasproperty,REMTR("Returns true if this element has a property with this name"))
      .def ("hide", &element::hide, hide_overloads("Hide an element",args("visible=False")))
	  .def ("setreadonlyconfig", &element::setreadonlyconfig, setreadonlyconfig_overloads("Enable or Disable the user access to this data.",args("name","readOnly","col")))
	  .def ("setreadonlyallconfig", &element::setreadonlyallconfig, setreadonlyallconfig_overloads("Enables / Disables the ability to change all the fields to the user for a given column.",args("readOnly","col")))
	  .def ("deleteelementbyxmlid", &element::deleteelementbyxmlid, deleteelementbyxmlid_overloads("Find and delete the son with this id. This is not a recursively search",args("xmlIdElement","setModification")))
	  .def("deleteallelementbytype",&element::deleteallelementbytype,REMTR("Removes all children from this element at this level with this type of element."),args("elementType"))
	  .def("deleteallelementbytyper",&element::deleteallelementbytyper,REMTR("Deletes recursively all descendants with this data type"),args("elementType"))
	  .def("register_update_manager",&element::register_update_manager,REMTR("english\n Link a python function to the event of element update.You can add multiple links. Links are alive until project close.\n param func Python class with a function called OnUpdate with one parameter, the updated element index, may be this element or a child."),args("class"))
	  .def("getid", &element::getid,REMTR("Return the element id."))
	  .def("register_appendchild_manager",&element::register_appendchild_manager,REMTR("Link a python function to the event of element append a new element child.You can add multiple links. Links are alive until project close."))
	  ;
  export_project_class_pyfilelement();
  registerexcept();

}


Element* GetElement(const wxInt32& xmlId)
{
	void *obj;
	if(InstanceManager::GetReference(OBJECT_REF_TYPE_ELEMENT,xmlId,&obj))
	{
		return (Element*)obj;
	}else{
		ThrowPyException(WXSTRINGTOCHARPTR(_("This element doesn't exist")));
		return NULL;
	}
}

namespace uictrl
{
element::element(const wxInt32& _xmlId)
{
	//On verifie si cet élément existe
	GetElement(_xmlId);
	xmlId=_xmlId;
}

wxInt32 element::getindice() const
{
	return xmlId;
}

element::element(const element& cpyFrom)
{
	xmlId=cpyFrom.getindice();
}

element::~element()
{

}

boost::python::dict element::getinfos()
{
	dict retval;
	Element* self=GetElement(xmlId);
	Element::t_elementInfo infoEl=self->GetElementInfos();

	retval["typeElement"]=infoEl.typeElement;

	retval["xmlIdElement"]=infoEl.xmlIdElement;
	retval["expanded"]=infoEl.expanded;
	retval["userDestroyable"]=infoEl.userDestroyable;
	retval["label"]=WXSTRINGTOSTDWSTRING(self->GetTreeLabel());
	retval["label_located"]=WXSTRINGTOSTDWSTRING(_(self->GetTreeLabel()));
	retval["name"]=WXSTRINGTOSTDWSTRING(infoEl.libelleElement);
	Element* elparent=self->GetElementParent();
	if(elparent)
		retval["parentid"]=elparent->GetXmlId();
	else
		retval["parentid"]=-1;
	return retval;
}
boost::python::list element::childs()
{
	list retObj;
	Element* self=GetElement(xmlId);
	std::list<Element*> childList;
	self->GetChildElement(childList);
	for(std::list<Element*>::iterator it=childList.begin();it!=childList.end();it++)
	{
		list elObj;
		const Element::t_elementInfo& elinfo=(*it)->GetElementInfos();
		elObj.append(elinfo.xmlIdElement);
		elObj.append(elinfo.typeElement);
		elObj.append(WXSTRINGTOSTDWSTRING(elinfo.libelleElement));
		retObj.append(elObj);
	}
	return retObj;
}
void element::setreadonlyconfig(const std::wstring& name,bool readOnly,int col)
{
	Element* self=GetElement(xmlId);
	self->SetReadOnlyConfig(name,readOnly,col);
}
void element::setreadonlyallconfig(bool readOnly,int col)
{
	Element* self=GetElement(xmlId);
	self->SetReadOnlyAllConfig(readOnly,col);
}
bool element::updatestringconfig(const std::wstring& name,const std::wstring& newValue)
{
	Element* self=GetElement(xmlId);
	return self->UpdateStringConfig(name,newValue);
}

bool element::updatedecimalconfig(const std::wstring& name,float newValue)
{
	Element* self=GetElement(xmlId);
	return self->UpdateDecimalConfig(name,newValue);
}

bool element::updatelistconfig(const std::wstring& name,int newIndex)
{
	Element* self=GetElement(xmlId);
	return self->UpdateListConfig(name,newIndex);
}
bool element::updateentierconfig(const std::wstring& name,int newValue)
{
	Element* self=GetElement(xmlId);
	return self->UpdateEntierConfig(name,newValue);
}
bool element::updateboolconfig(const std::wstring& name,bool newValue)
{
	Element* self=GetElement(xmlId);
	return self->UpdateBoolConfig(name,newValue);
}

bool element::updatepositionconfig(const std::wstring& name,const boost::python::list& newValue)
{
	Element* self=GetElement(xmlId);
	vec3 newpos(extract_or_throw<float>(newValue[0]),extract_or_throw<float>(newValue[1]),extract_or_throw<float>(newValue[2]));
	Element* posel=self->GetElementByLibelle(name);
	if(posel)
	{
		if(!posel->UpdateDecimalConfig("x",newpos.x))
			return false;
		if(!posel->UpdateDecimalConfig("y",newpos.y))
			return false;
		if(!posel->UpdateDecimalConfig("z",newpos.z))
			return false;
		return true;
	}else{
		return false;
	}
}
int element::appendpropertytext(const std::wstring& propertyName,const std::wstring& propertyLabel,const std::wstring& propertyDefaultValue,bool readOnly,bool exportToCore)
{
	Element* self=GetElement(xmlId);
	Element* newobj=self->AppendPropertyText(propertyName,propertyLabel,propertyDefaultValue,readOnly,exportToCore);
	if(newobj)
	{
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}
boost::python::list element::getmenu()
{
	Element* self=GetElement(xmlId);
	boost::python::list retLst;
	wxMenu menuToFill;
	self->OnRightClic(&menuToFill);
	return GetMenuItems(&menuToFill);
}

int element::appendpropertyposition(const std::wstring& propertyName,const std::wstring& propertyLabel,const boost::python::list& propertyDefaultValue,bool exportToCore)
{
	Element* self=GetElement(xmlId);
	Element* newobj=self->AppendPropertyPosition(propertyName,propertyLabel,vec3(extract_or_throw<float>(propertyDefaultValue[0]),extract_or_throw<float>(propertyDefaultValue[1]),extract_or_throw<float>(propertyDefaultValue[2])),exportToCore);
	if(newobj)
	{
		self->FillWxTree();
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}
int element::appendpropertylist(const std::wstring& propertyName,const std::wstring& propertyLabel,const boost::python::list& values,long defaultValue, bool asTitle, int hSize,bool exportToCore)
{
	Element* self=GetElement(xmlId);
	/*
	[['Premier element',
	'Deuxième element',
	'Troisième element'],[0,1,2]]

	*/
	std::vector<wxString> wxvalues;
	std::vector<int> wxindex;

	int size=GetPythonArraySize(values);
	if(size==0)
		return -1;

	if(size<=2)
	{
		std::vector<std::wstring> tmpvalues;
		if(!extract_array<std::wstring>(values[0],&tmpvalues))
		{
			ThrowPyException(_("Uncorrect data list"));
		}
		for(int id=0;id<tmpvalues.size();id++)
		{
			wxvalues.push_back(tmpvalues[id]);
		}
		if(size==2)
		{
			if(!extract_array<int>(values[1],&wxindex))
			{
				ThrowPyException(_("Uncorrect index list"));
			}
		}
	}else{
		return -1;
	}


	Element* newobj=self->AppendPropertyList(propertyName,propertyLabel,wxvalues,defaultValue,asTitle,hSize,wxindex,exportToCore);
	if(newobj)
	{
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}
int element::appendpropertycolor(const std::wstring& propertyName,const std::wstring& propertyLabel,long defaultRed,long defaultGreen,long defaultBlue)
{
	Element* self=GetElement(xmlId);
	Element* newobj=self->AppendPropertyColor(propertyName,propertyLabel,defaultRed,defaultGreen,defaultBlue);
	if(newobj)
	{
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}
int element::appendpropertyentier(const std::wstring& propertyName,const std::wstring& propertyLabel,int propertyDefaultValue,bool exportToCore,bool isMaxValue,bool isMinValue,int maxValue,int minValue)
{
	Element* self=GetElement(xmlId);
	Element* newobj=self->AppendPropertyInteger(propertyName,propertyLabel,propertyDefaultValue,exportToCore,isMaxValue,isMinValue,maxValue,minValue);
	if(newobj)
	{
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}
int element::appendpropertydecimal(const std::wstring& propertyName,const std::wstring& propertyLabel,float propertyDefaultValue,bool readOnly,int precision,bool isMaxValue,bool isMinValue,float maxValue,float minValue,bool exportToCore)
{
	Element* self=GetElement(xmlId);
	Element* newobj=self->AppendPropertyDecimal(propertyName,propertyLabel,propertyDefaultValue,readOnly,precision,isMaxValue,isMinValue,maxValue,minValue,exportToCore);
	if(newobj)
	{
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}
int element::appendpropertybool(const std::wstring& propertyName,const std::wstring& propertyLabel,bool propertyDefaultValue,bool exportToCore)
{
	Element* self=GetElement(xmlId);
	Element* newobj=self->AppendPropertyBool(propertyName,propertyLabel,propertyDefaultValue,exportToCore);
	if(newobj)
	{
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}


int element::appendpropertyfont(const std::wstring& propertyName,const std::wstring& propertyLabel,const std::wstring& propertyDefaultValue)
{
	Element* self=GetElement(xmlId);
	Element* newobj=self->AppendPropertyFont(propertyName,propertyLabel,wxFont(propertyDefaultValue));
	if(newobj)
	{
		return newobj->GetXmlId();
	}else{
		return -1;
	}
}

bool element::hasproperty(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	return self->IsPropertyExist(name);
}
std::wstring element::getstringconfig(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	return WXSTRINGTOSTDWSTRING(self->GetStringConfig(name));
}

boost::python::list element::getpositionconfig(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	vec3 pos=self->GetPositionConfig(name);
	boost::python::list retLst;
	retLst.append(pos.x);
	retLst.append(pos.y);
	retLst.append(pos.z);
	return retLst;
}
float element::getdecimalconfig(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	return self->GetDecimalConfig(name);
}
bool element::getboolconfig(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	return self->GetBoolConfig(name);
}
int element::getentierconfig(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	return self->GetIntegerConfig(name);
}
boost::python::list element::getcolorconfig(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	vec3 pos=self->GetColorConfig(name);
	boost::python::list retLst;
	retLst.append(pos.r);
	retLst.append(pos.v);
	retLst.append(pos.b);
	return retLst;
}
int element::getlistconfig(const std::wstring& name)
{
	Element* self=GetElement(xmlId);
	return self->GetListConfig(name);
}

boost::python::list element::getallelementbytype(Element::ELEMENT_TYPE typeElement)
{
	Element* self=GetElement(xmlId);
	boost::python::list retLst;
	std::vector<Element*> elLst;
	self->GetAllElementByType(typeElement,elLst);
	for(wxInt32 i=0;i<elLst.size();i++)
	{
		retLst.append(elLst[i]->GetXmlId());
	}
	return retLst;
}
int element::getelementbytype(Element::ELEMENT_TYPE typeElement)
{
	Element* self=GetElement(xmlId);
	Element* elSearch=self->GetElementByType(typeElement);
	if(elSearch)
		return elSearch->GetXmlId();
	else
		return -1;
}
bool element::deleteelementbyxmlid(int xmlIdElement,bool setModification)
{
	Element* self=GetElement(xmlId);
	return self->DeleteElementByXmlId(xmlIdElement,setModification);
}

void element::deleteallelementbytype(Element::ELEMENT_TYPE typeElement)
{
	Element* self=GetElement(xmlId);
	self->DeleteAllElementByType(typeElement);
}
void element::deleteallelementbytyper(Element::ELEMENT_TYPE typeElementToDelete)
{
	Element* self=GetElement(xmlId);
	self->DeleteAllElementByType(typeElementToDelete);
}

void element::hide(bool visible)
{
	Element* self=GetElement(xmlId);
	self->Hide(visible);
}
int element::getelementbylibelle(std::wstring libelle)
{
	Element* self=GetElement(xmlId);
	Element* elSearch=self->GetElementByLibelle(libelle);
	if(elSearch)
		return elSearch->GetXmlId();
	else
		return -1;
}
boost::python::object element::appenduserelement(const Element::ELEMENT_TYPE& baseType, const std::wstring& moduleName,const std::wstring& className )
{
	boost::python::object pyobj;
	Element* self=GetElement(xmlId);
	Element* newel=New_E_Python_Extension( baseType,self,moduleName,className,pyobj);
	self->AppendFils(newel,true);
	return pyobj;
}

int element::appendfilsbytype(Element::ELEMENT_TYPE etypefils,const std::wstring& libelle)
{
	Element* self=GetElement(xmlId);
	Element* created=self->AppendFilsByType(etypefils,libelle);
	if(created)
		return created->GetXmlId();
	else
		return -1;
}

void element::modified(int elementUpdated)
{
	Element* self=GetElement(xmlId);
	Element* updated=GetElement(elementUpdated);
	self->Modified(updated);
}


/**
 * Cette classe fait le lien entre un objet Element et une fonction python. Lors-ce que l'élément est mis à jour le script est appelé
 */
struct LinkBetweenElementUpdateEventAndPythonScript : public t_baseEvtCatcher
{
	LinkBetweenElementUpdateEventAndPythonScript(boost::python::object _updateFunction)
		:updateFunction(_updateFunction)
	{

	}
	virtual void OnModified(const int& elid)
	{
		try
		{
			updateFunction(elid);
		} catch( error_already_set ) {
			wxLogError(_("Error during call of Python(TM) script on element update"));
		}

	}
	boost::python::object updateFunction;
};
/**
 * Cette classe fait le lien entre un objet Element et une fonction python. Lors-ce que l'élément ajoute un nouveau fils alors le script est appelé
 */
struct LinkBetweenElementAppendChildEventAndPythonScript : public t_baseEvtCatcher
{
	LinkBetweenElementAppendChildEventAndPythonScript(boost::python::object _appendFunction)
		:appendFunction(_appendFunction)
	{

	}
	virtual void OnNewChildElement(const int& childType,const int& parentid,const int& childid)
	{
		try
		{
			appendFunction(childType,parentid,childid);
		} catch( error_already_set ) {
			wxLogError(_("Error during call of Python(TM) script on element append child"));
		}
	}
	boost::python::object appendFunction;
};
void element::register_update_manager(boost::python::object& pymethod)
{
	Element* self=GetElement(xmlId);
	EventCatcher evtptr(new LinkBetweenElementUpdateEventAndPythonScript(pymethod));
	self->AppendEventCatcher(evtptr);
}
void element::register_appendchild_manager(boost::python::object& pymethod)
{
	Element* self=GetElement(xmlId);
	EventCatcher evtptr(new LinkBetweenElementAppendChildEventAndPythonScript(pymethod));
	self->AppendEventCatcher(evtptr);
}


}

#endif
