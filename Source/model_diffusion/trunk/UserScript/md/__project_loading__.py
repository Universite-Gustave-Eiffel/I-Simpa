# -*- coding: cp1252 -*-
import new_element_witness

_=ui._ #on change le nom de la fonction de traduction
def inject_mdf_core():
    rootcore=ui.element(ui.application.getrootcore())
    if rootcore.getelementbylibelle("mdf_trans")==-1:
        rootcore.appenduserelement(ui.element_type.ELEMENT_TYPE_CORE_CORE,"md","mdf_trans")
        
##
# Ajoute les parametres propre au code de calcul
def AddVolumeProperty(elementId):
    #Si la propriété n'existe pas déjà
    propNode=ui.element(elementId)
    if(propNode.getelementbylibelle('propagation_volume')==-1):
        propNode.appendpropertybool('propagation_volume',_("Volume de propagation"),True,True)
def OnNewVolume(elementId):
    #Recherche de l'élément propriétés
    newvolEl=ui.element(elementId)
    idelprop=newvolEl.getelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_PROPRIETES)
    AddVolumeProperty(idelprop)
  

def AddCoreParameters():
    rootVolume=ui.element(ui.element(ui.application.getrootscene()).getelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_VOLUMES))
    #Pour les volumes existants on ajoute la propriété si nécessaire
    allPropNodes=rootVolume.getallelementbytype(ui.element_type.ELEMENT_TYPE_SCENE_VOLUMES_VOLUME_PROPRIETES)
    for volNode in allPropNodes:
        AddVolumeProperty(volNode)
    #Pour les futurs volumes, l'ajout de la propriété se fera lors de la mise à jour
    new_element_witness.LinkElementCreationWithFunction(rootVolume,OnNewVolume)
    
    
inject_mdf_core()
AddCoreParameters()
del inject_mdf_core
