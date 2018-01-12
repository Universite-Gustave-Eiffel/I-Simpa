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

## CALCULATION OF THE TIME-VARYING SOLUTION STARTING FROM THE STEADY STATE

#for iB=1:NBLOCKS
#  Aire_Abs_eq=  EquivAbsAreaBlock{iB};
#  TR60{iB}=0.16*VOLUME(iB)./Aire_Abs_eq; # Estimation of the reverberation time RT60 from Sabine's formula
#end

itmax=round(duration/dt); # Number of iterations
[MATinsta]= integVC_cst(V_VC); # Integration constant
NomFichier=strcat(domaine,'_WInstaFields');  

## LOOP ON FREQUENCY BANDS
for  N_Toct=1:NOct;

  # Solve diffusion equation
  [wi_saved]= Euler_implicite(w{N_Toct},mat_Toct{N_Toct},MATinsta,itmax(1),dt,tol,maxint);

  # Write results in HDF5 files
  ChpsInsta=strcat(NomFichier,int2str(TOB(SelectedFrequency(N_Toct))));   
  ChpsInsta2=strcat(ChpsInsta,'.hdf5');
  xx.a=dt;
  xx.b=wi_saved;
  save( '-float-hdf5', ChpsInsta2, 'xx')

end

