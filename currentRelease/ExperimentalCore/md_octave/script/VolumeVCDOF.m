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

## CALCUL OF THE VOLUME OF CONTROL vn AROUND EACH DOF
% c----$---1---------2---------3---------4---------5---------6---------7-c

function [vn]= VolumeVCDOF(x,y,z,nn,nbel,Tet ,Tet_Dof)
# Volume around each point

	vn=zeros(nn,1);
	for n=1:nbel
		ks=Tet(n,:);     
	ksDof=Tet_Dof(n,:);      
	[v]=volumeTetra(x(ks),y(ks),z(ks));
	vn(ksDof(1))=vn(ksDof(1))+v/4; vn(ksDof(2))=vn(ksDof(2))+v/4;
	vn(ksDof(3))=vn(ksDof(3))+v/4; vn(ksDof(4))=vn(ksDof(4))+v/4;
 
 end
  

                
%===========================================================

