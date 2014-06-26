# -*- coding: cp1252 -*-
"""
 Une application correspond à un modèle physique
 
 Pour la transmission, il y a un modèle physique de chaque coté des parois de transmission. Autrement il n'y a qu'une seule application regroupant tout les sous-domaines.
"""
##
# @file com_app.py
# \~english 
# @brief Define the comsol physics (Application) entity
#
# Each comsol solver(fem) input can contain one or many application. There is an application by side of transmission domains.
# \~french
# @brief Défini l'entité d'application.
# 
# Comsol peut contenir une ou plusieurs application. Chaque paroi semi-transparente rajoute une application.

from __future__ import print_function
from tools import *
import sys
##
# \~english 
# @brief Define the comsol physics (Application) entity
#
# Each comsol solver(fem) input can contain one or many application. There is an application by side of transmission domains.
# \~french
# @brief Défini l'entité d'application.
# 
# Comsol peut contenir une ou plusieurs application. Chaque paroi semi-transparente rajoute une application.
class Application(object):
    ##
    # \~english
    # Constructor
    # @param idapp Application Id [1-n]
    # @param domain_count [n] Size of the fem application list
    def __init__(self,idapp,domain_count):
        ## Array of included sub_domains. Correspond to the appl.equ.usage var
        self.inner_subdomains_volid=[]
        ## [1-n] Application Id
        self.idapp=idapp
        ## "c[2-n]" Application name appl.name
        self.nameapp="c"
        ## "u[2-n]" Application name appl.name
        self.varapp="u"
        ## "dom[1-n]" Application name appl.name
        self.outname="dom%i" % (idapp)
        self.usage=[0]*domain_count
        if self.idapp>1:
            self.nameapp+=str(self.idapp)
            self.varapp+=str(self.idapp)
    ##
    # \~english
    # Recursive method. Merge all sub-domains where inner boundary material is not transmissive.
    # feed the self.inner_subdomains_volid var.
    def ExpandUsageToNeighboors(self,rootdomainid,domains_lst):
        """
            Une application contient un ensemble de sous-domaine n'étant pas séparé par des parois de transmission.
            Cette méthode permet de séléctionner les sous-domaines qui seront contenu dans l'application.
        """
        domains_lst[rootdomainid].SetUsageApplication(self.idapp)
        self.inner_subdomains_volid.append(domains_lst[rootdomainid].volid)
        self.usage[rootdomainid]=domains_lst[rootdomainid].using
        for neighboor in domains_lst[rootdomainid].domains_neigh: #vol_data[3] correspond a la liste des domaines voisins voir VolumesSplitter::GetVolumeInformation()
            if domains_lst[neighboor].GetApplicationUsage()==-1:
                if not domains_lst[neighboor].rootdomaintransmission: #Le domaine voisin n'a aucune surface de transmission
                    self.ExpandUsageToNeighboors(neighboor,domains_lst)
                else:
                    #Le domaine voisin a une surface de transmission
                    #Toutefois on peut ajouter ce voisin à notre application si les surfaces en commun ne sont pas des surfaces de transmissions.
                    same_transface=False
                    for material_id, poly_id in domains_lst[rootdomainid].mattransfaces.iteritems():
                        if len(set(poly_id).intersection(set(domains_lst[neighboor].polyids)))>0: #Nos surfaces
                            same_transface=True
                            break
                    if not same_transface:
                        self.ExpandUsageToNeighboors(neighboor,domains_lst)
    ##
    # \~english
    # Return the matlab script "appl.bnd.gallfreq=.." that will replace the u variable by the specific u(n) variable.
    # @param coreconf coreConf.coreConf instance
    # @param domains_lst Domain list object
    # @param domains_dict Domain dictionnary with key as Domain Xml Id and value as Domain Indice in the domains_lst list.
    # @param app_lst Application list
    # \~french
    # Utilisé pour les matériaux de transmission. On modifie la variable U en fonction du voisins. cel*tau*u/4 -> cel*tau*u(n)/4
    def GetDomainDependentMaterialDeclaration(self,coreconf,domains_lst,domains_dict,app_lst):
        """
            Utilisé pour les matériaux de transmission. On modifie la variable U en fonction du voisins. cel*tau*u/4 -> cel*tau*u(n)/4
        """
        proplst=[]
        for mat in coreconf.materials:
            mat_decl=mat.get('g',0)#lit la propriété prop, ou affecte 0 si inexistant
            if mat_decl!=0:
                match_domains_keys= list(set(mat.domainasso.keys()).intersection(self.inner_subdomains_volid))
                if len(match_domains_keys)>=1:
                    #print("App pyid:"+str(domains_lst[domains_dict[mat.domainasso[match_domains_keys[0]]]].GetApplicationUsage()))
                    neigh_varappname=app_lst[domains_lst[domains_dict[mat.domainasso[match_domains_keys[0]]]].GetApplicationUsage()-1].varapp
                    #print("Loc varname:%s NeighVarname:%s\n" % (self.varapp,neigh_varappname))
                    mat_decl=mat_decl.replace("u",neigh_varappname)
                else:
                    
                    #print("Transmission material with outside boundary !",file=sys.stderr)
                    #print("mat[\"name\"]"+mat["name"],file=sys.stderr)
                    #print("mat.domainasso"+str(mat.domainasso),file=sys.stderr)
                    #print("self.inner_subdomains_volid"+str(self.inner_subdomains_volid),file=sys.stderr)
                    mat_decl=0
            proplst.append(mat_decl) 
        return lsttocomsoldict(proplst)
    ##
    # \~english
    # @param coreconf coreConfig.coreConfig instance
    # Read the most appropriate application name from XML configuration. It will use one of the application's volume name, the first one by default.
    def SetApplicationOutputName(self,coreconf):
        for volid in self.inner_subdomains_volid:
            if coreconf.subdomains.has_key(volid):
                self.outname=coreconf.subdomains[volid]["name"]
                break
    ##
    # \~english
    # @param coreconf coreConfig.coreConfig instance
    # @param domains_lst Domain list object
    # @param domains_dict Domain dictionnary with key as Domain Xml Id and value as Domain Indice in the domains_lst list.
    # @param app_lst Application list
    # @return The comsol script corresponding to the declaration of this application.
    def GetDeclaration(self,coreconf,domains_lst,domains_dict,app_lst):
        self.SetApplicationOutputName(coreconf)
        appbnd_decl="fem.appl{1}.bnd"
        appequ_decl="fem.appl{1}.equ"
        usage_decl=lsttocomsoldict(self.usage)
        if self.idapp==1:
            appbnd_decl="BuildBoundaryStructure(ft,mat)"
            appequ_decl="BuildSubDomainsStructures(st,vol)"
        decl="""\n\n%% Application mode %i
clear prop
prop.elemdefault='Lag1';
clear appl
appl.mode.class = 'FlPDEC';
appl.dim = {'%s','%s_t'};
appl.name = '%s';
appl.assignsuffix = '_%s';
appl.bnd = %s;
appl.bnd.gallfreq=%s;
appl.equ = %s;
appl.equ.usage = %s;
appl.prop = prop;
fem.appl{%i} = appl;""" % (self.idapp,
            self.varapp,self.varapp,
            self.nameapp,
            self.nameapp,
            appbnd_decl,
            self.GetDomainDependentMaterialDeclaration(coreconf,domains_lst,domains_dict,app_lst),
            appequ_decl,
            usage_decl,
            self.idapp)
        return decl
    
##
# \~english
# @param coreconf Project configuration object
# @param domains_lst Domain list object
# @param scene libsimpa.ioModel instance
# @param mesh libsimpa.mesh_reader.LoadMesh instance
# @param polygons List that contains sublist with triangle id.
# @return The comsol script corresponding to the declaration of this application.
def MakeApps(domains_lst,scene,mesh,coreconf,polygons):
    """
     Construit la liste des modèles physique en fonction des matériaux des domaines.
     S'il n'y a aucune surface avec de la transmission alors il n'y aura qu'un seul modèle physique.
    
     @param domains_lst Liste des sous-domaines
     @param scene Scene 3D
     @param mesh Maillage
     @param coreconf Paramètres de configuration
     @param polygons Association triangles/polygone
    """
    app_lst=[]
    for idomain,domain in enumerate(domains_lst):
        if domain.GetApplicationUsage()==-1 and domain.rootdomaintransmission:
            #Ce domaine a une paroi en transmission avec un autre domaine
            newapp=Application(len(app_lst)+1,len(domains_lst))
            app_lst.append(newapp)
            #il faut ajouter tout les domaines liés à ce domaine et qui ne sont pas rootdomain (récursivement)
            newapp.ExpandUsageToNeighboors(idomain,domains_lst)
    for idomain,domain in enumerate(domains_lst):
        if domain.GetApplicationUsage()==-1:
            #Ce domaine a une paroi en transmission avec un autre domaine
            newapp=Application(len(app_lst)+1,len(domains_lst))
            app_lst.append(newapp)
            #il faut ajouter tout les domaines liés à ce domaine et qui ne sont pas rootdomain (récursivement)
            newapp.ExpandUsageToNeighboors(idomain,domains_lst)
    return app_lst
