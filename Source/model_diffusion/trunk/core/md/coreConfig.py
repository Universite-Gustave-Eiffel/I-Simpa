# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import xmlreader
import sys
import libsimpa
from sourceElement import sourceElement
from tools import *
import math
from copy import deepcopy



##
# @file coreConfig.py
# \~english 
# This file contain the class coreconfig that read the XML file and feed some arrays and dict with data extracted from this file.


##
# \~english 
# @brief Represent a material, in comsol meanings this is a group of boundary representation.
# A material inherit from the dict class, then you can call material_obj[material_property_name] to give material property
# You can find in a material instance :
# - ["q"] Alpha boundary parameter, absorption.
# - ["name"] Material name
# - ["h"] Unknown boundary parameter. Change when switching from static to time dependant state.
# - ["type"] Dirichlet or Neuman boudary type. 'dir' or 'neu' in comsol declaration.
# - ["diff"] Diffusion value. Used when the volume iterate over materials to compute the Sub-Domain diffusion parameter value.
# - ["g"] Transmission parameter, work like a plane emitter.
class material(dict):
    ##
    # \~english 
    # Initialisation of a new material.
    # @param idmat_comsol Array indice of the material (Comsol indice)
    # @param idmat_xml I-Simpa material indice (found in Xml file)
    # @param nbfreq Number of frequencies band in the comsol core input. Usefull to init array that contains frequency dependent data like Absorption,Diffusion or Tau(Transmission)
    def __init__(self,idmat_comsol,idmat_xml,nbfreq):
        self.idmat_comsol=idmat_comsol
        self.idmat_xml=idmat_xml
        self.hastransmission=False
        self.domainasso={}
    ##
    # \~english 
    # @param weakening Weakening in dB.
    # @return The Tau value
    def ComputeTauFromWeakening(self,weakening):
        self.hastransmission=True
        return 10.**(-weakening/10.)
    ##
    # \~english
    # A Sub-Domain duplicate the material when a material is linked with two other subdomain, in case of transmission.
    # This method help to say if this is the case.
    # @param vol1 Sub-Domain Id.
    # @param vol2 Sub-Domain Id.
    # @see ClearDomainsAsso
    def LinkWithNewDomainsAsso(self,vol1,vol2):
        self.domainasso[vol1]=vol2
    ##
    # \~english
    # Clear domain association
    # @see LinkWithNewDomainsAsso
    def ClearDomainsAsso(self):
        self.domainasso={}
        
##
# \~english
# @brief Core Configuration
#
# Read the XML file and feed some arrays and dict with data extracted from this file.
class coreConfig(object):
    ##
    # \~english 
    # @param xmlfilepath The path and file name of the Xml file.
    def __init__(self,xmlfilepath):
        ## Xml root node. Use this member to give access to the raw input data of the core.
        self.rootnode=xmlreader.readXmlFile(xmlfilepath)
        self.lastsearch_xmlid=-500      #optimisation recherche d'indice de correspondance
        self.lastsearch_comsolid=-500   #optimisation recherche d'indice de correspondance
        ## A list of coreConfig.material instance.
        self.materials=[]
        ## A list of sourceElement.sourceElement instance.
        self.sources_lst=[]
        ## None or libsimpa.VolumesSplitter instance.
        self.splitter=None #Outil splitter, ajouté plus tard
        ## None or Model tetrahedral mesh data.
        self.tetmesh=None       #Maillage
        ## None or 3D model data.
        self.scene=None         #Scene 3D
        ## Dictionnary with Xml Id in Key and Surface Receiver name in values.
        self.recepteurssurf={} # clé: indice valeur: nom du récepteur surf
        ## Dictionnary giving path information.
        self.paths={}
        ## Main simulation simple data
        self.const={}
        ## A list, contains the frequencies values in Hz.
        self.freqlst=[]
        ## A dictionnary that return the frequency index(value) from the frequency value in hertz(key)
        self.freqlstassoc={}
        ## A list, only the frequency user selected, contains the coreConfig.freqlst list index.
        self.freqid_docalclst=[]
        ## Fittings data. A dictionnary with fittings Xml id in key and fitting Xml node in values.
        self.fittings={}
        ## A dictionnary with Volume(Sub-Domain) Xml id in key and another dictionnary in value. @see coreConfig.coreConfig.load_subdomains
        self.subdomains={}
        ## A list that contain the m parameter values. Atmospheric absorption computed from temperature, air pressure etc..
        self.abs_atmo=[]
        
        
        
        
        simunode=self.rootnode["simulation"]
        condition_atmospherique_node=self.rootnode["condition_atmospherique"]
        self.paths["workingdirectory"]=self.rootnode.getproperty("workingdirectory")
        self.paths["modelName"]=simunode.getproperty("modelName")
        self.paths["tetrameshFileName"]=simunode.getproperty("tetrameshFileName")
        self.paths["recepteurss_directory"]=self.rootnode.getproperty("recepteurss_directory")
        self.paths["recepteurss_filename"]=self.rootnode.getproperty("recepteurss_filename")
        self.paths["recepteursp_directory"]="Récepteurs_Ponctuels"
        self.paths["recepteursp_filename"]="soundpressure.recp"
        self.const["cel"]=libsimpa.CCalculsGenerauxThermodynamique.c_son(condition_atmospherique_node.getpropertyfloat("temperature")+273.15)
        self.const["temporel"]=(int(self.rootnode["simulation"].getproperty("solver_mode"))==0)
        self.const["temperature_celsius"]=condition_atmospherique_node.getpropertyfloat("temperature")
        self.const["temperature_kelvin"]=condition_atmospherique_node.getpropertyfloat("temperature")+273.15
        self.const["humidite"]=condition_atmospherique_node.getpropertyfloat("humidite")
        self.const["pression"]=condition_atmospherique_node.getpropertyfloat("pression")
        self.const["pasdetemps"]=simunode.getpropertyfloat("pasdetemps")
        self.const["duree_simulation"]=simunode.getpropertyfloat("duree_simulation")
        self.const["nbpasdetemps"]=int(self.const["duree_simulation"]/self.const["pasdetemps"])
        self.const["ajouter_son_direct"]=bool(int(simunode.getproperty("with_direct_sound")))
        self.load_freqlst(simunode["freq_enum"])
        #chargement des libellés des récepteur de surface
        for recsurf in self.rootnode["recepteurss"].lstnodesenum("recepteur_surfacique"):
            self.recepteurssurf[int(recsurf.getproperty("id"))]=recsurf.getproperty("name")
        for recsurf in self.rootnode["recepteurss"].lstnodesenum("recepteur_surfacique_coupe"):
            self.recepteurssurf[int(recsurf.getproperty("id"))]=recsurf.getproperty("name")
        self.load_materials()
        self.load_sources(self.rootnode["sources"])
        self.load_fittings()
        self.load_subdomains()
    ##
    # \~english 
    # Feed the following list : 
    # - freqlstassoc
    # - freqlst
    # - abs_atmo
    # - freqid_docalclst
    # @param freqenumnode Root frequency Xml Node
    def load_freqlst(self,freqenumnode):
        computation={}
        for freq in freqenumnode.lstnodesenum("bfreq"):
            freqval=int(freq.getproperty("freq"))
            self.freqlst.append(freqval)
            computation[freqval]=bool(int(freq.getproperty("docalc")))
        self.freqlst.sort()
        temperature_k=self.const["temperature_kelvin"]
        humidite=self.const["humidite"]
        pression=self.const["pression"]
        for idfreq in range(0,len(self.freqlst)):
            freqval=self.freqlst[idfreq]
            self.freqlstassoc[freqval]=idfreq
            if computation[freqval]:
                self.freqid_docalclst.append(idfreq+1)
            self.abs_atmo.append(libsimpa.CCalculsGenerauxThermodynamique.Coef_Att_Atmos( freqval, humidite, pression, temperature_k)*math.log(10.)/10.)
    ##
    # \~english 
    # Feed the sources_lst member.
    # @param nodesources Root source Xml Node
    def load_sources(self,nodesources):
        for source in nodesources.lstnodesenum("source"):
              self.sources_lst.append(sourceElement("s%i" %(len(self.sources_lst)+1),source,self))
    ##
    # \~english
    # Feed the fittings list
    def load_fittings(self):
        for fitting in self.rootnode["encombrement_enum"].lstnodesenum("encombrement"):
            self.fittings[int(fitting.getproperty("id"))]=fitting
    ##
    # \~english 
    # Feed the subdomains list
    def load_subdomains(self):
        for subdomain in self.rootnode["subdomains"].lstnodesenum("volume"):
            self.subdomains[int(subdomain.getproperty("id"))]={"name" : subdomain.getproperty("name"), "fcompute_mean_free_path" :  bool(int(subdomain.getproperty("fcompute_mean_free_path"))),  "mean_free_path" :  subdomain.getpropertyfloat("mean_free_path"), "propagation_volume" : bool(int(subdomain.getproperty("propagation_volume")))}
        
    ##
    # \~english 
    # Navigate through the Materials node and feed the mat dictionnary with material instance.
    def load_materials(self):
        #Ajout du matériau pour les sources
        mat=material(1,-1,len(self.freqlst))
        mat["q"]=lsttocomsoldict([0]*len(self.freqlst))
        mat["name"]="'Materiau sources'"
        mat["h"]=1
        mat["type"]="'neu'"
        self.materials.append(mat)
        self.materials_corresp={}
        #Lecture des matériaux du fichier XML
        for matnode in self.rootnode["surface_absorption_enum"].lstnodesenum("type_surface"):
            xmlidmat=int(matnode.getproperty("id"))
            mat=material(len(self.materials)+1,xmlidmat,len(self.freqlst))
            
            q_param=[]
            diff_param=[]
            g_param=[]
            #On demande de trier les noeuds par bande de fréquence
            matnode.SortChildsByProperty("bfreq","freq",True)
            containsTau=False
            for idfreq,chnode in enumerate(matnode.lstnodesenum("bfreq")):
                q_param.append("'cel*%.4f/4'" % (chnode.getpropertyfloat("absorb")))
                diff_param.append(chnode.getpropertyfloat("diffusion"))
                if chnode.hasproperty("affaiblissement"):
                    g_param.append("'cel*%.4f*u/4'" % (mat.ComputeTauFromWeakening(chnode.getpropertyfloat("affaiblissement"))))
                    containsTau=True
                else:
                    g_param.append("0");
            mat["diff"]=diff_param
            mat["q"]=lsttocomsoldict(q_param)
            if containsTau>0:
                mat["g"]=lsttocomsoldict(g_param)
            mat["name"]="'mat id:%i'" % (xmlidmat)
            mat["h"]=0
            mat["type"]="'dir'" #matériau de frontière extérieur

            self.materials_corresp[xmlidmat]=len(self.materials)+1
            self.materials.append(mat)

    ##
    # \~english 
    # Find the corresponding material comsol Id from the Xml material id.
    # @param materialxmlind Xml material Id
    # @return Integer [0-n] Comsol Script material ID
    def GetMaterialComsolIndiceByXmlMaterialIndice(self,materialxmlind):
        """
            Retourne l'indice du matériau déclaré dans comsol à partir de l'indice xml du matériau
        """
        if self.lastsearch_xmlid==materialxmlind:
            return self.lastsearch_comsolid
        else:
            self.lastsearch_xmlid=materialxmlind
            self.lastsearch_comsolid=self.materials_corresp[materialxmlind]
            return self.lastsearch_comsolid
    ##
    # \~english 
    # Duplication of the material.
    # @param comsol_mat_id Comsol material Id
    # @return The new comsol material id.
    def DuplicateMaterial(self,comsol_mat_id):
        self.materials.append(deepcopy(self.materials[comsol_mat_id-1]))
        return len(self.materials)
    ##
    # \~english 
    def __getitem__(self,item_name):
        return self.rootnode[item_name]
    ##
    # \~english 
    def __len__(self):
        return len(self.rootnode)
    ##
    # \~english 
    def __contains__(self,val):
        return self.rootnode.has_key(val)
    
