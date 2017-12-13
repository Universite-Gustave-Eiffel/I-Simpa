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

function [vtotal]= VolumeSalle(x,y,z,nbel,kne)
# usage: [vtotal]= VolumeSalle(x,y,z,nbel,kne)
# Calculate the volume of a room
# In:
#	- x,y,z : Nodes coordinates
#	- nd : Number of meshes
#	- kne : TODO: this parameter seems not needed
#
# Out:
#	- vtotal : Volume of a room

vtotal=0;
# The volume is equal to the sum of the volume of all the meshes
for n=1:nbel
  ks=kne(n,:);            
  [v]=volumeTetra(x(ks),y(ks),z(ks));
  vtotal=vtotal+v;
end
