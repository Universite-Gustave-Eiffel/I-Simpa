# -*- coding: UTF-8 -*-

def inject_md_core():
    rootcore=ui.element(ui.application.getrootcore())
    if rootcore.getelementbylibelle("Methode_Diffusion")==-1:
        rootcore.appenduserelement(ui.element_type.ELEMENT_TYPE_CORE_CORE,"methode_diffusion","Methode_Diffusion")
   
inject_md_core()
del inject_md_core
