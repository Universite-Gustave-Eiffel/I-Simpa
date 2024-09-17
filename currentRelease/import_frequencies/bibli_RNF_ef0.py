#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec  2 16:31:25 2019

@author: efoucaul
"""
#####################################################
#           bibli_RNF_ef0.py
#     Biblitothèques pour Room_Natural_Frequencies_ef0.py
#####################################
## Copyright (C) 2017 Christian Prax
## Institut PPRIME, CNRS - Universite de Poitiers  ENSMA, UPR 3346
## Ecole Nationale Superieure d'ingenieurs de Poitiers, ENSIP
##
## This file is part of Room_Natural_Frequencies_ef0.py
##
## It is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public
## License as published by the Free Software Foundation;
## either version 3 of the License, or (at your option) any
## later version.
##
## It is distributed in the hope that it will be useful,
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
# Version Python EF IV 2020 puis VI 2024
#
#######################################################
import numpy as np
#######################################################
def volumeTetra1(xn,yn,zn):
    import numpy as np
    A=np.array([xn[0],yn[0],zn[0]])
    B=np.array([xn[1],yn[1],zn[1]])
    C=np.array([xn[2],yn[2],zn[2]])
    D=np.array([xn[3],yn[3],zn[3]])
    M=np.zeros((3,3))
    M[:,0]=B-A
    M[:,1]=C-A
    M[:,2]=D-A
    v=(np.linalg.det(M))/6
    return(v)
#######################################################
def integ_pdv(x,y,z,el,nbel,nn):
    import numpy as np
    from scipy.sparse import lil_matrix
    # from volumeTetra1 import volumeTetra1
    MAT_B=lil_matrix ((nn, nn))
#Boucle sur tous les tetrahedres
    for n in range(nbel):
        ks=el[n,:] #Indices des Noeuds connectés au tétrahèdre
        xn=np.zeros(4, dtype=float)
        yn=np.zeros(4, dtype=float)
        zn=np.zeros(4, dtype=float)
        xn = x[ks-1]
        yn = y[ks-1]
        zn = z[ks-1]
        v_n=volumeTetra1(xn,yn,zn)   #Volume du tetra n
                            
        Cpt=v_n/4   # Contribution du tetra n aux 4 volumes finis
        MAT_B[ks[0]-1,ks[0]-1]=Cpt+MAT_B[ks[0]-1,ks[0]-1] 
        MAT_B[ks[1]-1,ks[1]-1]=Cpt+MAT_B[ks[1]-1,ks[1]-1]
        MAT_B[ks[2]-1,ks[2]-1]=Cpt+MAT_B[ks[2]-1,ks[2]-1]
        MAT_B[ks[3]-1,ks[3]-1]=Cpt+MAT_B[ks[3]-1,ks[3]-1]
    return MAT_B.tocsr()    #   je construis une matrice lil et je retourne une matrice csr...c'est plus rapide pour la suite
#######################################################
def line3D(xn,yn,zn):
    import numpy as np
    x21=xn[1]-xn[0]
    x31=xn[2]-xn[0]
    x41=xn[3]-xn[0]
    y21=yn[1]-yn[0]
    y31=yn[2]-yn[0]
    y41=yn[3]-yn[0]
    
    z21=zn[1]-zn[0]
    z31=zn[2]-zn[0]
    z41=zn[3]-zn[0]
          

    z43=zn[3]-zn[2]
    z14=zn[0]-zn[3]
    z32=zn[2]-zn[1]
    z24=zn[1]-zn[3]
    
    z42=zn[3]-zn[1]
    z13=zn[0]-zn[2]
    z34=zn[2]-zn[3]          
    z12=zn[0]-zn[1]
    z23=zn[1]-zn[2]

    y43=yn[3]-yn[2]
    y14=yn[0]-yn[3]
    y32=yn[2]-yn[1] 
    y24=yn[1]-yn[3]
    y42=yn[3]-yn[1] 
    y13=yn[0]-yn[2]

          
    v=(x21*(y31*z41-y41*z31)-x31*(y21*z41-y41*z21)+x41*(y21*z31-y31*z21))/6.

    a=np.zeros(np.shape(xn)[0])
    b=np.zeros(np.shape(xn)[0])
    c=np.zeros(np.shape(xn)[0])
    d=np.zeros(np.shape(xn)[0])
    
    a[0]=(xn[1]*(yn[2]*zn[3]-yn[3]*zn[2])+xn[2]*(yn[3]*zn[1]-yn[1]*zn[3])+xn[3]*(yn[1]*zn[2]-yn[2]*zn[1]))/(6.*v)
    a[1]=(-xn[2]*(yn[3]*zn[0]-yn[0]*zn[3])-xn[3]*(yn[0]*zn[2]-yn[2]*zn[0])-xn[0]*(yn[2]*zn[3]-yn[3]*zn[2]))/(6.*v)
    a[2]=(xn[3]*(yn[0]*zn[1]-yn[1]*zn[0])+xn[0]*(yn[1]*zn[3]-yn[3]*zn[1])+xn[1]*(yn[3]*zn[0]-yn[0]*zn[3]))/(6.*v)
    a[3]=(-xn[0]*(yn[1]*zn[2]-yn[2]*zn[1])-xn[1]*(yn[2]*zn[0]-yn[0]*zn[2])-xn[2]*(yn[0]*zn[1]-yn[1]*zn[0]))/(6.*v)

    b[0]=(yn[1]*z43+yn[2]*z24+yn[3]*z32)/(6.*v)
    b[1]=(-yn[2]*z14-yn[3]*z31-yn[0]*z43)/(6.*v)
    b[2]=(yn[3]*z21+yn[0]*z42+yn[1]*z14)/(6.*v)
    b[3]=(-yn[0]*z32-yn[1]*z13-yn[2]*z21)/(6.*v)

    c[0]=(xn[1]*z34+xn[2]*z42+xn[3]*z23)/(6.*v)
    c[1]=(-xn[2]*z41-xn[3]*z13-xn[0]*z34)/(6.*v)
    c[2]=(xn[3]*z12+xn[0]*z24+xn[1]*z41)/(6.*v)
    c[3]=(-xn[0]*z23-xn[1]*z31-xn[2]*z12)/(6.*v)

    d[0]=(xn[1]*y43+xn[2]*y24+xn[3]*y32)/(6.*v)
    d[1]=(-xn[2]*y14-xn[3]*y31-xn[0]*y43)/(6.*v)
    d[2]=(xn[3]*y21+xn[0]*y42+xn[1]*y14)/(6.*v)
    d[3]=(-xn[0]*y32-xn[1]*y13-xn[2]*y21)/(6.*v)
    
    return(a,b,c,d,v)
#######################################################
def INTEG_laplacien_cvfem3d(x,y,z,el,nbel,nn):
    from scipy.sparse import lil_matrix #, csr_matrix
    import numpy as np
    # from line3D1 import line3D
    A = lil_matrix ((nn, nn))
    xn=np.zeros(4, dtype=float)
    yn=np.zeros(4, dtype=float)
    zn=np.zeros(4, dtype=float)
    Cte = 2/3
    for  n in range(nbel):
        ks=el[n,:]
        xn = x[ks-1]
        yn = y[ks-1]
        zn = z[ks-1]
        [a,b,c,d,v]=line3D(xn,yn,zn)
        #calcul des centres de chaque segment appartenant au tetraedre
        xa=(xn[0]+xn[1])*.5  
        ya=(yn[0]+yn[1])*.5    
        za=(zn[0]+zn[1])*.5
        
        xb=(xn[1]+xn[2])*.5  
        yb=(yn[1]+yn[2])*.5
        zb=(zn[1]+zn[2])*.5
        
        xc=(xn[2]+xn[0])*.5
        yc=(yn[2]+yn[0])*.5
        zc=(zn[2]+zn[0])*.5
        
        xd=(xn[3]+xn[0])*.5  
        yd=(yn[3]+yn[0])*.5
        zd=(zn[3]+zn[0])*.5
        

        xe=(xn[3]+xn[1])*.5
        ye=(yn[3]+yn[1])*.5
        ze=(zn[3]+zn[1])*.5
        
        xf=(xn[3]+xn[2])*.5
        yf=(yn[3]+yn[2])*.5
        zf=(zn[3]+zn[2])*.5
        
        #
        ani1=Cte*((yc-ya)*(zd-za)-(zc-za)*(yd-ya))
        anj1=Cte*((zc-za)*(xd-xa)-(xc-xa)*(zd-za))
        ank1=Cte*((xc-xa)*(yd-ya)-(yc-ya)*(xd-xa))

        ani2=Cte*((ye-ya)*(zb-za)-(ze-za)*(yb-ya))
        anj2=Cte*((ze-za)*(xb-xa)-(xe-xa)*(zb-za))
        ank2=Cte*((xe-xa)*(yb-ya)-(ye-ya)*(xb-xa))

        ani3=Cte*((yb-yc)*(zf-zc)-(zb-zc)*(yf-yc))
        anj3=Cte*((zb-zc)*(xf-xc)-(xb-xc)*(zf-zc))
        ank3=Cte*((xb-xc)*(yf-yc)-(yb-yc)*(xf-xc))

        ani4=Cte*((yf-yd)*(ze-zd)-(zf-zd)*(ye-yd))
        anj4=Cte*((zf-zd)*(xe-xd)-(xf-xd)*(ze-zd))
        ank4=Cte*((xf-xd)*(ye-yd)-(yf-yd)*(xe-xd))
    
        
        anm=np.zeros(np.shape(xn)[0], dtype=float)
        for l in range (4):
            
            anm[0]=b[l]*ani1+c[l]*anj1+d[l]*ank1
            anm[1]=b[l]*ani2+c[l]*anj2+d[l]*ank2
            anm[2]=b[l]*ani3+c[l]*anj3+d[l]*ank3
            anm[3]=b[l]*ani4+c[l]*anj4+d[l]*ank4
            
            A[ks[0]-1,ks[l]-1]=A[ks[0]-1,ks[l]-1]+anm[0]
            A[ks[1]-1,ks[l]-1]=A[ks[1]-1,ks[l]-1]+anm[1]
            A[ks[2]-1,ks[l]-1]=A[ks[2]-1,ks[l]-1]+anm[2]
            A[ks[3]-1,ks[l]-1]=A[ks[3]-1,ks[l]-1]+anm[3]
    return A.tocsr()    #   je construis une matrice lil et je retourne une matrice csr...c'est plus rapide pour la suite
################################################################
def boundary_faces(el):
    import numpy as np    
    allF=np.zeros((4*np.shape(el)[0],3), dtype=float)
    
    allF[0:np.shape(el)[0],0]= el[:,0]
    allF[0:np.shape(el)[0],1]= el[:,1]
    allF[0:np.shape(el)[0],2]= el[:,2]
    
    allF[np.shape(el)[0]:2*np.shape(el)[0],0]= el[:,0]
    allF[np.shape(el)[0]:2*np.shape(el)[0],1]= el[:,2]
    allF[np.shape(el)[0]:2*np.shape(el)[0],2]= el[:,3]
    
    allF[2*np.shape(el)[0]:3*np.shape(el)[0],0]= el[:,0]
    allF[2*np.shape(el)[0]:3*np.shape(el)[0],1]= el[:,3]
    allF[2*np.shape(el)[0]:3*np.shape(el)[0],2]= el[:,1]
    
    allF[3*np.shape(el)[0]:4*np.shape(el)[0],0]= el[:,1]
    allF[3*np.shape(el)[0]:4*np.shape(el)[0],1]= el[:,3]
    allF[3*np.shape(el)[0]:4*np.shape(el)[0],2]= el[:,2]
    
    sortedF=np.zeros((4*np.shape(el)[0],3), dtype=float)
    for k in range(4*np.shape(el)[0]):
        sortedF[k,:]=sorted(allF[k,:])
    C,ia,ic=np.unique(sortedF,axis=0,return_index=True,return_inverse=True)
    counts=np.zeros(max(ic)+1)
    for k in range (np.shape(ic)[0]):
        counts[ic[k]]=counts[ic[k]]+1
    sorted_exteriorF=[]
    for j in range (np.shape(counts)[0]):
        if counts[j]==1:
            sorted_exteriorF.append(list(C[j,:]))
    F=[]
    for i in range(len(sortedF)):
        test=False
        if list(sortedF[i]) in sorted_exteriorF:
            test=True
            
        if test:
            F.append(allF[i,:])
    return(F)
################################################################
def position(L,a):
    k=0
    while k<len(L) and a!=L[k]:
        k+=1
    return(k)
################################################################
def VolumeSalle(x,y,z,nbel,el):
    import numpy as np
    vtotal=0
    for n in range(nbel):
        ks=el[n,:]
        xn=np.zeros(len(ks), dtype=float)
        yn=np.zeros(len(ks), dtype=float)
        zn=np.zeros(len(ks), dtype=float)
        xn = x[ks-1]
        yn = y[ks-1]
        zn = z[ks-1]
        
        v=volumeTetra1(xn,yn,zn)
       
        vtotal=vtotal+v
    return(vtotal)
################################################################
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