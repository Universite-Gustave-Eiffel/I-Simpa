#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec  2 16:31:25 2019

@author: efoucaul
"""
#####################################################
#           bibli_MD_ef2
#     Bibliothèques pour MD
# contents :
#       laplacienblocks2(x,y,z,Tet_Dof,NBLOCKS,NDOF,CoeffDiff)
#       VolumeVCDOF(x,y,z,nn,nbel,Tet ,Tet_Dof)
#       VolumeSalle(x,y,z,nbel,kne)
#       volumeTetra(xn,yn,zn)
#       Surfaces_Salle(x, y, z, nd, kne)
#       boundary_faces(T):
#       Coef_Att_Atmos(F,H,P,K)
#       changem(z,newcode, oldcode)
#####################################################
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
## Author: Eric Foucault  <eric.foucault@univ-poitiers.fr>
## Keywords: Room acoustics, diffusion model
## Adapted-By: Ifsttar <I-Simpa@ifsttar.fr>
#
# Version Python EF IV 2020 puis VI 2024
#
#######################################################
import numpy as np
from scipy.constants import atmosphere, zero_Celsius
#######################################################
# Définition locale de constantes :
#######################################################
TOB=np.array([50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000, 20000])
#SelectedFrequency=np.arange(7)
Temperature=20.0
Humidity=50.0
tol=0.000001
maxint=200
#dt=0.001000
#duration=2.000000
#atmos_absorption_calculation = 1
###############################################################
################   changem(z,newcode, oldcode)  ##############
def changem(z,newcode, oldcode) :
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
# usage: m, cson=Coef_Att_Atmos(F)
# Calculate the coefficient of atmospheric absorption according to ISO 9613-1
# with H, P & K standard atmospheric constants
#
# In:
#	- F : Frequency (Hz)
#
# Out:
#	- m : Coefficient of atmospheric absorption (Np/m)
#	- cson : Sound speed (m/s)
# Atmospheric information
  #	- K : Air temperature (K)
  #	- P : Atmospheric pressure (Pa)
  #	- H : Relative Humidity (%)
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
  Ps=Pref*(10**C)
  hmol=H*(Ps/Pref)
# Rotational absorption
  F=np.array(F)
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
################## Python ####################################
# usage: [v]=volumeTetra(xn,yn,zn)
# Calculate the volume of a given mesh
#
# In:
#	- xn,yn,zn : Coordinates of the mesh
#
# Out:
#	- v : Volume of a given mesh
################## Python ####################################
  XA=np.asarray([xn[0] , yn[0] , zn[0]])
  XB=np.asarray([xn[1] , yn[1] , zn[1]])
  XC=np.asarray([xn[2] , yn[2] , zn[2]])
  XD=np.asarray([xn[3] , yn[3] , zn[3]])
  v=np.dot(XD-XA,np.cross((XB-XA),(XC-XA)))/6
  return v
#___________________________________
def VolumeSalle(x,y,z,nbel,kne) :
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
################## Python ####################################
  vtotal=0
  # The volume is equal to the sum of the volume of all the meshes
  for n in range(nbel) :
      ks=kne[n,:]
      v=volumeTetra(x[ks-1],y[ks-1],z[ks-1])
      vtotal=vtotal+v
  return vtotal
################## VolumeVCDOF  Octave ####################################
def VolumeVCDOF(x,y,z,nn,nbel,Tet ,Tet_Dof) :
#% c----$---1---------2---------3---------4---------5---------6---------7-c
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
#___________________________________
def laplacienblocks2(x,y,z,Tet_Dof,NBLOCKS,NDOF,CoeffDiff) :
#######################  
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
  matrice = lil_matrix((NDOF+1, NDOF+1))    # dimension of the NDOF+1 matrix to keep the same indices as the Octave version
  k=np.zeros(5, dtype=int)
  xn=np.zeros(5, dtype=float)
  yn=np.zeros(5, dtype=float)
  zn=np.zeros(5, dtype=float)
  anm=np.zeros(5, dtype=float)
  Cte = 2/3
  for j in range(NBLOCKS):
    for  n in range(len(Tet_Dof[j])):
      for l in range(4):
        k[l]=Tet_Dof[j][n,l]
        xn[l+1]=x[k[l]-1]
        yn[l+1]=y[k[l]-1]
        zn[l+1]=z[k[l]-1]
      a, b, c, d, v = line3d(xn,yn,zn)
      xa=(xn[1]+xn[2])*.5;  ya=(yn[1]+yn[2])*.5;    za=(zn[1]+zn[2])*.5;
      xb=(xn[2]+xn[3])*.5;  yb=(yn[2]+yn[3])*.5;    zb=(zn[2]+zn[3])*.5;
      xc=(xn[3]+xn[1])*.5;  yc=(yn[3]+yn[1])*.5;    zc=(zn[3]+zn[1])*.5;
      xd=(xn[4]+xn[1])*.5;  yd=(yn[4]+yn[1])*.5;    zd=(zn[4]+zn[1])*.5;
  
      xe=(xn[4]+xn[2])*.5; ye=(yn[4]+yn[2])*.5;    ze=(zn[4]+zn[2])*.5;
      xf=(xn[4]+xn[3])*.5; yf=(yn[4]+yn[3])*.5;    zf=(zn[4]+zn[3])*.5;
  
      ani1=Cte*((yc-ya)*(zd-za)-(zc-za)*(yd-ya));
      anj1=Cte*((zc-za)*(xd-xa)-(xc-xa)*(zd-za));
      ank1=Cte*((xc-xa)*(yd-ya)-(yc-ya)*(xd-xa));
  
      ani2=Cte*((ye-ya)*(zb-za)-(ze-za)*(yb-ya));
      anj2=Cte*((ze-za)*(xb-xa)-(xe-xa)*(zb-za));
      ank2=Cte*((xe-xa)*(yb-ya)-(ye-ya)*(xb-xa));
  
      ani3=Cte*((yb-yc)*(zf-zc)-(zb-zc)*(yf-yc));
      anj3=Cte*((zb-zc)*(xf-xc)-(xb-xc)*(zf-zc));
      ank3=Cte*((xb-xc)*(yf-yc)-(yb-yc)*(xf-xc));
  
      ani4=Cte*((yf-yd)*(ze-zd)-(zf-zd)*(ye-yd));
      anj4=Cte*((zf-zd)*(xe-xd)-(xf-xd)*(ze-zd));
      ank4=Cte*((xf-xd)*(ye-yd)-(yf-yd)*(xe-xd));
      k=np.roll(k,1)    #  the vector k is shift by 1 element to the right so that the indices correspond to Octave
      for l in np.arange(1,5) :
        anm[1]=b[l]*ani1+c[l]*anj1+d[l]*ank1;
        anm[2]=b[l]*ani2+c[l]*anj2+d[l]*ank2;
        anm[3]=b[l]*ani3+c[l]*anj3+d[l]*ank3;
        anm[4]=b[l]*ani4+c[l]*anj4+d[l]*ank4;
  #
  
        matrice[k[1],k[l]]=matrice[k[1],k[l]]+anm[1]*CoeffDiff[j-1][0]; #le zéro est présent pour gérer la variable de coefficient de diffusion de chaque degré de liberté
        matrice[k[2],k[l]]=matrice[k[2],k[l]]+anm[2]*CoeffDiff[j-1][0];
        matrice[k[3],k[l]]=matrice[k[3],k[l]]+anm[3]*CoeffDiff[j-1][0];
        matrice[k[4],k[l]]=matrice[k[4],k[l]]+anm[4]*CoeffDiff[j-1][0];
  #   je remets la matrice aux dimensions (NDOF,NDPF)
  return matrice[1:,1:].tocsr()    #   a lil matrix is ​​builded but a csr matrix is ​​returned because it is faster for the rest
#___________________________________
