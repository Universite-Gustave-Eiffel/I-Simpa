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
#include <float.h>
#include "std_tools.hpp"

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
		Intb formatVersion;						/*!< Version du format de données */
		Longb t_FileHeader_Length;		/*!< Taille de la structure */
		Longb t_ColHeader_Length;		/*!< Taille de la structure */
		Longb cols;									/*!< Nombre de colonnes enregistré */
		bool readOnly;								/*!< Données en lecture seule */
	};

	struct t_ColHeader
	{
		UShortb colType;												/*!< GABE_OBJECTTYPE Type de la colonne */
		Longb rows;															/*!< Nombre de lignes */
		LongLongb sizeofCol;											/*!< Taille en octets de la colonne */
		LongLongb sizeofHeader;									/*!< Taille en octets de l'entete spécifique de la colonne*/
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
		strcpy(label,_label);
	}

	const char * GABE_Object::GetStringEquiv(int numRow)
	{
		return 0;
	}
	/**********************************************
	 * GABE_Data Class définition
	 **********************************************/

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
		cols=0;
		InitCols();
	}
	GABE::GABE(Longb _cols)
		:readOnly(false)
	{
		cols=_cols;
		InitCols();
	}

	GABE::GABE(const GABE& cpyFrom)
		:readOnly(cpyFrom.IsReadOnly())
	{
		cols=cpyFrom.GetCols();
		InitCols();
		for(Longb idcol=0;idcol<cols;idcol++)
			this->SetCol(idcol,*cpyFrom.GetConstCol(idcol));
	}
	void GABE::InitCols()
	{
		if(cols>0)
		{
			colsContainer= new GABE_Object*[cols];
			memset(colsContainer,0,sizeof(GABE_Object*)*cols);
		}else{
			colsContainer=NULL;
		}
	}
	GABE::GABE(const char *strFileName)
	{
		cols=0;
		InitCols();
		Load(strFileName);
	}

	GABE::~GABE()
	{
		Destroy();
	}

	void GABE::Redim(const Longb& colsCount)
	{
		Longb ansdim=cols;
		//Supprime les colonnes qui ne sont plus utilisés
		for(Longb idcol=colsCount;idcol<ansdim;idcol++)
		{
			delete colsContainer[idcol];
			colsContainer[idcol]=NULL;
		}
		//Genere le nouveau tableau
		GABE_Object** tmppt=new GABE_Object*[colsCount];
		memset(tmppt,0,sizeof(GABE_Object*)*colsCount);
		//Copie les anciennes colonnes
		for(Longb idcol=0;idcol<ansdim-1;idcol++)
			tmppt[idcol]=colsContainer[idcol];
		//Suppression des pointeurs
		delete[] colsContainer;
		colsContainer=tmppt;
		cols=colsCount;
	}

	void GABE::SetCol(Longb numCol,const GABE_Object& newCol) const
	{
		if(numCol>=0 && numCol<cols)
		{
			if(colsContainer[numCol]!=NULL)
				delete colsContainer[numCol];
			colsContainer[numCol]=newCol.GetCopy();
		}
	}
	void GABE::SetCol(Longb numCol,GABE_Object* newCol)
	{
		if(numCol>=0 && numCol<cols)
		{
			if(colsContainer[numCol]!=NULL)
				delete colsContainer[numCol];
			colsContainer[numCol]=newCol;
		}
	}

	GABE_Object* GABE::GetCol(Longb numCol)
	{
		if(numCol>=0 && numCol<cols)
			return colsContainer[numCol];
		else
			return NULL;
	}

	const GABE_Object* GABE::GetConstCol(Longb numCol) const
	{
		if(numCol>=0 && numCol<cols)
			return colsContainer[numCol];
		else
			return NULL;
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
		return cols;
	}
	void GABE::Destroy()
	{
		//Suppression des colonnes
		for(Longb idcol=0;idcol<cols;idcol++)
			delete colsContainer[idcol];
		//Suppression des pointeurs
		delete[] colsContainer;
		cols=0;
		colsContainer=NULL;
	}
	GABE_Object* GABE::operator[](int numCol)
	{
		return GetCol(numCol);
	}
	bool GABE::Load(const char *strFileName)
	{
		Destroy();
		//Namespace
		using namespace std;
		bool isOtherVersion=false;
		//Declarations

		fstream binFile (strFileName, ios::in | ios::binary);

		//Ecriture de l'entete du fichier
		t_FileHeader fileheader;

		if(binFile.read((char*)&fileheader ,sizeof(t_FileHeader)))
		{
			readOnly=fileheader.readOnly;
			cols=fileheader.cols;
			InitCols();

			if(fileheader.formatVersion!=(const int)GABE_VERSION) //Si la version du fichier est différente
				isOtherVersion=true;

			if(isOtherVersion)
				binFile.seekp((Longb)binFile.tellp()-(Longb)sizeof(t_FileHeader)+fileheader.t_FileHeader_Length);
			//Pour chaque colonne
			for(Longb idcol=0;idcol<cols;idcol++)
			{
				GABE_Object* newObject=NULL;
				t_ColHeader colHeader;
				binFile.read((char*)&colHeader ,sizeof(t_ColHeader));
				if(isOtherVersion)
					binFile.seekp((Longb)binFile.tellp()-(Longb)sizeof(t_ColHeader)+fileheader.t_ColHeader_Length);
				switch((GABE_OBJECTTYPE)colHeader.colType)
				{
					case GABE_OBJECTTYPE_FLOAT:
						newObject=new GABE_Data_Float(colHeader.rows);
						break;
					case GABE_OBJECTTYPE_SHORTSTRING:
						newObject=new GABE_Data_ShortString(colHeader.rows);
						break;
					case GABE_OBJECTTYPE_INT:
						newObject=new GABE_Data_Integer(colHeader.rows);
						break;
						/*
					default:
						newObject=new GABE_Data_Unknown<colHeader.sizeofCol/colHeader.rows,colHeader.sizeofHeader>(colHeader.rows);
						break;
						*/
				}
				if(newObject)
				{
					//Lecture des données génériques
					newObject->SetLabel(colHeader.label);
					//Lecture de l'entête spécifique de la colonne
					binFile.read(newObject->GetHeaderFirstChar(),colHeader.sizeofHeader);
					//Lecture du contenu de la colonne
					binFile.read(newObject->GetFirstChar(),colHeader.sizeofCol);
					if(isOtherVersion)
						binFile.seekp((LongLongb)binFile.tellp()-newObject->GetLength()+colHeader.sizeofCol);
					this->SetCol(idcol,newObject);
				}else{
					//Format de fichier inconnu
					binFile.seekp((LongLongb)binFile.tellp()+colHeader.sizeofCol);
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

	bool GABE::Save(const char *strFileName)
	{
		//Namespace
		using namespace std;

		//Declarations

		fstream binFile (strFileName, ios::out | ios::binary);

		//Ecriture de l'entete du fichier
		t_FileHeader fileheader;
		fileheader.cols=0;
		for(Longb idcol=0;idcol<cols;idcol++)
			if(colsContainer[idcol])
				fileheader.cols++;
		fileheader.t_FileHeader_Length=sizeof(t_FileHeader);
		fileheader.t_ColHeader_Length=sizeof(t_ColHeader);
		fileheader.readOnly=readOnly;
		fileheader.formatVersion=GABE_VERSION;

		binFile.write((char*)&fileheader ,sizeof(t_FileHeader));
		//Pour chaque colonne
		for(Longb idcol=0;idcol<cols;idcol++)
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
				strcpy(colHeader.label,currentObject->GetLabel());
				binFile.write((char*)&colHeader ,sizeof(t_ColHeader));
				//Ecriture de l'entete spécifique de la colonne
				binFile.write(currentObject->GetHeaderFirstChar(),currentObject->GetHeaderLength());
				//Ecriture du contenu de la colonne
				binFile.write(currentObject->GetFirstChar(),currentObject->GetLength());
			}
		}

		binFile.close();
		return true;
	}

} //fin namespace
