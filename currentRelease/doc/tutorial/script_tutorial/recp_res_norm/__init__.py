# -*- coding: cp1252 -*-

import uictrl as ui
import libsimpa as ls

# Déclaration de la méthode de traduction en cas d'erreur d'import du dictionnaire
def _(msg):
    return msg

try:
    import uilocale
    #Construction du chemin du script
    ScriptFolder=ui.application.getapplicationpath()["userscript"]+"recp_res_norm\\"
    #Déclaration de la méthode de traduction
    _=uilocale.InstallUiModule(ScriptFolder,ui.application.getlocale())
except:
    #En cas d'erreur afficher l'erreur et continuer
    import sys, traceback
    print("Unable to import the language dictionary !")
    traceback.print_exc(file=sys.stdout)

def MakeSPLNormalGrid(folderwxid, save_as):
    #Dictionnaire qui contiendra les données pour tout les récepteurs
    global_dict={}
    #folder devient l'objet dossier
    folder=ui.element(folderwxid)
    #dans un tableau on place les indices des fichiers de données des récepteurs ponctuels
    recplist=folder.getallelementbytype(ui.element_type.ELEMENT_TYPE_REPORT_GABE_RECP)
    #Pour chaque fichiers de type ELEMENT_TYPE_REPORT_GABE_RECP
    for idrecp in recplist:
        #recp devient l'objet ayant comme indice idrecp (entier)
        recp=ui.element(idrecp)
        #Stocke les informations sur cet élément
        recpinfos=recp.getinfos()
        #Si cet élément est le niveau sonore
        if recpinfos["name"]=="soundpressure":
            #Lecture du libellé du dossier parent
            ponc_receiver_name=ui.element(recpinfos["parentid"]).getinfos()["label"]
            #Lecture des informations traité par I-SIMPA. C'est à dire les niveaux directement en dB avec les colonnes de cumul
            gridspl=ui.application.getdataarray(recp)
            #Lecture du cumul sur les pas de temps et sur les fréquences
            global_spl=gridspl[-1][-1]
            #Enregistrement dans le dictionnaire
            global_dict[ponc_receiver_name]=global_spl
    ###
    #Interface de sélection du récepteur ponctuel de référence
    #Création de la liste des libellés des récepteurs ponctuels
    sorted_pr_labels=global_dict.keys()
    #Tri des libellés
    sorted_pr_labels.sort()
    #Création des champs à afficher à l'utilisateur
    lbl_pr_dialog=_("Ponctual receiver name")
    choices={lbl_pr_dialog : sorted_pr_labels}
    #Affichage de la fenêtre de dialogue
    user_choice=ui.application.getuserinput("Normalisation tool","Please choose the reference data.",choices)
    #Si l'utilisateur a validé
    if user_choice[0]:
        #Lecture du libellé du récepteur de référence
        pr_reference=user_choice[1][lbl_pr_dialog]
        sub_by=global_dict[pr_reference]
        ##
        # Création de l'objet d'écriture du format de grille de données. Le nombre de colonnes correspond au nombre de récepteurs ponctuels et une colonne de libellé
        out_grid=ls.Gabe_rw(len(global_dict.keys())+1)
        #Création d'un vecteur de chaine de caractères
        labels_vector=ls.stringarray()
        #Ajout d'une ligne
        labels_vector.append(_("Global").encode("cp1252"))
        #Ajout de la colonne de libellé
        out_grid.AppendStrCol(labels_vector,"")
        ##
        #Calcul du facteur de normalisation pour chaque récepteur ponctuel
        for pr_name in sorted_pr_labels:
            #Ajout d'une colonne par récepteur
            #Création d'un vecteur de nombre
            spl_vector=ls.floatarray()
            spl_vector.append(global_dict[pr_name]-sub_by)
            #Ajout de la colonne
            out_grid.AppendFloatCol(spl_vector,pr_name,3)
    
        #L'utilisateur ne pourra pas modifier la feuille de données
        out_grid.SetReadOnly()
        #Sauvegarde de la grille
        out_grid.Save(save_as)
        return True
    return False
#Déclaration de la classe qui contiendra l'indice de l'outil de normalisation
class manager:
    def __init__(self):
        #On enregistre la méthode afin d'obtenir l'indice à ajouter au menu popup
        self.MakeSPLNormalGridEventId=ui.application.register_event(self.OnMakeSPLNormalGrid)
    #Cette méthode sera exécutée lors-ce que l'utilisateur clique bouton droit sur un dossier de résultat
    def getmenu(self,typeel,idel,menu):
        el=ui.element(idel)
        infos=el.getinfos()
        if infos["name"]=="Récepteurs_Ponctuels":
            menu.insert(0,(_("Normalise SPL"),self.MakeSPLNormalGridEventId))
            return True
        else:
            return False
    def OnMakeSPLNormalGrid(self,idel):
        grp=ui.e_file(idel)
        if MakeSPLNormalGrid(idel,(grp.buildfullpath().decode('cp1252')+_("SPL_norm")+".gabe").encode("cp1252")):
            #raffraichie l'arbre de résultat
            ui.application.sendevent(grp,ui.idevent.IDEVENT_RELOAD_FOLDER)
ui.application.register_menu_manager(ui.element_type.ELEMENT_TYPE_REPORT_FOLDER, manager())
            
