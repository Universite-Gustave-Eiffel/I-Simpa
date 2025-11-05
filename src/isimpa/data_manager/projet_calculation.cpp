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
#include <iostream>
#include "nc_curves.cpp"

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

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

std::vector<wxFloat32> GetNCCurve(wxFloat32 NCidx,const std::vector<wxInt32>& freqTable)
{
	std::vector<wxFloat32> NCVal;
	//uzupelnic wartosciami krzwych nc
	return NCVal;
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
				sumJ += tab_wj[idBandeFreq][idStep];
			else if(operation==SUM_OPERATION_XY)
				sumJ += (tab_wj[idBandeFreq][idStep] * currentTime);
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
	for (int idFreq = 0; idFreq < tab_schroeder.size(); idFreq++)
	{
		wxFloat32 debT = 0;
		wxFloat32 endT = 0;
		//Calcul des des temps en fonction de dbL

		GetTimeRange(fromdbL, todbL, &debT, &endT, timeTable, tab_schroeder[idFreq]);
	
		LinearRegressionResult regRes=ComputeLinearRegression(idFreq,debT,endT,timeTable,tab_schroeder);
		if(idFreq!=tab_schroeder.size()-1) //Ne pas moyenner sur la ligne de somme
			sum+=(-60.f/regRes.a)/1000.f;
		newParameter->Set(idFreq,(-60/regRes.a)/1000);
	}
	newParameter->Set(newParameter->GetSize()-1,sum/(tab_schroeder.size()-1));
	if(fromdbL==0)
	{
		newParameter->SetLabel(wxGetTranslation("EDT (s)"));
		newParameter->headerData.numOfDigits=COMMA_PRECISION_TIME_S;
	}else{
		newParameter->SetLabel(wxString::Format(wxGetTranslation("RT-%g (s)"),todbL-5));
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

	newParameter->SetLabel(wxGetTranslation("Sound level (dB)"));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_DB;
	return newParameter;

}

formatGABE::GABE_Data_Float* dB_Sum_Param_surf(const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj)
{
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size());

	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		//Calcul de 0 à +inf
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq,0.f,-1.f,timeTable,tab_wj);

		//Conversion en dB
		wxFloat32 db_CurrentFreq=to_deciBelRsurf(wj_CurrentFreq);

		newParameter->Set(idFreq,db_CurrentFreq);
	}

	newParameter->SetLabel(wxGetTranslation("Sound level (dB)"));
	newParameter->headerData.numOfDigits=3;
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

	newParameter->SetLabel(wxGetTranslation("Sound level (dBA)"));
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
	// First sound wave is valided if superior than this value
	float refValue = pow(10, -180.0f / 10.0f);
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(refValue, timeTable, tab_wj[idFreq]);
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
	newParameter->SetLabel(wxGetTranslation("ST (dB)"));
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
	// First sound wave is valided if superior than this value
	float refValue = pow(10, -180.0f / 10.0f);
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(refValue, timeTable, tab_wj[idFreq]);
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
	newParameter->SetLabel(wxString::Format(wxGetTranslation("C-%g (dB)"),te));
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
	// First sound wave is valided if superior than this value
	float refValue = pow(10, -180.0f / 10.0f);
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(refValue, timeTable, tab_wj[idFreq]);
		double wte = wt0 + te;

		//Calcul de 0|te(p²(t))/te|+inf(p²(t))
		wxFloat32 wj_CurrentFreq=(GetSumLimit(idFreq,wt0,wte,timeTable,tab_wj)/GetSumLimit(idFreq,wt0,-1.f,timeTable,tab_wj))*100.f;

		if(idFreq!=tab_wj.size()-1)
			sum+=wj_CurrentFreq;
		newParameter->Set(idFreq,wj_CurrentFreq);
	}

	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(wxString::Format(wxGetTranslation("D-%g (%%)"),te));
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
	// First sound wave is valided if superior than this value
	float refValue = pow(10, -180.0f / 10.0f);
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(refValue, timeTable, tab_wj[idFreq]);

		//Calcul de 0|t(p²(t))/0|+inf(p²(t))
		wxFloat32 wj_CurrentFreq=GetSumLimit(idFreq,wt0,-1.f,timeTable,tab_wj,SUM_OPERATION_XY)/GetSumLimit(idFreq,wt0,-1.f,timeTable,tab_wj,SUM_OPERATION_Y);

		if(idFreq!=tab_wj.size()-1)
			sum+=wj_CurrentFreq;
		newParameter->Set(idFreq,wj_CurrentFreq);
	}

	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(wxGetTranslation("Ts (ms)"));
	newParameter->headerData.numOfDigits= COMMA_PRECISION_TIME_MS;
	return newParameter;
}


formatGABE::GABE_Data_Float* Compute_LEF_Param(const wxFloat32& t1,const wxFloat32& t2,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj,const std::vector<std::vector<wxFloat32> >& tab_wjTheta)
{
	using namespace formatGABE;
	GABE_Data_Float* newParameter=new GABE_Data_Float(tab_wj.size()+1); //autant de lignes de que de bande de fréquence

	wxFloat32 sum=0.f;
	// First sound wave is valided if superior than this value
	float refValue = pow(10, -180.0f / 10.0f);
	//Pour chaque bande de fréquence
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(refValue, timeTable, tab_wj[idFreq]);
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
	newParameter->SetLabel(wxString::Format(wxGetTranslation("LF%i"),(int)t2));
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
	newParameter->SetLabel(wxString::Format(wxGetTranslation("LFC%i"),(int)t2));
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
	// First sound wave is valided if superior than this value
	float refValue = pow(10, -180.0f / 10.0f);
	for(int idFreq=0;idFreq<tab_wjthetasqr.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(refValue, timeTable, tab_wjthetasqr[idFreq]);
		double wt1 = wt0 + t1;
		double wt2 = wt0 + t2;
		//Calcul
		wxFloat32 wj_CurrentFreq=(GetSumLimit(idFreq,wt1,wt2,timeTable,tab_wjthetasqr,SUM_OPERATION_Y))/(tab_wjsrc[idFreq]/(4*M_PI*100));
		
		if(idFreq!=tab_wjthetasqr.size()-1)
			sum+=wj_CurrentFreq;
		newParameter->Set(idFreq,10*log10f(wj_CurrentFreq));
	}
	newParameter->Set(newParameter->GetSize()-1,10*log10f(sum/(tab_wjthetasqr.size()-1)));
	newParameter->SetLabel(wxGetTranslation("LG (dB)"));
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
	// First sound wave is valided if superior than this value
	float refValue = pow(10, -180.0f / 10.0f);
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		// Wave time
		double wt0 = GetTimeDecay(refValue, timeTable, tab_wj[idFreq]);
		double wt1 = wt0 + t1;
		double wt2 = wt0 + t2;
		//Calcul
		wxFloat32 wj_CurrentFreq=(GetSumLimit(idFreq,wt1,wt2,timeTable,tab_wj,SUM_OPERATION_Y))/(tab_wjsrc[idFreq]/(4*M_PI*100));
		newParameter->Set(idFreq,to_deciBel(wj_CurrentFreq));
		if(idFreq!=tab_wj.size()-1)
			sum+=to_deciBel(wj_CurrentFreq);
	}
	newParameter->Set(newParameter->GetSize()-1,sum/(tab_wj.size()-1));
	newParameter->SetLabel(wxGetTranslation("G (dB)"));
	return newParameter;
}

/**
 * @brief Calcul du paramètre acoustique STI
 * STI calculation is based on IEC:60268-16:2011 standard.
 * TODO: Compensation for speaker spectrum.
 */
formatGABE::GABE_Data_Float* Compute_STI_Param(bool surf,wxFloat32 te,wxFloat32 gen,const std::vector<wxFloat32>& timeTable,const std::vector<std::vector<wxFloat32> >& tab_wj,const std::vector<wxInt32>& freqTable)
{
	//tab_wj=p^2 !!!
	
	bool do_calc=true;
	int freqSTI[7]={125,250,500,1000,2000,4000,8000};

	////////////////////////////////////////////////////////////////////////
	//check if all needed frequencies are present
	std::vector<wxInt32>::const_iterator idx;
	std::vector<std::vector<wxFloat32>> tab_wj_corr;
	formatGABE::GABE_Data_Float* newParameter=new formatGABE::GABE_Data_Float(tab_wj.size()+1); //+1 pour le calcul toute bande
	int idx_l[7];
	float STI=0;
	float MTIk[7]={0.f};

	for(int i=0;i<7;i++){
		idx=std::find(freqTable.begin(), freqTable.end(), freqSTI[i]);
		if(std::find(freqTable.begin(), freqTable.end(), freqSTI[i])==freqTable.end()){
			do_calc=false;
			wxLogError(wxGetTranslation("STI not calculated! The following frequency could not be found:%d"),freqSTI[i]);
			break;
		}
		idx_l[i]=idx - freqTable.begin();
		tab_wj_corr.push_back(tab_wj[idx_l[i]]);
	}
	if(do_calc){
		formatGABE::GABE_Data_Float* db_level;
		
		if(surf){
			db_level=dB_Sum_Param_surf(timeTable,tab_wj_corr);
		}else{
			db_level=dB_Sum_Param(timeTable,tab_wj_corr);
		}

		nc_curves* NC_data=new nc_curves;
		int* NC=NC_data->get_nc(te);
		float fm[14]={0.63,0.8,1,1.25,1.6,2,2.5,3.15,4,5,6.3,8,10,12.5};
		

		////////////////////////////////////////////////////////////////////////
		//calc SNR
		float SNR[7];
		float lvl_SNR[7];
		for(int i=0;i<7;i++){
			lvl_SNR[i]=10*log10(pow(10,db_level->GetValue(i)/10.)+pow(10,NC[i+1]/10.));
			SNR[i]=lvl_SNR[i]-NC[i+1];
		}

		////////////////////////////////////////////////////////////////////////
		//calc modulation transfer function (k,m)
		float mkf[14][7];
		float mkf_re=0;
		float mkf_im=0;
		float mkf_den=0;

		for(int m=0;m<14;m++){
			for(int k=0;k<7;k++){
				for(int t=0;t<timeTable.size();t++){
					mkf_re+=tab_wj_corr[k][t]*cos(2*M_PI*fm[m]*(timeTable[t]/1000));
					mkf_im+=tab_wj_corr[k][t]*sin(2*M_PI*fm[m]*(timeTable[t]/1000));
					mkf_den+=tab_wj_corr[k][t];
				}
				float mkf_num=sqrt(pow(mkf_re,2)+pow(mkf_im,2));
				mkf[m][k]=(mkf_num/mkf_den)/(1+pow(10,-SNR[k]/10));
				mkf_re=0;
				mkf_im=0;
				mkf_den=0;
			}
		}

		////////////////////////////////////////////////////////////////////////
		//Iamk - intensity correction calculation
		float Ik[7];
		float Iamk[7];
		float amdb[7];
		float amf[7];

		Iamk[0]=0;
		Ik[0]=pow(10,lvl_SNR[0]/10);

		for(int m=0;m<14;m++){
			for(int k=1;k<7;k++){
				Ik[k]=pow(10,lvl_SNR[k]/10);
				if(lvl_SNR[k]<63){
					amdb[k]=0.5*lvl_SNR[k]-65;
				}else if(lvl_SNR[k]<67){
					amdb[k]=1.8*lvl_SNR[k]-146.9;
				}else if(lvl_SNR[k]<100){
					amdb[k]=0.5*lvl_SNR[k]-59.8;
				}else{
					amdb[k]=-10;
				}
				amf[k]=pow(10,amdb[k]/10);
				Iamk[k]=Ik[k-1]*amf[k];
			}
		}

		////////////////////////////////////////////////////////////////////////
		//Irtk - intensity reception treshold

		float Irtk[7];

		Irtk[0]=pow(10,46/10.);
		Irtk[1]=pow(10,27/10.);
		Irtk[2]=pow(10,12/10.);
		Irtk[3]=pow(10,6.5/10.);
		Irtk[4]=pow(10,7.5/10.);
		Irtk[5]=pow(10,8/10.);
		Irtk[6]=pow(10,12/10.);

		////////////////////////////////////////////////////////////////////////
		//mkf' - adjusted mkf computation

		float mkf_[14][7];

		for(int m=0;m<14;m++){
			for(int k=0;k<7;k++){
				mkf_[m][k]=mkf[m][k]*(Ik[k])/(Ik[k]+Iamk[k]+Irtk[k]);
			}
		}

		////////////////////////////////////////////////////////////////////////
		//SNReff - corrected SNR computation

		float SNReff[14][7];
	
		for(int m=0;m<14;m++){
			for(int k=0;k<7;k++){
				SNReff[m][k]=10*log10(mkf_[m][k]/(1-mkf_[m][k]));
				if(SNReff[m][k]>15){SNReff[m][k]=15;}
				else if(SNReff[m][k]<-15){SNReff[m][k]=-15;}
			}
		}

		////////////////////////////////////////////////////////////////////////
		//TIkf - corrected SNR computation

		float TIkf[14][7];
	
		for(int m=0;m<14;m++){
			for(int k=0;k<7;k++){
				TIkf[m][k]=(SNReff[m][k]+15)/30;
			}
		}

		////////////////////////////////////////////////////////////////////////
		//MTIk - Modulation transfer index
	
		for(int k=0;k<7;k++){
			MTIk[k]=0;
			for(int m=0;m<14;m++){
				MTIk[k]+=TIkf[m][k];
			}
			MTIk[k]=MTIk[k]/14.;
		}

		////////////////////////////////////////////////////////////////////////
		//STI
		float alfa[7];
		float beta[7];
	
		if(gen='F'){
			alfa[0]=0;		beta[0]=0;
			alfa[1]=0.117;	beta[1]=0.099;
			alfa[2]=0.223;	beta[2]=0.066;
			alfa[3]=0.216;	beta[3]=0.062;
			alfa[4]=0.328;	beta[4]=0.025;
			alfa[5]=0.25;	beta[5]=0.076;
			alfa[6]=0.194;	beta[6]=0;
		}else{
			alfa[0]=0.085;	beta[0]=0.085;
			alfa[1]=0.127;	beta[1]=0.078;
			alfa[2]=0.23;	beta[2]=0.065;
			alfa[3]=0.233;	beta[3]=0.011;
			alfa[4]=0.309;	beta[4]=0.047;
			alfa[5]=0.224;	beta[5]=0.095;
			alfa[6]=0.173;	beta[6]=0;
		}
	
		for(int k=0;k<7;k++){
			STI+=alfa[k]*MTIk[k];
		}
		for(int k=0;k<6;k++){
			STI-=beta[k]*sqrt(MTIk[k]*MTIk[k+1]);
		}
		delete NC_data;
	}
	//Pour chaque bande de fréquence
	int* stiResultIndex;
	for(int idFreq=0;idFreq<tab_wj.size();idFreq++)
	{
		stiResultIndex = std::find(idx_l, idx_l + 7, idFreq);
		if(stiResultIndex !=idx_l+7){
			newParameter->Set(idFreq,MTIk[std::distance(idx_l, stiResultIndex)]);
		}else{
			newParameter->Set(idFreq,0);
		}
	}

	newParameter->Set(newParameter->GetSize()-1,STI);
	newParameter->SetLabel(wxString::Format(wxGetTranslation("STI, NC%g"),te));
	newParameter->headerData.numOfDigits=2;
	return newParameter;
}

void ProjectManager::OnMenuDoAcousticParametersComputation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* selectedEl)
{
	//On demande à l'utilisateur d'entrer les paramètres acoustiques
	bool do_computation=true;

	std::vector<wxFloat32> CParams;
	std::vector<wxFloat32> DParams;
	std::vector<wxFloat32> TRParams;
	std::vector<wxFloat32> NCParams;
	wxString Cdefault="50;80";
	wxString Ddefault="50";
	wxString TRdefault="15;30";
	wxString NCdefault="25";

	if (!pyeventmode)
	{
		wxCustomEntryDialog parametersDialog(mainFrame,wxGetTranslation("Please, gives calculation parameters. Use ';' to separate several parameters"),wxGetTranslation("Acoustic parameters calculation"));
		parametersDialog.AddTextControl(wxGetTranslation("Clarity (ms)"),Cdefault);
		parametersDialog.AddTextControl(wxGetTranslation("Definition (ms)"),Ddefault);
		parametersDialog.AddTextControl(wxGetTranslation("Sound decay value for RT calculation (dB)"),TRdefault);
		parametersDialog.AddTextControl(wxGetTranslation("NC curve for STI calculation"),NCdefault);

		if (parametersDialog.ShowModal() == wxID_OK)
		{

			std::vector<wxString> valeursChamps;
			parametersDialog.GetValues(valeursChamps);
			Cdefault=valeursChamps[0];
			Ddefault=valeursChamps[1];
			TRdefault=valeursChamps[2];
			NCdefault=valeursChamps[3];

		}else{
			do_computation=false;
		}
	}else{
		//Si existant, lecture du paramètre fournie
		this->GetParameter("C",Cdefault);
		this->GetParameter("D",Ddefault);
		this->GetParameter("TR",TRdefault);
		this->GetParameter("NC",NCdefault);
	}
	if(do_computation)
	{
		SplitString(Cdefault,";",CParams);
		SplitString(Ddefault,";",DParams);
		SplitString(TRdefault,";",TRParams);
		SplitString(NCdefault,";",NCParams);

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
				wxLogError(wxGetTranslation("The following file doesn't exist:\n%s"),fileName);
				return;
			}
			////////////////////////////////////
			//Une colonne par bande de fréquence
			using namespace formatGABE;
			GABE tabReader(fileName.utf8_str());
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
				freqTab[idbFreq - 1] = Convertor::ToInt(dataFloat->GetLabel());
				//wxString tmp=dataFloat->GetLabel();
				//tmp=tmp.SubString(0,tmp.size()-3);
				//freqTab[idbFreq-1]=wxAtoi(tmp);
				tab_wj[idbFreq-1]=std::vector<wxFloat32>(nbTimeStep);

				for(int idstep=0;idstep<nbTimeStep;idstep++)
				{
					tab_wj[idbFreq-1][idstep]=*(*dataFloat)[idstep];
					tab_wj[nbBandeFreq][idstep]+=*(*dataFloat)[idstep];
				}
			}
			rowLbls->SetString(nbBandeFreq,wxGetTranslation("Global"));
			rowLbls->SetString(nbBandeFreq+1,wxGetTranslation("Average"));

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
			for (int idParam = 0; idParam < CParams.size(); idParam++)
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
			
			/////////////////////////////////////////
            // Calcul paramètre STI
            /////////////////////////////////////////
			for (int idParam = 0; idParam < NCParams.size(); idParam++)
				tabParameters.push_back(Compute_STI_Param(false,NCParams[idParam],'M',timeTable,tab_wj,freqTab));
				
			
			//////////////////////////////////////////
			// Enregistrement des données des paramètres acoustique

			GABE tabWriter(tabParameters.size()+1);
			tabWriter.LockData(); //lecture seule pour l'utilisateur
			tabWriter.SetCol(0,rowLbls);
			for(int idparameter=0;idparameter<tabParameters.size();idparameter++)
				tabWriter.SetCol(idparameter+1,tabParameters[idparameter]);

			wxString path = (fileFolder + wxString(wxTRANSLATE("Acoustic parameters")) + wxString(".gabe")).ToStdString();
			tabWriter.Save(std::string(path.utf8_str()));

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
			cpRowLbls->SetString(nbBandeFreq, wxGetTranslation("Global"));
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
			wxString Schroederpath = (fileFolder + wxString(wxTRANSLATE("Schroeder curves")) + wxString(".gabe")).ToStdString();
			tabSchroederWriter.Save(std::string(Schroederpath.utf8_str()));


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

void ProjectManager::OnMenuRecepteurSurfDoSTIComputation(uiTreeCtrl* fromCtrl,Element* eRoot,Element* selectedEl,const wxString& parametertype)
{
	//On demande à l'utilisateur d'entrer les paramètres acoustiques
	std::vector<wxFloat32> NCParams;
	wxString NCdefault="25";

	if(!pyeventmode)
	{
		wxCustomEntryDialog parametersDialog(mainFrame,wxGetTranslation("Please, gives calculation parameters. Use ';' to separate several parameters"),wxGetTranslation("Acoustic parameters calculation"));
		parametersDialog.AddTextControl(wxGetTranslation("NC curve for STI calculation"),NCdefault);

		if (parametersDialog.ShowModal() == wxID_OK)
		{

			std::vector<wxString> valeursChamps;
			parametersDialog.GetValues(valeursChamps);
			NCdefault=valeursChamps[0];
		}
	}else{
		//Si existant, lecture du paramètre fournie
		this->GetParameter("NC",NCdefault);
	}
	SplitString(NCdefault,";",NCParams);


	E_Report_File* reportEle=dynamic_cast<E_Report_File*>(selectedEl);
	if(reportEle)
	{
		wxString filePath;
		wxString filePath_old;
		reportEle->BuildFullPath(filePath_old);

		using namespace formatRSBIN;
		using namespace formatGABE;

		t_ExchangeData mainData;

		std::vector<wxInt32> freqSTI;
		freqSTI.push_back(125); freqSTI.push_back(250); freqSTI.push_back(500); freqSTI.push_back(1000); freqSTI.push_back(2000); freqSTI.push_back(4000); freqSTI.push_back(8000);

		//Alimentation du tableau wj
		std::vector<std::vector<std::vector<std::vector<wxFloat32> > > >tab_wj;
		std::vector<wxFloat32> timeTable;

		RSBIN importer;
		//////////////////////////////////////////////////////////
		//Import of surface receivers file
		for(int freq=0;freq<7;freq++)
		{
			filePath=filePath_old;

			filePath.Replace(wxT("125 Hz"),wxString::Format(wxT("%i"), freqSTI[freq])+wxT(" Hz"));
			if(importer.ImportBIN(filePath,mainData))
			{
				if(timeTable.size()==0){
					for(int idstep=0;idstep<mainData.nbTimeStep;idstep++)
					{
						timeTable.push_back(mainData.timeStep*(idstep+1)*1000);
					}
				}
				for(wxInt32 idrs=0;idrs<mainData.tabRsSize;idrs++)
				{
					for(wxInt32 idface=0;idface<mainData.tabRs[idrs].dataRec.quantFaces;idface++)
					{
						if(tab_wj.size()<idrs+1){
							/*std::vector<wxFloat32> t1(mainData.nbTimeStep,0.f);
							std::vector<std::vector<wxFloat32> > t2(7,t1);
							std::vector<std::vector<std::vector<wxFloat32> > >* t3=new std::vector<std::vector<std::vector<wxFloat32> > >();
							//t3->resize(mainData.tabRs[idrs].dataRec.quantFaces);
							for(int i=0;i<mainData.tabRs[idrs].dataRec.quantFaces;i++){
								t3->push_back(t2);
							}
							//tab_wj.push_back(t3);*/
							try{
								tab_wj.push_back(std::vector<std::vector<std::vector<wxFloat32> > > (mainData.tabRs[idrs].dataRec.quantFaces,std::vector<std::vector<wxFloat32> >(7,std::vector<wxFloat32>(mainData.nbTimeStep,0.f))));
							}catch(std::exception& e){
								wxMessageDialog message(NULL,(wxT("The following exception occured %s"),e.what()));
								message.Show(true);
							}
						}
						//Pour chaque enregistrement de cette face
						int tmp=mainData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords;
						for(wxInt32 idenr=0;idenr<mainData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords;idenr++)
						{
							t_faceValue* faceval=&mainData.tabRs[idrs].dataFaces[idface].tabTimeStep[idenr];
							tab_wj[idrs][idface][freq][faceval->timeStep]=faceval->energy;
						}
					}
				}
			}
			
		}
		float STI;

		GABE_Data_Float* rsParams;
		//TODO tout les paramètres
		for(int i=0;i<tab_wj.size();i++){
			for(int j=0;j<tab_wj[i].size();j++){
				rsParams=Compute_STI_Param(true,NCParams[0],'M',timeTable,tab_wj[i][j],freqSTI);
				STI=rsParams->GetValue(7);

				delete[] mainData.tabRs[i].dataFaces[j].tabTimeStep;
				mainData.tabRs[i].dataFaces[j].tabTimeStep = new t_faceValue[1];
				mainData.tabRs[i].dataFaces[j].tabTimeStep[0]=t_faceValue(0,rsParams->GetValue(7));
				mainData.tabRs[i].dataFaces[j].dataFace.nbRecords=1;
			}
		}


	//Suppression des données temporaires
	delete rsParams;
	//Modification du type de fichier et des informations relative pour créer un fichier de cartographie du tr
	mainData.nbTimeStep=1;

	mainData.recordType=RECEPTEURS_RECORD_TYPE_STI;

	filePath.Replace(wxT("\\8000 Hz\\"),wxT("\\"));
	wxFileName nouvFich(filePath);
	nouvFich.SetName(parametertype);
	importer.ExportBIN(nouvFich.GetFullPath(),mainData);

	this->RefreshReportFolder();
	
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
	GABE gReader(gabePath.utf8_str());
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
		wxCustomEntryDialog parametersDialog(mainFrame,wxGetTranslation("Please, gives calculation parameters. Use ';' to separate several parameters"),wxGetTranslation("Acoustic parameters calculation"));
		parametersDialog.AddTextControl(wxGetTranslation("Early lateral fraction LF (ms)"),LFParam);
		parametersDialog.AddTextControl(wxGetTranslation("Early lateral fraction LFC (ms)"),LFCParam);
		//parametersDialog.AddTextControl(wxGetTranslation("Tenue acoustique latérale LG(ms)"),LGParam);
		//parametersDialog.AddTextControl(wxGetTranslation("Tenue acoustique G (ms)"),GParam);



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
			cpRowLbls->SetString(idfreq,(wxString(lstFreq->GetStringEquiv(idfreq))+wxGetTranslation(" Hz")).c_str());
		}
		cpRowLbls->SetString(nbBandeFreq,wxGetTranslation("Global"));
		cpRowLbls->SetString(nbBandeFreq+1,wxGetTranslation("Average"));
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
		wxString path = (fileFolder + wxString(wxTRANSLATE("Advanced acoustic parameters")) + wxString(".gabe")).ToStdString();
		tabWriter.Save(std::string(path.utf8_str()));
		gabeFolder->RefreshFolderContents();
		gabeFolder->FillWxTree(treeResult);
		}
}
