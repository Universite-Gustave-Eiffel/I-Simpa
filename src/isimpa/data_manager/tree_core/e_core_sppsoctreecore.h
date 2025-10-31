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

#include "data_manager/tree_core/e_core_core.h"

#ifndef __E_CORE_SPPS_OCTREE_
#define __E_CORE_SPPS_OCTREE_

/*! \file e_core_sppscore.h
    \brief Temporaire le temps de stabiliser le type de calcul octree spps
*/


/**
	\brief Element correspondant au moteur de calcul "Simulation de la propagation de particules sonores"
*/
class E_Core_SppsOctree: public E_Core_Core
{
protected:
	void InitTransmission(E_Core_Core_Configuration* confCore)
	{
		confCore->AppendPropertyDecimal("trans_epsilon","Limite de transmission",5,false,1,true,true,10,0,true);
		confCore->AppendPropertyBool("trans_calc","Calcul Transmission",true,true);
	}
public:

	E_Core_SppsOctree( Element* parent, wxXmlNode* noeudCourant)
		:E_Core_Core(parent,"SPPSoctree",ELEMENT_TYPE_CORE_SPPS_OCTREE,noeudCourant,"")
	{

		E_Core_Core_Configuration* confCore=dynamic_cast<E_Core_Core_Configuration*>(this->GetElementByType(ELEMENT_TYPE_CORE_CORE_CONFIG));
		if(confCore)
		{
			if(!confCore->IsPropertyExist("rayon_recepteurp"))
				confCore->AppendPropertyDecimal("rayon_recepteurp","Rayon des récepteurs ponctuels",.62035f,false,5,false,true,0,EPSILON,true);
			if(!confCore->IsPropertyExist("trans_epsilon")) //mise à jour projet < 12/11/2008
				InitTransmission(confCore);
		}
	}

	E_Core_SppsOctree( Element* parent)
		:E_Core_Core(parent,"SPPSoctree",ELEMENT_TYPE_CORE_SPPS_OCTREE)
	{

		E_Core_Core_Configuration* confCore=new E_Core_Core_Configuration(this);
		this->AppendFils(confCore);

		confCore->InitProperties();
		//Ajout des propriétés propre à spps
		std::vector<wxString> computationMethods;
		std::vector<int> computationMethodsIndex;
		computationMethods.push_back("Aléatoire");
		computationMethods.push_back("Énergétique");

		confCore->AppendPropertyInteger("nbparticules","Particules par source",1000000,true,false,true,0,1);
		confCore->AppendPropertyInteger("nbparticules_rendu","Particules par source (rendu)",0,true,false,true,0,0);
		confCore->AppendPropertyBool("abs_atmo_calc","Calcul Absorption atmosphérique",true,true);
		confCore->AppendPropertyBool("enc_calc","Calcul Encombrement",true,true);
		confCore->AppendPropertyBool("direct_calc","Calcul du Champ Direct uniquement",false,true);
		confCore->AppendPropertyList("computation_method","Méthode de calcul",computationMethods,0,false,1,computationMethodsIndex,true);
		confCore->AppendPropertyBool("outpout_recs_byfreq","Export des récepteurs de surface par bande de fréquence",true,true);

		InitTransmission(confCore);
		#if 0
		    // Code source à destination de PoEdit
			wxGetTranslation("SPPS");
			wxGetTranslation("Random");
			wxGetTranslation("Energetic");
			wxGetTranslation("Export surface receivers for each frequency band");
			wxGetTranslation("Calculation method");
			wxGetTranslation("Number of sound particles per source");
			wxGetTranslation("Number of sound particles per source (display)");
			wxGetTranslation("Active calculation of atmospheric absorption");
			wxGetTranslation("Active calculation of diffusion by fitting objects");
			wxGetTranslation("Active calculation of direct field only");
			wxGetTranslation("Calculation informations");
			wxGetTranslation("Radius of receivers (m)");
		#endif

		this->AppendFils(new E_Core_Core_Bfreqselection(this));
		coreData.modelName="mesh.cbin";
		coreData.corePath="sppsOctree"+wxFileName::GetPathSeparator();
		coreData.exeName="spps";
		coreData.tetrahedraMeshFileName="";
		coreData.particleOutputName="particle.cpbin";

		E_Core_Core::InitHiddenConfig();
	}
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent)
	{
		wxXmlNode* thisNode = E_Core_Core::SaveXMLDoc(NoeudParent);
		thisNode->SetName("spps");
		return thisNode;
	}
	void Modified(Element* eModif)
	{
		t_elementInfo filsInfo=eModif->GetElementInfos();
		if(filsInfo.libelleElement=="nbparticules" && eModif->GetElementParent()->IsPropertyExist("nbparticules_rendu"))
		{
			int nbpart=eModif->GetElementParent()->GetIntegerConfig("nbparticules");
			int nbpartrendu=eModif->GetElementParent()->GetIntegerConfig("nbparticules_rendu");
			if(nbpart<nbpartrendu)
				eModif->GetElementParent()->UpdateEntierConfig("nbparticules_rendu",nbpart);
		}else if(filsInfo.libelleElement=="nbparticules_rendu" && eModif->GetElementParent()->IsPropertyExist("nbparticules"))
		{
			int nbpart=eModif->GetElementParent()->GetIntegerConfig("nbparticules");
			int nbpartrendu=eModif->GetElementParent()->GetIntegerConfig("nbparticules_rendu");
			if(nbpart<nbpartrendu)
				eModif->GetElementParent()->UpdateEntierConfig("nbparticules",nbpartrendu);
		}
		Element::Modified(eModif);
	}
};

#endif
