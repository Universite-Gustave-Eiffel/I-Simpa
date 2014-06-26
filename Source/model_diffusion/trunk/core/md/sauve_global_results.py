# -*- coding: cp1252 -*-
import libsimpa as ls
import os

def SauveGlobalResults(coreconf,app_lst):
    """
        Il faut rassembler les bandes de fréquences et les convertir en fichier gabe sous l'extension .recp
        Puis supprimer les fichier txt générés par comsol
    """
    rootpath=coreconf.paths["workingdirectory"]
    data=[]
    labels=[]
    for comidfreq in coreconf.freqid_docalclst:
        idfreq=comidfreq-1
        nomfichier=rootpath+str(coreconf.freqlst[idfreq])+".global.txt"
        if os.path.exists(nomfichier):
            fich=open(nomfichier,'r')
            #Une ligne par application
            for idapp,line in enumerate(fich):
                labels.append(str(coreconf.freqlst[idfreq])+" Hz "+app_lst[idapp].outname)
                floatar=ls.floatarray()
                steps=line.split()
                for step in steps:
                    floatar.append(float(step))
                data.append(floatar) #on stocke les pas de temps dans les données
            fich.close()
            os.remove(nomfichier)
        else:
            print "Impossible de trouver :"
            print nomfichier
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
            #1 colonne par physique, dans le même ordre que le tableau app_lst
            steplst.append("Niveau stationnaire")
        gabe_out=ls.Gabe_rw(len(labels)+1)
        gabe_out.AppendStrCol(steplst,"SPL")
        for collabel,freqdata in zip(labels,data):
            gabe_out.AppendFloatCol(freqdata,collabel)
        gabe_out.Save(rootpath+"Cumul_energetique.recp")
