# -*- coding: cp1252 -*-
##
# @file md.py
# @brief Main script
#
# \~english This script execute the Building Script engine, Run Comsol batch exec, and run the output file conversion process.
from __future__ import print_function #compatibilité python 3.0
import sys
import os
import makecomscript
import libsimpa as ls
import xml.dom.minidom  as xmlReader
import comsol_finder as cf
import shutil
import subprocess as sp
import sauve_recsurf_results
from coreConfig import coreConfig
from sauve_recponct_results import SauveRecepteurPonctResults
from sauve_global_results import SauveGlobalResults
from sound_level_layer import SoundLevelLayer
import time
##
# \~english 
# @brief Custom stream encoding conversion
#
# This class is used in the Xml file modification process to fix special characters encoding
class EncodedWriter(file):
    ##
    # Write the sentence in the file with a specific encoding (iso-8859-1)
    # @param data String to convert
    def write(self,data):
        file.write(self,data.encode('iso-8859-1'))

##
# This method return the first son with nodeType==1
def GetImmediateNode( self, subnodename ):
    for item in self.childNodes:
        if item.nodeType==1:
            if  item.tagName==subnodename:
                return item
    return None
xmlReader.Element.GetImmediateNode=GetImmediateNode
#
#
#def GetFloat(strprop):
#    return float(strprop.replace(",","."))

##
# \~english 
# @param xmlFilePath Full path of the Xml file
# @return Tuple (XmlDocument, Boolean)
def LoadXmlFile( xmlFilePath ):
    doc=coreConfig(xmlFilePath)
    return doc,True

##
# \~english 
# This method run the Classical physics core for each source independently and return the results
# @return Dict object with source id in key and sound_level_layer.SoundLevelLayer instance in dict values
def runTC(xmlPathTc,coreconf):
    """
        Cette méthode génére pour chaque source un fichier xml et appel la théorie classique
    """
    if not coreconf.const["ajouter_son_direct"]:
        return {}
    tcpath="core\\theorie_classique\\TheorieClassique.exe"
    
    if not os.path.exists(tcpath):
        print("Impossible de trouver le coeur de calcul de théorie classique !",file=sys.stderr)
        exit()
    #On créé un fichier de configuration par source avec un repertoire de sortie différent
    doc=xmlReader.parse(xmlPathTc)
    simunode=doc.getElementsByTagName('simulation')[0]
    srclstnode=doc.getElementsByTagName('sources')[0]
    sources=srclstnode.getElementsByTagName('source')
    sub_tc=[]
    rootxml=xmlPathTc[0:xmlPathTc.rfind(os.sep)+1]
    for source in sources:
        srclstnode.removeChild(source)
    for source in sources:
        srclstnode.appendChild(source)
        idsource=source.getAttribute('id').encode('cp1252')
        s_xmlfile=rootxml+"config_s%s.xml" % (idsource)
        s_folder='direct_s%s' % (idsource)
        os.mkdir(rootxml+s_folder)
        simunode.setAttribute('output_folder',s_folder)
        simunode.setAttribute('do_angular_weighting','0') #Pas de pondération selon l'angle d'incidence de la source
        writer=EncodedWriter(s_xmlfile,'w')
        doc.writexml(writer,encoding="iso-8859-1")
        writer.close()
        sub_tc.append((int(idsource),s_xmlfile,s_folder))
        srclstnode.removeChild(source)

    for source in sources:
        srclstnode.appendChild(source)
    #Pour chaque source on execute la théorie classique séparement
    resultsModificationLayers={}
    for tc_process in sub_tc:
        print("Execution du programme de théorie classique :\n%s %s" % (tcpath,tc_process[1]))
        sp.check_call([tcpath,tc_process[1]])
        os.remove(tc_process[1])
        resultsModificationLayer=SoundLevelLayer()   
        resultsModificationLayer.LoadData(tc_process[2],coreconf,ls)
        os.removedirs(coreconf.paths["workingdirectory"]+tc_process[2]+os.sep)
        resultsModificationLayers[tc_process[0]]=resultsModificationLayer
    return resultsModificationLayers
##
# \~english 
# Main method
def run():
    xmlPath=sys.argv[1]

    print("Lecture du fichier XML")
    coreconf, success=LoadXmlFile(xmlPath)
    if not success:
        print("Impossible de lire le fichier XML !",file=sys.stderr)
        exit()
        
    scriptfolder=sys.argv[0][:sys.argv[0].rfind("\\")]+"\\"
    resultsModificationLayers=runTC(xmlPath,coreconf)
    
    #extraction du chemin d'accès au dossier du script md.py
    comscript,rslst,mesh,sommets,app_lst=makecomscript.BuildComsolScript( coreconf,
                                     scriptfolder
                                     )
    print("#10")

    print("Copie des fonctions comsol")
    copyarr=["BuildBoundaryStructure.m",
             "BuildSubDomainsStructures.m",
             "SaveRecSurfResults.m",
             "SaveRecPonctResults.m",
             "ChangeFreq.m",
             "SaveGlobalResults.m",
             "grp_face_to_poly.m"
             ]
    mscriptfolder=coreconf.paths["workingdirectory"]+os.sep #+"mscripts"+os.sep
    if not os.path.exists(mscriptfolder):
        os.mkdir(mscriptfolder)
    for fich in copyarr:
        shutil.copy(scriptfolder+fich,mscriptfolder)
    print("Execution de comsol")
    infos_comsol=cf.GetLatestComSolInfos()
    if not infos_comsol:
        print("Impossible de localiser une installation de comsol !",file=sys.stderr)
        exit()
    print("Execution du programme :")
    print("%s %s" % (infos_comsol["path"],comscript))
    oscurdir=os.getcwd()
    os.chdir(coreconf.paths["workingdirectory"])
    sp.check_call([infos_comsol["path"],comscript])
    os.chdir(oscurdir)

    print("#95")
    print("Conversion des formats de fichiers de résultats.")
    rootrecsurffolder=coreconf.paths["workingdirectory"]+coreconf.paths["recepteurss_directory"]
    if len(coreconf.recepteurssurf)>0:
        for idfreq in coreconf.freqid_docalclst:
            freqfold=coreconf.freqlbl[idfreq-1]
            sauve_recsurf_results.SauveRecepteurSurfResults(idfreq,rslst,mesh,sommets,coreconf["simulation"].getpropertyfloat("pasdetemps"),rootrecsurffolder+freqfold,rootrecsurffolder+freqfold+coreconf.paths["recepteurss_filename"],resultsModificationLayers,coreconf)
    SauveRecepteurPonctResults(coreconf,resultsModificationLayers)
    SauveGlobalResults(coreconf,app_lst)
    print("Fin de l'éxécution du calcul.")
    time.sleep(0.5)
run()
