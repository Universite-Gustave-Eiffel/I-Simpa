# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import libsimpa as ls
import os
from vec3 import *
import math

def GetNumStepBySource(pos,coreconf):
    """
        Retourne le pas de temps où le son direct est censé rejoindre la position pos pour chaque source sonore
    """
    ret_tab={}
    if coreconf.const["ajouter_son_direct"]:
        for src in coreconf.sources_lst:
            if coreconf.const["temporel"]:
                dist=(vec3(pos)-vec3(src.pos)).length()
                ret_tab[src.id]=int(math.floor(dist/(coreconf.const["cel"]*coreconf.const["pasdetemps"])))
            else:
                ret_tab[src.id]=0
    return ret_tab
def MakeTCData(coreconf,stepPositions,resultsModificationLayer,rsid,idfreq,faceid):
    """
        Retourne la décroissance pour un récepteur surfacique et une frequence (en champ direct)
    """
    tcdata=[0.]
    if coreconf.const["temporel"]:
        tcdata=[0.]*coreconf.const["nbpasdetemps"]
    for idsrc,idstep in stepPositions.iteritems():
        if resultsModificationLayer.has_key(idsrc) and resultsModificationLayer[idsrc].recsurf.has_key(rsid):
            tcdata[idstep]+=resultsModificationLayer[idsrc].recsurf[rsid][coreconf.freqid_docalclst.index(idfreq)][faceid]
    return tcdata
    
def SauveRecepteurSurfResults(idfreq,rslist, mesh,sommets,timestep, folderresults, resultfilename,resultsModificationLayer,coreconf):
    rsdata=ls.rsurf_data()
    nbstep=0
    for k,v in rslist.iteritems():
        nbstep=0
        currsdata=[]
        #lecture du fichier
        nomfichier=folderresults+str(k)+".rsurf.txt"
        if os.path.exists(nomfichier):
            fich=open(nomfichier,'r')
            for line in fich: #pour chaque face
                if line.find("\n")!=-1:
                    line=line[:-2]
                curstep=[]
                for gtrival in line.split(" "): #pour chaque pas de temps
                    curstep.append(float(gtrival))
                currsdata.append(curstep)
                nbstep+=1
            fich.close()
            #Suppression du fichier TXT
            os.remove(nomfichier)
            rslist[k].data=zip(*currsdata)
        else:
            print("Un fichier de récepteur surfacique est absent :\n" + nomfichier)
            return
        #sauvegarde dans la structure de données
    rsdata.Make(len(sommets),len(rslist),nbstep,timestep)
    for nodeid,sommet in enumerate(sommets):
        rsdata.SetNodeValue(nodeid,sommet[0],sommet[1],sommet[2])

    rscount=0
    for k,v in rslist.iteritems():
        rsdata.MakeRs(rscount,len(v.faceindex),str(v.name),k)
        facecount=0
        for face in v.faceindex:
            stepPositions=GetNumStepBySource(v.facelst[facecount],coreconf)
            tcData=MakeTCData(coreconf,stepPositions,resultsModificationLayer,k,idfreq,facecount)
            rsdata.SetFaceInfo(rscount,facecount,face[0],face[1],face[2],len(v.data[facecount]))
            recordcount=0
            for record in v.data[facecount]:
                rsdata.SetFaceEnergy(rscount,facecount,recordcount,recordcount,record+tcData[recordcount])
                recordcount+=1
            facecount+=1    
        rscount+=1
    ls.rsurf_io.Save(resultfilename,rsdata)
