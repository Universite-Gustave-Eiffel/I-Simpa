# -*- coding: cp1252 -*-
from meshElement import meshElement
from tools import *
##
# @file fittingElement.py
# \~english 
# This file contain the class fittingElement


##
# \~english 
# @brief Comsol's fitting Sub-Domain class
# 
# Contain the fitting Sub-Domain properties and formulas.
class fittingElement(meshElement):
    def InitParams(self,node,coreconfig):
        """
            Initialisation de l'objet
        """
        meshElement.InitParams(self,node,coreconfig)
    def InitFromSplitterData(self,vol_data,scene,coreconf,sceneLoader,polygons):
        """
        
            Charge "assoc" et "faces" et le paramètre c correspondant aux données provenant de VolSplitter
        """
        self.volxmlneigh=list(vol_data[3]) #Les volumes voisins seront utilisés dans PostCreationComputation
        meshElement.InitFromSplitterData(self,vol_data,scene,coreconf,sceneLoader,polygons)
        #Calcul de la diffusion de l'encombrement
        fittingNode=coreconf.fittings[self.volid]
        a_param=[0]*len(coreconf.freqlst)
        self.diff_f=[0]*len(coreconf.freqlst)
        for bfreq in fittingNode.lstnodesenum("bfreq"):
            idfreq=coreconf.freqlstassoc[int(bfreq.getproperty("freq"))]
            lambda_f=bfreq.getpropertyfloat("lambda")
            alpha_f=bfreq.getpropertyfloat("alpha")
            self.diff_f[idfreq]=lambda_f*coreconf.const["cel"]/3.
            #calcul de l'absorption atmosphérique et l'absorption de l'encombrement
            a_param[idfreq]=str(coreconf.const["cel"]*(self.local_m_param[idfreq]+alpha_f/lambda_f))
        self.SetDomainParam("a",lsttocomsoldict(a_param))
        

    def PostCreationComputation(self,domains_lst,domains_dict,coreconf):
        """
            Traitements à effectuer une fois que tout les domaines ont été déclarés
        """
        meshElement.PostCreationComputation(self,domains_lst,domains_dict,coreconf)
        #1ere etape, on recherche le volume voisin (on prend le premier de la liste self.volxmlneigh)
        if len(self.volxmlneigh)==0:
            return
        neigh=None
        for neigh_candidate in domains_lst:
            if neigh_candidate.volid==self.volxmlneigh[0]:
                neigh=neigh_candidate
                break
        #On calcul le coefficient de diffusion total en fonction du coeff du volume principal et du coeff de l'encombrement
        diff_tot=[0]*len(coreconf.freqlst) 
        for idfreq in coreconf.freqlstassoc.values():
            diff_tot[idfreq]=str(neigh.diff[idfreq]*self.diff_f[idfreq]/(neigh.diff[idfreq]+self.diff_f[idfreq]))
        self.SetDomainParam("c",lsttocomsoldict(diff_tot))