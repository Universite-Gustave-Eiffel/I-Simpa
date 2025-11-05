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

/**
 * @file WizardRemeshModel.h
 * @brief Outil de reconstruction du modèle 3D
 * 
 * Cet outil est capable de reconstruire un modèle pour le rendre compatible avec le maillage tétraèdrique.
 */

#include <wx/wizard.h>
#include <list>

#ifndef __MODEL_RECONSTRUCTION_WIZARD__
#define __MODEL_RECONSTRUCTION_WIZARD__

namespace ModelReconstructionUI
{
	typedef unsigned short VolumeId_t;
	typedef std::list<VolumeId_t> VolumeIdList_t;
	typedef std::list<wxFloat32> VolumeValueList_t;
	struct ReconstructionUserPreferences_t
	{
		ReconstructionUserPreferences_t()
			:depth(7),
			triangulationMethod(TRIANGULATION_METHOD_MARCHING_CUBE),
			adaptativeMeshMethod(ADAPTATIVE_MESH_METHOD_VERTICE_TRANSLATION),
			meshSimplificationMethod(MESH_SIMPLIFICATION_METHOD_EDGE_COLLAPSE),
			minTriangleQuality(.3f),
			mergeEpsilon(.998f),
			getVolStats(false)
		{}
		unsigned short depth; //Profondeur de discrétisation spatiale en (2^depth)^3 cellules
		enum TRIANGULATION_METHOD
		{
			TRIANGULATION_METHOD_MARCHING_CUBE,
			TRIANGULATION_METHOD_NONE,					//Pas encore implémentée
			TRIANGULATION_METHOD_POISSON_RECONSTRUCTION //Pas encore implémentée
		} triangulationMethod;
		enum ADAPTATIVE_MESH_METHOD
		{
			ADAPTATIVE_MESH_METHOD_NONE,
			ADAPTATIVE_MESH_METHOD_VERTICE_TRANSLATION,
			ADAPTATIVE_MESH_METHOD_TRIANGLE_TRANSLATION //Pas encore implémentée
		} adaptativeMeshMethod;
		enum MESH_SIMPLIFICATION_METHOD
		{
			MESH_SIMPLIFICATION_METHOD_EDGE_COLLAPSE,
			MESH_SIMPLIFICATION_METHOD_NONE //Pas encore implémentée
		} meshSimplificationMethod;
		wxFloat32 minTriangleQuality;	//Lié seulement au paramètre MESH_SIMPLIFICATION_METHOD_EDGE_COLLAPSE
		wxFloat32 mergeEpsilon;			//Lié seulement au paramètre MESH_SIMPLIFICATION_METHOD_EDGE_COLLAPSE
		VolumeIdList_t volumeIdToExport;//Indice des volumes à exporter
		bool getVolStats;				//On désire seulement avec des informations sur le modèle
	};
	/**
	 * Passerelle entre l'interface de configuration de reconstruction du modèle et l'executable de reconstruction. Ce classe doit être surchargé afin d'effectuer les opérations
	 */
	class ModelReconstructionManagerBase
	{
	public:

		/**
		 * Permet d'obtenir les informations sur les volumes pouvant être extrait de la scène
		 * @param preferences Préférences de reconstruction
		 * @param[out] avaibleVolumesIds Indices des volumes
		 * @param[out] avaibleVolumesValues Valeur des volumes (m^3)
		 * @return True en cas de succès de la recherche
		 */
		virtual bool GetAvaibleVolumes(const ReconstructionUserPreferences_t& preferences,VolumeIdList_t& avaibleVolumesIds,VolumeValueList_t& avaibleVolumesValues)=0;
		virtual void FinalizeModelReconstruction()=0;
		virtual void SetParameters(const ReconstructionUserPreferences_t& preferences)=0;
		virtual ~ModelReconstructionManagerBase(){};
	};

	class ModelReconstructionWizard : public wxWizard
	{
	public:
		ModelReconstructionWizard(wxFrame *frame,ModelReconstructionManagerBase* _modelReconstructionManager);
		void RefreshNextButtonState();
		//wxWizardPage *GetFirstPage() const { return m_page1; }
		wxWizardPage *GetFirstPage() { return m_page1; }
		ModelReconstructionManagerBase* GetModelReconstructionManager() { return modelReconstructionManager; }

		void OnFinish(wxWizardEvent& wizEvent);
	protected:
		wxWizardPageSimple *m_page1;
		ModelReconstructionManagerBase* modelReconstructionManager;
		DECLARE_EVENT_TABLE()
	};



	/**
	 * @brief 1ere page sélection de la qualitée
	 */
	
	class PageOneQualityParameters : public wxWizardPageSimple
	{
	public:
		PageOneQualityParameters(ModelReconstructionWizard *parent);
	protected:
		wxControl* AddCtrl(wxControl* newCtrl,const wxString& label,wxSizer* mainSizer);
		void OnPageChanging(wxWizardEvent& wizEvent);
		void OnPageChanged(wxWizardEvent& wizEvent);
		enum WIZARD_CTRL
		{
			WIZARD_CTRL_SLIDER_DEPTH,
			WIZARD_CTRL_SLIDER_TRIANGLE_QUALITY,
			WIZARD_CTRL_SLIDER_EPSILON_MERGE,
			WIZARD_CTRL_COMBO_ADAPTATIVE_REMESH_MODE,
			WIZARD_CTRL_COMBO_TRIANGULATION_MODE,
			WIZARD_CTRL_COMBO_SIMPLIFICATION_MODE
		};
		DECLARE_EVENT_TABLE()
	};
	

	/**
	 * @brief 2ème page sélection des volumes à extraire
	 */
	
	class PageTwoVolumeSelection : public wxWizardPageSimple
	{
	public:
		PageTwoVolumeSelection(ModelReconstructionWizard *parent);
		void ComputeVolumeListFromParameter(const ReconstructionUserPreferences_t& parameters);
		virtual wxWizardPage *GetNext() const;
		void OnVolumeSelectionChange(wxCommandEvent& wizEvent);
		void OnPageChanging(wxWizardEvent& wizEvent);
		void OnPageChanged(wxWizardEvent& wizEvent);
	protected:
		ModelReconstructionManagerBase* modelReconstructionManager;
		enum WIZARD_CTRL
		{
			WIZARD_CTRL_VOLUMES_LIST
		};
		ReconstructionUserPreferences_t localParameters;
		VolumeIdList_t volumesIds;
		VolumeValueList_t volumesValues;
		DECLARE_EVENT_TABLE()
	};


};
#endif