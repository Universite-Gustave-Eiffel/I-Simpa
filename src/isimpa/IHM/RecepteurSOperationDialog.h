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

/**
 * @file RecepteurSOperationDialog.h
 * @brief Séléction de fichier pour les opération de post-traitement sur les récepteur de surfaces
 *
 * Cette fenête permet la séléction d'un récepteur de surface de référence (SPL, TR ou EDT) et la séléction d'une ou plusieurs configurations altérnatives
 * Ce dialogue retourne alors une liste de fichiers de comparaison ainsi qu'un nom de sortie pour chaque fichiers de résulats à créer.
 * Le récépteur de surface de référence agit comme une filtre pour la séléction des récepteurs de surface à comparer. Il n'est pas possible de comparer l'EDT avec le SPL par exemple.
 */

#include <wx/wizard.h>
#include "data_manager/tree_rapport/e_report_file.h"

#ifndef __RECEPTEURS_SELECTION_WIZARD__
#define __RECEPTEURS_SELECTION_WIZARD__

class PageReferenceSelection;
class PageOtherRecepeurSurfSelection;

class RecepteurSelectionWizard : public wxWizard
{
public:
	struct t_UserSelection
	{
		wxString referenceFilePath;
		wxString resultFolderPath;
		std::vector<wxString> comparaisonFilesPath;
		std::vector<wxString> comparaisonFilesOutputFileNames;
	};
    RecepteurSelectionWizard(wxFrame *frame, E_Report_File* _rootFolder);
	void RefreshNextButtonState();
    wxWizardPage *GetFirstPage() const { return m_page1; }

	t_UserSelection GetUserPreferences();
	E_Report_File* GetRootFolder() { return rootFolder; }
protected:
	friend class PageReferenceSelection;
	friend class PageOtherRecepeurSurfSelection;
	t_UserSelection userSelection;
private:
	E_Report_File* rootFolder;
    wxWizardPageSimple *m_page1;
};




/**
 * @brief Zone de séléction des récépteurs de surfaces
 */

class PanelReferenceSelection : public wxPanel
{
public:
    PanelReferenceSelection(wxWindow* parent,RecepteurSelectionWizard *wizManager, wxWindowID id = wxID_ANY);

	void OnRecepteurSurfSelected(wxCommandEvent& event);
	/**
	  * Retourne l'élément séléctionné par l'utilisateur ou NULL si aucun ou non valide
	  */
	E_Report_File* GetSelectedReportFile();
protected:
	enum WIZARDCTRL
	{
		WIZARDCTRL_REFERENCE_SELECTION=wxID_HIGHEST
	};
    DECLARE_EVENT_TABLE()
};

class PageReferenceSelection : public wxWizardPageSimple
{
public:
    PageReferenceSelection(RecepteurSelectionWizard *parent);
protected:
	enum WIZARDRSCTRL
	{
		WIZARDRSCTRL_TEXTBOX_RESULT_FOLDER=wxID_HIGHEST+1
	};
	void OnPageChanging(wxWizardEvent& wizEvent);
	RecepteurSelectionWizard *wiz_parent;
	PanelReferenceSelection* panelCtrl;
    DECLARE_EVENT_TABLE()
};


/**
 * @brief 2eme page du guide de séléction des récépteur de surface à comparer
 */

class PageOtherRecepeurSurfSelection : public wxWizardPageSimple
{
public:
    PageOtherRecepeurSurfSelection(RecepteurSelectionWizard *parent);
    virtual wxWizardPage *GetNext() const;
protected:
	PageOtherRecepeurSurfSelection* nextRSchooser;
	void OnPageChanging(wxWizardEvent& wizEvent);
	void OnPageChanged(wxWizardEvent& wizEvent);
	void OnCheck(wxCommandEvent& ctrlEvent);
	enum WIZARDRSCTRL
	{
		WIZARDRSCTRL_CHECKBOX_ANOTHER_RS_ENTRY=wxID_HIGHEST+1, //id Controle du choix de la saisie d'un autre fichier de recepteur de surface
		WIZARDRSCTRL_TEXTBOX_RESULT_FILENAME
	};
	wxInt32 id_rsarray_entry; //Numero d'enregistrement du fichier dans le tableau du wizard
	RecepteurSelectionWizard *wiz_parent;
	PanelReferenceSelection* panelCtrl;
    DECLARE_EVENT_TABLE()
};

#endif
