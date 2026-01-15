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

#include "rsbin.h"
#include <fstream>
#ifdef WIN32
#include "input_output/pugixml/src/pugixml.hpp"
#endif // WIN32

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

    // Fonctions utilitaires privées pour garantir la lecture/écriture membre par membre
    namespace Helpers {
        void WriteHeader(std::ostream& os, const t_FileHeader& h) {
            os.write((char*)&h.formatVersion, sizeof(Intb));
            os.write((char*)&h.t_FileHeader_Length, sizeof(Longb));
            os.write((char*)&h.t_nodesPosition_Length, sizeof(Longb));
            os.write((char*)&h.t_RecepteurS_Length, sizeof(Longb));
            os.write((char*)&h.t_FaceRS_Length, sizeof(Longb));
            os.write((char*)&h.t_faceValue_Length, sizeof(Longb));
            os.write((char*)&h.quantNodes, sizeof(Intb));
            os.write((char*)&h.quantRS, sizeof(Intb));
            os.write((char*)&h.nbTimeStep, sizeof(Intb));
            os.write((char*)&h.timeStep, sizeof(Floatb));
            Intb rType = (Intb)h.recordType;
            os.write((char*)&rType, sizeof(Intb));
        }

        void ReadHeader(std::istream& is, t_FileHeader& h) {
            is.read((char*)&h.formatVersion, sizeof(Intb));
            is.read((char*)&h.t_FileHeader_Length, sizeof(Longb));
            is.read((char*)&h.t_nodesPosition_Length, sizeof(Longb));
            is.read((char*)&h.t_RecepteurS_Length, sizeof(Longb));
            is.read((char*)&h.t_FaceRS_Length, sizeof(Longb));
            is.read((char*)&h.t_faceValue_Length, sizeof(Longb));
            is.read((char*)&h.quantNodes, sizeof(Intb));
            is.read((char*)&h.quantRS, sizeof(Intb));
            is.read((char*)&h.nbTimeStep, sizeof(Intb));
            is.read((char*)&h.timeStep, sizeof(Floatb));
            Intb rType;
            is.read((char*)&rType, sizeof(Intb));
            h.recordType = (RECEPTEURS_RECORD_TYPE)rType;
        }
    }

    bool RSBIN::ReadHeaderBIN(const char *strFileName, t_ExchangeData& mainData)
    {
        #ifdef WIN32
            std::ifstream binFile(pugi::as_wide(strFileName), std::ios::in | std::ios::binary);
        #else
            std::ifstream binFile(strFileName, std::ios::in | std::ios::binary);
        #endif

        if(!binFile.is_open()) return false;

        t_FileHeader fileHeader;
        Helpers::ReadHeader(binFile, fileHeader);

        mainData.nbTimeStep = fileHeader.nbTimeStep;
        mainData.tabNodesSize = fileHeader.quantNodes;
        mainData.tabRsSize = fileHeader.quantRS;
        mainData.timeStep = fileHeader.timeStep;
        mainData.recordType = fileHeader.recordType;
        
        return true;
    }

    bool RSBIN::ImportBIN(const char *strFileName, t_ExchangeData& mainData)
    {
        #ifdef WIN32
            std::ifstream binFile(pugi::as_wide(strFileName), std::ios::in | std::ios::binary);
        #else
            std::ifstream binFile(strFileName, std::ios::in | std::ios::binary);
        #endif

        if(!binFile.is_open()) return false;

        t_FileHeader fileHeader;
        Helpers::ReadHeader(binFile, fileHeader);

        mainData.nbTimeStep = fileHeader.nbTimeStep;
        mainData.tabNodesSize = fileHeader.quantNodes;
        mainData.tabRsSize = fileHeader.quantRS;
        mainData.timeStep = fileHeader.timeStep;
        mainData.recordType = fileHeader.recordType;

        // Gestion de la compatibilité de version pour le header
        if (fileHeader.t_FileHeader_Length > 44) // 44 est la taille minimale calculée du header
            binFile.seekg((Longb)binFile.tellg() + fileHeader.t_FileHeader_Length - 44);

        mainData.tabNodes = new t_nodesPosition[mainData.tabNodesSize];
        for(int idNoeud=0; idNoeud < mainData.tabNodesSize; idNoeud++)
        {
            binFile.read((char*)mainData.tabNodes[idNoeud].node, sizeof(Floatb)*3);
            if(fileHeader.t_nodesPosition_Length > sizeof(Floatb)*3)
                binFile.seekg((Longb)binFile.tellg() + fileHeader.t_nodesPosition_Length - (sizeof(Floatb)*3));
        }

        mainData.tabRs = new t_ExchangeData_Recepteurs[mainData.tabRsSize];
        for(int idRs=0; idRs < mainData.tabRsSize; idRs++)
        {
            // Lecture t_RecepteurS
            binFile.read((char*)&mainData.tabRs[idRs].dataRec.xmlIndex, sizeof(Intb));
            binFile.read((char*)&mainData.tabRs[idRs].dataRec.quantFaces, sizeof(Intb));
            binFile.read((char*)mainData.tabRs[idRs].dataRec.recepteurSName, STRING_SIZE);

            if(fileHeader.t_RecepteurS_Length > (sizeof(Intb)*2 + STRING_SIZE))
                binFile.seekg((Longb)binFile.tellg() + fileHeader.t_RecepteurS_Length - (sizeof(Intb)*2 + STRING_SIZE));

            mainData.tabRs[idRs].dataFaces = new t_ExchangeData_Face[mainData.tabRs[idRs].dataRec.quantFaces];
            
            for(int idFace=0; idFace < mainData.tabRs[idRs].dataRec.quantFaces; idFace++)
            {
                // Lecture t_FaceRS
                binFile.read((char*)mainData.tabRs[idRs].dataFaces[idFace].dataFace.sommetsIndex, sizeof(Intb)*3);
                binFile.read((char*)&mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords, sizeof(Intb));

                if(fileHeader.t_FaceRS_Length > sizeof(Intb)*4)
                    binFile.seekg((Longb)binFile.tellg() + fileHeader.t_FaceRS_Length - (sizeof(Intb)*4));

                mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep = new t_faceValue[mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords];
                
                for(int idTimeStep=0; idTimeStep < mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords; idTimeStep++)
                {
                    // Lecture t_faceValue
                    binFile.read((char*)&mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idTimeStep].timeStep, sizeof(bCourt));
                    binFile.read((char*)&mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idTimeStep].energy, sizeof(Floatb));

                    if(fileHeader.t_faceValue_Length > (sizeof(bCourt)+sizeof(Floatb)))
                        binFile.seekg((Longb)binFile.tellg() + fileHeader.t_faceValue_Length - (sizeof(bCourt)+sizeof(Floatb)));
                }
            }
        }
        binFile.close();
        return true;
    }

    bool RSBIN::ExportBIN(const char *strFileName, t_ExchangeData& mainData)
    {
        #ifdef WIN32
            std::ofstream binFile(pugi::as_wide(strFileName), std::ios::out | std::ios::binary);
        #else
            std::ofstream binFile(strFileName, std::ios::out | std::ios::binary);
        #endif

        if(!binFile.is_open()) return false;

        t_FileHeader fileHeader;
        fileHeader.formatVersion = VERSION;
        fileHeader.nbTimeStep = mainData.nbTimeStep;
        fileHeader.quantNodes = mainData.tabNodesSize;
        fileHeader.quantRS = mainData.tabRsSize;
        // On définit des tailles fixes (sans padding) pour l'export
        fileHeader.t_FileHeader_Length = 44; 
        fileHeader.t_nodesPosition_Length = sizeof(Floatb) * 3;
        fileHeader.t_RecepteurS_Length = sizeof(Intb) * 2 + STRING_SIZE;
        fileHeader.t_FaceRS_Length = sizeof(Intb) * 4;
        fileHeader.t_faceValue_Length = sizeof(bCourt) + sizeof(Floatb);
        fileHeader.timeStep = mainData.timeStep;
        fileHeader.recordType = mainData.recordType;

        Helpers::WriteHeader(binFile, fileHeader);

        for(int idNoeud=0; idNoeud < mainData.tabNodesSize; idNoeud++) {
            binFile.write((char*)mainData.tabNodes[idNoeud].node, sizeof(Floatb)*3);
        }

        for(int idRs=0; idRs < mainData.tabRsSize; idRs++)
        {
            binFile.write((char*)&mainData.tabRs[idRs].dataRec.xmlIndex, sizeof(Intb));
            binFile.write((char*)&mainData.tabRs[idRs].dataRec.quantFaces, sizeof(Intb));
            binFile.write((char*)mainData.tabRs[idRs].dataRec.recepteurSName, STRING_SIZE);

            for(int idFace=0; idFace < mainData.tabRs[idRs].dataRec.quantFaces; idFace++)
            {
                binFile.write((char*)mainData.tabRs[idRs].dataFaces[idFace].dataFace.sommetsIndex, sizeof(Intb)*3);
                binFile.write((char*)&mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords, sizeof(Intb));

                for(int idTimeStep=0; idTimeStep < mainData.tabRs[idRs].dataFaces[idFace].dataFace.nbRecords; idTimeStep++)
                {
                    binFile.write((char*)&mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idTimeStep].timeStep, sizeof(bCourt));
                    binFile.write((char*)&mainData.tabRs[idRs].dataFaces[idFace].tabTimeStep[idTimeStep].energy, sizeof(Floatb));
                }
            }
        }
        binFile.close();
        return true;
    }
}

