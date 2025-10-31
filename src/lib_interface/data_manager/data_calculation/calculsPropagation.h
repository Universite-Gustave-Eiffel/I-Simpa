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

/**
 * @file calculsPropagation.h
 *  Define methods for computation of the sound propagation charactericts depending on parameters. 
 * @author J. Picaut (Ifsttar)
 */
#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
/**
 * @brief Thermodynamic calculations
 */
namespace CalculsGenerauxThermodynamique
{
	using namespace std;

	namespace CGTconst
	{

		// Thermodynamic constants
		const double K_0(273.15);	// Absolute zero in Celsius
		const double Pref(101325);	// Standard atmosphere atm (Pa)
		const double Kref(293.15);	// Reference ambiant atmospheric temperature (K)
		const double FmolO(0.209);	// Mole fraction of oxygen
		const double FmolN(0.781);	// Mole fraction of nitrogen
		const double KvibO(2239.1);	// Vibrational temperature of oxygen (K)
		const double KvibN(3352.0);	// Vibrational temperature of the nitrogen (K)
		const double K01(273.16);   // Isothermal temperature at the triple point (K)

		// Setting the number imaginary complex I
		const complex<double> I(0.,1.);
	}

	class CCalculsGenerauxThermodynamique
	{
	public:
		static double c_son(double x);
		static double masse_vol(double P, double K);
		static double Coef_Att_Atmos(double F, double H, double P, double K);
	};
};
