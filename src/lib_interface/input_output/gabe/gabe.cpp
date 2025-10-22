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

#include "gabe.h"
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "std_tools.hpp"
#ifdef WIN32
#include "input_output/pugixml/src/pugixml.hpp"
#endif // WIN32

namespace formatGABE
{
	const unsigned int GABE_VERSION=2; //Mettre à jour la version si les structures de données t_** sont modifiées
	//Types Import/Export

	/**
	 * @brief Structure d'entete du fichier
	 */
	struct t_FileHeader
	{
	    t_FileHeader():formatVersion(0),t_FileHeader_Length(0),t_ColHeader_Length(0),cols(0),readOnly(false){}
		int32_t formatVersion;						/*!< Version du format de données */
		int32_t t_FileHeader_Length;		/*!< Taille de la structure */
		int32_t t_ColHeader_Length;		/*!< Taille de la structure */
		int32_t cols;									/*!< Nombre de colonnes enregistré */
		bool readOnly;								/*!< Données en lecture seule */
	};

	struct t_ColHeader
	{
		uint16_t colType;												/*!< GABE_OBJECTTYPE Type de la colonne */
		int32_t rows;															/*!< Nombre de lignes */
		int64_t sizeofCol;											/*!< Taille en octets de la colonne */
		int64_t sizeofHeader;									/*!< Taille en octets de l'entete spécifique de la colonne*/
		char label[STRING_LABEL_LENGTH];			/*!< Libellé de la colonne */
		t_ColHeader()
		{
			memset(label,0,STRING_LABEL_LENGTH);
		}
	};






	/**********************************************
	 * GABE_Object Class définition
	 **********************************************/

	GABE_Object::GABE_Object(GABE_OBJECTTYPE _objectType)
		:objectType(_objectType)
	{
		memset(label,0,sizeof(char)*STRING_LABEL_LENGTH);
	}


	void GABE_Object::SetLabel(const char * _label)
	{
		std::strncpy(label,_label, sizeof(label));
	}
	
	const char * GABE_Object::GetStringEquiv(int numRow)
	{
		return 0;
	}
	/**********************************************
	 * GABE_Data Class définition
	 **********************************************/

	template<class Td, class Ts>
	bool GABE_Data<Td, Ts>::ReadFile(std::fstream & fs)
	{
		return false;
	}

	template<class Td, class Ts>
	bool GABE_Data<Td, Ts>::WriteFile(std::fstream & fs)
	{
		return false;
	}

	template<class Td,class Ts> char * GABE_Data<Td,Ts>::GetFirstChar()
	{
		return (char *)tabData;
	}

	template<class Td,class Ts> void GABE_Data<Td,Ts>::FastCopy(const Td * src)
	{
		memcpy(tabData,src,this->GetLength());
	}
	/**********************************************
	 * GABE_Data_Float Class définition
	 **********************************************/
	GABE_Data_Float::GABE_Data_Float(Longb _size)
		:GABE_Data<Floatb,t_HeaderFloat>(_size,GABE_OBJECTTYPE_FLOAT)
	{
		memset(tabData,0,sizeof(float)*_size);
	}
	bool GABE_Data_Float::WriteFile(std::fstream& fs) {
		int32_t numOfDigits = headerData.numOfDigits;
		fs.write((char*)&numOfDigits, sizeof(numOfDigits));
		fs.write((char*)tabData, sizeof(Floatb) * size);
		return true;
	}
	bool GABE_Data_Float::ReadFile(std::fstream& fs) {
		int32_t numOfDigits;
		fs.read((char*)&numOfDigits, sizeof(numOfDigits));
		headerData.numOfDigits = numOfDigits;
		fs.read((char*)tabData, sizeof(Floatb) * size);
		return true;
	}
	const char * GABE_Data_Float::GetStringEquiv(int numRow)
	{
		if(st_isfinite(tabData[numRow]))
		{
			//_gcvt(tabData[numRow],this->headerData.numOfDigits,buffer);
            std::string form;
            std::stringstream form_stream;
            form_stream<<"%"<<this->headerData.numOfDigits<<"g";
            form_stream>>form;
            sprintf(buffer,form.c_str(),tabData[numRow]);
		}else{
			if(tabData[numRow]>0)
				strcpy(buffer, "+inf");
			else
				strcpy(buffer, "-inf");
		}
		return buffer;
	}
	GABE_Object* GABE_Data_Float::GetCopy() const
	{
		return new GABE_Data_Float(*this);
	}
	/**********************************************
	 * GABE_Data_ShortString Class définition
	 **********************************************/

	GABE_Data_ShortString::GABE_Data_ShortString(Longb _size)
		:GABE_Data<t_StringShort>(_size,GABE_OBJECTTYPE_SHORTSTRING)
	{
		memset(tabData,0,sizeof(t_StringShort)*_size);
	}
	void GABE_Data_ShortString::SetString(int numRow,const char * strToCopy)
	{
		if(strToCopy)
			strcpy(tabData[numRow].strData,strToCopy);
	}

	bool GABE_Data_ShortString::WriteFile(std::fstream& fs) {
		fs.seekp(1, std::ios_base::cur);
		fs.write((char*)tabData, sizeof(t_StringShort) * size);
		return true;
	}
	bool GABE_Data_ShortString::ReadFile(std::fstream& fs) {
		fs.seekg(1, std::ios_base::cur);
		fs.read((char*)tabData, sizeof(t_StringShort) * size);
		return true;
	}
	GABE_Object* GABE_Data_ShortString::GetCopy() const
	{
		return new GABE_Data_ShortString(*this);
	}

	/**********************************************
	 * GABE_Data_Integer Class définition
	 **********************************************/
	GABE_Data_Integer::GABE_Data_Integer(Longb _size)
		:GABE_Data<Intb>(_size,GABE_OBJECTTYPE_INT)
	{
		memset(tabData,0,sizeof(int)*_size);
	}
	bool GABE_Data_Integer::ReadFile(std::fstream& fs) {
		fs.seekg(1, std::ios_base::cur);
		fs.read((char*)tabData, sizeof(int32_t) * size);
		return true;
	}
	bool GABE_Data_Integer::WriteFile(std::fstream& fs) {
		fs.seekp(1, std::ios_base::cur);
		fs.write((char*)tabData, sizeof(int32_t) * size);
		return true;
	}
	const char * GABE_Data_Integer::GetStringEquiv(int numRow)
	{
		//_itoa(tabData[numRow],buffer,10);
        sprintf(buffer,"%i",tabData[numRow]);
		return buffer;
	}


	GABE_Object* GABE_Data_Integer::GetCopy() const
	{
		return new GABE_Data_Integer(*this);
	}


	/**********************************************
	 * GABE Class définition
	 **********************************************/
	GABE::GABE( )
		:readOnly(false)
	{
		InitCols();
	}
	GABE::GABE(Longb _cols)
		:readOnly(false)
	{
		colsContainer.reserve(_cols);
	}

	GABE::GABE(const GABE& cpyFrom)
		:readOnly(cpyFrom.IsReadOnly())
	{
		InitCols();
		for(Longb idcol=0;idcol<cpyFrom.GetCols(); idcol++)
			colsContainer.push_back(cpyFrom.GetConstCol(idcol)->GetCopy());
	}
	void GABE::InitCols()
	{
		while(!colsContainer.empty()) {
			delete colsContainer.back();
			colsContainer.pop_back();
		}
	}
	GABE::GABE(const char *strFileName)
	{
		InitCols();
		Load(strFileName);
	}

	GABE::~GABE()
	{
		Destroy();
	}

	void GABE::Redim(const Longb & colsCount)
	{
		// Remove all colIds >= colsCount
		for (int idCol = colsCount; idCol < colsContainer.size(); idCol++ ) {
			if(colsContainer.at(idCol) != nullptr) {
				delete colsContainer.at(idCol);
				colsContainer.at(idCol) = nullptr;
			}
		}
		colsContainer.resize(colsCount);
	}
	
	void GABE::SetCol(Longb numCol,const GABE_Object& newCol)
	{
		if(numCol >= colsContainer.size()) {
			colsContainer.resize(numCol + 1, nullptr);
		}
		if(colsContainer.at(numCol) != nullptr) {
			delete colsContainer.at(numCol);
		}
		colsContainer.at(numCol) = newCol.GetCopy();
	}

	void GABE::SetCol(Longb numCol,GABE_Object* newCol)
	{
		if (numCol >= colsContainer.size()) {
			colsContainer.resize(numCol + 1, nullptr);
		}
		if (colsContainer.at(numCol) != nullptr) {
			delete colsContainer.at(numCol);
		}
		colsContainer.at(numCol) = newCol;
	}

	GABE_Object* GABE::GetCol(Longb numCol)
	{
		if(numCol>=0 && numCol<colsContainer.size())
			return colsContainer[numCol];
		else
			return nullptr;
	}

	const GABE_Object* GABE::GetConstCol(Longb numCol) const
	{
		if (numCol >= 0 && numCol<colsContainer.size())
			return colsContainer[numCol];
		else
			return nullptr;
	}
	bool GABE::GetCol(Longb numCol,GABE_Data_ShortString** objData)
	{
		*objData=NULL;
		*objData=dynamic_cast<GABE_Data_ShortString*>(GetCol(numCol));
		return *objData;
	}
	bool GABE::GetCol(Longb numCol,GABE_Data_Float** objData)
	{
		*objData=NULL;
		*objData=dynamic_cast<GABE_Data_Float*>(GetCol(numCol));
		return *objData;
	}
	bool GABE::GetCol(Longb numCol,GABE_Data_Integer** objData)
	{
		*objData=NULL;
		*objData=dynamic_cast<GABE_Data_Integer*>(GetCol(numCol));
		return *objData;
	}

	template<class Td,class Ts>
	bool GABE::GetCol(Longb numCol,GABE_Data<Td,Ts>** objData)
	{
		*objData=NULL;
		*objData=dynamic_cast<GABE_Data<Td,Ts>*>(GetCol(numCol));
		return *objData;
	}
	Longb GABE::GetCols() const
	{
		return colsContainer.size();
	}
	void GABE::Destroy()
	{
		InitCols();
	}
	GABE_Object* GABE::operator[](int numCol)
	{
		return GetCol(numCol);
	}
	bool GABE::Load(const std::string& strFileName)
	{
		Destroy();
		//Namespace
		using namespace std;
		
		
		#ifdef WIN32
			// If file failed to open that means that provided path are not in utf8
			// From wxString call myPath.utf8_str()
			fstream binFile(pugi::as_wide(strFileName), ios::in | ios::binary);			
		#else
			fstream binFile(strFileName, ios::in | ios::binary);
		#endif // WIN
		if (!binFile.is_open())
			return false;
		
		// Load file header
		t_FileHeader fileheader;
		binFile.read((char*)&fileheader.formatVersion, sizeof(t_FileHeader::formatVersion));
		binFile.read((char*)&fileheader.t_FileHeader_Length, sizeof(t_FileHeader::t_FileHeader_Length));
		binFile.read((char*)&fileheader.t_ColHeader_Length, sizeof(t_FileHeader::t_ColHeader_Length));
		binFile.read((char*)&fileheader.cols, sizeof(t_FileHeader::cols));
		binFile.read((char*)&fileheader.readOnly, sizeof(t_FileHeader::readOnly));
		// Skip 3 bytes
		binFile.seekg(3, std::ios_base::cur);


		if (fileheader.formatVersion > (const int)GABE_VERSION) {
			return false;
		}

		readOnly = fileheader.readOnly;
		colsContainer.reserve(fileheader.cols);
		InitCols();
		// For each column (Gabe object)
		for (Longb idcol = 0; idcol<fileheader.cols; idcol++)
		{
			GABE_Object* newObject = NULL;
			t_ColHeader colHeader;
			binFile.read((char*)&colHeader.colType, sizeof(t_ColHeader::colType));
			// Skip 2 bytes
			binFile.seekg(2, std::ios_base::cur);
			binFile.read((char*)&colHeader.rows, sizeof(t_ColHeader::rows));
			binFile.read((char*)&colHeader.sizeofCol, sizeof(t_ColHeader::sizeofCol));
			binFile.read((char*)&colHeader.sizeofHeader, sizeof(t_ColHeader::sizeofHeader));
			binFile.read((char*)&colHeader.label, sizeof(t_ColHeader::label));
			// Skip 1 bytes
			binFile.seekg(1, std::ios_base::cur);
			switch ((GABE_OBJECTTYPE)colHeader.colType)
			{
				case GABE_OBJECTTYPE_FLOAT:
					newObject = new GABE_Data_Float(colHeader.rows);
					break;
				case GABE_OBJECTTYPE_SHORTSTRING:
					newObject = new GABE_Data_ShortString(colHeader.rows);
					break;
				case GABE_OBJECTTYPE_INT:
					newObject = new GABE_Data_Integer(colHeader.rows);
					break;
			}
			if (newObject)
			{
				//Lecture des données génériques
				newObject->SetLabel(colHeader.label);
				if(!newObject->ReadFile(binFile)) {
					Destroy();
					delete newObject;
					binFile.close();
					return false;
				}
				colsContainer.push_back(newObject);
			}
			else {
				//Unkown column
				binFile.seekg(colHeader.sizeofCol, std::ios_base::cur);
				if(binFile.rdstate() != std::ifstream::goodbit) {
					Destroy();
					binFile.close();
					return false;
				}
			}
		}
		binFile.close();
		return true;
	}

	void GABE::LockData()
	{
		readOnly=true;
	}

	void GABE::UnlockData()
	{
		readOnly=false;
	}

	bool GABE::IsReadOnly() const
	{
		return readOnly;
	}

	bool GABE::Save(const std::string& strFileName)
	{
		//Namespace
		using namespace std;

		//Declarations

		#ifdef WIN32
				fstream binFile(pugi::as_wide(strFileName), ios::out | ios::binary);
		#else
				fstream binFile(strFileName, ios::out | ios::binary);
		#endif // WIN

		//Ecriture de l'entete du fichier
		t_FileHeader fileheader;
		fileheader.cols=0;
		for(Longb idcol=0;idcol<colsContainer.size();idcol++)
			if(colsContainer[idcol] != nullptr)
				fileheader.cols++;
		fileheader.t_FileHeader_Length=0;
		fileheader.t_ColHeader_Length=0;
		fileheader.readOnly=readOnly;
		fileheader.formatVersion=GABE_VERSION;

		binFile.write((char*)&fileheader.formatVersion ,sizeof(t_FileHeader::formatVersion));
		binFile.write((char*)&fileheader.t_FileHeader_Length, sizeof(t_FileHeader::t_FileHeader_Length));
		binFile.write((char*)&fileheader.t_ColHeader_Length, sizeof(t_FileHeader::t_ColHeader_Length));
		binFile.write((char*)&fileheader.cols, sizeof(t_FileHeader::cols));
		binFile.write((char*)&fileheader.readOnly, sizeof(t_FileHeader::readOnly));
		// Skip 3 bytes
		binFile.seekp(3, std::ios_base::cur);

		//Pour chaque colonne
		for(Longb idcol=0;idcol<colsContainer.size();idcol++)
		{
			GABE_Object* currentObject=colsContainer[idcol];
			if(currentObject)
			{
				//Ecriture de l'entete de la colonne
				t_ColHeader colHeader;
				colHeader.colType=currentObject->GetObjectType();
				colHeader.rows=currentObject->GetSize();
				colHeader.sizeofCol=currentObject->GetLength();
				colHeader.sizeofHeader=currentObject->GetHeaderLength();				
				binFile.write((char*)&colHeader.colType ,sizeof(t_ColHeader::colType));
				// Skip 2 bytes
				binFile.seekp(2, std::ios_base::cur);
				binFile.write((char*)&colHeader.rows, sizeof(t_ColHeader::rows));
				binFile.write((char*)&colHeader.sizeofCol, sizeof(t_ColHeader::sizeofCol));
				binFile.write((char*)&colHeader.sizeofHeader, sizeof(t_ColHeader::sizeofHeader));
				binFile.write(currentObject->GetLabel(), sizeof(t_ColHeader::label));
				// Skip 1 byte
				binFile.seekp(1, std::ios_base::cur);

				currentObject->WriteFile(binFile);
			}
		}

		binFile.close();
		return true;
	}

} //fin namespace
