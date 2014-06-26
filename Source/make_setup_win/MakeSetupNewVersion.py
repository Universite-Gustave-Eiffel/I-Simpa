# -*- coding: cp1252 -*-
import shutil
import os
import readpspsversion
import subprocess

sl=os.sep
path_pspssrc=".."+sl+"isimpa"+sl+"trunk"
path_dbgsrc=path_pspssrc+sl+"vc_mswd"+sl
path_release=".."+sl+"currentRelease"
path_nsis=".."+sl+".."+sl+"toolsSetup"+sl+"NSIS"+sl
path_releases=".."+sl+".."+sl
print("Construction du nouveau dossier release")
#nettoyage et copie debug
#rmtreelst=[
#path_release+sl+"UserScript"+sl,
#path_release+sl+"SystemScript"+sl,
#path_release+sl+"current"+sl,
#path_release+sl+"libsimpa"+sl]
#for treetorm in rmtreelst:
#    if os.path.exists(treetorm):
#        shutil.rmtree(treetorm)
#shutil.copytree(path_dbgsrc+"UserScript"+sl, path_release+sl+"UserScript"+sl)
#shutil.copytree(path_dbgsrc+"SystemScript"+sl, path_release+sl+"SystemScript"+sl)
#shutil.copytree(path_dbgsrc+"libsimpa"+sl, path_release+sl+"libsimpa"+sl)

clearlst=["I-SIMPA.lib","I-SIMPA.exp"]
for ele in clearlst:
    if os.path.exists(path_release+sl+ele):
        os.remove(path_release+sl+ele)

oldversionchain=[]
for element in os.listdir(path_releases+"versionReleaseHistory"):
    if os.path.isdir(path_releases+"versionReleaseHistory"+sl+element):
        oldversionchain.append(element)
print("Anciennes versions :")
print(oldversionchain)
#print("Copie des fichiers de languages")
#en
#langlist=["fr_FR","en_US"]
#for lang in langlist:
#    shutil.copy(path_pspssrc+sl+lang+sl+"internat.mo",path_release+sl+lang+sl)
#copie des coeurs de calculs

#exepaths=["core"+sl+"sppsNantes"+sl+"sppsNantes.exe",\
#          "core"+sl+"theorie_classique"+sl+"TheorieClassique.exe",\
##          #"core"+sl+"md"+sl,\
##          #"core"+sl+"nmpb"+sl,\
##          "meshing"+sl+"tetgen"+sl+"Preprocess.exe",\
##          "meshing"+sl+"vmr"+sl+"VolumetricMeshRepair.exe"\
##          ]
##
##for core in exepaths:
##    if os.path.isdir(path_dbgsrc+core):
##        shutil.rmtree(path_release+sl+core)
##        shutil.copytree(path_dbgsrc+core,path_release+sl+core)
##    else:
##        shutil.copy(path_dbgsrc+core,path_release+sl+core)
##
##
##
print("Lecture de la version")
versionchain=readpspsversion.GetVersion(path_pspssrc+sl)
print("Copie des fichiers release")
if os.path.exists(path_releases+"versionReleaseHistory"+sl+versionchain):
    print "Cette version du programme a déjà été construite, veuillez incrémenter la version"
    exit()
shutil.copytree(path_release+sl,path_releases+"versionReleaseHistory"+sl+versionchain)
print("Fin de copie des fichiers release")


nsiraw=open("rawsetup.nsi",'r')
nsiout=open("PspsSetup.nsi",'w')
for line in nsiraw:
    if line.find("APPVERSION")!=-1:
        line=line.replace("APPVERSION",versionchain)
    nsiout.write(line)
nsiraw.close()
nsiout.close()



print("Execution de la compilation du setup")
retcode = subprocess.call([path_nsis+'makensis.exe', 'PspsSetup.nsi'])
print "Setup créé"

print("Création des mises à jour...")


for oldver in oldversionchain:
    print("Création de la mise à jour "+oldver+" vers "+versionchain)
    #nettoyage
    clearlst=["patch.nsi","tmppatch"+sl]
    for ele in clearlst:
        if os.path.exists(ele):
            if os.path.isdir(ele):
                shutil.rmtree(ele)
                os.mkdir(ele)
            else:
                os.remove(ele)
    #creation du script de patch
    nsiraw=open("rawpatchpsps.nsi",'r')
    nsiout=open("patchpsps.nsi",'w')
    for line in nsiraw:
        if line.find("APPVERSION")!=-1:
            line=line.replace("APPVERSION",versionchain)
        if line.find("OLDVERSION")!=-1:
            line=line.replace("OLDVERSION",oldver)
        nsiout.write(line)
    nsiraw.close()
    nsiout.close()
    #nsisPatchGen versionReleaseHistory\1.0.0 versionReleaseHistory\1.0.1 tmppatch patch.nsi
    retcode = subprocess.call(['nsisPatchGen.exe', path_releases+'versionReleaseHistory'+sl+oldver,path_releases+'versionReleaseHistory'+sl+versionchain,'tmppatch','patch.nsi'])
    retcode = subprocess.call([path_nsis+'makensis.exe', 'patchpsps.nsi'])


#nettoyage
clearlst=["PspsSetup.nsi","patchpsps.nsi"]
for ele in clearlst:
    if os.path.exists(ele):
        os.remove(ele)
raw_input()
