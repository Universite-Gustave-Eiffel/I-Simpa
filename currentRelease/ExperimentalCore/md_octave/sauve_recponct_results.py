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
        saveFold = os.path.join(rootpath, recdata["name"])
        savepath = os.path.join(rootpath, recdata["name"], coreconf.paths["recepteursp_filename"])
        if "power_insta" in recdata:
            gabe_out = ls.Gabe_rw(2)
            stepcol = ls.stringarray()
            if len(recdata["power_insta"]) > 0:
                for step_id in range(len(recdata["power_insta"][0])):
                    stepcol.append("%.1f ms" % (float(coreconf.time_step * step_id * 1000)))
                gabe_out.AppendStrCol(stepcol, "SPL")
                # For each frequency
                # TODO use not hard writen frequency
                for idFreq, freq in enumerate(
                        coreconf.const["frequencies"]):
                    splcol = ls.floatarray()
                    for spl in recdata["power_insta"][idFreq]:
                        splcol.append(float(spl))
                    gabe_out.AppendFloatCol(splcol, "%i Hz" % freq)
                MakeFolderIfNeeded(saveFold)
                if not gabe_out.Save(savepath.decode(encoding).encode("UTF-8")):
                    print("Failed to save ponctual receiver file", file=sys.stderr)
            else:
                print("No data for receiver "+ recdata["name"])