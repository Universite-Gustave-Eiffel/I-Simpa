## Copyright (C) 2017 Christian Prax
## Institut PPRIME, CNRS - Universite de Poitiers  ENSMA, UPR 3346
## Ecole Nationale Superieure d'ingenieurs de Poitiers, ENSIP
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
## Author: Christian Prax <Christian.prax@univ-poitiers.fr>
## Keywords: Room acoustics, diffusion model
## Adapted-By: Ifsttar <I-Simpa@ifsttar.fr>

## CALCULATION OF THE SOLUTION
FileName_TimeVarying=strcat(domaine,'_WInstaFields');  
FileName_Stationary=strcat(domaine,'_WStaFields');  

## LOOP ON FREQUENCY BANDS
for  N_Toct=1:NOct;

	if state==1 # Time varying state calculation
	
	  # Parameters
	  itmax=round(duration/dt); # Number of iterations
	  [MATinsta]= integVC_cst(V_VC); # Integration constant
	
	  # Solve diffusion equation
	  [wi_saved]= Euler_implicite(w{N_Toct},mat_Toct{N_Toct},MATinsta,itmax(1),dt,tol,maxint);
	  
	  # Write results in HDF5 files
	  ChpsInsta=strcat(FileName_TimeVarying,int2str(TOB(SelectedFrequency(N_Toct))));   
	  ChpsInsta2=strcat(ChpsInsta,'.hdf5');
	  xx.a=dt;
	  xx.b=wi_saved;
	  save( '-float-hdf5', ChpsInsta2, 'xx')
	  
	  else # Stationary state
	  
	  # Write results in HDF5 files
	  ChpsSta=strcat(FileName_Stationary,int2str(TOB(SelectedFrequency(N_Toct))));   
	  ChpsSta2=strcat(ChpsSta,'.hdf5');
	  xx.b=w{N_Toct}
	  save( '-float-hdf5', ChpsSta2, 'xx')
  
	end

end

