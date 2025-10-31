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

#include "e_report_file.h"
#include "data_manager/appconfig.h"
#include <wx/log.h>
#include <input_output/exportRecepteurSurf/rsbin.h>

#ifndef __ELEMENT_FILENAME_RECEPTEURSSVISU__
#define __ELEMENT_FILENAME_RECEPTEURSSVISU__

/** \file e_report_partvisualisation.h
 *  \brief Classe représentant un ensemble d'enregistrements de recepteurs surfaciques
 */

/**
 *	\brief Classe représentant un ensemble d'enregistrements de recepteurs surfaciques
 */
class E_Report_RecepteurSurfaciqueVisualisation: public E_Report_File
{
private:
	bool onlySum; /*<! Si un seul pas de temps */
public:
	/**
	 *	Constructeur de création
	 * @param parent Element parent
	 * @param Nom			Nom de l'élément (libellé dans l'arbre du projet). @ref traductiontuto "Ne pas traduire le libellé".
	 * @param Path Chemin d'accès au fichier depuis l'executable de l'interface
	 */
	E_Report_RecepteurSurfaciqueVisualisation(Element* parent,wxString Nom,wxString Path)
		:E_Report_File(parent,Nom,Path,ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION)
	{
		
		SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_RS);
		wxGetTranslation("Surface receivers");
		onlySum=false;
		this->AppendPropertyDecimal("surfacetransparency",wxTRANSLATE("Face opacity"),1.f,false,2,true,true,1,0);
		this->AppendPropertyBool("rendersideone",wxTRANSLATE("Display front"),true);
		this->AppendPropertyBool("rendersidetwo",wxTRANSLATE("Display back"),true);
		//On charge le fichier de la première bande de fréquence disponible afin d'avoir le nombre de pas de temps
		using namespace formatRSBIN;
		t_ExchangeData mainData;
		RSBIN importer;
		//////////////////////////////////////////////////////////
		//Importation du premier fichier de recepteurs surfaciques
		wxString fullPath;
		BuildFullPath(fullPath);
		RECEPTEURS_RECORD_TYPE recordType=RECEPTEURS_RECORD_TYPE_SPL_STANDART;
		if(importer.ReadHeaderBIN(fullPath,mainData))
		{
			if(mainData.nbTimeStep==1)
				onlySum=true;
			else
				onlySum=false;
			recordType=mainData.recordType;
		}
		float vmin,vmax;
		wxString libelMax=wxTRANSLATE("Maximum value (dB)");
		wxString libelMin=wxTRANSLATE("Minimum value (dB)");
		if(recordType==RECEPTEURS_RECORD_TYPE_SPL_STANDART)
		{
			vmin=0;
			vmax=0;
		}else if(recordType==RECEPTEURS_RECORD_TYPE_SPL_GAIN)
		{
			vmin=0;
			vmax=0;
		}else{
			libelMax=wxTRANSLATE("Maximum value (s)");
			libelMin=wxTRANSLATE("Minimum value (s)");
			vmin=0;
			vmax=0;
		}
		this->AppendPropertyInteger("rstype","rs",recordType)->Hide();
		this->AppendPropertyBool("smooth_color",wxTRANSLATE("Smooth colors"),true);
		this->AppendPropertyDecimal("maxinterval",libelMax,vmax);
		this->AppendPropertyDecimal("mininterval",libelMin,vmin);
		this->AppendCurvesParameters();

		wxGetTranslation("Display recto");
		wxGetTranslation("Display verso");
		wxGetTranslation("Maximum value (dB)");
		wxGetTranslation("Minimum value (dB)");
		wxGetTranslation("Maximum value (s)");
		wxGetTranslation("Minimum value (s)");
	}

	/**
	 *	Constructeur XML
	 * @param parent Element parent
	 * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
	 */
	E_Report_RecepteurSurfaciqueVisualisation(Element* parent,wxXmlNode* nodeElement)
		:E_Report_File(parent,"Surface receivers display","",ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION,nodeElement)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_RS);
		onlySum=false;
		wxGetTranslation("Surface receivers display");
		wxString propVal;
		if(nodeElement!=NULL && nodeElement->GetAttribute("wxid",&propVal))
		{
			//Element initialisé AVEC Xml
			if(nodeElement->GetAttribute("onlysum",&propVal))
				onlySum=true;
			if(!nodeElement->HasAttribute("showisolvl"))
				this->AppendCurvesParameters();

		}
	}

	void AppendCurvesParameters()
	{
		this->AppendPropertyBool("showisolvl",wxTRANSLATE("Show iso-curves"),true);
		this->AppendPropertyText("isolvllist",wxTRANSLATE("Iso level list"),"");
		this->AppendPropertyInteger("isosmoothlvl",wxTRANSLATE("Smooth level"),0,false,false,true,0,0);
	}
		
	virtual bool IsFittingWithThisType( const ELEMENT_TYPE& typeTest )
	{
		using namespace formatRSBIN;
		RECEPTEURS_RECORD_TYPE recordType=(RECEPTEURS_RECORD_TYPE)this->GetIntegerConfig("rstype");
		if(typeTest==ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION)
			return true;
		else if ( typeTest==ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_STANDART && recordType==RECEPTEURS_RECORD_TYPE_SPL_STANDART)
			return true;
		else if ( typeTest==ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_GAIN && recordType==RECEPTEURS_RECORD_TYPE_SPL_GAIN)
			return true;
		else if ( typeTest==ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_TR && recordType==RECEPTEURS_RECORD_TYPE_TR)
			return true;		
		else if ( typeTest==ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION_EDT && recordType==RECEPTEURS_RECORD_TYPE_EDT)
			return true;
		else
			return E_Report_File::IsFittingWithThisType(typeTest);


	}
	

	void OnRightClic( wxMenu* leMenu )
	{
		using namespace formatRSBIN;
		RECEPTEURS_RECORD_TYPE recordType=(RECEPTEURS_RECORD_TYPE)this->GetIntegerConfig("rstype");
		wxMenu* subMenuSimu = new wxMenu;
		if(!onlySum)
		{
			subMenuSimu->Append(Element::GetMenuItem(subMenuSimu,IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP,wxGetTranslation("Instantaneous sound level")));
			subMenuSimu->Append(Element::GetMenuItem(subMenuSimu,IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP_SUM,wxGetTranslation("Cumulative instantaneous sound level")));
		}
		wxString libelCumul=wxGetTranslation("Cumulating sound level");
		if(recordType==RECEPTEURS_RECORD_TYPE_TR)
			libelCumul=wxGetTranslation("Reverberation time (RT)");
		else if( recordType==RECEPTEURS_RECORD_TYPE_EDT)
			libelCumul=wxGetTranslation("Early decay time (EDT)");
		else if( recordType==RECEPTEURS_RECORD_TYPE_STI)
			libelCumul=wxGetTranslation("Speech Transmision Index (STI)");
		subMenuSimu->Append(Element::GetMenuItem(subMenuSimu,IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM,libelCumul));

		if(recordType==RECEPTEURS_RECORD_TYPE_SPL_STANDART && !onlySum)
		{
			wxMenu* subMenuPostComputation = new wxMenu;
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_TR,wxGetTranslation("RT computation")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_EDT,wxGetTranslation("EDT calculation")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_CLARITY,wxGetTranslation("Clarity computation")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_DEFINITION,wxGetTranslation("D50 computation")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_TS,wxGetTranslation("TS computation")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_ST,wxGetTranslation("ST computation")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_STI,wxGetTranslation("STI computation")));
			leMenu->Append(GetMenuItem(leMenu,-1,wxGetTranslation("Acoustic parameters"),subMenuPostComputation));
		}

		leMenu->Append(GetMenuItem(leMenu,-1,wxGetTranslation("Load animation"),subMenuSimu));
		E_Report_File::OnRightClic(leMenu);
	}
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Report_File::SaveXMLDoc(NoeudParent);
		thisNode->SetName("folder"); // Nom de la balise xml ( pas d'espace autorise )
		if(onlySum)
		{
			thisNode->DeleteAttribute("onlysum");
			thisNode->AddAttribute("onlysum","1");
		}
		return thisNode;
	}	
};




#endif
