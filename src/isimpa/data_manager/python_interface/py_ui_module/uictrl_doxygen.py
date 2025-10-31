#----------------------------------------------------------------------
# I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
#
# I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
# to scientific acoustic simulations.
# Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
#
# I-SIMPA is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# I-SIMPA is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA or 
# see <http://ww.gnu.org/licenses/>
#
# For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or
# send an email to i-simpa@ifsttar.fr
#
# To contact Ifsttar, write to Ifsttar, 14-20 Boulevard Newton
# Cite Descartes, Champs sur Marne F-77447 Marne la Vallee Cedex 2 FRANCE
# or write to scientific.computing@ifsttar.fr
# ----------------------------------------------------------------------

##
 #  \~english
 #  \mainpage I-SIMPA scripting guide
 #  
 # There are two main ways to make script for I-SIMPA.
 #
 # The first way is aiming at append functionnality via the right-click on tree elements.
 # The goal of the second way is to append element data to projects tree.
 # - \subpage howtoappendfunctionnality
 # - \subpage howtoappenddata
 
 ##
 #  \~french
 #  \mainpage Guide de création de script I-SIMPA
 #  
 # Il y a deux finalitées à la création de script dans I-SIMPA
 # 
 # La première vise à ajouter des fonctionnalitées au logiciel via des menus supplémentaire lors-ce que l'on fait un clique droit sur un élément.
 # On peut également ajouter des données aux arbres du projet.
 # - \subpage howtoappendfunctionnality
 # - \subpage howtoappenddata
 
##
 # \~english
 # \page howtoappendfunctionnality I-SIMPA adding functionnality
 #
 # Adding popup menu functionnality need 4 steps :
 #
 # - Make module sub-folder in UserScript folder and add __ui_startup.py file that import this module.
 # - \ref registerevent in menu manager constructor.
 # - \ref registermenumanager object in __init__.py file.
 # - \ref definegetmenu that will append items in menu list structure.
 #
 # \section registerevent Register events
 # 
 # Each python implemented function had an integer index called event type. Register the function give this new event type index.
 # This index will be used later in the menu structure at the last step.
 #
 # To register new event type call the method uictrl::application::register_event
 # \par Sample
 # \dontinclude samples/__init__.py
 # \skip class manager
 # \until activation(idgrp,False)
 # \section registermenumanager Register a new menu manager
 #
 #
 # To register a new menu manager call the function uictrl::application::register_menu_manager
 # 
 # \par Sample
 # \skipline uictrl.application.register_menu_manager
 #
 # - The first parameter uictrl::element_type indicate the associated element type with the manager.
 # - The second parameter is the instance of the manager.
 #
 # \section definegetmenu Define getmenu method
 #
 # When the user rigth click on an items I-SIMPA will call the getmenu function of all menu manager
 # registered with the item element type.
 #
 # This method must return true if you have modified the menu list data, false otherwise.
 #
 # \par Sample
 # \dontinclude samples/__init__.py
 # \skip def getmenu
 # \until return True
 # \section linkupdateevent Run python code on element update
 #
 # You can link python method with any application element, this method is call when this element will be updated by I-SIMPA or the user. The method parameters must be an element id.
 # @warning Do not call ui::element::Update due to infinite loop.
 # The uictrl::element::register_update_manager do this operation
 
 ##
 # \~french
 # \page howtoappendfunctionnality Ajouter des fonctionnalitées
 #
 # L'ajout d'une nouvelle fonction se fait en 4 étapes :
 #
 # - Créer un dossier(module) dans le dossier UserScript// et ajouter un fichier dans ce dossier nommé "__ui_startup.py" qui va importer le module.
 # - \ref registerevent lors de l'initialisation du gestionnaire de menu.
 # - \ref registermenumanager dans le fichier __init__.py
 # - \ref definegetmenu qui va ajouter les nouvelles fonctions dans la structure de données en paramètre.
 #
 # \section registerevent Enregistrer les événements
 # 
 # Chaque fonction implémenté en python doit être associé à un indice. Cet indice est obtenu grâce à l'enregistrement de cette fonction.
 # Cette indice sera utilisé ultérieurement dans la structure du menu à la dernière étape
 #
 # Vous devez appeler la méthode uictrl::application::register_event et stocker l'indice retourné.
 # \par Exemple
 # \dontinclude samples/__init__.py
 # \skip class manager
 # \until activation(idgrp,False)
 # \section registermenumanager Enregistrer un nouveau gestionnaire de menu
 #
 #
 # La méthode uictrl::application::register_menu_manager permet de déclarer un nouveau gestionnaire de menu.
 # 
 # \par Exemple
 # \skipline uictrl.application.register_menu_manager
 #
 # - Le premier paramètre uictrl::element_type indique le type d'élément associé avec le gestionnaire.
 # - Le deuxième paramètre correspond à l'instance du gestionnaire
 #
 # \section definegetmenu Créer la méthode "getmenu"
 #
 # Quand l'utilisateur fait un clique droit sur un élément de l'arbre du projet I-SIMPA appel la méthode getmenu pour chaque gestionnaire lié à ce type d'élément.
 #
 # Votre méthode doit retourner vrai afin de prendre en compte les modifications.
 #
 # \par Exemple
 # \dontinclude samples/__init__.py
 # \skip def getmenu
 # \until return True
 #
 # \section linkupdateevent Executer du code lors de la mise à jour d'un élément
 #
 # Vous pouvez lier une méthode python à n'importe quel élément de l'application, quand cet élément sera modifié par le programme ou par l'utilisateur alors cette méthode sera appelé avec en paramètre l'indice de l'élément mis à jour (indice même de l'élément ou d'un de ses fils)
 # La méthode  uictrl::element::register_update_manager vous permet d'effectuer cette opération.
 
 ##
 # \~english
 # \page howtoappenddata Add data in projects tree
 #
 # Adding fields to projects is not suffisant. You must take control of theses fields to implement constraint
 # and to adding more functionnality.
 #
 # Step to add data in trees :
 # - Append a folder in UserScript/
 # - \ref createelementtype in the UserScript/yourmod/__init__.py
 # - \ref registernewmod in __project_loading__.py
 #
 # \section createelementtype Creation of the new element type
 # 
 # The sample in this guide is aiming at linking projet with a new calculation core.
 # First of all, you need to build the python class inherit from uictrl::element class and with the uictrl::element_type::ELEMENT_TYPE_CORE_CORE base id.
 # \par Sample
 # \dontinclude samples/add_data_sample.py
 # \skip class mdf
 # \until pass
 #
 # In the __init__ constructor you can add your mod's properties . But you have to test their existance because this constructor is also called when loading a project.
 # 
 # \subsection treelabel Tree label
 # By default the name shown in the tree is the class name. To set another label you must overide the gettreelabel function.
 # 
 # \par Sample
 # \skip gettreelabel
 # \until return
 #
 # \subsection icondecl Icon
 #
 # There are two kind of icon :
 # - Built-in icon referenced by the uictrl::graph enumeration and declared by element::geticonid(self) that return graph id.
 # - Local declaration of icon. Declared by element::geticonpath(self) that return the icon path.
 # 
 # \par Sample
 # \skip geticonid
 # \until return uictrl.graph.GRAPH_FOLDER
 #
 # \subsection eventupdt Modification event
 # 
 # From the property itself to the highest parent the method uictrl::element::modified is automatically called when the user change the value of a property.
 #
 # This is a usefull method to implement properties constraints.
 #
 # In our sample, whe use this method to disable time dependant properties when the user choose the static resolution method.
 # 
 # \par Sample
 # \skip modified
 # \until uictrl.element.modified
 #
 # \section registernewmod Register the new module
 #
 # All files named "__project_loading__.py" in the UserScript/ folder and sub folder is executed when the user create or load a project.
 #
 # You need to create this file in your module folder to register your new module in new projects and in existing projects that doesn't contain your module.
 # 
 # In the source code check first if your module was not already inserted in the project.
 # Then insert your module thanks to the uictrl::element::appenduserelement method.
 #
 # You can use the already imported library uictrl under the name "ui"
 #
 # \par Sample
 # \include samples/__project_loading__.py
  
 ##
 # \~french
 # \page howtoappenddata Ajouter des données dans les projets
 #
 # Ajouter des propriétés aux éléments n'est pas suffisant. Vous devez prendre le contrôle de ses élément afin d'implémenter des contraintes de propriétés et pour ajouter plus de fonctionnalitées.
 #
 # Les étapes pour ajouter un nouvel élément sont :
 # - Créer un dossier dans UserScript/
 # - \ref createelementtype dans le dossier UserScript/votre_module/__init__.py
 # - \ref registernewmod dans le script __project_loading__.py
 #
 # \section createelementtype Créer un nouveau type d'élément
 # 
 # L'exemple de notre guide est la création d'un lien entre les projets et un nouveau code de calcul.
 # En premier lieu, il faut créer une classe héritant de uictrl::element et ayant comme type d'élément de base uictrl::element_type::ELEMENT_TYPE_CORE_CORE.
 # \par Exemple
 # \dontinclude samples/add_data_sample.py
 # \skip class mdf
 # \until pass
 # Dans le constructeur __init__ les propriétés sont ajoutées. Toutefois rien n'est fait si elles existent déjà car ce constructeur est appellé également lors-ce que l'utilisateur charge un projet déjà existant.
 # 
 # \subsection treelabel Libellé de l'élément dans l'arbre du projet
 # Par défaut c'est le nom de la classe qui est affiché dans l'arbre du projet. Pour modifier le libellé vous devez surcharger la fonction gettreelabel.
 # 
 # \par Exemple
 # \skip gettreelabel
 # \until return
 #
 # \subsection icondecl Icone
 #
 # Il y a deux types d'icônes :
 # - Les indices fixes, visible dans l'énumeration uictrl::graph et déclaré par element::geticonid(self) qui doit retourne un membre de l'énumeration.
 # - La déclaration locale des icônes via la fonction element::geticonpath(self) qui doit retourner le chemin d'accès au fichier d'icône (*.ico)
 # 
 # \par Exemple
 # \skip geticonid
 # \until return uictrl.graph.GRAPH_FOLDER
 #
 # \subsection eventupdt Evenement de mise à jour
 # 
 # Dès que l'utilisateur modifie une propriété l'application appelle la méthode uictrl::element::modified à partir de la propriété jusqu'à l'élément racine du projet.
 #
 # Cette méthode trés utile vous permettra par exemple d'implémenter les contraintes dans les propriétés.
 #
 # Dans l'exemple, elle est utilisée afin de désactiver l'accès à la modification des propriétés dépendantes du temps si l'utilisateur séléctionne une méthode de résolution statique.
 # 
 # \par Exemple
 # \skip modified
 # \until uictrl.element.modified
 #
 # \section registernewmod Enregistrer le nouveau module
 #
 # Lors-ce que l'utilisateur créé ou charge un projet tout les fichiers nommés "__project_loading__.py" et présent dans le dossier UserScript/ et ses sous dossiers sont éxécutés.
 #
 # Il est nécessaire de créer ce fichier dans votre module afin d'ajouter celui-ci dans les projets.
 # 
 # Il faut au préalable vérifier que l'ajout n'a pas été déjà effectué. Dès lors il faut utiliser la méthode uictrl::element::appenduserelement.
 #
 # @note Vous pouvez utiliser la librairie uictrl déjà importé sous le nom ui.
 #
 # \par Exemple
 # \include samples/__project_loading__.py
 
 
 
 
 
 
