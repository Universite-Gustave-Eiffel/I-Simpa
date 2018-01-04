# -*- coding: cp1252 -*-
from __future__ import print_function  # compatibilité python 3.0
from libsimpa import vec3
from math import *


##
# @file make_recsurf_ar.py
# \~english 
# This file contain the Surface Receivers methods.

##
# \~english 
# @brief Surface receiver class
# Contains data about a surface receiver
class rsurf(object):
    ##
    # \~english 
    # Recsurf constructor
    # @param name Name of the surface receiver
    def __init__(self, index, label):
        ## List vertices coordinates
        self.vertices = []
        ## List faces with faces vertices index
        self.faceindex = []
        self.face_power = []
        self.index = index
        self.label = label
        self.props = {}  # tlmidmic

    def GetSquaresCenter(self):
        return [(self.vertices[square[1]] + self.vertices[square[3]]) / 2. for square in self.faceindex]

##
# \~english 
# @return Dict with Surface receiver instance list and verticies list
# @param coreconf coreConf.coreConf instance
def GetRecepteurSurfList(coreconf):
    rslst = {}
    # Ajout de récepteur de surface de coupe
    for (idrs, data) in coreconf.recepteurssurf.items():
        newrs = rsurf(idrs, data["name"])
        vertA = data["a"]
        vertB = data["b"]
        vertC = data["c"]
        resolution = data["resolution"]
        BC = vertC - vertB
        BA = vertA - vertB
        NbCellU = int(ceil(BC.length() / resolution))
        NbCellV = int(ceil(BA.length() / resolution))
        UCellSize = BC.length() / NbCellU
        VCellSize = BA.length() / NbCellV
        stepU = BC / BC.length() * UCellSize
        stepV = BA / BA.length() * VCellSize
        nbvertrow = NbCellU + 1;
        nbvertcol = NbCellV + 1;
        # Calcul des coordonnées des sommets
        newrs.vertices = [(vertB + (stepU * (idnoderow) + (stepV * (idnodecol)))) for idnoderow in
                          xrange(nbvertrow) for idnodecol in xrange(nbvertcol)]
        # Calcul des indices des sommets des faces triangulaire.
        newrs.faceindex = [[int((idcol + 1 + (idrow * nbvertcol))), int((idcol + (idrow * nbvertcol))),
                            int((idcol + ((idrow + 1) * nbvertcol))), int((idcol + 1 + ((idrow + 1) * nbvertcol)))] for
                           idcol in range(NbCellV) for idrow in range(NbCellU)]
        newrs.face_power = [[] for i in range(NbCellV*NbCellU)]
        # Ajout du récepteur surfacique
        rslst[idrs] = newrs
    return rslst
