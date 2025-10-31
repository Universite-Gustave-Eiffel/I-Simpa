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

// Cette fonction calcule le coefficient d'att�nuation
// atmosph�rique du son dans l'air
// Norme ISO 9613-1:1993(F)
//
// Auteur : J. Picaut (LCPC)
// Date de la derni�re modification : 23 juillet 2007
//
// En entr�e :
// - fr�quence acoustique (Hz)
// - humidit� relative (en %)
// - pression atmosph�rique (en Pa)
// - temp�rature en Kelvin (en K)
// En sortie : Absorption totale en dB/m

#include "calculsPropagation.h"


namespace CalculsGenerauxThermodynamique
{
	using namespace CGTconst;
	double CCalculsGenerauxThermodynamique::Coef_Att_Atmos(double F, double H, double P, double K)
	{
		// C�l�rit� du son
		double cson = CCalculsGenerauxThermodynamique::c_son(K);

		// Calcul de la fraction molaire de vapeur d'eau
		double C = -6.8346 * pow( K01 / K , 1.261) + 4.6151;
		double Ps = Pref * pow( 10. ,C);
		double hmol = H * Ps / Pref;

		// Absorption classique et rotationnelle
		double Acr = (Pref/P) * (1.60E-10) * sqrt( K / Kref) * pow(F,2);

		// Absorption vibratoire de l'oxyg�ne
		double Fr = (P/Pref) * (24. + 4.04E4 * hmol * (0.02 + hmol) / (0.391 + hmol) );
		double Am = 1.559 * FmolO * exp(-KvibO/K) * pow(KvibO/K,2);
		double AvibO = Am * (F/cson) * 2. *(F/Fr) / (1 + pow(F/Fr,2));

		// Absorption vibratoire de l'azote
		Fr = (P/Pref) * sqrt(Kref/K) * (9. + 280. * hmol * exp( -4.170 * (pow(K/Kref,-1./3.)-1)));
		Am = 1.559 * FmolN * exp(-KvibN/K) * pow(KvibN/K,2);
		double AvibN = Am * (F/cson) * 2. * (F/Fr) / (1+pow(F/Fr,2));

		// Absorption totale en dB/m
		double alpha = Acr + AvibO + AvibN;

		return alpha;
	}

};
