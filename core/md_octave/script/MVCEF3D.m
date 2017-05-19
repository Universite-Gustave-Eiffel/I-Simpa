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
 %domaine='cub05';% Cube 194028 elt 9260noeuds 48 secondes
% domaine='p01h01';% Salle 3*1*1 dx=dy=dz=0.1, prendre facteurEchelle=3
domaine='scene';% Maillage lache  1164 POINTS  5400 elt 3D
  %domaine='demicyl2';% Maillage moins lache

  %domaine='cub10';% Cube nbel 3d=  6107 tourne en 147 secondes
 %%domaine='salles_couplees';% Cube
 facteurEchelle=1
%facteurEchelle=3% pour cub05
 
 el = load(strcat(domaine,'_elements.txt'));
XYZ= load(strcat(domaine,'_nodes.txt'));
x=XYZ(:,1);y=XYZ(:,2);z=XYZ(:,3);


XYZ=XYZ*facteurEchelle;x=x*facteurEchelle;y=y*facteurEchelle;z=z*facteurEchelle;
%========================================================
nn=size(XYZ,1); % nombre de noeuds (nombre de ddl)
nbel=size(el,1);% nombre d'elements Tetraedres
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%RECHERCHE DES FACES FRONTIERES des Tetraedres
el2D = boundary_faces(el);% Elements Triangulaires

b = unique(el2D(:));
nel2D=max(size(el2D))% NOMBRE DE TRIANGLES EN CONDITION LIMITE

 %subplot(1,2,1);
%plot3(XYZ(b,1),XYZ(b,2),XYZ(b,3),'.');
%subplot(1,2,2);
 %plot TRIANGLES FRONTIERES
%trisurf(el2D,XYZ(:,1),XYZ(:,2),XYZ(:,3),'FaceAlpha',0.9),title('Maillage')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

[VOLUME]= VolumeSalle(x,y,z,nbel,el) % VOLUME de la salle
[Surf,AireTr]= Surfaces_Salle(x,y,z,nel2D,el2D) ;% Surface des parois+de chaque elt
% PARAMETRES DE LA THEORIE DE DIFFUSION ACOUSTIQUE
lambda=4*VOLUME/Surf;% LIBRE PARCOURS MOYEN
c0=342;

D=lambda*c0/3;% coeff de diffusion
%hcoef_echge=c0*alphamoy/4;% Coefficient d'echange
[V_VC]= VolumeVC(x,y,z,nn,nbel,el);% Volumes des nn Volumes de Controle 

%Atmospheric  attenuation
m=1e-3;
mc=m*c0;
 
RHS=zeros(nn,1);%SECOND MEMBRE 
% Construction des matrices DE COEFFICIENTS 

[mat]=laplaciencvfem3d(x,y,z,el,nbel,nn);
mat=-mat*D+mc*diag(V_VC);
h2d=ones(nel2D,1); %Tableau de coeff. d'echange attribu� � chaque triangle frontiere
% Deux zones d'absorbtion diff�rentes
XG_el2d=(x(el2D(:,1))+x(el2D(:,2))+x(el2D(:,3)))/3;% 1er coord. du centre de gravite de chaque Triangle
h2d(XG_el2d<=0)= c0/4*0.1;% Coefficient d'echange
h2d(XG_el2d>0) = c0/4*0.9;% Coefficient d'echange
Aire_Abs_eq=4/c0*sum(h2d.*AireTr);
alphamoySab=Aire_Abs_eq/Surf% Coeff moyen SABINE


 % Ajout des coefficients tires des Cond. Lim.
[mat]=fluxFronTriang2d_hvar(h2d,x,y,z,nel2D,el2D,mat);% Conditions aux limites

% Source d'energie acoustique, second membre e.d.p.
xs=2. ;ys=1.5;zs=1.5;% Position de la source
%xs=0. ;ys=0;zs=2.;% Position centrale source% Cylindre
xs=min(x) +(max(x)-min(x))/2;ys=min(y) +(max(y)-min(y))/2;zs=min(z) +(max(z)-min(z))/2;
dist2S=(x-xs).^2+(y-ys).^2+(z-zs).^2; 
rayonS2=0.1^2;% Carr� du Rayon de la source de puissance injectee
VolSource=-10;
while VolSource<=0
ind=1:nn;rayonS2=rayonS2*1.5;
ind=ind(dist2S<rayonS2);VolSource=sum(V_VC(ind));% Volume de la source
end   
RHS(ind)=1e-2*V_VC(ind)/VolSource;% 0.01Watt 100dB
%===================
w=mat\RHS;% Resolution du systeme edp
%=====================
rhoco2=1.2*c0^2;    LpdB=10*log10(w*rhoco2/(2e-5)^2);
% FICHIERS DE LA SOLUTION EN REGIME ETABLI au format TECPLOT
threed_to_tecplot(XYZ,el,w,strcat(domaine,'.dat'),'Densite d''Energie acoustique')
threed_to_tecplot(XYZ,el,LpdB,strcat(domaine,'dB.dat'),'Densite d''Energie acoustique')
fichier_sortie = strcat(domaine,'dB.vtk')';
titre = 'Energy density (dB) for diffusion problem';

%
 % threedscal_to_vtk ( XYZ,el,LpdB, fichier_sortie, titre );
  %pause
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       SOLUTION INSTATIONNAIRE PARTANT DU REGIME ETABLI -
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TR60=0.16*VOLUME/(Surf*alphamoySab)% SABINE
TR60E=0.16*VOLUME/(-Surf*log(1-alphamoySab))% SABINE
dt=1e-3;% Pas de temps
itmax=round(2*TR60/3/dt);% Nombre d'iterations
wi=w;% REGIME ETABLI  
 %[MATinsta]= integVC_lin(x,y,z,nn,nbel,el);% int�gration lineaire
[MATinsta]= integVC_cst(nn,V_VC);% integration const
 
mat2=dt*mat+ MATinsta;%Euler Implicite 1
%mat2=dt/2*mat+ MATinsta;%Cranck-Nicholson 1
%mat3=-dt/2*mat+ MATinsta;%Cranck-Nicholson 1

wmic=zeros(itmax,1);% Mesure temporelle en 1 Pt de Mesure
ipmic =200;% Indice du Pt de Mesure (au hazard!!!)
ind=1:nn;distance_recept=rayonS2*5;
ind=ind(dist2S>distance_recept);
ind=ind(z(ind)>1.5);ind=ind(x(ind)<8);ind=ind(y(ind)<8);ind=ind(x(ind)>-8);
ind=ind(y(ind)>-8);
ipmic =ind(1);

Coord_Pt_de_Mesure=[x(ipmic) y(ipmic) z(ipmic)]% Position du Pt de Mesure
wmic(1)=wi(ipmic);
tic
for it=1:itmax
    RHS=MATinsta*wi;%Euler Implicite 2
    %    RHS=mat3*wi;%Cranck-Nicholson 2

    wi=mat2\RHS;
  % wi= bicgstabl(mat2,RHS,1e-5,150);
  %wi= cgs(mat2,RHS,1e-6,200);
    wmic(it+1)=wi(ipmic);
end
toc
% ANALYSE DE LA SOLUTION PAR RAPPORT A SABINE TR 
LpdB=10*log10(wmic*rhoco2/4e-10);
id= find(LpdB-LpdB(1)<-5);% On débute apres 5dB de décroissance
i_debut=id(1);
 
id= find(LpdB-LpdB(i_debut)<-20); 
%%%%%%%%%%%%%%%%%%%%%%%%%%%
TRModel_Dif=(id(1)-i_debut)*dt*3
%%%%%%%%%%%%%%%%%%%%%%%%%%%
t=0:dt:itmax*dt;
%figure
plot(t,wmic),xlabel('t (s)'),ylabel('Energie w')
figure
set(gca,'fontsize',18)
hold on
plot(t,LpdB),xlabel('t (s)'),ylabel('Niveau en dB'), grid on
hold on
plot([0 TR60/2],[LpdB(1) LpdB(1)-60/2],'r')

plot([0 TR60E/2],[LpdB(1) LpdB(1)-60/2],'g')

 %legend('Modele diffusif', strcat('SABINE  : ',num2str(TR60),' s'), strcat('EYRING : ',num2str(TR60E),' s'))
  legend(strcat('Modele diffusion',num2str(TRModel_Dif),' s'), strcat('SABINE  : ',num2str(TR60),' s'), strcat('EYRING : ',num2str(TR60E),' s'))

 title(strcat(  'TR Modele Diffusion  :',num2str(TRModel_Dif),' s'))

