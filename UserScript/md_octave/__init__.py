# -*- coding: cp1252 -*-
import uictrl as ui
import uilocale
_=uilocale.InstallUiModule(ui.application.getapplicationpath()["userscript"]+"md_octave\\",ui.application.getlocale())

class MD_Octave(ui.element):
    """
      Declaration of MD computation core
    """
    def __init__(self,idel):
        ui.element.__init__(self, idel)

        if not self.hasproperty("exeName"):  # Nouveau projet
            ui.element(self.appendpropertytext("modelName", "", "mesh.cbin", True, True)).hide()
            ui.element(self.appendpropertytext("exeName", "", "md_octave.py")).hide()
            ui.element(self.appendpropertytext("corePath", "", "md_octave/")).hide()
            # Set maximum volume of 0.1 m3
            meshel = ui.element(self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE))
            meshel.updateboolconfig("ismaxvol", True)
            meshel.updatedecimalconfig("maxvol", float(0.1))
        if not self.hasproperty("with_direct_sound"):
            self.appendpropertybool("with_direct_sound", "Merge with direct field", True, True)
        if not self.hasproperty("tetrameshFileName"):
            ui.element(self.appendpropertytext("tetrameshFileName", "", "tetramesh.mbin", True, True)).hide()
        else:
            pass  # Existing project
    def gettreelabel(self):
        """
            Return core label
        """
        return "MD Octave"
    def geticonpath(self,state_open):
        """
            Return tree node icon
        """
        return "UserScript/md_octave/icon/md.ico"
