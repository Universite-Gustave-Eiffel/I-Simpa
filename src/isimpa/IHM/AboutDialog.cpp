/* ----------------------------------------------------------------------
* I-SIMPA (http://i-simpa.ifsttar.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) 200 7-2014 - IFSTTAR - Judicael Picaut, Nicolas Fortin
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

#include "AboutDialog.hpp"
#include <wx/bookctrl.h>
#include <wx/html/htmlwin.h>
#include <data_manager/appconfig.h>

wxBEGIN_EVENT_TABLE(AboutDialog, wxPropertySheetDialog)
    EVT_HTML_LINK_CLICKED(wxID_ANY, AboutDialog::OnHtmlLinkClicked)
wxEND_EVENT_TABLE()

bool AboutDialog::CreateAboutDialog(wxWindow *parent) {
    if (!wxPropertySheetDialog::Create(parent, wxID_ANY, wxGetTranslation("About I-SIMPA"), wxDefaultPosition))
        return false;
    CreateButtons(wxOK);

    GetBookCtrl()->SetMinSize(FromDIP(wxSize(400, 230)));

    // Add main about page
    wxHtmlWindow* mainAboutHtml = new wxHtmlWindow(GetBookCtrl());
    mainAboutHtml->SetPage(aboutHtml);
    GetBookCtrl()->AddPage(mainAboutHtml, wxGetTranslation("About"));

    // Add credits
    wxHtmlWindow* creditsAboutHtml = new wxHtmlWindow(GetBookCtrl());
    creditsAboutHtml->SetPage(creditsHtml);
    GetBookCtrl()->AddPage(creditsAboutHtml, wxGetTranslation("Credits"));

    // Add license
    wxHtmlWindow* licenseAboutHtml = new wxHtmlWindow(GetBookCtrl());
    licenseAboutHtml->LoadFile(wxFileName(ApplicationConfiguration::getResourcesFolder(), "license.html"));
    GetBookCtrl()->AddPage(licenseAboutHtml, wxGetTranslation("License"));


    wxSizer* innerSizer = GetInnerSizer();

    // Add header
    if(innerSizer) {
        wxStaticBitmap* dialogBanner = new wxStaticBitmap(this, wxID_ANY, wxBitmap("./Bitmaps/about_banner.png", wxBITMAP_TYPE_PNG));
        dialogBanner->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_AspectFit);
        innerSizer->Insert(0, dialogBanner);
    }

    LayoutDialog();

    return true;
}

AboutDialog::AboutDialog()
        : aboutHtml(wxString::Format(
        "<html><head> <meta charset=\"UTF-8\"></head><body><h2>I-SIMPA %i.%i.%i</h2><p><small>" + wxGetTranslation("Built on %s") + "</small></p><p>" +
        wxGetTranslation("An Open Source software for 3D sound propagation modelling.") + "</p>"
                "<p>" + wxGetTranslation("Classical Theory [%i.%i.%i]") + "</p><p>" + wxGetTranslation("SPPS [%i.%i.%i]") +
        "</p><p>Read more on <a href=\"http://i-simpa.ifsttar.fr\">http://i-simpa.ifsttar.fr</a></p></body></html>",
        ApplicationConfiguration::SPPS_UI_VERSION_MAJOR, ApplicationConfiguration::SPPS_UI_VERSION_MINOR,
        ApplicationConfiguration::SPPS_UI_VERSION_REVISION, ApplicationConfiguration::BUILD_DATE,
        ApplicationConfiguration::CTR_VERSION_MAJOR,
        ApplicationConfiguration::CTR_VERSION_MINOR, ApplicationConfiguration::CTR_VERSION_REVISION,
        ApplicationConfiguration::SPPS_VERSION_MAJOR, ApplicationConfiguration::SPPS_VERSION_MINOR,
        ApplicationConfiguration::SPPS_VERSION_REVISION)),
          creditsHtml("<html><head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n"
                              "<body>\n"
                              "<table width=\"100%\" border=0>\n"
                              "<tr><td valign=top colspan=2 bgcolor=\"LIGHT BLUE\"><strong>" + wxGetTranslation("Developers") +
                      "</strong></td></tr>\n"
                              "<tr><td valign=top>Judicaël Picaut</td><td valign=top>Université Gustave Eiffel (formely Ifsttar, LCPC)</td></tr>\n"
                              "<tr><td valign=top>Nicolas Fortin</td><td valign=top>Université Gustave Eiffel (formely Ifsttar, LCPC)</td></tr>\n"
                              "<tr><td valign=top> &nbsp </td></tr>\n"
                              "<tr><td valign=top colspan=2 bgcolor=\"LIGHT BLUE\"><strong>" + wxGetTranslation("Contributors") +
                      "</strong></td></tr>\n"
                              "<tr><td valign=top>Augustin Lacour</td><td valign=top>Ifsttar</td></tr>\n"
                              "<tr><td valign=top>Jimmy Farcy</td><td valign=top>Ifsttar</td></tr>\n"
                              "<tr><td valign=top>Wojciech Binek</td><td valign=top>AGH</td></tr>\n"
                              "<tr><td valign=top> &nbsp </td></tr>\n"
                              "<tr><td valign=top colspan=2 bgcolor=\"LIGHT BLUE\"><strong>" + wxGetTranslation("Translators") +
                      "</strong></td></tr>\n"
                              "<tr><td valign=top>Wojciech Binek</td><td valign=top>AGH</td></tr>\n"
                              "</table>\n"
                              "</body>\n"
                              "</html>") {


}

void AboutDialog::OnHtmlLinkClicked(wxHtmlLinkEvent &event) {
    wxLaunchDefaultBrowser(event.GetLinkInfo().GetHref());
}
