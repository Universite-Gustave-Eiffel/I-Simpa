#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec  2 16:31:25 2019

@author: efoucaul
"""
#####################################################
#           bibli_MD_ef0
#     Biblitothèques pour MD
# EF XII 2019
#####################################
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
#
# Version Python EF IV 2020
#
#######################################################
import numpy as np
from scipy.constants import atmosphere, zero_Celsius
#######################################################
# Définition locale de constantes :
#######################################################
TOB=np.array([50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000, 20000])
SelectedFrequency=np.arange(7)
Temperature=20.0
Humidity=50.0
tol=0.000001
maxint=200
dt=0.001000
duration=2.000000
atmos_absorption_calculation = 1
###############################################################
################   changem(z,newcode, oldcode)  ##############
def changem(z,newcode, oldcode):
################## Octave #####################################
#function mapout = changem(Z, newcode, oldcode)
## usage: mapout = changem(Z, newcode, oldcode)
## Identical to the Matlab Mapping Toolbox's changem
## Note the weird order: newcode, oldcode. I left it unchanged from Matlab.
##
## In:
##	- Z :
##	- newcode :
##	- oldcode :
##
## Out:
##	- mapout : 
#
#  if numel(newcode) ~= numel(oldcode)
#    error('newcode and oldcode must be of equal length');
#  end
#  mapout = Z;
#  for ii = 1:numel(oldcode)
#    mapout(Z == oldcode(ii)) = newcode(ii);
#  end
#end
################## Python 3 ####################################
  forme=z.shape
  z=z.flatten()
  newcode=newcode.flatten()
  oldcode=oldcode.flatten()
  for k in range(len(z)):
    val=z[k]
    if val in oldcode:
      result=np.where(oldcode==val)
      for l in result[0]:
        z[k]=newcode[l]
  return z.reshape(forme)
################  fin  changem(z,newcode, oldcode)  ##############
###############################################################
#############   Coef_Att_Atmos(F,H,P,K)  ##################"
def Coef_Att_Atmos(F,H,P,K):
# usage: m, cson=Coef_Att_Atmos(F,H,P,K)
# Calculate the coefficient of atmospheric absorption according to ISO 9613-1
#
# In:
#	- F : Frequency (Hz)
#	- H : Relative Humidity (%)
#	- P : Atmospheric pressure (Pa)
#	- K : Air temperature (K)
#
# Out:
#	- m : Coefficient of atmospheric absorption (Np/m)
#	- cson : Sound speed (m/s)
# Atmospheric information
    Pref=atmosphere	# Reference pressure (Pa)
    Kref=zero_Celsius+20	# Reference temperature (K)
    FmolO=0.209	# 
    FmolN=0.781	# 
    KvibO=2239.1	# 
    KvibN=3352.0	# 
    K01=273.16     # 
# Sound speed
    cson=343.2*np.sqrt(K/Kref)
# Molar concentration of water vapor
    C=-6.8346*((K01/K)**1.261)+4.6151
#    print('K01=',K01, ' K=',K)
    Ps=Pref*(10**C)
    hmol=H*(Ps/Pref)
# Rotational absorption
    Acr=(Pref/P)*(1.60e-10)*np.sqrt(K/Kref)*F**2
    print('####################################')
    print('Pref/P=',Pref/P)
    print('K/Kref=',K/Kref)
    print('F=',F)
# Relaxation frequency of molecular oxygen
    Fr=(P/Pref)*(24+(4.04e4)*hmol*(0.02+hmol)/(0.391+hmol))
    Am=1.559*FmolO*np.exp(-KvibO/K)*(KvibO/K)**2
    print('F/Fr=',F/Fr)
    AvibO=Am*(F/cson)*2.*(F/Fr)/(1+(F/Fr)**2)   
# Relaxation frequency of molecular nitrogen
    Fr=(P/Pref)*np.sqrt(Kref/K)*(9+280*hmol*np.exp(-4.170*((K/Kref)**(-1/3)-1)))
    Am=1.559*FmolN*np.exp(-KvibN/K)*(KvibN/K)**2
    AvibN=Am*(F/cson)*2.*(F/Fr)/(1+(F/Fr)**2) 
# Total absorption in Np/m
    m=(Acr+AvibO+AvibN)*np.log(10)/10
    return m, cson
#############  fin  Coef_Att_Atmos(F,H,P,K)  ##################"
###############################################################
def boundary_faces(T):
################ Octave ########################
#function [F , Fsort]= boundary_faces(T)
## usage: F = boundary_faces(T)
## Find the faces of the meshes on boundaries
##
## In:
##	- T : meshes (tetreahedrics) of size m
##
## Out:
##	- F : list of faces on boundaries
##############################""
#  allF = [ ...
#  T(:,1) T(:,2) T(:,3); ...
#  T(:,1) T(:,3) T(:,4); ...
#  T(:,1) T(:,4) T(:,2); ...
#  T(:,2) T(:,4) T(:,3)];
#  # Increasing sort faces
#  sortedF = sort(allF,2);
#  # Verify unicity of faces
#  [u,m,n] = unique(sortedF,'rows');  
#  # Determine counts for each unique face
#  counts = accumarray(n(:), 1);
#  # Extract faces that only occurred once
#  sorted_exteriorF = u(counts == 1,:);
#  # find in original faces so that ordering of indices is correct
#  F = allF(ismember(sortedF,sorted_exteriorF,'rows'),:);
#  Fsort=sort(F,2);
#end
################## Python ####################################
  ##    nb : peut être + simple avec block
  a=np.concatenate((T[:,0,np.newaxis],T[:,1,np.newaxis],T[:,2,np.newaxis]),axis=1)
  b=np.concatenate((T[:,0,np.newaxis],T[:,2,np.newaxis],T[:,3,np.newaxis]),axis=1)
  c=np.concatenate((T[:,0,np.newaxis],T[:,3,np.newaxis],T[:,1,np.newaxis]),axis=1)
  d=np.concatenate((T[:,1,np.newaxis],T[:,3,np.newaxis],T[:,2,np.newaxis]),axis=1)
  allF=np.concatenate((a,b,c,d))
  #  # Increasing sort faces
  sortedF = np.sort(allF)   # trie toutes les lignes
  ##  # Verify unicity of faces
  u,m,n,counts = np.unique(sortedF,return_index= True, return_inverse= True, return_counts=True, axis=0)
  ##  # Determine counts for each unique face
  ##  # Extract faces that only occurred once
  sorted_exteriorF = u[counts == 1,:]
  ##  # find in original faces so that ordering of indices is correct
  F=np.nonzero(np.all(sorted_exteriorF == sortedF[:,np.newaxis], axis=2))
  Fsort=sorted_exteriorF[F[1]]
  return F, Fsort
############  fin  boundary_faces(T)  #############"
#___________________________________
def Surfaces_Salle(x, y, z, nd, kne):
################ Octave ########################
### CALCULATION OF THE WHOLE SURFACE BOUNDARIES
#% c----$---1---------2---------3---------4---------5---------6---------7-c
#
#function [SurfSalle,AireTr]= Surfaces_Salle(x,y,z,nd,kne)
## usage: [SurfSalle,AireTr]= Surfaces_Salle(x,y,z,nd,kne)
## Calculate the whole surface of boundaries
##
## In:
##	- x,y,z : Nodes coordinates
##	- nd : Number of meshes
##	- kne : TODO: this parameter seems not needed
##
## Out:
##	- SurfSalle : Surface of the boundaries
##	- AireTr : Surface of a mesh element on boundaries (matrix of size nd)
#
## Initialisation
#SurfSalle=0;
#%nd
#AireTr=zeros(nd,1);
#
#for n=1:nd
#              
#  ks=kne(n,:);
#  XA=[x(ks(1)); y(ks(1)); z(ks(1))];
#  XB=[x(ks(2)); y(ks(2)); z(ks(2))];
#  XC=[x(ks(3)); y(ks(3)); z(ks(3))];
#
#  % Calculation of the surface of a mesh face
#  AireTr(n)= 1/2*norm(cross((XB-XA),(XC-XA)));
#  SurfSalle=AireTr(n)+SurfSalle;
#
#  end
#            
#end
################## Python ####################################
  SurfSalle=0
  nd=int(nd)
  AireTr=np.zeros(nd)
  for n in np.arange(0,nd):
    ks=kne[n,:]
    XA=np.asarray([x[ks[0]-1] , y[ks[0]-1] , z[ks[0]-1]])
    XB=np.asarray([x[ks[1]-1] , y[ks[1]-1] , z[ks[1]-1]])
    XC=np.asarray([x[ks[2]-1] , y[ks[2]-1] , z[ks[2]-1]])
  ###% Calculation of the surface of a mesh face
    AireTr[n]= 1/2*np.linalg.norm(np.cross((XB-XA),(XC-XA)))
    SurfSalle=AireTr[n]+SurfSalle
  return SurfSalle, AireTr
###############################################################
def volumeTetra(xn,yn,zn) :
################### Octave ####################################
#function [v]=volumeTetra(xn,yn,zn)
## usage: [v]=volumeTetra(xn,yn,zn)
## Calculate the volume of a given mesh
##
## In:
##	- xn,yn,zn : Coordinates of the mesh
##
## Out:
##	- v : Volume of a given mesh
#
#  XA=[xn(1); yn(1); zn(1)];
#  XB=[xn(2); yn(2); zn(2)];
#  XC=[xn(3); yn(3); zn(3)];
#  XD=[xn(4); yn(4); zn(4)];
#  v=dot(XD-XA,cross((XB-XA),(XC-XA)))/6;
#
#return
################## Python ####################################
# usage: [v]=volumeTetra(xn,yn,zn)
# Calculate the volume of a given mesh
#
# In:
#	- xn,yn,zn : Coordinates of the mesh
#
# Out:
#	- v : Volume of a given mesh

  XA=np.asarray([xn[0] , yn[0] , zn[0]])
  XB=np.asarray([xn[1] , yn[1] , zn[1]])
  XC=np.asarray([xn[2] , yn[2] , zn[2]])
  XD=np.asarray([xn[3] , yn[3] , zn[3]])
  v=np.dot(XD-XA,np.cross((XB-XA),(XC-XA)))/6

  return v
#___________________________________
def VolumeSalle(x,y,z,nbel,kne) :
################## Octave ####################################
#function [vtotal]= VolumeSalle(x,y,z,nbel,kne)
## usage: [vtotal]= VolumeSalle(x,y,z,nbel,kne)
## Calculate the volume of a room
## In:
##	- x,y,z : Nodes coordinates
##	- nd : Number of meshes
##	- kne : TODO: this parameter seems not needed
##
## Out:
##	- vtotal : Volume of a room
#
#vtotal=0;
## The volume is equal to the sum of the volume of all the meshes
#for n=1:nbel
#  ks=kne(n,:);            
#  [v]=volumeTetra(x(ks),y(ks),z(ks));
#  vtotal=vtotal+v;
#end
################## Python ####################################
#from volumeTetra import volumeTetra
#___________________________________
# usage: vtotal= VolumeSalle(x,y,z,nbel,kne)
# Calculate the volume of a room
# In:
#	- x,y,z : Nodes coordinates
#	- nd : Number of meshes
#	- kne : TODO: this parameter seems not needed
#
# Out:
#	- vtotal : Volume of a room
#
  vtotal=0
  # The volume is equal to the sum of the volume of all the meshes
#  print('nbel=',nbel)
  for n in range(nbel) :
      ks=kne[n,:]
      v=volumeTetra(x[ks-1],y[ks-1],z[ks-1])
      vtotal=vtotal+v
  return vtotal
################## VolumeVCDOF  Octave ####################################
def VolumeVCDOF(x,y,z,nn,nbel,Tet ,Tet_Dof) :
#% c----$---1---------2---------3---------4---------5---------6---------7-c
#
#function [vn]= VolumeVCDOF(x,y,z,nn,nbel,Tet ,Tet_Dof)
## Volume around each point
#
#	vn=zeros(nn,1);
#	for n=1:nbel
#		ks=Tet(n,:);     
#	ksDof=Tet_Dof(n,:);      
#	[v]=volumeTetra(x(ks),y(ks),z(ks));
#	vn(ksDof(1))=vn(ksDof(1))+v/4; vn(ksDof(2))=vn(ksDof(2))+v/4;
#	vn(ksDof(3))=vn(ksDof(3))+v/4; vn(ksDof(4))=vn(ksDof(4))+v/4;
# end
################## Python ####################################
# Volume around each point
  vn=np.zeros(nn)
  for n in range(nbel) :
    ks=Tet[n,:]     
    ksDof = Tet_Dof[n,:]
    v=volumeTetra(x[ks-1],y[ks-1],z[ks-1])
    vn[ksDof[0]-1]=vn[ksDof[0]-1]+v/4
    vn[ksDof[1]-1]=vn[ksDof[1]-1]+v/4
    vn[ksDof[2]-1]=vn[ksDof[2]-1]+v/4
    vn[ksDof[3]-1]=vn[ksDof[3]-1]+v/4
  return vn
#___________________________________
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
#
# Version Python EF IV 2020
#
################## Octave #####################################
#function [mat]=laplacienblocks2(x,y,z,Tet_Dof,NBLOCKS,NDOF,CoeffDiff)
## usage: [mat]=laplacienblocks2(x,y,z,Tet_Dof,NBLOCKS,NDOF,CoeffDiff)
## Built the matrix map, element by element
## Need the function linecoeftemp for the calculation of pressure and temperature derivatives
##
## In:
##	- x,y,z : Coordinates of the nodes
##	- Tet_Dof : 
##	- NBLOCKS : Number of volumes
##	- NDOF : Number of degree au freedom
##	- CoeffDiff : Diffusion coefficient
##
## Out:
##	- mat : 
#
#ia = [];
#ja = [];
#s = [];
#mat = sparse ( ia, ja, s, NDOF, NDOF,15);
#k=zeros(4,1);xn=zeros(4,1);yn=xn;zn=xn;
#
#  for j=1:NBLOCKS
#    for  n=1:size(Tet_Dof{j},1)
#      for l=1:4
#        k(l)=Tet_Dof{j}(n,l);
#        xn(l)=x(k(l));
#        yn(l)=y(k(l));
#        zn(l)=z(k(l));
#      end
#      [~,b,c,d,v]=line3d(xn,yn,zn);
#      xa=(xn(1)+xn(2))/2.;  ya=(yn(1)+yn(2))/2.;    za=(zn(1)+zn(2))/2.;
#      xb=(xn(2)+xn(3))/2.;  yb=(yn(2)+yn(3))/2.;    zb=(zn(2)+zn(3))/2.;
#      xc=(xn(3)+xn(1))/2.;  yc=(yn(3)+yn(1))/2.;    zc=(zn(3)+zn(1))/2.;
#      xd=(xn(4)+xn(1))/2.;  yd=(yn(4)+yn(1))/2.;    zd=(zn(4)+zn(1))/2.;
#
#      xe=(xn(4)+xn(2))/2.; ye=(yn(4)+yn(2))/2.;    ze=(zn(4)+zn(2))/2.;
#      xf=(xn(4)+xn(3))/2.; yf=(yn(4)+yn(3))/2.;    zf=(zn(4)+zn(3))/2.;
#
#      ani1=(2/3.)*((yc-ya)*(zd-za)-(zc-za)*(yd-ya));
#      anj1=(2/3.)*((zc-za)*(xd-xa)-(xc-xa)*(zd-za));
#      ank1=(2/3.)*((xc-xa)*(yd-ya)-(yc-ya)*(xd-xa));
#
#      ani2=(2/3.)*((ye-ya)*(zb-za)-(ze-za)*(yb-ya));
#      anj2=(2/3.)*((ze-za)*(xb-xa)-(xe-xa)*(zb-za));
#      ank2=(2/3.)*((xe-xa)*(yb-ya)-(ye-ya)*(xb-xa));
#
#      ani3=(2/3.)*((yb-yc)*(zf-zc)-(zb-zc)*(yf-yc));
#      anj3=(2/3.)*((zb-zc)*(xf-xc)-(xb-xc)*(zf-zc));
#      ank3=(2/3.)*((xb-xc)*(yf-yc)-(yb-yc)*(xf-xc));
#
#      ani4=(2/3.)*((yf-yd)*(ze-zd)-(zf-zd)*(ye-yd));
#      anj4=(2/3.)*((zf-zd)*(xe-xd)-(xf-xd)*(ze-zd));
#      ank4=(2/3.)*((xf-xd)*(ye-yd)-(yf-yd)*(xe-xd));
#
#      for l=1:4
#            
#        anm(1)=b(l)*ani1+c(l)*anj1+d(l)*ank1;
#        anm(2)=b(l)*ani2+c(l)*anj2+d(l)*ank2;
#        anm(3)=b(l)*ani3+c(l)*anj3+d(l)*ank3;
#        anm(4)=b(l)*ani4+c(l)*anj4+d(l)*ank4;
#
#        mat(k(1),k(l))=mat(k(1),k(l))+anm(1)*CoeffDiff(j);
#        mat(k(2),k(l))=mat(k(2),k(l))+anm(2)*CoeffDiff(j);
#        mat(k(3),k(l))=mat(k(3),k(l))+anm(3)*CoeffDiff(j);
#        mat(k(4),k(l))=mat(k(4),k(l))+anm(4)*CoeffDiff(j);
# 
#      end
#   
#    end
# 
#  end
#
#return
################## Python ####################################
#___________________________________
def laplacienblocks2(x,y,z,Tet_Dof,NBLOCKS,NDOF,CoeffDiff) :
#######################  
  import numpy as np
  from line3d import line3d
  from scipy.sparse import lil_matrix
# usage: laplacienblocks2(x,y,z,Tet_Dof,NBLOCKS,NDOF,CoeffDiff)
# Built the matrix map, element by element
# Need the function linecoeftemp for the calculation of pressure and temperature derivatives
#
# In:
#	- x,y,z : Coordinates of the nodes
#	- Tet_Dof : 
#	- NBLOCKS : Number of volumes
#	- NDOF : Number of degree of freedom
#	- CoeffDiff : Diffusion coefficient
#
# Out:
#	- mat : 
##########################  
  matrice = lil_matrix((NDOF+1, NDOF+1))    # dimension de la matrice NDOF+1 pour garder les mêmes indices que la version Octave
  k=np.zeros(5, dtype=int)
  xn=np.zeros(5)
  yn=np.zeros(5)
  zn=np.zeros(5)
  anm=np.zeros(5)
#  for j in np.arange(1,NBLOCKS+1):
  for j in range(NBLOCKS):
    for  n in range(len(Tet_Dof[j])):
      for l in range(4):
#        print(j,n,l)
        k[l]=Tet_Dof[j][n,l]
        xn[l+1]=x[k[l]-1]
        yn[l+1]=y[k[l]-1]
        zn[l+1]=z[k[l]-1]
      a, b, c, d, v = line3d(xn,yn,zn)
      xa=(xn[1]+xn[2])/2.;  ya=(yn[1]+yn[2])/2.;    za=(zn[1]+zn[2])/2.;
      xb=(xn[2]+xn[3])/2.;  yb=(yn[2]+yn[3])/2.;    zb=(zn[2]+zn[3])/2.;
      xc=(xn[3]+xn[1])/2.;  yc=(yn[3]+yn[1])/2.;    zc=(zn[3]+zn[1])/2.;
      xd=(xn[4]+xn[1])/2.;  yd=(yn[4]+yn[1])/2.;    zd=(zn[4]+zn[1])/2.;
  
      xe=(xn[4]+xn[2])/2.; ye=(yn[4]+yn[2])/2.;    ze=(zn[4]+zn[2])/2.;
      xf=(xn[4]+xn[3])/2.; yf=(yn[4]+yn[3])/2.;    zf=(zn[4]+zn[3])/2.;
  
      ani1=(2/3.)*((yc-ya)*(zd-za)-(zc-za)*(yd-ya));
      anj1=(2/3.)*((zc-za)*(xd-xa)-(xc-xa)*(zd-za));
      ank1=(2/3.)*((xc-xa)*(yd-ya)-(yc-ya)*(xd-xa));
  
      ani2=(2/3.)*((ye-ya)*(zb-za)-(ze-za)*(yb-ya));
      anj2=(2/3.)*((ze-za)*(xb-xa)-(xe-xa)*(zb-za));
      ank2=(2/3.)*((xe-xa)*(yb-ya)-(ye-ya)*(xb-xa));
  
      ani3=(2/3.)*((yb-yc)*(zf-zc)-(zb-zc)*(yf-yc));
      anj3=(2/3.)*((zb-zc)*(xf-xc)-(xb-xc)*(zf-zc));
      ank3=(2/3.)*((xb-xc)*(yf-yc)-(yb-yc)*(xf-xc));
  
      ani4=(2/3.)*((yf-yd)*(ze-zd)-(zf-zd)*(ye-yd));
      anj4=(2/3.)*((zf-zd)*(xe-xd)-(xf-xd)*(ze-zd));
      ank4=(2/3.)*((xf-xd)*(ye-yd)-(yf-yd)*(xe-xd));
      k=np.roll(k,1)    # on décale le vecteur k d'1 élément vers la droite pour que les indices correspondent à Matlab
      for l in np.arange(1,5) :
        anm[1]=b[l]*ani1+c[l]*anj1+d[l]*ank1;
        anm[2]=b[l]*ani2+c[l]*anj2+d[l]*ank2;
        anm[3]=b[l]*ani3+c[l]*anj3+d[l]*ank3;
        anm[4]=b[l]*ani4+c[l]*anj4+d[l]*ank4;
  #
  
        matrice[k[1],k[l]]=matrice[k[1],k[l]]+anm[1]*CoeffDiff[j-1];
        matrice[k[2],k[l]]=matrice[k[2],k[l]]+anm[2]*CoeffDiff[j-1];
        matrice[k[3],k[l]]=matrice[k[3],k[l]]+anm[3]*CoeffDiff[j-1];
        matrice[k[4],k[l]]=matrice[k[4],k[l]]+anm[4]*CoeffDiff[j-1];
  #   je remets la matrice aux dimensions (NDOF,NDPF)
  return matrice[1:,1:].tocsr()    #   je construis une matrice lil et je retourne une matrice csr...c'est plus rapide pour la suite
#___________________________________
#___________________________________


      