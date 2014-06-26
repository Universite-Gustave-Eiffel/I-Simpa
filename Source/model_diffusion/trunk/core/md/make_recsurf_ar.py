# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import libsimpa as ls
from libsimpa import vec3
import sauve_recsurf_results as rpsaver
from tools import *
from math import *
##
# @file make_recsurf_ar.py
# \~english 
# This file contain the Surface Receivers methods.


##
# \~english 
# @param sommets Tuple or list of vec3 instance corresponding to the 3 vertices of a triangle.
# @return The center of the triangle
def GetCentreTri(sommets):
    A,B,C=sommets
    #A=vec3([nodelst[trisommets[0]].node[0],nodelst[trisommets[0]].node[1],nodelst[trisommets[0]].node[2]])
    #B=vec3([nodelst[trisommets[1]].node[0],nodelst[trisommets[1]].node[1],nodelst[trisommets[1]].node[2]])
    #C=vec3([nodelst[trisommets[2]].node[0],nodelst[trisommets[2]].node[1],nodelst[trisommets[2]].node[2]])
    I=(B+C)/2
    AG=(I-A)*(2./3.)
    return A+AG

##
# \~english 
# @brief Surface receiver class
# Contains data about a surface receiver
class rsurf:
    ##
    # \~english 
    # Recsurf constructor
    # @param name Name of the surface receiver
    def __init__(self,name):
        ## List faces with faces vertices coordinates
        self.facelst=[]
        ## List faces with faces vertices index
        self.faceindex=[]
        self.appname=[]
        self.name=name
    ##
    # \~english 
    # Add a receiver triangle to this surface receiver.
    # @param vertpos Tuple or list of size 3 that contains Vec3 instance.
    # @param appname Application name corresponding to the triangle center value evaluation.
    def pushtriface(self,vertpos,appname):
        self.facelst.append([vertpos[0],vertpos[1],vertpos[2]])
        self.appname.append(appname)
    ##
    # \~english 
    # Add a receiver triangle to this surface receiver.
    # @param facesommets Tuple of size 3 containing the vertices index of the face.
    def pushfacesommets(self,facesommets):
        self.faceindex.append([facesommets[0],facesommets[1],facesommets[2]])

##
# \~english 
# Push vertice if it was not previously added in the two first parameters
# @param[in-out] transsommets list, contains vertices index
# @param[in-out] sommets list, contains vertices coordinates
# @param[out] trisommet List with vertices index
# @param[out] coordsommets List with vertices coordinates
def AddSommetIfUsed(transsommets,sommets, trisommet,coordsommets):
    """
        Ajoute dans sommets et transsommets les coordonnées et les indices du sommet si ils n'existents pas et retourne les indices
    """
    retval=[]
    for idsommet,sommet in enumerate(trisommet):
        if transsommets.has_key(sommet):
            retval.append(transsommets[sommet])
        else:
            sommets.append(coordsommets[idsommet])
            transsommets[sommet]=len(sommets)-1
            retval.append(len(sommets)-1)
    return retval
##
# \~english 
# @return Tuple with Surface receiver instance list and verticies list
# @param coreconf coreConf.coreConf instance
# @param domains_lst Domain list object
# @param domains_dict Domain dictionnary with key as Domain Xml Id and value as Domain Indice in the domains_lst list.
# @param appl_lst Application list
# @param mesh 3d mesh of the scene.
def GetRecepteurSurfList(scene,mesh,coreconf,appl_lst,domains_lst, domains_dict):
    """
        Cette méthode est assez lente si il y a beaucoup de mailles
        Il serai possible de passer ceci dans libsimpa afin d'optimiser la recherche
    """
    rslst={}
    transsommets={}
    sommets=[]
    #Ajout des récepteur de surface positionné par les surfaces de la scène
    for tetra in mesh.tetrahedres:
        for face in tetra.tetrafaces:
            if face.marker !=-1:
                idrs=scene.faces[face.marker].idRs
                if idrs!=-1:
                    if not rslst.has_key(idrs):
                        rslst[idrs]=rsurf(coreconf.recepteurssurf[idrs])
                    sommetsCoords=[vec3(sommet.node[0],sommet.node[1],sommet.node[2]) for sommet in [mesh.nodes[idverts] for idverts in face.sommets]]
                    centretri=GetCentreTri(sommetsCoords)
                    appname=GetApplFromPosition( centretri, coreconf.tetmesh, coreconf.splitter, appl_lst, domains_lst, domains_dict )
                    rslst[idrs].pushtriface(centretri,appname)
                    rslst[idrs].pushfacesommets(AddSommetIfUsed(transsommets,sommets,face.sommets,sommetsCoords))
    #Ajout de récepteur de surface de coupe
    for recsurf in coreconf.rootnode["recepteurss"].lstnodesenum("recepteur_surfacique_coupe"):
        idrs=int(recsurf.getproperty("id"))
        newrs=rsurf(idrs)
        vertA=vec3(float(recsurf.getpropertyfloat("ax")),float(recsurf.getpropertyfloat("ay")),float(recsurf.getpropertyfloat("az")))
        vertB=vec3(float(recsurf.getpropertyfloat("bx")),float(recsurf.getpropertyfloat("by")),float(recsurf.getpropertyfloat("bz")))
        vertC=vec3(float(recsurf.getpropertyfloat("cx")),float(recsurf.getpropertyfloat("cy")),float(recsurf.getpropertyfloat("cz")))
        resolution=float(recsurf.getpropertyfloat("resolution"))
        BC=vertC-vertB
        BA=vertA-vertB
        NbCellU=int(ceil(BC.length()/resolution))
        NbCellV=int(ceil(BA.length()/resolution))
        UCellSize=BC.length()/NbCellU
        VCellSize=BA.length()/NbCellV
        stepU=BC/BC.length()*UCellSize
        stepV=BA/BA.length()*VCellSize
        nbvertrow=NbCellU+1;
        nbvertcol=NbCellV+1;
        nodecount=len(sommets)
        #Calcul des coordonnées des sommets
        sommets+=[vertB+(stepU*(idnoderow)+(stepV*(idnodecol))) for idnoderow in range(nbvertrow) for idnodecol in range(nbvertcol)]
        #Calcul des indices des sommets des faces triangulaire.
        facesindex=[[int(nodecount+(idcol+1+(idrow*nbvertcol))),int(nodecount+(idcol+(idrow*nbvertcol))),int(nodecount+(idcol+((idrow+1)*nbvertcol)))] for idcol in range(NbCellV) for idrow in range(NbCellU)]
        facesindex+=[[int(nodecount+(idcol+1+(idrow*nbvertcol))),int(nodecount+(idcol+((idrow+1)*nbvertcol))),int(nodecount+(idcol+1+((idrow+1)*nbvertcol)))] for idcol in range(NbCellV) for idrow in range(NbCellU)]
        map(newrs.pushfacesommets,facesindex)
        #Calcul des coordonnées des points au centre des triangles
        #Et l'application correspondante aux points
        centretri=[GetCentreTri(sommetstri) for sommetstri in [ [sommets[i[0]],sommets[i[1]],sommets[i[2]]] for i in facesindex]]
        appnamelst=[GetApplFromPosition( centreoftri, coreconf.tetmesh, coreconf.splitter, appl_lst, domains_lst, domains_dict ) for centreoftri in centretri]
        [newrs.pushtriface(centreoftri,appname) for centreoftri,appname in zip(centretri,appnamelst)]
        #Ajout du récepteur surfacique
        rslst[idrs]=newrs
    return (rslst,sommets)
