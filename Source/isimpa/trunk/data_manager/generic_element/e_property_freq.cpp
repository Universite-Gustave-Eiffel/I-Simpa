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

#include "e_property_freq.h"
#include "manager/stringTools.h"
#include <algorithm>
#include "data_manager/e_data_row_ext_bandefreq.h"
#include <wx/log.h>
#include "manager/boollocker.h"
#include "last_cpp_include.hpp"

bool orderSpectrumsRows(Element* lSp,Element* rSp)
{
	long leftSpectrumId(0),rightSpectrumId(0);
	if(!lSp->GetElementInfos().libelleElement.ToLong(&leftSpectrumId))
		return lSp<rSp;
	if(!rSp->GetElementInfos().libelleElement.ToLong(&rightSpectrumId))
		return lSp<rSp;
	return leftSpectrumId<rightSpectrumId;
}
E_Property_Freq::E_Property_Freq( wxXmlNode* noeudCourant ,  Element* parent)
		:Element(parent,"Spectre",Element::ELEMENT_TYPE_PROPERTY_FREQ,noeudCourant)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_SPECTRUM);
		ignoreUpdate=false;
		askReload=false;
		_("Spectrum");
		wxString propVal;
		//Affecte la valeur de l'element

		typeSpectre=Element::ELEMENT_TYPE_GAMMEFREQ_APP;
		idSpectre=0;
		//Element initialisé AVEC Xml
		if(noeudCourant->GetPropVal("idspectre",&propVal))
			idSpectre=Convertor::ToInt(propVal);
		if(noeudCourant->GetPropVal("typespectre",&propVal))
			typeSpectre=(Element::ELEMENT_TYPE)Convertor::ToInt(propVal);
		if(noeudCourant->GetPropVal("valspectre",&propVal)) //Test pour l'ancienne version de cette élément psps avant le 31 ocobre 2008
		{
			float cValue=Convertor::ToFloat(propVal);
			CreateGlobalVal(cValue);
		}
		wxString propValue;
		wxXmlNode* currentChild;
		currentChild = noeudCourant->GetChildren();
		while(currentChild!=NULL)
		{
			if(currentChild->GetPropVal("eid",&propValue))
			{
				BoolLocker lockUpdate(&this->ignoreUpdate,true); 
				long typeEle;
				propValue.ToLong(&typeEle);
				if(typeEle==Element::ELEMENT_TYPE_ROW_EXTBFREQ)
				{
					this->AppendFils(new E_Data_Row_ExtBandeFreq(currentChild,this));
				}				
			}
			currentChild = currentChild->GetNext();
		}
		//this->DeleteAllElementByType(Element::ELEMENT_TYPE_LIST);
		Element* globalRow=this->GetElementByType(ELEMENT_TYPE_ROW);
		if(globalRow)
		{
			//Maj depuis version 1.1.1 en inférieur
			if(!globalRow->IsPropertyExist("att"))
			{
				BoolLocker lockUpdate(&this->ignoreUpdate,true); 
				globalRow->SetReadOnlyConfig("dba");
				globalRow->AppendPropertyDecimal("att","A",10*log10((float)ApplicationConfiguration::GetAllFrequencies().size()),true,2);
				globalRow->AppendPropertyDecimal("lw","Lw",globalRow->GetDecimalConfig("db"),true,2);
			}
			//Maj depuis version 1.1.6
			if(this->GetElementByType(ELEMENT_TYPE_ROW_EXTBFREQ)==NULL)
			{
				float attlvl(globalRow->GetDecimalConfig("lw")-globalRow->GetDecimalConfig("db"));
				BoolLocker lockUpdate(&this->ignoreUpdate,true); 
				globalRow->SetReadOnlyConfig("db");
				globalRow->SetReadOnlyConfig("lw");
				globalRow->SetReadOnlyConfig("dba");
				globalRow->SetReadOnlyConfig("att",false);
				CreateExtendedBFreq();
				this->SetGlobalLevel("att",("lw",10*log10(pow(10.f,attlvl/10.f)*ApplicationConfiguration::GetAllFrequencies().size())));
			}
			//Maj depuis la version 1.1.8
			if(globalRow->IsReadOnlyConfig("lw"))
				globalRow->SetReadOnlyConfig("lw",false);
		}
	}


E_Property_Freq::E_Property_Freq( Element* parent, wxString label)
	:ignoreUpdate(false),
	Element(parent,label,Element::ELEMENT_TYPE_PROPERTY_FREQ)
{
	SetIcon(GRAPH_STATE_ALL,GRAPH_SPECTRUM);
	askReload=false;
	typeSpectre=Element::ELEMENT_TYPE_GAMMEFREQ_APP;
	idSpectre=0;
	CreateExtendedBFreq();
	CreateGlobalVal(0.f);
}
void E_Property_Freq::CreateExtendedBFreq()
{
	BoolLocker lockUpdate(&this->ignoreUpdate,true); 
	const std::vector<ApplicationConfiguration::t_freq>& tabfreq=ApplicationConfiguration::GetAllFrequencies();
	//Ajout des bandes
	for(int i=0;i<tabfreq.size();i++)
	{
		wxString name(wxString::Format("%i",tabfreq[i]));
		wxString label(wxString::Format("%i Hz",tabfreq[i]));
		E_Data_Row_ExtBandeFreq* nouvLigne=new E_Data_Row_ExtBandeFreq(this,name,label);
		this->AppendFils(nouvLigne);
	}
}
void E_Property_Freq::LoadLwFromBdd()
{
	
	//Met à jour si nécessaire les données Lw (niveau d'origine) à partir de la base de données
	E_GammeFrequence* currentSpectre=ApplicationConfiguration::GetSpectrum(idSpectre,typeSpectre);
	if(currentSpectre)
	{
		std::vector<Element*> bfreq_rows;
		std::vector<Element*> this_bfreq_rows;
		currentSpectre->GetAllElementByType(ELEMENT_TYPE_ROW_BFREQ,bfreq_rows);
		std::sort(bfreq_rows.begin(),bfreq_rows.end(),orderSpectrumsRows);
		this->GetAllElementByType(ELEMENT_TYPE_ROW_EXTBFREQ,this_bfreq_rows);
		std::sort(this_bfreq_rows.begin(),this_bfreq_rows.end(),orderSpectrumsRows);
		if(this_bfreq_rows.size()!=bfreq_rows.size())
		{
			if(this->pere)
				wxLogWarning(_("Spectrum of element %s does not exist! Please, specify a valid spectrum"),this->pere->GetTreeLabel());
			return;
		}
		BoolLocker lockUpdate(&this->ignoreUpdate,true);
		float UserDefinedLwGlobal=GetGlobaldBVal("lw");
		for(std::size_t i=0;i<bfreq_rows.size();i++)
		{
			Element* bdd_el=bfreq_rows[i];
			Element* this_el=this_bfreq_rows[i];
			this_el->UpdateDecimalConfig("lw",bdd_el->GetDecimalConfig("db"));
		}
		CalcNiveauSonoreGlobal("lw");
		CalcNiveauSonoreGlobal("db");
		CalcNiveauSonoreGlobal("dba");
		CalcNiveauSonoreGlobal("att");

		lockUpdate.Release();
		SetGlobalLevel("lw",UserDefinedLwGlobal);
	}else{
		if(this->pere)
			wxLogWarning(_("Spectrum of element %s does not exist! Please, specify a valid spectrum"),this->pere->GetTreeLabel());
	}
}
void E_Property_Freq::CreateGlobalVal(float valDefault)
{
	BoolLocker lockUpdate(&this->ignoreUpdate,true); 
	Element* nouvLigne=this->AppendFils(new E_Data_Row(this,"cumul","Global"));
	nouvLigne->AppendPropertyDecimal("db","dB",valDefault,true,2);
	nouvLigne->AppendPropertyDecimal("dba","dB(A)",0,true,2);
	nouvLigne->AppendPropertyDecimal("att","Attenuation",0,false,2);
	nouvLigne->AppendPropertyDecimal("lw","Lw",0,false,2);
	#if 0
	_("Global");
	_("dB(A)");
	_("Attenuation");
	_("Lw");
	#endif
}


bool E_Property_Freq::orderSpectrums(Element* lEl,Element* rEl)
{
	E_GammeFrequence* lSpectre=dynamic_cast<E_GammeFrequence*>(lEl);
	E_GammeFrequence* rSpectre=dynamic_cast<E_GammeFrequence*>(lEl);
	if(lSpectre && rSpectre)
	{
		if(lSpectre->GetTypeSpectre()!=rSpectre->GetTypeSpectre())
		{
			if(lSpectre->GetTypeSpectre()==Element::ELEMENT_TYPE_GAMMEFREQ_USER)
				return false;
			else
				return true; //Spectre application en premier
		}else{
			return lSpectre->GetIdSpectre()<rSpectre->GetIdSpectre(); //Meme type de spectre
		}
	}else{
		return *lEl<*rEl;
	}
}

float E_Property_Freq::GetGlobaldBVal(const wxString& dbCol)
{
	Element* rowGlobal=this->GetElementByLibelle("cumul");
	if(rowGlobal)
		return rowGlobal->GetDecimalConfig(dbCol);
	else
		return 0;
}
wxXmlNode* E_Property_Freq::SaveXMLCoreDoc(wxXmlNode* NoeudParent)
{
	LoadLwFromBdd(); //met à jour depuis la base de données si nécessaire
	return Element::SaveXMLCoreDoc(NoeudParent);
}

bool E_Property_Freq::IsAskPropReload()
{
	return askReload;
}
void E_Property_Freq::InitProp()
{
	askReload=false;

	lastLst.clear();
	lastLst=ApplicationConfiguration::GetLstSpectre();
	BoolLocker lockUpdate(&this->ignoreUpdate,true); //On ne remonte pas les informations de modification
	this->DeleteAllElementByType(Element::ELEMENT_TYPE_LIST);
	std::vector<wxString> txtLst;
	long defval=0;
	txtLst.reserve(lastLst.size());
	for(int i=0;i<lastLst.size();i++)
	{
		txtLst.push_back(lastLst[i].nom);
		if(lastLst[i].idSpectre==idSpectre && lastLst[i].typeSpectre==typeSpectre)
			defval=i;
	}
	
	this->AppendPropertyList("idspectre","Type de spectre",txtLst,defval,true,4);
	("Type de spectre");
	LoadLwFromBdd();
}

void E_Property_Freq::CalcNiveauSonoreGlobal(const wxString& typeDb)
{
	float totdb=0.f;
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		const t_elementInfo& infoFils=(*itfils)->GetElementInfos();
		if(infoFils.typeElement==ELEMENT_TYPE_ROW_EXTBFREQ)
		{
			totdb+=pow(10,(*itfils)->GetDecimalConfig(typeDb)/10);
		}
	}
	totdb=10*log10f(totdb);
	Element* tot=this->GetElementByLibelle("cumul");
	if(tot!=NULL)
		tot->UpdateDecimalConfig(typeDb,totdb);
}


wxXmlNode* E_Property_Freq::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
	thisNode->SetName("spectre"); // Nom de la balise xml ( pas d'espace autorise )
	thisNode->DeleteProperty("idspectre");
	thisNode->AddProperty("idspectre",wxString::Format("%i",idSpectre));
	thisNode->DeleteProperty("typespectre");
	thisNode->AddProperty("typespectre",wxString::Format("%i",typeSpectre));
	return thisNode;
}

void E_Property_Freq::Modified(Element* eModif)
{
	if(!ignoreUpdate)
	{
		const t_elementInfo& elInfo(eModif->GetElementInfos());
		wxString parent_label;
		if(eModif->GetElementParent())
			parent_label=eModif->GetElementParent()->GetElementInfos().libelleElement;
		if(elInfo.libelleElement=="idspectre")
		{
			int idList=this->GetListConfig("idspectre");
			if(idList<lastLst.size() && idList>=0)
			{
				idSpectre=lastLst[idList].idSpectre;
				typeSpectre=lastLst[idList].typeSpectre;
				BoolLocker lockUpdate(&this->ignoreUpdate,true); 
				LoadLwFromBdd();
				askReload=true; //On demande la réouverture de la feuille de propriété.
			}
		}else if(parent_label!="cumul")
		{
			BoolLocker lockUpdate(&this->ignoreUpdate,true);
			CalcNiveauSonoreGlobal("db");
			CalcNiveauSonoreGlobal("dba");
			CalcNiveauSonoreGlobal("att");
			CalcNiveauSonoreGlobal("lw");
		}else if(parent_label=="cumul")
		{
			if(elInfo.libelleElement=="att")
			{
				BoolLocker lockUpdate(&this->ignoreUpdate,true);
				float newval=GetGlobaldBVal("att");
				CalcNiveauSonoreGlobal("att");
				SetGlobalLevel("att",newval);
			}else if(elInfo.libelleElement=="lw")
			{
				BoolLocker lockUpdate(&this->ignoreUpdate,true);
				float newval=GetGlobaldBVal("lw");
				CalcNiveauSonoreGlobal("lw");
				SetGlobalLevel("lw",newval);
			}
		}
		Element::Modified(eModif);
	}
}

void E_Property_Freq::SetGlobalLevel( const wxString& typeDb, wxFloat32 lvl )
{
	BoolLocker lockUpdate(&this->ignoreUpdate,true);
	Element* globalRow=this->GetElementByLibelle("cumul");
	if(globalRow)
	{
		//1ere étape, lire la valeur global précédente et calcul le delta
		float delta=lvl-globalRow->GetDecimalConfig(typeDb);
		//2eme étape mettre à jour chaque bande de fréquence
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			const t_elementInfo& infoFils=(*itfils)->GetElementInfos();
			if(infoFils.typeElement==ELEMENT_TYPE_ROW_EXTBFREQ)
			{
				float ansval=(*itfils)->GetDecimalConfig(typeDb);
				(*itfils)->UpdateDecimalConfig(typeDb,ansval+delta);
			}
		}
		globalRow->UpdateDecimalConfig(typeDb,lvl);
		if(typeDb!="dba")
			CalcNiveauSonoreGlobal("dba");
		if(typeDb!="db")
			CalcNiveauSonoreGlobal("db");
		if(typeDb!="att")
			CalcNiveauSonoreGlobal("att");

	}
}
