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

#include "data_manager/python_interface/boost_python_header.h"
#include "data_manager/element.h"
#include <wx/icon.h>
#include <wx/imaglist.h>
#include <wx/filename.h>
#include <wx/log.h>

/**
 * @file e_pyext.hpp
 * Implémentation de E_Python_Extension
 */

#ifndef __ELEMENT_PYTHON_EXT__
#define __ELEMENT_PYTHON_EXT__

#ifdef USE_PYTHON
namespace bp=boost::python;

//Implémenté dans projet_python.cpp
bool ins_pyelement(boost::python::object& py_el,const wxInt32& wxid, const wxString& module,const wxString& _pyclass);
void GetListItems(boost::python::list& pymenu, wxMenu& menu);
/**
 * Permet de revenir de remettre a faux le booléen même en cas de crash de l'evenement
 */
class callFromSelflocker
{
public:
	callFromSelflocker(bool* _marker)
		:marker(_marker)
	{
		*marker=true;
	}
	~callFromSelflocker()
	{
		*marker=false;
	}
private:
	bool* marker;
};
#define _BEGIN_CONST_PY_EXT_CALL_WORKING_AROUND \
	try\
	{
#define _BEGIN_PY_EXT_CALL_WORKING_AROUND \
	try\
	{\
		callFromSelflocker fixself(&callFromSelf);
#define _END_PY_EXT_CALL_WORKING_AROUND }\
	catch( bp::error_already_set )\
	{\
		wxLogError(_("There are errors in the Python(TM) script!"));\
	}


/**
 * @class E_Python_Extension
 * Elément tiers implémenté à l'extérieur du logiciel
 */
template <class ElementBase>
class E_Python_Extension : public ElementBase
{
protected:
	bp::object pyelement;
	wxString pymodule;	//nom du module de pyelement
	wxString pyclass;	//nom de la classe de pyelement
	bool callFromSelf;	//A vrai si c'est pyelement qui nous appel
	Element::ELEMENT_TYPE baseType;
public:


	E_Python_Extension(Element* parent,const Element::ELEMENT_TYPE& _baseType,const wxString& module,const wxString _pyclass="_pyel",wxXmlNode* noeudCourant=NULL)
	: ElementBase(parent,_pyclass,Element::ELEMENT_TYPE_PYTHON_EXTENSION,noeudCourant)
	{
		baseType=_baseType;
		callFromSelf=false;
		if(!noeudCourant)
		{
			pymodule=module;
			pyclass=_pyclass;
		}else{
			noeudCourant->GetAttribute("m",&pymodule);
			noeudCourant->GetAttribute("c",&pyclass);
		}
		if(!ins_pyelement(this->pyelement,this->elementInfo.xmlIdElement,pymodule,pyclass))
			this->elementInfo.userDestroyable=true;
	}

	Element::ELEMENT_TYPE GetBaseElementId()
	{
		return baseType;
	}
	virtual wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = ElementBase::SaveXMLDoc(NoeudParent);
		thisNode->SetName(this->pyclass);
		if(!thisNode->HasAttribute("m"))
			thisNode->AddAttribute("m",this->pymodule);
		if(!thisNode->HasAttribute("c"))
			thisNode->AddAttribute("c",this->pyclass);
		if(!thisNode->HasAttribute("beid"))
			thisNode->AddAttribute("beid",Convertor::ToString((int)baseType));
		return thisNode;
	}

	virtual void OnRightClic(wxMenu* leMenu)
	{

		if(hasattr(this->pyelement,"getmenu") && !callFromSelf)
		{
			boost::python::list pymenu;
			_BEGIN_PY_EXT_CALL_WORKING_AROUND
			pymenu=extract_or_throw<boost::python::list>(this->pyelement.attr("getmenu")());
			_END_PY_EXT_CALL_WORKING_AROUND
			GetListItems(pymenu, *leMenu);
			return;
		}
		ElementBase::OnRightClic(leMenu);
	}
	virtual wxString GetTreeLabel() const
	{
		if(hasattr(this->pyelement,"gettreelabel") && !callFromSelf)
		{
			_BEGIN_CONST_PY_EXT_CALL_WORKING_AROUND
			return extract_wxstring(this->pyelement.attr("gettreelabel")());
			_END_PY_EXT_CALL_WORKING_AROUND
		}
		return ElementBase::GetTreeLabel();
	}

	virtual void DrawItem( vec4 unitizeValue )
	{
		if(hasattr(this->pyelement,"drawitem") && !callFromSelf)
		{
			_BEGIN_PY_EXT_CALL_WORKING_AROUND
				this->pyelement.attr("drawitem")(bp::make_tuple(unitizeValue.x,unitizeValue.y,unitizeValue.z,unitizeValue.w));
			_END_PY_EXT_CALL_WORKING_AROUND
		}
	}
	virtual void FillWxTree(wxTreeCtrl* treeToFeed,const wxTreeItemId& parentId)
	{
		if(treeToFeed)
		{

			if(hasattr(this->pyelement,"geticonpath") && !callFromSelf)
			{
				wxString iconPath;
				wxString iconPathOpened;
				_BEGIN_PY_EXT_CALL_WORKING_AROUND
				iconPath=extract_wxstring(this->pyelement.attr("geticonpath")(false));
				_END_PY_EXT_CALL_WORKING_AROUND
				if(iconPath!="" && wxFileExists(iconPath))
				{
					int width,height;
					treeToFeed->GetImageList()->GetSize(0,width,height);
					wxFileName iconFileName(iconPath);
					if(iconFileName.FileExists())
					{
						wxIcon appIcon(iconFileName.GetFullPath(),wxBITMAP_TYPE_ANY,width,height);
						if(appIcon.Ok())
							this->elementInfo.graphElement=treeToFeed->GetImageList()->Add(appIcon);
						else
							wxLogWarning(_("The icon format of %s is not supported."),iconPath);
					}else{
						wxLogWarning(_("The icon %s doesn't exist!"),iconPath);
					}
				}
				_BEGIN_PY_EXT_CALL_WORKING_AROUND
				iconPathOpened=extract_wxstring(this->pyelement.attr("geticonpath")(true));
				_END_PY_EXT_CALL_WORKING_AROUND
				if(iconPathOpened!="" && iconPathOpened!=iconPath)
				{
					int width,height;
					treeToFeed->GetImageList()->GetSize(0,width,height);
					wxFileName iconFileName(iconPath);
					if(iconFileName.FileExists())
					{
						wxIcon appIcon(iconFileName.GetFullPath(),wxBITMAP_TYPE_ANY,width,height);
						if(appIcon.Ok())
							this->elementInfo.graphElementStateOpen=treeToFeed->GetImageList()->Add(appIcon);
						else
							wxLogWarning(_("The icon format of %s is not supported."),iconPath);
					}else{
						wxLogWarning(_("The icon %s doesn't exist!"),iconPath);
					}
				}else{
					this->elementInfo.graphElementStateOpen=this->elementInfo.graphElement;
				}

			}
			if(hasattr(this->pyelement,"geticonid") && !callFromSelf){
				int idgraph=-1;
				_BEGIN_PY_EXT_CALL_WORKING_AROUND
				idgraph=extract_or_throw<int>(this->pyelement.attr("geticonid")(false));
				_END_PY_EXT_CALL_WORKING_AROUND
				if(idgraph>=0)
					this->elementInfo.graphElement=idgraph;
				_BEGIN_PY_EXT_CALL_WORKING_AROUND
				idgraph=extract_or_throw<int>(this->pyelement.attr("geticonid")(true));
				_END_PY_EXT_CALL_WORKING_AROUND
				if(idgraph>=0)
					this->elementInfo.graphElementStateOpen=idgraph;
			}
		}
		ElementBase::FillWxTree(treeToFeed,parentId);
	}

	virtual wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(hasattr(this->pyelement,"getxmlnodename") && !callFromSelf)
		{
			wxString nodeName;
			_BEGIN_PY_EXT_CALL_WORKING_AROUND
			nodeName=extract_wxstring(this->pyelement.attr("getxmlnodename")());
			_END_PY_EXT_CALL_WORKING_AROUND
			if(!nodeName.empty())
				return ElementBase::SaveXMLCoreDoc(new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,nodeName));
		}
		return ElementBase::SaveXMLCoreDoc(NoeudParent);
	}
	/*
	 *
	 * @return L'élément python
	 */
	bp::object GetPyObj()
	{
		return this->pyelement;
	}

	virtual void Modified(Element* elementUpdated)
	{
		if(hasattr(this->pyelement,"modified") && !callFromSelf)
		{
			_BEGIN_PY_EXT_CALL_WORKING_AROUND
			this->pyelement.attr("modified")(elementUpdated->GetXmlId());
			_END_PY_EXT_CALL_WORKING_AROUND
		}else{
			ElementBase::Modified(elementUpdated);
		}
	}
};


#endif
#endif
