# -*- coding: cp1252 -*-

import uictrl as ui
import uilocale
ScriptFolder=ui.application.getapplicationpath()["userscript"]+"moveto_vertex\\"
_=uilocale.InstallUiModule(ScriptFolder,ui.application.getlocale())

class manager:
    def __init__(self):
        self.movetoothergroupeventid=ui.application.register_event(self.onmovetoothergroup)
    def getmenu(self,typeel,idel,menu):
        #Si le pere de l'element se nomme SPPS
        el=ui.element(idel)
        menu.insert(0,(_(u"Move to an existing group"),self.movetoothergroupeventid))
        return True
    def onmovetoothergroup(self,elid):
        #On stocke les groupes de surfaces du projet
        rootscene=ui.element(ui.application.getrootscene())
        rootgroups=ui.element(rootscene.getelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_GROUPESURFACES))
        maingroupchildlist=rootgroups.childs()
        listgroups=[(elname,idelement) for idelement,eltype,elname in maingroupchildlist]
        input_lbl=_(u"Label")
        input_values=[ elname for elname,idelement in listgroups]
        dialog_res=ui.application.getuserinput(_(u"Move faces"),_(u"(Please select the group of destination:"),{ input_lbl : input_values})
        if dialog_res[0]==True:
            ideldest=listgroups[input_values.index(dialog_res[1][input_lbl])][1]
            ui.application.dropselectionto(ui.element(ideldest))
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_SCENE_GROUPESURFACES_GROUPE_VERTEX, manager())
