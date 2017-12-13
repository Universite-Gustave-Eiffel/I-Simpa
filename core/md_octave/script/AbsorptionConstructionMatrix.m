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

# Load the absorption coefficient of each material in the corresponding file
# iAbs abs_50 abs_63 abs_80 .. abs_16000 abs_20000
Abs_Mater=load(strcat(domaine,'_materials_absorption.txt')); # Load the absorbing material
iAbs=Abs_Mater( find(sum(Abs_Mater(:,2:end),2)));  # iAbs: absorbing material index
[iAM,~]=find(sum(Abs_Mater(:,2:end),2)) ;
iAbs=Abs_Mater(iAM,1);
abs_prop=Abs_Mater(iAM,:);
abs_coef=abs_prop(:,1+(BdOct1:BdOctend));
el2di_Dof=el2di;
for iB=1:NBLOCKS
  [SurFaceA(iB),AireFace{iB}]=Surfaces_Salle(x,y,z,size(el2di{iB},1),el2di{iB});
  el2di_Dof{iB}(:,1:3)=changem(el2di{iB}(:,1:3), N_F{iB}(:,2), N_F{iB}(:,1));
  el2di_Dof{iB}(:,4)=changem(el2di_Dof{iB}(:,4),[1:size( iAbs,1)]' , iAbs ); 
  DofAbs{iB}=unique(el2di_Dof{iB}(:,1:3));
            
  for it=1:max(size(DofAbs{iB}))
    [ligne,col] = find((el2di_Dof{iB}(:,1:3)-DofAbs{iB}(it))==0);
    EquivAbsArea (it,1:NOct)=sum( AireFace{iB}(ligne).*  abs_coef( el2di_Dof{iB}(ligne,4),1:NOct),1)/3;
    idfFree= DofAbs{iB}(it);
           
    for nm=1:NOct
      mat_Toct{nm}( idfFree,  idfFree)=mat_Toct{nm}( idfFree,  idfFree)...
           +EquivAbsArea (it,nm)*c0/4;
    end
    
    EquivAbsAreaBlock{iB}=sum(EquivAbsArea,1);
  end          

end

%=================== Surfaces d'Absorption ===========================================