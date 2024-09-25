# -*- coding: utf-8 -*-
"""
Created on Wed Aug 12 17:02:02 2020

@author: C.PRAX, christian.prax@univ-poitiers .fr
University of Poitiers - Pprime Institute
Bâtiment B17, 6 rue Marcel Doré
TSA 41105
86073 POITIERS Cedex 9 - France
"""
############################################
# Modifs eric.foucault@univ-poitiers.fr
#   VI 2024
############################################
from bibli_RNF_ef0 import *
#import gmsh
def main(el,XYZ, coreconf):
    import numpy as np
    from scipy.sparse.linalg import eigs
    import time
    from operator import itemgetter
    c0=Coef_Att_Atmos(coreconf.const["temperature_kelvin"])#Sound Propagation Velocity
    #============================================================================
    t0 = time.time()
    ChoixNbmodes=20 #Choix du nombre de réponses modales à généraliser en l'ajoutant aux options de I-SIMPA
    ## READING MESHING AND NODES
    print('Read meshes and nodes')
    nbre_salles=np.size(np.unique(el[:,4])) # Number of volumes (i.e. rooms)
    NBLOCKS=nbre_salles # Number of blocks of the meshing
    print('Number of elements :', len(el))
    print('Number of points : ',len(XYZ))

    DOF_Sig=np.zeros(NBLOCKS, dtype=float)
    ndf=[]     # 
    Tet=[]     # 
    pts=[]     # 
    Node_DOF_Sig=[]     # 
    nnB=[]
    for nb in range(1,NBLOCKS+1): 
      val=[el[i,:] for i in np.nonzero(el[:,4]==nb)]
      Tet.extend(val)
      val=[np.unique([el[i,:4] for i in np.nonzero(el[:,4]==nb)])]
      pts.extend(val)
      NodeDOFo=np.zeros((len(pts[nb-1]),len(DOF_Sig)+1),dtype=int) 
      NodeDOFo[:,0]=pts[nb-1]
      NodeDOFo[:,nb]=1
      Node_DOF_Sig.append(NodeDOFo)
      nnB.append(len( pts[nb-1]))
      ndf.append(Node_DOF_Sig[nb-1])
    NDOF=sum([len(ndf[x]) for x in range(NBLOCKS)])
    val=ndf[0]
    for k in range(1,len(ndf)):
        val=np.concatenate((val,ndf[k]),axis=0)
    sortedndf=sorted(np.reshape(val,(len(val),nbre_salles+1)), key=itemgetter(0))
    sortedndf=np.asarray(sortedndf)
    nn=len(XYZ) # Number of nodes (that is different of the number of DOF)
    print('Number of nodes: ',NDOF)
    #################
    val =[np.where(sortedndf[:,0] == ij)  for ij in range(nn+1) ]
    val=np.array(val, dtype=object)
    val=val.flatten()
    ##%## TABLE OF CONNECTION BETWEEN THE NODE INDEX and the DOF index
    N_F=[]
    for ic in range(NBLOCKS) :
      lig=np.nonzero(sortedndf[:,ic+1])
      lig=np.array(lig)
      val=np.c_[sortedndf[lig,0].transpose(), (lig+1).transpose()]
      val=val.reshape(len(val),2)
      N_F.append(val)
    #########    Tet_Dof[i] définis sur les degrés de liberté
    Tet_Dof=[]
    for i in range(NBLOCKS):
      z=Tet[i][:,:4]
      oldcode=N_F[i][:,0]
      newcode=N_F[i][:,1]
      z=changem(z,newcode, oldcode)
      Tet_Dof.append(np.c_[z,Tet[i][:,4]])
    
    x=XYZ[:,0]
    y=XYZ[:,1]
    z=XYZ[:,2]
    xdf=x.take(sortedndf[:,0]-1)
    ydf=y.take(sortedndf[:,0]-1)
    zdf=z.take(sortedndf[:,0]-1)
    XYZ2=[xdf, ydf, zdf]
    XYZ3=np.transpose(XYZ2)
    XYZ3=np.array(XYZ3)
    XYZ=np.asarray(XYZ)
    x=np.asarray(x)
    y=np.asarray(y)
    z=np.asarray(z)
    el=np.asarray(el)
    
    nn=np.shape(x)[0] # nombre de noeuds
    nbel=np.shape(el)[0] # nombre d'elements Tetraedres
    
    
    #RECHERCHE LES FACES FRONTIERES
    el2D = boundary_faces(el)
    # # get boundary vertices
    
    #      [A]{p}=lambda[B]{p}
    #lambda sont les valeurs propres et {p} les vecteurs prores associés
    
    A=INTEG_laplacien_cvfem3d(x,y,z,el,nbel,nn)
    B= integ_pdv(x,y,z,el,nbel,nn)
     
    
    #--------Modif code 08/07/2024------------
    print("Calcul de la réponse modale")
    try: # On cherche les valeurs propres d'une matrice. Or la matrice est de la taille du nombre de points donc si moins de 20 points, on a un pb
        NumberEV=ChoixNbmodes# Number of Eigen Values
        V=eigs(A,NumberEV,M=B,sigma=None,which='SM')# Seules les NumberEV plus petites valeurs propres sont déterminées
    except:
        NumberEV=len(XYZ)-2
        V=eigs(A,NumberEV,M=B,sigma=None,which='SM')# Seules les NumberEV plus petites valeurs propres sont déterminées
        print("Trop peu de points. Calcul des ", NumberEV, "modes propres.")
    Vecps=V[1].real # Vecteurs propres
    Vecps=np.array(Vecps)
    Vecps=np.transpose(Vecps)
    Vecps= np.absolute(Vecps) # mise en valeur absolue pour un affichage correct en dB
    Valps=V[0].real # Valeurs propres correspondant à (kd)^2
    WaveNumbers = np.zeros(len(Valps), dtype=float)# kd
    Naturalfrequency = np.zeros(len(Valps), dtype=float)# kd
    Cte = c0/(2*np.pi)
    for k in range(np.shape(WaveNumbers)[0]):
        WaveNumbers[k]=np.sqrt( abs(Valps[k]))# La valeur propre est le carré du nombre d'onde
        Naturalfrequency[k]=Cte*np.sqrt( abs(Valps[k]))
    SortedWaveNumbers=sorted(WaveNumbers)
    I=np.zeros(len(SortedWaveNumbers), dtype=float)
    for k in range(len(SortedWaveNumbers)):
        I[k]=position( SortedWaveNumbers, WaveNumbers[k])
         
    liste = np.zeros(NumberEV*2, dtype=float)
    listef = np.zeros(NumberEV*2, dtype=float)
    print('                <============================================')
    SortedVecps=np.zeros(np.shape(Vecps))
    for nomb in range(NumberEV):    # Tri dans l'ordre croissant des réponses modales et des vecteurs propres
        pos=int(I[nomb])
        liste[pos]=I[nomb]  
        liste[NumberEV+pos]=WaveNumbers[nomb]
        listef[pos]=I[nomb]  
        listef[NumberEV+pos]=Naturalfrequency[nomb]
        SortedVecps[pos]=Vecps[nomb]
    
    
    t2=time.time()
    duree=t2-t0
    T=[0,0,0]
    while duree-60>0:
        if T[1]==60:
            T[1]=0
            T[0]=T[0]+1
        else:
            T[1]=T[1]+1
        duree=duree-60
    T[2]=duree
    print('INDICES DES PREMIERES VALEURS PROPRES')
    for nomb in range(0,NumberEV):
        print('indice=',liste[nomb], ' Valeur Propre ', liste[nomb+NumberEV])
    print('_________________________________________________')
    for nomb in range(0,NumberEV):
        print('indice=',listef[nomb], ' Freq Propre ', listef[nomb+NumberEV])
    print('_________________________________________________')
    print('durée du programme=',T[0],'h',T[1],'min',T[2],'s')
    # ####################################################################
    return XYZ3, SortedVecps, Tet_Dof, NumberEV, listef