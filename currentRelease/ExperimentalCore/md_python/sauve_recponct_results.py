# -*- coding: UTF-8 -*-
from __future__ import print_function #python 3.0
import libsimpa as ls
import os
import math
from operator import itemgetter
import sys
from sys import exit
#from itertools import imap

def MakeFolderIfNeeded(path):
    #modifs 27/08/2024
    if type(path)==bytes:
        path=path.decode('utf-8')
    #fin modifs
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
    for id,recdata in coreconf.recepteursponct.items():
        saveFold = os.path.join(rootpath, recdata["name"])
        savepath = os.path.join(rootpath, recdata["name"], coreconf.paths["recepteursp_filename"])
        #print("Recdata = ", recdata)
        if "power_insta" in recdata:
            gabe_out = ls.Gabe_rw(2)
            stepcol = ls.stringarray()
            if len(recdata["power_insta"]) > 0:
                if not coreconf.const['stationary']:
                    for step_id in range(len(recdata["power_insta"][0])):
                        stepcol.append("%.1f ms" % (float(coreconf.time_step * step_id * 1000)))
                else:
                    stepcol.append("Stationary")
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
                if not gabe_out.Save(savepath.decode('utf-8')):#.decode(encoding).encode("UTF-8")):
                    print("Failed to save ponctual receiver file", file=sys.stderr)
            else:
                
                #modifs code
                name=recdata["name"]
                if isinstance(name, bytes):
                  name = name.decode('utf-8')
                #fin modifs code
                
                print("No data for receiver "+ name)