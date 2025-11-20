# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import uictrl as ui
import sys
from libsimpa import vec3
import math
import uilocale
import os

_=uilocale.InstallUiModule(ui.application.getapplicationpath()["systemscript"]+"recp_tool"+os.sep,ui.application.getlocale())


def MakeLineRecp(recpgroupindice,startpoint=[0,0,0],quantity=1,step=[1,0,0]):
    """
        Crée une ligne de récepteurs ponctuels
    """
    #on recupere le groupe des recepteurs
    recpgroup=ui.element(recpgroupindice)
    #on recupere tout les indices des récepteurs ponctuels déjà présents
    oldrecplst=recpgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
    #On construit les quantity nouveaux récepteurs ponctuels
    for i in range(0,quantity):
        ui.application.sendevent(recpgroup,ui.idevent.IDEVENT_NEW_RECEPTEUR_P)
    #on cherche a obtenir les indices des nouveaux récepteurs ponctuels
    newrecplst=recpgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
    createdrecp=[]
    for recp in newrecplst:
        if recp not in oldrecplst:
            createdrecp.append(recp)
    #on modifie la position des récepteurs ponctuels selon les pas de positions en paramètre
    curpos=startpoint
    for recp in createdrecp:
        recpEl=ui.element(recp)
        #il nous faut affecter la position du recepteur ponctuel
        recpEl.updatepositionconfig("pos_recepteur",curpos)
        curpos=[curpos[0]+step[0],curpos[1]+step[1],curpos[2]+step[2]]
def MakeGridRecp(recpgroupindice,startpoint=[0,0,0],quantityline=1,quantitycol=1,stepline=[1,0,0],stepcol=[0,1,0]):
    curpos=startpoint
    for col in range(0,quantitycol):
        MakeLineRecp(recpgroupindice,curpos,quantityline,stepline)
        curpos=[curpos[0]+stepcol[0],curpos[1]+stepcol[1],curpos[2]+stepcol[2]]
class manager:
    def __init__(self):
        self.makelinerecpid=ui.application.register_event(self.makeline)
        self.alignfuncid=ui.application.register_event(self.align_on_same_point)
        self.rotaterpfuncid=ui.application.register_event(self.rotate)
        self.translaterpfuncid=ui.application.register_event(self.translate)
        self.defaultval=[]
    def getmenu(self,typeel,idel,menu):
        #si le groupe n'est pas vide
        if len(ui.element(idel).childs())>0:
            menu.insert(2,(_(u"Orient the group to a position"),self.alignfuncid))
            menu.insert(2,(_(u"Translation of receivers"),self.translaterpfuncid))
            menu.insert(2,(_(u"Rotation of receivers"),self.rotaterpfuncid))
            menu.insert(2,())
        menu.insert(2,(_(u"Create a receiver grid"),self.makelinerecpid,"Bitmaps/popup_new.png"))
        return True
    def makeline(self,idel):
        lbl_startptx=_(u"Starting position x (m)")
        lbl_startpty=_(u"Starting position y (m)")
        lbl_startptz=_(u"Starting position z (m)")
        lbl_nbrecp=_(u"Number of rows")
        lbl_nbrecpcol=_(u"Number of cols")
        lbl_stepx=_(u"Row step x (m)")
        lbl_stepy=_(u"Row step y (m)")
        lbl_stepz=_(u"Row step z (m)")
        lbl_stepcolx=_(u"Col step x (m)")
        lbl_stepcoly=_(u"Col step y (m)")
        lbl_stepcolz=_(u"Col step z (m)")
        inputs={ lbl_startptx : "0",
                lbl_startpty : "0",
                lbl_startptz : "0",
                  lbl_nbrecp : "1",
                  lbl_nbrecpcol : "1",
                   lbl_stepx : "1",
                   lbl_stepy : "0",
                   lbl_stepz : "0",
                   lbl_stepcolx : "0",
                   lbl_stepcoly : "1",
                   lbl_stepcolz : "0"
                    }
        res=ui.application.getuserinput(_(u"Create a receiver grid"),
                                    _(u"Please fill the following fields to create the receivers points grid"),
                                    inputs)
        if res[0]:
            startpoint=[float(res[1][lbl_startptx]),float(res[1][lbl_startpty]),float(res[1][lbl_startptz])]
            nbrecp=int(res[1][lbl_nbrecp])
            nbrecpcol=int(res[1][lbl_nbrecpcol])
            step=[float(res[1][lbl_stepx]),float(res[1][lbl_stepy]),float(res[1][lbl_stepz])]
            stepcol=[float(res[1][lbl_stepcolx]),float(res[1][lbl_stepcoly]),float(res[1][lbl_stepcolz])]
            MakeGridRecp(idel,startpoint,nbrecp,nbrecpcol,step,stepcol)
    def align_on_same_point(self,idel):
        
        lbl_toptx=_(u"Orient to position x (m)")
        lbl_topty=_(u"Orient to position y (m)")
        lbl_toptz=_(u"Orient to position z (m)")
        res=ui.application.getuserinput(_(u"Orient a group of receivers to a point"),
                                    _(u"Please enter the coordinates of the orientation point"),
                                    { lbl_toptx : "0",
                                    lbl_topty : "0",
                                    lbl_toptz : "0",
                                        })
        if res[0]:
            grprecp=ui.element(idel)
            recplst=grprecp.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
            topt=[float(res[1][lbl_toptx]), float(res[1][lbl_topty]), float(res[1][lbl_toptz])]
            for recpid in recplst:
                recp=ui.element(recpid)
                recp.updatepositionconfig("direction_dot",topt)
    def rotate(self,idgrp):
        lbl_vecx=_(u"Vector of rotation x (m)")
        lbl_vecy=_(u"Vector of rotation y (m)")
        lbl_vecz=_(u"Vector of rotation z (m)")
        lbl_angle=_(u"Angle of rotation (degrees)")
        lbl_rotation_posx=_(u"Rotation center x (m)")
        lbl_rotation_posy=_(u"Rotation center y (m)")
        lbl_rotation_posz=_(u"Rotation center z (m)")

        #on recupere le groupe des rp
        rpgroup=ui.element(idgrp)
        #on recupere tout les indices des rp déjà existante
        rplst=rpgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
        centregroup=vec3()
        for rp in rplst:
            rpEl=ui.element(rp)
            #On recupere la position du récepteur ponctuel
            pos = rpEl.getpositionconfig("pos_recepteur")
            centregroup += vec3(pos[0], pos[1], pos[2])
        centregroup/=len(rplst)
        res=ui.application.getuserinput(_(u"Rotation of a group of receivers"),
                                    "",
                                    { lbl_vecx : "0.",
                                      lbl_vecy : "0.",
                                      lbl_vecz : "1.",
                                      lbl_angle : "90",
                                      lbl_rotation_posx : "%0.8g" % centregroup[0],
                                      lbl_rotation_posy : "%0.8g" % centregroup[1],
                                      lbl_rotation_posz : "%0.8g" % centregroup[2]
                                        })
        if res[0]:
            try:
                vecrotation=vec3(float(res[1][lbl_vecx]), float(res[1][lbl_vecy]), float(res[1][lbl_vecz]))
                anglerotation=float(res[1][lbl_angle])
                centregroup=vec3(float(res[1][lbl_rotation_posx]),float(res[1][lbl_rotation_posy]),float(res[1][lbl_rotation_posz]))
            except:
                print(_(u"Wrong parameters"),file=sys.stderr)
                return
            for rp in rplst:
                rpEl=ui.element(rp)
                rppos = rpEl.getpositionconfig("pos_recepteur")
                rotatedpos=vec3(rppos[0], rppos[1], rppos[2])-centregroup
                rotatedpos=rotatedpos.Rotation(vecrotation,math.radians(anglerotation))+centregroup
                rpEl.updatepositionconfig("pos_recepteur",[rotatedpos[0],rotatedpos[1],rotatedpos[2]])
    def translate(self,idgrp):
        lbl_vecx=_(u"Direction x (m)")
        lbl_vecy=_(u"Direction y (m)")
        lbl_vecz=_(u"Direction z (m)")
        res=ui.application.getuserinput(_(u"Translation of a group of receivers"),
                                        _(u"Please enter the coordinates of the translation vector"),
                                    { lbl_vecx : "1.",
                                      lbl_vecy : "0.",
                                      lbl_vecz : "0.",
                                        })
        if res[0]:
            try:
                vectranslation=vec3(float(res[1][lbl_vecx]),float(res[1][lbl_vecy]),float(res[1][lbl_vecz]))
            except:
                print(_(u"Wrong parameters"),file=sys.stderr)
                return
            #on recupere le groupe des rp
            rpgroup=ui.element(idgrp)
            #on recupere tout les indices des rp déjà existante
            rplst=rpgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
            for rp in rplst:
                rpEl=ui.element(rp)
                #On recupere la position du rp
                posrecp = rpEl.getpositionconfig("pos_recepteur")
                newpos=vec3(posrecp[0], posrecp[1], posrecp[2])+vectranslation
                rpEl.updatepositionconfig("pos_recepteur",[newpos[0],newpos[1],newpos[2]])
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP, manager())

