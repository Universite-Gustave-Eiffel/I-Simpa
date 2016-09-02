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

enum WindowLanguageSelectorId
{
	WindowLanguageSelectorId_FLAG_LIST
};

class wxDirList : public wxDirTraverser
{
public:
    wxDirList(wxSortedArrayString& _folderlst) : folderlst(_folderlst) { }

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
		folderlst.push_back(dirname);
        return wxDIR_IGNORE;
    }

private:
	wxSortedArrayString& folderlst;
};

LanguageSelector::LanguageSelector(wxWindow *parent,
                        const wxString& message,
                        const wxString& caption,
						const wxString& rootLngFolder,
						const wxString& flagsFolder,
                        const wxPoint& pos )
	:wxDialog(parent, wxID_ANY, caption,
                              pos, wxDefaultSize)
{
	wxBeginBusyCursor();
	wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* lstsizer = new wxBoxSizer( wxVERTICAL );
	topsizer->Add(lstsizer,1,wxLEFT | wxTOP | wxGROW);

	wxListCtrl* flagList=new wxListCtrl(this,WindowLanguageSelectorId_FLAG_LIST,wxDefaultPosition,wxDefaultSize,wxLC_SMALL_ICON |
                                    wxSUNKEN_BORDER | wxLC_SINGLE_SEL );
	lstsizer->Add(flagList,0,wxLEFT | wxTOP | wxGROW,5);

	wxImageList* icoLst=new wxImageList(16,11);
	wxString ressourceFolder=rootLngFolder;
	wxString Canonical_lng,ISO3166_lng,ISO639_lng;
	int defaultSelection=0;
	int systemLanguage=wxLocale::GetSystemLanguage();
	wxSortedArrayString flagsFileName;
	wxSortedArrayString lngFolders;

	wxDirList traverser(lngFolders);
	wxDir folderRoot(rootLngFolder);
	if (folderRoot.Open(rootLngFolder)) {
		folderRoot.Traverse(traverser);
	}
	wxDir::GetAllFiles(flagsFolder,&flagsFileName,"*.png",wxDIR_FILES);


	for(int idlang=wxLANGUAGE_UNKNOWN+1;idlang<wxLANGUAGE_USER_DEFINED;idlang++)
	{
		if(wxLocale::IsAvailable(idlang))
		{
			wxLanguageInfo langInfo = *wxLocale::GetLanguageInfo(idlang);
			Canonical_lng = langInfo.CanonicalName;
			ISO3166_lng=Canonical_lng.Mid(Canonical_lng.rfind("_")+1).Lower();
			ISO639_lng=Canonical_lng.Left(Canonical_lng.rfind("_")).Lower();
 			if(lngFolders.Index(ressourceFolder+Canonical_lng)>=0)
			{
                if(systemLanguage==idlang)
                    defaultSelection=flagList->GetItemCount();
				wxString flag_filepath=flagsFolder+wxString::Format("%s.png",ISO3166_lng);
				if(flagsFileName.Index(flag_filepath)>=0)
				{
					wxIcon flagImage(flag_filepath, wxBITMAP_TYPE_PNG);
					if(flagImage.IsOk())
					{
						int indexico=icoLst->Add(flagImage);
						flagList->SetItemData(flagList->InsertItem(flagList->GetItemCount(),wxLocale::GetLanguageName(idlang),indexico),idlang);
					}
				}else{
					flagList->SetItemData(flagList->InsertItem(flagList->GetItemCount(),wxLocale::GetLanguageName(idlang)),idlang);
				}
			}
		}
	}
	flagList->AssignImageList(icoLst,wxIMAGE_LIST_SMALL);
	if(flagList->GetItemCount()>0)
        flagList->SetItemState(defaultSelection,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
    // 3) buttons if any
    wxSizer *buttonSizer = CreateSeparatedButtonSizer(wxOK | wxCANCEL);
    if ( buttonSizer )
    {
        topsizer->Add(buttonSizer, wxSizerFlags().Expand().DoubleBorder());
    }

    SetSizer( topsizer );
    SetAutoLayout( true );

    topsizer->SetSizeHints( this );
    topsizer->Fit( this );

    wxEndBusyCursor();
}

void LanguageSelector::OnOK(wxCommandEvent& event)
{


    EndModal(wxID_OK);
}

void LanguageSelector::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

int LanguageSelector::GetSelectedLanguage()
{
	//wxWindow* listwin=this->GetWindowChild(WindowLanguageSelectorId_FLAG_LIST);
	wxWindow* listwin=this->FindWindowById(WindowLanguageSelectorId_FLAG_LIST);
	wxListCtrl* lstCtrl=wxDynamicCast(listwin,wxListCtrl);
	if(lstCtrl)
	{
		int selection=-1;
		selection=lstCtrl->GetNextItem(selection,
                                     wxLIST_NEXT_ALL,
                                     wxLIST_STATE_SELECTED);
		if(selection>=0)
		{
			#ifdef _DEBUG
			wxLocale selloc(lstCtrl->GetItemData(selection));
			wxLogDebug("Language sélectionné %s",selloc.GetCanonicalName());
			#endif
			return lstCtrl->GetItemData(selection);
		}
	}
	return wxLANGUAGE_DEFAULT;
}
