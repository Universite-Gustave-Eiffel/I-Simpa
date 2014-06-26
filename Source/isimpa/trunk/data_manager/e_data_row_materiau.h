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

#include "data_manager/e_data_row.h"
#include "manager/stringTools.h"
#include <wx/log.h>

#ifndef __ELEMENT_ROW_MAT__
#define __ELEMENT_ROW_MAT__


/** \file e_data_row_materiau.h
   \brief Classe spécialisant E_Data_Row afin de faire le lien entre l'absorption, la diffusion et la loi.
*/

/**
	\brief Classe spécialisant E_Data_Row afin de faire le lien entre l'absorption, la diffusion et la loi.

	Il compose l'element e_materiau
	@see E_Materiau
	@see MainPropGrid
*/
class E_Data_Row_Materiau: public E_Data_Row
{
private:
	bool ignoreNextUpdate; /*!< Vrai si l'événement de mise à jour ne doit pas être propagé à l'élément parent*/
public:
	E_Data_Row_Materiau( wxXmlNode* noeudCourant,  Element* parent)
		:E_Data_Row( noeudCourant ,  parent,ELEMENT_TYPE_ROW_MATERIAU)
	{
		ignoreNextUpdate=true; //Empeche la modification par le programme de l'un, affecter l'autre
		wxString absorb;
		wxString diffusion;
		wxString affaiblissement="0";
		wxString loi;
		bool transmission;

		noeudCourant->GetPropVal("loi", &loi);
		transmission=noeudCourant->GetPropVal("affaiblissement", &affaiblissement);
		noeudCourant->GetPropVal("diffusion",&diffusion);
		noeudCourant->GetPropVal("absorb", &absorb);


		InitPropMat(Convertor::ToFloat(absorb),
			Convertor::ToFloat(diffusion),
			transmission,
			Convertor::ToFloat(affaiblissement),
			Convertor::ToFloat(loi));
		ignoreNextUpdate=false;

	}
	/**
	 *	@param parent Element parent
	 *	@param[in] dataName Nom du champ ( non visible par l'utilisateur)
	 *	@param[in] dataLabel Libellé de la ligne
	 *  @see E_Materiau
	 */
	E_Data_Row_Materiau( Element* parent, wxString dataName,wxString dataLabel)
		:E_Data_Row( parent, dataName, dataLabel,ELEMENT_TYPE_ROW_MATERIAU)
	{
		ignoreNextUpdate=false;
		InitPropMat(0,0,false,0,0);
	}
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxString tmpVal;
		wxXmlNode* thisNode = E_Data::SaveXMLDocWithoutChildren(NoeudParent);
		thisNode->SetName("bfreq"); // Nom de la balise xml ( pas d'espace autorise )

		//Sauvegarde des données enfants vers nos propriétés
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			(*itfils)->SaveXMLCoreDoc(thisNode);
		}
		bool transmission=this->GetBoolConfig("transmission");
		if(!transmission)
			thisNode->DeleteProperty("affaiblissement");
		return NULL;
	}
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* NoeudCourant=new wxXmlNode(NoeudParent,wxXML_ELEMENT_NODE,"bfreq");
		NoeudCourant->AddProperty("freq",this->elementInfo.libelleElement);
		bool transmission=this->GetBoolConfig("transmission");
		Element::SaveXMLCoreDoc(NoeudCourant);
		if(!transmission)
			NoeudCourant->DeleteProperty("affaiblissement");
		return NoeudCourant;
	}
protected:
	void Modified(Element* eModif)
	{
		t_elementInfo filsInfo=eModif->GetElementInfos();
		if(!ignoreNextUpdate)
		{   
			if(filsInfo.libelleElement=="absorb")
			{
				if(this->GetDecimalConfig("absorb")==1)
				{
					this->UpdateDecimalConfig("diffusion",0);
				    ignoreNextUpdate=true;
					this->SetReadOnlyConfig("diffusion");
				}else{
				    ignoreNextUpdate=true;
					this->SetReadOnlyConfig("diffusion",false);
				}
				bool transmission=this->GetBoolConfig("transmission");
				if(transmission)
				{
					//Test de l'affaiblissement
					float affaib=this->GetDecimalConfig("affaiblissement");
					float absorb=this->GetDecimalConfig("absorb");
					if(absorb==0)
					{
						ignoreNextUpdate=false;
						this->UpdateBoolConfig("transmission",false);
					}else{
						float tau=pow(10.,-affaib/10.);
						if(tau>absorb)
						{
							affaib=-10*log10(absorb);
							ignoreNextUpdate=true;
							this->UpdateDecimalConfig("affaiblissement",affaib);
							wxLogWarning(_("La valeur de l'affaiblissement est incompatible avec celle de l'absorption. Le niveau d'affaiblissement a été fixé à %.2f dB afin de conserver votre facteur d'absorption à %.3f."),affaib,absorb);
							ignoreNextUpdate=false;
						}
					}
				}
			}else if(filsInfo.libelleElement=="diffusion")
			{
				ignoreNextUpdate=true;
				if(this->GetDecimalConfig("diffusion")==0)
				{
					this->SetReadOnlyConfig("loi");
					this->UpdateListConfig("loi",0);
				}else{
					this->SetReadOnlyConfig("loi",false);
					this->UpdateListConfig("loi",2); //2 lambert par défaut
				}
								
			}else if(filsInfo.libelleElement=="loi")
			{
				ignoreNextUpdate=true;
				if(this->GetListConfig("loi")==0) //si spéculaire choisi
				{
					this->UpdateDecimalConfig("diffusion",0);
					this->SetReadOnlyConfig("loi");
				}				
			}else if(filsInfo.libelleElement=="transmission")
			{
				bool transmission=this->GetBoolConfig("transmission");
				if(transmission) //On essaye de conserver l'absorption saisie si on active la transmission
				{
					Element* absEle=this->GetElementByLibelle("absorb");
					if(absEle)
					{
						absEle->Modified(absEle);
						if(this->GetDecimalConfig("absorb")>0.f)
							this->SetReadOnlyConfig("affaiblissement",!transmission);
						else
							wxLogWarning(_("Vous devez saisir une valeur d'absorption supérieure à 0 avant de définir une transmission."));
					}
				}
			}else if(filsInfo.libelleElement=="affaiblissement")
			{
				bool transmission=this->GetBoolConfig("transmission");
				if(transmission)
				{
					//Test de l'affaiblissement
					float affaib=this->GetDecimalConfig("affaiblissement");
					float absorb=this->GetDecimalConfig("absorb");
					float tau=pow(10.,-affaib/10.);
					if(tau>absorb)
					{
						ignoreNextUpdate=true;
						this->UpdateDecimalConfig("absorb",tau);
						wxLogWarning(_("La valeur de l'affaiblissement est incompatible avec celle de l'absorption. Le facteur d'absorption a été fixé à %.3f afin de conserver votre niveau d'affaiblissement à %.2f dB."),tau,affaib);
						ignoreNextUpdate=false;
					}
				}
			}
			E_Data_Row::Modified(eModif);
			ignoreNextUpdate=false;
		}	
	}

	private:

	void InitPropMat(float absorb,float diffusion,bool transmission, float affaiblissement,int loi)
	{

		std::vector<wxString> Lstlois=ApplicationConfiguration::GetLstLoisReff();
		int precisionDecimal=2;
		this->AppendPropertyDecimal("absorb","Absorption",absorb,false,precisionDecimal,true,true,1,0,true);
		this->AppendPropertyDecimal("diffusion","Diffusion",diffusion,(absorb==1),precisionDecimal,true,true,1,0,true);
		this->AppendPropertyBool("transmission","Transmission",transmission,false);
		this->AppendPropertyDecimal("affaiblissement","Affaiblissement (dB)",affaiblissement,!transmission,1,false,true,0,0,true);
		this->AppendPropertyList("loi","Loi de diffusion",Lstlois,loi,false,1,std::vector<int>(),true);
		if(loi==0)
			this->SetReadOnlyConfig("loi");
		#if 0
			_("Absorption");
			_("Diffusion");
			_("Loi de diffusion");
			_("Affaiblissement (dB)");
			_("Transmission");
		#endif

	}
};

#endif