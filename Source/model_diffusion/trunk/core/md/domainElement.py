# -*- coding: cp1252 -*-
from tools import *

##
# @file domainElement.py
# \~english 
# This file contain the class DomainElement corresponding to the Comsol's Sub-Domain class.


##
# \~english 
# @brief Comsol's Sub-Domain class
# 
# Contain the Domain properties and formulas.
class DomainElement(object):
    """
        Element comportant un volume dans la scène.
    """
    ##
    # \~english
    # Sub-Domain declaration
    # @param name Sub-Domain name
    # @param node XmlDocument node instance
    # @param coreconfig CoreConfiguration instance @see coreConfig.coreConfig
    def __init__(self,name,node,coreconfig):
        """
            Declaration de l'element
            name est le nom de la variable de la geometrie
            node est le noeud XML de cet élément
        """
        ## \~english Dictionnary with Comsol material index as key and face id as value. \~french Ce dictionnaire fait le lien entre les indices des matériaux (ayant des transmissions) avec faces concernées. En fonction de ces données le postprocess modifiera ou non self.mat["assoc"]
        self.mattransfaces={}
        ## \~english Domain parameters @see GetDomainParam @see SetDomainParam
        self.params={}
        ## \~english Dictionnary, @see GetMaterialParam @see SetMaterialParam
        self.mat={}
        ## \~english Sub-Domain name
        self.name=name
        ## \~english Boolean. True if one of the Sub-Domain boundary material is a transmissive material.\~french A vrai si un des materiaux de ce domaine contient une information de transmission, dans ce cas com_app.py déclarera une nouvelle application
        self.rootdomaintransmission=False
        ## \~english Integer. 1 if this subdomain will be used in diffusion propagation \~french 1 si ce domaine est un domaine de propagation
        self.using=1
        
        self.params["names"]="'Domaine de %s'" % (name)
        if coreconfig.const["temporel"]:
            self.params["da"]=1
        else:
            self.params["da"]=0
        self.SetDomainParam("c","'Dif'")
        self.InitParams(node,coreconfig)
        ## \~english Sub-Domain's application (-1 if None) @see SetUsageApplication \~french Correspond au modèle physique associé à ce domaine
        self._application=-1
        self.domains_neigh=[]
    ##
    # \~english
    # Link this Sub-Domain with an application
    def SetUsageApplication(self,idapp):
        self._application=idapp
    ##
    # \~english
    # @return (integer) The Sub-Domain application        
    def GetApplicationUsage(self):
        return self._application
    
    ##
    # \~english
    # @param node XmlDocument node instance
    # @param coreconfig coreConfig.coreConfig instance.
    def InitParams(self,node,coreconfig):
        """
            Initialisation de l'objet
        """
        pass
    
    ##
    # \~english
    # @return The Sub-Domain name
    def GetDomainName(self):
        return self.name
    ##
    # \~english
    # Return the comsol Sub-Domain property (0 if undefined)
    def GetDomainParam(self,paramName):
        if self.params.has_key(paramName):
            return str(self.params[paramName])
        else:
            return "0"
    ##
    # \~english
    # Update a Sub-Domain property
    def SetDomainParam(self,paramName,paramValue):
        self.params[paramName]=paramValue
    ##
    # \~english
    # Return the equivalent comsol declaration in matlab script.
    def GetComsolElementDeclaration(self):
        """
            Retourne la déclaration du domaine
            par exemple pour une source sonore
            g2=sphere3('r_source','pos',{'2','2','2'},'axis',{'0','0','1'},'rot','0','const',fem.const);
        """
        return ""
    ##
    # \~english
    # @param materialparam "assoc" or "faces"
    # @return Link between materials and model faces.
    def GetMaterialParam(self,materialparam):
        """
            Retourne la déclaration du liens des faces avec les matériaux pour la définitions des equations de frontière
            par exemple pour une source sonore
            "assoc":[1,1,1,1,1,1,1,1]   //lien face de l'élément avec l'indice des materiaux
            "faces":[]                  //lien des faces de l'élément avec l'indice des faces déclarées dans le model
        """
        if self.mat.has_key(materialparam):
            return str(self.mat[materialparam])
        else:
            return "0"

    ##
    # \~english
    # Set the Sub-Domain material property
    def SetMaterialParam(self,paramName,paramValue):
        self.mat[paramName]=paramValue

    ##
    # \~english
    # Call when all Sub-Domain's property initialized
    # @param domains_lst Domain list objects
    # @param domains_dict Domains dictionnary with Xml id as keys and list id as values.
    # @param coreconf coreConfig instance.
    def PostCreationComputation(self,domains_lst,domains_dict,coreconf):
        """
            Traitements à effectuer une fois que tout les domaines ont été déclarés
        """
        pass
