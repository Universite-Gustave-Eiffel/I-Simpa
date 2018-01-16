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

function [wi_saved]= Euler_implicite(wi,mat,MATinsta,itmax,dt,tol,maxint)
# usage: [wi_saved]= Euler_implicite(wi,mat,MATinsta,itmax,dt,tol,maxint)
# Solve the diffusion equation using the Euleur implicit method
#
# In:
#	- wi : solution of the steady state
#	- mat :
#	- MATinsta :
#	- itmax : number of time step iterations
#	- dt : timestep (s)
#	- tol : tolerance of the method (bicgstab function)
#	- maxint : maximum number of iterations (bicgstab function)
#
# Out:
#	- wi_saved : sound energy decay after the steady state
  
  mat2=dt*mat+ MATinsta; # Euler Implicite 1
  wi_saved=zeros(max(size(wi)),itmax+1 );
   
  wi_saved(:,1)=wi;
  [L,U] = ilu(mat2,struct('type','ilutp','droptol',tol));

  for it=2:itmax+1
    RHS=MATinsta*wi; # Euler Implicite 2
    #[wi,flag]=bicgstab (mat2,RHS,tol,maxint,L,U,wi);
	wi=mat2\RHS;
    wi_saved(:,it)=wi;
  end

end