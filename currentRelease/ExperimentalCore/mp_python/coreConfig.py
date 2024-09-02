# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import xmlreader
import libsimpa
from libsimpa import vec3
import math

##
# @file coreConfig.py
# \~english 
# This file contain the class coreconfig that read the XML file and feed some arrays and dict with data extracted from this file.



        
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
        # self.const["frequencies"] = [int(freq.getproperty("freq")) for freq in self.rootnode["simulation"]["freq_enum"].lstnodesenum("bfreq") if freq.getproperty("docalc") == "1"]
        # self.const["frequencies"].sort()
        # self.const["allfrequencies"] = [int(freq.getproperty("freq")) for freq in self.rootnode["simulation"]["freq_enum"].lstnodesenum("bfreq")]
        # self.const["allfrequencies"].sort()
        self.const["pression"] = condition_atmospherique_node.getpropertyfloat("pression")
        self.const["with_direct_sound"] = bool(int(simunode.getproperty("with_direct_sound", "1")))
        self.const['timestep'] = simunode.getpropertyfloat('pasdetemps', 0.01)
        self.const['tolerance'] = simunode.getpropertyfloat('tolerance', 1e-6)
        self.const['maxint'] = simunode.getpropertyint('maxint', 200)
        self.const['duration'] = simunode.getpropertyfloat('duree_simulation', 2)
        self.const['do_abs_atmo'] = bool(int(simunode.getproperty("do_abs_atmo", "1")))
        self.const['stationary'] = bool(int(simunode.getproperty("stationary", "0")))

        # Load surface receiver
        for recsurf in self.rootnode["recepteurss"].lstnodesenum("recepteur_surfacique"):
            self.recepteurssurf[int(recsurf.getproperty("id"))]={ "name": recsurf.getproperty("name"), "id": int(recsurf.getproperty("id"))}
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
        self.load_fittings()
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

    # \~english
    # Feed the fittings list
    def load_fittings(self):
        for fitting in self.rootnode["encombrement_enum"].lstnodesenum("encombrement"):
            self.fittings[int(fitting.getproperty("id"))]=fitting
       

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
    
