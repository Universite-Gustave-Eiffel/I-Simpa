# -*- coding: UTF-8 -*-

def inject_mp_core():
    rootcore=ui.element(ui.application.getrootcore())
    if rootcore.getelementbylibelle("MP_Python")==-1:
        rootcore.appenduserelement(ui.element_type.ELEMENT_TYPE_CORE_CORE,"mp_python","MP_Python")
   
inject_mp_core()
del inject_mp_core
