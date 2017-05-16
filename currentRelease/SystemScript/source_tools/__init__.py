# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import uictrl as ui
import sys
from libsimpa import vec3
import math
import uilocale
import os

_=uilocale.InstallUiModule(ui.application.getapplicationpath()["systemscript"]+"source_tools"+os.sep,ui.application.getlocale())


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
        lbl_startptx=_(u"Starting position x (m)")
        lbl_startpty=_(u"Starting position y (m)")
        lbl_startptz=_(u"Starting position z (m)")
        lbl_nbsources=_(u"Number of sources")
        lbl_stepx=_(u"Step x (m)")
        lbl_stepy=_(u"Step y (m)")
        lbl_stepz=_(u"Step z (m)")
        res=ui.application.getuserinput(_(u"Create a line of sound sources"),
                                    _(u"Please fill the following fields to create the sound source line"),
                                    { lbl_startptx : "0",
                                    lbl_startpty : "0",
                                    lbl_startptz : "0",
                                      lbl_nbsources : "1",
                                       lbl_stepx : "1",
                                       lbl_stepy : "0",
                                       lbl_stepz : "0",
                                        })
        if res[0]:
            try:
                startpoint=[float(res[1][lbl_startptx]),float(res[1][lbl_startpty]),float(res[1][lbl_startptz])]
                nbsources=int(res[1][lbl_nbsources])
                step=[float(res[1][lbl_stepx]),float(res[1][lbl_stepy]),float(res[1][lbl_stepz])]
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
        lbl_vecx=_(u"Vector of rotation x")
        lbl_vecy=_(u"Vector of rotation y")
        lbl_vecz=_(u"Vector of rotation z")
        lbl_angle=_(u"Angle (degrees)")
        lbl_rotation_posx=_(u"Rotation centre x (m)")
        lbl_rotation_posy=_(u"Rotation centre y (m)")
        lbl_rotation_posz=_(u"Rotation centre z (m)")
        ##Evaluation du centre de rotation
        #on recupere le groupe des sources
        srcgroup=ui.element(idgrp)
        #on recupere tout les indices des sources déjà existante
        srclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
        centregroup=vec3()
        for src in srclst:
            sourceEl=ui.element(src)
            #On recupere la position de la source
            pos = sourceEl.getpositionconfig("pos_source")
            centregroup+=vec3(pos[0], pos[1], pos[2])
        centregroup/=len(srclst)
        
        res=ui.application.getuserinput(_(u"Rotation of a group of sound sources"),
                                    "",
                                    { lbl_vecx : "0",
                                    lbl_vecy : "0",
                                    lbl_vecz : "1",
                                      lbl_angle : "90",
                                      lbl_rotation_posx : str(centregroup[0]),
                                      lbl_rotation_posy : str(centregroup[1]),
                                      lbl_rotation_posz : str(centregroup[2])
                                        })
        if res[0]:
            vecrotation = vec3()
            try:
                vecrotation=vec3(float(res[1][lbl_vecx]), float(res[1][lbl_vecy]), float(res[1][lbl_vecz]))
                anglerotation=float(res[1][lbl_angle])
                centregroup=vec3(float(res[1][lbl_rotation_posx]), float(res[1][lbl_rotation_posy]), float(res[1][lbl_rotation_posz]))
            except:
                print(_(u"Wrong parameters"),file=sys.stderr)
                return
            #on recupere le groupe des sources
            srcgroup=ui.element(idgrp)
            #on recupere tout les indices des sources déjà existante
            srclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
            for src in srclst:
                sourceEl=ui.element(src)
                pos_source = sourceEl.getpositionconfig("pos_source")
                rotatedpos=vec3(pos_source[0], pos_source[1], pos_source[2])-centregroup
                rotatedpos=rotatedpos.Rotation(vecrotation,math.radians(anglerotation))+centregroup
                sourceEl.updatepositionconfig("pos_source",[rotatedpos[0],rotatedpos[1], rotatedpos[2]])
    def translate_src(self,idgrp):
        lbl_vecx=_(u"Direction x (m)")
        lbl_vecy=_(u"Direction y (m)")
        lbl_vecz=_(u"Direction z (m)")
        res=ui.application.getuserinput(_(u"Translation of a group of sound sources"),
                                    "",
                                    { lbl_vecx : "1",
                                    lbl_vecy : "0",
                                    lbl_vecz : "0"
                                        })
        if res[0]:
            try:
                vectranslation=vec3(float(res[1][lbl_vecx]), float(res[1][lbl_vecy]), float(res[1][lbl_vecz]))
            except ValueError:
                print(_(u"Wrong parameters"),file=sys.stderr)
                return
            #on recupere le groupe des sources
            srcgroup=ui.element(idgrp)
            #on recupere tout les indices des sources déjà existante
            srclst=srcgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
            for src in srclst:
                sourceEl=ui.element(src)
                #On recupere la position de la source
                pos_source = sourceEl.getpositionconfig("pos_source")
                newpos=vec3(pos_source[0], pos_source[1], pos_source[2])+vectranslation
                sourceEl.updatepositionconfig("pos_source",[newpos[0],newpos[1],newpos[2]])
                
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES, manager())
