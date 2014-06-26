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
#include "Recepteurs_surfacique.h"
#include <wx/progdlg.h>
#include <wx/log.h>
#include "manager/stringTools.h"
#include "data_manager/appconfig.h"
#include <wx/filename.h>
#include <input_output/exportRecepteurSurf/rsbin.h>
#include <wx/progdlg.h>
#include <input_output/progressionInfo.h>
#include "input_output/gpl_io.hpp"
#include "UtfConverter.h"
#include "GL/opengl_inc.h"
#include "last_cpp_include.hpp"
#define TIMESTEP_LIMITATION 1000000

/**
 * Cette classe est à attacher à l'élément qui permet à l'utilisateur de modifier les paramètres de rendu des animations
 */
struct RecepteurSUpdater : public t_baseEvtCatcher
{
	RecepteurSUpdater(Element* _rsPrefItem,RecepteursSControler* _recepteurSctrl)
		: rsPrefItem(_rsPrefItem),recepteurSctrl(_recepteurSctrl)
	{

	}
	virtual void OnModified(const int& elid)
	{
		recepteurSctrl->Refresh(rsPrefItem);
	}
	Element* rsPrefItem;
	RecepteursSControler* recepteurSctrl;
};

RecepteursSControler::RecepteursSControler()
: Animator(), colorPaletteCount(0)
{
	idStep=0;
	timeStep=0;
	legendRenderer=NULL;
	this->SetRendererName("recepteurs_surf");
}

RecepteursSControler::~RecepteursSControler()
{
}
wxString RecepteursSControler::GetRendererLabel()
{
	return _("Récepteur surfacique");
}
void RecepteursSControler::Init(const bool&  resetLoadingTime)
{
	idStep=-1;
	xmlIdFile=-1;
	opacity=0;
	nbStep=0;
	timeStep=0;
	recepteursS.clear();
	nodesData.clear();
	if(legendRenderer)
	{
		if(rs_legends.graduationBar)
		{
			legendRenderer->Delete(rs_legends.graduationBar);
			rs_legends.graduationBar=NULL;
		}
		if(rs_legends.currentFile)
		{
			legendRenderer->Delete(rs_legends.currentFile);
			rs_legends.currentFile=NULL;
		}
		if(rs_legends.currentTimeStep)
		{
			legendRenderer->Delete(rs_legends.currentTimeStep);
			rs_legends.currentTimeStep=NULL;
		}
	}
	Animator::Init(resetLoadingTime);
}


void RecepteursSControler::UpdateRecepteurSSConfig()
{

}

void RecepteursSControler::SetTimeStep(const int& _timeStp)
{
	if(this->nbStep>_timeStp)
		idStep=_timeStp;
	else
		idStep=0;
	if(this->rs_legends.currentTimeStep)
	{
		if(this->nbStep>0)
		{
			if(this->nbStep>1)
			{
				this->rs_legends.currentTimeStep->InitText(wxString::Format("%.4f s",timeStep*(idStep+1)));
			}else{
				if(RecordRecepteurSurfType==formatRSBIN::RECEPTEURS_RECORD_TYPE_SPL_STANDART)
					this->rs_legends.currentTimeStep->InitText(_("Niveau sonore cumulé"));
				else
					this->rs_legends.currentTimeStep->InitText(this->libellLegends.labelEl);
			}
		}
	}
}


	void RecepteursSControler::Refresh(Element* elConf,bool reloadAnimation)
	{
		if(elConf && elConf->GetElementInfos().xmlIdElement==xmlIdFile)
		{
			opacity=elConf->GetDecimalConfig("surfacetransparency");
			renderFaceOne=elConf->GetBoolConfig("rendersideone");
			renderFaceTwo=elConf->GetBoolConfig("rendersidetwo");
			libellLegends.v_max=elConf->GetDecimalConfig("maxinterval");
			libellLegends.v_min=elConf->GetDecimalConfig("mininterval");
			smoothColour=elConf->GetBoolConfig("smooth_color");
			renderIsoCurves=elConf->GetBoolConfig("showisolvl");
		}
		RedrawLegend();
		if(reloadAnimation)
			loadingTime=wxDateTime::UNow();
	}
	wxFloat32 RecepteursSControler::ComputeBestIsoInterval(wxFloat32 vmin, wxFloat32 vmax)
	{
		//
		// Méthode issu de PlPlot (pldtik.c,v 1.15 2005/04/27)
		//
		wxFloat32 t1, t2;
		wxInt32 np, ns;

		t1 = (wxFloat32) log10(abs(vmax - vmin));
		np = (wxInt32) floor(t1);
		t1 = t1 - np;

		if (t1 > 0.7781512503) {
			t2 = 2.0;
			ns = 4;
		}else if (t1 > 0.4771212549) {
			t2 = 1.0;
			ns = 5;
		}else if (t1 > 0.1760912591) {
			t2 = 5.0;
			ns = 5;
			np = np - 1;
		}else {
			t2 = 2.0;
			ns = 4;
			np = np - 1;
		}

		return t2 * pow(10.0, (double) np);
	}


bool RecepteursSControler::LoadGplFile(const wxString& gplFileName)
{
	using namespace GPL_IO;
	GPL_FileExchange gplReader;
	int retval=gplReader.Read(WXSTRINGTOSTDSTRING(gplFileName));
	if(retval==0)
	{
		colorPaletteCount=gplReader.GetColorsCount();
		colorPalette=new vec3[colorPaletteCount];
		unsigned short r,g,b;
		std::string colorname;
		for(int icolor=0;icolor<colorPaletteCount;icolor++)
		{

			gplReader.GetNextColor(r,g,b,colorname);
			colorPalette[icolor].set(r/255.f,g/255.f,b/255.f);
		}
		Refresh(NULL);
		return true;
	}else{
		return false;
	}
}

bool RecepteursSControler::LoadRecepteursSFile(wxArrayString& recepteurSFiles, vec4 UnitizeVal, Element* elConf,Element::IDEVENT loadingMethod)
{
	wxDateTime fileLoading=wxDateTime::UNow();
	const float dissipationFaceSurSommets=1.f;// /3.f;
	wxProgressDialog progDialog(_("Chargement du fichier de récepteur de surface"),_("Chargement du fichier de récepteur de surface"),100,NULL,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE );

	if(recepteurSFiles.size()==0)
		return false;
	using namespace formatRSBIN;
	// Préparation de la chaine à afficher dans la vue 3D à partir du nom de dossier
	wxFileName filePath=recepteurSFiles[0];
	filePath.MakeRelativeTo(ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+ApplicationConfiguration::CONST_REPORT_FOLDER_PATH);
	//filePath.RemoveLastDir();
	//filePath.RemoveLastDir();
	filePath.ClearExt();
	filePath.SetName("");
	libellLegends.path=filePath.GetFullPath();
	// Chargement du(des) fichier(s)
	Init();
	float maxValue=130; //Echelle
	float minValue=0;
	float maxfacetread(0),minfacetread(0);
	//Propriétés iso
	renderIsoCurves=false;
	unsigned int iso_smooth=0;
	std::list<float> isolvls;
	if(elConf)
	{
		//Ajout du lien qui mettra à jour la configuration locale lors d'une modification
		elConf->AppendEventCatcher(EventCatcher(new RecepteurSUpdater(elConf,this)));
		xmlIdFile=elConf->GetElementInfos().xmlIdElement;
		opacity=elConf->GetDecimalConfig("surfacetransparency");
		renderFaceOne=elConf->GetBoolConfig("rendersideone");
		renderFaceTwo=elConf->GetBoolConfig("rendersidetwo");
		maxValue=elConf->GetDecimalConfig("maxinterval");
		minValue=elConf->GetDecimalConfig("mininterval");
		smoothColour=elConf->GetBoolConfig("smooth_color");
		renderIsoCurves=elConf->GetBoolConfig("showisolvl");
		iso_smooth=elConf->GetEntierConfig("isosmoothlvl");
		wxString isolvllst=elConf->GetStringConfig("isolvllist").Trim();
		if(isolvllst.size()>0)
		{
			wxArrayString isolvlchain;
			SplitString(isolvllst,";",isolvlchain);
			for(wxArrayString::iterator itiso=isolvlchain.begin();itiso!=isolvlchain.end();itiso++)
			{
				isolvls.push_back(Convertor::ToFloat(*itiso));
			}
		}
		RecordRecepteurSurfType=elConf->GetEntierConfig("rstype");
		libellLegends.unit=_("dB");
		if(RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_TR ||
			RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_EDT ||
			RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_TS)
		{
			libellLegends.unit=_("s");
		}else if(RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_DEFINITION){
			libellLegends.unit=_("%");
		}else if(RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_PRESSURE){
			libellLegends.unit=_("pascal");
		}
		libellLegends.labelEl=elConf->GetElementInfos().libelleElement;
	}
	libellLegends.v_min=minValue;
	libellLegends.v_max=maxValue;

	t_ExchangeData mainData;


	RSBIN importer;
	//////////////////////////////////////////////////////////
	//Importation du premier fichier de recepteurs surfaciques
	int lastprog=0;
	wxString mfilename=recepteurSFiles[0];
	if(importer.ImportBIN(mfilename,mainData))
	{
		//Importation du fichier de rendu reussi
		nbStep=mainData.nbTimeStep;
		timeStep=mainData.timeStep;
		if(loadingMethod==Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM)
		{
			nbStep=1;

		}
		//Copie en mémoire des noeuds
		nodesData.reserve(mainData.tabNodesSize);
		for(int idNode=0;idNode<mainData.tabNodesSize;idNode++)
		{
			nodesData.push_back(t_nodeS());
			t_nodeS* newNode=&nodesData[nodesData.size()-1];
			newNode->position=coordsOperation::CommonCoordsToGlCoords(UnitizeVal,mainData.tabNodes[idNode].node);
		}
		//Copie en mémoire des recepteurs surfacique
		progressionInfo progOutputManager(mainData.tabRsSize);
		recepteursS.reserve(mainData.tabRsSize);
		for(int idRs=0;idRs<mainData.tabRsSize;idRs++)
		{
			formatRSBIN::t_ExchangeData_Recepteurs& currentRS=mainData.tabRs[idRs];
			progressOperation opRs(progOutputManager.GetMainOperation(),currentRS.dataRec.quantFaces);
			recepteursS.push_back(t_recepteurS());
			t_recepteurS* newRecepteurss=&recepteursS[recepteursS.size()-1];

			//Pour chaque face composant le recepteur surfacique
			newRecepteurss->faces.reserve(currentRS.dataRec.quantFaces);
			for(int idFace=0;idFace<currentRS.dataRec.quantFaces;idFace++)
			{
				formatRSBIN::t_ExchangeData_Face& curFace=currentRS.dataFaces[idFace];
				progressOperation opface(&opRs);
				Intb* firstVal=curFace.dataFace.sommetsIndex;
				ivec3 faceSommets(firstVal[0],firstVal[1],firstVal[2]);
				newRecepteurss->faces.push_back(t_rs_face(faceSommets));
				t_rs_face* currentFace=&newRecepteurss->faces[newRecepteurss->faces.size()-1];
				//currentFace->normal=FaceNormal(nodesData[faceSommets.a].position,nodesData[faceSommets.b].position,nodesData[faceSommets.c].position);
				//Initialisation des noeuds liés à cette face
				for(int idSommet=0;idSommet<3;idSommet++)
				{
					nodesData[faceSommets[idSommet]].quantFacesLinked++;
					if(nodesData[faceSommets[idSommet]].energy.size()==0)
						nodesData[faceSommets[idSommet]].energy=std::vector<float>(nbStep);
				}
				// Pour chaque pas de temps
				currentFace->energy=std::vector<wxFloat32>(nbStep);
				if(lastprog!=int(progOutputManager.GetCurrentProgression()))
				{
					lastprog=int(progOutputManager.GetCurrentProgression());
					if(!progDialog.Update(lastprog))
						return false;
				}
				float sumEnergy=0;
				for(int idCol=0;idCol<curFace.dataFace.nbRecords;idCol++)
				{
					formatRSBIN::t_faceValue& curTimeStep=curFace.tabTimeStep[idCol];
					int nodeTimeStep=curTimeStep.timeStep;

					if(loadingMethod==Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM) //si le rendu doit se faire sur un seul pas de temps cumulé
						nodeTimeStep=0;
					if(nodeTimeStep>=nbStep)
					{
						wxLogError("File receiver corrupted, too enough timestep..");
						return false;
					}
					if(!wxFinite(curTimeStep.energy))
						curTimeStep.energy=0;
					currentFace->energy[nodeTimeStep]+=curTimeStep.energy;
					sumEnergy+=curTimeStep.energy;
					for(int idSommet=0;idSommet<3;idSommet++)
					{
						nodesData[faceSommets[idSommet]].energy[nodeTimeStep]+=dissipationFaceSurSommets*curTimeStep.energy;
					}
				}
				if(sumEnergy==0 || !wxFinite(sumEnergy))
				{
					nodesData[faceSommets[0]].quantFacesLinked--;
					nodesData[faceSommets[1]].quantFacesLinked--;
					nodesData[faceSommets[2]].quantFacesLinked--;
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////
		// Traitement des méthodes de chargement
		int QNodesTimeStep=0;
		if(loadingMethod==Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_BY_TIMESTEP_SUM)
		{
			for(int idRs=0;idRs<recepteursS.size();idRs++)
			{
				t_recepteurS* currentRs=&recepteursS[idRs];
				int nbFaces=currentRs->faces.size();
				for(int idFace=0;idFace<nbFaces;idFace++)
				{
					QNodesTimeStep=currentRs->faces[idFace].energy.size();
					for(int idts=1;idts<QNodesTimeStep;idts++)
					{
						currentRs->faces[idFace].energy[idts]+=currentRs->faces[idFace].energy[idts-1];
					}
				}
			}
			for(int idNode=0;idNode<mainData.tabNodesSize;idNode++)
			{
				QNodesTimeStep=nodesData[idNode].energy.size();
				for(int idts=1;idts<QNodesTimeStep;idts++)
				{
					nodesData[idNode].energy[idts]+=nodesData[idNode].energy[idts-1];
				}
			}
		}
		//////////////////////////////////////////////////////
		//  Conversion en dB
		if(RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_STANDART || RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_GAIN)
		{
			for(int idRs=0;idRs<recepteursS.size();idRs++)
			{
				t_recepteurS* currentRs=&recepteursS[idRs];
				int nbFaces=currentRs->faces.size();
				for(int idFace=0;idFace<nbFaces;idFace++)
				{
					QNodesTimeStep=currentRs->faces[idFace].energy.size();
					bool finiteDb=false;
					for(int idts=0;idts<QNodesTimeStep;idts++)
					{
						float* faceEnergy=&currentRs->faces[idFace].energy[idts];
						if(wxFinite(*faceEnergy) && (*faceEnergy)!=0 && ((*faceEnergy)>maxfacetread || maxfacetread==0))
							maxfacetread=(*faceEnergy);
						if(wxFinite(*faceEnergy) && (*faceEnergy)!=0 && ((*faceEnergy)<minfacetread || minfacetread==0))
							minfacetread=(*faceEnergy);
						if(!finiteDb && (*faceEnergy)>0 && wxFinite(*faceEnergy))
							finiteDb=true;

						(*faceEnergy)=10*log10f((*faceEnergy)/pow(10.f,-12.f));
					}
					if(!finiteDb)
						currentRs->faces[idFace].render=false;
				}
			}
			for(int idNode=0;idNode<mainData.tabNodesSize;idNode++)
			{
				QNodesTimeStep=nodesData[idNode].energy.size();
				for(int idts=0;idts<QNodesTimeStep;idts++)
				{
					//Conversion en dB
					if(nodesData[idNode].quantFacesLinked>0)
					{
						float* nodeEnergy=&nodesData[idNode].energy[idts];
							(*nodeEnergy)/=nodesData[idNode].quantFacesLinked;
						(*nodeEnergy)=10*log10f((*nodeEnergy)/pow(10.f,-12.f));
					}
				}
			}
		}else
		{
			for(int idRs=0;idRs<recepteursS.size();idRs++)
			{
				t_recepteurS* currentRs=&recepteursS[idRs];
				int nbFaces=currentRs->faces.size();
				for(int idFace=0;idFace<nbFaces;idFace++)
				{
					QNodesTimeStep=currentRs->faces[idFace].energy.size();
					bool finiteTime=false;
					for(int idts=0;idts<QNodesTimeStep;idts++)
					{
						float* faceTime=&currentRs->faces[idFace].energy[idts];
						if(!finiteTime && wxFinite(*faceTime))
							finiteTime=true;
					}
					if(!finiteTime)
						currentRs->faces[idFace].render=false;
				}
			}
		}

		//////////////////////////////////////////////////////
		// Pour le mode gradiant(couleur par noeud) de autre chose que l'energie, faire la moyenne de temps sur les noeuds, donc diviser la valeur par noeud par le nombre de faces contributrices
		if(!(RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_STANDART || RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_GAIN))
		{
			for(int idNode=0;idNode<mainData.tabNodesSize;idNode++)
			{
				QNodesTimeStep=nodesData[idNode].energy.size();
				for(int idts=0;idts<QNodesTimeStep;idts++)
				{
					//Moyenne de temps
					nodesData[idNode].energy[idts]/=nodesData[idNode].quantFacesLinked;
					if(wxFinite(nodesData[idNode].energy[idts]) && (nodesData[idNode].energy[idts]>maxfacetread || maxfacetread==0))
						maxfacetread=nodesData[idNode].energy[idts];
					if(wxFinite(nodesData[idNode].energy[idts]) && (nodesData[idNode].energy[idts]<minfacetread || minfacetread==0))
						minfacetread=nodesData[idNode].energy[idts];
				}
			}
		}

		if(	libellLegends.v_min==0 && libellLegends.v_max==0)
		{
			libellLegends.v_min=minfacetread;
			if(RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_STANDART || RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_GAIN)
				libellLegends.v_min=10*log10f(minfacetread/pow(10.f,-12.f));
			if(elConf)
				elConf->UpdateDecimalConfig("mininterval",libellLegends.v_min);
			libellLegends.v_max=maxfacetread;
			if(RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_STANDART || RecordRecepteurSurfType==(int)RECEPTEURS_RECORD_TYPE_SPL_GAIN)
				libellLegends.v_max=10*log10f(maxfacetread/pow(10.f,-12.f));
			if(elConf)
				elConf->UpdateDecimalConfig("maxinterval",libellLegends.v_max);

		}
		//Construction des iso_courbes
		if(renderIsoCurves && loadingMethod==Element::IDEVENT_LOAD_RECEPTEURSS_SIMULATION_SUM)
		{
			if(isolvls.size()==0)
			{
				float tick=ComputeBestIsoInterval(libellLegends.v_min,libellLegends.v_max);
				float counter=libellLegends.v_min-fmod(libellLegends.v_min , tick);;
				//(xMin)-(wxFloat32)fmod((xMin) , intervalLabel);
				wxString field_isolvl;
				while(counter+tick<libellLegends.v_max)
				{
					counter+=tick;
					field_isolvl+=Convertor::ToString(counter)+";";
					isolvls.push_back(counter);
				}
				field_isolvl=field_isolvl.Left(field_isolvl.size()-1);
				if(elConf)
				{
					elConf->UpdateStringConfig("isolvllist",field_isolvl);
				}
			}
			for(std::size_t idRs=0;idRs<recepteursS.size();idRs++)
			{
				t_recepteurS* newRecepteurss=&recepteursS[idRs];
				rsurf_data datars;
				datars.SetRsBinData(mainData);
				rsurf_iso_contouring contouringTool(datars,idRs);
				//contouringTool.SmoothGrid(iso_smooth);
				for(std::list<float>::iterator itlvls=isolvls.begin();itlvls!=isolvls.end();itlvls++)
				{
					newRecepteurss->iso_curves.push_back(t_isocurves());
					t_isocurves& new_isocurve=newRecepteurss->iso_curves.back();
					new_isocurve.isolvl=*itlvls;
					contouringTool.GetIsoLevelCurves(*itlvls,new_isocurve.segments);

					for(std::list<formatRSBIN::t_curve>::iterator itsegments=new_isocurve.segments.begin();itsegments!=new_isocurve.segments.end();itsegments++)
					{
						memcpy((*itsegments).A,coordsOperation::CommonCoordsToGlCoords(UnitizeVal,vec3((*itsegments).A)),sizeof(vec3));
						memcpy((*itsegments).B,coordsOperation::CommonCoordsToGlCoords(UnitizeVal,vec3((*itsegments).B)),sizeof(vec3));
					}

				}
			}
		}

		wxDateTime LegendloadingTime=wxDateTime::UNow();
		RedrawLegend();
		//wxLogDebug("Chargement du récepteur surfacique en %i ms",wxDateTime::UNow().GetValue()-fileLoading.GetValue());
		//wxLogDebug("Création de la légende en %i ms",wxDateTime::UNow().GetValue()-LegendloadingTime.GetValue());
		loadingTime=wxDateTime::UNow();
		return true;
	}else{
		return false;
	}
}
float RecepteursSControler::Ponderate(const float& val)
{
	//Pondération [0;1]
	if(val<=libellLegends.v_max)
	{
		if(val<libellLegends.v_min)
			return 0;
		else
			return (val-libellLegends.v_min)/(libellLegends.v_max-libellLegends.v_min);
	}
	else
	{
		return 1.f;
	}
}

vec4 RecepteursSControler::GetColor(const float& val)
{
	if(colorPalette.get())
	{
		float baseValue(Ponderate(val));
		int index=baseValue*(float)colorPaletteCount;
		if(index==colorPaletteCount)
			index-=1;
		vec4 color(colorPalette[index],opacity);
		return color;
	}else{
		return vec4();
	}
}

void RecepteursSControler::Render(const int& _timeStp)
{
	int timeStp=_timeStp;
	if(timeStp==-1)
		timeStp=timeStep;

	//Rendu des isocourbes
	if(this->nbStep==1 && renderIsoCurves)
	{
		glDisable(GL_LIGHTING);
		glColor3fv(vec3(isolinecolor.red,isolinecolor.green,isolinecolor.blue)/255);
		glLineWidth(1);

		//glEnable(GL_LINE_STIPPLE);
		//glLineStipple (1, 0x0C0F);
		//glBegin (GL_LINE_STRIP);


		glBegin(GL_LINES);
		for(std::vector<t_recepteurS>::iterator itrs=recepteursS.begin();itrs!=recepteursS.end();itrs++)
		{
			for(std::list<t_isocurves>::iterator itisolvl=(*itrs).iso_curves.begin();itisolvl!=(*itrs).iso_curves.end();itisolvl++)
			{
				for(std::list<formatRSBIN::t_curve>::iterator itsegments=(*itisolvl).segments.begin();itsegments!=(*itisolvl).segments.end();itsegments++)
				{
					glVertex3fv((*itsegments).A);
					glVertex3fv((*itsegments).B);
				}
			}
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LESS); //si supeposition, on affiche bien la sélection par dessus

	if(renderFaceOne != renderFaceTwo)
	{
		glEnable(GL_CULL_FACE);//Pas de test de direction dans le dessin

		if(renderFaceOne)
			glCullFace(GL_BACK );
		else
			glCullFace(GL_FRONT );
	}else{
		glDisable(GL_CULL_FACE);//Pas de test de direction dans le dessin
	}
	glDisable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D);
	if(this->opacity==1.f)
		glDisable(GL_BLEND);
	else
		glEnable(GL_BLEND);

	glDisable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.f,1.f);
	glBegin(GL_TRIANGLES);

	for(std::vector<t_recepteurS>::iterator itrs=recepteursS.begin();itrs!=recepteursS.end();itrs++)
	{
		for(std::vector<t_rs_face>::iterator itface=(*itrs).faces.begin();itface!=(*itrs).faces.end();itface++)
		{
			if((*itface).render)
			{
				if(!smoothColour)
					glColor4fv(GetColor((*itface).energy[timeStp]));
				if(smoothColour)
					glColor4fv(GetColor(nodesData[(*itface).sommets[0]].energy[timeStp]));
				glVertex3fv(nodesData[(*itface).sommets[0]].position);
				if(smoothColour)
					glColor4fv(GetColor(nodesData[(*itface).sommets[1]].energy[timeStp]));
				glVertex3fv(nodesData[(*itface).sommets[1]].position);
				if(smoothColour)
					glColor4fv(GetColor(nodesData[(*itface).sommets[2]].energy[timeStp]));
				glVertex3fv(nodesData[(*itface).sommets[2]].position);
			}
		}
	}
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDepthFunc(GL_LESS);

}

int RecepteursSControler::GetNbTimeStep()
{
	return nbStep;
}

RecepteursSControler::t_rs_face* RecepteursSControler::GetFaceByCoord(vec3 insideDotCoords)
{
	vec3 dotTranslation;
	#ifdef _DEBUG
		wxLogDebug("Get Position[%f;%f;%f]",insideDotCoords.x,insideDotCoords.y,insideDotCoords.z);
	#endif
	float ecart,pfs,pft,minecart(-1);
	t_rs_face* bestFace(NULL);
	for(int idRs=0;idRs<recepteursS.size();idRs++)
	{
		t_recepteurS* currentRs=&recepteursS[idRs];
		int nbFaces=currentRs->faces.size();
		for(int idFace=0;idFace<nbFaces;idFace++)
		{
			t_rs_face* currentFace=&currentRs->faces[idFace];
			ecart=ClosestDistanceBetweenDotAndTriangle(nodesData[currentFace->sommets.a].position*100,nodesData[currentFace->sommets.b].position*100,nodesData[currentFace->sommets.c].position*100,insideDotCoords*100,&pfs,&pft);

			if(ecart<0.02)
			{
				#ifdef _DEBUG
					wxLogDebug("Ecart de %f",ecart);
				#endif
				return currentFace;
			}
			if(ecart<minecart || minecart==-1)
			{
				minecart=ecart;
				bestFace=currentFace;
			}
		}
	}
	#ifdef _DEBUG
		wxLogDebug("Ecart mini de %f",minecart);
	#endif
	if(minecart<1.)
		return bestFace;
	else
		return NULL;
}

bool RecepteursSControler::UserWantToKnowAcousticLevelAtPosition( vec3 extractPosition )
{
	t_rs_face* selectedFace=GetFaceByCoord(extractPosition);
	if(selectedFace)
	{
		if(selectedFace->energy.size()>idStep)
		{
			float energyValue=selectedFace->energy[idStep];
			if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_SPL_STANDART || RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_SPL_GAIN)
			{
				if(wxFinite(energyValue))
					wxLogInfo(_("Le niveau sonore à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
				else
					wxLogInfo(_("Niveau acoustique nul."));
			}else{
					if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_TR)
						wxLogInfo(_("La valeur du TR à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
					else if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_CLARITY)
						wxLogInfo(_("La valeur de la clarté à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
					else if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_DEFINITION)
						wxLogInfo(_("La valeur de la définition à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
					else if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_EDT)
						wxLogInfo(_("La valeur du TS à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
					else if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_EDT)
						wxLogInfo(_("La valeur du ST à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
					else if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_PRESSURE)
						wxLogInfo(_("La valeur de la pression à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
					else if(RecordRecepteurSurfType==(int)formatRSBIN::RECEPTEURS_RECORD_TYPE_EDT)
						wxLogInfo(_("La valeur de l'EDT à cet endroit est de %.2f ")+libellLegends.unit,energyValue);
			}
		}
	}else{
		wxLogError(_("Cette zone ne correspond pas à un récepteur de surface."));
	}
	return false;
}

void RecepteursSControler::InitLegend(legendRendering::ForeGroundGlBitmap& _legendRenderer)
{

	legendRenderer=&_legendRenderer;




}
void RecepteursSControler::SetIsoLineColor(const ivec3& _isolineColor)
{
	isolinecolor=_isolineColor;
}


void RecepteursSControler::RedrawLegend()
{
	if(this->recepteursS.size()==0 || !this->IsRenderingEnable())
		return;

	wxString precision("%.1f %s");
	if(libellLegends.unit=="s")
		precision="%.3f %s";
	libellLegends.maxValue=wxString::Format(precision,libellLegends.v_max,libellLegends.unit);
	libellLegends.avgValue=wxString::Format(precision,(libellLegends.v_max-libellLegends.v_min)/2+libellLegends.v_min,libellLegends.unit);
	libellLegends.minValue=wxString::Format(precision,libellLegends.v_min,libellLegends.unit);
	////////////////////////////////////////////////////////////////////////////
	// Generation de la barre de légende de niveaux sonore
	for(int idligne=0;idligne<LevelGraphHeight;idligne++)
	{
		int index=(1-(float)idligne/LevelGraphHeight)*(float)colorPaletteCount;
		if(index==colorPaletteCount)
			index-=1;

		if(colorPalette.get())
		{
			int color[3]={colorPalette[index].r*255,colorPalette[index].g*255,colorPalette[index].b*255};
			int lineoffset(idligne*LevelGraphWidth*LevelGraphBpp);
			for(int idcol=0;idcol<LevelGraphWidth;idcol++)
			{
				int coloffset=idcol*LevelGraphBpp;
				LevelGraphbytes[lineoffset + coloffset]=color[0];
				LevelGraphbytes[lineoffset + coloffset + 1]=color[1];
				LevelGraphbytes[lineoffset + coloffset + 2]=color[2];
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
	// Ajout des objet de légendes
	using namespace legendRendering;
	// Barre à gauche
	if(legendRenderer)
	{
		legendText* gradLegend;
		if(!this->rs_legends.graduationBar)
		{
			gradLegend=new legendText();
			gradLegend->Init(LevelGraphWidth,LevelGraphHeight);
		}else{
			gradLegend=this->rs_legends.graduationBar;
		}

		//Ajout des libellés de niveaux

		gradLegend->SetFont( legendFont );

		gradLegend->SetTextForeground(text_foreground_color);
		gradLegend->SetTextBackground(text_background_color);

		//Ajout des lignes sur la légende
		if(!recepteursS.empty())
		{
			for(std::list<t_isocurves>::iterator itisolvl=(*recepteursS.begin()).iso_curves.begin();itisolvl!=(*recepteursS.begin()).iso_curves.end();itisolvl++)
			{
				int ypos=(LevelGraphHeight-LevelGraphHeight*(((*itisolvl).isolvl-libellLegends.v_min)/(libellLegends.v_max-libellLegends.v_min)));
				int offset=ypos*LevelGraphWidth*LevelGraphBpp;
				if(offset>=0 && offset<sizeof(LevelGraphbytes))
				{
					for(unsigned short idcol=0;idcol<LevelGraphWidth;idcol++)
					{
						int coloffset=idcol*LevelGraphBpp;
						LevelGraphbytes[offset+ coloffset]=isolinecolor.red;
						LevelGraphbytes[offset+ coloffset+1]=isolinecolor.green;
						LevelGraphbytes[offset+ coloffset+2]=isolinecolor.blue;
					}
				}
					//memset(&LevelGraphbytes[offset],0,LevelGraphWidth*LevelGraphBpp*sizeof(char));
			}
		}
		gradLegend->Copy(LevelGraphbytes,NULL,LevelGraphWidth,LevelGraphHeight);
		//Ajout du texte

		if(!recepteursS.empty() && this->renderIsoCurves && this->nbStep==1)
		{
			for(std::list<t_isocurves>::iterator itisolvl=(*recepteursS.begin()).iso_curves.begin();itisolvl!=(*recepteursS.begin()).iso_curves.end();itisolvl++)
			{

				wxString textlgd=wxString::Format(precision,(*itisolvl).isolvl,libellLegends.unit);
				wxSize txtSize=gradLegend->GetTextSize(textlgd);
				int ypos=(LevelGraphHeight-LevelGraphHeight*(((*itisolvl).isolvl-libellLegends.v_min)/(libellLegends.v_max-libellLegends.v_min)));
				if(ypos<0)
					ypos=0;
				if(ypos+txtSize.GetHeight()>LevelGraphHeight)
					ypos=LevelGraphHeight-txtSize.GetHeight();
				gradLegend->AppendText(textlgd,0, ypos);
			}
		}else{
			wxSize txtSize=gradLegend->GetTextSize(libellLegends.maxValue);
			gradLegend->AppendText(libellLegends.maxValue,0, 0);
			gradLegend->AppendText(libellLegends.avgValue,0, LevelGraphHeight/2.f-txtSize.GetHeight()/2);
			gradLegend->AppendText(libellLegends.minValue,0, LevelGraphHeight-txtSize.GetHeight());
		}

		if(!this->rs_legends.graduationBar)
		{
			this->rs_legends.graduationBar=gradLegend;
			legendCfg currentObjConf=this->rs_legends.graduationBar->GetCfg();
			currentObjConf.redim=false;
			currentObjConf.redim_width_perc=.11f;
			currentObjConf.redim_height_perc=.8f;
			currentObjConf.alignementV=ALIGNEMENT_V_CENTER;
			currentObjConf.alignementH=ALIGNEMENT_H_LEFT;
			currentObjConf.border_left=10;
			currentObjConf.border_bottom=20;
			this->rs_legends.graduationBar->SetCfg(currentObjConf);
			legendRenderer->Push(this->rs_legends.graduationBar);
		}
	}
	//Nom du fichier de recepteur surfacique
	if(legendRenderer)
	{

		bool pushLegend=false;
		if(!this->rs_legends.currentFile)
		{
			this->rs_legends.currentFile=new legendText();
			pushLegend=true;
		}
		this->rs_legends.currentFile->SetFont( legendFont);
		this->rs_legends.currentFile->SetTextForeground(text_foreground_color);
		this->rs_legends.currentFile->SetTextBackground(text_background_color);
		this->rs_legends.currentFile->InitText(_("Récepteurs surfaciques : ")+libellLegends.path);
		if(pushLegend)
		{
			legendCfg currentObjConf=this->rs_legends.currentFile->GetCfg();
			currentObjConf.alignementV=ALIGNEMENT_V_BOTTOM;
			currentObjConf.alignementH=ALIGNEMENT_H_LEFT;
			this->rs_legends.currentFile->SetCfg(currentObjConf);
			legendRenderer->Push(this->rs_legends.currentFile);
		}

	}
	//Pas de temps courant
	if(legendRenderer)
	{
		if(!this->rs_legends.currentTimeStep)
		{
			legendText* tmstepLegend=new legendText();
			legendCfg currentObjConf=tmstepLegend->GetCfg();
			currentObjConf.alignementV=ALIGNEMENT_V_BOTTOM;
			currentObjConf.alignementH=ALIGNEMENT_H_LEFT;
			tmstepLegend->SetCfg(currentObjConf);
			tmstepLegend->SetFont(legendFont);
			tmstepLegend->SetTextForeground(text_foreground_color);
			tmstepLegend->SetTextBackground(text_background_color);

			if(this->nbStep==1)
			{
				if(RecordRecepteurSurfType==formatRSBIN::RECEPTEURS_RECORD_TYPE_SPL_STANDART)
					tmstepLegend->InitText(_("Niveau sonore cumulé"));
				else
					tmstepLegend->InitText(this->libellLegends.labelEl);
			}else{
				tmstepLegend->InitText("0.000 s");
			}
			legendRenderer->Push(tmstepLegend);
			this->rs_legends.currentTimeStep=tmstepLegend;
		}else{
			this->rs_legends.currentTimeStep->SetFont(legendFont);
			this->rs_legends.currentTimeStep->SetTextForeground(text_foreground_color);
			this->rs_legends.currentTimeStep->SetTextBackground(text_background_color);
			this->SetTimeStep(idStep);
		}
	}
}
