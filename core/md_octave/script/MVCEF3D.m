 %**********************************************
%***********************************************************
%    Christian.prax@univ-poitiers.fr
%Institut PPRIME, CNRS - UniversitÃ© de Poitiers â€“ ENSMA, UPR 3346
%Ecole Nationale SupÃ©rieure dâ€™ingÃ©nieurs de Poitiers, ENSIP
%   MAI 2016
%************************************************************
clc; clear all; close all, clf;
%************************************************************
% RESOLUTION DE l'EQUATION DE DIFFUSION 3D
% METHODE DE VOLUMES DE CONTROLES/ELEMENTS FINIS (MVCEF)
% TETRAEDRES 4 pts INTERPOLATION LINEAIRE
% APPLICATION ENERGIE ACOUSTIQUE DANS UN LOCAL
% MEME COEFFICIENT d'ECHANGE pour TOUTES LES CONDITIONS AUX LIMITES 
%************************************************************
% LECTURE MAILLAGE/ CONNECTIVITES et coordonnees des noeuds
 % Intitule du fichier 
%domaine='cub05';% Cube 194028 elt
% domaine='p01h01';% Salle 3*1*1 dx=dy=dz=0.1, prendre facteurEchelle=3
domaine='scene';% Maillage lache  1164 POINTS 
%  domaine='demicyl2';% Maillage moins lache
  %domaine='cub10';% Cube
    % domaine='boule';% Cube

 %domaine='p01h02';% Salle cubique 4*4*4
% domaine='p01';% Salle cubique 4*4*4
% domaine='coul8x2_4x1'
 %domaine='salles_couplees';% Cube

%facteurEchelle=3
 
 el = load(strcat(domaine,'_elements.txt'));
XYZ= load(strcat(domaine,'_nodes.txt'));
x=XYZ(:,1);y=XYZ(:,2);z=XYZ(:,3);

%tet_mesh_tet_neighbors ( domaine )


%XYZ=XYZ*facteurEchelle;x=x*facteurEchelle;y=y*facteurEchelle;z=z*facteurEchelle;


%========================================================
nn=size(XYZ,1)% nombre de noeuds (nombre de ddl)
nbel=size(el,1)% nombre d'elements Tetraedres
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%RECHERCHE DES FACES FRONTIERES des Tetraedres
el2Dtyp= load(strcat(domaine,'_faces.txt'));
el2D=el2Dtyp(:,1:3);%ELEMENTS TRINGULAIRES UNIQUEMENT LES CL
face_mat=el2Dtyp(:,4);% MATERIAU
nel2D=max(size(el2D))% NOMBRE DE TRIANGLES EN CONDITION LIMITE

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
%lambda=zeros(size(x));
[VOLUME]= VolumeSalle(x,y,z,nbel,el) % VOLUME de la salle
[Surf,AireTr]= Surfaces_Salle(x,y,z,nel2D,el2D) ;% Surface des parois+de chaque elt
% PARAMETRES DE LA THEORIE DE DIFFUSION ACOUSTIQUE
lambda=4*VOLUME/Surf;% LIBRE PARCOURS MOYEN
c0=342;

D=lambda*c0/3;% coeff de diffusion
%hcoef_echge=c0*alphamoy/4;% Coefficient d'echange
[V_VC]= VolumeVC(x,y,z,nn,nbel,el);% Volumes des nn Volumes de Controle 

%Atmospheric  attenuation
m=1e-3;mc=m*c0;
 
RHS=zeros(nn,1);%SECOND MEMBRE 
% Construction des matrices DE COEFFICIENTS 
        ia = [];        ja = [];        s = [];         mat = sparse ( ia, ja, s, nn, nn,12 );
tic
[mat]=laplaciencvfem3d(x,y,z,el,nbel,nn);% coeff Points internes
mat=-mat*D+mc*diag(V_VC);
%%%%%%%%%%%%%%%%%%%%%ù%%%%%%%
%    BOUCLER sur les Tiers Octave
%%%%%%%%%%%%%%%%%%%%%ù%%%%%%%
M_abs=load(strcat(domaine,'_materials_absorption.txt'));
abs_prop=M_abs(1:end,:);
%abs_prop=M_abs(2,:);


%Aire_Abs_eq=sum(4/c0*h2d.*AireTr);
%alphamoy=Aire_Abs_eq/Surf% Coeff moyen SABINE
 


 % Ajout des coefficients tires des Cond. Lim.
%[mat_octav]=fluxFronTriang2d_oct(abs_prop,x,y,z,nel2D,el2D,mat);% Conditions aux limites
 %[mat_100, mat_125, mat_160, mat_200, mat_250, mat_315]
 [mat_100, mat_125, mat_160, mat_200, mat_250, mat_315, mat_400,...
mat_500,mat_630,mat_800, mat_1k,mat_1250, mat_1600, mat_2k,mat_2500,mat_3150, mat_4k,mat_5k]=...
fluxFronTriang2d_oct(abs_prop,x,y,z,nel2D,el2D,mat,face_mat);
 
% Source d'energie acoustique, second membre e.d.p.
Srce_sonore=load(strcat(domaine,'_sources.txt'));
abs_prop=M_abs(2:end,:);

xs=Srce_sonore(1,1) ;ys=Srce_sonore(1,2) ;zs=Srce_sonore(1,3) ;% Position de la source
%xs=0. ;ys=0;zs=2.;% Position centrale source% Cylindre
%xs=min(x) +(max(x)-min(x))/2;ys=min(y) +(max(y)-min(y))/2;zs=min(z) +(max(z)-min(z))/3;
dist2S=(x-xs).^2+(y-ys).^2+(z-zs).^2; 
rayonS2=0.15^2;% Rayon de la source de puissance injectee
VolSource=-10;
while VolSource<=0
ind=1:nn;rayonS2=rayonS2*1.5;
ind=ind(dist2S<rayonS2);VolSource=sum(V_VC(ind));% Volume de la source
end   
Srce_sonore(1,4:end)=10.^(Srce_sonore(1,4:end)/10)*1e-12;
RHS(ind)=Srce_sonore(1,7)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 100Hz
w100=mat_100\RHS;w=w100;% Resolution du systeme edp
RHS(ind)=Srce_sonore(1,8)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 125Hz
w125=mat_125\RHS;
RHS(ind)=Srce_sonore(1,9)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 160Hz
w160=mat_160\RHS;
RHS(ind)=Srce_sonore(1,10)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 200Hz
w200=mat_200\RHS;
RHS(ind)=Srce_sonore(1,11)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 250Hz
w250=mat_250\RHS;
RHS(ind)=Srce_sonore(1,12)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 315Hz
w315=mat_315\RHS;
RHS(ind)=Srce_sonore(1,13)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 400Hz
w400=mat_400\RHS;
RHS(ind)=Srce_sonore(1,14)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 500Hz
w500=mat_500\RHS;
RHS(ind)=Srce_sonore(1,15)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 630Hz
w630=mat_630\RHS;
RHS(ind)=Srce_sonore(1,16)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 800Hz
w800=mat_800\RHS;
RHS(ind)=Srce_sonore(1,17)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 1000Hz
w1k=mat_1k\RHS;
RHS(ind)=Srce_sonore(1,18)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 2000Hz
w2k=mat_2k\RHS;
RHS(ind)=Srce_sonore(1,19)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 3600Hz
w3150=mat_3150\RHS;
RHS(ind)=Srce_sonore(1,20)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 4000Hz
w4k=mat_4k\RHS;
RHS(ind)=Srce_sonore(1,21)*V_VC(ind)/VolSource;% Puissance sonre sur le tiers d'octave 5000Hz
w5k=mat_5k\RHS;


toc
%========================================================================================================================================================
% Sauvegarde pour ISIMPA
% Ecriture des résultats par bande de 1/3 octave
ChpsStatio100_5k=strcat(domaine,'_WStatioFields.txt')
   [fid] = fopen(ChpsStatio100_5k,'w');
   print_str = ' %12.8f %12.8f %12.8f %12.8f %12.8f %12.8f%12.8f %12.8f %12.8f %12.8f %12.8f %12.8f %12.8f %12.8f \r\n';
 
 fprintf(fid,print_str , [w100 w125 w160 w200 w315 w400 w500 w630 w800 w1k w2k w3150 w4k w5k]);
 fclose(fid)
%========================================================================================================================================================



rhoco2=1.2*c0^2;    LpdB=10*log10(w*rhoco2/(2e-5)^2);
% FICHIERS DE LA SOLUTION EN REGIME ETABLI au format TECPLOT
sauv_tecplot(XYZ,el,w,strcat(domaine,'.dat'),'Densite d''Energie acoustique')
sauv_tecplot(XYZ,el,LpdB,strcat(domaine,'dB.dat'),'Densite d''Energie acoustique (dB)')
output_filename = strcat(domaine,'dB.vtk')';
titre = 'Energy density (dB) for diffusion problem';
  threedscal_to_vtk ( XYZ,el,LpdB, output_filename', titre );
  %threedp_to_vtu ( XYZ,el,LpdB, output_filename2', titre );
  %msh2.t=el';msh2.p=XYZ';  p   = msh2.p;
%  dim = rows (p) ;% 2D or 3D
%  fpl_vtk_write_field (output_filename1', msh2, {LpdB, 'Lp (dB)'}, {}, 0);
  
%PlotFieldonMesh(XYZ,el,w)

  trisurf(el2D,XYZ(:,1),XYZ(:,2),XYZ(:,3),w), colorbar
   
   
   figure,
    LpdB=10*log10(w*rhoco2/(2e-5)^2);
   trisurf(el2D,XYZ(:,1),XYZ(:,2),XYZ(:,3),LpdB), title('LpdB'), colorbar

 % Calcul_insta
