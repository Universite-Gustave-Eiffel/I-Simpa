# -*- coding: UTF-8 -*-

def inject_mp_core():
    rootcore=ui.element(ui.application.getrootcore())
    if rootcore.getelementbylibelle("Reponse_modale")==-1:
        rootcore.appenduserelement(ui.element_type.ELEMENT_TYPE_CORE_CORE,"reponse_modale","Reponse_modale")
   
inject_mp_core()
del inject_mp_core
