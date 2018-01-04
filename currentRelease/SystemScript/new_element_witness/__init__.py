# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import uictrl as ui

##
# Ce module permet d'appeler une fonction dans le cas où un nouvel élément d'un certain type est créé
#
#
class ElementWatcher:
    def __init__(self,elementToWatch,func):
        self.elid=elementToWatch.getid()
        self.oldchild=[]
        for child in elementToWatch.childs():
            self.oldchild.append(child[0])
        self.oldchild=set(self.oldchild)
        self.func=func
    def OnUpdate(self,elid):
        if self.elid==elid:
            lastchilds=set([child[0] for child in ui.element(self.elid).childs()])
            if len(lastchilds)>len(self.oldchild):
                newchilds=lastchilds-self.oldchild
                for newchild in newchilds:
                    self.func(newchild)
            self.oldchild=lastchilds
                
    

def LinkElementCreationWithFunction(element,func):
    newevtwatcher=ElementWatcher(element,func)
    element.register_update_manager(newevtwatcher.OnUpdate)
