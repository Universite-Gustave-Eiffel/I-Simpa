///////////////////////////////////////////////////////////////////////////////
// Name:        uimenuitem.h
// Purpose:     wxUiMenuItem class
// Author:      Nicolas Fortin
// Modified by:
// Created:     18.03.2010
// Copyright:   LCPC 2010
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef   _UI_MENUITEM_H
#define   _UI_MENUITEM_H

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include  <wx/menuitem.h>  // base class

// ----------------------------------------------------------------------------
// wxUiMenuItem: an item in the menu
// ----------------------------------------------------------------------------
//wxString keys and values
WX_DECLARE_STRING_HASH_MAP( wxString, StringHashMap );

class wxUiMenuItem : public wxMenuItem
{
public:
    // ctor & dtor
    wxUiMenuItem(wxMenu *parentMenu = NULL,
               int id = wxID_SEPARATOR,
               const wxString& name = wxEmptyString,
               const wxString& help = wxEmptyString,
               wxItemKind kind = wxITEM_NORMAL,
               wxMenu *subMenu = NULL);
	virtual ~wxUiMenuItem(){};

	wxString GetBitmapPath();
	void SetBitmapPath(const wxString& _bitmapPath);
	/** Retourne une chaine qui sera disponible lors de l'évenement de l'appel du menu
	 *  @return Valeur de la données dataName ou chaine vide si la donnée n'existe pas
	 */
	wxString GetUserData(const wxString& dataName);
	void SetUserData(const wxString& dataName,const wxString& dataValue);

private:
	wxString bitmapPath;
};

#endif  //_UI_MENUITEM_H
