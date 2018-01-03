## Copyright (C) 2017 Ifsttar
## The French institute of science and technology for transport,
## development and networks
##
## This file is part of MD (Room Acoustics Diffusion Model).
##
## MD is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public
## License as published by the Free Software Foundation;
## either version 3 of the License, or (at your option) any
## later version.
##
## MD is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public
## License along with Octave; see the file COPYING.  If not,
## see <http://www.gnu.org/licenses/>.
##
## Author: Ifsttar <I-Simpa@ifsttar.fr>
## Keywords: Room acoustics, diffusion model

function [m,cson]=Coef_Att_Atmos(F,H,P,K)
# usage: m=Coef_Att_Atmos(F,H,P,K)
# Caculate the coefficient of atmospheric absorption according to ISO 9613-1
#
# In:
#	- F : Frequency (Hz)
#	- H : Relative Humidity (%)
#	- P : Atmospheric pressure (Pa)
#	- K : Air temperature (K)
#
# Out:
#	- m : Coefficient of atmospheric absorption (Np/m)
#	- cson : Sound speed (m/s)

# Atmospheric information
Pref=101325;	# Reference pressure (Pa)
Kref=293.15;	# Reference temperature (K)
FmolO=0.209;	# 
FmolN=0.781;	# 
KvibO=2239.1;	# 
KvibN=3352.0;	# 
K01=273.16;     # 

# Sound speed
cson=343.2*sqrt(K/Kref);

# Molar concentration of water vapor
C=-6.8346*((K01./K)^1.261)+4.6151;
Ps=Pref*(10^C);
hmol=H*(Ps/Pref);

# Rotational absorption
Acr=(Pref/P)*(1.60e-10)*sqrt(K/Kref)*F.^2;

# Relaxation frequency of molecular oxygen
Fr=(P/Pref)*(24+(4.04e4)*hmol*(0.02+hmol)/(0.391+hmol));
Am=1.559*FmolO*exp(-KvibO/K)*(KvibO/K)^2;
AvibO=Am*(F/cson)*2.*(F/Fr)./(1+(F/Fr).^2);   
    
# Relaxation frequency of molecular nitrogen
Fr=(P/Pref)*sqrt(Kref/K)*(9+280*hmol*exp(-4.170*((K/Kref)^(-1/3)-1)));
Am=1.559*FmolN*exp(-KvibN/K)*(KvibN/K)^2;
AvibN=Am*(F/cson)*2.*(F/Fr)./(1+(F/Fr).^2); 

# Total absorption in Np/m
m=(Acr+AvibO+AvibN)*log(10)/10;
