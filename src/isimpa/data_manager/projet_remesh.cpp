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

#include "projet.h"
#include "IHM/WizardRemeshModel.hpp"
#include "manager/processManager.h"
#include <wx/progdlg.h>
#include <wx/textfile.h>
#include <en_numeric.hpp>
#include "last_cpp_include.hpp"

namespace ModelReconstructionUI
{
	class ModelReconstructionManager : public ModelReconstructionManagerBase
	{
		protected:
			ReconstructionUserPreferences_t finalPreferences;
			wxString originalFileName;
			wxString outputFileDir;
			wxString outputFileName;
			std::list<wxString> modelOutputFileNames; //Un fichier de sortie par volume
			wxString GetVolumeInfoFileName()
			{
				return "VolumeInfo.txt";
			}
			wxString GetArgFromPreferences(const ReconstructionUserPreferences_t& preferences)
			{
				EnglishTemporaryLocale dotNumericOnly;
				wxString argChain(ApplicationConfiguration::CONST_TETROOT_PATH+"vmr"+wxFileName::GetPathSeparator()+"vmr.exe ");
				argChain+=wxString::Format(" -depth%i",(int)(preferences.depth));

				if(preferences.getVolStats)
				{
					argChain+=" -volstats\""+outputFileDir+GetVolumeInfoFileName()+"\"";
				}else{
					if(preferences.meshSimplificationMethod==ReconstructionUserPreferences_t::MESH_SIMPLIFICATION_METHOD_EDGE_COLLAPSE)
					{
						argChain+=wxString::Format(" -q%f",preferences.minTriangleQuality);
						argChain+=wxString::Format(" -mq%f",preferences.mergeEpsilon);
					}
					if(preferences.adaptativeMeshMethod==ReconstructionUserPreferences_t::ADAPTATIVE_MESH_METHOD_NONE)
						argChain+=" -noadaptative";

					//Pas encore implémenté
					//if(preferences.meshSimplificationMethod==ReconstructionUserPreferences_t::MESH_SIMPLIFICATION_METHOD_NONE)
					//	argChain+=" -noreduction";

					for(ModelReconstructionUI::VolumeIdList_t::const_iterator itvolid=preferences.volumeIdToExport.begin();itvolid!=preferences.volumeIdToExport.end();itvolid++)
					{
						argChain+=wxString::Format(" -iv%i",*itvolid);
					}
				}

				argChain+=" -i\""+originalFileName+"\"";
				if(!preferences.getVolStats && !preferences.volumeIdToExport.empty())
				{
					argChain+=" -o\""+outputFileDir+outputFileName+"\"";
					modelOutputFileNames.clear();
					for(VolumeIdList_t::const_iterator itVolId=preferences.volumeIdToExport.begin();itVolId!=preferences.volumeIdToExport.end();itVolId++)
					{
						modelOutputFileNames.push_back(outputFileDir+wxString::Format("%i_",*itVolId)+outputFileName);
					}
				}
				return argChain;
			}
		public:
			ModelReconstructionManager()
			{
			}
			void SetModelSourceFilePath(const wxString& filePath)
			{
				wxFileName modificationFileNameObj(filePath);
				originalFileName=filePath;
				outputFileDir=modificationFileNameObj.GetPath()+wxFileName::GetPathSeparator();
				outputFileName="out_"+modificationFileNameObj.GetFullName();
			}
			virtual bool GetAvaibleVolumes(const ReconstructionUserPreferences_t& preferences,VolumeIdList_t& avaibleVolumesIds,VolumeValueList_t& avaibleVolumesValues)
			{
				ReconstructionUserPreferences_t reconsPref(preferences);
				reconsPref.getVolStats=true;
				wxString exeArgs=GetArgFromPreferences(reconsPref);
				wxProgressDialog progDialog(_("Execution of the model remesh."),_("Searching model volumes ..."),10000,NULL,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
				if(!uiRunExe(NULL,exeArgs,_("VolumetricMeshRepair: "), &progDialog))
					return false;
				//Lecture du fichier de statistiques
				wxString statsFilePath(outputFileDir+GetVolumeInfoFileName());
				if(!wxFileExists(statsFilePath))
				{
					wxLogError(_("Unable to load the stats file:\n%s"),statsFilePath);
					return false;
				}
				wxTextFile infile(statsFilePath);
				infile.Open();
				if(!infile.IsOpened())
					return false;
				wxString str;
				infile.GetFirstLine(); //La première ligne correspond uniquement aux libellés
				for ( str = infile.GetNextLine(); !infile.Eof(); str = infile.GetNextLine() )
				{
					wxArrayString lineCols;
					SplitString(str,";",lineCols);
					if(lineCols.size()>=2)
					{
						long volId;
						double volValue;

						if(lineCols[0].ToLong(&volId))
						{
							volValue=Convertor::ToFloat(lineCols[1]);
							avaibleVolumesIds.push_back((VolumeId_t)volId);
							avaibleVolumesValues.push_back((wxFloat32)volValue);
						}
					}
				}
				infile.Close();
				wxRemoveFile(statsFilePath);
				return true;
			}
			/**
			 * Retourne le chemin d'accès au modèle final
			 */
			wxString GetFinalModelFilePath()
			{
				if(!modelOutputFileNames.empty())
					return modelOutputFileNames.front();
				else
					return wxString();
			}

			virtual void FinalizeModelReconstruction()
			{
				MakeModel(finalPreferences);
			}
			virtual void SetParameters(const ReconstructionUserPreferences_t& preferences)
			{
				finalPreferences=preferences;
			}
			void MakeModel(const ReconstructionUserPreferences_t& preferences)
			{

				wxString exeArgs=GetArgFromPreferences(preferences);
				wxProgressDialog progDialog(_("Reconstruction of input model."),_("Searching model volumes ..."),10000,NULL,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
				uiRunExe(NULL,exeArgs,_("VolumetricMeshRepair: "), &progDialog);
			}
	};
};

void ProjectManager::OnStartRemeshWizard()
{
	using namespace ModelReconstructionUI;

	ModelReconstructionManager reconstructionManager;
	//Il faut sauvegarder le model au format PLY
	wxString tmpFolder=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+"temp"+wxFileName::GetPathSeparator();
	if(!wxDirExists(tmpFolder))
		wxMkDir(tmpFolder,0777);
	wxString plyFilePath(tmpFolder+"sourceRecons.ply");
	if(!this->sceneMesh.Save(plyFilePath))
	{
		wxLogError(_("Unable to save the file under the PLY format"));
		return;
	}
	reconstructionManager.SetModelSourceFilePath(plyFilePath);

	ModelReconstructionWizard reconstructionWiz(mainFrame,&reconstructionManager);


	if(reconstructionWiz.RunWizard(reconstructionWiz.GetFirstPage()))
	{
		wxString reconstructedSceneFilePath(reconstructionManager.GetFinalModelFilePath());
		this->sceneMesh.Load(reconstructedSceneFilePath);
		//this->sceneMesh.Save(meshName);
		wxRemoveFile(plyFilePath);
		wxRemoveFile(reconstructedSceneFilePath);
	}

}
