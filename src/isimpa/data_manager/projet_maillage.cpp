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
 * @brief Specification des méthode de ProjectManager ayant un rapport avec le maillage
 */
#include "projet.h"
#include "manager/processManager.h"
#include <tools/vol_identifier.hpp>
#include <tools/vol_splitter.hpp>
#include "tree_scene/e_scene_groupesurfaces_groupe.h"
#include "last_cpp_include.hpp"
#include "logger_tetgen_debug.hpp"

bool ProjectManager::RunCoreMaillage(Element* selectedCore)
{
	if(selectedCore)
	{

		Element* configMaillage=selectedCore->GetElementByType(Element::ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE);
		if(configMaillage) //si c'est tetgen le mailleur
		{

			param_TetGenMaillage parametresMaillage;
			parametresMaillage.isMaxVolume=configMaillage->GetBoolConfig("ismaxvol");
			parametresMaillage.maxVolume=configMaillage->GetDecimalConfig("maxvol");
			parametresMaillage.quality=configMaillage->GetDecimalConfig("minratio");
			parametresMaillage.cmd_append=configMaillage->GetStringConfig("appendparams");
			parametresMaillage.user_defined_params=configMaillage->GetStringConfig("userdefineparams");
			parametresMaillage.maxAreaOnRecepteurss=configMaillage->GetDecimalConfig("constraintrecepteurss");
			parametresMaillage.isAreaConstraint=configMaillage->GetBoolConfig("isareaconstraint");
			parametresMaillage.doMeshRepair=configMaillage->GetBoolConfig("preprocess");
			parametresMaillage.debugMode=configMaillage->GetBoolConfig("debugmode");
			return RunTetGenMaillage(parametresMaillage);

		}
		return true;
	}
	return false;
}

bool ProjectManager::RunRemeshProcess(wxString fileToRemesh)
{
	wxString MeshRegenPath=ApplicationConfiguration::CONST_PREPROCESS_EXE_PATH;
	wxString MeshRegenExe=ApplicationConfiguration::CONST_PREPROCESS_EXE_FILENAME;

	wxString lblOutput=MeshRegenExe+" - ";
	wxString cmd=MeshRegenExe;
	cmd+=" "+fileToRemesh;

	//On attend que l'execution soit terminée

	wxProgressDialog progDialog(_("Reparing 3D scene"),_("Preparing 3D scene"),10000,mainFrame,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );

	bool result=uiRunExe(mainFrame,MeshRegenPath+cmd,lblOutput,&progDialog);

	if(result)
	{
		wxLogMessage(_("Scene repair complete"));
		return true;
	}else{
		wxLogError(_("Scene repair canceled"));
		return false;
	}
}
bool ProjectManager::RunTetGenBoundaryMesh( wxString cmd, wxString cacheFolder,wxString sceneName, wxString sceneNameExt)
{
	wxDateTime timeDebOperation=wxDateTime::UNow();
	wxString meshFilePath(cacheFolder+sceneName+"."+sceneNameExt);

	wxString tetgenPath=ApplicationConfiguration::CONST_TETGEN_EXE_PATH;
	wxString tetgenExe=ApplicationConfiguration::CONST_TETGEN_EXE_FILENAME;
	wxString lblOutput=tetgenExe+" : ";
	cmd=tetgenExe+" "+cmd+" \""+meshFilePath+"\"";

	wxLogMessage(_("Meshing with the following parameters:\n%s"),cmd);

	///////////////////////////////////////////
	///	Verifications de l'existance du coeur de calcul
	///////////////////////////////////////////
	#ifdef __WXMSW__
	if(!wxFileExists(tetgenPath+tetgenExe))
	{
		wxLogMessage(_("Calculation program file not found."));
		return false;
	}
	#endif

	///////////////////////////////////////////
	///	Suppression des anciens fichier si existant
	///////////////////////////////////////////
	wxString face=cacheFolder+sceneName+".1.face";
	wxString node=cacheFolder+sceneName+".1.node";

	
	///////////////////////////////////////////
	///	On execute le logiciel de calcul
	///////////////////////////////////////////
	wxProgressDialog progDialog(_("Code execution mesh generation"),_("Generating mesh"),10000,mainFrame,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );

	bool result=uiRunExe(mainFrame,tetgenPath+cmd,lblOutput,&progDialog);

	if(result)
	{
		wxLogMessage(_("Meshing complete"));

		///////////////////////////////////////////
		///	On transfert les données vers l'objet de la scène
		///////////////////////////////////////////
		wxLogMessage(_("Loading mesh ASCII file ..."));
		sceneMesh._LoadFaceFile(face.ToStdString());
		wxLogMessage(_("Loading ASCII files from mesh generator complete"));
	}
	
	wxLongLong durationOperation=wxDateTime::UNow().GetValue()-timeDebOperation.GetValue();
	wxLogMessage(_("Total time of meshing: %lld ms"),durationOperation.GetValue());
	if(!result)
		wxLogError(_("Meshing aborted: please, see console messages for more information "));
	return true;


}

bool ProjectManager::RunTetGenMaillage(param_TetGenMaillage& paramMaillage)
{
	wxDateTime timeDebOperation=wxDateTime::UNow();
	wxString tetgenPath=ApplicationConfiguration::CONST_TETGEN_EXE_PATH;
	wxString tetgenExe=ApplicationConfiguration::CONST_TETGEN_EXE_FILENAME;
	wxString cacheFolder=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+"temp"+wxFileName::GetPathSeparator();
	if(!wxDirExists(cacheFolder))
		wxMkdir(cacheFolder);
	this->ClearFolder(cacheFolder);
	wxString sceneName="scene_mesh";
	wxString sceneNameExt=".poly";
	wxString cmd=tetgenExe;
	wxString lblOutput=tetgenExe+" : ";
	std::vector<t_faceIndex> faceInd;

	//Construction de la chaine d'arguments de l'executable
	if(paramMaillage.user_defined_params=="")
	{
		if(!paramMaillage.debugMode)
		{
			if(paramMaillage.isMaxVolume)
				cmd+=" -a"+Convertor::ToString(paramMaillage.maxVolume,".");
			cmd+=" -pq"+Convertor::ToString(paramMaillage.quality,".");
			cmd+=" -A -n "+paramMaillage.cmd_append;
		}else{
			cmd+=" -d";
			wxLogWarning(_("Debug mode mesh, nothing will be done"));
		}
	}else{
		cmd+=" "+paramMaillage.user_defined_params;
		wxLogWarning(_("Meshing with user parameters"));
	}
	wxString meshFilePath=cacheFolder+sceneName+sceneNameExt;
	wxString constraintFilePath=cacheFolder+sceneName+".var";
	cmd+=" \""+meshFilePath+"\"";

	///////////////////////////////////////////
	///	Verifications de l'existance du coeur de calcul
	///////////////////////////////////////////
	#ifdef __WXMSW__
	if(!wxFileExists(tetgenPath+tetgenExe))
	{
		wxLogMessage(_("Calculation .exe file not found."));
		return false;
	}
    #endif
	///////////////////////////////////////////
	///	Suppression des anciens fichier si existant
	///////////////////////////////////////////
	wxString face=cacheFolder+sceneName+".1.face";
	wxString ele=cacheFolder+sceneName+".1.ele";
	wxString node=cacheFolder+sceneName+".1.node";
	wxString neigh=cacheFolder+sceneName+".1.neigh";

	///////////////////////////////////////////
	///	Conversion de la scène au format POLY
	///////////////////////////////////////////

	if(!sceneMesh._SavePOLY(meshFilePath.ToStdString(),true,paramMaillage.doMeshRepair,true,&faceInd))
	{
		wxLogError(_("No 3D scene for meshing"));
		return false;
	}
	// Correction du modèle
	if(paramMaillage.doMeshRepair && !paramMaillage.debugMode)
		RunRemeshProcess(meshFilePath);
	///////////////////////////////////////////
	/// Création du fichier de contrainte
	///////////////////////////////////////////
	if(paramMaillage.isAreaConstraint && !paramMaillage.debugMode)
	{
		if(!sceneMesh.BuildVarConstraintFile(constraintFilePath.ToStdString(),paramMaillage.maxAreaOnRecepteurss))
		{
			wxLogError(_("Constraint file can not be created"));
			return false;
		}
	}
	///////////////////////////////////////////
	///	On execute le logiciel de calcul
	///////////////////////////////////////////
	wxProgressDialog progDialog(_("Code execution mesh generation"),_("Generating mesh"),10000,mainFrame,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );

	wxLogMessage(_("Meshing with the following parameters:\n%s"),cmd);
	TetgenDebugLogger* logger=new TetgenDebugLogger(); //Do not delete smart_ptr will do this
	smart_ptr<InterfLogger> errorCollector=smart_ptr<InterfLogger>(logger);
	bool result=uiRunExe(mainFrame,tetgenPath+cmd,lblOutput,&progDialog,errorCollector);

	if(result)
	{
		wxLogMessage(_("Meshing complete"));

		///////////////////////////////////////////
		///	On transfert les données vers l'objet de la scène
		///////////////////////////////////////////
		if(!paramMaillage.debugMode)
		{
            progDialog.Update(50, _("Loading mesh ASCII file ..."));
			wxLogMessage(_("Loading mesh ASCII file ..."));
			sceneMesh.LoadMaillage(WXSTRINGTOSTDSTRING(face),WXSTRINGTOSTDSTRING(ele),WXSTRINGTOSTDSTRING(node),WXSTRINGTOSTDSTRING(neigh));
			wxLogMessage(_("Loading ASCII files from mesh generator complete"));
		}else{
			std::vector<int>& faces=logger->GetFaces();
			
			std::vector<t_faceIndex> faceInErr;
			faceInErr.reserve(faceInd.size());
			for(std::vector<int>::iterator itf=faces.begin();itf!=faces.end();itf++)
			{
				int errIndex=(*itf)-1; //First face in tetgen is 1 not 0
				if(errIndex>-1 && errIndex<faceInd.size())
				{
					faceInErr.push_back(faceInd[errIndex]);
				}else{
					#ifdef _DEBUG
					wxLogDebug("Tetgen debug face index out of bound !");
					#endif
				}
			}
			this->OnSelectVertex(faceInErr,false);
		}
	}
	
	wxLongLong durationOperation=wxDateTime::UNow().GetValue()-timeDebOperation.GetValue();
	wxLogMessage(_("Total time of meshing: %lld ms"),durationOperation.GetValue());
	if(!result)
		wxLogError(_("Meshing aborted: please, see console messages for more information "));
	return true;
}

void ProjectManager::OnConvertSubVolumeToFitting(Element* volume_el)
{
	Element* newEncEl=this->OnMenuNewEncombrement(this->rootScene->GetElementByType(Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS),Element::ELEMENT_TYPE_SCENE_ENCOMBREMENTS_ENCOMBREMENT);
	if(!newEncEl)
		return;
	//Il faut affecter les faces
	E_Scene_Groupesurfaces_Groupe* groupSurfVol=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(volume_el->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE));
	if(!groupSurfVol)
		return;
	std::vector<t_faceIndex> facesToMove;
	groupSurfVol->GetFaces(facesToMove);

	E_Scene_Groupesurfaces_Groupe* groupFitting=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(newEncEl->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE));
	if(!groupFitting)
		return;
	for(std::vector<t_faceIndex>::iterator it=facesToMove.begin();it!=facesToMove.end();it++)
	{
		groupFitting->AddFace((*it).f,(*it).g);
	}
	//On affecte la position
	vec3 posVol=volume_el->GetPositionConfig("volpos");
	newEncEl->UpdatePositionConfig("volpos",posVol);
	//On supprime le volume
	volume_el->GetElementParent()->DeleteElementByXmlId(volume_el->GetXmlId());
}
void ProjectManager::OnFindSubVolumes(Element* volumes_el)
{
	volumes_el->DeleteAllElementByType(Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME);
	formatMBIN::trimeshmodel tet_model;
	formatCoreBIN::ioModel modelExport;
	this->sceneMesh.ToCBINFormat(modelExport);
	this->sceneMesh.GetTetraMesh(tet_model,true);

	if(tet_model.tetrahedres.size()>0)
	{
		formatMBIN::trimeshmodel tet_model_origin=tet_model;
		//1ere étape, associe un nombre à chaque sous-domaine
		volume_identifier::VolumeIdentifier::IdentifyVolumes(tet_model);
		//2eme étape, regrouper les tétraèdres par domaine
		volumes_splitter::VolumesSplitter splitter;
		splitter.LoadDomain(modelExport,tet_model);
		wxLogMessage("%i volumes ont été détectés.",splitter.GetVolumes());
		for(int idgrp=0;idgrp<splitter.GetVolumes();idgrp++)
		{
			std::vector<std::size_t> internal_faces;
			int xmlidvol=splitter.GetVolumeXmlId(idgrp);
			vec3 centre_vol;
			int idtetra=0;
			for(std::vector<formatMBIN::bintetrahedre>::iterator ittet=tet_model.tetrahedres.begin();ittet!=tet_model.tetrahedres.end();ittet++)
			{
				if((*ittet).idVolume==xmlidvol)
				{
					centre_vol=GetGTetra(tet_model.nodes[(*ittet).vertices[0]].node,tet_model.nodes[(*ittet).vertices[1]].node,tet_model.nodes[(*ittet).vertices[2]].node,tet_model.nodes[(*ittet).vertices[3]].node);
					break;
				}else{
					idtetra++;
				}
			}
			//Si le volume n'est pas un encombrement déjà défini par l'utilisateur
			if(tet_model_origin.tetrahedres[idtetra].idVolume==0)
			{
				splitter.GetInternalFaces(internal_faces,xmlidvol);
				Element* newvol=volumes_el->AppendFilsByType(Element::ELEMENT_TYPE_SCENE_VOLUMES_VOLUME);
				newvol->FillWxTree(this->treeScene);

				newvol->UpdatePositionConfig("volpos",centre_vol);
				E_Scene_Groupesurfaces_Groupe* grpNewEl=dynamic_cast<E_Scene_Groupesurfaces_Groupe*>(newvol->GetElementByType(Element::ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE));
				if(grpNewEl)
				{
					for(std::vector<std::size_t>::iterator itface=internal_faces.begin();itface!=internal_faces.end();itface++)
					{
						t_faceIndex idface=this->sceneMesh.FindFaceGroupWithFaceIndex((*itface));
						grpNewEl->AddFace(idface.f,idface.g);
					}
				}
			}
		}
	}else{
		wxLogError(_("You must launch the mesh operation before run this method."));
	}
}
