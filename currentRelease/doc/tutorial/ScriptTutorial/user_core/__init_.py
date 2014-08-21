# -*- coding: cp1252 -*-
import uictrl as ui

# Déclaration de la méthode de traduction en cas d'erreur d'import du dictionnaire
def _(msg):
    return msg

try:
    import uilocale
    #Construction du chemin du script
    ScriptFolder=ui.application.getapplicationpath()["userscript"]+"user_core\\"
    #Déclaration de la méthode de traduction
    _=uilocale.InstallUiModule(ScriptFolder,ui.application.getlocale())
except:
    #En cas d'erreur afficher l'erreur et continuer
    import sys, traceback
    print("Unable to import the language dictionary !")
    traceback.print_exc(file=sys.stdout)


class user_core(ui.element):
    """
      Code de calcul utilisateur
    """
    def __init__(self,idel):
		#Initialisation de l'élément
        ui.element.__init__(self,idel)
        
        if not self.hasproperty("exeName"): #La propriété n'existe pas s'il s'agit d'un nouveau projet, elle existe si le projet est chargé
            #Création des paramètres du maillage tetgen, ne pas le faire si le calcul n'a pas besoin de maillage tétraèdrique
            self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFMAILLAGE)
			#Ajout de la sélection des bandes de fréquences, ne pas le faire si c'est inutile
            self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_BFREQSELECTION)
			#Ajout du noeud de configuration, qui contient par défaut les propriétés de pas de temps et de durée de simulation
            coreconf=ui.element(self.appendfilsbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG))
			####
			#Ces propriétés sont nécessaire pour que I-SIMPA connaisse les fichiers relatifs au code de calcul
			#Nom et format du modèle 3D
            ui.element(self.appendpropertytext("modelName","","mesh.cbin",True,True)).hide()
			#Nom du maillage tétraèdrique
            ui.element(self.appendpropertytext("tetrameshFileName","","tetramesh.mbin",True,True)).hide()
			#Nom et type (exe,py ou pyc) du fichier executable
            ui.element(self.appendpropertytext("exeName","","user_core.exe")).hide()
            ui.element(self.appendpropertytext("corePath","","usercore\\")).hide()
            
            #Création des paramètres de calculs
            coreconf.appendpropertylist("solver_mode",_("Mode de calcul"),[[_("Temporel"),_("Stationnaire")],[0,1]],0,False,1,True)
        else:
            #Chargement d'un projet existant
            pass
    def gettreelabel(self):
        """
            Retourne le libellé visible dans l'arbre
        """
        return "Code de calcul utilisateur"
    def geticonid(self,state_open):
        """
            Retourne l'indice de l'icone de l'élément en fonction de son état
        """
        if state_open:
            return ui.graph.GRAPH_FOLDER_OPEN
        else:
            return ui.graph.GRAPH_FOLDER
    ###
    # Cette méthode est appelé par I-SIMPA lors-ce qu'un sous élément du code de calcul est modifié
    # Certains paramètres sont liés entres eux, cette méthode permet de modifier l'accès de modification selon l'état de la méthode de calcul
    def modified(self,idelmodified):
        #Le mode de calcul stationnaire n'a pas besoin des paramètres de pas de temps ni de durée
        if ui.element(idelmodified).getinfos()["name"]=="solver_mode":
            #le mode de calcul a été modifié par l'utilisateur
            elconf=ui.element(self.getelementbytype(ui.element_type.ELEMENT_TYPE_CORE_CORE_CONFIG))
            is_temporel=(elconf.getlistconfig("solver_mode")==0)
            elconf.setreadonlyconfig("duree_simulation",not is_temporel)
            elconf.setreadonlyconfig("pasdetemps",not is_temporel)
        ui.element.modified(self,idelmodified)
