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

## Initialization
clc;
clear all;
close all;

## LOAD PARAMETERS
Input_parameters;

## CALCULATE ADDITIONNAL PARAMETERS FROM LOADED PARAMETERS
# Frequency bands
Frequency=TOB(SelectedFrequency);
# Selected frequency bands
NOct=size(Frequency,2);
NNOct=1:NOct;
# Atmospheric absorption and sound speed values
[m,c0]=Coef_Att_Atmos(Frequency,Humidity,Pressure,273.15+Temperature);
if atmos_absorption_calculation==0
	# No amtospheric absorption coefficient
	mc=zeros(1,NOct);
	else
	# Use atmospheric absorption coefficient
	mc=m*c0;
end

## READING MESHING AND NODES
display('Read meshes and nodes')
domaine='scene'; # Meshing file name
el = load(strcat(domaine,'_elements.txt'));
nbre_salles=numel(unique(el(:,5))); # Number of volumes (i.e. rooms)
NBLOCKS=nbre_salles; # Number of blocks of the meshing
idBloc=unique(el(:,5)); # Block identification
disp('Number of volumes: '), disp(nbre_salles)

## TABLE OF DOF SIGNATURES
# DOF_Sig <--Table des signatures pour les DOF
# Example NBLOCKS= 3, include 1 0 1 for a node associate to the volume 1 et volume 3
disp('Create table of signatures')
DOF_Sig=1:NBLOCKS;
DOF_Sig=DOF_Sig*0;
ndf=[];
for i=1:NBLOCKS
  Tet{i}=el(el(:,5)==idBloc(i),:);% Meshs for the volume i
  pts{i}=unique(Tet{i}(:,1:4));
  NodeDOFo=zeros(size(pts{i},1),size( DOF_Sig,2)+1);
  NodeDOFo(:,1)=pts{i};NodeDOFo(:,i+1)=1;
  Node_DOF_Sig{i}=NodeDOFo;
  nnB(i)=size( pts{i}, 1);
  ndf=[ndf ;Node_DOF_Sig{i}];
end

# NodeDOFs{i}: ID Node of the Volume i, the signature of the DOL
NDOF=size(ndf,1);
[~,I] = sort(ndf(:,1));
sortedndf=ndf(I,:);
clear I;
XYZ= load(strcat(domaine,'_nodes.txt'));
nn=size(XYZ,1); # Number of nodes (that is different of the nimber of DOF)
NDOF=size(ndf,1);
disp('Number of nodes: '), disp(NDOF)
NFS=zeros(nn,NBLOCKS+1);% NFS= NODE FREEDOM SIGNATURE
for ij=1:nn
  [irow,IL,~]=find(sortedndf(:,1)==ij);
  cs=sum (sortedndf(irow,2:end),1);
  NFS(ij,1:NBLOCKS+1)=[ ij  cs];% NFS= NODE FREEDOM SIGNATURE: example 117 1 0 1
end

## TABLE OF CONNECTION BETWEEN THE NODE INDEX and the DOF index
for ic=1:NBLOCKS
  [lig, col] = find (sortedndf(:,ic+1));
  N_F{ic}=[sortedndf(lig,1) lig]; # Example N_F{1} corresponds to the DOF node in Volume 1
end

##  Tet_Dof{i} d�finis sur les degr�s de libert�
for i=1:NBLOCKS
  Tet_Dof{i}= changem(Tet{i}(:,1:4),N_F{i}(:,2), N_F{i}(:,1));
  Tet_Dof{i}(:,5)=Tet{i}(:,5);
end

## NODES AND DOF COORDINATES
x=XYZ(:,1);y=XYZ(:,2);z=XYZ(:,3); # Coordinates of each node
xdf=x(sortedndf(:,1));ydf=y(sortedndf(:,1));zdf=z(sortedndf(:,1)); # Coordinates of each DOF
XYZ2=[xdf, ydf, zdf];

## READING MESHES ON BOUNDARIES
disp('Identifying boundaries')
el2Dtypd= load(strcat(domaine,'_faces.txt'));
nel2D0=size(el2Dtypd,1);
[FT,r]=sort(el2Dtypd(:,1:3),2);
[FTu,l]=unique(FT,'rows' );
el2Dtyp=el2Dtypd(l,:);
nel2Dt=size(el2Dtyp,1);
duplicate_l = setdiff(1:nel2D0, l);

## IDENTIFYING MESHES WITH TRANSMISSION
el2D_Trsm=el2Dtypd(duplicate_l,:);

## INFORMATION ON MESHS ON BOUNDARIES
el2D=el2Dtyp(:,1:3); # Meshes onboundaries
n_materiau=el2Dtyp(:,4); # Identification of boundary material
nel2D=size(el2D,1); # Number of meshes on boundaries

## SEARCH BONDARY FACES OF EACH BLOCK
[sortedel2D,IS]= sort(el2D,2); # Sort on each line
nel2D=zeros(NBLOCKS,1);
Surf=zeros(NBLOCKS,1);
VOLUME=zeros(NBLOCKS,1);
lambda=zeros(NBLOCKS,1);
for i=1:NBLOCKS
  [~, sortedel2di] = boundary_faces(Tet{i}); # Meshes of Volume i
  el2di{i}= el2Dtyp(ismember(sortedel2D,sortedel2di,'rows'),:);
end

## Rooms surface and volume calculation
V_VC=zeros(sum(nnB),1);
for i=1:NBLOCKS
  nel2D(i)=max(size( el2di{i} )); # Number of meshes on boundaries
  [Surf(i),AireTr{i}]= Surfaces_Salle(x,y,z,nel2D(i),el2di{i}); # Boundary surfaces
  [VOLUME(i)]= VolumeSalle(x,y,z,size( Tet{i},1), Tet{i}); # Size of volume i (m3)
  [V_VCi{i}]=VolumeVCDOF(x,y,z,sum(nnB),size(Tet{i},1),Tet{i}, Tet_Dof{i}); # Table of the volumes of control
  V_VC=V_VCi{i}+V_VC;
end

## PARAMETERS OF THE ROOM ACOUSTICS DIFFUSION MODEL
lambda=4*VOLUME./Surf; # Mean Free path (m)
CoeffDiff=lambda*c0/3; # Diffusion coefficient TODO: Fix the diffusion coefficient value for mixed surface reflection and atmospheric absorption

## GLOBAL MATRIX ASSEMBLAGE
disp('Global matrix assemblage')
RHS=zeros(NDOF,1); # Memory allocation
# Tetra Assemblage of Global mat for diffusion operator
[mat]=laplacienblocks2(xdf,ydf,zdf,Tet_Dof,NBLOCKS,NDOF,CoeffDiff);
#mat=-mat+mc*diag(V_VC);

## DIFFUSION EQUATION LEFT MEMBER
for N_Toct=1:NOct;
  mat_Toct{N_Toct}=-mat+mc(N_Toct)*diag(V_VC);
end

## ACOUSTIC TRANSMISSION
if NBLOCKS>1
  TransmissionConstructionMatrix;
end

## ACOUSTIC ABSORPTION
AbsorptionConstructionMatrix;

## SOUND SOURCES
# Load sound sources information, for EDP second member
disp('Load sound sources information')
Srce_sonore_I=load(strcat(domaine,'_sources.txt'));
[Ns,Ls]=size(Srce_sonore_I);

for s=1:Ns

  # Source positions
  xs=Srce_sonore_I(s,1);
  ys=Srce_sonore_I(s,2);
  zs=Srce_sonore_I(s,3);

  dist2S=(xdf-xs).^2+(ydf-ys).^2+(zdf-zs).^2; # Distance from source to DOF
  rayonS2=0.15^2;
  VolSource(s)=-10;

  while VolSource(s)<=0
	ind=1:NDOF;
	rayonS2=rayonS2*1.25; # Radius incrementation. TODO: should be justified?
	indss{s}=ind(dist2S<rayonS2); # Search all DOF inside the source radius
	VolSource(s)=sum(V_VC(indss{s})); # Volume of the 'real' source (sum of the volum of each dol(ind))
  end

  Srce_sonore(s,3+NNOct)=1e-12*10.^(Srce_sonore_I(s,3+SelectedFrequency)/10); # Octave band sound power
  Volumic_Power_Srce=Srce_sonore(s,3+NNOct)/VolSource(s); # Volumic normalisation

  # Display sound source information
  disp('Sound source number: '), disp (s)
  disp('Volume source: '), disp (VolSource(s))
  disp('Location: '), disp(indss{s})
  #disp('Volumic power: '), disp(Volumic_Power_Srce)

end

RHS=zeros(NDOF,1); # Memory allocation
for  N_Toct=1:NOct;
  for s=1:Ns
    RHS(indss{s})=Volumic_Power_Srce(N_Toct)*V_VC(indss{s}); # Octave band sound power
  end
  w{N_Toct}=mat_Toct{N_Toct}\RHS;
end
#affichepatchBlocks

## TIME VARYING STATE CALCULATION
disp('Time varying state calculation')
Calcul_insta

## SAVE COORDINATES AND MESHES
disp('Save data into HDF5 files')
NomFichier=strcat(domaine,'_XYZ_TETRA');
CoordDOF=strcat(NomFichier,'.hdf5');
topo.XYZ=XYZ2;
topo.TetDOF=Tet_Dof;
save( '-hdf5', CoordDOF, 'topo')
