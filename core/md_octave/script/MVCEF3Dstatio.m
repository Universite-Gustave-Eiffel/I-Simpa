%**********************************************
%***********************************************************
%    Christian.prax@univ-poitiers.fr
%Institut PPRIME, CNRS - Université de Poitiers – ENSMA, UPR 3346
%Ecole Nationale Supérieure d’ingénieurs de Poitiers, ENSIP
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
domaine='demicyl3';% Maillage lache  1164 POINTS 
%  domaine='demicyl2';% Maillage moins lache
 % domaine='cub10';% Cube
    %domaine='boule';% Cube

 %domaine='p01h02';% Salle cubique 4*4*4
% domaine='p01';% Salle cubique 4*4*4
% domaine='coul8x2_4x1'
 %%domaine='salles_couplees';% Cube

facteurEchelle=1
 
 el = load(strcat(domaine,'_elements.txt'));
XYZ= load(strcat(domaine,'_nodes.txt'));

tet_mesh_tet_neighbors ( domaine )
x=XYZ(:,1);y=XYZ(:,2);z=XYZ(:,3);


XYZ=XYZ*facteurEchelle;x=x*facteurEchelle;y=y*facteurEchelle;z=z*facteurEchelle;
%========================================================
nn=size(XYZ,1)% nombre de noeuds (nombre de ddl)
nbel=size(el,1)% nombre d'elements Tetraedres
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%RECHERCHE DES FACES FRONTIERES des Tetraedres
el2D = boundary_faces(el);% Elements Triangulaires

b = unique(el2D(:));
nel2D=max(size(el2D))% NOMBRE DE TRIANGLES EN CONDITION LIMITE

 % subplot(1,2,1);
% plot3(XYZ(b,1),XYZ(b,2),XYZ(b,3),'.');
% subplot(1,2,2);
% plot TRIANGLES FRONTIERES
%trisurf(el2D,XYZ(:,1),XYZ(:,2),XYZ(:,3),'FaceAlpha',0.9),title('Maillage')
%pause
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%lambda=zeros(size(x));
[VOLUME]= VolumeSalle(x,y,z,nbel,el) % VOLUME de la salle
[Surf]= SurfacesSalle(x,y,z,nel2D,el2D) % Surface des parois
% PARAMETRES DE LA THEORIE DE DIFFUSION ACOUSTIQUE
lambda=4*VOLUME/Surf;% LIBRE PARCOURS MOYEN
c0=342;
alphamoy=0.1% Coeff SABINE
D=lambda*c0/3;% coeff de diffusion
hcoef_echge=c0*alphamoy/4;% Coefficient d'echange
[V_VC]= VolumeVC(x,y,z,nn,nbel,el);% Volumes des nn Volumes de Controle 

%Atmospheric  attenuation
m=1e-3;
mc=m*c0;
 
RHS=zeros(nn,1);%SECOND MEMBRE 
% Construction des matrices DE COEFFICIENTS 

[mat]=laplaciencvfem3d(x,y,z,el,nbel,nn);
mat=-mat*D+mc*diag(V_VC);
 % Ajout des coefficients tires des Cond. Lim.
[mat]=fluxFronTriang2d(hcoef_echge,x,y,z,nel2D,el2D,mat);% Conditions aux limites

% Source d'energie acoustique, second membre e.d.p.
xs=3. ;ys=1.5;zs=1.5;% Position de la source
%xs=0. ;ys=0;zs=2.;% Position centrale source% Cylindre
xs=min(x) +(max(x)-min(x))/2;ys=min(y) +(max(y)-min(y))/2;zs=min(z) +(max(z)-min(z))/3;
dist2S=(x-xs).^2+(y-ys).^2+(z-zs).^2; 
rayonS2=0.15^2;% Rayon de la source de puissance injectee
VolSource=-10;
while VolSource<=0
ind=1:nn;rayonS2=rayonS2*1.5;
ind=ind(dist2S<rayonS2);VolSource=sum(V_VC(ind));% Volume de la source
end   
RHS(ind)=1e-2*V_VC(ind)/VolSource;% 0.01Watt 100dB
w=mat\RHS;% Resolution du systeme edp
rhoco2=1.2*c0^2;    LpdB=10*log10(w*rhoco2/(2e-5)^2);
% FICHIERS DE LA SOLUTION EN REGIME ETABLI au format TECPLOT
sauv_tecplot(XYZ,el,w,strcat(domaine,'.dat'),'Densite d''Energie acoustique')
sauv_tecplot(XYZ,el,LpdB,strcat(domaine,'dB.dat'),'Densite d''Energie acoustique (dB)')
output_filename = strcat(domaine,'dB.vtk')';
output_filename2 = strcat(domaine,'dB.vtu')';output_filename1 = strcat(domaine,'1dB')';
titre = 'Energy density (dB) for diffusion problem';
  threedscal_to_vtk ( XYZ,el,LpdB, output_filename', titre );
  threedp_to_vtu ( XYZ,el,LpdB, output_filename2', titre );
  msh2.t=el';msh2.p=XYZ';
  p   = msh2.p;
%  dim = rows (p) ;% 2D or 3D
%  fpl_vtk_write_field (output_filename1', msh2, {LpdB, 'Lp (dB)'}, {}, 0);
  
%PlotFieldonMesh(XYZ,el,w)
 wa=sum(w(el),2)/4;w2=sum(w(el2D),2);
%tetramesh(el,XYZ,wa);
% trisurf(el2D,XYZ(:,1),XYZ(:,2),XYZ(:,3),w2),title('Maillage')
%figure
%plotmesh(XYZ,el,'x<1 & x>2');
