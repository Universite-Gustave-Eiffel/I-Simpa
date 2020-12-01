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


#ifndef __DIALOG_IMPORT_SCENE__
#define __DIALOG_IMPORT_SCENE__

#include <wx/dialog.h>

/*! \file customdlg.h
 *    @brief Boite de dialogue contenant un ou plusieurs champs de saisie
 */

/**
 * @brief Boite de dialogue importation de modèle 3D
 * 
 * Boite de dialogue où l'utilisateur choisi quel traitement a effectuer sur le modèle 3D à importer
 */
class sceneLoadOptionDialog : public wxDialog
{
public:
	/**
	 * Constructeur
	 * @param parent Fenêtre parente
	 * @param message Message de la fenêtre
	 * @param caption Titre de la fenêtre
	 * @param pos Position de la fenêtre
	 */
    sceneLoadOptionDialog(wxWindow *parent,
                        const wxString& message,
                        const wxString& caption,
                        const wxString& defaultMeshParams,
                        const wxPoint& pos = wxDefaultPosition);

	
	/** Evenement de validation
	*/
    void OnOK(wxCommandEvent& event);
	/** Evenement d'annulation
	*/
    void OnCancel(wxCommandEvent& event);
	void OnCheckMeshModel(wxCommandEvent& evt);
	void OnLauchRemeshWizard(wxCommandEvent& evt);
	void OnCheckRestoreGroups(wxCommandEvent& evt);

	wxString GetMeshParameters();
	bool IsMeshRepair();
	bool IsMeshSurface();
	bool IsKeepExistingFaceLinks();
	/**
	 * @return Distance maximale de lien avec les anciens groupes en m.
	 */
	float GetEpsilonLinkingFaceGroup();
	/**
	 * L'utilisateur veut il executer l'operation de remaillage du modèle
	 */
	bool IsRemeshModel();


private:
	wxTextCtrl* txt_ParamMesh;
	wxCheckBox* cb_TryToRepairMesh;
	wxCheckBox* cb_TryToMeshSurface;
	wxCheckBox* cb_KeepExistingFaceLinks;
	wxCheckBox* cb_RemeshModel;
	wxChoice* lb_ModelImportUnit;

	wxString epsilonValue;
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(sceneLoadOptionDialog)
    DECLARE_NO_COPY_CLASS(sceneLoadOptionDialog)
};

#endif // __NUMDLGH_G__
