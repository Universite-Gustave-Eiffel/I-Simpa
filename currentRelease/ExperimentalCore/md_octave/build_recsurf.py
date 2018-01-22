# -*- coding: cp1252 -*-
from __future__ import print_function  # compatibilité python 3.0
from libsimpa import vec3
from math import *


##
# @file make_recsurf_ar.py
# \~english 
# This file contain the Surface Receivers methods.

def to_vec3(vec):
    return vec3(vec[0], vec[1], vec[2])

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
        if len(self.faceindex) > 0 and len(self.faceindex[0]) == 4:
            return [(self.vertices[square[1]] + self.vertices[square[3]]) / 2. for square in self.faceindex]
        else:
            return [(to_vec3(self.vertices[triangle[0]]) + to_vec3(self.vertices[triangle[1]]) +
                     to_vec3(self.vertices[triangle[2]])) / 3. for triangle in self.faceindex]

##
# \~english 
# @return Dict with Surface receiver instance list and verticies list
# @param coreconf coreConf.coreConf instance
def GetRecepteurSurfList(coreconf, scene, mesh):
    rslst = {}
    # Ajout de récepteur de surface de coupe
    for (idrs, data) in coreconf.recepteurssurf.items():
        if "resolution" not in data:
            continue
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
        nbvertrow = NbCellU + 1
        nbvertcol = NbCellV + 1
        # Calcul des coordonnées des sommets
        newrs.vertices = [(vertB + (stepU * (idnoderow) + (stepV * (idnodecol)))) for idnoderow in
                          xrange(nbvertrow) for idnodecol in xrange(nbvertcol)]
        # Calcul des indices des sommets des faces triangulaire.
        newrs.faceindex = [[int((idcol + 1 + (idrow * nbvertcol))), int((idcol + (idrow * nbvertcol))),
                            int((idcol + ((idrow + 1) * nbvertcol))), int((idcol + 1 + ((idrow + 1) * nbvertcol)))] for
                           idcol in range(NbCellV) for idrow in range(NbCellU)]
        # Ajout du récepteur surfacique
        rslst[idrs] = newrs
    # Add scene surface receiver
    for tetra in mesh.tetrahedres:
        for idface in range(4):
            face = tetra.getFace(idface)
            if face.marker != -1:
                modelface = scene.faces[face.marker]
                idrs = modelface.idRs
                if idrs != -1:
                    if not rslst.has_key(idrs):
                        rslst[idrs] = rsurf(idrs, coreconf.recepteurssurf[idrs]["name"])
                    surface_struct = rslst[idrs]
                    nodes = [mesh.nodes[face.vertices[0]], mesh.nodes[face.vertices[1]],
                             mesh.nodes[face.vertices[2]]]
                    face_index = []
                    for node in nodes:
                        try:
                            face_index.append(surface_struct.vertices.index(node))
                        except ValueError:
                            # Vertex not in list
                            face_index.append(len(surface_struct.vertices))
                            surface_struct.vertices.append(node)
                    surface_struct.faceindex.append(face_index)

    for rs in rslst.itervalues():
        rs.face_power = [[] for i in range(len(rs.faceindex))]

    return rslst
