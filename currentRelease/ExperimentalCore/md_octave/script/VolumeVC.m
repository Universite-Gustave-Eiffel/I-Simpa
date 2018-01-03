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

## CALCUL OF THE VOLUME OF CONTROL vn

function [vn]= VolumeVC(x,y,z,nn,nbel,kne)
# usage: [vn]= VolumeVC(x,y,z,nn,nbel,kne)
# Calculate the volume of a mesh
#
# In:
#	- x,y,z : Nodes coordinates
#	- nd : Number of meshes
#	- nbel : 
#	- kne : TODO: this parameter seems not needed
#
# Out:
#	- vn : Volume of a mesh

# Volume around each point

	vn=zeros(nn,1);
	for n=1:nbel
		ks=kne(n,:);            
	[v]=volumeTetra(x(ks),y(ks),z(ks));
	vn(ks(1))=vn(ks(1))+v/4; vn(ks(2))=vn(ks(2))+v/4;
	vn(ks(3))=vn(ks(3))+v/4; vn(ks(4))=vn(ks(4))+v/4;
	 
end
 