# -*- coding: cp1252 -*-
#----------------------------------------------------------------------
# I-SIMPA (http://i-simpa.ifsttar.fr). This file is part of I-SIMPA.
#
# I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
# to scientific acoustic simulations.
# Copyright (C) 2007-2014 - IFSTTAR - Judicael Picaut, Nicolas Fortin
#
# I-SIMPA is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# I-SIMPA is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA or 
# see <http://ww.gnu.org/licenses/>
#
# For more information, please consult: <http://i-simpa.ifsttar.fr> or 
# send an email to i-simpa@ifsttar.fr
#
# To contact Ifsttar, write to Ifsttar, 14-20 Boulevard Newton
# Cite Descartes, Champs sur Marne F-77447 Marne la Vallee Cedex 2 FRANCE
# or write to scientific.computing@ifsttar.fr
# ----------------------------------------------------------------------


_=uictrl._ #Make an alias of the translation function

class mdf(uictrl.element):
    """
      Diffusion model calculation core.
    """
    def __init__(self,idel):
        uictrl.element.__init__(self,idel)
        
        if not self.hasproperty("exeName"): #Test if this is a new project initialisation
            #If this is a new project then we add properties
            #Add tetgen parameters
            self.appendfilsbytype(uictrl.element_type.ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
            #Add frequencies selection
            self.appendfilsbytype(uictrl.element_type.ELEMENT_TYPE_CORE_CORE_BFREQSELECTION)
            #Add configuration core
            coreconf=uictrl.element(self.appendfilsbytype(uictrl.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG))
            #Append hidden config, used by I-SIMPA to find the core files and binaries
            uictrl.element(self.appendpropertytext("modelName","","mesh.cbin",True,True)).hide()
            uictrl.element(self.appendpropertytext("tetrameshFileName","","tetramesh.mbin",True,True)).hide()
            uictrl.element(self.appendpropertytext("exeName","","md.py")).hide()
            if sys.platform() == "Windows":
            uictrl.element(self.appendpropertytext("corePath","","md\\")).hide()
            else:
                uictrl.element(self.appendpropertytext("corePath","","md/")).hide()         
            
            #User options
            coreconf.appendpropertylist("solver_mode","Calculation mode",[["Time","Static"],[0,1]],0,False,1,True)
            coreconf.appendpropertybool("with_direct_sound","Use direct sound",True,True)
            _("Calculation mode")
            _("Use direct sound")
            _("Time")
            _("Static")
            
            
        else:
            pass #Here in case of loading an existing project
    def gettreelabel(self):
        """
            Return label
        """
        return "Mdf"
    def geticonid(self,state_open):
        """
            Return tree icon Id
        """
        if state_open:
            return uictrl.graph.GRAPH_FOLDER_OPEN
        else:
            return uictrl.graph.GRAPH_FOLDER
    def modified(self,idelmodified):
        #In case of sub element modification this func is call by ui
        #We disable the time dependant parameters in case of static solver mode
        if uictrl.element(idelmodified).getinfos()["name"]=="solver_mode":
            elconf=uictrl.element(self.getelementbytype(uictrl.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG))
            is_temporal=(elconf.getlistconfig("solver_mode")==0)
            elconf.setreadonlyconfig("duree_simulation",not is_temporal)
            elconf.setreadonlyconfig("pasdetemps",not is_temporal)
        uictrl.element.modified(self,idelmodified)
