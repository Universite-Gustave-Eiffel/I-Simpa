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


#ifndef __CUSTDIALOG_G__
#define __CUSTDIALOG_G__
#include <vector>
#include <wx/dialog.h>

/*! \file customdlg.h
 *    @brief Boite de dialogue contenant un ou plusieurs champs de saisie
 */

/**
 * @brief Boite de dialogue contenant un ou plusieurs champs de saisie
 *
 * Surcharge de wxDialog afin de créer une boite de dialogue contenant un ou plusieurs champs texte
 * Son utilisation est simple :
 *	-On déclare
 *	-On ajoute des champs via la méthode wxCustomEntryDialog::AddTextControl()
 *	-On affiche via la méthode wxCustomEntryDialog::Show()
 *	-On récupere les valeurs via wxCustomEntryDialog::GetValues()
 * 
 */
class wxCustomEntryDialog : public wxDialog
{
public:
	/**
	 * Constructeur
	 * @param parent Fenêtre parente
	 * @param message Message de la fenêtre
	 * @param caption Titre de la fenêtre
	 * @param pos Position de la fenêtre
	 */
    wxCustomEntryDialog(wxWindow *parent,
                        const wxString& message,
                        const wxString& caption,
                        const wxPoint& pos = wxDefaultPosition);

	/**
	 * Ajouter un champ dans la fenêtre
	 * @param label Libellé du champ
	 * @param defaultvalue Valeur d'origine du champ
	 */
	void AddTextControl(     const wxString& label,
                        const wxString& defaultvalue
						);
	
	/**
	 * Ajouter un champ de liste dans la fenêtre
	 * @param label Libellé du champ
	 * @param defaultvalue Valeur d'origine du champ
	 * @param Values Valeur sélectionnable
	 */
	void AddListBox( const wxString& label,
		const wxString& defaultValue,
		const wxArrayString& Values);
	/**
	 * Charge les valeurs dans le tableau passé en paramètre
	 * @param[out] values Tableau de chaîne de caractères.Ce tableau est alimenté dans l'ordre d'appel de AddTextControl()
	 */
	void GetValues(std::vector<wxString> &values);
    // implementation only
	/** Evenement de validation
	*/
    void OnOK(wxCommandEvent& event);
	/** Evenement d'annulation
	*/
    void OnCancel(wxCommandEvent& event);

private:

	enum CUSTOM_DIALOG_CTRLS
	{
		CUSTOM_DIALOG_CTRLS_GRID
	};

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxCustomEntryDialog)
    DECLARE_NO_COPY_CLASS(wxCustomEntryDialog)
};

#endif // __NUMDLGH_G__
