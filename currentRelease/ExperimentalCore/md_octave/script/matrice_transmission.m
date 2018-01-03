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

function M= matrice_transmission(M,ks,ks2,TauxAireTr3)           
# usage: M=matrice_transmission(M,ks,ks2,TauxAireTr3)
# Calculate the element of transmission matrix
#
# In:
#	- M :
#	- ks:
#	- s2:
#	- TauxAireTr3 :
#
# Out:
#	- M :

  M(ks2(1),ks(1))=M(ks2(1),ks(1))+TauxAireTr3; 
  M(ks2(2),ks(2))=M(ks2(1),ks(2))+TauxAireTr3;
  M(ks2(3),ks(3))=M(ks2(3),ks(3))+TauxAireTr3;
  M(ks(1),ks2(1))=M(ks(1),ks2(1))+TauxAireTr3; 
  M(ks(2),ks2(2))=M(ks(2),ks2(2))+TauxAireTr3;
  M(ks(3),ks2(3))=M(ks(3),ks2(3))+TauxAireTr3;
                
end