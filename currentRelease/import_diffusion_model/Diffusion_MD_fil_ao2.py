#! /usr/bin/env python3.6
# -*- coding:Utf8 -*-
#############################################################################
# Pg Python adaptation de mainDiffusion.m (Room Acoustics Diffusion Model). #
#                                                                           #
## Author: Christian Prax <Christian.prax@univ-poitiers.fr>
## Keywords: Room acoustics, diffusion mode                                 #
#
#  portage Python 3.6 E.Foucault  XII 2019 --> VI 2020
# version paralèle pour le calcul instationnaire      #
# décomposition LU
#                                                     #
#############################################################################
# Importation de fonctions externes :
from bibli_MD_ef1 import *   # bibliothhèque EF pour MD
def main(el,XYZ,el2Dtypd,Abs_Mater,i_nT,Wall_TL,Srce_sonore_I,coreConf):
    import os,sys
    import numpy as np
    from copy import deepcopy
    from scipy.sparse import csr_matrix, csc_matrix, lil_matrix, linalg  #, csc_matrix
    from operator import itemgetter
    import time
    from threading import Thread
    ############################################################################
    ###################### début programme ################################
    #
    start_time = time.time()
    ## CALCULATE ADDITIONNAL PARAMETERS FROM LOADED PARAMETERS
    # Frequency bands
    Frequency=coreConf.const["frequencies"]
    # Selected frequency bands
    SelectedFrequency=[]
    for f in Frequency:
        for k in range(len(TOB)):
            if f==TOB[k]:
                SelectedFrequency.append(k)
    # nonSelected frequency bands
    NonSelectedFrequency=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]
    for f in SelectedFrequency:
        for i in range(len(NonSelectedFrequency)-1,-1,-1):
            if f==NonSelectedFrequency[i]:
                NonSelectedFrequency=np.delete(NonSelectedFrequency,i,0)
    NOct=len(Frequency)
    NNOct=np.arange(NOct)
    ## Atmospheric absorption and sound speed values
    m,c0=Coef_Att_Atmos(Frequency,Humidity,atmosphere,273.15+Temperature)
    #print(m,'~',c0)
    if atmos_absorption_calculation==0:
    # No amtospheric absorption coefficient
      mc=np.zeros(NOct, dtype=float)
    else:
    # Use atmospheric absorption coefficient
      mc=m*c0
    ## READING MESHING AND NODES
    #display('Read meshes and nodes')
    #domaine='scene'; # Meshing file name
    #el = load(strcat(domaine,'_elements.txt'));
    #nbre_salles=numel(unique(el(:,5))); # Number of volumes (i.e. rooms)
    #NBLOCKS=nbre_salles; # Number of blocks of the meshing
    #idBloc=unique(el(:,5)); # Block identification
    #disp('Number of volumes '), disp(nbre_salles)
    print('Read meshes and nodes')
    domaine='scene' # Meshing file name
    #el=np.loadtxt(domaine+'_elements.txt',dtype=int)			# charge le fichier de data
    nbre_salles=np.size(np.unique(el[:,4])) # Number of volumes (i.e. rooms)
    NBLOCKS=nbre_salles # Number of blocks of the meshing
    idBloc=np.unique(el[:,4]) # Block identification
    print('Number of volumes ',nbre_salles)
    
    ## TABLE OF DOF SIGNATURES
    # DOF_Sig <--Table des signatures pour les DOF
    # Example NBLOCKS= 3, include 1 0 1 for a node associate to the volume 1 et volume 3
    print('Create table of signatures')
    #DOF_Sig=np.arange(NBLOCKS)                   # pourquoi créer à priori ce tableau de zéros ??
    #DOF_Sig=DOF_Sig*0.
    DOF_Sig=np.zeros(NBLOCKS, dtype=float)
    ndf=[]     # 
    Tet=[]     # 
    pts=[]     # 
    Node_DOF_Sig=[]     # 
    #nnB=['nnB']
    nnB=[]
    for nb in range(1,NBLOCKS+1):     # attention...pb des indices qui commencent à 1 et pas à zéro !!!!
      val=[el[i,:] for i in np.nonzero(el[:,4]==nb)]
      Tet.extend(val)
    #  Tet=num2cell(val)
      val=[np.unique([el[i,:4] for i in np.nonzero(el[:,4]==nb)])]
      pts.extend(val)
    #  NodeDOFo=zeros(size(pts{i},1),size( DOF_Sig,2)+1);
      NodeDOFo=np.zeros((len(pts[nb-1]),len(DOF_Sig)+1),dtype=int)  # Pourquoi le remplir de 0 d'abord ????
      #  NodeDOFo(:,1)=pts{i};NodeDOFo(:,i+1)=1;
      NodeDOFo[:,0]=pts[nb-1]
      NodeDOFo[:,nb]=1
    #  Node_DOF_Sig{i}=NodeDOFo;
      Node_DOF_Sig.append(NodeDOFo)
    #  nnB(i)=size( pts{i}, 1);
      nnB.append(len( pts[nb-1]))
    #  ndf=[ndf ;Node_DOF_Sig{i}];
      ndf.append(Node_DOF_Sig[nb-1])
    #NDOF=size(ndf,1);
    #NDOF=(len(ndf[1])+(len(ndf)-1))
    NDOF=sum([len(ndf[x]) for x in range(NBLOCKS)])
    ##[~,I] = sort(ndf(:,1));
    ##sortedndf=ndf(I,:);
    ##clear I;
    
    #-----------MODIFS CODE 25/06/2024--------------- voir avec un try
    val=ndf[0]
    for k in range(1,len(ndf)):
        val=np.concatenate((val,ndf[k]),axis=0)
    sortedndf=sorted(np.reshape(val,(len(val),nbre_salles+1)), key=itemgetter(0))
    #-----------FIN MODIFS CODE 25/06/2024-----------
    
    sortedndf=np.asarray(sortedndf)
    ##XYZ= load(strcat(domaine,'_nodes.txt'));
    #XYZ=np.loadtxt(domaine+'_nodes.txt',dtype=float)			# charge le fichier de data
    ##nn=size(XYZ,1); # Number of nodes (that is different of the nimber of DOF)
    nn=len(XYZ) # Number of nodes (that is different of the nimber of DOF)
    ##NDOF=size(ndf,1);
    #NDOF=ndf[0]*len(ndf[1])-1   # np.shape(ndf[1])
    ##disp('Number of nodes: '), disp(NDOF)
    print('Number of nodes: ',NDOF)
    ###
    #################
    ##NFS=zeros(nn,NBLOCKS+1);% NFS= NODE FREEDOM SIGNATURE
    #NFS=np.zeros((nn,NBLOCKS+1),dtype=float) # NFS= NODE FREEDOM SIGNATURE
    ##for ij=1:nn
    ##%irow=find(sortedndf(:,1)==ij);
    ##  [irow,IL,~]=find(sortedndf(:,1)==ij); % A quoi sert IL ??
    ##  cs=sum (sortedndf(irow,2:end),1); % c'est une somme sur les colonnes ?
    ##  NFS(ij,1:NBLOCKS+1)=[ ij  cs];% NFS= NODE FREEDOM SIGNATURE: example 117 1 0 1
    ##end
    val =[np.where(sortedndf[:,0] == ij)  for ij in range(nn+1) ]
    val=np.array(val, dtype=object)
    val=val.flatten()
    cs=[np.sum(sortedndf[val[ij],1:],axis=0) for ij in range(nn+1)]
    cs=np.array(cs)
    #print('cs= ',cs)
    col=np.arange(len(cs))
    NFS=np.append(col[:,np.newaxis], cs, 1)
    # ou NFS=np.concatenate((col[:,np.newaxis],cs),axis=1)
    #
    ##%## TABLE OF CONNECTION BETWEEN THE NODE INDEX and the DOF index
    ##%for ic=1:NBLOCKS
    ##%  [lig, col] = find (sortedndf(:,ic+1)); 
    ##%  N_F{ic}=[sortedndf(lig,1) lig]; # Example N_F{1} corresponds to the DOF node in Volume 1
    ##%end
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
    ##%## NODES AND DOF COORDINATES
    x=XYZ[:,0]
    y=XYZ[:,1]
    z=XYZ[:,2] # Coordinates of each node
    xdf=x.take(sortedndf[:,0]-1)
    ydf=y.take(sortedndf[:,0]-1)
    zdf=z.take(sortedndf[:,0]-1)
    XYZ2=[xdf, ydf, zdf] # transmis à affichepatchBlocks.m
    XYZ3=np.transpose(XYZ2)
    XYZ3=np.array(XYZ3)
    ### READING MESHES ON BOUNDARIES
    print('Identifying boundaries')
    #el2Dtypd= np.loadtxt(domaine+'_faces.txt',dtype=int)			# charge le fichier de data
    nel2D0=el2Dtypd.shape[0]
    FT =np.sort(el2Dtypd[:,0:3])
    FTu, l, i, c =np.unique(FT, axis=0, return_index=True, return_counts=True, return_inverse=True)    # le l retourné n'est pas le même que Octave !!!
    ## Attention !!!
    ## le l retourné n'est pas le même que Octave !!!
    ## en effet la fct unique renvoie l'indice de la 1ère occurence rencontrée
    ## alors que unique dans Octave renvoie la dernière !!!
    n=0
    for m in c:
      if m!=1:
        a=l[n]
        b=np.where(np.all(FT==FT[a],axis=1))[0][-1]
    #    print(n,' l=',a,' last',b)
        l[n]=b
      n+=1
    el2Dtyp=el2Dtypd[l,:]
    nel2Dt=len(el2Dtyp)
    ll=np.arange(nel2D0)
    duplicate_l = np.setdiff1d(ll, l, assume_unique=True)
    ### IDENTIFYING MESHES WITH TRANSMISSION
    el2D_Trsm=el2Dtypd[duplicate_l,:]
    #### INFORMATION ON MESHS ON BOUNDARIES
    el2D=el2Dtyp[:,0:3] # Meshes onboundaries
    n_materiau=el2Dtyp[:,3] # Identification of boundary material
    nel2D=len(el2D) # Number of meshes on boundaries
    #################
    ### SEARCH BONDARY FACES OF EACH BLOCK
    sortedel2D= np.sort(el2D, axis=1) # Sort on each line....à quoi sert IS ???
    nel2D=np.zeros((NBLOCKS,1))
    Surf=np.zeros((NBLOCKS,1))
    VOLUME=np.zeros((NBLOCKS,1))
    Lambda=np.zeros((NBLOCKS,1))  # Attention lambda est un mot réservé en Python => Lambda
    el2di=[]
    for i in range(NBLOCKS):
      ef, sortedel2di = boundary_faces(Tet[i]); # Meshes of Volume i
      a= np.nonzero(np.all(sortedel2di == sortedel2D[:,np.newaxis], axis=2))
      el2di.append(el2Dtyp[a[0],:])
    ############################################  
    #### ????
    V_VC=np.zeros(np.sum(nnB))
    AireTr=[NBLOCKS+1]
    VOLUME=[]
    V_VCi=[]
    for i in range(NBLOCKS):
      nel2D[i]=len( el2di[i] )# Number of meshes on boundaries ## max inutile : python renvoie directement le nb de lignes
      Surf, aire= Surfaces_Salle(x,y,z,nel2D[i][0],el2di[i])  # Surface des parois+de chaque elt
      AireTr.append(aire)
    #  print('SurfSalle=',Surf,' AireTr=',AireTr[-1])
      VolSalle= VolumeSalle(x,y,z,len( Tet[i]), Tet[i]) ; # Size of volume i (m3)
      VOLUME.append(VolSalle)
    #  print('VOLUME= {0:06.2f} m^3'.format(VOLUME[i]))
      V_VCi=VolumeVCDOF(x,y,z,sum(nnB),len(Tet[i]),Tet[i], Tet_Dof[i]); # Table of the volumes of control
      V_VC+=V_VCi
    #  print('V_VC=',V_VC[:20])
    ############################################
    ### PARAMETERS OF THE ROOM ACOUSTICS DIFFUSION MODEL
    Lambda=4*(VOLUME/Surf)    # Mean Free path (m)
    CoeffDiff=Lambda*c0/3    # Diffusion coefficient TODO: Fix the diffusion coefficient value for mixed surface reflection and atmospheric absorption
    ###########################################
    ## GLOBAL MATRIX ASSEMBLAGE
    print('Global matrix assemblage')
    # Tetra Assemblage of Global mat for diffusion operator
    mat=laplacienblocks2(xdf,ydf,zdf,Tet_Dof,NBLOCKS,NDOF,CoeffDiff)   # mat sparse CSR matrix
    #print('matrice creuse mat=',type(mat))
    ##mat=-mat+mc*np.diag(V_VC);
    ####### Affichage ######################
    ##fig,ax00 = plt.subplots()   #  aperçu de la matrice creuse
    ##plt.spy(mat,precision=0.01, markersize=1)    # idem spy(mat) avec Octave
    ##fig.tight_layout()
    ##plt.show()
    ###
    ## Affichage du temps d execution
    #print('Temps d \'execution ={0:06.2f}s'.format(time.time() - start_time))
    #############################################
    ##### DIFFUSION EQUATION LEFT MEMBER
    mat_Toct=[]
    for N_Toct in range(NOct):
      mat_Toct.append(csr_matrix(mc[N_Toct]*np.diag(V_VC))-mat)
    #  print('mat_Toct matrix_csr ?',isspmatrix_csr(mat_Toct[N_Toct]))
    mat_Toct0= deepcopy(mat_Toct)
    ############################################
    #### ACOUSTIC ABSORPTION
    ################# Début AbsorptionConstructionMatrix ###########################
    ### Load the absorption coefficient of each material in the corresponding file
    ### iAbs abs_50 abs_63 abs_80 .. abs_16000 abs_20000
    #Abs_Mater = np.loadtxt(domaine+'_materials_absorption.txt',dtype=float) # Load the absorbing material
    iAM = np.where(np.sum(Abs_Mater[:,1:],axis=1)!=0)[0] # iAbs: row of absorbing material index
    iAbs = Abs_Mater[iAM,0]          # iAbs: absorbing material index
    abs_prop=Abs_Mater[iAM,:]    
    abs_coef=abs_prop[:,SelectedFrequency]
    el2di_Dof=deepcopy(el2di)
    SurFaceA=[]
    AireFace=[]
    DofAbs=[]
    EquivAbsAreaBlock=[]
    for n in range( NOct) :
      mat_Toct[n]=mat_Toct[n].tolil()
    for iB in range(NBLOCKS) :
    #  el2di_Dof[iB][:,1:3]=changem(el2di[iB][:,1:3], N_F[iB][:,2], N_F[iB][:,1])
      zz=deepcopy(el2di)[iB][:,0:3]    # Attention il existe déjà un z lu dans le fichier de data donc je définis zz
      oldcode= N_F[iB][:,0]
      newcode= N_F[iB][:,1]
      el2di_Dof[iB][:,0:3]=changem(zz,newcode, oldcode)
      zz=deepcopy(el2di_Dof)[iB][:,3]
      oldcode =iAbs
      newcode =np.arange(len( iAbs))
      el2di_Dof[iB][:,3]=changem(zz,newcode, oldcode ) 
    #
      surf, aire = Surfaces_Salle(xdf,ydf,zdf,len(el2di_Dof[iB]),el2di_Dof[iB])
      SurFaceA.append(surf)
      AireFace.append(aire)
    #  print('iB=',iB,'surf=',surf,'aire=',aire)
    ##  
      DofAbs.append(np.unique(el2di_Dof[iB][:,0:3]))
      EquivAbsArea=[] 
      for it in range(len(DofAbs[iB])) :
        ligne, col =np.where((el2di_Dof[iB][:,:3]-DofAbs[iB][it]) == 0)
    #    print('ligne=',lc[0], 'col=',lc[1])
        val= np.sum( AireFace[iB][ligne]*abs_coef[ el2di_Dof[iB][ligne,3],:].transpose(), axis=1)/3/4 # Sabine's based absorption
        EquivAbsArea.append(val)
        idfFree= DofAbs[iB][it]
        idfFree += -1   # le tableau commence à 0
        for nm in range(NOct) :
          mat_Toct[nm][ idfFree,idfFree] += EquivAbsArea[it][nm]*c0
      EquivAbsAreaBlock.append(np.sum(EquivAbsArea,axis=0))
    ####  on trouve exactement la même matrice que Matlab à ce stade
    #print('iB=',iB,'',EquivAbsAreaBlock)
    #print(mat_Toct[0][:,:6])
    ### je trouve exactement la même chose pour les 2 volumes...il faudrait comparer de près avec le pg Matlab !!!
    ### je ne comprends pas pourquoi !!!
    #  print(EquivAbsAreaBlock)
    ################# Fin AbsorptionConstructionMatrix ###########################
    ###%%## ACOUSTIC TRANSMISSION
    #
    #if NBLOCKS>1        #### Attention !!!! il faudra remettre la condition ici !!!!
    #
    ### % TransmissionConstructionMatrixVF;%Navarro;
    ###  TransmissionConstructionMatrix4;%Navarro;%Navarro;     # cf ci-dessous, explicitement dans le pg prinicipal
    ###  % TransmissionConstructionMatrix3Navarro;
    ###end
    ################## Début TransmissionConstructionMatrix4;%Navarro;%Navarro;##################
    ##### CALCULATE FACES TRANSMISSION el2diTrsm{ib}
    #### i_nT= Index table of all node with transmission
    ###
    #### Load the transmission coefficient of each material in the corresponding file
    #### iAbs TL_50 TL_63 TL_80 .. TL_16000 TL_20000
    
    #-----------MODIFS CODE 26/06/2024---------------
    # if nbre_salles!=1:
    #      i_nT=np.loadtxt(domaine+'_shared_vertices.txt',dtype=float)
    #      i_nT=np.sort(i_nT)
    # else:
    #      i_nT=np.array([], dtype=float)
    #-----------FIN MODIFS CODE 26/06/2024-----------
    
    #Wall_TL=np.loadtxt(domaine+'_materials_transmission.txt',dtype=float) # Load material
    iTrsm=Wall_TL[np.where(np.sum(Wall_TL[:,1:],axis=1)!=0)[0],0] # iTrsm: index of Material with transmission
    print("Wall_TL : ", Wall_TL)
    print("SelectedFrequency[np.newaxis,:] : ", SelectedFrequency)
    l=np.where(np.sum(Wall_TL[:,1:],axis=1)!=0)[0]
    print(l)
    c=[0]+[k+1 for k in NonSelectedFrequency] #suppression des fréquences non choisies et de la première valeur 16/07/2024
    #print("l[:,np.newaxis] : ", l[:,np.newaxis])
    Wall_TLnp=np.array(Wall_TL)
    TL=np.delete(Wall_TLnp,c,1) # Transmission Loss idMat &values
    print("TL = ", TL)
    TAU=10.**(-TL[:,:NOct]/10)
    Surfaceporte=0
    el2diTrsm=[]
    for ib in range(NBLOCKS) :
        el2diTrsm.append(el2di[ib][np.in1d( el2di[ib][:,-1], iTrsm)])
    #print(el2di[ib][np.in1d(el2di[ib][:,-1], iTrsm)])    # idem Octave
    ###############
    
    #--------------MODIFS 02/07/2024---------------
    it=0
    FacesTij=[]
    FacesTi_Dof=[]
    FacesTj_Dof=[]
    for i in range(NBLOCKS-1) :
      el2Di=el2diTrsm[i]
      forme=el2Di.shape
      nbParoiTransm=0
      for j in np.arange(i+1,NBLOCKS) :
        el2Dj=el2diTrsm[j]
        #Facescommunesij=el2Di(ismember(el2Di,el2Dj,'rows'),:);
        #Facescommunesij=el2Di[np.isin( el2Di, el2Dj)].reshape(forme)  # je ne comprends pas pourquoi je suis obligé de faire ça...
        
        #changement de stratégie pour la récupération des surfaces de contact
        Facescommunesij=np.array([])
        isin=np.isin( el2Di, el2Dj) #récupération de la matrice des valeurs présente dans la salle I qui sont aussi présentes dans la salle J
        init=0      #valeur permettant de gérer les premières étapes de construction de la matrice
        for k in range(len(isin)):
            if np.sum(isin[k,0:3])==3: #si les 3 valeurs de la faces sont bien dans les 2 salles, alors elles sont communes
                if init==0: #utiliser un reshape
                 			Facescommunesij=np.array(el2Di[k])
                 			init=1
                elif init==1:
                 			Facescommunesij=np.append([Facescommunesij],[el2Di[k]], axis=0)
                 			init=2
                else:
                 			Facescommunesij=np.append(Facescommunesij,[el2Di[k]], axis=0)
        # #fin changement de stratégie pour la récupération des surfaces de contact
        
        cond=Facescommunesij.size==0
        if not(cond) :
          FacesTij.append(Facescommunesij)
          FacesTij[it][:,3]= changem(FacesTij[it][:,3],np.arange(1,len(iTrsm)+1),iTrsm )     
          FacesTi_Dof.append( changem(FacesTij[it][:,0:3], N_F[i][:,1], N_F[i][:,0]))    #
          FacesTi_Dof[it]=np.c_[FacesTi_Dof[it][:,:],FacesTij[it][:,3]]   # ajout d'une colonne en dernière position
    #      print('FacesTi_Dof=',FacesTi_Dof[it][:10,:])
          FacesTj_Dof.append( changem(FacesTij[it][:,0:3], N_F[j][:,1], N_F[j][:,0]))
          FacesTj_Dof[it]=np.c_[FacesTj_Dof[it][:,:],FacesTij[it][:,3]]   # ajout d'une colonne en dernière position
    #      print('FacesTj_Dof=',FacesTj_Dof[it][:10,:])
          it+=1
    NbTransmWall=it          #  idem Octave
    
    #--------------FIN MODIFS 02/07/2024-----------
    
    ##
    #### EXCHANGE SURFACES (sum of the mesh face surfaces associated with each point, divided by 3
    SurFaceT=[]
    AireFace=[]
    ptsTrsm=[]
    Dofi=[]
    Dofj=[]
    for i in range(NbTransmWall) :
      surf, aire= Surfaces_Salle(x,y,z,len(FacesTij[i]),FacesTij[i])    # j'ai l'impression que l'on a déjà calculé l'aire
      SurFaceT.append(surf)
      AireFace.append(aire)
      ptsTrsm.append(np.unique(FacesTij[i][:,0:3]))
      Dofi.append(np.unique(FacesTi_Dof[i][:,0:3]))
      Dofj.append(np.unique(FacesTj_Dof[i][:,0:3]))
      portei=[]
      portej=[]
    #
      Surf_ech=[]  
      PondEchSurf=[]
      for it in range(len(ptsTrsm[i])) :
        ligne, col  = np.where((FacesTij[i][:,0:3]-ptsTrsm[i][it])==0)
    #    print('ligne=',ligne)
    #    print('col=',col)
        Surf_ech.append(np.sum(AireFace[i][ligne])/3)  #  idem Octave
    #% TEST +++++++++++++++++++++++++++++++++++++++++++++++++++++++    
    #%if it ==7 
    #% it
    #%   AireFace{i}(ligne)
    #%    FacesTij{i}(ligne,4)
    #%    TAU(FacesTij{i}(ligne,4),1:end)
    #% 'TEST +++++++++++++++++++++++++++++++++++++++++++++++++++++++ '   
    #%    end
    #%       PondEchSurf (it,1:NOct) = sum(   AireFace{i}(ligne))/3;
    
    #   PondEchSurf (it,1:NOct) = sum( AireFace{i}(ligne).*  TAU(FacesTij{i}(ligne,4),1:end),1  )/3;
        PondEchSurf.append(np.sum( AireFace[i][ligne,np.newaxis] * TAU[FacesTij[i][ligne,3]-1,:], axis=0  )/3)
    ##############  PondEchSurf idem Octave ########
    		## Contribution of the transmission
        for nm in range(NOct) :
    #      % Pour les parois en transmission totale
    #     % TAU(FacesTij{i}(ligne,4),nm)==1
    #% iter=0;
          condporte = False
    ##############  testporte ####### procedure Octave
          for ilg in range(len(ligne)) :
            if (TAU[FacesTij[i][ligne[ilg],3]-1,nm]==1.) :
              condporte = True
    #############################################
          if condporte :
            if nm==4 :
              portej.append(Dofj[i][it])
              portei.append(Dofi[i][it])
    #######################  idem Matlab ######################
    #  print('i=',i,'it=',it,'nm=',nm,portej)
    #%  %Tauf=TAU(FacesTij{i}(ligne,4),nm)
    #%  %ligne
    #          np.concatenate((portej, Dofj[i][it]))
    #      portei=[portei Dofi{i}(it)];
    #%%TAU(FacesTij{i}(ligne,4),nm)
    #%  % Dofi{i}(it)
    #%  %   ligne1=ligne
    #%    % [nm it Dofi{i}(it) Dofj{i}(it)]
    #%    %XYZ2(Dofi{i}(it),: )
    #%     %plot3(XYZ2(Dofi{i}(it),1 ), XYZ2(Dofi{i}(it),2 ),XYZ2(Dofi{i}(it),3 ),'vk'), hold on,xlabel ('x')
    #%   %  Surfaceporte=Surfaceporte+PondEchSurf (it,nm) ;
    #%  % AireFace{i}(ligne)
    #%    surf_porte
    #%      %    Surfaceporte=Surfaceporte+sum( AireFace{i}(ligne))/3;
    # end
    #     surf_porte
    ###############  surf_porte ####### procedure Octave
            SurfaceAbs=0
            for ilg in np.arange(0,len(ligne)) :
              if (TAU[FacesTij[i][ligne[ilg],3]-1,nm]==1):
                SurfaceAbs+=AireFace[i][ligne[ilg]]/3*c0/4
    #######################  idem Matlab ######################
    ##     %pause
    ##%       nm1=nm
    ##       %TAU(FacesTij{i}(ligne,4),nm)==1
    ##        %	[lnz,~] = find(mat_Toct{nm}(Dofi{i}(it), :));
    ##%       mat_Toct{nm}(Dofj{i}(it), lnz)=mat_Toct{nm}(Dofj{i}(it), lnz)+mat_Toct{nm}(Dofi{i}(it), lnz);
    ##%        mat_Toct{nm}(Dofj{i}(it), lnz)=mat_Toct{nm}(Dofi{i}(it), lnz);
    ##%        mat_Toct{nm}(Dofj{i}(it), Dofj{i}(it))=-mat(Dofj{i}(it), Dofj{i}(it))-mat(Dofi{i}(it), Dofi{i}(it));% sans absorption
    ##%        mat_Toct{nm}(Dofi{i}(it), :)=0;
    ##%        mat_Toct{nm}(Dofi{i}(it), Dofi{i}(it))=1;  mat_Toct{nm}(Dofi{i}(it), Dofj{i}(it))=-1;
    ##
    ##%mat_Toct{nm}(Dofj{i}(it), :)=-mat(Dofj{i}(it), :)-mat(Dofi{i}(it), :);% sans absorption
    ##%SurfaceAbs=sum(AireFace{i}(ligne))/3*c0/4;
    ##%mat_Toct{nm}(Dofj{i}(it), :)=mat_Toct{nm}(Dofj{i}(it), :)+mat_Toct{nm}(Dofi{i}(it), :);% sans absorption
    ##%mat_Toct{nm}(Dofi{i}(it), :)=mat_Toct{nm}(Dofi{i}(it), :)+mat_Toct{nm}(Dofj{i}(it), :);% sans absorption
    ##%mat_Toct{nm}(Dofi{i}(it), :)=0;% sans absorption
    ##%mat_Toct{nm}(Dofi{i}(it),Dofi{i}(it))=1e8;  mat_Toct{nm}(Dofi{i}(it),Dofj{i}(it))=-1e8;
    ##
    ##%mat_Toct{nm}(Dofi{i}(it), Dofi{i}(it))=mat_Toct{nm}(Dofi{i}(it), Dofi{i}(it))-2*SurfaceAbs;
    ##
    ##%mat_Toct{nm}(Dofj{i}(it), Dofj{i}(it))=mat_Toct{nm}(Dofj{i}(it), Dofj{i}(it))+mat_Toct{nm}(Dofi{i}(it), Dofi{i}(it));
    ##%mat_Toct{nm}(Dofj{i}(it), Dofj{i}(it))=mat_Toct{nm}(Dofj{i}(it), Dofj{i}(it))+mat_Toct{nm}(Dofi{i}(it), Dofi{i}(it));
    ##%mat_Toct{nm}(Dofj{i}(it), Dofi{i}(it))=0;
    ##%mat_Toct{nm}(Dofi{i}(it), :)=mat_Toct{nm}(Dofj{i}(it), :)+mat_Toct{nm}(Dofi{i}(it), :);% sans absorption
    ##%mat_Toct{nm}(Dofi{i}(it), Dofi{i}(it))=mat_Toct{nm}(Dofi{i}(it), Dofi{i}(it))+SurfaceAbs/2;
    ##
    ##
    ##%mat_Toct{nm}(Dofi{i}(it), :)=-mat{nm}(Dofj{i}(it), :)-mat(Dofi{i}(it), :);% sans absorption
    ##%mat_Toct{nm}(Dofi{i}(it), :)=0;% sans absorption
    ##%mat_Toct{nm}(Dofi{i}(it),:)=mat_Toct{nm}(Dofj{i}(it),:)
    ##%
    ##%iter=iter+1;Dirichlet{nm}(iter)=Dofi{i}(it);
    ####  		mat_Toct{nm}(Dofi{i}(it), :)=mat_Toct0{nm}(Dofi{i}(it), :)...
    ####				+mat_Toct0{nm}(Dofj{i}(it), :);
    ####        
    ####		 		mat_Toct{nm}(Dofj{i}(it), :)=mat_Toct0{nm} (Dofj{i}(it), :)...
    ####				+mat_Toct0{nm}(Dofi{i}(it), :);
    ##%===================================
    ####          		mat_Toct{nm}(Dofi{i}(it), Dofj{i}(it))=mat_Toct{nm}(Dofi{i}(it), Dofj{i}(it))...
    ####				-PondEchSurf (it,nm) *c0/4;
    ####			mat_Toct{nm}(Dofj{i}(it), Dofi{i}(it))=mat_Toct{nm}(Dofj{i}(it), Dofi{i}(it))...
    ####				-PondEchSurf (it,nm) *c0/4;
    ##%========================
            mat_Toct[nm][Dofi[i][it]-1, :]+=mat_Toct0[nm][Dofj[i][it]-1, :]	  
            mat_Toct[nm][Dofi[i][it]-1, Dofj[i][it]-1]+=-PondEchSurf[it][nm]*c0/4
    #        print(klne,'_ne',mat_Toct[nm][Dofi[i][it], Dofj[i][it]])
            mat_Toct[nm][Dofj[i][it]-1, :]+=mat_Toct0[nm][Dofi[i][it]-1, :]	 
    ##%+PondEchSurf (it,nm) /1*mat_Toct{nm}(Dofi{i}(it), :);	 
    ##
    ##   
            mat_Toct[nm][Dofj[i][it]-1, Dofi[i][it]-1]+=-PondEchSurf[it][nm]*c0/4
    #        print(klne,'_ne',mat_Toct[nm][Dofj[i][it], Dofi[i][it]])
    ##
    ##
    ##%===============%
    ##
          else :
    ##     mat_Toct{nm}(Dofi{i}(it), :)=mat_Toct{nm}(Dofi{i}(it), :)...
    ##+0.001/(2-0.1)*mat_Toct0{nm}(Dofj{i}(it), :);	  
            mat_Toct[nm][Dofi[i][it]-1, Dofj[i][it-1]-1]+=-PondEchSurf[it][nm]*c0/4
    ##          mat_Toct{nm}(Dofj{i}(it), :)=mat_Toct{nm}(Dofj{i}(it), :)...
    ##+0.001/(2-0.1)*mat_Toct0{nm}(Dofi{i}(it), :);	  
            mat_Toct[nm][Dofj[i][it-1]-1, Dofi[i][it]-1]+=-PondEchSurf[it][nm]*c0/4
    ##%sum(PondEchSurf ,1)
    ##	clear Surf_ech
    ##	clear PondEchSurf
    ##end
    #
    #########################################################
    #print(mat_Toct[0][:2,:])
    ##fig,ax00 = plt.subplots()   #  aperçu de la matrice creuse
    ##plt.spy(mat_Toct[1],precision=0.01, markersize=1)    # idem spy(mat) avec Octave
    ##fig.tight_layout()
    #################### fin TransmissionConstructionMatrix4 ###########################
    ###### SOUND SOURCES
    ####%# Load sound sources information, for EDP second member    ######
    print('Sound sources information')
    #Srce_sonore_I=np.loadtxt(domaine+'_sources.txt',dtype=float) # Load sources sonores
    
    #-----------MODIFS CODE 25/06/2024---------------
    if len(Srce_sonore_I.shape)==1:  #cas avec une seule source, Srce_sonore_I.shape = (longeur,) or on veut (nb source, longueur)
        Ns, Ls = 1, Srce_sonore_I.shape[0]
        Srce_sonore_I=np.array([Srce_sonore_I,np.zeros(Ls)]) #ajout d'une 2ème source neutre pour avoir une matrice
    else:
        Ns, Ls = Srce_sonore_I.shape
    #-----------FIN MODIFS CODE 25/06/2024-----------
    
    #
    VolSource=np.zeros(Ns)
    indss = []
    for s in range(Ns) :
      print('s=',s)
      xs=Srce_sonore_I[s,0]
      ys=Srce_sonore_I[s,1]
      zs=Srce_sonore_I[s,2]
    #%
      dist2S=(xdf-xs)**2+(ydf-ys)**2+(zdf-zs)**2; # Distance from source to DOF
      rayonS2=0.15**2
      VolSource[s]=-10    # pourquoi -10 ??
      while VolSource[s]<=0 :
        rayonS2 = rayonS2*1.25; # Radius incrementation. TODO: should be justified?
        res=np.nonzero(dist2S<rayonS2)[0]
        VolSource[s]=sum(V_VC[res]) # Volume of the 'real' source (sum of the volum of each dol(ind))
      indss.append(res)    # Search all DOF inside the source radius
    #  
      Srce_sonore=np.zeros((Ns,3+len(NNOct)),float)
      SelectedFrequency2=[k+3 for k in SelectedFrequency]
      Srce_sonore[s,3+NNOct]=1e-12*10**(Srce_sonore_I[s,SelectedFrequency2]/10) # Octave band sound power
      Volumic_Power_Srce=Srce_sonore[s,3+NNOct]/VolSource[s] # Volumic normalisation
    #%  # Display sound source information
    print('Sound source N°: ',s)
    print('Volume source = {0:06.4f}'.format(VolSource[s]))
    print('Location: ',indss[s])
    ############ sauvegarde éléments non nuls de la matrice creuse ########################
    #with open("mat_Toct{1}_non_nuls.txt", "w") as sauvegarde:
    #  print(mat_Toct[0], file=sauvegarde )
    ##############################################
    RHS=np.zeros(NDOF) # Memory allocation
    w = []
    for  N_Toct in range(NOct) :
    #for  N_Toct in np.arange(1,NOct+1) :
      for s in np.arange(Ns) :
    #    print('N_Toct=',N_Toct,'s=',s)
        RHS[indss[s]]=Volumic_Power_Srce[N_Toct]*V_VC[indss[s]] # Octave band sound power
    #  print('valeurs non nulles dans RHS', [x for x in RHS if x>0])  #  idem Octave
    #  print('indices valeurs non nulles dans RHS', np.where((RHS)!=0))  #  idem Octave
      w.append(linalg.spsolve(mat_Toct[N_Toct].tocsr(), RHS))  # mat_Toct[N_Toct+1] est bien identique à mat_Toct{N_Toct} (Octave) et RHS à RHS (Octave)
    ##########################   affichepatchBlocks Python  sans affichage    ###############################
    ####################   pour tentatives d'affichage cf Diffusion_MD_ef2_dep0-2.py  ###############################
    iaf=4 # % 5==>125HZ
    waf=w[iaf]        # 
    rhoco2=1.2*c0**2
    LpdB=10*np.log10(waf*rhoco2/(2e-5)**2)
    #########################   fin  affichepatchBlocks Python    ###############################
    #%## TIME VARYING STATE CALCULATION
    print('Time varying state calculation')
    Sc=1.8
    A2=0.1*5*5*6-2
    A22=A2+Sc
    k2=Sc/(A2+Sc)
    A11=A22
    print('10*log10(k2)=',10*np.log10(k2))
    L1=10*np.log10(1/1.2/c0**2*103*c0/4*A22/(A22*A11-Sc**2)/1e-12)
    val=10*np.log10(el2di_Dof[-1][:,0:3]*rhoco2/(2e-5)**2)
    L1moy=np.mean(np.mean(val))
    #iaf=4 # 5==>125HZ    # déjà défini dans affichepatchBlocks
    #
    #waf=w[iaf]
    #rhoco2=1.2*c0**2 
    
    #-----------MODIFS CODE 26/06/2024---------------
    #calcul de la différence d'energie entre les salles (itération en f° nbre de salles)
    LpdBwmoy=[]
    for k in range(nbre_salles):
        LpdBwmoy.append(10*np.log10(np.mean(np.mean(waf[Tet_Dof[k][:,:4]-1]))*rhoco2/(2e-5)**2))
    # LpdBwmoy1=10*np.log10(np.mean(np.mean(waf[Tet_Dof[0][:,:4]-1]))*rhoco2/(2e-5)**2)
    # LpdBwmoy2=10*np.log10(np.mean(np.mean(waf[Tet_Dof[1][:,:4]-1]))*rhoco2/(2e-5)**2)
    # deltaL_w=LpdBwmoy1-LpdBwmoy2
    deltaL_w=np.zeros((nbre_salles,nbre_salles))
    for i in range(1,nbre_salles):
        for j in range(1,i):
            deltaL_w[i][j]=LpdBwmoy[i]-LpdBwmoy[j]
            deltaL_w[j][i]=-1*deltaL_w[i][j]
    
    #DELTALTet= np.mean(np.mean(LpdB[Tet_Dof[0][:,:4]-1]))-np.mean(np.mean(LpdB[Tet_Dof[1][:,:4]-1]))
    #DELTAL= np.mean(np.mean(LpdB[el2di_Dof[0][:,:3]-1]))- np.mean(np.mean(LpdB[el2di_Dof[1][:,:3]-1]))
    DELTATet=np.zeros((nbre_salles,nbre_salles))
    DELTAL=np.zeros((nbre_salles,nbre_salles))
    for i in range(1,nbre_salles):
        for j in range(1,i):
            DELTATet[i][j]=np.mean(np.mean(LpdB[Tet_Dof[i][:,:4]-1]))-np.mean(np.mean(LpdB[Tet_Dof[j][:,:4]-1]))
            DELTATet[j][i]=-1*DELTATet[i][j]
            DELTAL[i][j]=np.mean(np.mean(LpdB[el2di_Dof[i][:,:3]-1]))- np.mean(np.mean(LpdB[el2di_Dof[j][:,:3]-1]))
            DELTAL[j][i]=-1*DELTAL[i][j]
    #-----------FIN MODIFS CODE 26/06/2024-----------
    
    Sc2=Surfaceporte    #   ces valeurs sont à 0 ce qui entraîne une erreur sur 10*np.log10(k2)
    k2=Sc2/(A2+Sc2)
    A11=A22
    #print('10*log10(k2)=',10*np.log10(k2))         # divide by zero encountered in log10
    Ac=0.1*(5*5*6-Sc)+Sc
    Cr=10*np.log10(Ac/25)
     
    TheorieR0R10=10*np.log10(25/(Sc*10**(-0.1*0)+(25-Sc)*10**(-0.1*10)))+Cr
    TheorieR0R20=10*np.log10(25/(Sc*10**(-0.1*0)+(25-Sc)*10**(-0.1*20)))+Cr
    TheorieR0R30=10*np.log10(25/(Sc*10**(-0.1*0)+(25-Sc)*10**(-0.1*30)))+Cr
    print('TheorieR0R10 = {0:02.4e}'.format(TheorieR0R10))
    print('TheorieR0R20 = {0:02.4e}'.format(TheorieR0R20))
    print('TheorieR0R30 = {0:02.4e}'.format(TheorieR0R30))
    TheorieR0Rinf=10*np.log10(25/(Sc*10**(-0.1*0)+(25-Sc)*10**(-0.1*400)))+Cr
    Rlim=  10*np.log10(25/Sc)+Cr
    print('TheorieR0Rinf = {0:02.4e}'.format(TheorieR0Rinf))
    print('Rlim = {0:02.4e}'.format(Rlim))
    ##
    #with open("mat_non_nuls.txt", "w") as sauvegarde:
    #  print(mat_Toct[6], file=sauvegarde )
    ##print(mat_Toct[0])
    ##################   CalculInsta Python   #####################
    itmax=round(duration/dt) # Number of iterations
    MATinsta= np.diag(V_VC) # Integration constant   #  je ne suis pas sûr de bien comprendre comment fonctionnait la fontion integVC_cst
    #print('MATinsta=',MATinsta)
    ##  NomFichier=strcat(domaine+'_WInstaFields')  
    #%=======================================================================
    #% Pour verification de decroissance energie sonore 
    #%=========================================================================
    xmicro=2.
    ymicro=2.
    zmicro=2.
    dist2m=(xdf-xmicro)**2+(ydf-ymicro)**2+(zdf-zmicro)**2   # Distance from source to DOF
    rayonS2=0.15**2
    Volum=-10
    #while Volum<=0:
    #  ind=range(NDOF)
    #  rayonS2=rayonS2*1.25   # Radius incrementation. TODO: should be justified?
    #  inds=ind(dist2m<rayonS2)   # Search all DOF inside the micro radius
    #  Volum=sum(V_VC[inds])  # Volume of the 'real' source (sum of the volum of each dol(ind))
    ###%inds est l'indice du point de contr�le
    #ind=range(NDOF)
    while Volum<=0:
      rayonS2=rayonS2*1.25   # Radius incrementation. TODO: should be justified?
    #  inds=ind(dist2m<rayonS2)   # Search all DOF inside the micro radius
      inds=[x for x in range(NDOF) if dist2m[x] < rayonS2]
      Volum=sum(V_VC[inds])  # Volume of the 'real' source (sum of the volum of each dol(ind))
    #  print('volum=',Volum)
    #  
    #  ## LOOP ON FREQUENCY BANDS
    temps_apres_arret_Srce= np.arange(0,itmax*dt,dt)
    ######    parallélisation  #########
    ### marche aussi en ne passant que N_Toct....
    class Insta(Thread):
      def __init__(self, n):
        Thread.__init__(self)
        self.n = n     # N_Toct
      def run(self):
        """Code à exécuter pendant l'exécution du thread."""
    #    mat2=csr_matrix(dt*mat_Toct[self.n].tocsr()+ MATinsta) # Euler Implicite 1 matrice csr pour calcul direct
        mat2=csc_matrix(dt*mat_Toct[self.n].tocsr()+ MATinsta) # Euler Implicite 1 matrice csc pour décomposition LU
        wi=deepcopy(w[self.n])     # pas sûr que le deepcopy soit indispensable
    #    print('dt=',dt,'itmax+1=',itmax+1,'len(wi)=',len(wi))
        wi_saved=np.zeros((len(wi),itmax+1) )
        wi_saved[:,0]=wi
        lu = linalg.splu(mat2)
          # Resolution  du  systeme  AX = B en  utilisant  la forme  factorisee  dans C14
        for it in np.arange(1,itmax+1) :
          RHS= MATinsta @ wi # Euler Implicite 2
      #    wi=mat2\RHS;
      #    wi,_ = linalg.bicgstab(mat2, RHS, atol=tol, maxiter= maxint)   # ne donne pas le même résultat 17s avec T30= 0.594 & T30= 0.604
      #    wi, _ = linalg.minres(mat2, RHS)     # donne presque le même résultat 63s avec T30= 1.586 & T30= 1.532
      #    wi = linsolve.spsolve(mat2, RHS)    # donne le même résultat que Octave...mais très lent 300s : T30= 1.612 & T30= 1.54
    #
    #      wi = linalg.spsolve(mat2, RHS)    # donne le même résultat que Octave...mais très lent 296s : T30= 1.612 & T30= 1.54
          wi = lu.solve( RHS )
    #      wi, _ = linalg.bicg(mat2, RHS)    # 
          wi_saved[:,it]=wi
        LdBi=10*np.log10(np.abs(wi_saved[inds,:]*rhoco2/(2e-5)**2))
        LdBi=LdBi-LdBi[0][0]
        _, jn=np.where((LdBi+30.)>0.)  #  On cherche l'indice pour lequel on a descendu de 30dB
        T30=2*temps_apres_arret_Srce[jn[-1]-1]  #   Estimation du TR60 sur une dynamique de 30dB
        print(self.n,'T30=',T30)   # idem Octave
    ######   fin parallélisation  #########
    ### Création des threads
    thread_1 = Insta(0)
    thread_2 = Insta(1)
    thread_3 = Insta(2)
    thread_4 = Insta(3)
    thread_5 = Insta(4)
    thread_6 = Insta(5)
    thread_7 = Insta(6)
    #
    ## Lancement des threads
    thread_1.start()
    thread_2.start()
    thread_3.start()
    thread_4.start()
    thread_5.start()
    thread_6.start()
    thread_7.start()
    #
    ## Attend que les threads se terminent
    thread_1.join()
    thread_2.join() 
    thread_3.join() 
    thread_4.join()
    thread_5.join()
    thread_6.join() 
    thread_7.join() 
    ##
    #### 392 s sur pc Hadrien
    #### 28 s avec décomposition LU
    ##    wi,_ = linalg.bicgstab(mat2, RHS, atol=tol, maxiter= maxint)   # ne donne pas le même résultat 17s avec T30= 0.594 & T30= 0.604
    ##    wi, _ = linalg.minres(mat2, RHS)     # donne presque le même résultat 63s avec T30= 1.586 & T30= 1.532
    ##    wi = linsolve.spsolve(mat2, RHS)    # donne le même résultat que Octave...mais très lent 300s : T30= 1.612 & T30= 1.54
    ##    wi = linalg.spsolve(mat2, RHS)    # donne le même résultat que Octave...mais très lent 296s : T30= 1.612 & T30= 1.54
    #    wi, _ = linalg.bicg(mat2, RHS)    # 
    #
    ##with open("mat_non_nuls.txt", "w") as sauvegarde:
    ##  with np.printoptions(threshold=3000):
    ##    print(wi_saved, file=sauvegarde )
    ##print(wi_saved)
    ########## fin EUler Implicite  ######
    ###################  Fin Calcul_insta   ####################################
    ####%toc
    ####%## SAVE COORDINATES AND MESHES
    ##display('Save data into HDF5 files')
    ##NomFichier=strcat(domaine,'_XYZ_TETRA'); 
    ##CoordDOF=strcat(NomFichier,'.hdf5');
    ##topo.XYZ=XYZ2;
    ##topo.TetDOF=Tet_Dof;
    ##save( '-hdf5', CoordDOF, 'topo')
    ##############"" les sauvegardes ne semblent pas marcher ????
    ##print('Save data into HDF5 files')
    ##NomFichier = (domaine+'_XYZ_TETRA') 
    ##CoordDOF = (NomFichier+'.hdf5')
    ##topo.XYZ = XYZ2
    ##topo.TetDOF = Tet_Dof
    ##save( '-hdf5', CoordDOF, 'topo')
    ##%%FacesTj_Dof{1}(:,1:3)
    ##%%[waf(portei) waf(portej)   10*log10(waf(portei)./ waf(portej))]
    ##print(' paroi') 
    ##%%[waf(Dofj{1}(:)) waf(Dofi{1}(:)) waf(Dofj{1}(:))./waf(Dofi{1}(:))]
    ##[waf(portej(:)) waf(portei(:)) 10*log10(waf(portej(:))./waf(portei(:)))]
    ##print([('{0:02.4e} & {1:02.4e}& {1:02.4e} \n'.format(waf[x], waf[y], 10*np.log10(waf[x]/waf[y]))) for x in portej for y in portei])
    ###
    #### Affichage du temps d execution
    print('Temps d execution : {0:02.4e} s '.format(time.time() - start_time))
    print("taille :", len(w), len(w[0]))
    print("type :", type(w), type(w[0]))
    print("w :", w)
    return XYZ3,Tet_Dof,w,dt
