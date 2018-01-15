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

## CALCULATE FACES TRANSMISSION el2diTrsm{ib}
# i_nT= Index table of all node with transmission

# Load the transmission coefficient of each material in the corresponding file
# iAbs TL_50 TL_63 TL_80 .. TL_16000 TL_20000
i_nT=load(strcat(domaine,'_shared_vertices.txt')); # Load index
i_nT=sort(i_nT);
Wall_TL=load(strcat(domaine,'_materials_transmission.txt')); # Load material
iTrsm=Wall_TL( find(sum(Wall_TL(:,2:end),2)),1);%iTrsm: index of Material with transmission
TL=Wall_TL( find(sum(Wall_TL(:,2:end),2)),1+SelectedFrequency); % Transmission Loss idMat &values
TAU(:,1:NOct)=10.^(-TL(:,1:NOct)/10);

for ib=1:NBLOCKS
	el2diTrsm{ib}=el2di{ib}(ismember( el2di{ib}(:,end),iTrsm),:);
end

it=0;
for i=1:NBLOCKS-1
	el2Di=el2diTrsm{i};
	nbParoiTransm=0;
	for j=i+1:NBLOCKS    
		el2Dj=el2diTrsm{j};
		Facescommunesij=el2Di(ismember(el2Di,el2Dj,'rows'),:);
		cond=isempty(Facescommunesij);
		if not(cond)
			it=it+1;
			FacesTij{it}=Facescommunesij;
			FacesTij{it}(:,4)= changem(FacesTij{it}(:,4),[1:size(iTrsm,1)]' ,iTrsm );     
			FacesTi_Dof{it}= changem(FacesTij{it}(:,1:3), N_F{i}(:,2), N_F{i}(:,1));
			FacesTi_Dof{it}(:,4)= FacesTij{it}(:,4);
			FacesTj_Dof{it}= changem(FacesTij{it}(:,1:3), N_F{j}(:,2), N_F{j}(:,1));
			FacesTj_Dof{it}(:,4)= FacesTij{it}(:,4);  
		end   
	end
end
NbTransmWall=it

## EXCHANGE SURFACES (sum of the mesh face surfaces associated with each point, divided by 3)
for i=1:NbTransmWall
	[SurFaceT(i),AireFace{i}]= Surfaces_Salle(x,y,z,size(FacesTij{i},1),FacesTij{i});
	ptsTrsm{i}=unique(FacesTij{i}(:,1:3));
	Dofi{i}=unique(FacesTi_Dof{i}(:,1:3));
	Dofj{i}=unique(FacesTj_Dof{i}(:,1:3));
	for it=1:max(size(ptsTrsm{i}))
		[ligne,col] = find((FacesTij{i}(:,1:3)-ptsTrsm{i}(it))==0);
		Surf_ech(it)=sum(AireFace{i}(ligne))/3;  
        PondEchSurf (it,1:NOct) = sum( AireFace{i}(ligne).*  TAU(FacesTij{i}(ligne,4),1:end),1  )/3;
		## Contribution of the transmission
		for nm=1:NOct      
			if TAU(FacesTij{i}(ligne,4),nm)==1        
				mat_Toct{nm}(Dofj{i}(it), :)=-mat(Dofj{i}(it), :)-mat(Dofi{i}(it), :); # No absorption
				mat_Toct{nm}(Dofi{i}(it), :)=-mat(Dofj{i}(it), :)-mat(Dofi{i}(it), :); # No absorption
			else  
				mat_Toct{nm}(Dofi{i}(it), Dofj{i}(it))=mat_Toct{nm}(Dofi{i}(it), Dofj{i}(it))...
					-PondEchSurf (it,nm) *c0/4;
				mat_Toct{nm}(Dofj{i}(it), Dofi{i}(it))=mat_Toct{nm}(Dofj{i}(it), Dofi{i}(it))...
					-PondEchSurf (it,nm) *c0/4;
			end
	    end
      
	end          
	clear Surf_ech
	clear PondEchSurf
end