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

#include "languageSelection.hpp"
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/intl.h>
#include <wx/log.h>
#include <wx/list.h>
#include <wx/dir.h>
#include <list>
#include "last_cpp_include.hpp"

BEGIN_EVENT_TABLE(LanguageSelector, wxDialog)
    EVT_BUTTON(wxID_OK, LanguageSelector::OnOK)
    EVT_BUTTON(wxID_CANCEL, LanguageSelector::OnCancel)
END_EVENT_TABLE()

enum WindowLanguageSelectorId {
    WindowLanguageSelectorId_LIST
};

class wxDirList : public wxDirTraverser {
public:
    wxDirList(std::vector<wxString> &_folderlst) : folderlst(_folderlst) {
    }

    virtual wxDirTraverseResult OnFile(const wxString &filename) {
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString &dirname) {
        folderlst.push_back(dirname);
        return wxDIR_IGNORE;
    }

private:
    std::vector<wxString> &folderlst;
};

LanguageSelector::LanguageSelector(wxWindow *parent,
                                   const wxString &message,
                                   const wxString &caption,
                                   const wxString &rootLngFolder,
                                   const wxPoint &pos)
    : wxDialog(parent, wxID_ANY, message,
               pos, wxDefaultSize) {
    wxBeginBusyCursor();
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *lstsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(lstsizer, 1, wxLEFT | wxTOP | wxGROW);

    wxListCtrl *languageList = new wxListCtrl(this, WindowLanguageSelectorId_LIST, wxDefaultPosition, wxDefaultSize,
                                          wxLC_REPORT | wxSUNKEN_BORDER | wxLC_SINGLE_SEL);
    languageList->InsertColumn(0, caption, wxLIST_FORMAT_LEFT, -1);

    // Set the column width to match the list control width
    languageList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);

    lstsizer->Add(languageList, 0, wxLEFT | wxTOP | wxGROW, 5);

    const wxString &resourceFolder = rootLngFolder;
    int defaultSelection = 0;
    int systemLanguage = wxLocale::GetSystemLanguage();
    std::vector<wxString> lngFolders;
    wxDirList traverser(lngFolders);
    wxDir folderRoot(rootLngFolder);
    if (folderRoot.Open(rootLngFolder)) {
        folderRoot.Traverse(traverser);
    }
    // Add key language
    lngFolders.push_back(resourceFolder + "en");
    // Iterate over language sub-folder
    for (auto &lngFolder: lngFolders) {
        // Extract folder name
        wxString langName = lngFolder.SubString(resourceFolder.size(), lngFolder.size() - 4);
        const wxLanguageInfo *lngInfo = wxLocale::FindLanguageInfo(langName);
        if (lngInfo != nullptr && wxLocale::IsAvailable(lngInfo->Language)) {
            // Is system default
            if (systemLanguage == lngInfo->Language)
                defaultSelection = languageList->GetItemCount();
            languageList->SetItemData(
                languageList->InsertItem(languageList->GetItemCount(), wxLocale::GetLanguageName(lngInfo->Language) + "(" + wxLocale::GetLanguageCanonicalName(lngInfo->Language) + ")"), lngInfo->Language);
        }
    }
    if (languageList->GetItemCount() > 0)
        languageList->SetItemState(defaultSelection,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
    // 3) buttons if any
    wxSizer *buttonSizer = CreateSeparatedButtonSizer(wxOK | wxCANCEL);
    if (buttonSizer) {
        topsizer->Add(buttonSizer, wxSizerFlags().Expand().DoubleBorder());
    }

    SetSizer(topsizer);
    SetAutoLayout(true);

    topsizer->SetSizeHints(this);
    topsizer->Fit(this);

    // After the window is shown and has a size
    int width = languageList->GetClientSize().GetWidth();
    languageList->SetColumnWidth(0, width);

    wxEndBusyCursor();
}

void LanguageSelector::OnOK(wxCommandEvent &event) {
    EndModal(wxID_OK);
}

void LanguageSelector::OnCancel(wxCommandEvent &event) {
    EndModal(wxID_CANCEL);
}

int LanguageSelector::GetSelectedLanguage() {
    //wxWindow* listwin=this->GetWindowChild(WindowLanguageSelectorId_FLAG_LIST);
    wxWindow *listwin = this->FindWindowById(WindowLanguageSelectorId_LIST);
    wxListCtrl *lstCtrl = wxDynamicCast(listwin, wxListCtrl);
    if (lstCtrl) {
        int selection = -1;
        selection = lstCtrl->GetNextItem(selection,
                                         wxLIST_NEXT_ALL,
                                         wxLIST_STATE_SELECTED);
        if (selection >= 0) {
#ifdef _DEBUG
            wxLocale selloc(lstCtrl->GetItemData(selection));
            wxLogDebug("Language sélectionné %s", selloc.GetCanonicalName());
#endif
            return lstCtrl->GetItemData(selection);
        }
    }
    return wxLANGUAGE_DEFAULT;
}
