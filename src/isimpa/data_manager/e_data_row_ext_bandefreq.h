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

#include "data_manager/e_data_row_bandefreq.h"

#ifndef __ELEMENT_ROW_EXT_FREQ__
#define __ELEMENT_ROW_EXT_FREQ__

/** \file e_data_row_ext_bandefreq.h
    \brief Classe spécialisant E_Data_Row_BandeFreq afin de rajouter les informations spécifiques tel que l'attenuation
*/

/**
	\brief Classe spécialisant E_Data_Row_BandeFreq afin de rajouter les informations spécifiques tel que l'attenuation

	Il compose l'element e_gamme_frequence.
	@see E_GammeFrequence
	@see MainPropGrid
*/
class E_Data_Row_ExtBandeFreq: public E_Data_Row_BandeFreq
{
public:
	E_Data_Row_ExtBandeFreq( wxXmlNode* noeudCourant ,  Element* parent)
		: E_Data_Row_BandeFreq( noeudCourant ,  parent,ELEMENT_TYPE_ROW_EXTBFREQ)
	{

	}

	
	/**
	 *	@param parent Element parent
	 *	@param[in] dataName Nom du champ ( non visible par l'utilisateur)
	 *	@param[in] dataLabel Libellé de la ligne
	 *	@param[in] _ponderation Valeur de la pondération entre dB et dB(A)
	 *  @see E_GammeFrequence
	 */
	E_Data_Row_ExtBandeFreq( Element* parent, const wxString& dataName,const wxString& dataLabel)
		:E_Data_Row_BandeFreq( parent, dataName, dataLabel,ELEMENT_TYPE_ROW_EXTBFREQ)
	{
		SetPonderationState(false);
		AppendPropertyDecimal("att","Attenuation",0,false,PRECISION_DB);
		AppendPropertyDecimal("lw","Lw",0,true,PRECISION_DB);
		this->SetReadOnlyConfig("db");
		this->SetReadOnlyConfig("dba");
		SetPonderationState(true);
		wxGetTranslation("Attenuation");
		wxGetTranslation("Lw");
	}

	/*
	 * Cette méthode permet de bloquer/activer la ponderation dB->dB(A) et inverse
	 * @param doPonderation Vrai si la ponderation sera effectué lors de la mise à jour d'un des champs
	 */
	virtual void SetPonderationState( bool doPonderation )
	{
		if(doPonderation)
		{
			ComputeAttenuated();
		}
		E_Data_Row_BandeFreq::SetPonderationState(doPonderation);
	}
protected:
	void Modified(Element* eModif)
	{
		t_elementInfo filsInfo=eModif->GetElementInfos();
		if(!ignoreNextUpdate)
		{
			if(filsInfo.libelleElement=="att"){
				ignoreNextUpdate=true;
				ComputeAttenuated();
				PonderationFromDb();
			}else if(filsInfo.libelleElement=="lw"){
				ignoreNextUpdate=true;
				ComputeAttenuated();
				PonderationFromDb();
			}
			ignoreNextUpdate=false;
			E_Data_Row_BandeFreq::Modified(eModif);
		}	
	}

	//La valeur dB a changé il faut recalculer l'attenuation correspondante
	void ComputeAttenuation()
	{
		this->UpdateDecimalConfig("att",this->GetDecimalConfig("lw")-this->GetDecimalConfig("db"));
	}
	//La valeur Lw ou att a changé il faut recalculer la valeur en dB
	void ComputeAttenuated()
	{
		this->UpdateDecimalConfig("db",this->GetDecimalConfig("lw")-this->GetDecimalConfig("att"));
	}
};

#endif
