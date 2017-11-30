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

## Convert MDF receiver format to I-Simpa receiver format
# @param coreconf Core configuration
def SauveRecepteurPonctResults(coreconf, encoding=sys.getfilesystemencoding()):
    rootpath=os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["recepteursp_directory"])
    for id,recdata in coreconf.recepteursponct.iteritems():
        # print(str(id) + ":" + str(recdata["power_statio"]))
        saveFold = os.path.join(rootpath, recdata["name"]).encode(encoding)
        savepath = os.path.join(rootpath, recdata["name"], coreconf.paths["recepteursp_filename"]).encode(encoding)
        if "power_statio" in recdata:
            gabe_out = ls.Gabe_rw(2)
            stepcol = ls.stringarray()
            stepcol.append("Global")
            gabe_out.AppendStrCol(stepcol, "SPL")
            # For each frequency
            # TODO use not hard writen frequency
            for idFreq, freq in enumerate(
                    [100, 125, 160, 200, 315, 400, 500, 630, 800, 1000, 2000, 3150, 4000, 5000]):
                splcol = ls.floatarray()
                splcol.append(recdata["power_statio"][idFreq])
                gabe_out.AppendFloatCol(splcol, str(freq))
            MakeFolderIfNeeded(saveFold)
            if not gabe_out.Save(savepath):
                print("Failed to save ponctual receiver file")
        # spl=recdata["power"]
        # splcol=ls.floatarray()
        # map(splcol.append,spl)

        # stepcol=ls.stringarray()
        # map(stepcol.append,steplst)
        # gabe_out.AppendStrCol(stepcol,"SPL")
        # gabe_out.AppendFloatCol(splcol,"Global")
        # MakeFolderIfNeeded(rppath)
        # gabe_out.Save(savepath)
