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

#include "recepteursurf_difference.hpp"
#include <wx/filename.h>
#include <wx/log.h>
#include "last_cpp_include.hpp"

RecepteurSurfAttenuationComputation::RecepteurSurfAttenuationComputation(wxString rsurfReferenceFile)
{
	if(!LoadRs(rsurfReferenceFile,&referenceRS))
	{
		wxLogError(_("Impossible de charger le récepteur surfacique de référence :\n%s"),rsurfReferenceFile);
	}
}
/*
//Methode de difference sur chaque pas de temps
bool RecepteurSurfAttenuationComputation::SaveAttenuationRecepteurSurfFile(wxString rsurfSecondFile, wxString destinationResultFile )
{
	using namespace formatRSBIN;
	//Chargement du recepteur de surface
	t_process_data secondRecepteurSurf;
	if(LoadRs(rsurfSecondFile,&secondRecepteurSurf))
	{
		if(secondRecepteurSurf.rawData.nbTimeStep!=referenceRS.rawData.nbTimeStep)
		{
			wxLogError(_("Impossible de comparer ce récepteur surfacique car le nombre de pas de temps est différend que celui du fichier de référence :\n%s"),rsurfSecondFile);
			return false;
		}
		if(secondRecepteurSurf.rawData.timeStep!=referenceRS.rawData.timeStep)
		{
			wxLogError(_("Impossible de comparer ce récepteur surfacique car le pas de temps (delta T) est différend que celui du fichier de référence :\n%s"),rsurfSecondFile);
			return false;
		}
		if(secondRecepteurSurf.rawData.tabRsSize!=referenceRS.rawData.tabRsSize)
		{
			wxLogError(_("Impossible de comparer avec le fichier de référence car il n'y a pas le même nombre de récepteur de surface :\n%s"),rsurfSecondFile);
			return false;
		}
		wxFloat32 energyRef=0;
		wxFloat32 energyRS=0;
		wxFloat32 energyDiff=0;
		wxInt32 indexRec=-1;
		for(wxInt32 idrs=0;idrs<secondRecepteurSurf.rawData.tabRsSize;idrs++) //Pour chaque récepteur de surface
		{
			if(secondRecepteurSurf.rawData.tabRs[idrs].dataRec.quantFaces!=referenceRS.rawData.tabRs[idrs].dataRec.quantFaces)
			{
				wxLogError(_("La structure du récepteur de surface suivant ne contient pas le même nombre de surfaces que le fichier de référence :\n%s"),rsurfSecondFile);
				return false;				
			}
			for(wxInt32 idface=0;idface<secondRecepteurSurf.rawData.tabRs[idrs].dataRec.quantFaces;idface++) //Pour chaque face
			{
				std::vector<t_faceValue> newFaceValues;
				newFaceValues.reserve(secondRecepteurSurf.rawData.nbTimeStep);
				for(wxInt32 idstep=0;idstep<secondRecepteurSurf.rawData.nbTimeStep;idstep++) //Pour chaque pas de temps
				{
					energyRef=0;
					energyRS=0;
					energyDiff=0;
					//Lecture energie du recepteur de surface à évaluer
					indexRec=secondRecepteurSurf.recepteurSurf[idrs].faces[idface].timeStepToIndex[idstep];
					if(indexRec!=-1)
					{
						energyRS=secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep[indexRec].energy;
					}
					//Lecture energie du recepteur de surface de référence
					indexRec=referenceRS.recepteurSurf[idrs].faces[idface].timeStepToIndex[idstep];
					if(indexRec!=-1)
					{
						energyRef=referenceRS.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep[indexRec].energy;
					}
					//Calcul de l'attenuation
					//Todo, evaluer l'effet de p0
					if(energyRS==0)
						energyRS=EPSILON; //L'energie doit être non nul pour être en mesure d'exprimer une attenuation non illimitée
					if(energyRef==0)
						energyRef=EPSILON; //L'energie doit être non nul pour être en mesure d'exprimer une attenuation non illimitée
					energyDiff=(energyRef/energyRS)*pow(10.f,-12.f);
					newFaceValues.push_back(t_faceValue(idstep,energyDiff));
				}
				//Copie des nouvelles valeurs
				secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords=newFaceValues.size();
				delete[] secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep;
				if(newFaceValues.size()>0)
					secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep=new t_faceValue[newFaceValues.size()];
				else
					secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep=NULL;
				for(int idrecord=0;idrecord<newFaceValues.size();idrecord++)
				{
					secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep[idrecord]=newFaceValues[idrecord];
				}
			}
		}
		//Sauvegarde de la modification dans le dossier spécifié
		return SaveRs(destinationResultFile,secondRecepteurSurf);
	}else{
		wxLogError(_("Impossible de charger le récepteur surfacique suivant :\n%s"),rsurfSecondFile);
		return false;
	}
}
*/

bool RecepteurSurfAttenuationComputation::SaveAttenuationRecepteurSurfFile(wxString rsurfSecondFile, wxString destinationResultFile )
{
	using namespace formatRSBIN;
	//Chargement du recepteur de surface
	t_process_data secondRecepteurSurf;
	if(LoadRs(rsurfSecondFile,&secondRecepteurSurf))
	{
		if(secondRecepteurSurf.rawData.nbTimeStep!=referenceRS.rawData.nbTimeStep)
		{
			wxLogError(_("Impossible de comparer ce récepteur surfacique car le nombre de pas de temps est différend que celui du fichier de référence :\n%s"),rsurfSecondFile);
			return false;
		}
		if(secondRecepteurSurf.rawData.timeStep!=referenceRS.rawData.timeStep)
		{
			wxLogError(_("Impossible de comparer ce récepteur surfacique car le pas de temps (delta T) est différend que celui du fichier de référence :\n%s"),rsurfSecondFile);
			return false;
		}
		if(secondRecepteurSurf.rawData.tabRsSize!=referenceRS.rawData.tabRsSize)
		{
			wxLogError(_("Impossible de comparer avec le fichier de référence car il n'y a pas le même nombre de récepteur de surface :\n%s"),rsurfSecondFile);
			return false;
		}
		wxFloat32 energyRef=0;
		wxFloat32 energyRS=0;
		wxFloat32 energyDiff=0;
		wxInt32 indexRec=-1;
		for(wxInt32 idrs=0;idrs<secondRecepteurSurf.rawData.tabRsSize;idrs++) //Pour chaque récepteur de surface
		{
			if(secondRecepteurSurf.rawData.tabRs[idrs].dataRec.quantFaces!=referenceRS.rawData.tabRs[idrs].dataRec.quantFaces)
			{
				wxLogError(_("La structure du récepteur de surface suivant ne contient pas le même nombre de surfaces que le fichier de référence :\n%s"),rsurfSecondFile);
				return false;				
			}
			for(wxInt32 idface=0;idface<secondRecepteurSurf.rawData.tabRs[idrs].dataRec.quantFaces;idface++) //Pour chaque face
			{
				energyRef=0;
				energyRS=0;
				energyDiff=0;
				for(wxInt32 idenr=0;idenr<secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords;idenr++)
				{
					energyRS+=secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep[idenr].energy;
				}				
				for(wxInt32 idenr=0;idenr<referenceRS.rawData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords;idenr++)
				{
					energyRef+=referenceRS.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep[idenr].energy;
				}
				//Calcul de l'attenuation
				//Todo, evaluer l'effet de p0
				if(energyRS==0)
					energyRS=EPSILON; //L'energie doit être non nul pour être en mesure d'exprimer une attenuation non illimitée
				if(energyRef==0)
					energyRef=EPSILON; //L'energie doit être non nul pour être en mesure d'exprimer une attenuation non illimitée
				energyDiff=(energyRS/energyRef)*pow(10.f,-12.f);
				//Copie des nouvelles valeurs
				secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords=1;
				delete[] secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep;
				secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep=new  t_faceValue[1];
				secondRecepteurSurf.rawData.tabRs[idrs].dataFaces[idface].tabTimeStep[0]=t_faceValue(0,energyDiff);
			}
		}
		secondRecepteurSurf.rawData.nbTimeStep=1;
		secondRecepteurSurf.rawData.recordType=RECEPTEURS_RECORD_TYPE_SPL_GAIN;
		//Sauvegarde de la modification dans le dossier spécifié
		return SaveRs(destinationResultFile,secondRecepteurSurf);
	}else{
		wxLogError(_("Impossible de charger le récepteur surfacique suivant :\n%s"),rsurfSecondFile);
		return false;
	}
}
bool RecepteurSurfAttenuationComputation::LoadRs(wxString pathRsFile,t_process_data* dataDestination)
{
	using namespace formatRSBIN;
	RSBIN fileLoader;
	if(fileLoader.ImportBIN(pathRsFile,dataDestination->rawData))
	{
		//Afin d'optimiser les performances de traitements il faut établir les tables de correspondance entre les pas de temps et l'indices dans les tableaux d'energies
		for(wxInt32 idrs=0;idrs<dataDestination->rawData.tabRsSize;idrs++)
		{
			dataDestination->recepteurSurf.push_back(t_r_surfprocess());
			t_r_surfprocess* currentRs=&dataDestination->recepteurSurf[dataDestination->recepteurSurf.size()-1];
			currentRs->faces=std::vector<t_rs_face>(dataDestination->rawData.tabRs[idrs].dataRec.quantFaces);
			for(wxInt32 idface=0;idface<currentRs->faces.size();idface++)
			{
				t_rs_face* currentFace=&currentRs->faces[idface];
				currentFace->timeStepToIndex=std::vector<wxInt32>(dataDestination->rawData.nbTimeStep,-1);
				for(wxInt32 idenr=0;idenr<dataDestination->rawData.tabRs[idrs].dataFaces[idface].dataFace.nbRecords;idenr++)
				{
					currentFace->timeStepToIndex[dataDestination->rawData.tabRs[idrs].dataFaces[idface].tabTimeStep[idenr].timeStep]=idenr;
				}
			}
		}
	}
	return true;
}

bool RecepteurSurfAttenuationComputation::SaveRs(wxString pathRsFile,t_process_data& dataSource)
{
	using namespace formatRSBIN;
	RSBIN fileSaver;
	return fileSaver.ExportBIN(pathRsFile,dataSource.rawData);
}