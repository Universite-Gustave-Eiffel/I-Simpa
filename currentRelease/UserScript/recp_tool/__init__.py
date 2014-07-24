# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import uictrl as ui
import sys
from libsimpa import vec3
import math
import uilocale

_=uilocale.InstallUiModule(ui.application.getapplicationpath()["userscript"]+"recp_tool\\",ui.application.getlocale())


def MakeLineRecp(recpgroupindice,startpoint=[0,0,0],quantity=1,step=[1,0,0]):
    """
        Crée une ligne de récepteurs ponctuels
    """
    #on recupere le groupe des recepteurs
    recpgroup=ui.element(recpgroupindice)
    #on recupere tout les indices des récepteurs ponctuels déjà présents
    oldrecplst=recpgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
    #On construit les quantity nouveaux recepteur ponctuels
    for i in range(0,quantity):
        ui.application.sendevent(recpgroup,ui.idevent.IDEVENT_NEW_RECEPTEUR_P)
    #on cherche a obtenir les indices des nouvaux recepteur ponctuels
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
            menu.insert(2,(_(u"Orienter le groupe vers.."),self.alignfuncid))
            menu.insert(2,(_(u"Translation des récepteurs ponctuels"),self.translaterpfuncid))
            menu.insert(2,(_(u"Rotation des récepteurs ponctuels"),self.rotaterpfuncid))
            menu.insert(2,())
        menu.insert(2,(_(u"Créer une grille de récepteurs ponctuels"),self.makelinerecpid,"Bitmaps/popup_new.png"))
        return True
    def makeline(self,idel):
        lbl_startpt=_(u"Point de départ [x,y,z]")
        lbl_nbrecp=_(u"Nombre de récepteurs ponctuels en ligne")
        lbl_nbrecpcol=_(u"Nombre de récepteurs ponctuels en colonne")
        lbl_step=_(u"Pas ligne [x,y,z]")
        lbl_stepcol=_(u"Pas colonne [x,y,z]")
        inputs={ lbl_startpt : "[0.,0.,0.]",
                  lbl_nbrecp : "1",
                  lbl_nbrecpcol : "1",
                   lbl_step : "[1.,0.,0.]",
                   lbl_stepcol : "[0.,1.,0.]"
                    }
        res=ui.application.getuserinput(_(u"Créer une grille de récepteurs ponctuels"),
                                    _(u"Veuillez saisir les informations pour créer la grille de récepteurs ponctuels."),
                                    inputs)
        if res[0]:
            startpoint=eval(res[1][lbl_startpt])
            nbrecp=int(res[1][lbl_nbrecp])
            nbrecpcol=int(res[1][lbl_nbrecpcol])
            step=eval(res[1][lbl_step])
            stepcol=eval(res[1][lbl_stepcol])
            MakeGridRecp(idel,startpoint,nbrecp,nbrecpcol,step,stepcol)
    def align_on_same_point(self,idel):

        lbl_topt=_(u"Orienter vers [x,y,z]")
        res=ui.application.getuserinput(_(u"Orienter un groupe de récepteur vers un point dans l'espace."),
                                    _(u"Veuillez saisir les coordonnées du point d'orientation."),
                                    { lbl_topt : "[0.,0.,0.]"
                                        })
        if res[0]:
            grprecp=ui.element(idel)
            recplst=grprecp.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
            topt=eval(res[1][lbl_topt])
            for recpid in recplst:
                recp=ui.element(recpid)
                recp.updatepositionconfig("direction_dot",topt)
    def rotate(self,idgrp):
        lbl_vec=_(u"Vecteur de rotation")
        lbl_angle=_(u"Angle (degrés)")
        lbl_rotation_pos=_(u"Centre de rotation")

        #on recupere le groupe des rp
        rpgroup=ui.element(idgrp)
        #on recupere tout les indices des rp déjà existante
        rplst=rpgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
        centregroup=vec3()
        for rp in rplst:
            rpEl=ui.element(rp)
            #On recupere la position du récepteur ponctuel
            centregroup+=vec3(rpEl.getpositionconfig("pos_recepteur"))
        centregroup/=len(rplst)
        res=ui.application.getuserinput(_(u"Rotation d'un groupe de récepteurs ponctuels"),
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
                print(_(u"Erreur de saisie des paramètres"),file=sys.stderr)
                return
            for rp in rplst:
                rpEl=ui.element(rp)
                rotatedpos=vec3(rpEl.getpositionconfig("pos_recepteur"))-centregroup
                rotatedpos=rotatedpos.rotate(vecrotation,math.radians(anglerotation))+centregroup
                rpEl.updatepositionconfig("pos_recepteur",[rotatedpos.x,rotatedpos.y,rotatedpos.z])
    def translate(self,idgrp):
        lbl_vec=_(u"Direction (m)")
        res=ui.application.getuserinput(_(u"Translation d'un groupe de récepteurs ponctuel"),
                                    "",
                                    { lbl_vec : "[1.,0.,0.]"
                                        })
        if res[0]:
            try:
                vectranslation=vec3(eval(res[1][lbl_vec]))
            except:
                print(_(u"Erreur de saisie des paramètres"),file=sys.stderr)
                return
            #on recupere le groupe des rp
            rpgroup=ui.element(idgrp)
            #on recupere tout les indices des rp déjà existante
            rplst=rpgroup.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR)
            for rp in rplst:
                rpEl=ui.element(rp)
                #On recupere la position du rp
                newpos=vec3(rpEl.getpositionconfig("pos_recepteur"))+vectranslation
                rpEl.updatepositionconfig("pos_recepteur",[newpos.x,newpos.y,newpos.z])
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP, manager())

