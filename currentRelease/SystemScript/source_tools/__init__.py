# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import uictrl as ui
import sys
from libsimpa import vec3
import math
import uilocale
import os

_=uilocale.InstallUiModule(ui.application.getapplicationpath()["userscript"]+"source_tools"+os.sep,ui.application.getlocale())


def MakeLineSrc(srcgroupindice,startpoint=[0,0,0],quantity=1,step=[1,0,0]):
    """
        Crée une ligne de source sonores
    """
    #on recupere le groupe des sources
    srcgroup=ui.element(srcgroupindice)
    #on recupere tout les indices des sources déjà existante
    oldsrclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
    #On construit les quantity nouvelles sources
    for i in range(0,quantity):
        ui.application.sendevent(srcgroup,ui.idevent.IDEVENT_NEW_SOURCE)
    #on cherche a obtenir les indices des nouvelles sources
    newsrclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
    createdsrc=[]
    for src in newsrclst:
        if src not in oldsrclst:
            createdsrc.append(src)
    #on modifie la position des sources selon les pas de positions en paramètre
    curpos=startpoint
    for src in createdsrc:
        srcEl=ui.element(src)
        #il nous faut la position de la source
        srcEl.updatepositionconfig("pos_source",curpos)
        curpos=[curpos[0]+step[0],curpos[1]+step[1],curpos[2]+step[2]]

class manager:
    def __init__(self):
        self.makelinesrcid=ui.application.register_event(self.makeline)
        self.enable_grp_sourcesid=ui.application.register_event(self.enable_grp_sources)
        self.disable_grp_sourcesid=ui.application.register_event(self.disable_grp_sources)
        self.rotate_grp_sourcesid=ui.application.register_event(self.rotate_src)
        self.translation_grp_sourcesid=ui.application.register_event(self.translate_src)
    def getmenu(self,typeel,idel,menu):
        submenu=[(_(u"Enable"),self.enable_grp_sourcesid),
                 (_(u"Disable"),self.disable_grp_sourcesid),
                 (),
                 (_(u"Rotation"),self.rotate_grp_sourcesid),
                 (_(u"Translation"),self.translation_grp_sourcesid)]
        if len(ui.element(idel).childs())>0:
            menu.insert(2,(_(u"All sources"),submenu))
            menu.insert(2,())
        menu.insert(2,(_(u"Create a line of sound sources"),self.makelinesrcid,"Bitmaps/popup_new.png"))
        return True
    def makeline(self,idel):
        lbl_startpt=_(u"Starting position [x,y,z] (m)")
        lbl_nbsources=_(u"Number of sources")
        lbl_step=_(u"Step [x,y,z] (m)")
        res=ui.application.getuserinput(_(u"Create a line of sound sources"),
                                    _(u"Please fill the following fields to create the sound source line"),
                                    { lbl_startpt : "[0.,0.,0.]",
                                      lbl_nbsources : "1",
                                       lbl_step : "[1.,0.,0.]"
                                        })
        if res[0]:
            try:
                startpoint=eval(res[1][lbl_startpt])
                nbsources=int(res[1][lbl_nbsources])
                step=eval(res[1][lbl_step])
            except:
                print(_(u"Wrong parameters"),file=sys.stderr)
                return
            MakeLineSrc(idel,startpoint,nbsources,step)
    
    def set_grp_src_activation(self,idgrp,newstate):
        grpsrc=ui.element(idgrp)
        all_property=grpsrc.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES)
        for prop in all_property:
            ui.element(prop).updateboolconfig("enable",newstate)

    def enable_grp_sources(self,idgrp):
        self.set_grp_src_activation(idgrp,True)
    def disable_grp_sources(self,idgrp):
        self.set_grp_src_activation(idgrp,False)
    def rotate_src(self,idgrp):
        lbl_vec=_(u"Vector of rotation")
        lbl_angle=_(u"Angle (degrees)")
        lbl_rotation_pos=_(u"Rotation centre")
        ##Evaluation du centre de rotation
        #on recupere le groupe des sources
        srcgroup=ui.element(idgrp)
        #on recupere tout les indices des sources déjà existante
        srclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
        centregroup=vec3()
        for src in srclst:
            sourceEl=ui.element(src)
            #On recupere la position de la source
            centregroup+=vec3(sourceEl.getpositionconfig("pos_source"))
        centregroup/=len(srclst)
        
        res=ui.application.getuserinput(_(u"Rotation of a group of sound sources"),
                                    "",
                                    { lbl_vec : "[0.,0.,1.]",
                                      lbl_angle : "90",
                                      lbl_rotation_pos : str(centregroup)
                                        })
        if res[0]:
            try:
                vecrotation=vec3(eval(res[1][lbl_vec]))
                anglerotation=float(res[1][lbl_angle])
                centregroup=vec3(eval(res[1][lbl_rotation_pos]))
            except:
                print(_(u"Wrong parameters"),file=sys.stderr)
                return
            #on recupere le groupe des sources
            srcgroup=ui.element(idgrp)
            #on recupere tout les indices des sources déjà existante
            srclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
            for src in srclst:
                sourceEl=ui.element(src)
                rotatedpos=vec3(sourceEl.getpositionconfig("pos_source"))-centregroup
                rotatedpos=rotatedpos.rotate(vecrotation,math.radians(anglerotation))+centregroup
                sourceEl.updatepositionconfig("pos_source",[rotatedpos.x,rotatedpos.y,rotatedpos.z])
    def translate_src(self,idgrp):
        lbl_vec=_(u"Direction (m)")
        res=ui.application.getuserinput(_(u"Translation of a group of sound sources"),
                                    "",
                                    { lbl_vec : "[1.,0.,0.]"
                                        })
        if res[0]:
            try:
                vectranslation=vec3(eval(res[1][lbl_vec]))
            except:
                print(_(u"Wrong parameters"),file=sys.stderr)
                return
            #on recupere le groupe des sources
            srcgroup=ui.element(idgrp)
            #on recupere tout les indices des sources déjà existante
            srclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
            for src in srclst:
                sourceEl=ui.element(src)
                #On recupere la position de la source
                newpos=vec3(sourceEl.getpositionconfig("pos_source"))+vectranslation
                sourceEl.updatepositionconfig("pos_source",[newpos.x,newpos.y,newpos.z])
                
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES, manager())
