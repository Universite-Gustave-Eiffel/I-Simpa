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
#include "data_manager/element.h"
/**
 * @file element_pywrap.hpp
 * Contrôle d'un élément implémenté en python ou c++
 *
 */
#ifndef __PYTHON_ELEMENT_WRAP__
#define __PYTHON_ELEMENT_WRAP__

#ifdef USE_PYTHON
boost::python::list GetMenuItems(wxMenu* menuToFill);

namespace uictrl
{

/**
 * \~french @brief Contrôle d'un élément implémenté en python ou c++
 *
 * Toute les données du projet dérivent de cette classe. De la simple propriété chaine de caractère, booléen et nombre à décimal jusqu'a un noeud de l'arbre du projet.
 * \~english @brief Give control on a built-in(c++) or python implemented element
 *
 * An element is the base class of all project data. From the basic string,bool,float and integer to a project tree node.
 */
class element : public boost::noncopyable
{
public:
	element(const wxInt32& _xmlId);
	element(const element& cpyFrom);
	wxInt32 getindice() const;
	/**
	 * \~french Retourne un dictionnaire comportant les informations suivantes :
	 * - typeElement : Type de l'élément (uictrl.element_type)
	 * - xmlIdElement : Indice de l'élément ( @see GetIndice )
	 * - expanded : Indique si l'élément est déplié ou non
	 * - userDestroyable : Indique si l'élément peut être supprimé par l'utilisateur
	 * - label : Libellé d'origine de l'élément
	 * - label_located : Libellé traduit dans la langue de l'utilisateur
	 * - name : Nom de l'élément
	 * - parentid : Indice de l'élément parent. -1 si aucun
	 * \~english Return a dict with the following keys :
	 * - typeElement : Element Type (uictrl::element_type)
	 * - xmlIdElement : Element index
	 * - expanded : True if element tree is expanded
	 * - userDestroyable : True if the user is able to destroy this element
	 * - label : Untranslated version of the element label
	 * - label_located : Translated element label using the locale language of application.
	 * - name : Element name.
	 * - parentid : Index of the parent element. -1 if none.
	 */
	boost::python::dict getinfos();
	/**
	 * \~english For each element child this function return The index of element,the type, the name
	 * \~french Retourne l'indice, le type et le nom de chaque enfants
	 */
	boost::python::list childs();
	virtual ~element();
	
	/**
	 * \~english Enable or disable the write access to a property
	 * @param name Name of the property
	 * @param readOnly New state of access
	 * @param col For row property, you can set read only on a specific col only.
	 * \~french Active/Désactive la possibilité de modifier le champ pour l'utilisateur pour un champ donné
	 * @param name Nom du champ
	 * @param readOnly Nouvel état du champ
	 * @param col Pour les champs ayant plusieurs colonnes tel que E_Data_Row indique quel colonne sera affecté par la lecture seule.
	 */
	void setreadonlyconfig(const std::wstring& name,bool readOnly=true,int col=0);
	/**
	 * \~english Enable or disable the write access to all property of an element.
	 * @param readOnly New state of access
	 * @param col For row property, you can set read only on a specific col only.
	 * \~french Active/Désactive la possibilité de modifier tout les champs pour l'utilisateur pour une colonne donnée
	 * @param readOnly Nouvel état des champs
	 * @param col Pour les champ ayant plusieurs colonnes tel que E_Data_Row indique quel colonne sera affectée par la lecture seule.
	 */
	void setreadonlyallconfig(bool readOnly=true,int col=0);
	/**
	 * \~english Update the value of a property.
	 * @param name Name of the property
	 * @param newValue New value of the property
	 * @return True if the property has been found and updated
	 * \~french Met à jour la valeur d'une propriété
	 * @param name Nom de la propriété
	 * @param newValue Nouvelle valeur de la propriété
	 * @return Vrai si la propriété a été trouvé et mis à jour
	 */
	bool updatestringconfig(const std::wstring& name,const std::wstring& newValue);
	/**
	 * \~english Update the value of a property.
	 * @param name Name of the property
	 * @param newValue New value of the property
	 * @return True if the property has been found and updated
	 * \~french Met à jour la valeur d'une propriété
	 * @param name Nom de la propriété
	 * @param newValue Nouvelle valeur de la propriété
	 * @return Vrai si la propriété a été trouvé et mis à jour
	 */
	bool updatedecimalconfig(const std::wstring& name,float newValue);
	/**
	 * \~english Update the value of a property.
	 * @param name Name of the property
	 * @param newIndex New value of the property
	 * @return True if the property has been found and updated
	 * \~french Met à jour la valeur d'une propriété
	 * @param name Nom de la propriété
	 * @param newIndex Nouvelle valeur de la propriété
	 * @return Vrai si la propriété a été trouvé et mis à jour
	 */
	bool updatelistconfig(const std::wstring& name,int newIndex);	
	/**
	 * \~english Update the value of a property.
	 * @param name Name of the property
	 * @param newValue New value of the property
	 * @return True if the property has been found and updated
	 * \~french Met à jour la valeur d'une propriété
	 * @param name Nom de la propriété
	 * @param newValue Nouvelle valeur de la propriété
	 * @return Vrai si la propriété a été trouvé et mis à jour
	 */
	bool updateentierconfig(const std::wstring& name,int newValue);	
	/**
	 * \~english Update the value of a property.
	 * @param name Name of the property
	 * @param newValue New value of the property
	 * @return True if the property has been found and updated
	 * \~french Met à jour la valeur d'une propriété
	 * @param name Nom de la propriété
	 * @param newValue Nouvelle valeur de la propriété
	 * @return Vrai si la propriété a été trouvé et mis à jour
	 */
	bool updateboolconfig(const std::wstring& name,bool newValue);
	/**
	 * \~english Update the value of a property.
	 * @param name Name of the property
	 * @param newValue New value of the property [x,y,z]
	 * @return True if the property has been found and updated
	 * \~french Met à jour la valeur d'une propriété
	 * @param name Nom de la propriété
	 * @param newValue Nouvelle valeur de la propriété [x,y,z]
	 * @return Vrai si la propriété a été trouvé et mis à jour
	 */
	bool updatepositionconfig(const std::wstring& name,const boost::python::list& newValue);
	/**
	 * \~english Search inside the element's properties, and return True if a property had this name.
	 * @param name Property name
	 * \~french Retourne Vrai si le nom correspond à une des propriétés de l'élément.
	 * @param name Nom de la propriété
	 */
	bool hasproperty(const std::wstring& name);
	/**
	 * \~english Return the value of a property
	 * @param name Name of the property. Please check with element::hasproperty if there are any doubt.
	 * @return Value of the property
	 * \~french Retourne la valeur d'une propriété
	 * @param name Nom de la propriété. En cas de doute de l'existance de cette propriété merci de vérifier celle-ci via  element::hasproperty
	 * @return Valeur de la propriété
	 */
	std::wstring getstringconfig(const std::wstring& name);
	/**
	 * \~english Return the value of a property
	 * @param name Name of the property. Please check with element::hasproperty if there are any doubt.
	 * @return Value of the property [x,y,z] (m)
	 * \~french Retourne la valeur d'une propriété
	 * @param name Nom de la propriété. En cas de doute de l'existance de cette propriété merci de vérifier celle-ci via  element::hasproperty
	 * @return Valeur de la propriété [x,y,z] (m)
	 */
	boost::python::list getpositionconfig(const std::wstring& name); 
	/**
	 * \~english Return the value of a property
	 * @param name Name of the property. Please check with element::hasproperty if there are any doubt.
	 * @return Value of the property
	 * \~french Retourne la valeur d'une propriété
	 * @param name Nom de la propriété. En cas de doute de l'existance de cette propriété merci de vérifier celle-ci via  element::hasproperty
	 * @return Valeur de la propriété
	 */
	float getdecimalconfig(const std::wstring& name);
	/**
	 * \~english Return the value of a property
	 * @param name Name of the property. Please check with element::hasproperty if there are any doubt.
	 * @return Value of the property
	 * \~french Retourne la valeur d'une propriété
	 * @param name Nom de la propriété. En cas de doute de l'existance de cette propriété merci de vérifier celle-ci via  element::hasproperty
	 * @return Valeur de la propriété
	 */
	bool getboolconfig(const std::wstring& name);
	/**
	 * \~english Return the value of a property
	 * @param name Name of the property. Please check with element::hasproperty if there are any doubt.
	 * @return Value of the property
	 * \~french Retourne la valeur d'une propriété
	 * @param name Nom de la propriété. En cas de doute de l'existance de cette propriété merci de vérifier celle-ci via  element::hasproperty
	 * @return Valeur de la propriété
	 */
	int getentierconfig(const std::wstring& name);
	/**
	 * \~english Return the value of a property
	 * @param name Name of the property. Please check with element::hasproperty if there are any doubt.
	 * @return Value of the property [red,green,blue] [0-255]
	 * \~french Retourne la valeur d'une propriété
	 * @param name Nom de la propriété. En cas de doute de l'existance de cette propriété merci de vérifier celle-ci via  element::hasproperty
	 * @return Valeur de la propriété [rouge,vert,bleu] [0-255]
	 */
	boost::python::list getcolorconfig(const std::wstring& name); 
	/**
	 * \~english Return the value of a property
	 * @param name Name of the property. Please check with element::hasproperty if there are any doubt.
	 * @return Value of the property
	 * \~french Retourne la valeur d'une propriété
	 * @param name Nom de la propriété. En cas de doute de l'existance de cette propriété merci de vérifier celle-ci via  element::hasproperty
	 * @return Valeur de la propriété
	 */
	int getlistconfig(const std::wstring& name);

	/**
	 * \~english Append a new property to an element.
	 * @param propertyName Name of the property. Each property name must be unique.
	 * @param propertyLabel Untranslated property label. Use the language catalog (.po) to append the translation correspondance.
	 * @param propertyDefaultValue Default value of the property
	 * @param readOnly Set it to True to forbid write user access to this property.
	 * @param exportToCore Set it to True to make this property visible in calculation core xml configuration document.
	 * @return The element index of the new property or of the property of the same name. -1 in case of failure.
	 * \~french Ajoute une nouvelle propriété à un élément.
	 * @param propertyName Nom de la propriété. Ce nom doit être unique pour chaque propriété.
	 * @param propertyLabel Libellé non traduite de la propriété.
	 * @param propertyDefaultValue Valeur par défaut de la propriété
	 * @param readOnly A vrai l'utilisateur ne pourra pas modifier la valeur du champ. Sauf via la fenêtre python.
	 * @param exportToCore Spécifie l'exportation ou non de cette propriété vers les code de calcul.
	 * @return L'indice xml de l'élément ajouté, ou l'indice de l'élément portant déjà ce nom. -1 si la procédure a echoué.
	 */
	int appendpropertytext(const std::wstring& propertyName,const std::wstring& propertyLabel,const std::wstring& propertyDefaultValue,bool readOnly=false,bool exportToCore=false); 
	/**
	 * \~english Append a new property to an element.
	 * @param propertyName Name of the property. Each property name must be unique.
	 * @param propertyLabel Untranslated property label. Use the language catalog (.po) to append the translation correspondance.
	 * @param propertyDefaultValue Default value of the property [x,y,z]
	 * @param exportToCore Set it to True to make this property visible in calculation core xml configuration document.
	 * @return The element index of the new property or of the property of the same name. -1 in case of failure.
	 * \~french Ajoute une nouvelle propriété à un élément.
	 * @param propertyName Nom de la propriété. Ce nom doit être unique pour chaque propriété.
	 * @param propertyLabel Libellé non traduite de la propriété.
	 * @param propertyDefaultValue Valeur par défaut de la propriété [x,y,z]
	 * @param exportToCore Spécifie l'exportation ou non de cette propriété vers les code de calcul.
	 * @return L'indice xml de l'élément ajouté, ou l'indice de l'élément portant déjà ce nom. -1 si la procédure a echoué.
	 */
	int appendpropertyposition(const std::wstring& propertyName,const std::wstring& propertyLabel,const boost::python::list& propertyDefaultValue,bool exportToCore=false);
	/**
	 * \~english Append a new property to an element.
	 * @param propertyName Name of the property. Each property name must be unique.
	 * @param propertyLabel Untranslated property label. Use the language catalog (.po) to append the translation correspondance.
	 * @param values List option with untranslated labels and index. [["first list item","second list item"],[0,1]]
	 * @param defaultValue Default value of the property
	 * @param asTitle At true this list will be the first property shown.
	 * @param hSize Number of cols occupied by this property.
	 * @param exportToCore Set it to True to make this property visible in calculation core xml configuration document.
	 * @return The element index of the new property or of the property of the same name. -1 in case of failure.
	 * \~french Ajoute une nouvelle propriété à un élément.
	 * @param propertyName Nom de la propriété. Ce nom doit être unique pour chaque propriété.
	 * @param propertyLabel Libellé non traduite de la propriété.
	 * @param values Elements de la liste (non traduit) et leurs indices. [["première option","deuxième option"],[0,1]]
	 * @param defaultValue Valeur par défaut de la propriété
	 * @param asTitle Vrai si cette propriété doit être la première affichée
	 * @param hSize Nombre de colonne occupée par cette propriété.
	 * @param exportToCore Spécifie l'exportation ou non de cette propriété vers les code de calcul.
	 * @return L'indice xml de l'élément ajouté, ou l'indice de l'élément portant déjà ce nom. -1 si la procédure a echoué.
	 */
	int appendpropertylist(const std::wstring& propertyName,const std::wstring& propertyLabel,const boost::python::list& values,long defaultValue, bool asTitle=false, int hSize=1,bool exportToCore=false);
	/**
	 * \~english Append a new property to an element.
	 * @param propertyName Name of the property. Each property name must be unique.
	 * @param propertyLabel Untranslated property label. Use the language catalog (.po) to append the translation correspondance.
	 * @param defaultRed Default red color. [0-255]
	 * @param defaultGreen Default green color. [0-255]
	 * @param defaultBlue Default blue color. [0-255]
	 * @return The element index of the new property or of the property of the same name. -1 in case of failure.
	 * \~french Ajoute une nouvelle propriété à un élément.
	 * @param propertyName Nom de la propriété. Ce nom doit être unique pour chaque propriété.
	 * @param propertyLabel Libellé non traduite de la propriété.
	 * @param defaultRed Niveau de rouge par défaut. [0-255]
	 * @param defaultGreen Niveau de vert par défaut. [0-255]
	 * @param defaultBlue Niveau de bleu par défaut. [0-255]
	 * @return L'indice xml de l'élément ajouté, ou l'indice de l'élément portant déjà ce nom. -1 si la procédure a echoué.
	 */
	int appendpropertycolor(const std::wstring& propertyName,const std::wstring& propertyLabel,long defaultRed=0,long defaultGreen=0,long defaultBlue=0);
	/**
	 * \~english Append a new property to an element.
	 * @param propertyName Name of the property. Each property name must be unique.
	 * @param propertyLabel Untranslated property label. Use the language catalog (.po) to append the translation correspondance.
	 * @param propertyDefaultValue Default value of the property
	 * @param exportToCore Set it to True to make this property visible in calculation core xml configuration document.
	 * @param isMaxValue True to activate max value constraint.
 	 * @param isMinValue True to activate min value constraint.
  	 * @param maxValue Maximum value of the field.
   	 * @param minValue Minimum value of the field.
	 * @return The element index of the new property or of the property of the same name. -1 in case of failure.
	 * \~french Ajoute une nouvelle propriété à un élément.
	 * @param propertyName Nom de la propriété. Ce nom doit être unique pour chaque propriété.
	 * @param propertyLabel Libellé non traduite de la propriété.
	 * @param propertyDefaultValue Valeur par défaut de la propriété
	 * @param exportToCore Spécifie l'exportation ou non de cette propriété vers les code de calcul.
	 * @param isMaxValue Vrai pour activer les contraintes de limite maximale.
 	 * @param isMinValue Vrai pour activer les contraintes de limite minimale.
  	 * @param maxValue Valeur maximum autorisée.
   	 * @param minValue Valeur minimale autorisée.
	 * @return L'indice xml de l'élément ajouté, ou l'indice de l'élément portant déjà ce nom. -1 si la procédure a echoué.
	 */
	int appendpropertyentier(const std::wstring& propertyName,const std::wstring& propertyLabel,int propertyDefaultValue=0,bool exportToCore=false,bool isMaxValue=false,bool isMinValue=false,int maxValue=0,int minValue=0); 
	/**
	 * \~english Append a new property to an element.
	 * @param propertyName Name of the property. Each property name must be unique.
	 * @param propertyLabel Untranslated property label. Use the language catalog (.po) to append the translation correspondance.
	 * @param propertyDefaultValue Default value of the property
	 * @param readOnly Set it to True to forbid write user access to this property.
	 * @param precision Precision showed after dot decimal separator.
	 * @param isMaxValue True to activate max value constraint.
 	 * @param isMinValue True to activate min value constraint.
  	 * @param maxValue Maximum value of the field.
   	 * @param minValue Minimum value of the field.
 	 * @param exportToCore Set it to True to make this property visible in calculation core xml configuration document.
	 * @return The element index of the new property or of the property of the same name. -1 in case of failure.
	 * \~french Ajoute une nouvelle propriété à un élément.
	 * @param propertyName Nom de la propriété. Ce nom doit être unique pour chaque propriété.
	 * @param propertyLabel Libellé non traduite de la propriété.
	 * @param propertyDefaultValue Valeur par défaut de la propriété
	 * @param readOnly A vrai l'utilisateur ne pourra pas modifier la valeur du champ. Sauf via la fenêtre python.
	 * @param precision Nombre de chiffre affiché après la virgule.
	 * @param isMaxValue Vrai pour activer les contraintes de limite maximale.
 	 * @param isMinValue Vrai pour activer les contraintes de limite minimale.
  	 * @param maxValue Valeur maximum autorisée.
   	 * @param minValue Valeur minimale autorisée.
 	 * @param exportToCore Spécifie l'exportation ou non de cette propriété vers les code de calcul.
	 * @return L'indice xml de l'élément ajouté, ou l'indice de l'élément portant déjà ce nom. -1 si la procédure a echoué.
	 */
	int appendpropertydecimal(const std::wstring& propertyName,const std::wstring& propertyLabel,float propertyDefaultValue=0.f,bool readOnly=false,int precision=4,bool isMaxValue=false,bool isMinValue=false,float maxValue=0,float minValue=0,bool exportToCore=false); 
	/**
	 * \~english Append a new property to an element.
	 * @param propertyName Name of the property. Each property name must be unique.
	 * @param propertyLabel Untranslated property label. Use the language catalog (.po) to append the translation correspondance.
	 * @param propertyDefaultValue Default value of the property
 	 * @param exportToCore Set it to True to make this property visible in calculation core xml configuration document.
	 * @return The element index of the new property or of the property of the same name. -1 in case of failure.
	 * \~french Ajoute une nouvelle propriété à un élément.
	 * @param propertyName Nom de la propriété. Ce nom doit être unique pour chaque propriété.
	 * @param propertyLabel Libellé non traduite de la propriété.
	 * @param propertyDefaultValue Valeur par défaut de la propriété
 	 * @param exportToCore Spécifie l'exportation ou non de cette propriété vers les code de calcul.
	 * @return L'indice xml de l'élément ajouté, ou l'indice de l'élément portant déjà ce nom. -1 si la procédure a echoué.
	 */
	int appendpropertybool(const std::wstring& propertyName,const std::wstring& propertyLabel,bool propertyDefaultValue,bool exportToCore=false); 
	/**
	 * \~english Not implemented
	 * \~french Non implémenté.
	 */
	int appendpropertyfont(const std::wstring& propertyName,const std::wstring& propertyLabel,const std::wstring& propertyDefaultValue=L"");
	/**
	 * \~english Return the final (built-in+python) menu with nested list containing tuple (translated name, event_id)
	 * \~french Retourne le menu final (logiciel+python menu manager) sous forme de liste en arbre contenant pour chaque élément de menu le tuple ( libellé traduit, indice event_id )
	 */
	boost::python::list getmenu();
	/**
	 * \~english Navigate recursively through childrens and return an index list of all corresponding elements.
	 * \~french Parcourt récursivement tout les enfants de l'élément et retourne la liste de tout les éléments ayant ce type.
	 */
	boost::python::list getallelementbytype(Element::ELEMENT_TYPE typeElement);
	/**
	 * \~english Return the element id of the first children with the element_type
	 * \~french Retourne le premier enfant ayant le même type que le paramètre de la méthode.
	 */
	int getelementbytype(Element::ELEMENT_TYPE typeElement);
	/**
	 * \~english Return the element id of the first children with the same element name
	 * \~french Retourne le premier enfant ayant le même libellé
	 */
	int getelementbylibelle(std::wstring libelle);
	/**
	 * \~english Set user defined python object as a children of this element
	 * @param baseType Base type of the new element
	 * @param moduleName Module name of the new element
	 * @param className Class name of the new element
	 * \~french Affecte un module python en tant qu'enfant de se module.
	 * @param baseType Type de base de l'élément
	 * @param moduleName Nom du module python (nom du dossier dans UserScript/)
	 * @param className Nom de la classe du nouvel élément.
	 */
	boost::python::object appenduserelement( const Element::ELEMENT_TYPE& baseType, const std::wstring& moduleName,const std::wstring& className );
	/**
	 * \~english Add a new child of etypefils element type and return this xml id. Return -1 in case of failure.
	 * @param etypefils Element type of the children
	 * \~french Ajoute un nouvel enfant de type etypefils et retourne son indice xml. Retourne -1 en cas d'échec.
	 * @param etypefils Type de l'élément à ajouter
	 * @param libelle Libellé de l'élément (n'est pas disponible pour tout les types d'éléments)
	 */
	int appendfilsbytype(Element::ELEMENT_TYPE etypefils,const std::wstring& libelle=L"");
	/**
	 * \~english Tag this element and its parents as modified and will be saved later.
	 * @param elementUpdated Updated element xml id
	 * \~french Affecte lui et ses parent comme modifié, et il sera sauvegardé à la prochaine sauvegarde.
	 * @param elementUpdated Indice xml de l'élément modifié
	 */
	void modified(int elementUpdated);
	/**
	 * \~english Hide this element. It can't be seen by the user.
	 * @param visible New state of visibility
	 * \~french Cache cet élément de sorte qu'il ne puisse être visible pour l'utilisateur.
	 * @param visible Nouvel état de l'élément
	 */
	void hide(bool visible=false);
	/**
	 * \~english Delete immediate child element corresponding to this id.
	 * @param xmlIdElement Element id
	 * @param setModification If true, call element::modified automatically
	 * \~french Supprime l'enfant immédiat ayant cet indice.
	 * @param xmlIdElement Indice de l'élément fils
	 * @param setModification Cette méthode appelera automatiquement element::modified.
	 */
	bool deleteelementbyxmlid(int xmlIdElement,bool setModification=true);
	/**
	 * \~english Delete immediate childs elements corresponding to this element type
	 * @param typeElement Element type
	 * \~french Supprime les enfants immédiat ayant ce type d'élément.
	 * @param typeElement Type d'élément
	 */
	void deleteallelementbytype(Element::ELEMENT_TYPE typeElement);
	/**
	 * \~english Delete recusively childs elements corresponding to this element type
	 * @param typeElement Element type
	 * \~french Supprime les enfants ayant ce type d'élément de facon récursive.
	 * @param typeElement Type d'élément
	 */
	void deleteallelementbytyper(Element::ELEMENT_TYPE typeElementToDelete);



	/**
	 * \~english Link a python function to the event of element update.You can add multiple links. Links are alive until project close.
	 * @param pymethod Python method with one parameter, the updated element index, may be this element or a child.
	 * \~french Lie une fonction python à l'événement de mise à jour d'un élément. Vous pouvez ajouter plusieurs liens, ces liens restent en place jusqu'à la fermeture du projet.
	 * @param pymethod Méthode python avec un paramètre correspondant à l'indice de l'élément mis à jour (l'élément courant ou un de ses fils)
	 */
	void register_update_manager(boost::python::object& pymethod);


	/**
	 * \~english Link a python function to the event of element append a new element child.You can add multiple links. Links are alive until project close.
	 * @param pymethod Python method with three parameters, the new element type, the parent element id, and the new element id.
	 * \~french Lie une fonction python à l'événement d'ajout d'un sous élément. Vous pouvez ajouter plusieurs liens, ces liens restent en place jusqu'à la fermeture du projet.
	 * @param pymethod Méthode python avec trois paramètre correspondant au type de d'élément ajouté, à l'indice de l'élément parent, et l'indice de l'élément fils
	 */
	void register_appendchild_manager(boost::python::object& pymethod);
	/**
	 * @return Element index
	 */
	wxInt32 getid() { return xmlId; }
protected:
	wxInt32 xmlId;
};
}
#endif
#endif