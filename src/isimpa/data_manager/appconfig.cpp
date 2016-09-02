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

#include "appconfig.h"
#include <algorithm>
#include "tree_scene/e_scene_groupesurfaces.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "last_cpp_include.hpp"

wxXmlDocument ApplicationConfiguration::appConfig;
std::vector<E_GammeFrequence*> ApplicationConfiguration::allSpectrum;
std::vector<E_Materiau*> ApplicationConfiguration::allMaterial;
std::vector<E_Directivity*> ApplicationConfiguration::allDirectivity;
std::vector<ApplicationConfiguration::t_vec_param> ApplicationConfiguration::vec_param;
wxString ApplicationConfiguration::nameNodeSpectre="appspectrums";
wxString ApplicationConfiguration::nameNodeMateriaux = "appmaterials";
wxString ApplicationConfiguration::nameNodeDirectivity="appdirectivities";
smart_ptr<wxFileConfig> ApplicationConfiguration::projectConfig=NULL;
Element* ApplicationConfiguration::rootScene=NULL;
smart_ptr<Element> ApplicationConfiguration::rootUserConfig;
std::vector<ApplicationConfiguration::t_freq> ApplicationConfiguration::tabFreq;
std::vector<ApplicationConfiguration::t_ponderation> ApplicationConfiguration::tabPonderation;
std::vector<wxString> ApplicationConfiguration::tabLoisReflexion;
ApplicationConfiguration::tab_HashElement ApplicationConfiguration::tab_refElementLst;

const wxString ApplicationConfiguration::CONST_REPORT_FOLDER_PATH=wxString("report")+wxFileName::GetPathSeparator();
const wxString ApplicationConfiguration::CONST_REPORT_PARTICLE_FOLDER_PATH=wxString("particles")+wxFileName::GetPathSeparator();
const wxString ApplicationConfiguration::CONST_REPORT_PARTICLE_FILENAME=wxString(_("particles"))+wxString(".pbin");
const wxString ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FOLDER_PATH=wxString(_("recepteurss"))+wxFileName::GetPathSeparator();
/* const wxString ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FILENAME=_("Sound_level.csbin");*/
const wxString ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FILENAME=wxString(_("Sound_level"))+wxString(".csbin");
const wxString ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_CUT_FILENAME="rs_cut.csbin";
const wxString ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FILENAME_TR="TR.csbin";
const wxString ApplicationConfiguration::CONST_REPORT_RECEPTEURSS_FILENAME_EDT="EDT.csbin";
const wxString ApplicationConfiguration::CONST_REPORT_DIRECTIVITIES_FOLDER_PATH = wxString("loudspeakers") + wxFileName::GetPathSeparator();;
const wxString ApplicationConfiguration::CONST_MODEL_SCENE_FILENAME="sceneMesh.bin";


const wxString ApplicationConfiguration::CONST_PREPROCESS_EXE_FILENAME="preprocess";
const wxString ApplicationConfiguration::CONST_GRAPH_CONFIG_PATH="SIMPLEGRAPH";
const wxString ApplicationConfiguration::CONST_USER_PREFERENCE_FILE_NAME="isimpa_pref.xml";

const wxString ApplicationConfiguration::CONST_RESOURCE_BITMAP_FOLDER=wxString("Bitmaps")+wxFileName::GetPathSeparator();
const wxString ApplicationConfiguration::CONST_RESOURCE_ISO_FOLDER=wxString("iso")+wxFileName::GetPathSeparator();
const wxString ApplicationConfiguration::CONST_RESOURCE_DIRECTIVITY_FOLDER = wxString("Directivities") + wxFileName::GetPathSeparator();


const wxString ApplicationConfiguration::CONST_TETROOT_PATH=wxString("meshing")+wxFileName::GetPathSeparator();

#ifndef _WIN32
const wxString ApplicationConfiguration::CONST_TETGEN_EXE_FILENAME = "tetgen";
#else
const wxString ApplicationConfiguration::CONST_TETGEN_EXE_FILENAME = "tetgen.exe";
#endif

const wxString ApplicationConfiguration::CONST_TETGEN_EXE_PATH=ApplicationConfiguration::CONST_TETROOT_PATH+wxString("tetgen")+wxFileName::GetPathSeparator();
const wxString ApplicationConfiguration::CONST_CORE_PATH=wxString("core") + wxFileName::GetPathSeparator();
const wxString ApplicationConfiguration::CONST_PREPROCESS_EXE_PATH=ApplicationConfiguration::CONST_TETROOT_PATH+"tetgen"+wxFileName::GetPathSeparator();
const wxString ApplicationConfiguration::CONST_STATIC_XML_FILE="appconst.xml";
//const unsigned long ApplicationConfiguration::CONST_WORKINGLIMIT=1325329200;



ApplicationConfiguration::t_App_Info ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS;



ApplicationConfiguration::t_GLOBAL_VAR ApplicationConfiguration::GLOBAL_VAR={"current","current",false};

DEFINE_EVENT_TYPE( wxEVT_PSPS_MAIN_EVENT )

const std::vector<wxString>& ApplicationConfiguration::GetLstLoisReff( )
{
	return tabLoisReflexion;
}

void ApplicationConfiguration::LoadConfiguration(wxString propFile)
{
	if(!wxFileExists(propFile))
	{
		//Le fichier de configuration n'existe pas, on va créer son architecture
		wxXmlDocument tmpDocXml;
		wxXmlNode* xmlRoot=new wxXmlNode(wxXML_ELEMENT_NODE,"configuration");
		tmpDocXml.SetRoot(xmlRoot);
		//Noeud spectres programme
		wxXmlNode* spectreRoot=new wxXmlNode(xmlRoot,wxXML_ELEMENT_NODE,nameNodeSpectre);
		tmpDocXml.Save(propFile);
		//Noeud matériaux programme
		wxXmlNode* materiauRoot=new wxXmlNode(xmlRoot,wxXML_ELEMENT_NODE,nameNodeMateriaux);
		tmpDocXml.Save(propFile);
		//Noeud directivités programme
		wxXmlNode* directivityRoot = new wxXmlNode(xmlRoot, wxXML_ELEMENT_NODE, nameNodeDirectivity);
		tmpDocXml.Save(propFile);
	}
	appConfig.Load(propFile);
	//Création des tableaux de fréquence
	tabFreq.clear();
	const int nbfreq=27;
	int temptabfreq[nbfreq]={50,63,80,100,125,160,200,250,315,400,500,630,800,1000,1250,1600,2000,2500,3150,4000,5000,6300,8000,10000,12500,16000,20000};
	for(int i=0;i<nbfreq;i++)
		tabFreq.push_back(temptabfreq[i]);
	tabPonderation.clear();
	float tempPonderation[nbfreq]={-30.2f,-26.2f,-22.5f,-19.1f,-16.1f,-13.4f,-10.9f,-8.6f,-6.6f,-4.8f,-3.2f,-1.9f,-0.8f,0.0,0.6f,1.0f,1.2f,1.3f,1.2f,1.0f,0.5f,-0.1f,-1.1f,-2.5f,-4.2f,-6.7f,-9.3f};
	for(int i=0;i<nbfreq;i++)
		tabPonderation.push_back(tempPonderation[i]);

	tabLoisReflexion.clear();
	tabLoisReflexion.push_back("Specular");
	tabLoisReflexion.push_back("Uniform");
	tabLoisReflexion.push_back("Lambert");
	tabLoisReflexion.push_back("W2");
	tabLoisReflexion.push_back("W3");
	tabLoisReflexion.push_back("W4");
	tabLoisReflexion.push_back("Semi-diffuse");
	_("Specular");
	_("Uniform");
	_("W2");
	_("W3");
	_("W4");
	_("Semi-diffuse");
}

//Retourne la liste des toutes les frequences par bande de l'application
const std::vector<ApplicationConfiguration::t_freq>& ApplicationConfiguration::GetAllFrequencies( )
{
	return tabFreq;
}

void ApplicationConfiguration::GetFaceLink( std::vector<std::vector<t_PropFace> > &facesAssociations)
{
	if(rootScene)
	{
		/////////////////////////////////////
		// Chargement des propriétés de materiaux
		Element* egroupeSurfFolder=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES);
		if(egroupeSurfFolder)
		{
			E_Scene_Groupesurfaces* groupeSurfFolder=dynamic_cast<E_Scene_Groupesurfaces*>(egroupeSurfFolder);
			if(groupeSurfFolder)
			{
				groupeSurfFolder->GetMaterialsLink(facesAssociations);
			}
		}
		/////////////////////////////////////
		// Chargement des propriétés de recepteurs surfaciques
		Element* egroupeRSFolder=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_RECEPTEURSS);
		if(egroupeRSFolder)
		{
			std::list<Element*> lstRecepteursS;
			egroupeRSFolder->GetChildElement(lstRecepteursS);
			//Pour chaque sous element du dossier contenant les recepteurs surfaciques
			for(std::list<Element*>::iterator itfils=lstRecepteursS.begin();itfils!=lstRecepteursS.end();itfils++)
			{
				if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_RECEPTEURSS_RECEPTEUR)
				{
					//Pour chaque récepteur surfacique
					Element* egroupeSurfFolder=(*itfils)->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE);
					if(egroupeSurfFolder)
					{
						E_Scene_Groupesurfaces_Groupe* groupeSurfFolder=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(egroupeSurfFolder);
						if(groupeSurfFolder)
						{
							groupeSurfFolder->GetMaterialsLink(facesAssociations);
						}
					}
				}
			}
		}
		/////////////////////////////////////
		// Chargement des propriétés des encombrements
		Element* egroupeENFolder=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS);
		if(egroupeENFolder)
		{
			std::list<Element*> lstEncombrement;
			egroupeENFolder->GetChildElement(lstEncombrement);
			//Pour chaque sous element du dossier contenant les encombrements
			for(std::list<Element*>::iterator itfils=lstEncombrement.begin();itfils!=lstEncombrement.end();itfils++)
			{
				if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT)
				{
					// Si l'utilisateur désire utiliser cet encombrement dans le calcul
					if((*itfils)->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT_PROPRIETES)->GetBoolConfig("useforcalculation"))
					{
						//Pour chaque encombrement
						Element* egroupeSurfFolder=(*itfils)->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE);
						if(egroupeSurfFolder)
						{
							E_Scene_Groupesurfaces_Groupe* groupeSurfFolder=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(egroupeSurfFolder);
							if(groupeSurfFolder)
							{
								groupeSurfFolder->GetMaterialsLink(facesAssociations);
							}
						}
					}
				}
			}
		}
		/////////////////////////////////////
		// Chargement des propriétés des volumes
		Element* egroupeVolFolder=rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_VOLUMES);
		if(egroupeVolFolder)
		{
			std::list<Element*> lstVolumes;
			egroupeVolFolder->GetChildElement(lstVolumes);
			//Pour chaque sous element du dossier contenant les encombrements
			for(std::list<Element*>::iterator itfils=lstVolumes.begin();itfils!=lstVolumes.end();itfils++)
			{
				if((*itfils)->GetElementInfos().typeElement==Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME)
				{
					//Pour chaque encombrement
					Element* egroupeSurfFolder=(*itfils)->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE);
					if(egroupeSurfFolder)
					{
						E_Scene_Groupesurfaces_Groupe* groupeSurfFolder=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(egroupeSurfFolder);
						if(groupeSurfFolder)
						{
							groupeSurfFolder->GetMaterialsLink(facesAssociations);
						}
					}
				}
			}
		}
	}

}

//Retourne la liste des ponderations
const std::vector<ApplicationConfiguration::t_ponderation>& ApplicationConfiguration::GetAllPonderation( )
{
	return tabPonderation;
}
ApplicationConfiguration::t_ponderation ApplicationConfiguration::GetPonderationVal(const wxString& sfreqValue)
{
	int idfreq=-1;
	t_freq freqVal=Convertor::ToInt(sfreqValue);
	for(int i=0;i<tabFreq.size();i++)
	{
		if(tabFreq[i]==freqVal)
		{
			idfreq=i;
			break;
		}
	}
	if(idfreq>=0)
		return tabPonderation[idfreq];
	else
		return 0;
}

wxXmlNode* ApplicationConfiguration::GetAppSpectreNode()
{
	wxXmlNode* rootCfg=appConfig.GetRoot();
	wxXmlNode* currentNode=NULL;
	if(rootCfg)
	{
		currentNode=rootCfg->GetChildren();
		while(currentNode!=NULL && currentNode->GetName()!=nameNodeSpectre)
			currentNode=currentNode->GetNext();
	}
	return currentNode;
}

wxFileConfig* ApplicationConfiguration::GetFileConfig()
{
	if(projectConfig==NULL)
	{
		wxString userDir = wxStandardPaths::Get().GetUserDataDir();
		projectConfig=new wxFileConfig("i-simpa","Lcpc", userDir+wxFileName::GetPathSeparator()+"isimpa_conf.ini", userDir+"\\isimpa_conf.ini",wxCONFIG_USE_LOCAL_FILE);
	}
	return projectConfig.get();
}

smart_ptr<Element> ApplicationConfiguration::GetUserPreferenceRoot()
{
	return rootUserConfig;
}
void ApplicationConfiguration::OnApplicationClose()
{
	rootUserConfig=(Element*)NULL;
}
int ApplicationConfiguration::GetIdVecParam(VECPARAM_ELEMENT param)
{
	for(int i=0;i<vec_param.size();i++)
		if(vec_param[i].idParam==param)
			return i;
	return -1;
}
vec3 ApplicationConfiguration::GetVecParam(VECPARAM_ELEMENT param)
{
	int idparam=GetIdVecParam(param);
	if(idparam>=0)
		return vec_param[idparam].value;
	else
		return vec3(0,0,0);
}

void ApplicationConfiguration::AddParam(VECPARAM_ELEMENT param,vec3 Value)
{
	t_vec_param newparam={param,Value};
	vec_param.push_back(newparam);
}

void ApplicationConfiguration::SetVecParam(VECPARAM_ELEMENT param,vec3 newVal)
{
	int idparam=GetIdVecParam(param);
	if(idparam>=0)
		vec_param[GetIdVecParam(param)].value=newVal;
	else
		AddParam(param,newVal);
}
void ApplicationConfiguration::SetUserPreferenceElement(smart_ptr<Element> userPreference)
{
	rootUserConfig=userPreference;
}
void ApplicationConfiguration::SetRootScene(Element* _rootScene)
{
	rootScene=_rootScene;
}

Element* ApplicationConfiguration::GetRootScene()
{
	return rootScene;
}

void ApplicationConfiguration::AddSpectrum(E_GammeFrequence* elToAdd)
{
	allSpectrum.push_back(elToAdd);
}

void ApplicationConfiguration::DeleteSpectrum(int xmlId)
{
	for(long i=0;i<allSpectrum.size();i++)
	{
		if(allSpectrum[i]->GetElementInfos().xmlIdElement==xmlId)
		{
			allSpectrum.erase(allSpectrum.begin()+i);	//retire le pointeur de la liste
			break;
		}
	}

}
E_GammeFrequence* ApplicationConfiguration::GetSpectrum(int idSpectre,Element::ELEMENT_TYPE typeSpectre)
{
	E_GammeFrequence* findSpectre=NULL;
	for(int i=0;i<allSpectrum.size();i++)
	{
		if(allSpectrum[i]->GetIdSpectre()==idSpectre && allSpectrum[i]->GetTypeSpectre()==typeSpectre)
		{
			findSpectre=allSpectrum[i];
			break;
		}
	}
	return findSpectre;
}
bool ApplicationConfiguration::orderSpectrums(E_GammeFrequence* lSp,E_GammeFrequence* rSp)
{
	return lSp->GetIdSpectre()<rSp->GetIdSpectre();
}

int ApplicationConfiguration::GetFreeSpectrumId()
{

	std::sort(allSpectrum.begin( ), allSpectrum.end( ), orderSpectrums );
	//On ordonne les spectres pour avoir le résultat attendu dans l'algorithme de recherche
	int freespectre=100;
	for(int i=0;i<allSpectrum.size();i++)
	{
		if(allSpectrum[i]->GetTypeSpectre()==Element::ELEMENT_TYPE_GAMMEFREQ_USER)
		{
			if(allSpectrum[i]->GetIdSpectre()==freespectre)
				freespectre++;
		}
	}
	return freespectre;
}

bool ApplicationConfiguration::IsIdSpectrumExist(int idSpectre)
{
	for(int i=0;i<allSpectrum.size();i++)
		if(allSpectrum[i]->GetIdSpectre()==idSpectre)
			return true;
	return false;
}


std::vector<ApplicationConfiguration::t_lstSpec> ApplicationConfiguration::GetLstSpectre()
{
	std::vector<t_lstSpec> retVec;
	for(int i=0;i<allSpectrum.size();i++)
	{
		Element::t_elementInfo infoSpec=allSpectrum[i]->GetElementInfos();
		t_lstSpec curSpec;
		curSpec.idSpectre=allSpectrum[i]->GetIdSpectre();
		curSpec.typeSpectre=infoSpec.typeElement;
		curSpec.nom=infoSpec.libelleElement;
		retVec.push_back(curSpec);
	}
	return retVec;
}



void ApplicationConfiguration::AddMateriau(E_Materiau* elToAdd)
{
	allMaterial.push_back(elToAdd);
}

void ApplicationConfiguration::DeleteAllMateriaux()
{
	allMaterial.clear();
}

void ApplicationConfiguration::DeleteMateriau(int xmlId)
{
	for(long i=0;i<allMaterial.size();i++)
	{
		if(allMaterial[i]->GetElementInfos().xmlIdElement==xmlId)
		{
			allMaterial.erase(allMaterial.begin()+i);	//retire le pointeur de la liste
			break;
		}
	}
}


int ApplicationConfiguration::GetMateriauId(Element* pereEle)
{
	if(!pereEle)
		return -1;
	//Premiere étape, retrouver l'indice de l'élément du matériau
	Element* elMat;
	elMat=pereEle->GetElementByType(Element::ELEMENT_TYPE_MATERIAU_USER);
	if(!elMat)
		elMat=pereEle->GetElementByType(Element::ELEMENT_TYPE_MATERIAU_APP);
	if(!elMat)
		return -1;
	for(int i=0;i<allMaterial.size();i++)
	{
		if(allMaterial[i]==elMat)
			return allMaterial[i]->GetIdMateriau();
	}
	return -1;
}

E_Materiau* ApplicationConfiguration::GetMateriau(int idMateriau)
{
	E_Materiau* findMateriau=NULL;
	for(int i=0;i<allMaterial.size();i++)
	{
		if(allMaterial[i]->GetIdMateriau()==idMateriau )
		{
			findMateriau=allMaterial[i];
			break;
		}
	}
	return findMateriau;
}


bool ApplicationConfiguration::IsIdMateriauExist(int idMateriau)
{
	for(int i=0;i<allMaterial.size();i++)
	{
		if(allMaterial[i]->GetIdMateriau()==idMateriau)
		{
			return true;
		}
	}
	return false;

}
int ApplicationConfiguration::GetFreeMateriauId()
{

	std::sort(allMaterial.begin( ), allMaterial.end( ), orderMateriaux );
	//On ordonne les spectres pour avoir le résultat attendu dans l'algorithme de recherche
	int freemateriau=100;
	for(int i=0;i<allMaterial.size();i++)
	{
		if(allMaterial[i]->GetTypeMateriau()==Element::ELEMENT_TYPE_MATERIAU_USER)
		{
			if(allMaterial[i]->GetIdMateriau()==freemateriau)
				freemateriau++;
		}
	}
	return freemateriau;
}


bool ApplicationConfiguration::orderMateriaux(E_Materiau* lSp,E_Materiau* rSp)
{
	return lSp->GetIdMateriau()<rSp->GetIdMateriau();
}


wxXmlNode* ApplicationConfiguration::GetAppMateriauNode()
{
	wxXmlNode* rootCfg=appConfig.GetRoot();
	wxXmlNode* currentNode=NULL;
	if(rootCfg)
	{
		currentNode=rootCfg->GetChildren();
		while(currentNode!=NULL && currentNode->GetName()!=nameNodeMateriaux)
			currentNode=currentNode->GetNext();
	}
	return currentNode;
}



std::vector<ApplicationConfiguration::t_lstMat> ApplicationConfiguration::GetLstMateriaux()
{
	std::vector<t_lstMat> retVec;
	for(int i=0;i<allMaterial.size();i++)
	{
		Element::t_elementInfo infoSpec=allMaterial[i]->GetElementInfos();
		Element* ePere=allMaterial[i]->GetElementParent();
		t_lstMat curMat;
		curMat.idMateriau=allMaterial[i]->GetIdMateriau();
		curMat.typeMateriau=infoSpec.typeElement;
		curMat.nom=ePere->GetElementInfos().libelleElement;
		retVec.push_back(curMat);
	}
	return retVec;
}

void ApplicationConfiguration::AddDirectivity(E_Directivity* elToAdd) 
{
	allDirectivity.push_back(elToAdd);
}

void ApplicationConfiguration::DeleteDirectivity(int xmlId)
{
	for (long i = 0; i<allDirectivity.size(); i++)
	{
		if (allDirectivity[i]->GetElementInfos().xmlIdElement == xmlId)
		{
			allDirectivity.erase(allDirectivity.begin() + i);	//retire le pointeur de la liste
			break;
		}
	}
}

E_Directivity* ApplicationConfiguration::GetDirectivity(int idDirectivity)
{
	E_Directivity* findDirectivity = NULL;
	for (int i = 0; i<allDirectivity.size(); i++)
	{
		if (allDirectivity[i]->GetIdDirectivity() == idDirectivity)
		{
			findDirectivity = allDirectivity[i];
			break;
		}
	}
	return findDirectivity;
}

int ApplicationConfiguration::GetDirectivityId(Element* pereEle)
{
	if (!pereEle)
		return -1;
	//Premiere étape, retrouver l'indice de l'élément de la directivité
	Element* elMat;
	elMat = pereEle->GetElementByType(Element::ELEMENT_TYPE_DIRECTIVITIES_USER);
	if (!elMat)
		elMat = pereEle->GetElementByType(Element::ELEMENT_TYPE_DIRECTIVITIES_APP);
	if (!elMat)
		return -1;
	for (int i = 0; i<allDirectivity.size(); i++)
	{
		if (allDirectivity[i] == elMat)
			return allDirectivity[i]->GetIdDirectivity();
	}
	return -1;
}

bool ApplicationConfiguration::orderDirectivity(E_Directivity* lSp, E_Directivity* rSp)
{
	return lSp->GetIdDirectivity()<rSp->GetIdDirectivity();
}

int ApplicationConfiguration::GetFreeDirectivityId()
{
	std::sort(allDirectivity.begin(), allDirectivity.end(), orderDirectivity);
	//On ordonne les directivités pour avoir le résultat attendu dans l'algorithme de recherche
	int freeDirectivity = 100;
	for (int i = 0; i<allDirectivity.size(); i++)
	{
		if (allDirectivity[i]->GetTypeDireciticity() == Element::ELEMENT_TYPE_DIRECTIVITIES_USER)
		{
			if (allDirectivity[i]->GetIdDirectivity() == freeDirectivity)
				freeDirectivity++;
		}
	}
	return freeDirectivity;
}

bool ApplicationConfiguration::IsIdDirectivityExist(int idDirectivity)
{
	for (int i = 0; i<allDirectivity.size(); i++)
	{
		if (allDirectivity[i]->GetIdDirectivity() == idDirectivity)
		{
			return true;
		}
	}
	return false;
}

wxString ApplicationConfiguration::getResourcesFolder() {
    return wxPathOnly(wxStandardPaths::Get().GetExecutablePath())+wxFileName::GetPathSeparator();
}

std::vector<ApplicationConfiguration::t_lstDirectiv> ApplicationConfiguration::GetLstDirectivity()
{
	std::vector<t_lstDirectiv> retVec;
	for (int i = 0; i<allDirectivity.size(); i++)
	{
		Element::t_elementInfo infoSpec = allDirectivity[i]->GetElementInfos();
		t_lstDirectiv curDirectiv;
		curDirectiv.idDirectivity = allDirectivity[i]->GetIdDirectivity();
		curDirectiv.typeDirectivity = infoSpec.typeElement;
		curDirectiv.nom = allDirectivity[i]->GetElementInfos().libelleElement;
		retVec.push_back(curDirectiv);
	}
	return retVec;
}

wxXmlNode* ApplicationConfiguration::GetAppDirectivityNode()
{
	wxXmlNode* rootCfg = appConfig.GetRoot();
	wxXmlNode* currentNode = NULL;
	if (rootCfg)
	{
		currentNode = rootCfg->GetChildren();
		while (currentNode != NULL && currentNode->GetName() != nameNodeDirectivity)
			currentNode = currentNode->GetNext();
	}
	return currentNode;
}

void ApplicationConfiguration::AppendRefElement(Element* newRefEl,ELEMENT_REF_TYPE hashIndex)
{
	tab_refElementLst.refElementLst[hashIndex][newRefEl->GetElementInfos().xmlIdElement]=newRefEl;
}

void ApplicationConfiguration::DeleteRefElement(Element* refElToBroke,ELEMENT_REF_TYPE hashIndex)
{
	tab_refElementLst.refElementLst[hashIndex].erase(refElToBroke->GetElementInfos().xmlIdElement);
}


t_HashElement::iterator ApplicationConfiguration::BeginRef(ELEMENT_REF_TYPE hashIndex)
{
	return tab_refElementLst.refElementLst[hashIndex].begin();
}
t_HashElement::iterator ApplicationConfiguration::EndRef(ELEMENT_REF_TYPE hashIndex)
{
	return tab_refElementLst.refElementLst[hashIndex].end();
}
t_HashElement::size_type ApplicationConfiguration::SizeRef(ELEMENT_REF_TYPE hashIndex)
{
	return tab_refElementLst.refElementLst[hashIndex].size();
}
