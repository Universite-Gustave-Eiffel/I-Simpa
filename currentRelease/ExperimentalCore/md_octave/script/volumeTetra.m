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

function [v]=volumeTetra(xn,yn,zn)
# usage: [v]=volumeTetra(xn,yn,zn)
# Calculate the volume of a given mesh
#
# In:
#	- xn,yn,zn : Coordinates of the mesh
#
# Out:
#	- v : Volume of a given mesh

  XA=[xn(1); yn(1); zn(1)];
  XB=[xn(2); yn(2); zn(2)];
  XC=[xn(3); yn(3); zn(3)];
  XD=[xn(4); yn(4); zn(4)];
  v=dot(XD-XA,cross((XB-XA),(XC-XA)))/6;

return
