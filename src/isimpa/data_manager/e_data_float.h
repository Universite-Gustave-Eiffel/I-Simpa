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

#include "data_manager/e_data.h"

#ifndef __ELEMENT_FLOAT__
#define __ELEMENT_FLOAT__

/** \file e_data_float.h
   \brief Classe spécialisant E_Data afin de représenter un nombre à décimal
*/
bool GetFormulaEvaluation(const wxString& formula,double& evalResult);
/**
	\brief Classe spécialisant E_Data afin de représenter un nombre à décimal
	@see MainPropGrid
*/
class E_Data_Float: public E_Data
{
protected:
	float elementValue;
	float maxValue;
	float minValue;
	bool isMaxValue;
	bool isMinValue;
	wxString precision;
public:
	E_Data_Float( wxXmlNode* noeudCourant ,  Element* parent)
		:E_Data(parent,"Unnamedprop","",ELEMENT_TYPE_FLOAT,noeudCourant)
	{
		precision="4";

		maxValue=0;
		minValue=0;
		isMaxValue=false;
		isMinValue=false;

		wxString propVal;
		//Affecte la valeur de l'element
		if(noeudCourant->GetAttribute("value",&propVal))
			elementValue=StringToFloat(propVal);
		if(noeudCourant->GetAttribute("maxValue",&propVal))
		{
			maxValue=StringToFloat(propVal);
			isMaxValue=true;
		}
		if(noeudCourant->GetAttribute("minValue",&propVal))
		{
			minValue=StringToFloat(propVal);
			isMinValue=true;
		}
		if(noeudCourant->GetAttribute("pr",&propVal))
			precision=propVal;
	}

	/**
	*	@see Element::AppendPropertyDecimal()
		@param parent Element parent
		@param dataName Nom du champ ( non visible par l'utilisateur)
		@param dataLabel Libellé du champ
		@param defaultDataValue Valeur par défaut
		@param _readonly Vrai si en lecture seul
		@param _precision Nombre de decimal Après la virgule
	*/
		E_Data_Float( Element* parent, wxString dataName,wxString dataLabel, float defaultDataValue,bool _readonly=false, int _precision=4)
		:E_Data(parent,dataName,dataLabel,ELEMENT_TYPE_FLOAT)
	{
		maxValue=0;
		minValue=0;
		isMaxValue=false;
		isMinValue=false;
		precision<<_precision;
		elementValue=defaultDataValue;
		readOnly=_readonly;
	}


	wxXmlNode* SaveXMLCoreDoc(wxXmlNode* NoeudParent)
	{
		if(exportToCore)
		{
			NoeudParent->DeleteAttribute(this->elementInfo.libelleElement);
			NoeudParent->AddAttribute(this->elementInfo.libelleElement,FloatToString(elementValue));
		}
		return NoeudParent;
	}


	virtual void FillWxGrid(wxGrid* gridToFeed,int col=0)
	{
		int row=gridToFeed->GetNumberRows()-1;
		long prec;
		this->precision.ToLong(&prec);
		// wxGridCellFloatEditor ne comprend pas le . comme séparateur de décimale
		//gridToFeed->SetCellEditor(row, col, new wxGridCellFloatEditor(-1,prec));
		E_Data::FillWxGrid(gridToFeed,col);
		SetGridValue(gridToFeed,row,col);
	}


	void UpdateValue( wxGrid* gridCtrl, int row, int col=0)
	{
		E_Data::UpdateValue(gridCtrl,row,col);
		if(gridCtrl->GetCellValue(row,col)!=FloatToString(elementValue))
			SetGridValue(gridCtrl,row,col);
	}


	virtual void OnValueChanged(wxGridEvent& ev,wxGrid* gridCtrl)
	{
		int row=ev.GetRow();
		int col=ev.GetCol();
		wxString sValue=gridCtrl->GetCellValue(row,col);
		//Si l'utilisateur utilise le . du pavé numerique et n'est pas accordé par le système
		//On le convertie en ,
		bool correctValue(false);
		float userVal=StringToFloat(sValue,&correctValue);
		if(correctValue && (!isMaxValue || userVal<=maxValue) && (!isMinValue || userVal>=minValue))
		{
			this->SetValue(userVal);
			SetGridValue(gridCtrl,row,col);
		}else{
			wxBell();
			ev.Veto();
			if(isMaxValue && isMinValue)
				wxLogMessage(_("Data must be in the range %s - %s"),FloatToString(minValue),FloatToString(maxValue));
			else if(isMaxValue)
				wxLogMessage(_("Authorized maximum value: %s"),FloatToString(maxValue));
			else if(isMinValue)
				wxLogMessage(_("Authorized minimum value: %s"),FloatToString(minValue));
		}
	}

	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Data::SaveXMLDoc(NoeudParent);
		thisNode->DeleteAttribute("value");
		thisNode->AddAttribute("value",FloatToString(elementValue));	

		thisNode->DeleteAttribute("pr");
		wxString sPrecision;
		sPrecision<<precision;
		thisNode->AddAttribute("pr",sPrecision);	
		thisNode->DeleteAttribute("maxValue");
		thisNode->DeleteAttribute("minValue");
		if(isMinValue)
		{
			thisNode->AddAttribute("minValue",FloatToString(minValue));	
		}
		if(isMaxValue)
		{
			thisNode->AddAttribute("maxValue",FloatToString(maxValue));
		}
		return thisNode;
	}

	/**
	 * Modifie la valeur du champ
	 * @param nvval Nouvelle valeur du champ
	 * @see Element::UpdateDecimalConfig()
	 */
	void SetValue(float nvval)
	{
		if(nvval!=this->elementValue)
		{
			this->elementValue=nvval;
			this->Modified(this);
		}
	}

	/**
	 * Modifie la valeur du champ
	 * @return Valeur du champ
	 * @see Element::GetDecimalConfig()
	 */
	float GetValue()
	{
		return elementValue;
	}
	

	/**
	 *  Affecte une condition de valeur maximal
	 *  @param[in] _MaxValue Valeur de limite
	 *  @param[in] _isMaxValue Active/Désactive la condition de limite
	 */
	void SetMaxValue(float _MaxValue,bool _isMaxValue=true)
	{
		maxValue=_MaxValue;
		isMaxValue=_isMaxValue;
	}
	/**
	 *  Affecte une condition de valeur minimal
	 *  @param[in] _MinValue Valeur de limite
	 *  @param[in] _isMinValue Active/Désactive la condition de limite
	 */
	void SetMinValue(float _MinValue,bool _isMinValue=true)
	{
		minValue=_MinValue;
		isMinValue=_isMinValue;
	}
private:


	void SetValue(wxString newValue)
	{
		bool isfloat(false);
		float nvval=StringToFloat(newValue);
		if(isfloat)
		{
			if(nvval!=this->elementValue)
			{
				this->elementValue=nvval;
				this->Modified(this);
			}
		}
	}
	
	void SetGridValue(wxGrid* gridCtrl, int row, int col)
	{
		gridCtrl->SetCellValue(row,col,FloatToString(elementValue,true));
	}

	wxString FloatToString(float val,bool setPrecision=false)
	{
		wxString sval;
		if(setPrecision)
		{
			sval.Printf("%."+precision+"f",val);
			if(StringToFloat(precision)>0)
				sval=sval.Left(sval.find_last_not_of("0")+1);
		}else{
			sval.Printf("%f",val);
		}
		return sval;
	}
	
	float StringToFloat(wxString sval,bool* ok=NULL)
	{
		if(ok)
			*ok=true;
		double rval(0.f);
		if(sval.ToDouble(&rval))
		{
			return (float)rval;
		}
		else
		{ //Conversion decimal si necessaire
			if(sval.Contains("."))
				sval.Replace(wxT("."), ",");
			else
				sval.Replace(wxT(","), ".");
			if(sval.ToDouble(&rval))
			{
				Modified(this); //A la prochaine sauvegarde tout les float seront convertie en local
				return (float)rval;
			}else{
				sval.Replace(wxT(","), ".");
				if(GetFormulaEvaluation(sval,rval))
					return (float)rval;
			}
		}
		
		if(ok)
			*ok=false;
		return 0.f;
	}
};

#endif
