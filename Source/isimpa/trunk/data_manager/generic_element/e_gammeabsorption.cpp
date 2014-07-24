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

#include "e_gammeabsorption.h"
#include "data_manager/e_data_row.h"
#include "manager/stringTools.h"
#include "data_manager/appconfig.h"
#include "last_cpp_include.hpp"
#include "data_manager/e_data_list.h"

E_GammeAbsorption::E_GammeAbsorption(wxXmlNode* nodeElement,Element* parent)
	:Element(parent,"Paramètres acoustiques",ELEMENT_TYPE_GAMMEABSORPTION,nodeElement)
{
	SetIcon(GRAPH_STATE_ALL,GRAPH_SPECTRUM);
	ignoreNextUpdate=false;
	//Mise à jour version 1.1.9
	//Ajout de deux nouvelles lois de diffusion
	

	bool somethingUpdated(false);
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		
		t_elementInfo infoEl=(*itfils)->GetElementInfos();
		if(infoEl.libelleElement!="moyenne" && infoEl.typeElement==ELEMENT_TYPE_ROW)
		{
			Element* listel=(*itfils)->GetElementByLibelle("loi_diff");
			if(listel)
			{
				E_Data_List* datalst=dynamic_cast<E_Data_List*>(listel);
				if(datalst->GetItemsLabel().size()==1)
				{
					datalst->AppendItem("Collision uniforme",1);
					datalst->AppendItem("Collision Lambert",2);
					somethingUpdated=true;
				}else{
					break;
				}
			}
		}
	}
	if(somethingUpdated)
	{
		this->ForceBottomModified();
	}
}

E_GammeAbsorption::E_GammeAbsorption(Element* parent)
:Element(parent,"Paramètres acoustiques",ELEMENT_TYPE_GAMMEABSORPTION,NULL)
{
	SetIcon(GRAPH_STATE_ALL,GRAPH_SPECTRUM);
	_("Acoustic parameters");
	ignoreNextUpdate=true;
	InitProperties();
	ignoreNextUpdate=false;
}


wxXmlNode* E_GammeAbsorption::SaveXMLCoreDoc(wxXmlNode* NoeudParent)
{

	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		t_elementInfo infoEl=(*itfils)->GetElementInfos();
		if(infoEl.libelleElement!="moyenne" && infoEl.typeElement==ELEMENT_TYPE_ROW)
		{
			wxXmlNode* NoeudCourant=new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"bfreq");
			NoeudCourant->AddProperty("freq",infoEl.libelleElement);
			(*itfils)->SaveXMLCoreDoc(NoeudCourant);
		}
	}
	return NoeudParent;
}

void E_GammeAbsorption::InitProperties()
{
	std::vector<wxString> Lstlois;
	Lstlois.push_back("Uniforme");
	//Lstlois.push_back("Collision uniforme");
	//Lstlois.push_back("Collision Lambert");
	_("Uniform");
	//_("Collision uniforme");
	//_("Collision Lambert");
	std::vector<int> LstloisIndex(Lstlois.size());

	for(int i=0;i<LstloisIndex.size();i++)
		LstloisIndex[i]=i;
	const std::vector<ApplicationConfiguration::t_freq> tabfreq=ApplicationConfiguration::GetAllFrequencies();

	int precisionDecimal=2;
	//Ajout des bandes
	for(int i=0;i<tabfreq.size();i++)
	{
		wxString name;
		wxString label;
		label<<tabfreq[i]<<" Hz";
		name<<tabfreq[i];
		E_Data_Row* nouvLigne=new E_Data_Row(this,name,label);
		nouvLigne->SetXmlCoreVisibility(true); //Exporter
		nouvLigne->AppendPropertyDecimal("alpha","Alpha",0.f,false,precisionDecimal,true,true,1.f,0.f,true);
		nouvLigne->AppendPropertyDecimal("lambda","Lambda",1.f,false,precisionDecimal,false,true,0,.01f,true);
		nouvLigne->AppendPropertyList("loi_diff","Loi de diffusion",Lstlois,0,false,1,LstloisIndex,true);
		this->AppendFils(nouvLigne);
	}
	//Ajout du cumul
	int i=tabfreq.size();
	E_Data_Row* nouvLigne=new E_Data_Row(this,"moyenne","Moyenne");
	nouvLigne->SetXmlCoreVisibility(false); //Ne pas exporter
	nouvLigne->AppendPropertyDecimal("alpha","Alpha",0,false,precisionDecimal,true,true,1.f,0.f,true);
	nouvLigne->AppendPropertyDecimal("lambda","Lambda",1.f,false,precisionDecimal,false,true,0,.01f,true);
	nouvLigne->AppendPropertyText("l_diff","Loi de diffusion","",true);
	_("Average");
	_("Alpha");
	_("Lambda");
	_("Diffusion law");
	this->AppendFils(nouvLigne);
}

wxXmlNode* E_GammeAbsorption::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
	thisNode->SetName("absorption"); // Nom de la balise xml ( pas d'espace autorise )
	return thisNode;
}
void E_GammeAbsorption::CopyAverageToEachRow(wxString typeS)
{
	float fMoyenne=0;
	Element* moy=this->GetElementByLibelle("moyenne");
	if(moy!=NULL)
		fMoyenne=moy->GetDecimalConfig(typeS);
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const t_elementInfo& infoFils=(*itfils)->GetElementInfos();
		if(infoFils.libelleElement!="moyenne" && infoFils.typeElement==ELEMENT_TYPE_ROW)
		{
			(*itfils)->UpdateDecimalConfig(typeS,fMoyenne);
		}
	}
}

void E_GammeAbsorption::CalcNiveauSonoreGlobalMoyen(wxString typeS)
{
	float fMoyenne=0;
	int nbEl=0;
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		t_elementInfo infoFils=(*itfils)->GetElementInfos();
		if(infoFils.libelleElement!="moyenne" && infoFils.typeElement==ELEMENT_TYPE_ROW)
		{
			fMoyenne+=(*itfils)->GetDecimalConfig(typeS);
			nbEl++;
		}
	}
	if(nbEl>0)
		fMoyenne=fMoyenne/nbEl;
	Element* moy=this->GetElementByLibelle("moyenne");
	if(moy!=NULL)
		moy->UpdateDecimalConfig(typeS,fMoyenne);
}

void E_GammeAbsorption::Modified(Element* eModif)
{
	Element* pereEModif=eModif->GetElementParent();
	t_elementInfo elInfo=eModif->GetElementInfos();
	if(pereEModif!=NULL)
	{
		t_elementInfo pereInfo=pereEModif->GetElementInfos();
		if(!ignoreNextUpdate)
		{
			if(pereInfo.libelleElement!="moyenne")
			{
				if(elInfo.libelleElement=="alpha")
				{
					ignoreNextUpdate=true;
					CalcNiveauSonoreGlobalMoyen("alpha");
					ignoreNextUpdate=false;
				}
				else if(elInfo.libelleElement=="lambda")
				{
					ignoreNextUpdate=true;
					CalcNiveauSonoreGlobalMoyen("lambda");
					ignoreNextUpdate=false;
				}
			}else{
				if(elInfo.libelleElement=="alpha")
				{
					ignoreNextUpdate=true;
					CopyAverageToEachRow("alpha");
					ignoreNextUpdate=false;
				}
				else if(elInfo.libelleElement=="lambda")
				{
					ignoreNextUpdate=true;
					CopyAverageToEachRow("lambda");
					ignoreNextUpdate=false;
				}
			}
			Element::Modified(eModif); //Element de base modifié
		}
	}
}
