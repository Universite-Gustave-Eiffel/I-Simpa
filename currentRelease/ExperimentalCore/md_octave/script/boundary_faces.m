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

function [F , Fsort]= boundary_faces(T)
# usage: F = boundary_faces(T)
# Find the faces of the meshes on boundaries
#
# In:
#	- T : meshes (tetreahedrics) of size m
#
# Out:
#	- F : list of faces on boundaries

  allF = [ ...
  T(:,1) T(:,2) T(:,3); ...
  T(:,1) T(:,3) T(:,4); ...
  T(:,1) T(:,4) T(:,2); ...
  T(:,2) T(:,4) T(:,3)];

  # Increasing sort faces
  sortedF = sort(allF,2);
  # Verify unicity of faces
  [u,m,n] = unique(sortedF,'rows');
  # Determine counts for each unique face
  counts = accumarray(n(:), 1);
  # Extract faces that only occurred once
  sorted_exteriorF = u(counts == 1,:);
  # find in original faces so that ordering of indices is correct
  F = allF(ismember(sortedF,sorted_exteriorF,'rows'),:);
  Fsort=sort(F,2);

end