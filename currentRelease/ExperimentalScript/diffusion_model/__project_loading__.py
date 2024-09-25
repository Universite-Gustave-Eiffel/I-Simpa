# -*- coding: UTF-8 -*-

def inject_md_core():
    rootcore=ui.element(ui.application.getrootcore())
    if rootcore.getelementbylibelle("Diffusion_Model")==-1:
        rootcore.appenduserelement(ui.element_type.ELEMENT_TYPE_CORE_CORE,"diffusion_model","Diffusion_Model")
   
inject_md_core()
del inject_md_core
