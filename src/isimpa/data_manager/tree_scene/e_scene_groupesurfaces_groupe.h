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

#include "data_manager/drawable_element.h"
#include "e_scene_groupesurfaces_groupe_vertex.h"
#include "data_manager/appconfig.h"
#include <map>

#ifndef __ELEMENT_GROUPVERTEX__
#define __ELEMENT_GROUPVERTEX__

/** \file e_scene_groupesurfaces_groupe.h
   \brief Cet élément contient la liste des triangles d'une partie du modèle
*/

/**
   \brief Cet élément contient la liste des triangles d'une partie du modèle
*/

class E_Scene_Groupesurfaces_Groupe: public E_Drawable
{
private:
	bool vertexInFile;
	bool vertexFileIsLoaded;
	std::list<t_faceIndex> vertexLst;
	typedef std::pair<t_faceIndex,wxTreeItemId> mapValue_t;
	typedef std::multimap<int,mapValue_t> faceIndexMapType;
	typedef std::pair<int,mapValue_t> mapPairInsert_t;
	faceIndexMapType faceIndexToElementTreeId;
	wxString faceFileName;
	bool isPointerGroup;
	bool ignoreModification;
	void InitGroupProp();
	void sendStringToVector();
	void DeleteDoublons();
	static bool mod_equal ( t_faceIndex elem1, t_faceIndex elem2 );

	static bool mod_diff ( t_faceIndex elem1, t_faceIndex elem2 );

	void InitProp();
	void PushFace(std::vector<std::vector<ApplicationConfiguration::t_PropFace> >& vectorToFeed,t_faceIndex faceIndex);
public:
	E_Scene_Groupesurfaces_Groupe( wxXmlNode* noeudCourant ,  Element* parent);

	E_Scene_Groupesurfaces_Groupe( Element* parent,wxString name="Group",bool isPointer=false);
	~E_Scene_Groupesurfaces_Groupe();
	void BeginDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree);

	/**
	 * Alimente un tableau de face avec les faces contenues dans ce groupe de surfaces
	 * @param[out] vectorToFeed Tableau à alimenter (il ne sera pas effacé avant l'opération)
	 */
	void GetFaces(std::vector<t_faceIndex>& vectorToFeed);
	void GetFaces(std::list<t_faceIndex>& vectorToFeed);
	wxXmlNode* SaveXMLDoc(wxXmlNode* NoeudParent);

	/**
	 * Cette méthode permet de générer un tableau correspondant à l'association matériau-faces du projet
	 * @param[out] matFacesAssociations Tableau[group][face]=idMaterial
	 **/
	void GetMaterialsLink( std::vector<std::vector<ApplicationConfiguration::t_PropFace> > &matFacesAssociations);

	void EndDrag(wxTreeEvent& treeEvent,wxTreeCtrl* tree,std::vector<Element*> dragLst);


	void OnRightClic(wxMenu* leMenu);

	/**
	 * Ajout d'une face dans le groupe
	 * @param face Indice de la face
	 * @param group Indice du groupe
	 */
	void AddFace(long face,long group,bool updateTreeCtrl=true);
	void AddFaces(const std::list<t_faceIndex>& faceList);
	void OnExpanding(wxTreeEvent& evt);
	void ClearGroup();
	bool OnElementRemoved();
	void OnCollapsing(wxTreeEvent& evt);

	bool IsEmpty();
	bool HasChildren( );
	/**
	 * Parcourt cet élément et les éléments fils à la recherche de la face
	 * @param[in] face numéro de face
	 * @param[in] group numéro de groupe (Indice du groupe du modèle 3D pas l'élément de groupe)
	 * @return Indice de l'élément face ou groupe correspondant aux paramètres
	 * @see E_Scene_Groupesurfaces::GetIdElementByFaceAndGroup()
	 */
	wxTreeItemId GetIdElementByFaceAndGroup(long face,long group);

	/**
	 * Transfert les éléments de triangles vers le tableau de face de cet élément (Gain de place dans le fichier XML, gain de rapidité pour l'interface en général)
	 */
	void HideFaces();
	/**
	 * Transfert du tableau de face de cet élément vers des éléments de triangles(Uniquement lors de la requete d'ouverture de l'utilisateur)
	 */
	void ShowFaces();
	void OnBeginLabelEdit(wxTreeEvent& treeEvent);

	/**
	 * Un groupe Pointeur est un groupe qui peut contenir des copies de faces, ce groupe peut être vidé sans conséquences pour le logiciel.
	 * On utilise ce type de groupe si l'on veut qu'un élément puisse contenir une liste de faces avec le comportement drag&drop de triangles déjà implémenté.
	 * @return Vrai si ce groupe contient des copies de faces.
	 */
	bool IsPointer();


	void OnEndLabelEdit(wxTreeEvent& treeEvent);
	/**
	 * Cette surcharge permet de supprimer les élément de faces et l'élément de groupe lui-même
	 */
	void DeleteAllElementByTypeR(ELEMENT_TYPE typeElementToDelete);

	void Modified(Element* eModif);

	void DrawTriangles(vec4 unitizeValue,std::vector<t_el_draw_object> &drawObject);
	virtual bool IsContainsProperties();
};

#endif
