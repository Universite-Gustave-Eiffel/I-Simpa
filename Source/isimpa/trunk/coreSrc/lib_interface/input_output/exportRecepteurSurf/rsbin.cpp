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

#include "rsbin.h"
#include <fstream>

namespace formatRSBIN
{
	/**
	 * @brief Structure d'entete du fichier
	 */
	struct t_FileHeader
	{
		Intb formatVersion;				/*!< Version du format de données */
		Longb t_FileHeader_Length;		/*!< Taille de la structure */
		Longb t_nodesPosition_Length;	/*!< Taille de la structure */
		Longb t_RecepteurS_Length;		/*!< Taille de la structure */
		Longb t_FaceRS_Length;		    /*!< Taille de la structure */
		Longb t_faceValue_Length;	 	/*!< Taille de la structure */
		Intb quantNodes;				/*!< Nombre de noeuds (point liant les faces) */
		Intb quantRS;					/*!< Nombre de récepteur surfacique */
		Intb nbTimeStep;				/*!< Nombre de pas de temps */
		Floatb timeStep;				/*!< Pas de temps (s)*/
		RECEPTEURS_RECORD_TYPE recordType; /*!< Type d'enregistrement de ce récepteur de surface */
	};
    bool RSBIN::ReadHeaderBIN(const char *strFileName,t_ExchangeData& mainData)
	{
		//Namespace
		using namespace std;

		//Declarations

		//Sauvegarde
		fstream binFile (strFileName, ios::in | ios::binary);

		//*************************
		// Lecture de l'entete du fichier
		t_FileHeader fileHeader;
		binFile.read((char*)&fileHeader,sizeof(t_FileHeader));

		mainData.nbTimeStep=fileHeader.nbTimeStep;
		mainData.tabNodesSize=fileHeader.quantNodes;
		mainData.tabRsSize=fileHeader.quantRS;
		mainData.timeStep=fileHeader.timeStep;
		mainData.recordType=fileHeader.recordType;
		binFile.close();
		return true;

	}

	bool RSBIN::ImportBIN(const char *strFileName,t_ExchangeData& mainData)
	{
		//Namespace
		using namespace std;

		//Declarations

		//Sauvegarde
		fstream binFile (strFileName, ios::in | ios::binary);

		//*************************
		// Lecture de l'entete du fichier
		t_FileHeader fileHeader;
		binFile.read((char*)&fileHeader,sizeof(t_FileHeader));

		// Ce booleen permet de rester compatible avec les futurs versions des fichiers
		// A condition que les données soient rajouté à la fin des structures des données et que la constante de version ai été mis à jour
		bool versionConflict=(fileHeader.formatVersion!=(const int)VERSION);
		mainData.nbTimeStep=fileHeader.nbTimeStep;
		mainData.tabNodesSize=fileHeader.quantNodes;
		mainData.tabRsSize=fileHeader.quantRS;
		mainData.tabNodes=new t_nodesPosition[mainData.tabNodesSize];
		mainData.tabRs = new t_ExchangeData_Recepteurs[mainData.tabRsSize];
		mainData.timeStep=fileHeader.timeStep;
		mainData.recordType=fileHeader.recordType;

		if(versionConflict)
			binFile.seekp((Longb)binFile.tellp()+fileHeader.t_FileHeader_Length-sizeof(t_FileHeader));

		//***************************
		// Lecture des noeuds
		for(int idNoeud=0;idNoeud<mainData.tabNodesSize;idNoeud++)
		{
			binFile.read((char*)&mainData.tabNodes[idNoeud],sizeof(t_nodesPosition));
			if(versionConflict)
				binFile.seekp((Longb)binFile.tellp()+fileHeader.t_nodesPosition_Length-sizeof(t_nodesPosition));
		}
		//***************************
		// Lecture des recepteurs surfaciques
		for(int idRs=0;idRs<mainData.tabRsSize;idRs++)
		{
			binFile.read((char*)&mainData.tabRs[idRs].dataRec,sizeof(t_RecepteurS));
			if(versionConflict)
				binFile.seekp((Longb)binFile.tellp()+fileHeader.t_RecepteurS_Length-sizeof(t_RecepteurS));
			mainData.tabRs[idRs].dataFaces = new t_ExchangeData_Face[mainData.tabRs[idRs].dataRec.quantFaces];
			//Lecture des faces
			for(int idFace=0;idFace<mainData.tabRs[idRs].dataRec.quantFaces;idFace++)
			{
				binFile.read((char*)&mainData.tabRs[idRs].dataFaces[idFace].dataFace,sizeof(t_FaceRS));
				if(versionConflict)
					binFile.seekp((Longb)binFile.tellp()+fileHeader.t_FaceRS_Length-sizeof(t_FaceRS));
				mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep = new t_faceValue[mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords];
				//Lecture des valeurs des pas de temps
				for(int idTimeStep=0;idTimeStep<mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords;idTimeStep++)
				{
					binFile.read((char*)&mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idTimeStep],sizeof(t_faceValue));
					if(versionConflict)
						binFile.seekp((Longb)binFile.tellp()+fileHeader.t_faceValue_Length-sizeof(t_faceValue));
				}
			}
		}
		binFile.close();
		return true;
	}


	bool RSBIN::ExportBIN(const char *strFileName,t_ExchangeData& mainData)
	{
		//Namespace
		using namespace std;

		//Declarations

		//Sauvegarde du modèle 3D
		fstream binFile (strFileName, ios::out | ios::binary);

		//*************************
		//Ecriture de l'entete du fichier
		t_FileHeader fileHeader;
		fileHeader.formatVersion=VERSION;
		fileHeader.nbTimeStep=mainData.nbTimeStep;
		fileHeader.quantNodes=mainData.tabNodesSize;
		fileHeader.quantRS=mainData.tabRsSize;
		fileHeader.t_FileHeader_Length=sizeof(t_FileHeader);
		fileHeader.t_nodesPosition_Length=sizeof(t_nodesPosition);
		fileHeader.t_RecepteurS_Length=sizeof(t_RecepteurS);
		fileHeader.t_FaceRS_Length=sizeof(t_FaceRS);
		fileHeader.t_faceValue_Length=sizeof(t_faceValue);
		fileHeader.timeStep=mainData.timeStep;
		fileHeader.recordType=mainData.recordType;

		binFile.write((char*)&fileHeader,sizeof(t_FileHeader));
		//***************************
		// Ecriture des noeuds
		for(int idNoeud=0;idNoeud<mainData.tabNodesSize;idNoeud++)
			binFile.write((char*)&mainData.tabNodes[idNoeud],sizeof(t_nodesPosition));
		//***************************
		// Ecriture des recepteurs surfaciques
		for(int idRs=0;idRs<mainData.tabRsSize;idRs++)
		{
			binFile.write((char*)&mainData.tabRs[idRs].dataRec,sizeof(t_RecepteurS));
			//Ecriture des faces
			for(int idFace=0;idFace<mainData.tabRs[idRs].dataRec.quantFaces;idFace++)
			{
				binFile.write((char*)&mainData.tabRs[idRs].dataFaces[idFace].dataFace,sizeof(t_FaceRS));
				//Ecriture des valeurs des pas de temps
				for(int idTimeStep=0;idTimeStep<mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords;idTimeStep++)
				{
					binFile.write((char*)&mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idTimeStep],sizeof(t_faceValue));
				}
			}
		}
		binFile.close();
		return true;
	}

} //fin namespace

