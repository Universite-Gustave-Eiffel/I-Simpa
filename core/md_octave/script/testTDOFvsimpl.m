%***********************************************************
%    Christian.prax@univ-poitiers.fr
%Institut PPRIME, CNRS - Universite de Poitiers  ENSMA, UPR 3346
%Ecole Nationale Superieure d'ingenieurs de Poitiers, ENSIP
%   -  NOVEMBRE 2017 -
%************************************************************
clc; clear all; close all% clf;
% LECTURE MAILLAGE/ CONNECTIVITES et coordonnees des noeuds
 domaine='scene';% NOM DU FICHIER DE Maillage 
el = load(strcat(domaine,'_elements.txt'));
nbre_salles=numel(unique(el(:,5)))
NBLOCKS=nbre_salles;% Nombre de blocs du maillage
idBloc=unique(el(:,5));% Identifiant des Blocs
%  DOF_Sig <--Table des signatures pour les DOF
% exemple NBLOCKS= 3, contient 1 0 1 pour un noeud associé à salle 1 et salle 3
 DOF_Sig=1:NBLOCKS; DOF_Sig=DOF_Sig*0;
ndf=[];
 for i=1:NBLOCKS
         Tet{i}=el(el(:,5)==idBloc(i),:);% Les TETRA par Salle i
        pts{i}=unique(Tet{i}(:,1:4));
         NodeDOFo=zeros(size(pts{i},1),size( DOF_Sig,2)+1);
         NodeDOFo(:,1)=pts{i};NodeDOFo(:,i+1)=1;
         Node_DOF_Sig{i}=NodeDOFo;
         nnB(i)=size( pts{i}, 1);
 ndf=[ndf ;Node_DOF_Sig{i}];
end

 % - NodeDOFs{i}: ID noeud du Volume i, la signature des degres de liberté
NDOF=size(ndf,1);
[~,I] = sort(ndf(:,1)); sortedndf=ndf(I,:); clear I; 
%%===================================================================================
XYZ= load(strcat(domaine,'_nodes.txt'));
%========================================================
nn=size(XYZ,1);% nombre de noeuds (différent du nombre de Degre Of Freedom)
NDOF=size(ndf,1)
NFS=zeros(nn,NBLOCKS+1);% NFS= NODE FREEDOM SIGNATURE
 for ij=1:nn
  [irow,IL,~]=find(sortedndf(:,1)==ij);
      cs=sum (sortedndf(irow,2:end),1);
      NFS(ij,1:NBLOCKS+1)=[ ij  cs];% NFS= NODE FREEDOM SIGNATURE: exple 117 1 0 1
  end
 %=============================================================================================
% N_F:Tableau de correspondance indiceNoeud vs indice DOF
 %=============================================================================================
for ic=1:NBLOCKS
       [lig, col] = find (sortedndf(:,ic+1)); 
       N_F{ic}=[sortedndf(lig,1) lig];% exple N_F{1} correspondance Noeud DOF salle 1
 end
 %=============================================================================================
 %  Tet_Dof{i} définis sur les degrés de liberté
  for i=1:NBLOCKS
  Tet_Dof{i}= changem(Tet{i}(:,1:4),N_F{i}(:,2), N_F{i}(:,1));
  Tet_Dof{i}(:,5)=Tet{i}(:,5);
  end
%===================================================================================

x=XYZ(:,1);y=XYZ(:,2);z=XYZ(:,3);% Coordonnées de chaque noeud
xdf=x(sortedndf(:,1));ydf=y(sortedndf(:,1));zdf=z(sortedndf(:,1));% Coordonnées de chaque DOF
%========================================================
 %nbel=size(Tet,1);% nombre d'elements Tetraedres
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%             Lecture des FACES FRONTIERES des Tetraedres
el2Dtypd= load(strcat(domaine,'_faces.txt'));
nel2D0=size(el2Dtypd,1)
[FT,r]=sort(el2Dtypd(:,1:3),2);      
[FTu,l]=unique(FT,'rows' ); %size(FT),size(FTu)
el2Dtyp=el2Dtypd(l,:);
nel2Dt=size(el2Dtyp,1)
duplicate_l = setdiff(1:nel2D0, l);
%==============================
% Les triangles en transmission 
%==============================
el2D_Trsm=el2Dtypd(duplicate_l,:);
%affiche_triangle(x,y,z, el2D_Trsm,'xb-')
%==============================
el2D=el2Dtyp(:,1:3);%ELEMENTS TRIANGULAIRES UNIQUEMENT LES CL
n_materiau=el2Dtyp(:,4);% MATERIAU
nel2D=size(el2D,1)% NOMBRE DE TRIANGLES EN CONDITION LIMITE
%=====================================================================================
  %RECHERCHE DES FACES FRONTIERES de chaque BLOCKS
%=====================================================================================
[sortedel2D,IS]= sort(el2D,2);% Mise dans l'ordre sur chaque ligne
nel2D=zeros(NBLOCKS,1);  Surf=zeros(NBLOCKS,1);
VOLUME=zeros(NBLOCKS,1);lambda=zeros(NBLOCKS,1);
for i=1:NBLOCKS
   [~, sortedel2di] = boundary_faces(Tet{i});% Elements Triangulaires SALLE i
 el2di{i}= el2Dtyp(ismember(sortedel2D,sortedel2di,'rows'),:);
end  

%===========        FIN RECHERCHE DES FACES FRONTIERES, el2di{i}                         =========
%========================================================================================
%
V_VC=zeros(sum(nnB),1);
 for i=1:NBLOCKS
nel2D(i)=max(size( el2di{i} ));% NOMBRE DE TRIANGLES EN CONDITION LIMITE
[Surf(i),AireTr{i}]= Surfaces_Salle(x,y,z,nel2D(i),el2di{i}) ;% Surface des parois+de chaque elt
[VOLUME(i)]= VolumeSalle(x,y,z,size( Tet{i},1), Tet{i}) ;% VOLUME de la salle en m3
[V_VCi{i}]=VolumeVCDOF(x,y,z,sum(nnB),size(Tet{i},1),Tet{i}, Tet_Dof{i});% Tableau des Volumes des Volumes de Controle 
V_VC=V_VCi{i}+V_VC;
end
%clear V_VCi
%===================================================================================
%                   PARAMETRES DE LA THEORIE DE DIFFUSION ACOUSTIQUE
%===================================================================================
global c0
c0=342;% sound velocity
lambda=4*VOLUME./Surf ;%Free Mean Pass (LIBRE PARCOURS MOYEN)
CoeffDiff=lambda*c0/3;%  Diffusion coefficient
m=1e-3;        mc=m*c0;%Atmospheric  attenuation
mc=0;
%===================================================================================
%===================================================================================
%%                                 GLOBAL MATRIX ASSEMBLAGE 
%===================================================================================
RHS=zeros(NDOF,1);% Memory allocation
 %Tetra Assemblage of Global mat for diffusion operator
[mat]=laplacienblocks(x,y,z,Tet,Tet_Dof,NBLOCKS,NDOF,CoeffDiff);
 
mat=-mat+mc*diag(V_VC);
%===================================================================================
%toc
BdOct1=5;      % 100Hz
BdOctend=23; % 5kHz
NOct=size(BdOct1:BdOctend,2);
%%%%%%%%%%%%%%%%%%%%%ù%%%%%%%
 for N_Toct=1:NOct;
                            mat_Toct100Hz_5k{N_Toct}=mat;
 end
%===================================================================================
% <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   TRANSMISSION
if NBLOCKS>1
  TransmissionConstructionMatrix;
end
 %===================================================================================
% <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ABSORPTION

% [mat_Toct100Hz_5k]=...
% BCMatrix(NBLOCKS,el2di,el2di_Dof,mat_Toct100Hz_5k,abs_prop,x,y,z,NOct)  ;
AbsorptionConstructionMatrix;
 %toc
%===================================================================================
% <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   SOUND SOURCE
 % Source d'energie acoustique, second membre e.d.p.
localiseSource
Srce_Power=Srce_sonore(1,7:end);
%========================================================================================================================================================
%   ><<<<<<<<<<<<<<<<<<<         PDE system resolution 
tic                                                                                           
for  N_Toct=1:NOct;
RHS(ind)=Srce_Power(1,N_Toct)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave N_Toct
w{N_Toct}=mat_Toct100Hz_5k{N_Toct}\RHS;
%w{N_Toct}= cgs(mat_Toct100Hz_5k{N_Toct},RHS,1e-6,200);
%   [L,U] = ilu(mat_Toct100Hz_5k{N_Toct},struct('type','ilutp','droptol',1e-6));
%   [w{N_Toct},flag]=bicgstab (mat_Toct100Hz_5k{N_Toct},RHS,1e-6,200,L,U);
end
toc
%========================================================================================================================================================

%w=wa{1};
waf=w{3};rhoco2=1.2*c0^2;    LpdB=10*log10(waf*rhoco2/(2e-5)^2);
XYZ2=XYZ(sortedndf(:,1),:);
colormap jet
subplot (2,2,1),patch('Faces',el2di_Dof{1}(:,1:3),'Vertices',XYZ2,'FaceVertexCData',LpdB,'FaceColor','interp'); 
colorbar,xlabel('X'), ylabel('Y'),zlabel('Z')
subplot (2,2,2),patch('Faces',el2di_Dof{2}(:,1:3),'Vertices',XYZ2,'FaceVertexCData',LpdB,'FaceColor','interp'); colorbar
subplot (2,2,3),patch('Faces',el2di_Dof{3}(:,1:3),'Vertices',XYZ2,'FaceVertexCData',LpdB,'FaceColor','interp'); colorbar
%XYZ1=XYZ2; XYZ1(:,1)=XYZ1(:,1)-5;
%XYZ3=XYZ1; XYZ3(:,2)=XYZ3(:,2)+3;
%figure
%colormap jet
%patch('Faces',el2di_Dof{3}(:,1:3),'Vertices',XYZ1,'FaceVertexCData',LpdB,'FaceColor','interp'); colorbar
%hold on
%patch('Faces',el2di_Dof{2}(:,1:3),'Vertices',XYZ2,'FaceVertexCData',LpdB,'FaceColor','interp');
%patch('Faces',el2di_Dof{1}(:,1:3),'Vertices',XYZ3,'FaceVertexCData',LpdB,'FaceColor','interp');
%xlabel('X'), ylabel('Y'),zlabel('Z')
%titre = 'Energy density (dB) for diffusion problem';



  %Calcul_insta