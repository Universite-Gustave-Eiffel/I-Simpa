# -*- coding: UTF-8 -*-
from __future__ import print_function #python 3.0
import libsimpa as ls
import os
import math
from operator import itemgetter
import sys
from itertools import imap

def MakeFolderIfNeeded(path):
    list=path.split(os.path.sep)
    complete=""
    if os.path.isabs(path):
        complete=list.pop(0)+os.sep
    while len(list)!=0:
        fold=list.pop(0)
        if not os.path.exists(os.path.join(complete,fold)):
            mkpath=os.path.join(complete,fold)
            os.mkdir(mkpath)
        complete=os.path.join(complete,fold)

def labeling(cell):
    return "%g ms" % (cell*1000)
def powertwo(cell):
    return cell**2

## Convert TLM receiver format to I-Simpa receiver format
# @param coreconf Core configuration
# @param micform File name of receiver ex:R_EsssiMIC%i"
def SauveRecepteurPonctResults(coreconf,micform,outfolder,encoding=sys.getfilesystemencoding(),exportwavs=False):
    rootpath=os.path.join(coreconf.paths["workingdirectory"],coreconf.paths["recepteursp_directory"])
    recfilename=micform
    fullpathrec=os.path.join(outfolder,recfilename)
    if os.path.exists(fullpathrec):
        colstoextract=[0]
        for rec,recdata in coreconf.recepteursponct.iteritems():
            colstoextract.append(recdata["tlmidmic"]+1)
        colstoextract.sort()
        recin=open(fullpathrec,"r")
        ##
        #Extract only ponctual receivers
        rows=[[float(row[idcol]) for idcol in colstoextract] for row in imap(str.rsplit,recin)]
        recin.close()
        steplst=map(labeling,map(itemgetter(0), rows))
        for rec,recdata in coreconf.recepteursponct.iteritems():
            #
            rppath=os.path.join(rootpath,recdata["name"]+os.sep)
            spl=map(powertwo,map(itemgetter(recdata["tlmidmic"]+1), rows))
            splcol=ls.floatarray()
            map(splcol.append,spl)
            gabe_out=ls.Gabe_rw(2) # Global only, FFT unused
            stepcol=ls.stringarray()
            map(stepcol.append,steplst)
            gabe_out.AppendStrCol(stepcol,"SPL")
            gabe_out.AppendFloatCol(splcol,"Global")
            MakeFolderIfNeeded(rppath)
            savepath=(rppath+coreconf.paths["recepteursp_filename"]).encode(encoding)
            gabe_out.Save(savepath)
