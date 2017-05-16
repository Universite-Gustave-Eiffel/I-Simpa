# -*- coding: cp1252 -*-
##
# Cet outil permet de lier une propriété de directivité d'un récepteur
# à la position d'une source.
#
import uilocale
import uictrl as ui
import os
ScriptFolder=ui.application.getapplicationpath()["systemscript"]+"preceiv_sourceTracker"+os.sep
#Déclaration de la méthode de traduction
_=uilocale.InstallUiModule(ScriptFolder,ui.application.getlocale())
##
# L'instance de cette classe est placée dans l'élément position d'une source et est appelé si la position de la source est mis à jour
class PositionUpdater:
    ##
    # Initialisation
    # @param receiverId Receiver id
    # @param isPositionTracker True if this is a link between source position and receiver position, False if this is a link between the source position and the receiver orientation
    def __init__(self,receiverId,isPositionTracker=False):
        self.isPositionTracker=isPositionTracker
        self.receiverid=receiverId
        self.sourceposid=-1
        self.srcid=-1
        self.srclbl=""
        self.UpdateLinker()
        if self.isPositionTracker:
            self.old_src_pos=ui.element(self.srcid).getpositionconfig("pos_source")
    def Unlink(self):
        self.sourceposid=-1
    def OnSourcePositionChange(self,idsourcepos):
        if(self.sourceposid==idsourcepos):
            try:
                source=ui.element(ui.element(idsourcepos).getinfos()["parentid"])
                recp=ui.element(self.receiverid)
                if not self.isPositionTracker:
                    recp.updatepositionconfig("direction_dot",source.getpositionconfig("pos_source"))
                else:
                    recp_pos=recp.getpositionconfig("pos_recepteur")
                    new_source_pos=source.getpositionconfig("pos_source")
                    new_recppos=[recp_pos[i]-self.old_src_pos[i]+new_source_pos[i] for i in range(3)]
                    recp.updatepositionconfig("pos_recepteur",new_recppos)
                    self.old_src_pos=new_source_pos
            except:
                self.sourceposid=-1 #le recepteur n'existe plus
    def OnSourceLabelChange(self,idsource):
        if idsource == self.srcid:
            newlblsrc=ui.element(idsource).getinfos()["name"]
            if newlblsrc!=self.srclbl:
                self.srclbl=newlblsrc
                try:
                    receiver=ui.element(self.receiverid)
                    if not self.isPositionTracker:
                        dir=ui.element(receiver.getelementbylibelle("direction_dot"))
                    else:
                        dir=ui.element(receiver.getelementbylibelle("pos_recepteur"))
                    dir.updatestringconfig("linkedsource",ui.element(idsource).getinfos()["name"])
                except:
                    self.sourceposid=-1 #le recepteur n'existe plus
            
    def UpdateLinker(self):
        receiver=ui.element(self.receiverid)
        dir=None
        if not self.isPositionTracker:
            dir=ui.element(receiver.getelementbylibelle("direction_dot"))
        else:
            dir=ui.element(receiver.getelementbylibelle("pos_recepteur"))
        if dir.hasproperty("linkedsource"):
            labelsource=dir.getstringconfig("linkedsource")
            self.srclbl=labelsource
            rootscene=ui.element(ui.application.getrootscene())
            sourcroot=ui.element(rootscene.getelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES))
            sourceid=sourcroot.getelementbylibelle(labelsource)
            if(sourceid!=-1):
                self.srcid=sourceid
                src=ui.element(sourceid)
                posid=src.getelementbylibelle("pos_source")
                pos=ui.element(posid)
                self.sourceposid=posid
                pos.register_update_manager(self.OnSourcePositionChange) #Lie la méthode à la mise à jour de la position de la source
                src.register_update_manager(self.OnSourceLabelChange) #Lie la méthode à la mise à jour de la source pour le changement de libellé
                if not self.isPositionTracker:
                    receiver.updatepositionconfig("direction_dot",src.getpositionconfig("pos_source"))
    
SourcePosTrackerLst={}
SourcePosToReceiverPosTrackerLst={}
def LinkSourcePositionWithReceiverPosition(sourceId,ReceiverId):
    #Il faut ajouter les coordonnées X,Y,Z de décalage avec la source
    #Ainsi que le nom de la source dans cette propriété
    receiver=ui.element(ReceiverId)
    source=ui.element(sourceId)
    posel=ui.element(receiver.getelementbylibelle("pos_recepteur"))
    if(posel.hasproperty("linkedsource")):
        #Mettre a jour la propriete
        posel.updatestringconfig("linkedsource",source.getinfos()["name"])  
    else:
        #Add property
        _("linkedsource")
        ui.element(posel.appendpropertytext("linkedsource","linkedsource",source.getinfos()["name"])).hide() 
    if not SourcePosToReceiverPosTrackerLst.has_key(ReceiverId):
        SourcePosToReceiverPosTrackerLst[ReceiverId]=PositionUpdater(ReceiverId,True)
    else:
        SourcePosToReceiverPosTrackerLst[ReceiverId].UpdateLinker()    
    
def LinkSourcePositionWithReceiverOrientation(sourceId,ReceiverId):
    #Il faut ajouter un indice dans l'element de propriete de direction du recepteur ponctuel afin de retrouver la source au prochaine chargement du projet
    receiver=ui.element(ReceiverId)
    source=ui.element(sourceId)
    posel=ui.element(receiver.getelementbylibelle("direction_dot"))
    if(posel.hasproperty("linkedsource")):
        #Mettre a jour la propriete
        posel.updatestringconfig("linkedsource",source.getinfos()["name"])
    else:
        #Ajouter la propriete
        ui.element(posel.appendpropertytext("linkedsource","linkedsource",source.getinfos()["name"])).hide()
    if not SourcePosTrackerLst.has_key(ReceiverId):
        SourcePosTrackerLst[ReceiverId]=PositionUpdater(ReceiverId)
    else:
        SourcePosTrackerLst[ReceiverId].UpdateLinker()
class managerLinkWithSourcePosition:
    def __init__(self):
        self.linkWithSourcePositionEventId=ui.application.register_event(self.LinkWithSourcePosition)
        self.linkPositionWithSourcePositionEventId=ui.application.register_event(self.LinkPositionWithSourcePosition)
        self.unlinkReceiverDirEventId=ui.application.register_event(self.UnlinkReceiverDir)
    def UnlinkReceiverDir(self,idel):
        positionel=ui.element(idel)
        ReceiverId=positionel.getinfos()["parentid"]
        if SourcePosTrackerLst.has_key(ReceiverId):
            positionel.deleteelementbyxmlid(positionel.getelementbylibelle("linkedsource"))
            SourcePosTrackerLst[ReceiverId].Unlink()
    def AskUserToSelectASource(self):
        #Il faut faire la liste des libellés de toutes les sources
        rootscene=ui.element(ui.application.getrootscene())
        sourclst_ids=rootscene.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE)
        dict_src={}
        for sourceid in sourclst_ids:
            src=ui.element(sourceid)
            dict_src[src.getinfos()["name"]]=sourceid
        srcLst=dict_src.keys()
        srcLst.sort()
        srcsellabel=_(u"Source name")
        dialog_res=ui.application.getuserinput(_(u"Source selection"),_(u"Please choose the source:"),{srcsellabel : srcLst})
        
        if(dialog_res[0]==1 and srcsellabel in dialog_res[1].keys()):
            selected_sourcename=dialog_res[1][srcsellabel]
            if selected_sourcename in dict_src.keys():
                selected_sourceid=dict_src[selected_sourcename]
                return selected_sourceid
            else:
                return -1
        else:
            return -1
    def LinkPositionWithSourcePosition(self,idel):
        selected_sourceid=self.AskUserToSelectASource()
        if selected_sourceid!=-1:
            LinkSourcePositionWithReceiverPosition(selected_sourceid,ui.element(idel).getinfos()["parentid"])        
        
    def LinkWithSourcePosition(self,idel):
        selected_sourceid=self.AskUserToSelectASource()
        if selected_sourceid!=-1:
            LinkSourcePositionWithReceiverOrientation(selected_sourceid,ui.element(idel).getinfos()["parentid"])
    def getmenu(self,typeel,idel,menu):
        #On ajoute la fonction seulement si il s'agit de la position d'un récepteur ponctuel
        infoEl=ui.element(idel).getinfos()
        infoParent=ui.element(infoEl["parentid"]).getinfos()
        if infoParent["typeElement"]==ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR:
            if infoEl["name"]==u"direction_dot":
                menu.insert(1,(_(u"Please choose the source:"),self.linkWithSourcePositionEventId))
                if ui.element(idel).hasproperty("linkedsource"):
                    
                    menu.insert(1,(_(u"Delete the link with the source"),self.unlinkReceiverDirEventId))
                return True
            elif infoEl["name"]==u"pos_recepteur":
                menu.insert(1,(_(u"Please choose the source:"),self.linkPositionWithSourcePositionEventId))
                #todo tester si la position du récepteur est déjà lié a une source
                return True
        return False
            
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_POSITION, managerLinkWithSourcePosition())