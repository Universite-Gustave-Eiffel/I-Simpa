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

#include "e_report_gabe_recps.h"
#include "IHM/simpleGraphManager.h"
#include <wx/aui/auibook.h>
#include <wx/wupdlock.h>
#include "data_manager/appconfig.h"
#include "last_cpp_include.hpp"

E_Report_Gabe_Recps::E_Report_Gabe_Recps(Element* parent,wxString Nom,wxString Path)
	:E_Report_Gabe(parent,Nom,Path, ELEMENT_TYPE_REPORT_GABE_RECPS)
{
	wxGetTranslation("Sound level"); //Libellé du fichier standard
}

E_Report_Gabe_Recps::E_Report_Gabe_Recps(Element* parent,wxXmlNode* nodeElement)
	:E_Report_Gabe(parent,nodeElement, ELEMENT_TYPE_REPORT_GABE_RECPS)
{

}

void E_Report_Gabe_Recps::OnRightClic(wxMenu* leMenu)
{
	leMenu->Append(GetMenuItem(leMenu,IDEVENT_RECP_COMPUTE_ACOUSTIC_PARAMETERS,wxGetTranslation("Calculate acoustic parameters")));
	E_Report_Gabe::OnRightClic(leMenu);
}

bool E_Report_Gabe_Recps::GetArrayData(wxWindow* auiBookWin,wxString& arrayTitle,std::vector<wxString>& lblCols,std::vector<wxString>& lblRows,std::vector<wxString>& cells,std::vector<float>& cellsValue)
{
	bool ok=E_Report_Gabe::GetArrayData(NULL,arrayTitle,lblCols,lblRows,cells,cellsValue);
	const float p_0=1/pow((float)(20*pow(10.f,(int)-6)),(int)2);
	/////////////////////////////////////////////////////
	//Post traitement
	// J/m3 -> dB
	arrayTitle+=wxGetTranslation(" (dB)");
	lblCols.push_back(wxGetTranslation("Global"));
	lblRows.push_back(wxGetTranslation("Total"));
	int nbrow=lblRows.size();
	int nbcol=lblCols.size();
	int nbrow_src=nbrow-1;
	int nbcol_src=nbcol-1;

	std::vector<float> tmpcellsValue=std::vector<float>(nbrow*nbcol);
	cells=std::vector<wxString>(nbrow*nbcol);

	for(int idrow=0;idrow<nbrow_src;idrow++)
	{
		for(int idcol=0;idcol<nbcol_src;idcol++)
		{
			tmpcellsValue[(idcol*nbrow)+idrow]=cellsValue[(idcol*nbrow_src)+idrow];
			//cells[(idcol*nbrow)+idrow]="";
		}
	}
	cellsValue=tmpcellsValue;


	//////////////////////////////////////
	// Row sum

	for(int idrow=0;idrow<nbrow-1;idrow++)
	{
		float totPression=0.f;
		for(int idcol=0;idcol<nbcol-1;idcol++)
		{
			float* wVal=&cellsValue[((idcol)*nbrow)+idrow];
			if(*wVal!=0)
			{
				totPression+=(*wVal);
			}
		}
		if(totPression>0)
		{
			cellsValue[((nbcol-1)*nbrow)+idrow]=totPression;
		}else{
			cellsValue[((nbcol-1)*nbrow)+idrow]=0;
		}
	}

	//////////////////////////////////////
	//Cumul sur les colonnes

	for(int idcol=0;idcol<nbcol;idcol++)
	{
		float totPression=0.f;

		for(int idrow=0;idrow<nbrow-1;idrow++)
		{
			float* wVal=&cellsValue[((idcol)*nbrow)+idrow];
			if(*wVal!=0)
			{
				totPression+=(*wVal);
			}
		}
		if(totPression>0)
		{
			cellsValue[((idcol)*nbrow)+nbrow-1]=totPression;
		}else{
			cellsValue[((idcol)*nbrow)+nbrow-1]=0;
		}
	}

	//////////////////////////////////////
	// Conversion en db
	for(int idrow=0;idrow<nbrow;idrow++)
	{
		for(int idcol=0;idcol<nbcol;idcol++)
		{
			float* wVal=&cellsValue[((idcol)*nbrow)+idrow];
			float dbVal=10*log10f((*wVal)*p_0);
			(*wVal)=dbVal;
			cells[(idcol*nbrow)+idrow]=wxString::Format("%.1f",dbVal); //Précision de 1 chiffre aprés la virgule
		}
	}

	//Intervertir les lignes et les colonnes
	std::vector<wxString> tmpNewCols=lblRows;
	std::vector<wxString> tmpNewRows=lblCols;
	std::vector<wxString> tmpCells(cells.size());
	std::vector<float> tmpCellsVal(cellsValue.size());
	for(int idrow=0;idrow<nbrow;idrow++)
	{
		for(int idcol=0;idcol<nbcol;idcol++)
		{
			tmpCellsVal[((idrow)*nbcol)+idcol]=cellsValue[((idcol)*nbrow)+idrow];
			tmpCells[((idrow)*nbcol)+idcol]=cells[((idcol)*nbrow)+idrow];
		}
	}
	lblRows=tmpNewRows;
	lblCols=tmpNewCols;
	cellsValue=tmpCellsVal;
	cells=tmpCells;

	/////////////////////////////////////////////////////////////////////////
	// Création du graphique Niveau sonore en fonction des bandes de fréquences
	wxAuiNotebook* noteBookWin=dynamic_cast<wxAuiNotebook*>(auiBookWin);
	if(noteBookWin)
	{
		wxWindowUpdateLocker lockWin(noteBookWin);
		using namespace sgSpace;

		MainSimpleGraphWindow* graphPage;
		//Ajout des données au graphique
		int nbrowel=lblRows.size();
		int nbcolel=lblCols.size();

		graphPage=new MainSimpleGraphWindow(noteBookWin,-1);
		wxString gabeFolder;
		this->BuildFullPath(gabeFolder);
		graphPage->SetDefaultSaveGraphSavePath(gabeFolder);

		noteBookWin->AddPage(graphPage,wxGetTranslation("Spectrum"));
		SG_Element_List* drawingEl;
		//Add spectrum for each source
		for (int idcol = 0; idcol < nbcolel - 1; idcol++)
		{
			drawingEl=new SG_Element_List(graphPage,lblCols[idcol]);
			drawingEl->el_style.SetDrawingMethod(DRAWING_METHOD_DOT_CLOUD);
			drawingEl->el_style.Randomize(); // Randomize brush
			drawingEl->el_style.RandomizeMarker(); // Randomize marker (cross, circle etc..)
			for(int idrow=0;idrow<nbrowel-1;idrow++)
			{
				drawingEl->PushValue(cellsValue[(idcol*nbrowel)+idrow]);
			}
		}


		//////////////////
		// Add axes
		SG_Element_Axis* axeEly=new SG_Element_Axis(graphPage,wxVERTICAL,0.f,"dB ");
		axeEly->el_style.SetDrawingMethod(DRAWING_METHOD_COLS); //Ainsi le style de l'axe dépend de la méthode de dessin général
		SG_Element_Axis_Labeled* axeElx=new SG_Element_Axis_Labeled(graphPage,wxHORIZONTAL,1,"");
		axeElx->el_style.SetTextRotation(90);
		axeElx->el_style.SetDrawingMethod(DRAWING_METHOD_COLS); //Ainsi le style de l'axe dépend de la méthode de dessin général
		for(int idrow=0;idrow<lblRows.size()-1;idrow++)
		{
			axeElx->PushLabel(lblRows[idrow]);
		}

		//Add legend
		SG_Legend::Add(new SG_Legend(graphPage,LEGEND_PLACEMENT_HORIZONTAL,-1), wxSizerFlags(0).Align(wxALIGN_TOP));

		//////////////
		graphPage->ZoomFit();
		graphPage->LoadConfig(ApplicationConfiguration::GetFileConfig(),ApplicationConfiguration::CONST_GRAPH_CONFIG_PATH,true);

	}

	return ok;
}

bool E_Report_Gabe_Recps::OpenFileInGrid()
{
	return false;
}
