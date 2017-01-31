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

#include "e_report_file.h"

#include "e_report_folder.h"
#include "e_report_partvisualisation.h"
#include "e_report_recepteurssvisualisation.h"
#include "e_report_gabe.h"
#include "e_report_gabe_recp.h"
#include "e_report_gabe_recps.h"
#include "e_report_gabe_gap.h"
#include "e_report_unknown_file.h"
#include "e_report_rpi.h"
#include "data_manager/appconfig.h"
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include "last_cpp_include.hpp"
bool isFileNameValid(const wxString& fileName)
{
    if ( fileName.empty() )
    {
		wxLogError(_("Folder name or file name can't be empty"));
		return false;
    }

    const size_t len = fileName.length();
    for ( size_t n = 0; n < len; n++ )
    {
		const wxChar currChar(fileName[n]);
		if ( currChar == 92 || currChar == 47 || currChar == 58 || currChar == 42 || currChar == 63 || currChar == 34 || currChar == 60 || currChar == 62 || currChar == 124 )
        {
			wxLogError(_("Folder name can't use the following letters: \\/:*?\"<>|"));
            return false;
        }
    }
	return true;
}


void GetAllFolderItems(wxString rootFolder,wxArrayString& folderDir)
{
	if(wxDirExists(rootFolder))
	{
		wxDir dir(rootFolder);
		wxString filename;
		bool cont = dir.GetFirst(&filename);
		while ( cont )
		{
			folderDir.Add(rootFolder+filename);
			if(wxDirExists(rootFolder+filename))
				GetAllFolderItems(rootFolder+filename+wxFileName::GetPathSeparator(),folderDir);
			cont = dir.GetNext(&filename);
		}
	}
}

bool FuncDeleteFolder( wxString folderToDelete )
{
    wxFileName pathToDelete(folderToDelete);
	if(!pathToDelete.IsDir()) //Si le chemin correspond à un fichier existant
	{
		if(!wxRemoveFile(folderToDelete)) {
            wxLogError(_("Cannot remove file/directory"));
            return false;
        } else {
		    return true;
        }
	} else {
        //Delete folder
        if(!pathToDelete.Rmdir(wxPATH_RMDIR_RECURSIVE)) {
            wxLogError(_("Cannot remove file/directory"));
            return false;            
        } else {
            return true;
        }
    }
}



void E_Report_File::OnEndLabelEdit(wxTreeEvent& treeEvent)
{
	//L'utilisateur désire renommer le fichier
	wxString newLabel=treeEvent.GetLabel();
	if(!newLabel.IsEmpty() && isFileNameValid(newLabel))
	{
		wxString thisFileName;
		this->BuildFullPath(thisFileName);
		wxFileName newFileName(thisFileName);
		if((newFileName.IsDir() && !wxDirExists(thisFileName)) || (!newFileName.IsDir() && !wxFileExists(thisFileName)))
		{
			E_Report_File* pereFile=dynamic_cast<E_Report_File*>(this->pere);
			if(pereFile)
			{
				pereFile->RefreshFolderContents();
				pereFile->FillWxTree();
			}
			treeEvent.Veto();
			return;
		}
		wxString newFullName;
		if(newFileName.IsDir())
		{
			newFileName.RemoveLastDir();
			newFileName.AppendDir(newLabel);
			newFullName=newLabel+newFileName.GetPathSeparator();
		}else{
			wxString fileExt=newFileName.GetExt();
			newFileName.SetName(newLabel);
			newFullName=newFileName.GetFullName();
		}
		wxString newFileNamePath=newFileName.GetFullPath();
		if(wxRenameFile(thisFileName,newFileNamePath,false))
		{
			this->elementInfo.libelleElement=newLabel;
			filePath=newFullName;
			this->Modified(this); //Indique le l'element a été modifié
		}else{
			treeEvent.Veto();
			if(wxFileExists(newFileNamePath))
				wxLogError(_("File can't be renamed, a file with the same name already exists"));

		}
	}else{
		treeEvent.Veto();
	}
}

E_Report_File::E_Report_File(Element* parent,wxString Nom,wxString Path,ELEMENT_TYPE _type,wxXmlNode* nodeElement)
	:Element(parent,Nom,_type,nodeElement)
{
	if(parent && parent->GetElementInfos().typeElement!=ELEMENT_TYPE_RESULT_ROOT)
		this->elementInfo.userDestroyable=true;
	SetIcon(GRAPH_STATE_ALL,GRAPH_DISK_DEFAULT_FILE);
	blockUpdateNotification=false;
	wxString propVal;
	if(nodeElement!=NULL && nodeElement->GetAttribute("wxid",&propVal))
	{
		//Element initialisé AVEC Xml
		if(nodeElement->GetAttribute("filepath",&propVal) && propVal!="")
		{
			filePath=propVal;
		}
		wxXmlNode* currentChild;
		currentChild = nodeElement->GetChildren();
		// On va créer les fils de notre noeudCourant
		wxString propValue;
		while(currentChild!=NULL)
		{
			if(currentChild->GetAttribute("eid",&propValue))
			{
				long typeEle;
				propValue.ToLong(&typeEle);
				if(typeEle==ELEMENT_TYPE_REPORT_FOLDER)
				{
					this->AppendFils(new E_Report_Folder(this,currentChild));
				}else if(typeEle==ELEMENT_TYPE_REPORT_PARTVISUALISATION)
				{
					this->AppendFils(new E_Report_ParticulesVisualisation(this,currentChild));
				}else if(typeEle==ELEMENT_TYPE_REPORT_RECEPTEURSSVISUALISATION)
				{
					this->AppendFils(new E_Report_RecepteurSurfaciqueVisualisation(this,currentChild));
				}else if(typeEle==ELEMENT_TYPE_REPORT_GABE)
				{
					this->AppendFils(new E_Report_Gabe(this,currentChild));
				}else if(typeEle==ELEMENT_TYPE_REPORT_GABE_RECP)
				{
					this->AppendFils(new E_Report_Gabe_Recp(this,currentChild));
				}else if(typeEle==ELEMENT_TYPE_REPORT_GABE_GAP)
				{
					this->AppendFils(new E_Report_Gabe_Gap(this,currentChild));
				}else if(typeEle==ELEMENT_TYPE_REPORT_RPI)
				{
					this->AppendFils(new E_Report_ReceiverPonctualIntensity(this,currentChild));
				}else if(typeEle==ELEMENT_TYPE_REPORT_UNKNOWN)
				{
					this->AppendFils(new E_Report_Unknown(this,currentChild));
				} else if(typeEle==ELEMENT_TYPE_REPORT_GABE_RECPS) {
					this->AppendFils(new E_Report_Gabe_Recps(this, currentChild));
				}
			}
			currentChild = currentChild->GetNext();
		}
	}else{
		filePath=Path;
		this->Modified(this);
	}
}

void E_Report_File::OnBeginLabelEdit(wxTreeEvent& treeEvent)
{
	if(this->pere->GetElementInfos().typeElement==ELEMENT_TYPE_RESULT_ROOT)
		treeEvent.Veto();
}
void E_Report_File::PushElementPath(wxString& prefix,wxArrayString& tabPath)
{
	tabPath.push_back(prefix+filePath);
}

bool E_Report_File::IsExist(wxString& fullPath)
{
	return (wxFileExists(fullPath+filePath) || wxDirExists(fullPath+filePath));
}

void E_Report_File::DeleteFolder()
{
	wxString FullPath;
	this->BuildFullPath(FullPath);
	FuncDeleteFolder(FullPath);
    RefreshFolderContents();
}

void E_Report_File::RefreshFolderContents()
{

	///////////////////
	// Initialisation des variables
	///////////////////

	wxString fullPath;
	this->BuildFullPath(fullPath);
	//Chargement des éléments déjà présent comme fils
	wxSortedArrayString currentFileChilds;
	wxString thisFullPath;
	this->BuildFullPath(thisFullPath);
	for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
	{
		E_Report_File* eleFile=dynamic_cast<E_Report_File*>((*itfils));
		if(eleFile)
		{
			eleFile->PushElementPath(thisFullPath,currentFileChilds);
			eleFile->RefreshFolderContents();
		}
	}

	//Si cet élément est un dossier et si il existe
	if(wxDirExists(fullPath))
	{
 		///////////////////
		// Chargement des nouveau éléments dans le dossier
		//////////////////
		wxDir dir(fullPath);
		wxString fileName;
		wxString fileExt;
		wxArrayString realFileChilds;
		E_Report_File::GetAllFolderItems(fullPath,realFileChilds);
		for(unsigned long i=0;i<realFileChilds.size();i++)
		{
			wxFileName childFile(realFileChilds[i]);
			if(currentFileChilds.Index(realFileChilds[i])==wxNOT_FOUND )
			{
				//Le fichier n'a pas été référencé
				// Ajout de ce nouvel élément
				if(wxDirExists(childFile.GetFullPath()))
				{
					wxArrayString dirs=childFile.GetDirs();
					wxString dirName=dirs[dirs.size()-1];
					//Type inconnue, création d'un élément dossier générique
					E_Report_Folder* newFolder=new E_Report_Folder(this,dirName,dirName+wxFileName::GetPathSeparator());
					this->AppendFils(newFolder);
					newFolder->RefreshFolderContents();
				} else if(wxFileExists(childFile.GetFullPath()))
				{
					fileName=childFile.GetName();
					fileExt=childFile.GetExt();
					if(fileExt=="gabe")
					{
						E_Report_Gabe* newGabeFile=new E_Report_Gabe(this,fileName,childFile.GetFullName());
						this->AppendFils(newGabeFile);
					}else if(fileExt=="pbin")
					{
						E_Report_ParticulesVisualisation* newPartFolder=new E_Report_ParticulesVisualisation(this,fileName,childFile.GetFullName());
						this->AppendFils(newPartFolder);
					}else if(fileExt=="recp")
					{
						E_Report_Gabe_Recp* newGabeFile=new E_Report_Gabe_Recp(this,fileName,childFile.GetFullName());
						this->AppendFils(newGabeFile);
					}else if (fileExt == "recps")
					{
						E_Report_Gabe_Recps* newGabeFile = new E_Report_Gabe_Recps(this, fileName, childFile.GetFullName());
						this->AppendFils(newGabeFile);
					}
					else if(fileExt=="csbin")
					{
						E_Report_RecepteurSurfaciqueVisualisation* newRssFolder=new E_Report_RecepteurSurfaciqueVisualisation(this,fileName,childFile.GetFullName());
						this->AppendFils(newRssFolder);
					}else if(fileExt=="gap")
					{
						E_Report_Gabe_Gap* newGabeFile=new E_Report_Gabe_Gap(this,fileName,childFile.GetFullName());
						this->AppendFils(newGabeFile);
					}else if(fileExt=="rpi")
					{
						this->AppendFils(new E_Report_ReceiverPonctualIntensity(this,fileName,childFile.GetFullName()));
					}else if(E_Report_Unknown::ExtKnownByCurrentOs(childFile.GetFullName()))
					{
						this->AppendFils(new E_Report_Unknown(this,fileName,childFile.GetFullName()));
					}
				}
			}
		}
	}
	bool deleted=true;
	while(deleted)
	{
		deleted=false;
		for(std::list<Element*>::iterator itfils=this->fils.begin();itfils!=this->fils.end();itfils++)
		{
			E_Report_File* eleFile=dynamic_cast<E_Report_File*>((*itfils));
			if(eleFile)
			{
				if(!eleFile->IsExist(thisFullPath))
				{
					this->DeleteElementByXmlId(eleFile->GetElementInfos().xmlIdElement);
					deleted=true;
					break;
				}
			}
		}
	}
}
bool E_Report_File::OnElementRemoved()
{
	wxString FullPath;
	this->BuildFullPath(FullPath);
	if(wxFileExists(FullPath) || wxDirExists(FullPath))
	{
		wxMessageDialog dialog( NULL, wxString::Format(_("Are you sure to delete this element forever?\n%s"),FullPath),
			APPLICATION_NAME, wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		if( dialog.ShowModal()==wxID_NO)
		{
			return false;
		}else{
			DeleteFolder();
			return Element::OnElementRemoved();
		}
	}else{
		return Element::OnElementRemoved();
	}
}
void E_Report_File::OnRightClic( wxMenu* leMenu )
{
	wxFileName fileInfo(filePath);
	if(fileInfo.IsDir())
	{
		leMenu->Append(GetMenuItem(leMenu,IDEVENT_RELOAD_FOLDER,_("Refresh folder")));
	}
	Element::OnRightClic(leMenu);
}

void E_Report_File::GetAllFolderItems(wxString rootFolder,wxArrayString& folderDir)
{
	if(wxDirExists(rootFolder))
	{
		wxDir dir(rootFolder);
		wxString filename;
		bool cont = dir.GetFirst(&filename);
		while ( cont )
		{
			if(wxDirExists(rootFolder+filename))
				filename+=wxFileName::GetPathSeparator();
			folderDir.Add(rootFolder+filename);
			cont = dir.GetNext(&filename);
		}
	}
}

void E_Report_File::BuildFullPath(wxString& fullPath)
{
	fullPath=filePath+fullPath;
	if(this->pere)
	{
		E_Report_File* fatherFolder=dynamic_cast<E_Report_File*>(this->pere);
		if(fatherFolder)
			fatherFolder->BuildFullPath(fullPath);
		else
			fullPath=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+fullPath;
	}
}

void E_Report_File::SaveProperties(wxXmlNode* thisNode)
{
	thisNode->SetName("file"); // Nom de la balise xml ( pas d'espace autorise )
	thisNode->DeleteAttribute("filepath");
	thisNode->AddAttribute("filepath",filePath);
}

wxXmlNode* E_Report_File::SaveXMLDoc(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = Element::SaveXMLDoc(NoeudParent);
	SaveProperties(thisNode);
	return thisNode;
}

wxXmlNode* E_Report_File::SaveXMLDocWithoutChildren(wxXmlNode* NoeudParent)
{
	wxXmlNode* thisNode = Element::SaveXMLDocWithoutChildren(NoeudParent);
	SaveProperties(thisNode);
	return thisNode;
}

void E_Report_File::Modified(Element* eModif)
{
	if(!blockUpdateNotification)
		Element::Modified(eModif);
}
