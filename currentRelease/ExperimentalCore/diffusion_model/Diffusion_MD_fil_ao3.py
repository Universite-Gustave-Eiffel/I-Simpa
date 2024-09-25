#! /usr/bin/env python3.6
# -*- coding:Utf8 -*-
#############################################################################
# Pg Python adaptation of mainDiffusion.m (Room Acoustics Diffusion Model). #
#                                                                           #
## Author: Christian Prax <Christian.prax@univ-poitiers.fr>
## Keywords: Room acoustics, diffusion mode                                 #
#
#  portage Python 3.6 E.Foucault  XII 2019 --> VI 2020
# parallel version for unsteady calculation    #
# LU decomposition
#  some cosmetic changes EF VI 2024                           #
#############################################################################
# Importing external functions :
from bibli_MD_ao1 import *   # EF library for MD
def main(el,XYZ,el2Dtypd,Abs_Mater,i_nT,Wall_TL,Srce_sonore_I,coreConf):
    import multiprocessing
    import numpy as np
    from copy import deepcopy
    from scipy.sparse import csr_matrix, csc_matrix, linalg
    from operator import itemgetter
    import time, sys#,os
    from threading import Thread
    ############################################################################
    ###################### start program ################################
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
    m, c0 = Coef_Att_Atmos(Frequency,Humidity,atmosphere,273.15+Temperature)
    if coreConf.const['do_abs_atmo']==False:
    # No amtospheric absorption coefficient
      mc=np.zeros(NOct, dtype=float)
    else:
      if coreConf.const['imposed_abs_atmo']==True:
          m=coreConf.const['abs_atmo']*np.ones(len(SelectedFrequency))
    # Use atmospheric absorption coefficient
      mc=m*c0
    ## READING MESHING AND NODES
    print('Read meshes and nodes')
    domaine='scene' # Meshing file name
    nbre_salles=np.size(np.unique(el[:,4])) # Number of volumes (i.e. rooms)
    NBLOCKS=nbre_salles # Number of blocks of the meshing
    idBloc=np.unique(el[:,4]) # Block identification
    print('Number of volumes ',nbre_salles)
    
    ## TABLE OF DOF SIGNATURES
    # DOF_Sig <--Table of signatures for DOF
    # Example NBLOCKS= 3, include 1 0 1 for a node associate to the volume 1 et volume 3
    print('Create table of signatures')
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
    cs=[np.sum(sortedndf[val[ij],1:],axis=0) for ij in range(nn+1)]
    cs=np.array(cs)
    col=np.arange(len(cs))
    NFS=np.append(col[:,np.newaxis], cs, 1)
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
    ##%## NODES AND DOF COORDINATES
    x=XYZ[:,0]
    y=XYZ[:,1]
    z=XYZ[:,2] # Coordinates of each node
    xdf=x.take(sortedndf[:,0]-1)
    ydf=y.take(sortedndf[:,0]-1)
    zdf=z.take(sortedndf[:,0]-1)
    XYZ2=[xdf, ydf, zdf]
    XYZ3=np.transpose(XYZ2)
    XYZ3=np.array(XYZ3)
    ### READING MESHES ON BOUNDARIES
    print('Identifying boundaries')
    nel2D0=el2Dtypd.shape[0]
    FT =np.sort(el2Dtypd[:,0:3])
    FTu, l, i, c =np.unique(FT, axis=0, return_index=True, return_counts=True, return_inverse=True)    # le l retourné n'est pas le même que Octave !!!
    n=0
    for m in c:
      if m!=1:
        a=l[n]
        b=np.where(np.all(FT==FT[a],axis=1))[0][-1]
        l[n]=b
      n+=1
    el2Dtyp=el2Dtypd[l,:]
    nel2Dt=len(el2Dtyp)
    ll=np.arange(nel2D0)
    duplicate_l = np.setdiff1d(ll, l, assume_unique=True)
    ### IDENTIFYING MESHES WITH TRANSMISSION
    el2D_Trsm=el2Dtypd[duplicate_l,:]
    #### INFORMATION ON MESHS ON BOUNDARIES
    el2D=el2Dtyp[:,0:3] # Meshes on boundaries
    n_materiau=el2Dtyp[:,3] # Identification of boundary material
    nel2D=len(el2D) # Number of meshes on boundaries
    #################
    ### SEARCH BONDARY FACES OF EACH BLOCK
    sortedel2D= np.sort(el2D, axis=1) # Sort on each line
    nel2D=np.zeros((NBLOCKS,1))
    Surf=np.zeros((NBLOCKS,1))
    VOLUME=np.zeros((NBLOCKS,1))
    Lambda=np.zeros((NBLOCKS,1)) 
    el2di=[]
    for i in range(NBLOCKS):
      ef, sortedel2di = boundary_faces(Tet[i]); # Meshes of Volume i
      a= np.nonzero(np.all(sortedel2di == sortedel2D[:,np.newaxis], axis=2))
      el2di.append(el2Dtyp[a[0],:])
    ############################################  
    V_VC=np.zeros(np.sum(nnB))
    AireTr=[NBLOCKS+1]
    VOLUME=[]
    V_VCi=[]
    for i in range(NBLOCKS):
      nel2D[i]=len( el2di[i] )# Number of meshes on boundaries ## max inutile : python renvoie directement le nb de lignes
      Surf, aire= Surfaces_Salle(x,y,z,nel2D[i][0],el2di[i])  # Surface des parois+de chaque elt
      AireTr.append(aire)
      VolSalle= VolumeSalle(x,y,z,len( Tet[i]), Tet[i]) ; # Size of volume i (m3)
      VOLUME.append(VolSalle)
      V_VCi=VolumeVCDOF(x,y,z,sum(nnB),len(Tet[i]),Tet[i], Tet_Dof[i]); # Table of the volumes of control
      V_VC+=V_VCi
    ############################################
    ### PARAMETERS OF THE ROOM ACOUSTICS DIFFUSION MODEL Modifié
    Lambda=4*(VOLUME/Surf)    # Mean Free path (m)
    CoeffDiff=Lambda*c0/3    # Diffusion coefficient TODO: Fix the diffusion coefficient value for mixed surface reflection and atmospheric absorption
    CoeffDiff=[]
    for i in range(len(Tet_Dof)):
        CoeffDiff.append([])
        for j in range(len(Tet_Dof[i])):
            CoeffDiff[i].append(Lambda[i]*c0/3)
    ###########################################
    ## GLOBAL MATRIX ASSEMBLAGE
    print('Global matrix assemblage')
    # Tetra Assemblage of Global mat for diffusion operator
    mat=laplacienblocks2(xdf,ydf,zdf,Tet_Dof,NBLOCKS,NDOF,CoeffDiff)   # mat sparse CSR matrix
    #############################################
    ##### DIFFUSION EQUATION LEFT MEMBER
    mat_Toct=[]
    for N_Toct in range(NOct):
      mat_Toct.append(csr_matrix(mc[N_Toct]*np.diag(V_VC))-mat)
    mat_Toct0= deepcopy(mat_Toct)
    ############################################
    #### ACOUSTIC ABSORPTION
    ################# AbsorptionConstructionMatrix ########################
    ### Load the absorption coefficient of each material in the corresponding file
    iAM = np.where(np.sum(Abs_Mater[:,1:],axis=1)!=0)[0] # iAbs: row of absorbing material index (ATTENTION SUPPRIME les matériaux 100% réfléchissants avec que des 0)
    iAbs = Abs_Mater[iAM,0]          # iAbs: absorbing material index
    abs_prop=Abs_Mater[iAM,:]
    SelectedFrequency3=[k+1 for k in SelectedFrequency]  #incrément de 1 pour éviter l'identifiant du matériau
    abs_coef=abs_prop[:,SelectedFrequency3]
    el2di_Dof=deepcopy(el2di)
    SurFaceA=[]
    AireFace=[]
    DofAbs=[]
    EquivAbsAreaBlock=[]
    for n in range( NOct) :
      mat_Toct[n]=mat_Toct[n].tolil()
    for iB in range(NBLOCKS) :
      zz=deepcopy(el2di)[iB][:,0:3]
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
    ##  
      DofAbs.append(np.unique(el2di_Dof[iB][:,0:3]))
      EquivAbsArea=[] 
      for it in range(len(DofAbs[iB])) :
        ligne, col =np.where((el2di_Dof[iB][:,:3]-DofAbs[iB][it]) == 0)
        val= np.sum( AireFace[iB][ligne]*abs_coef[ el2di_Dof[iB][ligne,3],:].transpose(), axis=1)/3/4 # Sabine's based absorption
        EquivAbsArea.append(val)
        idfFree= DofAbs[iB][it]
        idfFree += -1   # array starts at 0
        for nm in range(NOct) :
          mat_Toct[nm][ idfFree,idfFree] += EquivAbsArea[it][nm]*c0
      EquivAbsAreaBlock.append(np.sum(EquivAbsArea,axis=0))
    ################# end AbsorptionConstructionMatrix ###########################
    ##### CALCULATE FACES TRANSMISSION el2diTrsm{ib}
    #### Load the transmission coefficient of each material in the corresponding file
    #### iAbs TL_50 TL_63 TL_80 .. TL_16000 TL_20000
    iTrsm=Wall_TL[np.where(np.sum(Wall_TL[:,1:],axis=1)!=0)[0],0] # iTrsm: index of Material with transmission
    l=np.where(np.sum(Wall_TL[:,1:],axis=1)!=0)[0]
    c=[0]+[k+1 for k in NonSelectedFrequency]
    Wall_TLnp=np.array(Wall_TL)
    TL=np.delete(Wall_TLnp,c,1)
    TAU=10.**(-TL[:,:NOct]/10)
    Surfaceporte=0
    el2diTrsm=[]
    for ib in range(NBLOCKS) :
        el2diTrsm.append(el2di[ib][np.in1d( el2di[ib][:,-1], iTrsm)])
    ###############
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
        #changement de stratégie pour la récupération des surfaces de contact
        Facescommunesij=np.array([])
        isin=np.isin(el2Di, el2Dj)
        init=0
        for k in range(len(isin)):
            if np.sum(isin[k,0:3])==3:
                if init==0:
                    Facescommunesij=np.array(el2Di[k])
                    init=1
                elif init==1:
                    Facescommunesij=np.append([Facescommunesij],[el2Di[k]], axis=0)
                    init=2
                else:
                    Facescommunesij=np.append(Facescommunesij,[el2Di[k]], axis=0)
        #fin changement de stratégie pour la récupération des surfaces de contact
        cond=Facescommunesij.size==0
        if not(cond) :
          FacesTij.append(Facescommunesij)
          FacesTij[it][:,3]= changem(FacesTij[it][:,3],np.arange(1,len(iTrsm)+1),iTrsm )     
          FacesTi_Dof.append( changem(FacesTij[it][:,0:3], N_F[i][:,1], N_F[i][:,0]))    #
          FacesTi_Dof[it]=np.c_[FacesTi_Dof[it][:,:],FacesTij[it][:,3]]   # adding a column in last position
          FacesTj_Dof.append( changem(FacesTij[it][:,0:3], N_F[j][:,1], N_F[j][:,0]))
          FacesTj_Dof[it]=np.c_[FacesTj_Dof[it][:,:],FacesTij[it][:,3]]   # adding a column in last position
          it+=1
    NbTransmWall = it
    ##
    #### EXCHANGE SURFACES (sum of the mesh face surfaces associated with each point, divided by 3
    SurFaceT=[]
    AireFace=[]
    ptsTrsm=[]
    Dofi=[]
    Dofj=[]
    for i in range(NbTransmWall) :
      surf, aire= Surfaces_Salle(x,y,z,len(FacesTij[i]),FacesTij[i])    # j'ai l'impression que l'on a déjà calculer l'aire
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
        Surf_ech.append(np.sum(AireFace[i][ligne])/3)  #  idem Octave
        PondEchSurf.append(np.sum( AireFace[i][ligne,np.newaxis] * TAU[FacesTij[i][ligne,3]-1,:], axis=0  )/3)
    		## Contribution of the transmission
        for nm in range(NOct) :
    ####     For total transmission walls
          condporte = False
    ##############  test door ###
          for ilg in range(len(ligne)) :
            if (TAU[FacesTij[i][ligne[ilg],3]-1,nm]==1.) :
              condporte = True
    #############################################
          if condporte :
            if nm==4 :
              portej.append(Dofj[i][it])
              portei.append(Dofi[i][it])
    ###############  surf_door ####### 
            SurfaceAbs=0
            for ilg in np.arange(0,len(ligne)) :
              if (TAU[FacesTij[i][ligne[ilg],3]-1,nm]==1):
                SurfaceAbs+=AireFace[i][ligne[ilg]]/3*c0/4
            mat_Toct[nm][Dofi[i][it]-1, :]+=mat_Toct0[nm][Dofj[i][it]-1, :]	  
            mat_Toct[nm][Dofi[i][it]-1, Dofj[i][it]-1] += -PondEchSurf[it][nm]*c0/4
            mat_Toct[nm][Dofj[i][it]-1, :]+=mat_Toct0[nm][Dofi[i][it]-1, :]	 
    ##   
            mat_Toct[nm][Dofj[i][it]-1, Dofi[i][it]-1] += -PondEchSurf[it][nm]*c0/4
    ##
    ##===============
    ##
          else :
            mat_Toct[nm][Dofi[i][it]-1, Dofj[i][it-1]-1] += -PondEchSurf[it][nm]*c0/4
            mat_Toct[nm][Dofj[i][it-1]-1, Dofi[i][it]-1] += -PondEchSurf[it][nm]*c0/4
    #
    ###### SOUND SOURCES
    ####%# Load sound sources information, for EDP second member    ######
    print('Sound sources information')
    if len(Srce_sonore_I.shape)==1:  #cas avec une seule source, Srce_sonore_I.shape = (longeur,) or on veut (nb source, longueur)
        Ns, Ls = 1, Srce_sonore_I.shape[0]
        Srce_sonore_I=np.array([Srce_sonore_I,np.zeros(Ls)]) #ajout d'une 2ème source neutre pour avoir une matrice
    else:
        Ns, Ls = Srce_sonore_I.shape
    #
    VolSource=np.zeros(Ns)
    indss = []
    for s in range(Ns) :
      print('s=',s)
      xs=Srce_sonore_I[s,0]
      ys=Srce_sonore_I[s,1]
      zs=Srce_sonore_I[s,2]
    #
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
      SelectedFrequency2=[k+3 for k in SelectedFrequency]  #incrément de 3 pour éviter les coordonnées spatiales des sources
      Srce_sonore[s,3+NNOct]=1e-12*10**(Srce_sonore_I[s,SelectedFrequency2]/10) # Octave band sound power
      Volumic_Power_Srce=Srce_sonore[s,3+NNOct]/VolSource[s] # Volumic normalisation
    #### Display sound source information
      print('Sound source N°: ',s)
      print('Volume source = {0:06.4f}'.format(VolSource[s]))
      print('Location: ',indss[s])
    ##############################################
    RHS=np.zeros(NDOF) # Memory allocation
    w = []
    for  N_Toct in range(NOct) :
      for s in np.arange(Ns) :
        RHS[indss[s]]=Volumic_Power_Srce[N_Toct]*V_VC[indss[s]] # 
      w.append(linalg.spsolve(mat_Toct[N_Toct].tocsr(), RHS))# Cette ligne là est peut-être à l'origine du pb pour 50 Hz
    #
    # iaf=4 # % 50==>125HZ
    waf=w[len(SelectedFrequency)-1]        # 
    rhoco2=1.2*c0**2
    LpdB=10*np.log10(waf*rhoco2/(2e-5)**2)
    #%## TIME VARYING STATE CALCULATION
    print('Time varying state calculation')
    Sc=1.8
    A2=0.1*5*5*6-2
    A22=A2+Sc
    k2=Sc/(A2+Sc)
    A11=A22
    print('10*log10(k2)=',10*np.log10(k2))
    LpdBwmoy=[]
    DELTATet=np.zeros((nbre_salles,nbre_salles))
    DELTAL=np.zeros((nbre_salles,nbre_salles))
    for i in range(1,nbre_salles):
        for j in range(1,i):
            DELTATet[i][j]=np.mean(np.mean(LpdB[Tet_Dof[i][:,:4]-1]))-np.mean(np.mean(LpdB[Tet_Dof[j][:,:4]-1]))
            DELTATet[j][i]=-1*DELTATet[i][j]
            DELTAL[i][j]=np.mean(np.mean(LpdB[el2di_Dof[i][:,:3]-1]))- np.mean(np.mean(LpdB[el2di_Dof[j][:,:3]-1]))
            DELTAL[j][i]=-1*DELTAL[i][j]
    
    Sc2=Surfaceporte
    k2=Sc2/(A2+Sc2)
    A11=A22
    Ac=0.1*(5*5*6-Sc)+Sc
    Cr=10*np.log10(Ac/25)

    ##################   CalculInsta Python   #####################
    duration=coreConf.const["duration"]
    dt=coreConf.const["timestep"]
    itmax=round(duration/dt) # Number of iterations
    MATinsta= np.diag(V_VC) # Integration constant 
    ################### Verification of sound energy decay
    xmicro=2.
    ymicro=2.
    zmicro=2.
    dist2m=(xdf-xmicro)**2+(ydf-ymicro)**2+(zdf-zmicro)**2   # Distance from source to DOF
    rayonS2=0.15**2
    Volum=-10
    while Volum<=0:
      rayonS2=rayonS2*1.25   # Radius incrementation. TODO: should be justified?
      inds=[x for x in range(NDOF) if dist2m[x] < rayonS2]
      Volum=sum(V_VC[inds])  # Volume of the 'real' source (sum of the volum of each dol(ind))
    #  ## LOOP ON FREQUENCY BANDS
    time_after_stop_Srce= np.arange(0,itmax*dt,dt)
    ######    parallelization  #########
    w_insta=deepcopy(w)
    for k in range(len(w)):
        w_insta[k]=list(w_insta[k])
    class Insta(Thread):
      def __init__(self, n):
        Thread.__init__(self)
        self.n = n     # N_Toct
      def run(self):
        """Code to be executed during thread execution."""
        mat2=csc_matrix(dt*mat_Toct[self.n].tocsr()+ MATinsta) # Euler Implicit 1 csc matrix for LU decomposition
        wi=deepcopy(w[self.n])     # pas sûr que le deepcopy soit indispensable
        wi_saved=np.zeros((len(wi),itmax+1) )
        wi_saved[:,0]=wi
        lu = linalg.splu(mat2)
          # Solving the system AX = B using the factored form in C14
        for it in np.arange(1,itmax+1) :
          RHS= MATinsta @ wi # Euler Implicite 2
          wi = lu.solve( RHS )
          wi_saved[:,it]=wi
        LdBi=10*np.log10(np.abs(wi_saved[inds,:]*rhoco2/(2e-5)**2))
        LdBi=LdBi-LdBi[0][0]
        _, jn=np.where((LdBi+30.)>0.)  # We are looking for the index for which we have gone down 30dB
        #T30=2*time_after_stop_Srce[jn[-1]-1]  #  TR60 estimation on a dynamic of 30dB
        #print(self.n,'T30=',T30)   # idem Octave
        w_insta[self.n][0]=wi_saved[0]
        for kbis in range(1,len(wi_saved)):
            w_insta[self.n][kbis]=wi_saved[kbis]
    ######   end of parallelization  #########
    ### Opti 05/09/2024 ###
    if coreConf.const["stationary"]==False:
        nb_coeurs=multiprocessing.cpu_count()
        k=0
        freq_restantes=len(SelectedFrequency)
        while freq_restantes > 0:
            if freq_restantes>nb_coeurs: #looking if there's more cpu cores than frequencies to calculate
                n=nb_coeurs
            else:
                n=freq_restantes
            for j in range(n):
                ### Creating threads in local variables
                locals()[f'thread_{j}']=Insta(k+j)
            for j in range(n):
                ## Lancement des threads in local variables
                locals()[f'thread_{j}'].start()
            for j in range(n):
                ## Attend que les threads se terminent
                locals()[f'thread_{j}'].join()
            k+=n
            freq_restantes-=n
            
        for k in range(len(w_insta)):
            w_insta[k]=np.array(w_insta[k])
    ##
    ########## Implicit Euler end ######
    ###################  end Calcul_insta   ####################################
    ##cas stationnaire, il faut ajouter une 3ème dimension et faire un array entre la 2ème et 3ème pour la passerelle
    if coreConf.const["stationary"]==True:
        for k1 in range(len(w_insta)):
            w_insta[k1]=np.array(w_insta[k1])
        w_insta = np.expand_dims(w_insta, axis=-1)
    #### Displaying execution time
    print('Execution time : {0:02.4e} s '.format(time.time() - start_time))
    ###
    return XYZ3,Tet_Dof,w_insta,dt
    
    
    
    
    
    
    
    
    
    
    
    
