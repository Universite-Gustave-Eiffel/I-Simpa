# -*- coding: cp1252 -*-
import uictrl as ui
import uilocale
_=uilocale.InstallUiModule(ui.application.getapplicationpath()["userscript"]+"md_octave\\",ui.application.getlocale())

class MD_Octave(ui.element):
    """
      Declaration of MD computation core
    """
    def __init__(self,idel):
        ui.element.__init__(self,idel)
        
        if not self.hasproperty("exeName"): #Nouveau projet
            ui.element(self.appendpropertytext("modelName","","mesh.cbin",True,True)).hide()
            ui.element(self.appendpropertytext("exeName","","md_octave.py")).hide()
            ui.element(self.appendpropertytext("corePath","","md_octave/")).hide() 
            self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
        if not self.hasproperty("tetrameshFileName"): 
            ui.element(self.appendpropertytext("tetrameshFileName","","tetramesh.mbin",True,True)).hide()
        else:
            pass #Chargement d'un projet existant
    def gettreelabel(self):
        """
            Retourne le libellé visible dans l'arbre
        """
        return "MD Octave"
    def geticonpath(self,state_open):
        """
            Retourne  le chemin d'accès au fichier d'icône
        """
        return "UserScript/md_octave/icon/md.ico"
