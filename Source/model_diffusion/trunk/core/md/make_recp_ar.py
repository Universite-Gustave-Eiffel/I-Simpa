# -*- coding: cp1252 -*-
from tools import *

##
# @file make_recp_ar.py
# \~english 
# This file contain the GetPonctualReceiverDeclaration method


##
# \~english 
# @return Comsol script of the ponctual receiver structures.
# @param coreconf coreConf.coreConf instance
# @param domains_lst Domain list object
# @param domains_dict Domain dictionnary with key as Domain Xml Id and value as Domain Indice in the domains_lst list.
# @param appl_lst Application list
def GetPonctualReceiverDeclaration(coreconf,appl_lst,domains_lst, domains_dict):
    comscript=""
    positions=[]
    names=[]
    ids=[]
    freq_lbls=[]
    application_rel=[]

    for rpnode in coreconf.rootnode["recepteursp"].lstnodesenum("recepteur_ponctuel"):
        names.append("'"+rpnode.getproperty("lbl")+"\\'")
        x,y,z=(rpnode.getpropertyfloat("x"),rpnode.getpropertyfloat("y"),rpnode.getpropertyfloat("z"))
        application_rel.append(GetApplFromPosition( [x,y,z], coreconf.tetmesh, coreconf.splitter, appl_lst, domains_lst, domains_dict ))
        positions.append("[%f;%f;%f]" % (x,y,z))
        ids.append(int(rpnode.getproperty("id")))
    for freq in coreconf.freqlst:
        freq_lbls.append("'%s'" % (str(freq)))
    comscript+="rec_poncts.positions=%s;\n" % (lsttocomsoldict(positions))
    comscript+="rec_poncts.names=%s;\n" % (lsttocomsoldict(names))
    comscript+="rec_poncts.freqlbl=%s;\n" % (lsttocomsoldict(freq_lbls))
    comscript+="rec_poncts.folderrponct='%s\\';\n" % (coreconf.paths["recepteursp_directory"])
    comscript+="rec_poncts.appl=%s;\n" % (lsttocomsoldict(application_rel))
    return comscript

