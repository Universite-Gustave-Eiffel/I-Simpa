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

function [M_int_cst]= integVC_cst(V_VC)
# usage: [M_int_cst]= integVC_cst(V_VC)
# Built of the matrix M_int_Lin
# Integration on the sub-tetreahedrics mesh of voulme v/4
#
# In:
#	- V_VC : 
#
# Out:
#	- M_int_cst : 


	ia = [];
	ja = [];
	s = [];
	nDoFr=size(V_VC,1);
	M_int_cst = sparse ( ia, ja, s, nDoFr,nDoFr,1 );
	M_int_cst=diag(V_VC);

end

