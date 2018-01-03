# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import xmlreader
import libsimpa
from libsimpa import vec3
import math
from build_recsurf import GetRecepteurSurfList

##
# @file coreConfig.py
# \~english 
# This file contain the class coreconfig that read the XML file and feed some arrays and dict with data extracted from this file.


class SourceElement:
    def __init__(self):
        self.pos=libsimpa.vec3()
        self.id=-1
    def __init__(self,node, db):
        self.pos=libsimpa.vec3(node.getpropertyfloat("x"),node.getpropertyfloat("y"),node.getpropertyfloat("z"))
        self.id=int(node.getproperty("id"))
        self.db = db

class material(dict):
    ##
    # \~english 
    # Initialisation of a new material.
    # @param idmat_xml I-Simpa material indice (found in Xml file)
    # @param nbfreq Number of frequencies band in the comsol core input. Usefull to init array that contains frequency dependent data like Absorption,Diffusion or Tau(Transmission)
    def __init__(self,idmat_comsol,idmat_xml,nbfreq):
        self.idmat_xml=idmat_xml
        self.hastransmission=False
        
        
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
        ## A list of coreConfig.material instance. Key: XmlIdMaterial
        self.materials={}
        ## A list of sourceElement.sourceElement instance.
        self.sources_lst=[]
        ## None or 3D model data.
        self.scene=None
        ## Dictionnary with Xml Id in Key and Surface Receiver name in values.
        self.recepteursponct={}
        self.recepteurssurf={}
        self.tlm_receivers=0 #Mixing ponctual and surface receivers cells. Keep the same order in the mic.dat file.
        ## Dictionnary giving path information.
        self.paths={}
        ## Main simulation simple data
        self.const={}
        ## A list, contains the frequencies values in Hz.
        self.freqlst=[]
        ## A dictionnary that return the frequency index(value) from the frequency value in hertz(key)
        self.freqlstassoc = {}
        ## A list, only the frequency user selected, contains the coreConfig.freqlst list index.
        self.freqid_docalclst = []
        ## Fittings data. A dictionnary with fittings Xml id in key and fitting Xml node in values.
        self.fittings = {}
        ## A list that contain the m parameter values. Atmospheric absorption computed from temperature, air pressure etc..
        self.abs_atmo = []
        self.tr = libsimpa.vec3()  # translation of model between python and fortran code
        simunode = self.rootnode["simulation"]
        condition_atmospherique_node = self.rootnode["condition_atmospherique"]
        self.paths["workingdirectory"] = self.rootnode.getproperty("workingdirectory")
        self.paths["modelName"] = simunode.getproperty("modelName")
        self.paths["tetrameshFileName"] = simunode.getproperty("tetrameshFileName")
        self.paths["recepteurss_directory"] = simunode.getproperty("recepteurss_directory")
        self.paths["recepteurss_filename"] = simunode.getproperty("recepteurss_filename")
        self.paths["recepteursp_directory"] = simunode.getproperty("receiversp_directory")
        self.paths["recepteursp_filename"] = simunode.getproperty("receiversp_filename")
        self.const["cel"] = libsimpa.CCalculsGenerauxThermodynamique.c_son(
            condition_atmospherique_node.getpropertyfloat("temperature") + 273.15)
        self.const["temperature_celsius"] = condition_atmospherique_node.getpropertyfloat("temperature")
        self.const["temperature_kelvin"] = condition_atmospherique_node.getpropertyfloat("temperature") + 273.15
        self.const["humidite"] = condition_atmospherique_node.getpropertyfloat("humidite")
        self.const["frequencies"] = [100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500,
                                     3200, 4000, 5000]
        self.const["allfrequencies"] = [50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000,
         5000, 6300, 8000, 10000, 12500, 16000, 20000]
        self.const["pression"] = condition_atmospherique_node.getpropertyfloat("pression")
        self.const["with_direct_sound"] = bool(int(simunode.getproperty("with_direct_sound", "1")))
        self.const['timestep'] = simunode.getpropertyfloat('pasdetemps', 0.01)
        self.const['tolerance'] = simunode.getpropertyfloat('tolerance', 1e-6)
        self.const['maxint'] = simunode.getpropertyint('maxint', 200)
        self.const['duration'] = simunode.getpropertyfloat('duree_simulation', 2)
        self.const['do_abs_atmo'] = bool(int(simunode.getproperty("do_abs_atmo", "1")))

        # Load surface receiver
        for recsurf in self.rootnode["recepteurss"].lstnodesenum("recepteur_surfacique_coupe"):
            self.recepteurssurf[int(recsurf.getproperty("id"))]={ "name": recsurf.getproperty("name"), "a" : vec3(float(recsurf.getpropertyfloat("ax")),float(recsurf.getpropertyfloat("ay")),float(recsurf.getpropertyfloat("az"))), "b" :vec3(float(recsurf.getpropertyfloat("bx")),float(recsurf.getpropertyfloat("by")),float(recsurf.getpropertyfloat("bz"))) , "c" : vec3(float(recsurf.getpropertyfloat("cx")),float(recsurf.getpropertyfloat("cy")),float(recsurf.getpropertyfloat("cz"))), "resolution" : recsurf.getpropertyfloat("resolution") }
        for recponct in self.rootnode["recepteursp"].lstnodesenum("recepteur_ponctuel"):
            self.recepteursponct[int(recponct.getproperty("id"))] = {"name": recponct.getproperty("name"),
                                                                     "power_statio": [],
                                                                     "power_insta": [],
                                                                     "pos": libsimpa.vec3(
                                                                         recponct.getpropertyfloat("x"),
                                                                         recponct.getpropertyfloat("y"),
                                                                         recponct.getpropertyfloat("z"))}
        self.load_materials()
        self.load_sources(self.rootnode["sources"])
        self.load_fittings()
        self.recsurf=GetRecepteurSurfList(self)
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
        for sourcenode in nodesources.lstnodesenum("source"):
            db=[]
            #On demande de trier les noeuds par bande de fréquence
            sourcenode.SortChildsByProperty("bfreq","freq",True)
            for idfreq,chnode in enumerate(sourcenode.lstnodesenum("bfreq")):
                db.append(chnode.getpropertyfloat("db"))
            self.sources_lst.append(SourceElement(sourcenode, db))
    ##
    # \~english
    # Feed the fittings list
    def load_fittings(self):
        for fitting in self.rootnode["encombrement_enum"].lstnodesenum("encombrement"):
            self.fittings[int(fitting.getproperty("id"))]=fitting
       
    ##
    # \~english 
    # Navigate through the Materials node and feed the mat dictionnary with material instance.
    def load_materials(self):
        #Lecture des matériaux du fichier XML
        for matnode in self.rootnode["surface_absorption_enum"].lstnodesenum("type_surface"):
            xmlidmat=int(matnode.getproperty("id"))
            mat=material(len(self.materials)+1,xmlidmat,len(self.freqlst))
            
            q_param=[]
            diff_param=[]
            g_param=[]
            #On demande de trier les noeuds par bande de fréquence
            matnode.SortChildsByProperty("bfreq","freq",True)
            for idfreq,chnode in enumerate(matnode.lstnodesenum("bfreq")):
                q_param.append(chnode.getpropertyfloat("absorb"))
                diff_param.append(chnode.getpropertyfloat("diffusion"))
                if chnode.hasproperty("affaiblissement"):
                    g_param.append(chnode.getpropertyfloat("affaiblissement"))
                else:
                    g_param.append(0);
            mat["diff"]=diff_param
            mat["q"]=q_param
            mat["g"]=g_param
            mat["name"]="'mat id:%i'" % (xmlidmat)
            mat["h"]=0
            self.materials[xmlidmat]=mat

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
    
