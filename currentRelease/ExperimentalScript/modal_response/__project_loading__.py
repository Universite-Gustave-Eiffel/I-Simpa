# -*- coding: UTF-8 -*-

def inject_mp_core():
    rootcore=ui.element(ui.application.getrootcore())
    if rootcore.getelementbylibelle("Modal_Response")==-1:
        rootcore.appenduserelement(ui.element_type.ELEMENT_TYPE_CORE_CORE,"modal_response","Modal_Response")
   
inject_mp_core()
del inject_mp_core
