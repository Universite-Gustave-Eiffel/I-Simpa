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
        if not self.hasproperty("timestep"):
            ui.element(self.appendpropertydecimal("timestep", _("Time step (s)").encode("UTF-8"), float(0.01), False, int(4), False, True, float(0),
                                                  float(1e-4), True))
        if not self.hasproperty("tolerance"):
            ui.element(self.appendpropertydecimal("tolerance", _("Tolerance"), float(1e-6), False, int(8)))
        if not self.hasproperty("maxint"):
            ui.element(self.appendpropertyentier("maxint", _("Maximum number of iterations"), int(200)))
        if not self.hasproperty("duration"):
            ui.element(self.appendpropertydecimal("duration", _("Simulation length (s)"), float(2)))
        if not self.hasproperty("do_abs_atmo"):
            self.appendpropertybool("do_abs_atmo", "Atmospheric calculation", True, True)
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
