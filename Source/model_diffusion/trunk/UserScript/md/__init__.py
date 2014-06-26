# -*- coding: cp1252 -*-
import uictrl as ui

import uilocale
_=uilocale.InstallUiModule(ui.application.getapplicationpath()["userscript"]+"md\\",ui.application.getlocale())

class mdf_trans(ui.element):
    """
      Code de calcul de modèle de diffusion
      Implanté totalement dans I-SIMPA via des modules python
    """
    def __init__(self,idel):
        ui.element.__init__(self,idel)
        #Ce code de calcul a surement des propriétés
        #Il faut les créer(nouveau projet) ou les mettre à jour (chargement) ici
        
        if not self.hasproperty("exeName"): #Nouveau projet
            #Création des paramètre du maillage tetgen
            self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
            self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_BFREQSELECTION)
            coreconf=ui.element(self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG))
            ui.element(self.appendpropertytext("modelName","","mesh.cbin",True,True)).hide()
            ui.element(self.appendpropertytext("tetrameshFileName","","tetramesh.mbin",True,True)).hide()
            ui.element(self.appendpropertytext("exeName","","md.py")).hide()
            ui.element(self.appendpropertytext("corePath","","md\\")).hide()
            
            #Création des paramètres de calculs
            coreconf.appendpropertylist("solver_mode","Mode de calcul",[["Temporel","Stationnaire"],[0,1]],0,False,1,True)
            coreconf.appendpropertybool("with_direct_sound","Fusion avec le champ direct",True,True)
            _("Fusion avec le champ direct")
            _("Mode de calcul")
            _("Temporel")
            _("Stationnaire")
            
            
        else:
            #Chargement d'un projet existant
            if self.getstringconfig("corePath")=="md_trans\\":
                self.updatestringconfig("corePath","md\\")
    def gettreelabel(self):
        """
            Retourne le libellé visible dans l'arbre
        """
        return "Modèle de diffusion"
    """
    ##
    # @return Indice de l'icone de l'élément en fonction de son état
    def geticonid(self,state_open):
        if state_open:
            return ui.graph.GRAPH_FOLDER_OPEN
        else:
            return ui.graph.GRAPH_FOLDER
    """
    def modified(self,idelmodified):
        #Le mode de calcul stationnaire n'a pas besoin des paramètres de pas de temps ni de durée
        if ui.element(idelmodified).getinfos()["name"]=="solver_mode":
            #le mode de calcul a été modifié par l'utilisateur
            elconf=ui.element(self.getelementbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG))
            is_temporel=(elconf.getlistconfig("solver_mode")==0)
            elconf.setreadonlyconfig("duree_simulation",not is_temporel)
            elconf.setreadonlyconfig("pasdetemps",not is_temporel)
        ui.element.modified(self,idelmodified)
