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

// Cette fonction calcule la masse volumique l'air
//
// Auteur : J. Picaut (LCPC)
// Date de la dernière modification : 19 février 2007
//
// En entrée :	- Pression (Pa)
//				- Température (Kelvin)
// En sortie :	- Masse volumique

#include "calculsPropagation.h"


using namespace std;

namespace CalculsGenerauxThermodynamique
{
	using namespace CGTconst;
	const double M(28.9644);			// Masse volumique de l'air (en kg/kmol)
	const double R(8314.32);			// Constante des gaz parfaits (en J/K.Kmol)

	// Calcul de la masse volumique
	double CCalculsGenerauxThermodynamique::masse_vol(double P, double K)
	{
		double rho = P * M / (R * K);
		return rho;
	}
}
