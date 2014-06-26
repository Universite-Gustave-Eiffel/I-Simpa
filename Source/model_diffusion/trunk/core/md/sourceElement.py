# -*- coding: cp1252 -*-
from domainElement import DomainElement
from tools import *

##
# @file sourceElement.py
# \~english 
# This file contain the class sourceElement corresponding to the Comsol's source Sub-Domain class.


##
# \~english 
# @brief Comsol's source Sub-Domain class
# 
# Contain the source Domain properties and formulas.
class sourceElement(DomainElement):
    def InitParams(self,node,coreconfig):
        """
            Initialisation de l'objet
        """
        self.id=int(node.getproperty("id"))
        self.volid=self.id
        self.pos=[node.getpropertyfloat("x"),node.getpropertyfloat("y"),node.getpropertyfloat("z")]
        self.dblist=[0]*len(coreconfig.freqlst)
        for chnode in node.lstnodesenum("bfreq"):
            self.dblist[coreconfig.freqlstassoc[int(chnode.getproperty("freq"))]]=chnode.getpropertyfloat("db")

        if coreconfig.const["temporel"]:
            self.SetDomainParam("f",0)
            domparam=[]
            for freq in self.dblist:
                domparam.append("{'(10^(%f/10))*Dt/(V_source)',0}" % (freq))
            self.SetDomainParam("init",lsttocomsoldict(domparam))
        else:
            domparam=[]
            for freq in self.dblist:
                domparam.append("'(10^(%f/10))/V_source'" % (freq))
            self.SetDomainParam("f",lsttocomsoldict(domparam))
            self.SetDomainParam("init",0)
        self.SetMaterialParam("assoc","[1,1,1,1,1,1,1,1]") #le premier matériau correspond aux frontières interne par défaut
        self.SetMaterialParam("faces","[]")

    def GetComsolElementDeclaration(self):
        return "%s=sphere3('r_source','pos',{'%f','%f','%f'},'axis',{'0','0','1'},'rot','0','const',fem.const);" % (self.name,self.pos[0],self.pos[1],self.pos[2]) #il faut inclure normallement uniquement ce qui correspond au volume

    def PostCreationComputation(self,domains_lst,domains_dict,coreconf):
        """
            Traitements à effectuer une fois que tout les domaines ont été déclarés
        """
        DomainElement.PostCreationComputation(self,domains_lst,domains_dict,coreconf)

        #Il faut retrouver dans quel volume est cette source
        #Pour cela il suffit d'utiliser l'outil splitter
        idtet=coreconf.splitter.GetTetraByPosition( coreconf.tetmesh, self.pos[0],self.pos[1],self.pos[2])
        domain_container_xmlid=coreconf.tetmesh.tetrahedres[idtet].idVolume
        domain_container_locid=domains_dict[domain_container_xmlid]
        domains_lst[domain_container_locid].domains_neigh.append(domains_dict[self.volid])
        self.domains_neigh.append(domain_container_locid)