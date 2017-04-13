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

#ifndef __GABE__
#define __GABE__
#include <memory.h>
#include <assert.h>
#include <fstream> 
/*! @file gabe.h
 *  @brief Generic Array Binary Exchange
 *
 *  Fichier d'échange générique
 */

/**
 *  @brief Generic Array Binary Exchange
 *
 *  Fichier d'échange générique
 */
namespace formatGABE
{
	/**
	 * @brief Type de données connu par le format
	 */
	enum GABE_OBJECTTYPE
	{
		GABE_OBJECTTYPE_USER_DEFINED=49,	/*<! Type de donnée définit par l'utilisateur [0-49] */
		GABE_OBJECTTYPE_FLOAT,				/*<! Type de données flottant */
		GABE_OBJECTTYPE_INT,				/*<! Type de données entier */
		GABE_OBJECTTYPE_SHORTSTRING			/*<! Type de données chaine de caractère de 50 octets max */
	};

	const unsigned int STRING_LABEL_LENGTH=255;
	const unsigned int STRING_NUMBER_BUFFER=20;

	typedef unsigned short UShortb;
	typedef int Intb;
	typedef float Floatb;
	typedef unsigned long ULongb ;
	typedef long Longb ;
	typedef long long LongLongb ;
	typedef bool Boolb ;
/**
 * @brief Classe virtuelle permettant de définir plusieurs types d'objets avec un type commun.
 */

class GABE_Object
{
protected:
	char label[STRING_LABEL_LENGTH];
private:
	GABE_OBJECTTYPE objectType;
public:
	GABE_Object(GABE_OBJECTTYPE _objectType);
	GABE_Object(const GABE_Object& cpyFrom)
		:objectType(cpyFrom.GetObjectType())
	{
		SetLabel(cpyFrom.GetLabel());
	}
	virtual ~GABE_Object(){};
	virtual const char * GetLabel() const{ return label;}
	/**
	 * Permet d'obtenir l'équivalent en chaine de caractère d'une cellule quelque soit le type de l'objet
	 */
	virtual const char * GetStringEquiv(int numRow);
	virtual void SetLabel(const char * _label);
	/**
	 * Retourne la taille en octets du tableau
	 */
	virtual LongLongb GetLength() const{return 0;}
	/**
	 * Retourne le premier octet du tableau de données
	 */
	virtual char * GetFirstChar(){ return 0; }
	/**
	 * Retourne la taille en octets de la structure d'entête
	 */
	virtual LongLongb GetHeaderLength(){return 0;}
	/**
	 * Retourne le premier octet de la structure d'entête
	 */
	virtual char * GetHeaderFirstChar(){ return 0; }

	GABE_OBJECTTYPE GetObjectType() const{ return objectType;}
	/**
	 * Retourne la taille du tableau
	 */
	virtual ULongb GetSize() const{ return 0; }

	/**
	 * Retourne l'adresse d'une copie de l'objet
	 */
	virtual GABE_Object* GetCopy() const =0;

	/**
	 * Load the content of this column from 
	 */
	virtual bool ReadFile(std::fstream& fs) { return false; }

	virtual bool WriteFile(std::fstream& fs) { return false; }
};
/**
 * Structure de données par défaut
 */
struct t_NoStruct
{

};
/**
 *	\brief Classe générique contenant une colonne de donnée (même type)
 */
template<class Td,class Ts=t_NoStruct>
class GABE_Data: public GABE_Object
{
public:
	GABE_Data(Longb _size,GABE_OBJECTTYPE objectType)
		:size(_size),
		GABE_Object(objectType)
	{
		tabData = new Td[size];
	}
	GABE_Data(const GABE_Data& cpyFrom)
		:size(cpyFrom.GetSize()),
		headerData(cpyFrom.headerData),
		GABE_Object(cpyFrom)
	{
		tabData = new Td[cpyFrom.GetSize()];
		memcpy(tabData,cpyFrom.GetFirstData(),cpyFrom.GetLength());
	}
	virtual ~GABE_Data(){delete[] tabData; }

	virtual void Set(Longb index,Td data){ assert(index>=0 && index<size);  tabData[index]=data; }
	Td* GetFirstData () const{ return tabData; }
	/**
	 * Surcharge de l'opérateur this[]
	 */
	Td* operator[](ULongb _i) { return &tabData[_i]; }
	/**
	 * Retourne la valeur pour la ligne i
	 */
	const Td& GetValue(ULongb _i)
	{
		return tabData[_i];
	}
	virtual GABE_Object* GetCopy() const=0;
	virtual const char * GetStringEquiv(int numRow){ return (const char *)&tabData[numRow]; }
	virtual bool ReadFile(std::fstream& fs);
	virtual const char * GetLabel() const{ return GABE_Object::GetLabel();}
	virtual void SetLabel(const char * _label) { GABE_Object::SetLabel(_label); }
	/**
	 * Retourne le pointeur vers le premier élément
	 */
	char * GetFirstChar();
	/**
	 * Retourne la taille en octets du tableau
	 */
	virtual LongLongb GetLength() const{ return sizeof(Td)*size; }
	virtual LongLongb GetHeaderLength(){ return sizeof(Ts); }
	virtual char * GetHeaderFirstChar(){ return (char*)&headerData; }
	/**
	 * Retourne le nombre d'éléments du tableau
	 */
	ULongb GetSize() const{ return size; }
	/**
	 * Copie rapide des données vers cet élément. Ces données doivent être du même type et de même longueur que GetLength().
	 * @param src Pointeur vers le premier élément du tableau source.
	 */
	virtual void FastCopy(const Td * src);
	Ts headerData;
protected:
	Longb size;
	Td* tabData;
};

/**
 *	\brief Type de données de l'entete de  GABE_Data_Float
 */
struct t_HeaderFloat
{
	Intb numOfDigits; /*<! En sortie de  GABE_Data_Float::GetStringEquiv . Nombre de chiffre visibles.*/
	t_HeaderFloat(){numOfDigits=12;}
};
/**
 *	\brief Classe contenant une colonne de donnée de type nombre à decimal.
 */
class GABE_Data_Float: public GABE_Data<Floatb,t_HeaderFloat>
{
	private:
		char buffer[STRING_NUMBER_BUFFER];
	public:
		GABE_Data_Float(Longb _size);
		GABE_Data_Float(const GABE_Data_Float& cpyFrom)
			:GABE_Data<Floatb,t_HeaderFloat>(cpyFrom)
		{
		}
		virtual const char * GetStringEquiv(int numRow);
		virtual bool ReadFile(std::fstream& fs);
		virtual GABE_Object* GetCopy() const;
};
/**
 *	\brief Type de données de la colonne GABE_Data_ShortString
 */
struct t_StringShort
{
	char strData[50];
};
/**
 *	\brief Classe contenant une colonne de donnée de type chaîne de caractère
 */
class GABE_Data_ShortString: public GABE_Data<t_StringShort>
{
	private:
		UShortb _stringLength;
	public:
		GABE_Data_ShortString(Longb _size);
		GABE_Data_ShortString(const GABE_Data_ShortString& cpyFrom):GABE_Data<t_StringShort>(cpyFrom){}
		void SetString(int numRow,const char * strToCopy);
		virtual GABE_Object* GetCopy() const;
		virtual bool ReadFile(std::fstream& fs);
		virtual const char * GetStringEquiv(int numRow){ return this->GetValue(numRow).strData; }
};
/**
 *	\brief Classe contenant une colonne de donnée de type nombre entier
 */
class GABE_Data_Integer: public GABE_Data<Intb>
{
	private:
		char buffer[STRING_NUMBER_BUFFER];
	public:
		GABE_Data_Integer(Longb _size);
		GABE_Data_Integer(const GABE_Data_Integer& cpyFrom):GABE_Data<Intb>(cpyFrom){}
		virtual const char * GetStringEquiv(int numRow);
		virtual bool ReadFile(std::fstream& fs);
		virtual GABE_Object* GetCopy() const;
};


/**
 	\brief Classe contenant des données inconnu par GABE (définit par l'utilisateur)
template<Intb sizeofEl>
struct el_Unknown
{
	char data[sizeofEl];
};
template<Intb sizeofTabEl,Intb sizeofHeaderEl>
class GABE_Data_Unknown: public GABE_Data<el_Unknown<sizeofTabEl>,el_Unknown<sizeofHeaderEl>>
{
	public:
		GABE_Data_Unknown(Longb _size);
		virtual const char * GetStringEquiv(int numRow){return &tabData[numRow].data};
};
 */

/**
 *	\brief Classe de sauvegarde et de chargement d'une serie de listes de format différend
 */
class GABE
{
public:
	GABE();
	/** Constructeur créateur
	 * @param _size Nombre de lignes
	 */
	GABE(Longb _cols);
	/** Constructeur lecture
	 */
	GABE( const char *strFileName );
	/** Constructeur copie
	 */
	GABE(const GABE& cpyFrom);
	/** Destructeur
	 */
	~GABE();
	/*
	 *	Redimensionne le tableau
	 */
	void Redim(const Longb& colsCount);
	/**
	 * Ajout d'une colonne. Cette surcharge prend possession de l'objet passé en paramètre et le détruira une fois l'objet Gabe détruit.
	 * @param numCol Numero de la colonne [0,GetCols()-1]
	 * @param newCol Adresse de l'objet contenant les données ( cette donnée sera déchargé par GABE lors de sa destruction.
	 */
	void SetCol(Longb numCol,GABE_Object* newCol);
	/**
	 * Ajout d'une colonne. Cette surcharge effectue une copie de l'objet passé en paramètre.
	 * @param numCol Numero de la colonne [0,GetCols()-1]
	 * @param newCol Adresse de l'objet contenant les données ( cette donnée sera déchargé par GABE lors de sa destruction.
	 */
	void SetCol(Longb numCol,const GABE_Object& newCol) const;

	/**
	 * Récuperation d'une colonne
	 * @param numCol Numero de la colonne [0,GetCols()-1]
	 * @return Adresse de l'objet contenant les données ( cette donnée sera déchargé par GABE lors de sa destruction.). Nul si aucun, ou n'est pas de ce type.
	 */
	GABE_Object* GetCol(Longb numCol);
	const GABE_Object* GetConstCol(Longb numCol) const;
	/**
	 * Récuperation d'une colonne
	 * @param[in] numCol Numero de la colonne [0,GetCols()-1]
	 * @param[out] objData Adresse de l'objet contenant les données ( cette donnée sera déchargé par GABE lors de sa destruction.). Nul si aucun, ou n'est pas de ce type.
	 * @return Faux si la donnée ne peut être interprété par le format en paramètre
	 */
	bool GetCol(Longb numCol,GABE_Data_Float** objData);
	bool GetCol(Longb numCol,GABE_Data_ShortString** objData);
	bool GetCol(Longb numCol,GABE_Data_Integer** objData);

	template<class Td,class Ts>
	bool GetCol(Longb numCol,GABE_Data<Td,Ts>** objData);
	GABE_Object* operator[](int numCol);
	/**
	 * Indique le nombre de colonne contenu dans GABE
	 */
	Longb GetCols() const;
	/**
	 * Chargement du tableau
	 * @param strFileName Chemin du fichier
	 */
    bool Load(const std::string& strFileName);
	/**
	 * Sauvegarde du tableau
	 * @param strFileName Chemin du fichier
	 */
	bool Save(const std::string& strFileName);

	/**
	 * Le fichier ne sera pas éditable par l'utilisateur
	 */
	void LockData();
	/**
	 * Le fichier sera éditable par l'utilisateur
	 */
	void UnlockData();
	/**
	 * Les données sont elles en lecture seule pour l'utilisateur.
	 */
	bool IsReadOnly() const;
protected:
	void Destroy();
	void InitCols();
	GABE_Object** colsContainer; //Tableau 1 dimension, pointeur vers un objet de colonne
	Longb cols;
	bool readOnly;
};


} //namespace
#endif
/// @endcond
