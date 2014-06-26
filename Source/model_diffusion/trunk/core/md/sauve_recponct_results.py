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
def MakeTCData(coreconf,stepPositions,resultsModificationLayer,rpid,idfreq):
    """
        Retourne la décroissance pour un récepteur ponctuel et une frequence (en champ direct)
    """
    tcdata=[0.]
    if coreconf.const["temporel"]:
        tcdata=[0.]*coreconf.const["nbpasdetemps"]
    for idsrc,idstep in stepPositions.iteritems():
        if resultsModificationLayer.has_key(idsrc) and resultsModificationLayer[idsrc].recp.has_key(rpid):
            tcdata[idstep]+=resultsModificationLayer[idsrc].recp[rpid][coreconf.freqid_docalclst.index(idfreq+1)]
    return tcdata
    
def SauveRecepteurPonctResults(coreconf,resultsModificationLayer):
    """
        Il faut rassembler les bandes de fréquences pour chaque
        récepteur ponctuel et les convertir en fichier gabe sous l'extension .recp
        Puis supprimer les fichiers txt.
    """
    rootpath=coreconf.paths["workingdirectory"]+coreconf.paths["recepteursp_directory"]+"\\"
    for rpnode in coreconf.rootnode["recepteursp"].lstnodesenum("recepteur_ponctuel"):
        rpid=int(rpnode.getproperty("id"))
        stepPositions=GetNumStepBySource([rpnode.getpropertyfloat("x"),rpnode.getpropertyfloat("y"),rpnode.getpropertyfloat("z")],coreconf)
        rppath=rootpath+rpnode.getproperty("lbl")+"\\"
        data=[]
        labels=[]
        for comidfreq in coreconf.freqid_docalclst:
            idfreq=comidfreq-1
            nomfichier=rppath+str(coreconf.freqlst[idfreq])+".rponct.txt"
            if os.path.exists(nomfichier):
                floatar=ls.floatarray()
                layer_data=MakeTCData(coreconf,stepPositions,resultsModificationLayer,rpid,idfreq)
                labels.append(str(coreconf.freqlst[idfreq])+" Hz")
                fich=open(nomfichier,'r')
                idstep=0
                for line in fich:
                    if line.strip()!="":
                        floatar.append(float(line)+layer_data[idstep])
                        idstep+=1
                data.append(floatar) #on stocke les pas de temps dans les données
                fich.close()
                os.remove(nomfichier)
            else:
                print("Impossible de trouver :")
                print(nomfichier)
        ##########
        ## Création du fichier de récepteur ponctuel
        if len(labels)>0:
            #libellé des pas de temps
            stepval=coreconf["simulation"].getpropertyfloat("pasdetemps")
            curstep=stepval
            maxstep=coreconf["simulation"].getpropertyfloat("duree_simulation")
            steplst=ls.stringarray()
            if coreconf.const["temporel"]:
                while round(curstep,4)<=maxstep:
                    steplst.append("%i ms" % (int(curstep*1000)))
                    curstep+=stepval
            else:
                steplst.append("Niveau stationnaire")
            gabe_out=ls.Gabe_rw(len(labels)+1)
            gabe_out.AppendStrCol(steplst,"SPL")
            for collabel,freqdata in zip(labels,data):
                gabe_out.AppendFloatCol(freqdata,collabel)
            gabe_out.Save(rppath+"soundpressure.recp")
                
