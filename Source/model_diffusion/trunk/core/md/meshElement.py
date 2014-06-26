# -*- coding: cp1252 -*-
from domainElement import DomainElement
from tools import *
import math

##
# @file meshElement.py
# \~english 
# This file contain the class meshElement corresponding to the Comsol's Sub-Domain class.

##
# \~english 
# Add one to the parameter.Used as map function parameter.
# @return item+1
def add(item):
    return item+1

##
# \~english 
# @return tuple with local domain polygon id as first element and global polygon id as second element.
def get_poly_tab(domain_faces,poly_faces):
    """
        Retourne La liste des polygones contenue dans le domaine
        Cette fonction nécessite peut être suffisament de ressources pour être inclu dans libsimpa
    """
    polydic={}
    for idface in domain_faces:
        for idpoly in range(0,len(poly_faces)):
            if idface in poly_faces[idpoly]:
                polydic[idpoly]=idface
                break
    return (polydic.keys(),polydic.values())


##
# \~english 
# @brief Comsol's Sub-Domain mesh class
# 
# Contain the Domain mesh properties and formulas.
class meshElement(DomainElement):
    def InitParams(self,node,coreconfig):
        """
            Initialisation de l'objet
        """
        self.diff=[0]*len(coreconfig.freqlst)
        self.SetDomainParam("f",0)
        self.SetDomainParam("init",0)
        absatmo_param=[]
        self.local_m_param=coreconfig.abs_atmo
        #Calcul de l'absorption atmosphérique si demandé par l'utilisateur
        if int(coreconfig.rootnode["condition_atmospherique"].getproperty("disable_absatmo_computation"))==0:
            for m_param in self.local_m_param:
                absatmo_param.append("'%f*cel'" % (m_param))
            self.SetDomainParam("a",lsttocomsoldict(absatmo_param))
        else:
            self.local_m_param=[coreconfig.rootnode["condition_atmospherique"].getpropertyfloat("absatmo")]*len(coreconfig.freqlst)
            self.SetDomainParam("a",str(coreconfig.rootnode["condition_atmospherique"].getpropertyfloat("absatmo")))
    def GetComsolElementDeclaration(self):
        return "%s=geomcoerce('solid',facearr(%s));" % (self.name,self.mat["faces"]) #il faut inclure normallement uniquement ce qui correspond au volume

    def InitFromSplitterData(self,vol_data,scene,coreconf,sceneLoader,polygons):
        """
            Charge "assoc" et "faces" et le paramètre c correspondant aux données provenant de VolSplitter
        """
        self.vol_data=vol_data
        self.volid=vol_data[0]
        #Lecture du paramètre de propagation
        if self.volid in coreconf.subdomains and not coreconf.subdomains[self.volid]["propagation_volume"]:
            self.using=0
        #Calcul de la surface totale du volume
        faces=list(vol_data[2])
        #Extraction des indices de polygones à partir des indices des triangles du domaine
        polyids,polymat=get_poly_tab(faces,polygons)
        self.polyids=polyids
        matfaces=map(add,polyids) #increment de 1 sur chaque cellule
        volume=vol_data[1]
        self.mat["faces"]=matfaces
        totaire=0.
        diff_moyen=[0]*len(coreconf.freqlst)
        mat=[0]*len(polymat)
        count=0
        #Pour chaque polygone on a l'indice d'une de ses faces triangulaire
        for idpoly,idface in zip(polyids,polymat):
            idmat=coreconf.GetMaterialComsolIndiceByXmlMaterialIndice(scene.faces[idface].idMat)
            mat[count]=idmat
            if coreconf.materials[idmat-1].hastransmission:
                self.rootdomaintransmission=True
                if not self.mattransfaces.has_key(idmat):
                    self.mattransfaces[idmat]=[idpoly]
                else:
                    self.mattransfaces[idmat].append(idpoly)
            count+=1
        #pour chaque triangle du volume
        #Calcul du libre parcourt moyen 4V/S
        for idface in faces:
            surfaire=sceneLoader.GetSurfAire(scene,idface)
            totaire+=surfaire
            idmat=coreconf.GetMaterialComsolIndiceByXmlMaterialIndice(scene.faces[idface].idMat)
            for idfreq in range(0,len(coreconf.freqlst)):
               diff_moyen[idfreq]+=coreconf.materials[idmat-1]["diff"][idfreq]*surfaire
            
        param_c=[0]*len(coreconf.freqlst)
        #Calcul de la diffusion, en fonction du libre parcourt moyen et de la diffusion des matériaux.
        for idfreq in range(0,len(coreconf.freqlst)):
            diffmat=diff_moyen[idfreq]/totaire
            K=1.
            if diffmat<1:
                if diffmat<=0.001:
                    diffmat=.001
                K=-2.238*math.log(diffmat)+1.549
            #param_c[idfreq]="'((4*%f/%f)*cel/3)*%f'" % (volume,totaire,K)
            self.diff[idfreq]=((4*volume/totaire)*coreconf.const["cel"]/3)*K
            param_c[idfreq]=str(self.diff[idfreq])
        self.mat["assoc"]=mat
        self.SetDomainParam("c",lsttocomsoldict(param_c))
    def PostCreationComputation(self,domains_lst,domains_dict,coreconf):
        """
            Traitements à effectuer une fois que tout les domaines ont été déclarés
        """
        DomainElement.PostCreationComputation(self,domains_lst,domains_dict,coreconf)
        for idvolxml in self.vol_data[3]:
            self.domains_neigh.append(domains_dict[idvolxml])
            
        #Ce bloc correspond à la duplication de matériau de transmission si un même matériau est utilisé avec deux voisins.
        if self.rootdomaintransmission:
            for mat_trans_id,poly_id_lst in self.mattransfaces.iteritems():
                set_poly=set(poly_id_lst)
                mat_already_used_with_another_domain=False
                for neighid in self.domains_neigh:
                    neigh=domains_lst[neighid]
                    if neigh.mattransfaces.has_key(mat_trans_id):
                        if not set_poly.isdisjoint(neigh.mattransfaces[mat_trans_id]):
                            if not mat_already_used_with_another_domain:
                                coreconf.materials[mat_trans_id-1].LinkWithNewDomainsAsso(self.volid,neigh.volid)
                                mat_already_used_with_another_domain=True
                            else:
                                #Le matériau doit être dupliqué car il sera utilisé dans au moins deux autres modèle physique (com_app)
                                newid=coreconf.DuplicateMaterial(mat_trans_id)
                                coreconf.materials[newid-1].ClearDomainsAsso()
                                coreconf.materials[newid-1].LinkWithNewDomainsAsso(self.volid,neigh.volid)
                                coreconf.materials[newid-1].LinkWithNewDomainsAsso(neigh.volid,self.volid)
                                coreconf.materials[newid-1]["name"]="'Copy dom:%i of %i'" % (self.volid,coreconf.materials[newid-1].idmat_xml)
                                #On affecte ce matériau sur les polygones correspondant
                                idpoly_toupdate=list(set_poly.intersection(neigh.mattransfaces[mat_trans_id]))
                                for idpoly in idpoly_toupdate:
                                    self.mat["assoc"][self.mat["faces"].index(idpoly+1)]=newid
                                    neigh.mat["assoc"][neigh.mat["faces"].index(idpoly+1)]=newid
