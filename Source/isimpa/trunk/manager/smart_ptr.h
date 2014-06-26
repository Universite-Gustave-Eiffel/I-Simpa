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

#include "first_header_include.hpp"

// Smart Pointer
// par Christophe [Groove] Riccio
// www.g-truc.net
// Modification ULR VALOR accès d'un pointeur tableau

#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

/**
 * 
 *  \brief Classe de gestion de pointeur
 *
 *	Cette classe permet de gérer les pointeurs
 *	L'utilité est de ne pas avoir besoin de supprimer l'objet à la fin de son utilisation.
 *  La fonction est donc de ne plus avoir de fuites mémoire.
 * 
 */

template <typename T, bool Array = false>
class smart_ptr
{
public:
	smart_ptr();
	smart_ptr(const smart_ptr& SmartPtr);
	smart_ptr(T* Pointer);
	~smart_ptr();

    smart_ptr& operator=(const smart_ptr& SmartPtr);
    smart_ptr& operator=(T* Pointer);

	T& operator[](unsigned i); //Ajout 13/07/2007 pour gérer l'utilisation du array à base de smart_ptr
	
    T& operator*();
    T* operator->();
	T* get() const; //Ajout 27/07/2009 pour accéder à l'élément.
    const T& operator*() const;
    const T* operator->() const;

	bool operator==(const smart_ptr& SmartPtr) const;
	bool operator!=(const smart_ptr& SmartPtr) const;

private:
	void _Release();

    int* _RefCounter;
    T* _Pointer;
};

#include "smart_ptr.inl"

#endif //__SMART_PTR_H__
