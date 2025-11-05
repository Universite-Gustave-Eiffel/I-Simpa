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

// Cette fonction calcule la masse volumique l'air
//
// Auteur : J. Picaut (LCPC)
// Date de la derni�re modification : 19 f�vrier 2007
//
// En entr�e :	- Pression (Pa)
//				- Temp�rature (Kelvin)
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
