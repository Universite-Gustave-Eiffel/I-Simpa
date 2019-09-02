/* ----------------------------------------------------------------------
* I-SIMPA (http://i-simpa.ifsttar.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) 2007-2014 - IFSTTAR - Judicael Picaut, Nicolas Fortin
*
* I-SIMPA is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
* 
* I-SIMPA is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA or 
* see <http://ww.gnu.org/licenses/>
*
* For more information, please consult: <http://i-simpa.ifsttar.fr> or 
* send an email to i-simpa@ifsttar.fr
*
* To contact Ifsttar, write to Ifsttar, 14-20 Boulevard Newton
* Cite Descartes, Champs sur Marne F-77447 Marne la Vallee Cedex 2 FRANCE
* or write to scientific.computing@ifsttar.fr
* ----------------------------------------------------------------------*/

#include "first_header_include.hpp"

#include "data_manager/python_interface/boost_python_header.h"
#include "element_pywrap.hpp"
#include "data_manager/projet.h"
#include <wx/intl.h>

#ifdef USE_PYTHON

/**
 * @file Application.hpp
 * \~english \brief Python application control class
 * \~french \brief Classe python de contrôle de I-SIMPA
 */


class Element;
#ifndef __APPLICATION_PY__CLASS__
#define __APPLICATION_PY__CLASS__
/**
 * @namespace uictrl
 * \~french  @brief Portage des fonctions c++ vers python
 * \~english  @brief Python embedding of c++ class
 */
namespace uictrl
{

/**
 * \~english  @brief Python application control class
 * \~french @brief Classe python de contrôle de I-SIMPA 
 */
class application
{
protected:
	/**
	 * \~english Return tree object (or null) from the element tree id
	 */
	static uiTreeCtrl* GetTreeFromElement(const wxTreeItemId& treeItemId);
	
public:
	/**
	 * \~english Clear the log window history  
	 * \~french  Efface l'historique de la fenêtre de messages
	 */
	static void clearlogdata();
	/**
	 * \~english Clear the python log window history  
	 * \~french  Efface l'historique de la fenêtre de console python
	 */
	static void clearshellhisto();

    /**
    * \~english Print message in the log window
    * \~french  Affiche un message dans la fenêtre de console
    */
    static void loginfo(const std::wstring& path);

    /**
    * \~english Print message in the log window
    * \~french  Affiche un message dans la fenêtre de console
    */
    static void logwarning(const std::wstring& path);

    /**
    * \~english Print message in the log window
    * \~french  Affiche un message dans la fenêtre de console
    */
    static void logerror(const std::wstring& path);


	/**
	 * \~english Close current project and make a new one 
	 * \~french  Ferme le projet courant et en créé un nouveau
	 */
	static void newproject();
	/**
	 * \~english Save the loaded project
	 * @param path File save path
	 * \~french  Sauvegarde le projet ouvert
	 * @param path Dossier de sauvegarde du projet
	 */
	static void saveproject(const std::wstring& path=L"");
	/**
	 * \~english Load a file project
	 * @param path File load path
	 * \~french  Charge un fichier de projet
	 * @param path Chemin du fichier de projet à charger
	 */
	static void loadproject(const std::wstring& path);
	/**
	 * \~english Give the access to the root node of a projet tree
	 * @return The element id of the root scene node
	 * \~french  Acceder à l'élément racine d'un arbre
	 * @return L'indice de l'élément racine de l'arbre de la scène
	 */
	static int getrootscene();
	/**
	 * \~english Give the access to the root node of a projet tree
	 * @return The element id of the root core node
	 * \~french  Acceder à l'élément racine d'un arbre
	 * @return L'indice de l'élément racine de l'arbre des codes de calcul
	 */
	static int getrootcore();
	/**
	 * \~english Give the access to the root node of a projet tree
	 * @return The element id of the root report node
	 * \~french  Acceder à l'élément racine d'un arbre
	 * @return L'indice de l'élément racine de l'arbre des résulats de calculs
	 */
	static int getrootreport();
	/**
	 * \~english Give the access to the user preference root node of the application tree
	 * @return The element id of the root user preference
	 * \~french  Acceder à l'élément racine des préférences utilisateur
	 * @return L'indice de l'élément racine de l'arbre propre à l'utilisateur (et non au projet)
	 */
	static int getrootpreference();
	/**
	 * \~english Save the content of the message log window
	 * @param path Save file path
	 * \~french  Sauvegarde le contenu de la fenêtre de messages.
	 * @param path Chemin du fichier à créer
	 */
	static void savelog(const std::wstring& path);
	/**
	 * \~english Save the content of the python log window
	 * @param path Save file path
	 * \~french  Sauvegarde le contenu de la fenêtre de console python.
	 * @param path Chemin du fichier à créer
	 */
	static void saveshell(const std::wstring& path);
	/**
	 * \~english Import an outside defined model. Supported file format is *.3ds;*.ply;*.bin;*.poly,*.stl
	 * @param path Model file path
	 * @param keepexistingfacegroup If True, it will try to fill face groups with the new faces thanks to old-new faces position.
	 * @param docorrection Execute preprocess to split or destroy triangle faces if needed.
	 * @param domeshsurface Remesh the surface of the scene. This operation may increase the number of facets and destroy material color and textures data.
	 * @param paramTetgen User defined parameter for the mesh software. Useless parameter if domeshsurface at False.
	 * @return True on success, false if import fails, see message log for details
	 * \~french  Importe un model 3D qui remplacera le modèle du projet courant. Les formats de model supportés sont *.3ds;*.ply;*.bin;*.poly,*.stl
	 * @param path Chemin du fichier de modèle
	 * @param keepexistingfacegroup A vrai, Le groupes seront conservés grâce aux coordonnées similaires entre les anciennes et nouvelles faces.
	 * @param docorrection Exécute preprocess pour éclater ou supprimer les faces afin de tente de rendre possible le maillage.
	 * @param domeshsurface Tente de mailler la surface du modèle. Cette opération peut augmenter le nombre de faces et elle détruira les informations de couleurs et de textures.
	 * @param paramTetgen Paramètres de maillage. Inutile si domeshsurface à faux.
	 * @return Vrai en cas de succès. Faux si l'importation a échoué, se référer à la fenêtre de message pour plus d'information.
	 * @param path Chemin du fichier à créer
	 */
	static bool importscene(const std::wstring& path,bool keepexistingfacegroup=true,bool docorrection=true,bool domeshsurface=false,const std::wstring& paramTetgen=L"");
	/**
	 * \~english Send an event to the interface.
	 * @param pyel Corresponding tree item node.
	 * @param idevent Built-in uictrl::idevent or python defined event by application::register_event
	 * @param parameters See uictrl::idevent member for more specific details.
	 * \~french Execution d'un événement.
	 * @param pyel Element de l'arbre concerné
	 * @param idevent Evénement interne uictrl::idevent ou événement d'un module python déclaré via application::register_event
	 * @param parameters Voir les membres de uictrl::idevent pour plus de détails.
	 */
	static void sendevent(const element& pyel, const int& idevent, boost::python::dict parameters=boost::python::dict());
	/**
	 * \~english Return the associated data array with the element.
	 * For scene and core elements, the returned array is the property tab.
	 * For report element that herits from gabe element, this method return the post-processed array.
	 * @param pyel The data array will be extracted from this parameter.
	 * \~french Retourne les données sous forme de tableau associé à un élément
	 * Pour les éléments ayant des propriétés il est rendu un tableau de propriété
	 * Pour les éléments e_file portant des données binaires il est rendu le tableau avec le post-traitement de I-SIMPA.
	 * @param pyel Les données du tableau seront extraites à partir de cet élément.
	 */
	static boost::python::list getdataarray(const element& pyel);

	/**
	 * \~english Show a window form where the user can write text in each field.
	 * @param title Title of the window
	 * @param msg Message text information.
	 * @param rows Message fields.dict sample {"field one" : "default value" ,"field two" : "default value", "field three" : ["Value 1", "Value 2"] }
	 * @return Tuple (Bool, list) The first cell is the button user choice ok:True cancel:False. The second cell contain a list that had the same size of the rows parameter but contains the new fields values.
	 * \~french Demande à l'utilisateur d'entrer des valeurs dans des champs via l'interface
	 * @param title Titre de la fenêtre
	 * @param msg Message affiché à l'utilisateur
	 * @param rows Champs en entrée {"champ1" : "valeur par défaut","champ2" : "valeur par défaut","liste champ3" : ["choix 1","choix 2","choix 3"],..}
	 * @return Tuple (Booléen, Liste) Le booléen indique le choix de l'utilisateur (ok, annuler) et la liste contient les valeurs des champs dans le même ordre que le paramètre rows.
	 */
	static boost::python::tuple getuserinput(const std::wstring& title,const std::wstring& msg,boost::python::dict rows);
	/**
	 * \~english Append a new menu manager for this element type. See @ref howtoappendfunctionnality
	 * @param element_typeid uictrl::element_type Manager's linked element type.
	 * @param manager Manager object instance.
	 * \~french Voir @ref howtoappendfunctionnality .Enregistre un nouveau gestionnaire de menu pour ce type d'élément.
	 * @param element_typeid uictrl::element_type Type d'élément lié à ce gestionnaire de menu.
	 * @param manager Instance de l'objet du manager.
	 */
	static void register_menu_manager(const int& element_typeid, boost::python::object& manager);
	/**
	 * \~french Voir @ref howtoappendfunctionnality. Permet de lier un indice de menu à une fonction python.
	 * @param func Méthode python à appeler en cas d'execution d'un événement.
	 * @return Indice du nouveau type d'événement de I-SIMPA.
	 * \~english Append a new event to I-SIMPA. See @ref howtoappendfunctionnality
	 * @param func Reference to the python function.
	 * @return Integer id of the new function
	 */
	static int register_event(boost::python::object& func);
	/**
	 * Recharge les faces à partir du modèle
	 *
	 */
	static void reloadgroupsfrommodel();

	/**
	 * \~french Retourne un dictionnaire contenant les chemins de l'application
	 * - userscript Chemin complet du dossier de script utilisateur;
	 * - cachedir Chemin d'accès au dossier temporaire du projet;
	 * - lastcalculationpath Chemin du dernier calcul effectué;
	 * \~english Return a dictionary that contains multiple path of the application
	 * - userscript Full path of the userscript folder;
	 * - cachedir Projet cache directory;
	 * - lastcalculationpath Last computation result folder;
	 */
	static boost::python::dict getapplicationpath();

	/**
	 * \~french Retourne la langue séléctionné par l'utilisateur dans la forme standart. La forme standart correspond à celle utilisée dans les système UNIX: Il s'agit d'une chaine de caractères sous forme xx ou xx_YY, où xx correspond à la norme ISO 639 et YY à la norme ISO 3166 relatif au pays. Il y a par exemple "en", "en_GB", "en_US" ou "fr_FR".
	 * \~english Return the user selected language corresponding to the canonical form of current locale name. Canonical form is the one that is used on UNIX systems: it is a two- or five-letter string in xx or xx_YY format, where xx is ISO 639 code of language and YY is ISO 3166 code of the country. Examples are "en", "en_GB", "en_US" or "fr_FR".
	 */
	static std::wstring getlocale();

	/**
	 * \~french Effectue un glisser déposer des éléments sélectionné vers l'élément dont l'indice est passé en argument.
	 * \~english Drag And Drop the selection to the element id in parameter.
	 */
	static void dropselectionto(const element& pyel);

	/**
	 * \~french Sélectionne les éléments dont l'indice est passé en paramètre
	 * @param intlst Liste avec l'indice des éléments à sélectionner
	 * @warning Ils doivent tous appartenir à la même arborescence
	 * \~english Unselect previous selection, then select all the element passed as parameter.
	 * @param intlst Integer list with the element id as list item.
	 * @warning All the items in the list must be from the same tree.
	 */	
	static void setselection( const boost::python::list& intlst);

    /**
     * @return application version
     */
    static std::wstring getversion();
};
}
#endif
#endif