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

        if self.getelementbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE) == -1:
            # Set maximum volume of 0.1 m3
            meshel = ui.element(self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE))
            meshel.updateboolconfig("ismaxvol", True)
            meshel.updatedecimalconfig("maxvol", float(0.1))
        if self.getelementbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG) == -1:
            property_node = ui.element(self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG))
            property_node.appendpropertybool("with_direct_sound", "Merge with direct field", True, True)
            ui.element(self.appendpropertytext("tetrameshFileName", "", "tetramesh.mbin", True, True)).hide()
            ui.element(property_node.appendpropertydecimal("tolerance", _("Diffusion equation resolution - Tolerance"), float(1e-6), False, int(8), False, True, float(0),
                                                      float(1e-4), True))
            ui.element(property_node.appendpropertyentier("maxint", _("Diffusion equation resolution - Maximum number of iterations"), int(200)))
            property_node.appendpropertybool("do_abs_atmo", "Active calculation of atmospheric absorption", True, True)
        if self.getelementbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_BFREQSELECTION) == -1:
            freqs = ui.element(self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_BFREQSELECTION))
            # Limit frequencies from 100 to 5000
            # ui.application.sendevent(freqs, ui.idevent.IDEVENT_BFREQ_PRESELECTION_BUILDING_THIRD_BAND)

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
