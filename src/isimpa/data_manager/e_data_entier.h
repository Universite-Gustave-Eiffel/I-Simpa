/* ----------------------------------------------------------------------
* I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
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
* For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or
* send an email to contact@noise-planet.org
*
* ----------------------------------------------------------------------*/

#include "first_header_include.hpp"

#include "data_manager/e_data.h"

#ifndef __ELEMENT_INTEGER__
#define __ELEMENT_INTEGER__

/** \file e_data_entier.h
   \brief Classe spécialisant E_Data afin de représenter un entier
*/

bool GetFormulaEvaluation(const wxString& formula,double& evalResult);
/**
	\brief Classe spécialisant E_Data afin de représenter un entier
	@see MainPropGrid
*/
class E_Data_Integer: public E_Data
{
protected:
	int elementValue;
	int maxValue;
	int minValue;
	bool isMaxValue;
	bool isMinValue;
public:
	E_Data_Integer( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Data(parent,"Unnamedprop","",Element::ELEMENT_TYPE_INTEGER,noeudCourant)
	{
		isMaxValue=false;
		isMinValue=false;
		wxString propVal;
		//Affecte la valeur de l'element
		long lval;
		if(noeudCourant->GetAttribute("value",&propVal))
		{
			propVal.ToLong(&lval);
			elementValue=lval;
		}
		if(noeudCourant->GetAttribute("maxValue",&propVal))
		{
			maxValue=StringToInt(propVal);
			isMaxValue=true;
		}
		if(noeudCourant->GetAttribute("minValue",&propVal))
		{
			minValue=StringToInt(propVal);
			isMinValue=true;
		}
	}

	/**
		@param parent Parent element
		@param dataName Field name ( hidden from user)
		@param dataLabel Field label
		@param defaultDataValue Default field value
	* @see Element::AppendPropertyInteger()
	*/
		E_Data_Integer( Element* parent, wxString dataName,wxString dataLabel, int defaultDataValue)
		:E_Data(parent,dataName,dataLabel,Element::ELEMENT_TYPE_INTEGER)
	{
		isMaxValue=false;
		isMinValue=false;
		elementValue=defaultDataValue;
	}

	void UpdateValue( wxGrid* gridCtrl, int row, int col=0)
	{
		E_Data::UpdateValue(gridCtrl,row,col);
		if(gridCtrl->GetCellValue(row,col)!=IntToString(elementValue))
			SetGridValue(gridCtrl,row,col);
	}

	void SetMaxValue(int _maxValue)
	{
		isMaxValue=true;
		maxValue=_maxValue;
	}

	void SetMinValue(int _minValue)
	{
		isMinValue=true;
		minValue=_minValue;
	}
	virtual void FillWxGrid(wxGrid* gridToFeed,int col=0)
	{
		E_Data::FillWxGrid(gridToFeed,col);
		SetGridValue(gridToFeed,gridToFeed->GetNumberRows()-1, col);
	}

	virtual void OnValueChanged(wxGridEvent& ev,wxGrid* gridCtrl)
	{
		int row(ev.GetRow()),col(ev.GetCol());
		wxString sValue=gridCtrl->GetCellValue(row,col);
		bool isint(false);
		float userVal=StringToInt(sValue,&isint);
		if(isint && (!isMaxValue || userVal<=maxValue) && (!isMinValue || userVal>=minValue))
		{
			this->SetValue(userVal);
			SetGridValue(gridCtrl,row,col);
		}else{
			wxBell();
			ev.Veto();
			if(isMaxValue && isMinValue)
				wxLogMessage(wxGetTranslation("Data must be in the range %i - %i"),minValue,maxValue);
			else if(isMaxValue)
				wxLogMessage(wxGetTranslation("Authorized maximum value: %i"),maxValue);
			else if(isMinValue)
				wxLogMessage(wxGetTranslation("Authorized minimum value: %i."),minValue);
		}
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Data::SaveXMLDoc(NoeudParent);
		thisNode->DeleteAttribute("value");
		wxString sval;
		sval<<elementValue;
		thisNode->AddAttribute("value",sval);		
		thisNode->DeleteAttribute("maxValue");
		thisNode->DeleteAttribute("minValue");
		if(isMinValue)
		{
			thisNode->AddAttribute("minValue",IntToString(minValue));	
		}
		if(isMaxValue)
		{
			thisNode->AddAttribute("maxValue",IntToString(maxValue));
		}
		return thisNode;
	}
	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(exportToCore)
		{
			NoeudParent->DeleteAttribute(this->elementInfo.libelleElement);
			NoeudParent->AddAttribute(this->elementInfo.libelleElement,IntToString(elementValue));
		}
		return NoeudParent;
	}
	void SetValue(wxString newValue)
	{
		bool isint(false);
		int nvval=StringToInt(newValue,&isint);
		if(isint)
		{
			if(nvval!=this->elementValue)
			{
				this->elementValue=nvval;
				this->Modified(this);
			}
		}
	}

	void SetValue(int newValue)
	{
		if(newValue!=this->elementValue)
		{
			this->elementValue=newValue;
			this->Modified(this);
		}
	}
	int GetValue()
	{
		return elementValue;
	}
private:



	wxString IntToString(int val)
	{
		return wxString::Format("%i",val);
	}


	int StringToInt(wxString sval,bool* isInt=NULL)
	{
		if(isInt)
			*isInt=true;
		long rval;
		if(sval.ToLong(&rval))
		{
			return rval;
		}else{
			double val(0.f);
			if(GetFormulaEvaluation(sval,val))
			{
				return (int)val;
			}else{
				if(isInt)
					*isInt=false;
				return 0;
			}
		}
	}
	void SetGridValue(wxGrid* gridCtrl, int row, int col)
	{
		gridCtrl->SetCellValue(row,col,IntToString(elementValue));
	}

};

#endif
