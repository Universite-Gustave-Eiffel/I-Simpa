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
		_("recepteurss");
		onlySum=false;
		this->AppendPropertyDecimal("surfacetransparency","Opacité des faces",1.f,false,2,true,true,1,0);
		_("Opacité des faces");
		this->AppendPropertyBool("rendersideone","Afficher recto",true);
		this->AppendPropertyBool("rendersidetwo","Afficher verso",true);
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
		wxString libelMax="Valeur maximale (dB)";
		wxString libelMin="Valeur minimale (dB)";
		if(recordType==RECEPTEURS_RECORD_TYPE_SPL_STANDART)
		{
			vmin=0;
			vmax=0;
		}else if(recordType==RECEPTEURS_RECORD_TYPE_SPL_GAIN)
		{
			vmin=0;
			vmax=0;
		}else{
			libelMax="Valeur maximale (s)";
			libelMin="Valeur minimale (s)";
			vmin=0;
			vmax=0;
		}
		this->AppendPropertyEntier("rstype","rs",recordType)->Hide();
		this->AppendPropertyBool("smooth_color","Adoucir les différences de couleurs",true);
		this->AppendPropertyDecimal("maxinterval",libelMax,vmax);
		this->AppendPropertyDecimal("mininterval",libelMin,vmin);
		this->AppendCurvesParameters();

		_("Afficher recto");
		_("Afficher verso");
		_("Valeur maximale (dB)");
		_("Valeur minimale (dB)");
		_("Valeur maximale (s)");
		_("Valeur minimale (s)");
	}

	/**
	 *	Constructeur XML
	 * @param parent Element parent
	 * @param nodeElement	Noeud XML si création depuis un fichier XML, sinon Pointeur nul (par défaut)
	 */
	E_Report_RecepteurSurfaciqueVisualisation(Element* parent,wxXmlNode* nodeElement)
		:E_Report_File(parent,"Visualisation recepteurs surfaciques","",ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION,nodeElement)
	{
		SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_RS);
		onlySum=false;
		_("Visualisation récepteurs surfaciques");
		wxString propVal;
		if(nodeElement!=NULL && nodeElement->GetPropVal("wxid",&propVal))
		{
			//Element initialisé AVEC Xml
			if(nodeElement->GetPropVal("onlysum",&propVal))
				onlySum=true;
			if(!nodeElement->HasProp("showisolvl"))
				this->AppendCurvesParameters();

		}
	}

	void AppendCurvesParameters()
	{
		this->AppendPropertyBool("showisolvl","Afficher les lignes de niveaux",true);
		this->AppendPropertyText("isolvllist","Liste des niveaux iso","");
		this->AppendPropertyEntier("isosmoothlvl","Niveau de lissage",0,false,false,true,0,0);
		_("Afficher les lignes de niveaux");
		_("Liste des niveaux iso");
		_("Niveau de lissage");
	}
		
	virtual bool IsFittingWithThisType( const ELEMENT_TYPE& typeTest )
	{
		using namespace formatRSBIN;
		RECEPTEURS_RECORD_TYPE recordType=(RECEPTEURS_RECORD_TYPE)this->GetEntierConfig("rstype");
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
		RECEPTEURS_RECORD_TYPE recordType=(RECEPTEURS_RECORD_TYPE)this->GetEntierConfig("rstype");
		wxMenu* subMenuSimu = new wxMenu;
		if(!onlySum)
		{
			subMenuSimu->Append(Element::GetMenuItem(subMenuSimu,IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP,_("Niveau Sonore (Instantané)")));
			subMenuSimu->Append(Element::GetMenuItem(subMenuSimu,IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP_SUM,_("Niveau Sonore (Instantané avec cumul)")));
		}
		wxString libelCumul=_("Niveau Sonore (Cumul)");
		if(recordType==RECEPTEURS_RECORD_TYPE_TR)
			libelCumul=_("Temps de réverbération (TR)");
		else if( recordType==RECEPTEURS_RECORD_TYPE_EDT)
			libelCumul=_("Temps de décroissance initial (EDT)");
		subMenuSimu->Append(Element::GetMenuItem(subMenuSimu,IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM,libelCumul));

		if(recordType==RECEPTEURS_RECORD_TYPE_SPL_STANDART && !onlySum)
		{
			wxMenu* subMenuPostComputation = new wxMenu;
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_TR,_("Calcul du TR15 et TR30")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_EDT,_("Calcul de l'EDT")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_CLARITY,_("Calcul de C50 et C80")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_DEFINITION,_("Calcul de D50")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_TS,_("Calcul de TS")));
			subMenuPostComputation->Append(Element::GetMenuItem(subMenuPostComputation,IDEVENT_RECEPTEURS_COMPUTE_ST,_("Calcul de ST")));
			leMenu->Append(GetMenuItem(leMenu,-1,_("Paramètres acoustiques"),subMenuPostComputation));
		}

		leMenu->Append(GetMenuItem(leMenu,-1,_("Charger la simulation"),subMenuSimu));
		E_Report_File::OnRightClic(leMenu);
	}
	
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Report_File::SaveXMLDoc(NoeudParent);
		thisNode->SetName("folder"); // Nom de la balise xml ( pas d'espace autorise )
		if(onlySum)
		{
			thisNode->DeleteProperty("onlysum");
			thisNode->AddProperty("onlysum","1");
		}
		return thisNode;
	}	
};




#endif