# -*- coding: UTF-8 -*-

def inject_md_core():
    rootcore=ui.element(ui.application.getrootcore())
    if rootcore.getelementbylibelle("MD_Python")==-1:
        rootcore.appenduserelement(ui.element_type.ELEMENT_TYPE_CORE_CORE,"md_python","MD_Python")
   
inject_md_core()
del inject_md_core
