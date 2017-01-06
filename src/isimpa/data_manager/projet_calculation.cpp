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

/**
 * @file projet_maillage.cpp
 * @brief Specification des méthode de ProjectManager ayant un rapport avec les calculs acoustiques (post-traitements)
 */
#include "projet.h"
#include <input_output/gabe/gabe.h>
#include <input_output/exportRecepteurSurf/rsbin.h>
#include "tree_rapport/e_report_gabe.h"
#include "IHM/customdialog/customdlg.h"
#include "last_cpp_include.hpp"

const float p_0=1/pow((float)(20*pow(10.f,(int)-6)),(int)2);
wxFloat32 to_deciBel(const wxFloat32& wjVal)
{
	return 10*log10f(wjVal);
}
wxFloat32 to_deciBelP0(const wxFloat32& wjVal)
{
	return 10*log10f(wjVal*p_0);
}
wxFloat32 to_deciBelRsurf(const wxFloat32& wjVal)
{
	return 10*log10f(wjVal/pow(10.f,-12.f));
}
void MakeSchroederArray(const std::vector<std::vector<wxFloat32> >& srcArray,std::vector<std::vector<wxFloat32> >& dstArray,wxFloat32 funcDbConv(const wxFloat32& wjVal)=&to_deciBelP0)
{
	//Copie brute
	dstArray=srcArray;

	//Méthode de Schroeder
	for(int idFreq=0;idFreq<dstArray.size();idFreq++)
	{
		float sumW=0;
		for(int idStep=dstArray[idFreq].size()-1;idStep>=0;idStep--)
		{
				sumW+=dstArray[idFreq][idStep];
				dstArray[idFreq][idStep]=funcDbConv(sumW);
		}
	}
}
/**
 * Opération de calcul au sein de SUM_OPERATION
 */
enum SUM_OPERATION
{
	SUM_OPERATION_X, // sum time
	SUM_OPERATION_Y, // sum power
	SUM_OPERATION_XY,// sum power * time
	SUM_OPERATION_X2 // sum time²
};

wxFloat32 GetSumLimit(wxInt32 idBandeFreq,wxFloat32 fromTime,wxFloat32 toTime,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj, SUM_OPERATION operation=SUM_OPERATION_Y,wxInt32* counter=NULL)
{
	wxFloat32 sumJ=0;
	wxFloat32 currentTime;
	for(int idStep=0;idStep<timeTable.size();idStep++)
	{
		currentTime=timeTable[idStep];
		if(currentTime>=fromTime && (currentTime<=toTime || toTime==-1.f))
		{
			if(counter)
				(*counter)++;
			if(operation==SUM_OPERATION_Y)
				sumJ+=tab_wj[idBandeFreq][idStep];
			else if(operation==SUM_OPERATION_XY)
				sumJ+=(tab_wj[idBandeFreq][idStep]*currentTime);
			else if(operation==SUM_OPERATION_X)
				sumJ+=currentTime;
			else if(operation==SUM_OPERATION_X2)
				sumJ+=currentTime*currentTime;
		}
		if(currentTime>toTime && toTime!=-1.f)
			break;
	}
	return sumJ;
}
struct LinearRegressionResult
{
	LinearRegressionResult(	wxFloat32 _a,wxFloat32 _b):a(_a),b(_b){};
	wxFloat32 a;
	wxFloat32 b;
};

wxFloat32 GetTimeDecay(wxFloat32 fromdB, const std::vector<wxFloat32>& timeTable, const std::vector<wxFloat32>& row_db) {
	wxFloat32 timeDeb = timeTable[0];
	wxFloat32 lastdB = row_db[0];
	for (int idStep = 0; idStep<row_db.size(); idStep++)
	{
		if (abs(row_db[idStep] - lastdB) >= fromdB) {
			break;
		} else {
			timeDeb = timeTable[idStep];
		}
	}
	return timeDeb;
}
/**
 * A partir des bornes de décroissance en dB retourne les bornes correspondantes en ms
 */
void GetTimeRange( wxFloat32 fromdB,wxFloat32 todB, wxFloat32* timeDeb, wxFloat32* timeEnd,const std::vector<wxFloat32>& timeTable,const std::vector<wxFloat32>& row_db )
{
	bool insideBound=false;
	if(row_db.size()==0)
		return;

	*timeDeb=timeTable[0];
	*timeEnd=timeTable[timeTable.size()-1];

	wxFloat32 lastdB=row_db[0];
	for(int idStep=0;idStep<row_db.size();idStep++)
	{
		if(!insideBound)
		{
			if(abs(row_db[idStep]-lastdB)>=fromdB)
			{
				*timeDeb=timeTable[idStep];
				insideBound=true;
			}
		}else{
			if(abs(row_db[idStep]-lastdB)>=todB)
			{
				*timeEnd=timeTable[idStep];
				break;
			}
		}
	}
}
/**
 * Régression linéaire par la méthode des moindres carré
 * Utilisé pour le calcul du TR et de l'EDT @see Compute_TR_Param()
 */
LinearRegressionResult ComputeLinearRegression( wxInt32 idBandeFreq,wxFloat32 fromTime,wxFloat32 toTime,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_db)
{
	wxInt32 n=0;
	wxFloat32 Sx=GetSumLimit(idBandeFreq,fromTime,toTime,timeTable,tab_db,SUM_OPERATION_X,&n);
	wxFloat32 Sy=GetSumLimit(idBandeFreq,fromTime,toTime,timeTable,tab_db,SUM_OPERATION_Y);
	wxFloat32 Sx2=GetSumLimit(idBandeFreq,fromTime,toTime,timeTable,tab_db,SUM_OPERATION_X2);
	wxFloat32 Sxy=GetSumLimit(idBandeFreq,fromTime,toTime,timeTable,tab_db,SUM_OPERATION_XY);

	wxFloat32 a=(n*Sxy-Sx*Sy)/(n*Sx2-(Sx*Sx));
	wxFloat32 b=(Sy*Sx2-Sx*Sxy)/(n*Sx2-(Sx*Sx));
	return LinearRegressionResult(a,b);
}
/*
 * Calcul du TR et de l'EDT selon la norme ISO 3382:1997(F)
 */
formatGABE::GABE_Data_Float* Compute_TR_Param(wxFloat32 fromdbL,wxFloat32 todbL,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj,const std::vector<std::vector<wxFloat32> >& tab_schroeder)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_schroeder.size()+1); //+1 for average

	//Pour chaque bande de fréquence
	wxFloat32 sum=0.f; 
	for(int idFreq=0;idFreq<tab_schroeder.size();idFreq++)
	{
		wxFloat32 debT=0;
		wxFloat32 endT=0;
		//Calcul des des temps en fonction de dbL

		GetTimeRange(fromdbL,todbL,&debT,&endT,timeTable,tab_schroeder[idFreq]);

		LinearRegressionResult regRes=ComputeLinearRegression(idFreq,debT,endT,timeTable,tab_schroeder);
		if(idFreq!=tab_schroeder.size()-1) //Ne pas moyenner sur la ligne de somme
			sum+=(-60.f/regRes.a)/1000.f;
		newParameter->Set(idFreq,(-60/regRes.a)/1000);
	}
	newParameter->Set(newParameter->GetSize()-1,sum/(tab_schroeder.size()-1));
	if(fromdbL==0)
	{
		newParameter->SetLabel(_("EDT (s)"));
		newParameter->headerData.numOfDigits=COMMA_PRECISION_TIME_S;
	}else{
		newParameter->SetLabel(wxString::Format(_("RT-%g (s)"),todbL-5));
		newParameter->headerData.numOfDigits=COMMA_PRECISION_TIME_S;
	}

	return newParameter;
}
formatGABE::GABE_Data_Float* dB_Sum_Param(const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size());

	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		//Calcul de 0 à +inf
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq,0.f,-1.f,timeTable,tab_wj);

		//Conversion en dB
		wxFloat32 db_CurrentFreq=to_deciBelP0(wj_CurrentFreq);

		newParameter->Set(idFreq,db_CurrentFreq);
	}

	newParameter->SetLabel(_("Sound level (dB)"));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_DB;
	return newParameter;

}
formatGABE::GABE_Data_Float* dBa_Sum_Param(const std::vector<wxFloat32>& timeTable,const std::vector<wxInt32>& freqTable,const std::vector<std::vector<wxFloat32> >& tab_wj)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size());
	std::vector<wxFloat32> ponderation=ApplicationConfiguration::GetAllPonderation();
	std::vector<wxInt32> allfreq=ApplicationConfiguration::GetAllFrequencies();
	wxFloat32 totdb=0;
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		//Calcul de 0 à +inf
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq,0.f,-1.f,timeTable,tab_wj);

		//Conversion en dB
		wxFloat32 db_CurrentFreq=to_deciBelP0(wj_CurrentFreq);

		//Conversion en dBA
		if(idFreq<freqTable.size())
		{
			for(int idf=0;idf<allfreq.size();idf++)
			{
				if(allfreq[idf]==freqTable[idFreq])
				{
					db_CurrentFreq+=ponderation[idf];
					break;
				}
			}
			totdb+=pow(10,db_CurrentFreq/10);
		}else{
			//Niveau global
			db_CurrentFreq=10*log10f(totdb);
		}

		newParameter->Set(idFreq,db_CurrentFreq);
	}

	newParameter->SetLabel(_("Sound level (dBA)"));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_DB;
	return newParameter;

}


/**
 * @brief Calcul du paramètre acoustique "support précoce" ST
 * $ST=10log\frac{\int_{t_2}^{t_3} g^2(t)dt}{\int_{0}^{t_1} g^2(t)dt}$
 * @param t1 En milliseconde. Voir la formule.
 * @param t2 En milliseconde. Voir la formule.
 * @param t3 En milliseconde. Voir la formule.
 */
formatGABE::GABE_Data_Float* Compute_ST_Param(wxFloat32 t1,wxFloat32 t2,wxFloat32 t3,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size()+1);


	//Pour chaque bande de fréquence
	wxFloat32 sum=0.f;
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(EPSILON, timeTable, tab_wj[idFreq]);
		double wt1 = wt0 + t1;
		double wt2 = wt0 + t2;
		double wt3 = wt0 + t3;

		//Calcul de 0|te(p²(t))/te|+inf(p²(t))
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq, wt2, wt3,timeTable,tab_wj)/GetSumLimit(idFreq, wt0, wt1,timeTable,tab_wj);

		//Conversion en dB
		wxFloat32 db_CurrentFreq=to_deciBel(wj_CurrentFreq);
		
		if(idFreq!=tab_wj.size()-1)
			sum+=db_CurrentFreq;
		newParameter->Set(idFreq,db_CurrentFreq);
	}
	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(_("ST (dB)"));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_DB;
	return newParameter;
}
/**
 * @brief Calcul du paramètre acoustique "clarté" C
 * $C_{t_e}=10log\frac{\int_0^{t_e} g^2(t)dt}{\int_{t_e}^{\infty} g^2(t)dt}$
 * @param te En milliseconde. Voir la formule.
 */
formatGABE::GABE_Data_Float* Compute_C_Param(wxFloat32 te,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size()+1);

	
	wxFloat32 sum=0.f;
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(EPSILON, timeTable, tab_wj[idFreq]);
		double wte = wt0 + te;

		//Calcul de 0|te(p²(t))/te|+inf(p²(t))
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq,wt0,wte,timeTable,tab_wj)/GetSumLimit(idFreq,wte,-1.f,timeTable,tab_wj);

		//Conversion en dB
		wxFloat32 db_CurrentFreq=to_deciBel(wj_CurrentFreq);

		if(idFreq!=tab_wj.size()-1)
			sum+=db_CurrentFreq;
		newParameter->Set(idFreq,db_CurrentFreq);
	}

	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(wxString::Format(_("C-%g (dB)"),te));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_DB;
	return newParameter;
}
/**
 * @brief Calcul du paramètre acoustique "Définition" D
 * $C_{t_e}=100\times\frac{\int_0^{t_e} g^2(t)dt}{\int_{0}^{\infty} g^2(t)dt}$
 * @param te En milliseconde. Voir la formule.
 */
formatGABE::GABE_Data_Float* Compute_D_Param(wxFloat32 te,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size()+1); //+1 pour le calcul toute bande

	wxFloat32 sum=0.f;
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(EPSILON, timeTable, tab_wj[idFreq]);
		double wte = wt0 + te;

		//Calcul de 0|te(p²(t))/te|+inf(p²(t))
		wxFloat32 wj_CurrentFreq=(GetSumLimit(idFreq,wt0,wte,timeTable,tab_wj)/GetSumLimit(idFreq,wt0,-1.f,timeTable,tab_wj))*100.f;

		if(idFreq!=tab_wj.size()-1)
			sum+=wj_CurrentFreq;
		newParameter->Set(idFreq,wj_CurrentFreq);
	}

	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(wxString::Format(_("D-%g (%%)"),te));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_PERCENT;
	return newParameter;
}

/**
 * @brief Calcul du paramètre acoustique "temps central" $t_s$
 * $C_{t_e}=\frac{\int_0^{\infty} t\times g^2(t)dt}{\int_{0^{\infty} g^2(t)dt}$
 */
formatGABE::GABE_Data_Float* Compute_TS_Param(const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size()+1); //+1 pour le calcul toute bande

	wxFloat32 sum=0.f;
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(EPSILON, timeTable, tab_wj[idFreq]);

		//Calcul de 0|t(p²(t))/0|+inf(p²(t))
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq,wt0,-1.f,timeTable,tab_wj,SUM_OPERATION_XY)/GetSumLimit(idFreq,wt0,-1.f,timeTable,tab_wj,SUM_OPERATION_Y);

		if(idFreq!=tab_wj.size()-1)
			sum+=wj_CurrentFreq;
		newParameter->Set(idFreq,wj_CurrentFreq);
	}

	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(_("Ts (ms)"));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_TIME_MS;
	return newParameter;
}


formatGABE::GABE_Data_Float* Compute_LEF_Param(const wxFloat32& t1,const wxFloat32& t2,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj,const std::vector<std::vector<wxFloat32> >& tab_wjTheta)
{
	using namespace formatGABE;
	GABE_Data_Float* newParameter=new GABE_Data_Float(tab_wj.size()+1); //autant de lignes de que de bande de fréquence

	wxFloat32 sum=0.f;
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(EPSILON, timeTable, tab_wj[idFreq]);
		double wt1 = wt0 + t1;
		double wt2 = wt0 + t2;

		//Calcul
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq,wt1,wt2,timeTable,tab_wjTheta,SUM_OPERATION_Y)/GetSumLimit(idFreq,wt0,wt2,timeTable,tab_wj,SUM_OPERATION_Y)*100.f;
		
		if(idFreq!=tab_wj.size()-1)
			sum+=wj_CurrentFreq;
		newParameter->Set(idFreq,wj_CurrentFreq);
	}
	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	return newParameter;
}


/**
 * @brief Calcul du paramètre acoustique LF "Fraction d'énérgie latérale précoce"
 *
 * $LF_{t_{1}}^{t_{2}}=100\times\frac{\int_{t_{1}}^{t_{2}}\left[g^{2}\left(t\right).cos^{2}\theta\right]dt}{\int_{0}^{t_{2}}g\left(t\right)^{2}dt}$
 *
 * @param t1 En milliseconde. Voir la formule.
 * @param t2 En milliseconde. Voir la formule.
 * @return Paramètre LF
 */
formatGABE::GABE_Data_Float* Compute_LF_Param(const wxFloat32& t1,const wxFloat32& t2,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj,const std::vector<std::vector<wxFloat32> >& tab_wjTheta)
{
	using namespace formatGABE;
	GABE_Data_Float* newParameter=Compute_LEF_Param(t1,t2,timeTable,tab_wj,tab_wjTheta);
	newParameter->SetLabel(wxString::Format(_("LF%i"),(int)t2));
	return newParameter;
}

/**
 * @brief Calcul du paramètre acoustique LFC "Fraction d'énérgie latérale précoce"
 *
 * $LFC_{t_{1}}^{t_{2}}=100\times\frac{\int_{t_{1}}^{t_{2}}g^{2}\left(t\right)\mid cos\theta\mid dt}{\int_{0}^{t_{2}}g^{2}\left(t\right)dt}$
 *
 * ISO 3382 cf - Kleiner AA 27 p321. (1989)
 * @param t1 En milliseconde. Voir la formule.
 * @param t2 En milliseconde. Voir la formule.
 * @return Paramètre LFC
 */
formatGABE::GABE_Data_Float* Compute_LFC_Param(const wxFloat32& t1,const wxFloat32& t2,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj,const std::vector<std::vector<wxFloat32> >& tab_wjTheta)
{
	using namespace formatGABE;
	GABE_Data_Float* newParameter=Compute_LEF_Param(t1,t2,timeTable,tab_wj,tab_wjTheta);
	newParameter->SetLabel(wxString::Format(_("LFC%i"),(int)t2));
	return newParameter;
}


/**
 * @brief Calcul du paramètre acoustique LG ou $L_j$ "Niveau sonore de l'énergie latérale tardive"
 *
 * $LG_{t_{1}}^{t_{2}}=10log_{10}\frac{\int_{t_{1}}^{t_{2}}g^{2}\left(t\right)cos^{2}\theta dt}{\sum\frac{wi}{4\pi*100}}$
 * @param t1 En milliseconde. Voir la formule.
 * @param t2 En milliseconde. Voir la formule.
 * @return Paramètre LFC
 */
formatGABE::GABE_Data_Float* Compute_LG_Param(const wxFloat32& t1,const wxFloat32& t2,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wjthetasqr,const std::vector<wxFloat32>& tab_wjsrc)
{
	using namespace formatGABE;

	GABE_Data_Float* newParameter=new GABE_Data_Float(tab_wjthetasqr.size()+1); //autant de lignes de que de bande de fréquence

	//Pour chaque bande de fréquence
	wxFloat32 sum=0.f;
	for(int idFreq=0;idFreq<tab_wjthetasqr.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(EPSILON, timeTable, tab_wjthetasqr[idFreq]);
		double wt1 = wt0 + t1;
		double wt2 = wt0 + t2;
		//Calcul
		wxFloat32 wj_CurrentFreq=(GetSumLimit(idFreq,wt1,wt2,timeTable,tab_wjthetasqr,SUM_OPERATION_Y))/(tab_wjsrc[idFreq]/(4*M_PI*100));
		
		if(idFreq!=tab_wjthetasqr.size()-1)
			sum+=wj_CurrentFreq;
		newParameter->Set(idFreq,10*log10f(wj_CurrentFreq));
	}
	newParameter->Set(newParameter->GetSize()-1,10*log10f(sum/(tab_wjthetasqr.size()-1)));
	newParameter->SetLabel(_("LG (dB)"));
	return newParameter;
}

/**
 * @brief Calcul du paramètre acoustique G "Tenu acoustique" ou "force sonore"
 *
 * $G_{t_{1}}^{t_{2}}=10log_{10}\frac{\int_{t_{1}}^{t_{2}}g^{2}\left(t\right)dt}{\sum\frac{wi}{4\pi*100}}$
 * @param t1 En milliseconde. Voir la formule.
 * @param t2 En milliseconde. Voir la formule.
 * @return Paramètre LFC
 */
formatGABE::GABE_Data_Float* Compute_G_Param(const wxFloat32& t1,const wxFloat32& t2,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj,const std::vector<wxFloat32>& tab_wjsrc)
{
	using namespace formatGABE;

	GABE_Data_Float* newParameter=new GABE_Data_Float(tab_wj.size()+1); //autant de lignes de que de bande de fréquence

	//Pour chaque bande de fréquence
	wxFloat32 sum=0.f;
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(EPSILON, timeTable, tab_wj[idFreq]);
		double wt1 = wt0 + t1;
		double wt2 = wt0 + t2;
		//Calcul
		wxFloat32 wj_CurrentFreq=(GetSumLimit(idFreq,wt1,wt2,timeTable,tab_wj,SUM_OPERATION_Y))/(tab_wjsrc[idFreq]/(4*M_PI*100));
		newParameter->Set(idFreq,to_deciBel(wj_CurrentFreq));
		if(idFreq!=tab_wj.size()-1)
			sum+=to_deciBel(wj_CurrentFreq);
	}
	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(_("G (dB)"));
	return newParameter;
}

void ProjectManager::OnMenuDoAcousticParametersComputation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* selectedEl)
{
	//On demande à l'utilisateur d'entrer les paramètres acoustiques
	bool do_computation=true;

	std::vector<wxFloat32> CParams;
	std::vector<wxFloat32> DParams;
	std::vector<wxFloat32> TRParams;
	wxString Cdefault="50;80";
	wxString Ddefault="50";
	wxString TRdefault="15;30";
	if(!pyeventmode)
	{
		wxCustomEntryDialog parametersDialog(mainFrame,_("Please, gives calculation parameters. Use ';' to separate several parameters"),_("Acoustic parameters calculation"));
		parametersDialog.AddTextControl(_("Clarity (ms)"),Cdefault);
		parametersDialog.AddTextControl(_("Definition (ms)"),Ddefault);
		parametersDialog.AddTextControl(_("Sound decay value for RT calculation (dB)"),TRdefault);

		if (parametersDialog.ShowModal() == wxID_OK)
		{

			std::vector<wxString> valeursChamps;
			parametersDialog.GetValues(valeursChamps);
			Cdefault=valeursChamps[0];
			Ddefault=valeursChamps[1];
			TRdefault=valeursChamps[2];
		}else{
			do_computation=false;
		}
	}else{
		//Si existant, lecture du paramètre fournie
		this->GetParameter("C",Cdefault);
		this->GetParameter("D",Ddefault);
		this->GetParameter("TR",TRdefault);
	}
	if(do_computation)
	{
		SplitString(Cdefault,";",CParams);
		SplitString(Ddefault,";",DParams);
		SplitString(TRdefault,";",TRParams);
		E_Report_Gabe* reportEle=dynamic_cast<E_Report_Gabe*>(selectedEl);
		E_Report_File* reportFolder=dynamic_cast<E_Report_File*>(selectedEl->GetElementParent());
		if(reportEle && reportFolder)
		{
			wxString fileName;
			reportEle->BuildFullPath(fileName);
			wxString fileFolder;
			reportFolder->BuildFullPath(fileFolder);

			if(!wxFileExists(fileName))
			{
				wxLogError(_("The following file doesn't exist:\n%s"),fileName);
				return;
			}
			////////////////////////////////////
			//Une colonne par bande de fréquence
			using namespace formatGABE;
			GABE tabReader(fileName);
			wxInt32 nbBandeFreq=tabReader.GetCols()-1;
			////////////////////////////////////
			//On récupere la liste des t (ms)
			GABE_Data_ShortString* dataLbl;
			tabReader.GetCol(0,&dataLbl);
			wxInt32 nbTimeStep=dataLbl->GetSize();

			std::vector<wxFloat32> timeTable(nbTimeStep);
			for(int idstep=0;idstep<nbTimeStep;idstep++)
			{
				wxString timeStepVal(dataLbl->GetStringEquiv(idstep));
				timeTable[idstep]=Convertor::ToFloat(timeStepVal.Left(timeStepVal.find(" ")));
			}

			/////////////////////////////////////
			// Création du tableau tab_wj[idBandeFrequence][idstep]
			// Création du tableau schroeder[idBandeFrequence][idstep]
			std::vector<std::vector<wxFloat32> > tab_wj(nbBandeFreq+1);
			std::vector<std::vector<wxFloat32> > tab_schroeder;

			tab_wj[nbBandeFreq]=std::vector<wxFloat32>(nbTimeStep);

			GABE_Data_ShortString* rowLbls=new GABE_Data_ShortString(nbBandeFreq+2);
			std::vector<wxInt32> freqTab(nbBandeFreq);
			//Transfert des données du fichier vers les tableaux
			for(int idbFreq=1;idbFreq<=nbBandeFreq;idbFreq++)
			{
				GABE_Data_Float* dataFloat;
				tabReader.GetCol(idbFreq,&dataFloat);
				rowLbls->SetString(idbFreq-1,dataFloat->GetLabel());
				freqTab[idbFreq-1]=Convertor::ToInt(dataFloat->GetLabel());
				tab_wj[idbFreq-1]=std::vector<wxFloat32>(nbTimeStep);

				for(int idstep=0;idstep<nbTimeStep;idstep++)
				{
					tab_wj[idbFreq-1][idstep]=*(*dataFloat)[idstep];
					tab_wj[nbBandeFreq][idstep]+=*(*dataFloat)[idstep];
				}
			}
			rowLbls->SetString(nbBandeFreq,_("Global"));
			rowLbls->SetString(nbBandeFreq+1,_("Average"));

			MakeSchroederArray(tab_wj,tab_schroeder);

			//////////////////////////////////////////
			// Création du tableau des paramètres acoustiques
			std::vector<GABE_Data_Float*> tabParameters;

			//////////////////////////////////////////
			// Calcul du niveau sonore cumulé (db et dbA) sur les pas de temps

			tabParameters.push_back(dB_Sum_Param(timeTable,tab_wj));

			tabParameters.push_back(dBa_Sum_Param(timeTable,freqTab,tab_wj));
			//////////////////////////////////////////
			// Calcul de la clarté C
			for(int idParam=0;idParam<CParams.size();idParam++)
				tabParameters.push_back(Compute_C_Param(CParams[idParam],timeTable,tab_wj));
			//////////////////////////////////////////
			// Calcul de la définition D
			for(int idParam=0;idParam<DParams.size();idParam++)
				tabParameters.push_back(Compute_D_Param(DParams[idParam],timeTable,tab_wj));
			//////////////////////////////////////////
			// Calcul du temps central TS
			tabParameters.push_back(Compute_TS_Param(timeTable,tab_wj));


			//////////////////////////////////////////
			// Calcul du temps de réponse TR
			for(int idParam=0;idParam<TRParams.size();idParam++)
				tabParameters.push_back(Compute_TR_Param(5,TRParams[idParam]+5,timeTable,tab_wj,tab_schroeder));


			//////////////////////////////////////////
			// Calcul de la durée de réverberation initiale EDT
			tabParameters.push_back(Compute_TR_Param(0,10,timeTable,tab_wj,tab_schroeder));

            /////////////////////////////////////////
            // Calcul paramètre ST
            /////////////////////////////////////////
            tabParameters.push_back(Compute_ST_Param(10,20,100,timeTable,tab_wj));


			//////////////////////////////////////////
			// Enregistrement des données des paramètres acoustique

			GABE tabWriter(tabParameters.size()+1);
			tabWriter.LockData(); //lecture seule pour l'utilisateur
			tabWriter.SetCol(0,rowLbls);
			for(int idparameter=0;idparameter<tabParameters.size();idparameter++)
				tabWriter.SetCol(idparameter+1,tabParameters[idparameter]);


			tabWriter.Save(fileFolder+wxString(wxTRANSLATE("Acoustic parameters"))+wxString(".gabe"));

			//////////////////////////////////////////
			// Enregistrement des données des courbes de shroeder
			//Pour chaque pas de temps on créé un enregistrement
			
			GABE_Data_ShortString* cpRowLbls = new GABE_Data_ShortString(nbBandeFreq + 1);
			//Transfert des données du fichier vers les tableaux
			for (int idbFreq = 1; idbFreq <= nbBandeFreq; idbFreq++)
			{
				GABE_Data_Float* dataFloat;
				tabReader.GetCol(idbFreq, &dataFloat);
				cpRowLbls->SetString(idbFreq - 1, dataFloat->GetLabel());
			}
			cpRowLbls->SetString(nbBandeFreq, _("Global"));
			GABE tabSchroederWriter(nbTimeStep+1);
			tabSchroederWriter.LockData(); //lecture seule pour l'utilisateur
			tabSchroederWriter.SetCol(0,cpRowLbls);
			for(int idstep=0;idstep<nbTimeStep;idstep++)
			{
				GABE_Data_Float* colTimeStep=new GABE_Data_Float(nbBandeFreq+1);
				colTimeStep->SetLabel(dataLbl->GetStringEquiv(idstep));
				colTimeStep->headerData.numOfDigits= COMMA_PRECISION_DB;
				for(int idFreq=0;idFreq<tab_schroeder.size();idFreq++)
				{
					colTimeStep->Set(idFreq,tab_schroeder[idFreq][idstep]);
				}
				tabSchroederWriter.SetCol(idstep+1,colTimeStep);
			}
			tabSchroederWriter.Save(fileFolder+wxString(wxTRANSLATE("Schroeder curves"))+wxString(".gabe"));


			reportFolder->RefreshFolderContents();
			reportFolder->FillWxTree(fromCtrl);
		}

	}
}


void ProjectManager::OnMenuRecepteurSurfDoAcousticParametersComputation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* selectedEl,const wxString& parametertype)
{
	E_Report_File* reportEle=dynamic_cast<E_Report_File*>(selectedEl);
	if(reportEle)
	{
		wxString filePath;
		reportEle->BuildFullPath(filePath);

		using namespace formatRSBIN;
		using namespace formatGABE;

		t_ExchangeData mainData;

		RSBIN importer;
		//////////////////////////////////////////////////////////
		//Importation du fichier de recepteurs surfaciques

		if(importer.ImportBIN(filePath,mainData))
		{
			std::vector<wxFloat32> timeTable(mainData.nbTimeStep);
			for(int idstep=0;idstep<mainData.nbTimeStep;idstep++)
			{
				timeTable[idstep]=mainData.timeStep*(idstep+1)*1000;
			}
			for(wxInt32 idrs=0;idrs<mainData.tabRsSize;idrs++)
			{
				//Alimentation du tableau wj
				std::vector<std::vector<wxFloat32> > tab_wj(mainData.tabRs[idrs].dataRec.quantFaces);
				for(wxInt32 idface=0;idface<mainData.tabRs[idrs].dataRec.quantFaces;idface++)
				{
					tab_wj[idface]=std::vector<wxFloat32>(mainData.nbTimeStep,0);
					//Pour chaque enregistrement de cette face
					for(wxInt32 idenr=0;idenr<mainData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords;idenr++)
					{
						t_faceValue* faceval=&mainData.tabRs[idrs].dataFaces[idface].tabTimeStep[idenr];
						tab_wj[idface][faceval->timeStep]=faceval->energy;
					}
				}
				//Le tableau w_j est prêt
				//Il faut maintenant calculer l'equivalence avec la méthode de shroeder
				std::vector<std::vector<wxFloat32> > schroederArray;
				MakeSchroederArray(tab_wj,schroederArray,&to_deciBelRsurf);
				GABE_Data_Float* rsParams;
				//TODO tout les paramètres
				if( parametertype=="TR15")
				{
					//Calcul du TR 15 pour chaque face
					rsParams=Compute_TR_Param(5,20,timeTable,tab_wj,schroederArray);
				}else if(parametertype=="TR30"){
					//Calcul du TR 30 pour chaque face
					rsParams=Compute_TR_Param(5,35,timeTable,tab_wj,schroederArray);
				}else if(parametertype=="EDT"){
					//Calcul du EDT pour chaque face
					rsParams=Compute_TR_Param(0,10,timeTable,tab_wj,schroederArray);
				}else if(parametertype=="C50"){
					//Calcul du C50 pour chaque face
					rsParams=Compute_C_Param(50,timeTable,tab_wj);
				}else if(parametertype=="C80"){
					//Calcul du C80 pour chaque face
					rsParams=Compute_C_Param(80,timeTable,tab_wj);
				}else if(parametertype=="D50"){
					//Calcul du D50 pour chaque face
					rsParams=Compute_D_Param(50,timeTable,tab_wj);
				}else if(parametertype=="TS"){
					//Calcul du TS pour chaque face
					rsParams=Compute_TS_Param(timeTable,tab_wj);
				}else if(parametertype=="ST"){
					//Calcul du TS pour chaque face
					rsParams=Compute_ST_Param(10,20,100,timeTable,tab_wj);
				}
				//Affectation des valeurs
				for(wxInt32 idface=0;idface<mainData.tabRs[idrs].dataRec.quantFaces;idface++)
				{
					delete[] mainData.tabRs[idrs].dataFaces[idface].tabTimeStep;
					mainData.tabRs[idrs].dataFaces[idface].tabTimeStep = new t_faceValue[1];
					mainData.tabRs[idrs].dataFaces[idface].tabTimeStep[0]=t_faceValue(0,*(*rsParams)[idface]);
					mainData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords=1;
				}

				//Suppression des données temporaires
				delete rsParams;
			}
			//Modification du type de fichier et des informations relative pour créer un fichier de cartographie du tr
			mainData.nbTimeStep=1;
			if( parametertype=="TR15" || parametertype=="TR30")
			{
				mainData.recordType=RECEPTEURS_RECORD_TYPE_TR;
			}else if( parametertype=="EDT"){
				mainData.recordType=RECEPTEURS_RECORD_TYPE_EDT;
			}else if( parametertype=="C50" || parametertype=="C80"){
				mainData.recordType=RECEPTEURS_RECORD_TYPE_CLARITY;
			}else if( parametertype=="D50"){
				mainData.recordType=RECEPTEURS_RECORD_TYPE_DEFINITION;
			}else if( parametertype=="TS"){
				mainData.recordType=RECEPTEURS_RECORD_TYPE_TS;
			}else if( parametertype=="ST"){
				mainData.recordType=RECEPTEURS_RECORD_TYPE_ST;
			}
			wxFileName nouvFich(filePath);
			nouvFich.SetName(parametertype);
			importer.ExportBIN(nouvFich.GetFullPath(),mainData);

			this->RefreshReportFolder();
		}
	}
}

void ProjectManager::OnMenuDoAdvancedAcousticParametersComputation(Element* selectedElement)
{
	bool do_computation=true;
	E_Report_File* gabeFile=dynamic_cast<E_Report_File*>(selectedElement);
	E_Report_File* gabeFolder=dynamic_cast<E_Report_File*>(selectedElement->GetElementParent());
	if(!gabeFile || !gabeFolder)
		return;
	wxString gabePath;
	wxString fileFolder;
	gabeFile->BuildFullPath(gabePath);
	gabeFolder->BuildFullPath(fileFolder);
	if(!wxFileExists(gabePath))
		return;
	using namespace formatGABE;
	GABE gReader(gabePath);
	if(gReader.GetCols()==0)
		return;

	formatGABE::GABE_Data_Integer* indexCol;
	formatGABE::GABE_Data_Integer* lstFreq;
	formatGABE::GABE_Data_Float* lstFloatParam;
	formatGABE::GABE_Data_Float* lstSrcEnergy;
	if(!gReader.GetCol(0,&indexCol))
		return;
	if(!gReader.GetCol(indexCol->GetValue(2),&lstFreq))
		return;
	if(!gReader.GetCol(indexCol->GetValue(0),&lstFloatParam))
		return;
	if(!gReader.GetCol(indexCol->GetValue(1),&lstSrcEnergy))
		return;

	int nbBandeFreq=indexCol->GetValue(5);
	int nbtimeStep=indexCol->GetValue(7);
	int nbsubfreq=3;
	float timeStep=lstFloatParam->GetValue(0)*1000; //Recuperation de deltaT
	float firstTimeStep=lstFloatParam->GetValue(1)*1000; //Recuperation de t0

	std::vector<wxFloat32> LFParams;
	std::vector<wxFloat32> LFCParams;
	std::vector<wxFloat32> LGParams;
	std::vector<wxFloat32> GParams;
	wxFloat32 lasttimestep=firstTimeStep+(nbtimeStep-1)*timeStep;
	wxString LFParam("80"),LFCParam("80"),LGParam(wxString::Format("%i",(int)lasttimestep)),GParam(wxString::Format("%i",(int)lasttimestep));

	if(!pyeventmode)
	{
		wxCustomEntryDialog parametersDialog(mainFrame,_("Please, gives calculation parameters. Use ';' to separate several parameters"),_("Acoustic parameters calculation"));
		parametersDialog.AddTextControl(_("Early lateral fraction LF (ms)"),LFParam);
		parametersDialog.AddTextControl(_("Early lateral fraction LFC (ms)"),LFCParam);
		//parametersDialog.AddTextControl(_("Tenue acoustique latérale LG(ms)"),LGParam);
		//parametersDialog.AddTextControl(_("Tenue acoustique G (ms)"),GParam);



		if (parametersDialog.ShowModal() == wxID_OK)
		{
			std::vector<wxString> valeursChamps;
			parametersDialog.GetValues(valeursChamps);

			LFParam=valeursChamps[0];
			LFCParam=valeursChamps[1];
			//LGParam=valeursChamps[2];
			//GParam=valeursChamps[3];
		}else{
			do_computation=false;
		}
	}else{
		this->GetParameter("LF",LFParam);
		this->GetParameter("LFC",LFCParam);
		this->GetParameter("LG",LGParam);
		this->GetParameter("G",GParam);
	}

	if(do_computation)
	{
		SplitString(LFParam,";",LFParams);
		SplitString(LFCParam,";",LFCParams);
		SplitString(LGParam,";",LGParams);
		SplitString(GParam,";",GParams);

		/////////////////////////////////////////
		// Construction time_table
		/////////////////////////////////////////
		std::vector<wxFloat32> timeTable(nbtimeStep);
		for(int idstep=0;idstep<nbtimeStep;idstep++)
		{
			timeTable[idstep]=firstTimeStep+idstep*timeStep;
		}

		/////////////////////////////////////////
		// Construction tab_wj_source
		/////////////////////////////////////////
		std::vector<wxFloat32> tab_wj_source(nbBandeFreq+1,0); //+1 pour le cumul sur les bandes de fréquences
		float cumul=0;
		for(int idfreq=0;idfreq<nbBandeFreq;idfreq++)
		{
			tab_wj_source[idfreq]=lstSrcEnergy->GetValue(idfreq);
			cumul+=lstSrcEnergy->GetValue(idfreq);
		}
		tab_wj_source[nbBandeFreq]=cumul;
		/////////////////////////////////////////
		// Construction tab_wj
		/////////////////////////////////////////
		std::vector<std::vector<wxFloat32> > tab_wj(nbBandeFreq+1,std::vector<wxFloat32>(nbtimeStep,0)); //+1 pour le cumul sur les bandes de fréquences
		/////////////////////////////////////////
		// Construction tab_wj_phi
		/////////////////////////////////////////
		std::vector<std::vector<wxFloat32> > tab_wj_phi(nbBandeFreq+1,std::vector<wxFloat32>(nbtimeStep,0)); //+1 pour le cumul sur les bandes de fréquences
		/////////////////////////////////////////
		// Construction tab_wj_phi_sqr
		/////////////////////////////////////////
		std::vector<std::vector<wxFloat32> > tab_wj_phi_sqr(nbBandeFreq+1,std::vector<wxFloat32>(nbtimeStep,0)); //+1 pour le cumul sur les bandes de fréquences
		/////////////////////////////////////////
		// Affectaction des valeurs tab_wj,tab_wj_phi et tab_wj_phi_sqr
		/////////////////////////////////////////
		int currentIndexCol=indexCol->GetValue(4);
		int colWpStep=indexCol->GetValue(6);
		int idrow=0;

		for(int idfreq=0;idfreq<nbBandeFreq;idfreq++)
		{
			for(int idsubcol=0;idsubcol<nbsubfreq;idsubcol++)
			{
				formatGABE::GABE_Data_Float* colWpVal;
				if(!gReader.GetCol(currentIndexCol+idsubcol,&colWpVal))
					return;
				if(idsubcol==0)
				{
					for(int idstep=0;idstep<nbtimeStep;idstep++)
					{
						tab_wj[idfreq][idstep]=colWpVal->GetValue(idstep);
						tab_wj[nbBandeFreq][idstep]+=colWpVal->GetValue(idstep);
					}
				}else if(idsubcol==1)
				{
					for(int idstep=0;idstep<nbtimeStep;idstep++)
					{
						tab_wj_phi[idfreq][idstep]=colWpVal->GetValue(idstep);
						tab_wj_phi[nbBandeFreq][idstep]+=colWpVal->GetValue(idstep);
					}
				}else{
					for(int idstep=0;idstep<nbtimeStep;idstep++)
					{
						tab_wj_phi_sqr[idfreq][idstep]=colWpVal->GetValue(idstep);
						tab_wj_phi_sqr[nbBandeFreq][idstep]+=colWpVal->GetValue(idstep);
					}
				}
			}
			currentIndexCol+=colWpStep;
		}


		//////////////////////////////////////////
		// Création du tableau des paramètres acoustiques
		std::vector<GABE_Object*> tabToExport;

		/////////////////////////////////////////
		// Affectaction des valeurs des libellés
		/////////////////////////////////////////
		GABE_Data_ShortString* cpRowLbls= new GABE_Data_ShortString(nbBandeFreq+2);
		for(int idfreq=0;idfreq<nbBandeFreq;idfreq++)
		{
			cpRowLbls->SetString(idfreq,(wxString(lstFreq->GetStringEquiv(idfreq))+_(" Hz")).c_str());
		}
		cpRowLbls->SetString(nbBandeFreq,_("Global"));
		cpRowLbls->SetString(nbBandeFreq+1,_("Average"));
		tabToExport.push_back(cpRowLbls);

		/////////////////////////////////////////
		// Calcul paramètre LF
		/////////////////////////////////////////
		for(int idParam=0;idParam<LFParams.size();idParam++)
			tabToExport.push_back(Compute_LF_Param(5,LFParams[idParam],timeTable,tab_wj,tab_wj_phi_sqr));
		/////////////////////////////////////////
		// Calcul paramètre LFC
		/////////////////////////////////////////
		for(int idParam=0;idParam<LFCParams.size();idParam++)
			tabToExport.push_back(Compute_LFC_Param(5,LFCParams[idParam],timeTable,tab_wj,tab_wj_phi));
		/////////////////////////////////////////
		// Calcul paramètre LG
		/////////////////////////////////////////
		for(int idParam=0;idParam<LGParams.size();idParam++)
			tabToExport.push_back(Compute_LG_Param(0,LGParams[idParam],timeTable,tab_wj_phi_sqr,tab_wj_source));
		/////////////////////////////////////////
		// Calcul paramètre G
		/////////////////////////////////////////
		for(int idParam=0;idParam<GParams.size();idParam++)
			tabToExport.push_back(Compute_G_Param(0,GParams[idParam],timeTable,tab_wj,tab_wj_source));




		/////////////////////////////////////////
		// Enregistrement des calculs
		/////////////////////////////////////////
		GABE tabWriter(tabToExport.size());
		tabWriter.LockData(); //lecture seule pour l'utilisateur
		for(int idparameter=0;idparameter<tabToExport.size();idparameter++)
			tabWriter.SetCol(idparameter,tabToExport[idparameter]);
		tabWriter.Save(fileFolder+wxString(wxTRANSLATE("Advanced acoustic parameters"))+wxString(".gabe"));
		gabeFolder->RefreshFolderContents();
		gabeFolder->FillWxTree(treeResult);
		}
}
