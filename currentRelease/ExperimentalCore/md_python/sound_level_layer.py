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
        
        #------------Code modifié---------------------
        working_directory = coreconf.paths["workingdirectory"]
        if isinstance(working_directory, bytes):
           working_directory = working_directory.decode('utf-8')  # Convertir bytes en str

        if isinstance(folderdirect, bytes):
           folderdirect = folderdirect.decode('utf-8')  # Convertir bytes en str si nécessaire

       # Assurez-vous que os.sep est une chaîne de caractères
        os_sep = os.sep
        if isinstance(os_sep, bytes):
           os_sep = os_sep.decode('utf-8')  # Convertir bytes en str si nécessaire
        othercorepath = working_directory + folderdirect + os_sep
        #------------FIN Code modifié-----------------
        
        #othercorepath = coreconf.paths["workingdirectory"] + folderdirect + os.sep
        if os.path.exists(othercorepath):
            # Chargement des récepteurs ponctuels
            rpfile = othercorepath + "rp.gabe"
            reader = ls.Gabe_rw()
            if os.path.exists(rpfile) and reader.Load(rpfile):#.encode("utf-8")):
                data = reader.ToList()
                for rp in data:
                    self.recp[int(rp[0])] = rp[1:]
            # Chargement des récepteurs de surfaces
            for idrecsurf in coreconf.recepteurssurf.keys():
                # If surface receiver
                filerecsurf = othercorepath + ("rs%i.gabe" % (idrecsurf))
                if os.path.exists(filerecsurf) and reader.Load(filerecsurf):#.encode("utf-8")):
                    recsurf_values = []
                    for idcol in range(0, len(reader)):
                        recsurf_values.append(list(reader.ReadColFloat(idcol)))
                    self.recsurf[idrecsurf] = recsurf_values
                # If cut receiver
                filerecsurf = othercorepath + ("rscut%i.gabe" % (idrecsurf))
                if os.path.exists(filerecsurf) and reader.Load(filerecsurf):#.encode("utf-8")):
                    recsurf_values = []
                    for idcol in range(0, len(reader)):
                        recsurf_values.append(list(reader.ReadColFloat(idcol)))
                    self.recsurf[idrecsurf] = recsurf_values

