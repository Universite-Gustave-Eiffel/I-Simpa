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

#include "data_calculation/calculsPropagation.h"
#include "base_core_configuration.h"
#include <iostream>
#include <string.h>
#include "input_output/directivity/directivityParser.h"
#include <codecvt>

using namespace CalculsGenerauxThermodynamique;
using namespace CGTconst;

Base_Core_Configuration::Base_Core_Configuration( )
{
	tabIntProp = new entier[IPROP_LAST];
	tabFloatProp = new decimal[FPROP_LAST];
	memset(tabIntProp,0,sizeof(entier)*IPROP_LAST);
	memset(tabFloatProp,0,sizeof(decimal)*FPROP_LAST);
	tabStringProp = new CoreString[SPROP_LAST];
}

string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}


bool Base_Core_Configuration::LoadCfgFile( CXml& fichierXml, bool verbose_mode)
{
	bool force_abs_atmo=false;
	l_decimal abs_atmo=0;
	using namespace std;

	CXmlNode* root=fichierXml.GetRoot();
	if(root)
	{
		SetConfigInformation(SPROP_CORE_WORKING_DIRECTORY, root->GetProperty("workingdirectory"));
		if (verbose_mode) { cout << "Loading of the atmospheric condition.." << endl; }
		CXmlNode* atmoNode=root->GetChild("condition_atmospherique");
		if(atmoNode)
		{
			SetConfigInformation(FPROP_HUMIDITE,atmoNode->GetProperty("humidite").ToFloat());
			SetConfigInformation(FPROP_PRESSION_ATMOSPHERIQUE,atmoNode->GetProperty("pression").ToFloat());
			SetConfigInformation(FPROP_TEMPERATURE,atmoNode->GetProperty("temperature").ToFloat());
			//Calcul de la célérite du son
			SetConfigInformation(FPROP_CELERITE,(float)CCalculsGenerauxThermodynamique::c_son(*FastGetConfigValue(FPROP_TEMPERATURE)+273.15));
			//Calcul de la masse volumique
			SetConfigInformation(FPROP_RHO,(float)CCalculsGenerauxThermodynamique::masse_vol(*FastGetConfigValue(FPROP_PRESSION_ATMOSPHERIQUE),*FastGetConfigValue(FPROP_TEMPERATURE)+273.15));
			SetConfigInformation(FPROP_Z0,atmoNode->GetProperty("z0").ToFloat());
			SetConfigInformation(FPROP_ALOG,atmoNode->GetProperty("alog").ToFloat());
			SetConfigInformation(FPROP_BLIN,atmoNode->GetProperty("blin").ToFloat());
			force_abs_atmo=(atmoNode->GetProperty("disable_absatmo_computation").ToInt()==1);
			abs_atmo=atmoNode->GetProperty("absatmo").ToFloat();
		}

		if (verbose_mode) { cout << "Loading of the simulation configuration.." << endl; }
		CXmlNode* simuNode=root->GetChild("simulation");
		if(simuNode)
		{
			SetConfigInformation(SPROP_RECEPTEUR_SURFACIQUE_FOLDER_PATH,simuNode->GetProperty("recepteurss_directory"));
			SetConfigInformation(SPROP_RECEPTEUR_SURFACIQUE_FILE_PATH,simuNode->GetProperty("recepteurss_filename"));

			if(simuNode->IsPropertyExist("recepteurss_cut_filename"))
				SetConfigInformation(SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH,simuNode->GetProperty("recepteurss_cut_filename"));
			else
				SetConfigInformation(SPROP_RECEPTEUR_SURFACIQUE_FILE_CUT_PATH,"rs_cut.csbin");

			SetConfigInformation(SPROP_PONCTUAL_RECEIVER_FOLDER_PATH,simuNode->GetProperty("receiversp_directory"));
			SetConfigInformation(SPROP_PONCTUAL_RECEIVER_FILE_PATH,simuNode->GetProperty("receiversp_filename"));
			SetConfigInformation(SPROP_ADV_PONCTUAL_RECEIVER_FILE_PATH,simuNode->GetProperty("receiversp_filename_adv"));
			SetConfigInformation(SPROP_CUMUL_FILE_PATH,simuNode->GetProperty("cumul_filename"));
			SetConfigInformation(SPROP_MODEL_FILE_PATH,simuNode->GetProperty("modelName"));
			SetConfigInformation(SPROP_TETRAHEDRALIZATION_FILE_PATH,simuNode->GetProperty("tetrameshFileName"));
			SetConfigInformation(FPROP_TIME_STEP,simuNode->GetProperty("pasdetemps").ToFloat());
			SetConfigInformation(FPROP_SIMULATION_TIME,simuNode->GetProperty("duree_simulation").ToFloat());
			SetConfigInformation(IPROP_QUANT_TIMESTEP,(entier)(ceil((*FastGetConfigValue(FPROP_SIMULATION_TIME))/(*FastGetConfigValue(FPROP_TIME_STEP)))));

			SetConfigInformation(SPROP_DIRECTIVITY_FOLDER_PATH, simuNode->GetProperty("directivities_directory"));

			CXmlNode* frequNode=simuNode->GetChild("freq_enum");
			if(frequNode)
			{
				frequNode->OrderChildsByProperty("freq");
				std::vector<CXmlNode*>::iterator iterateurNoeuds;
				for (iterateurNoeuds = frequNode->GetFirstChild(); iterateurNoeuds != frequNode->GetLastChild(); iterateurNoeuds++)
				{
					t_FreqUsage* nvFreq=new t_FreqUsage;
					(*nvFreq).doCalculation=((*iterateurNoeuds)->GetProperty("docalc")=="1");
					if((*nvFreq).doCalculation)
						SetConfigInformation(IPROP_QUANT_BFREQ_TO_CALCULATE,(*FastGetConfigValue(IPROP_QUANT_BFREQ_TO_CALCULATE))+1);
					(*nvFreq).freqValue=(*iterateurNoeuds)->GetProperty("freq").ToInt();
					(*nvFreq).freqIndex=freqList.size();
					if(force_abs_atmo)
						(*nvFreq).absorption_atmospherique=abs_atmo;
					else
						(*nvFreq).absorption_atmospherique=CCalculsGenerauxThermodynamique::Coef_Att_Atmos((*nvFreq).freqValue,*FastGetConfigValue(FPROP_HUMIDITE),*FastGetConfigValue(FPROP_PRESSION_ATMOSPHERIQUE),*FastGetConfigValue(FPROP_TEMPERATURE)+273.15)*(logf(10.f)/10.f);
					(*nvFreq).densite_proba_absorption_atmospherique=expf(-(*nvFreq).absorption_atmospherique*(*FastGetConfigValue(FPROP_CELERITE)*(*FastGetConfigValue(FPROP_TIME_STEP))));
					freqList.push_back(nvFreq);
				}
			}
		}
		if (verbose_mode) { cout << "Loading of the emitter list.." << endl; }
		CXmlNode* srcNode=root->GetChild("sources");
		if(srcNode)
		{
			//Pour chaque source
			std::vector<CXmlNode*>::iterator iterateurNoeuds;

			for (iterateurNoeuds = srcNode->GetFirstChild(); iterateurNoeuds != srcNode->GetLastChild(); iterateurNoeuds++)
			{

				t_Source *nvSource=new t_Source;
				(*nvSource).Position.set((*iterateurNoeuds)->GetProperty("x").ToFloat(),(*iterateurNoeuds)->GetProperty("y").ToFloat(),(*iterateurNoeuds)->GetProperty("z").ToFloat());
				(*nvSource).type=(SOURCE_TYPE)(*iterateurNoeuds)->GetProperty("directivite").ToInt();
				(*nvSource).sourceDelay=(*iterateurNoeuds)->GetProperty("delay").ToFloat();
				nvSource->sourceName=(*iterateurNoeuds)->GetProperty("name");
				if((*nvSource).type==SOURCE_TYPE_UNIDIRECTION || (*nvSource).type == SOURCE_TYPE_DIRECTION)
				{
					vec3 uvwSrc((*iterateurNoeuds)->GetProperty("u").ToFloat(),(*iterateurNoeuds)->GetProperty("v").ToFloat(),(*iterateurNoeuds)->GetProperty("w").ToFloat());
					(*nvSource).Direction=(uvwSrc/uvwSrc.length());
				}
				(*iterateurNoeuds)->OrderChildsByProperty("freq");
				nvSource->bandeFreqSource=new t_Source_Freq[(*iterateurNoeuds)->CountChildrens()];
				std::size_t idfrq=0;
				for (std::vector<CXmlNode*>::iterator iterateurFreqNoeuds = (*iterateurNoeuds)->GetFirstChild(); iterateurFreqNoeuds != (*iterateurNoeuds)->GetLastChild(); iterateurFreqNoeuds++)
				{
					if(idfrq<freqList.size())
					{
						nvSource->bandeFreqSource[idfrq].db=(*iterateurFreqNoeuds)->GetProperty("db").ToFloat();
						//Calcul de la puissance W
						nvSource->bandeFreqSource[idfrq].w_j=_REF_W0_*pow(10,nvSource->bandeFreqSource[idfrq].db/10);
						idfrq++;
					}
				}
				nvSource->idsource=srcList.size();

				if ((*nvSource).type == SOURCE_TYPE_DIRECTION)
				{
					string directivityFile = (*iterateurNoeuds)->GetProperty("directivity_file");
					if (!directivityFile.empty())
					{
						if ( directivityList.find(directivityFile) == directivityList.end() )
						{
							string directivityFile_path = *FastGetConfigValue(SPROP_CORE_WORKING_DIRECTORY);
							directivityFile_path += *FastGetConfigValue(SPROP_DIRECTIVITY_FOLDER_PATH);
							directivityFile_path += directivityFile;

							t_DirectivityBalloon *directivity = new t_DirectivityBalloon();
							xhn_DirectivityParser fileparser;
							fileparser.parse(directivityFile_path, directivity);
							nvSource->directivity = directivity;
							directivityList[directivityFile] = nvSource->directivity;
							if (verbose_mode) { cout << "Directivity loaded : " << directivityFile_path << endl; }
						}
						else
						{
							nvSource->directivity = directivityList[directivityFile];
							if (verbose_mode) { cout << "Directivity linked to source : " << directivityFile << endl; }
						}
					}
				}

				srcList.push_back(nvSource);
			}
		}
		if (verbose_mode) { cout << srcList.size() << " emitters has been loaded." << endl; }
		CXmlNode* surfacesNode=root->GetChild("surface_absorption_enum");
		if (verbose_mode) { cout << "Loading of the materials.." << endl; }
		if(surfacesNode)
		{
			//Pour chaque matériaux
			std::vector<CXmlNode*>::iterator iterateurNoeuds;

			for (iterateurNoeuds = surfacesNode->GetFirstChild(); iterateurNoeuds != surfacesNode->GetLastChild(); iterateurNoeuds++)
			{

				t_Material* nvMaterial= new t_Material;
				nvMaterial->outsideMaterialIndice=(*iterateurNoeuds)->GetProperty("id").ToInt();
				if((*iterateurNoeuds)->IsPropertyExist("side_material"))
				{
					nvMaterial->doubleSidedMaterialEffect=((*iterateurNoeuds)->GetProperty("side_material").ToInt()==1);
				}
				(*iterateurNoeuds)->OrderChildsByProperty("freq"); //trie des frequences par frequence croissante
				nvMaterial->matSpectrumProperty= new t_Material_BFreq[(*iterateurNoeuds)->CountChildrens()];
				uentier indice=0;
				for (std::vector<CXmlNode*>::iterator iterateurBFreq = (*iterateurNoeuds)->GetFirstChild(); iterateurBFreq != (*iterateurNoeuds)->GetLastChild(); iterateurBFreq++)
				{
					if(indice<freqList.size())
					{
						nvMaterial->matSpectrumProperty[indice].absorption=(*iterateurBFreq)->GetProperty("absorb").ToFloat();
						nvMaterial->matSpectrumProperty[indice].diffusion=(*iterateurBFreq)->GetProperty("diffusion").ToFloat();
						if((*iterateurBFreq)->IsPropertyExist("affaiblissement"))
						{
							nvMaterial->matSpectrumProperty[indice].affaiblissement=(*iterateurBFreq)->GetProperty("affaiblissement").ToFloat();
							nvMaterial->matSpectrumProperty[indice].tau=pow(10,-nvMaterial->matSpectrumProperty[indice].affaiblissement/10.);
							nvMaterial->matSpectrumProperty[indice].dotransmission=true;
						}else{
							nvMaterial->matSpectrumProperty[indice].affaiblissement=0;
							nvMaterial->matSpectrumProperty[indice].tau=0;
							nvMaterial->matSpectrumProperty[indice].dotransmission=false;
						}
						uentier refLaw=(*iterateurBFreq)->GetProperty("loi").ToInt();
						nvMaterial->matSpectrumProperty[indice].reflectionLaw=(REFLECTION_LAW)refLaw;
						indice++;
					}
				}
				materialList.push_back(nvMaterial);
			}
		}
		if (verbose_mode) { cout << materialList.size() << " materials has been loaded." << endl; }
		CXmlNode* recepteurspNode=root->GetChild("recepteursp");
		if (verbose_mode) { cout << "Loading of the punctual receivers" << endl; }
		if(recepteurspNode)
		{
			//Pour chaque récepteur ponctuel
			// Initialisation des valeurs
			std::vector<CXmlNode*>::iterator iterateurNoeuds;
			const entier nbpasdetemps=*FastGetConfigValue(IPROP_QUANT_TIMESTEP);
			for (iterateurNoeuds = recepteurspNode->GetFirstChild(); iterateurNoeuds != recepteurspNode->GetLastChild(); iterateurNoeuds++)
			{
				t_Recepteur_P *nvRecepteurp=new t_Recepteur_P(freqList.size(),nbpasdetemps);
				//Initialisation du tableau des pas de temps en fonction des bandes de fréquences utilisées
				for(uentier idfreq=0;idfreq<freqList.size();idfreq++)
					if(freqList[idfreq]->doCalculation)
						nvRecepteurp->InitFreq(idfreq);
				(*nvRecepteurp).position.set((*iterateurNoeuds)->GetProperty("x").ToFloat(),(*iterateurNoeuds)->GetProperty("y").ToFloat(),(*iterateurNoeuds)->GetProperty("z").ToFloat());
				(*nvRecepteurp).index=(*iterateurNoeuds)->GetProperty("id").ToInt();
				(*nvRecepteurp).lblRp=(*iterateurNoeuds)->GetProperty("lbl");
				(*nvRecepteurp).idrp=recepteur_p_List.size();
				vec3 uvwRecp((*iterateurNoeuds)->GetProperty("u").ToFloat(),(*iterateurNoeuds)->GetProperty("v").ToFloat(),(*iterateurNoeuds)->GetProperty("w").ToFloat());
				(*nvRecepteurp).orientation=(uvwRecp/uvwRecp.length());
				(*iterateurNoeuds)->OrderChildsByProperty("freq");
				uentier idfrq=0;
				for (std::vector<CXmlNode*>::iterator iterateurFreqNoeuds = (*iterateurNoeuds)->GetFirstChild(); iterateurFreqNoeuds != (*iterateurNoeuds)->GetLastChild(); iterateurFreqNoeuds++)
				{
					if(idfrq<freqList.size())
					{
						nvRecepteurp->bruit_spectre[idfrq].db=(*iterateurFreqNoeuds)->GetProperty("db").ToFloat();
						idfrq++;
					}
				}
				recepteur_p_List.push_back(nvRecepteurp);
			}
		}
		CXmlNode* recepteurssNode=root->GetChild("recepteurss");
		if (verbose_mode) { cout << "Loading of the surfaces receivers" << endl; }
		if(recepteurssNode)
		{
			//Pour chaque récepteur surfacique
			// Initialisation des valeurs
			std::vector<CXmlNode*>::iterator iterateurNoeuds;
			for (iterateurNoeuds = recepteurssNode->GetFirstChild(); iterateurNoeuds != recepteurssNode->GetLastChild(); iterateurNoeuds++)
			{
				if((*iterateurNoeuds)->GetName()=="recepteur_surfacique")
				{
					r_Surf *nvRecepteurs=new r_Surf();
					(*nvRecepteurs).idRecepteurS=(*iterateurNoeuds)->GetProperty("id").ToInt();
					(*nvRecepteurs).name=(*iterateurNoeuds)->GetProperty("name");
					recepteur_s_List.push_back(nvRecepteurs);
				}else if((*iterateurNoeuds)->GetName()=="recepteur_surfacique_coupe")
				{
					this->SetConfigInformation(IPROP_CUTTING_PLANE_USAGE,true);
					r_SurfCut* nvRecepteurs=new r_SurfCut();
					(*nvRecepteurs).name=(*iterateurNoeuds)->GetProperty("name");
					(*nvRecepteurs).idRecepteurS=(*iterateurNoeuds)->GetProperty("id").ToInt();
					(*nvRecepteurs).Avert.set((*iterateurNoeuds)->GetProperty("ax").ToFloat(),(*iterateurNoeuds)->GetProperty("ay").ToFloat(),(*iterateurNoeuds)->GetProperty("az").ToFloat());
					(*nvRecepteurs).Bvert.set((*iterateurNoeuds)->GetProperty("bx").ToFloat(),(*iterateurNoeuds)->GetProperty("by").ToFloat(),(*iterateurNoeuds)->GetProperty("bz").ToFloat());
					(*nvRecepteurs).Cvert.set((*iterateurNoeuds)->GetProperty("cx").ToFloat(),(*iterateurNoeuds)->GetProperty("cy").ToFloat(),(*iterateurNoeuds)->GetProperty("cz").ToFloat());
					(*nvRecepteurs).planeNormal=FaceNormal((*nvRecepteurs).Avert,(*nvRecepteurs).Bvert,(*nvRecepteurs).Cvert);
					decimal resolution=(*iterateurNoeuds)->GetProperty("resolution").ToFloat();
					vec3 BC=(*nvRecepteurs).Cvert-(*nvRecepteurs).Bvert;
					vec3 BA=(*nvRecepteurs).Avert-(*nvRecepteurs).Bvert;
					uentier NbCellU=(uentier)ceilf(BC.length()/resolution);
					uentier NbCellV=(uentier)ceilf(BA.length()/resolution);
					float Ucell_size=BC.length()/NbCellU;
					float Vcell_size=BA.length()/NbCellV;
					(*nvRecepteurs).NbCellU=NbCellU;
					(*nvRecepteurs).NbCellV=NbCellV;
					(*nvRecepteurs).UCellSize=Ucell_size;
					(*nvRecepteurs).VCellSize=Vcell_size;
					recepteur_scut_List.push_back(nvRecepteurs);
				}
			}
		}
		CXmlNode* encombrementNode=root->GetChild("encombrement_enum");
		if (verbose_mode) { cout << "Loading of the obstructions" << endl; }
		if(encombrementNode)
		{
			//Pour chaque encombrement
			// Initialisation des valeurs
			std::vector<CXmlNode*>::iterator iterateurNoeuds;
			for (iterateurNoeuds = encombrementNode->GetFirstChild(); iterateurNoeuds != encombrementNode->GetLastChild(); iterateurNoeuds++)
			{
				t_Encombrement *nvEncombrement=new t_Encombrement(freqList.size());
				(*nvEncombrement).idEncombrement=(*iterateurNoeuds)->GetProperty("id").ToInt();
				(*iterateurNoeuds)->OrderChildsByProperty("freq"); //trie des frequences par frequence croissante
				//Pour chaque bande de fréquence
				uentier indice=0;
				for (std::vector<CXmlNode*>::iterator iterateurBFreq = (*iterateurNoeuds)->GetFirstChild(); iterateurBFreq != (*iterateurNoeuds)->GetLastChild(); iterateurBFreq++)
				{
					if(indice<freqList.size())
					{
						nvEncombrement->encSpectrumProperty[indice].alpha =(*iterateurBFreq)->GetProperty("alpha").ToFloat();
						nvEncombrement->encSpectrumProperty[indice].lambda =(*iterateurBFreq)->GetProperty("lambda").ToFloat();
						nvEncombrement->encSpectrumProperty[indice].law_diffusion =(DIFFUSION_LAW)(*iterateurBFreq)->GetProperty("loi_diff").ToInt();
						indice++;
					}
				}

				encombrement_List.push_back(nvEncombrement);
			}
		}
	}
	return true;
}

Base_Core_Configuration::~Base_Core_Configuration( )
{
	delete[] tabFloatProp;
	delete[] tabIntProp;
	delete[] tabStringProp;

	for(uentier i=0;i<materialList.size();i++)
		delete materialList[i];
	for(uentier i=0;i<srcList.size();i++)
		delete srcList[i];
	for(uentier i=0;i<freqList.size();i++)
		delete freqList[i];
	for (std::map<std::string, t_DirectivityBalloon*>::iterator it = directivityList.begin(); it != directivityList.end(); ++it)
		delete it->second;
	for(uentier i=0;i<recepteur_p_List.size();i++)
		delete recepteur_p_List[i];
	for(uentier i=0;i<recepteur_s_List.size();i++)
		delete recepteur_s_List[i];
	for(uentier i=0;i<recepteur_scut_List.size();i++)
		delete recepteur_scut_List[i];
	for(uentier i=0;i<encombrement_List.size();i++)
		delete encombrement_List[i];
}


void Base_Core_Configuration::SetConfigInformation(BASE_FPROP propertyIndex,decimal valeur)
{
	tabFloatProp[propertyIndex]=valeur;
}
void Base_Core_Configuration::SetConfigInformation(BASE_SPROP propertyIndex,CoreString valeur)
{
	tabStringProp[propertyIndex]=valeur;
}
void Base_Core_Configuration::SetConfigInformation(BASE_IPROP propertyIndex,entier valeur)
{
	tabIntProp[propertyIndex]=valeur;
}


t_Material* Base_Core_Configuration::GetMaterialByOutsideIndex( uentier outsideIndex )
{
	uentier sizeOfMaterials=materialList.size();
	for(uentier i=0;i<sizeOfMaterials;i++)
		if((*materialList[i]).outsideMaterialIndice==outsideIndex)
			return materialList[i];
	return NULL;
}


t_Encombrement* Base_Core_Configuration::GetEncombrementByOutsideIndex( uentier outsideIndex )
{
	uentier sizeOfEncombrements=encombrement_List.size();
	for(uentier i=0;i<sizeOfEncombrements;i++)
		if((*encombrement_List[i]).idEncombrement==(entier)outsideIndex)
			return encombrement_List[i];
	return NULL;
}
