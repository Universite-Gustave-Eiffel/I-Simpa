# -*- coding: cp1252 -*-
preceiv_sourceTracker.SourcePosTrackerLst.clear()              #supprime les anciens trackers
preceiv_sourceTracker.SourcePosToReceiverPosTrackerLst.clear() #supprime les anciens trackers

def CheckPonctTracker():
    rootscene=ui.element(ui.application.getrootscene())
    rp_roots=ui.element(rootscene.getelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP))
    rplist=rp_roots.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_RECEPTEURSP_RECEPTEUR )
    for idrp in rplist:
        rp=ui.element(idrp)
        pos=ui.element(rp.getelementbylibelle("pos_recepteur"))
        if pos.hasproperty("linkedsource"):
            preceiv_sourceTracker.SourcePosToReceiverPosTrackerLst[idrp]=preceiv_sourceTracker.PositionUpdater(idrp,True)
        dir=ui.element(rp.getelementbylibelle("direction_dot"))
        if dir.hasproperty("linkedsource"):
            preceiv_sourceTracker.SourcePosTrackerLst[idrp]=preceiv_sourceTracker.PositionUpdater(idrp)

CheckPonctTracker()