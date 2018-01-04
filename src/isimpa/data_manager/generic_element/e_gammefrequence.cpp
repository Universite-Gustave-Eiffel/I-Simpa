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

#include "e_gammefrequence.h"
#include "data_manager/appconfig.h"
#include "data_manager/e_data_row_bandefreq.h"
#include "last_cpp_include.hpp"

E_GammeFrequence::E_GammeFrequence(Element* parent,wxString Nom,ELEMENT_TYPE _type,wxXmlNode* nodeElement)
	:Element(parent,Nom,_type,nodeElement)
{
	SetIcon(GRAPH_STATE_ALL,GRAPH_SPECTRUM);
	ignoreNextUpdate=false;
	idSpectre=-1;
	wxString propVal;
	//Affecte la valeur de l'element

	if(nodeElement!=NULL) // && nodeElement->GetAttribute("wxid",&propVal)
	{
		//Element initialisé AVEC Xml
		long lval;
		if(nodeElement->GetAttribute("idspectre",&propVal))
		{
			propVal.ToLong(&lval);
			if(ApplicationConfiguration::IsIdSpectrumExist(lval))
				idSpectre=ApplicationConfiguration::GetFreeSpectrumId();
			else
				idSpectre=lval;
		}
		wxString propValue;

		wxXmlNode* currentChild;
		currentChild = nodeElement->GetChildren();
		while(currentChild!=NULL)
		{
			if(currentChild->GetAttribute("eid",&propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				if(typeEle==Element::ELEMENT_TYPE_ROW_BFREQ)
				{
					this->AppendFils(new E_Data_Row_BandeFreq(currentChild,this));
				}				
			}
			currentChild = currentChild->GetNext();
		}
		Element* globalRow=this->GetElementByType(ELEMENT_TYPE_ROW);
		if(globalRow)
		{
			//Maj depuis version 1.1.6 en inférieur
			if(globalRow->IsPropertyExist("att"))
			{
				ignoreNextUpdate=true;
				globalRow->DeleteElementByXmlId(globalRow->GetElementByLibelle("att")->GetXmlId());
				globalRow->DeleteElementByXmlId(globalRow->GetElementByLibelle("lw")->GetXmlId());
				ignoreNextUpdate=false;
			}
		}
	}else{
		idSpectre=ApplicationConfiguration::GetFreeSpectrumId();
		Modified(this); //Ajout de la propriété en cours d'execution
	}
	if(idSpectre==-1)
		idSpectre=ApplicationConfiguration::GetFreeSpectrumId();
	ApplicationConfiguration::AddSpectrum(this);
}
E_GammeFrequence::~E_GammeFrequence()
{
	ApplicationConfiguration::DeleteSpectrum(this->elementInfo.xmlIdElement);
}

wxFloat32 E_GammeFrequence::GetGlobalLevel( const wxString& typeDb)
{
	Element* globalRow=this->GetElementByLibelle("cumul");
	if(globalRow)
	{
		//1ere étape, lire la valeur global précédente et calcul le delta
		return globalRow->GetDecimalConfig(typeDb);
	}else{
		return 0;
	}
}

void E_GammeFrequence::SetGlobalLevel( const wxString& typeDb, wxFloat32 lvl )
{
	Element* globalRow=this->GetElementByLibelle("cumul");
	if(globalRow)
	{
		//1ere étape, lire la valeur global précédente et calcul le delta
		float delta=lvl-globalRow->GetDecimalConfig(typeDb);
		//2eme étape mettre à jour chaque bande de fréquence
		ignoreNextUpdate=true;
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			const t_elementInfo& infoFils=(*itfils)->GetElementInfos();
			if(infoFils.typeElement==ELEMENT_TYPE_ROW_BFREQ)
			{
				float ansval=(*itfils)->GetDecimalConfig(typeDb);
				(*itfils)->UpdateDecimalConfig(typeDb,ansval+delta);
			}
		}
		globalRow->UpdateDecimalConfig(typeDb,lvl);
		if(typeDb=="db")
			CalcNiveauSonoreGlobal("dba");
		else
			CalcNiveauSonoreGlobal("db");

		ignoreNextUpdate=false;
	}
}

void E_GammeFrequence::InitProperties()
{
	ignoreNextUpdate=true;
	const std::vector<ApplicationConfiguration::t_freq>& tabfreq=ApplicationConfiguration::GetAllFrequencies();
	//Ajout des bandes
	for(int i=0;i<tabfreq.size();i++)
	{
		wxString name;
		wxString label;
		label<<tabfreq[i]<<" Hz";
		name<<tabfreq[i];
		E_Data_Row_BandeFreq* nouvLigne=new E_Data_Row_BandeFreq(this,name,label);
		this->AppendFils(nouvLigne);
	}
	//Ajout du cumul
	int i=tabfreq.size();
	E_Data_Row* nouvLigne=new E_Data_Row(this,"cumul","Global");
	nouvLigne->AppendPropertyDecimal("db","dB",0,true,PRECISION_DB);
	nouvLigne->AppendPropertyDecimal("dba","dB(A)",0,true,PRECISION_DB);
	_("Global");
	this->AppendFils(nouvLigne);
	ignoreNextUpdate=false;
}
void E_GammeFrequence::InitProp()
{
}
int E_GammeFrequence::GetIdSpectre()
{
	return this->idSpectre;
}

Element::ELEMENT_TYPE E_GammeFrequence::GetTypeSpectre()
{
	return this->elementInfo.typeElement;
}

wxXmlNode* E_GammeFrequence::SaveXMLCoreDoc(wxXmlNode* NoeudParent)
{
	return Element::SaveXMLCoreDoc(NoeudParent);
}

wxXmlNode* E_GammeFrequence::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
	thisNode->SetName("frequences"); // Nom de la balise xml ( pas d'espace autorise )
	thisNode->DeleteAttribute("idspectre");
	thisNode->AddAttribute("idspectre",Convertor::ToString(idSpectre));	
	return thisNode;
}

void E_GammeFrequence::CalcNiveauSonoreGlobal(const wxString& typeDb)
{
	float totdb=0;
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const t_elementInfo& infoFils=(*itfils)->GetElementInfos();
		if(infoFils.typeElement==ELEMENT_TYPE_ROW_BFREQ)
		{
			totdb+=pow(10,(*itfils)->GetDecimalConfig(typeDb)/10);
		}
	}
	totdb=10*log10f(totdb);
	Element* tot=this->GetElementByLibelle("cumul");
	if(tot!=NULL)
		tot->UpdateDecimalConfig(typeDb,totdb);
}

void E_GammeFrequence::Modified(Element* eModif)
{
	if(!ignoreNextUpdate)
	{
		t_elementInfo elInfo=eModif->GetElementInfos();
		Element* pereEModif=eModif->GetElementParent();
		if(pereEModif!=NULL)
		{
			t_elementInfo pereInfo=pereEModif->GetElementInfos();
			if(pereInfo.libelleElement!="cumul")
			{
				ignoreNextUpdate=true;
				CalcNiveauSonoreGlobal("db");
				CalcNiveauSonoreGlobal("dba");
				ignoreNextUpdate=false;
			}else{
				ignoreNextUpdate=true;
				float nouvVal=pereEModif->GetDecimalConfig(elInfo.libelleElement);
				CalcNiveauSonoreGlobal(elInfo.libelleElement);
				SetGlobalLevel( elInfo.libelleElement, nouvVal);
				ignoreNextUpdate=false;
			}
			Element::Modified(eModif); //Element de base modifié
		}
	}
}
