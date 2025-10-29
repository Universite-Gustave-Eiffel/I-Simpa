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
#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
#include "last_cpp_include.hpp"
WX_DEFINE_OBJARRAY(ArrayOfXmlTree);


void ProjectManager::OnMenuUndo()
{
	//Si on peut annuler la modification
	if(CanUndo())
	{
		if(currentHistoryNavigation==0)
		{
			PushModificationToHistory(true);
			currentHistoryNavigation++;
		}
		currentHistoryNavigation++;
		projetConfig=configHistory[configHistory.size()-currentHistoryNavigation];
		LoadCurrentProject(false);
		Init();
	}
}
void ProjectManager::OnMenuRedo()
{
	if(CanRedo())
	{
		projetConfig=configHistory[configHistory.size()-currentHistoryNavigation];
		currentHistoryNavigation--;
		LoadCurrentProject(false);
		Init();
	}
}


bool ProjectManager::PushModificationToHistory(bool force)
{
	wxDateTime maintenant=wxDateTime::UNow();
	if(force || (maintenant-lastHistoryUpdate).GetMilliseconds()>UNDO_HISTORY_TIMESTEP)
	{
		lastHistoryUpdate=maintenant;
		if(this->rootUserConfig.get() != nullptr && this->rootUserConfig->GetElementByLibelle("Settings")->GetElementByLibelle("History")->GetBoolConfig("keep_modification_history"))
		{
			if(!force)
				wxLogDebug(wxGetTranslation("Add modifications in the change log"));
			else
				wxLogDebug(wxGetTranslation("Forced adding changes in the change log."));
			this->UpdateXmlFile("",false);
			configHistory.Add(new wxXmlDocument(projetConfig));
			//Dû à la modification on perd soit la plus vieille modification soit tout les dernieres modifications ( en fonction de l'etat de navigation dans l'historique)
			if(currentHistoryNavigation==0)
			{
				if(configHistory.size()>UNDO_HISTORY_SIZE)
					configHistory.RemoveAt(0);
			}else{
				//L'utilisateur a fait annuler et il a ensuite modifié le projet, l'utilisateur ne peut donc plus utiliser refaire
				//Supprime de la version courante jusqu'à la dernière version
				configHistory.RemoveAt(configHistory.size()-currentHistoryNavigation,currentHistoryNavigation-1);
				currentHistoryNavigation=0;
			}
			wxCommandEvent eventUpdate( wxEVT_PSPS_MAIN_EVENT, mainFrame->GetId() );
			eventUpdate.SetInt(ApplicationConfiguration::MAIN_EVENT_UNDO_REDO_UPDATE);
			wxPostEvent(mainFrame,eventUpdate);
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}
bool ProjectManager::CanUndo()
{
	return currentHistoryNavigation<configHistory.size();
}

bool ProjectManager::CanRedo()
{
	return currentHistoryNavigation>0;
}
void ProjectManager::ClearUndoRedoHistory()
{
	currentHistoryNavigation=0;
	configHistory.Clear();
}