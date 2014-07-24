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

#include "data_manager/element.h"
#include "data_manager/tree_rapport/e_report_folder.h"

/** \file e_report.h
   \brief Element représentant la racine des rapports de calcul d'un projet
*/

/**
   \brief Element représentant la racine des rapports de calcul d'un projet
*/

class E_Report: public Element
{
private:
void (*pointeurFonctionEvent)(Element*);
bool eventBinded;
public:
	E_Report( wxXmlNode* noeudCourant )
		:Element(NULL,"Résultats",Element::ELEMENT_TYPE_RESULT_ROOT ,noeudCourant)
	{
		eventBinded=false;
		_("Results");
		if(noeudCourant!=NULL)
		{
			wxXmlNode* currentChild;
			currentChild = noeudCourant->GetChildren();
			// On va créer les fils de notre noeudCourant
			wxString propValue;
			while(currentChild!=NULL)
			{
				if(currentChild->GetPropVal("eid",&propValue))
				{
					long typeEle;
					propValue.ToLong(&typeEle);
					if(typeEle==ELEMENT_TYPE_REPORT_FOLDER)
					{

						//maj version 1.0.3
						if(currentChild->GetPropVal("filepath",&propValue) && propValue==wxString("current")+wxFileName::GetPathSeparator()+wxString("report")+wxFileName::GetPathSeparator())
						{
							currentChild->DeleteProperty("filepath");
							currentChild->AddProperty("filepath",ApplicationConfiguration::CONST_REPORT_FOLDER_PATH);
						}
						//fin maj version < 1.0.3
						this->AppendFils(new E_Report_Folder(this,currentChild));
					}
				}
				currentChild = currentChild->GetNext();
			}
		}
	}

	E_Report( )
		:Element(NULL,"Résultats",Element::ELEMENT_TYPE_RESULT_ROOT)
	{
		eventBinded=false;
		wxString ReportFolder=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_REPORT_FOLDER_PATH;
		if(!wxFileExists(ReportFolder))
			wxMkdir(ReportFolder);
		this->AppendFils(new E_Report_Folder(this,"Résultats",ApplicationConfiguration::CONST_REPORT_FOLDER_PATH));
	}
	/**
		Initialise le pointeur de méthode vers le gestionnaire de projet
		@param _pointeurFonctionEvent Méthode d'événement de mise à jour
		@see OnElementRootUpdate()
	*/
	void BindUpdateEvent(void (*_pointeurFonctionEvent)(Element*))
	{
		pointeurFonctionEvent=_pointeurFonctionEvent;
		eventBinded=true;
	}
	/**
	 * Actualise la liste des rapports de résultat
	 */
	void RefreshReportFolder()
	{
		if(!this->fils.empty())
		{
			Element* eleBase=this->GetElementByType(Element::ELEMENT_TYPE_REPORT_FOLDER);
			if(eleBase)
			{
				E_Report_Folder* reportEle=dynamic_cast<E_Report_Folder*>(eleBase);
				if(reportEle)
				{
					reportEle->RefreshFolderContents();
					reportEle->FillWxTree(treeCtrl,this->elementInfo.idElement);
				}
			}
		}
	}


	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
		thisNode->SetName("Report"); // Nom de la balise xml ( pas d'espace autorise )
		return thisNode;
	}
protected:

	void Modified(Element* eModif)
	{
		Element::Modified(eModif);
		if(eventBinded)
			(*pointeurFonctionEvent)(eModif);
	}
};
