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

## CALCULATION OF THE WHOLE SURFACE BOUNDARIES
% c----$---1---------2---------3---------4---------5---------6---------7-c

function [SurfSalle,AireTr]= Surfaces_Salle(x,y,z,nd,kne)
# usage: [SurfSalle,AireTr]= Surfaces_Salle(x,y,z,nd,kne)
# Calculate the whole surface of boundaries
#
# In:
#	- x,y,z : Nodes coordinates
#	- nd : Number of meshes
#	- kne : TODO: this parameter seems not needed
#
# Out:
#	- SurfSalle : Surface of the boundaries
#	- AireTr : Surface of a mesh element on boundaries (matrix of size nd)

# Initialisation
SurfSalle=0;
AireTr=zeros(nd,1);

for n=1:nd
              
  ks=kne(n,:);
  XA=[x(ks(1)); y(ks(1)); z(ks(1))];
  XB=[x(ks(2)); y(ks(2)); z(ks(2))];
  XC=[x(ks(3)); y(ks(3)); z(ks(3))];

  % Calculation of the surface of a mesh face
  AireTr(n)= 1/2*norm(cross((XB-XA),(XC-XA)));
  SurfSalle=AireTr(n)+SurfSalle;

  end
            
end

