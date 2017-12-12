# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import os

class SoundLevelLayer(object):
    """
        Charge le niveaux sonore calculés par les autres méthodes de calculs
    """
    def __init__(self):
        self.recsurf={}
        self.recp={}
    def LoadData(self,folderdirect,coreconf,ls):
        """
            folderdirect Dossier où se trouve le résultat du calcul
            coreconf Classe de configuration du code de calcul
            ls Librairie libsimpa
        """
        othercorepath = coreconf.paths["workingdirectory"] + folderdirect + os.sep
        if os.path.exists(othercorepath):
            # Chargement des récepteurs ponctuels
            rpfile = othercorepath + "rp.gabe"
            reader = ls.Gabe_rw()
            if os.path.exists(rpfile) and reader.Load(rpfile.encode("utf-8")):
                data = reader.ToList()
                for rp in data:
                    self.recp[int(rp[0])] = rp[1:]
            # Chargement des récepteurs de surfaces
            for idrecsurf in coreconf.recepteurssurf.keys():
                filerecsurf = othercorepath + ("rs%i.gabe" % (idrecsurf))
                if os.path.exists(filerecsurf) and reader.Load(filerecsurf):
                    recsurf_values = []
                    for idcol in range(0, len(reader)):
                        recsurf_values.append(list(reader.ReadColFloat(idcol)))
                    self.recsurf[idrecsurf] = recsurf_values
