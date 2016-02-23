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

/**
 * @file simpleGraph.h
 * @brief Classe de rendu 2D de données graphiques
 *
 * Basé sur wxWidgets ce contrôle permet l'affichage de données graphiques.
 */
#include <wx/control.h>
#include <wx/bitmap.h>
#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/dcbuffer.h>


#include "simpleGraphArrays.h"

#ifndef __SIMPLEGRAPH__
#define __SIMPLEGRAPH__

namespace sgSpace
{

	wxString EscapeDelimiters(const wxString& s);
	wxString RetrieveDelimiters(const wxString& s);
	void SplitString(const wxString& src,const wxString& delimiter,wxArrayString& dst);
	/**
	 * Cette méthode permet de calculer le meilleur interval possible pour les libellés d'un axe
	 */
	wxFloat32 ComputeBestTickInterval(wxFloat32 vmin, wxFloat32 vmax);
	//Pré-déclaration afin de pouvoir lier les objets à ce controle

	class simpleGraph;

	/**
	 * @brief Paramètres du contrôle général de type entier
	 */
	enum I_PARAM
	{
		I_PARAM_BORDER_TITLE,		/*!< Bordure en pixel du titre du graphique */
		I_PARAM_VIEW_WIDTH,			/*!< Largeur en pixel de la zone de rendu des courbes */
		I_PARAM_VIEW_HEIGHT,		/*!< Hauteur en pixel de la zone de rendu des courbes */
		I_PARAM_TIC_SIZE,			/*!< Largeur ou Hauteur en pixel du tiret dans les axes X et Y */
		I_PARAM_X_BORDER_AXIS,		/*!< Bordure en pixel entre le graphique et l'axe X*/
		I_PARAM_Y_BORDER_AXIS,		/*!< Bordure en pixel entre le graphique et l'axe Y*/
		I_PARAM_LEGEND_ICON_SIZE,	/*!< Taille d'un coté de l'icone de représentation de la légende */
		I_PARAM_COMPUTED_MARKER_SIZE, //Taille calculé des marqueurs dans la zone de dessin
		I_PARAM_LAST				//Dernier élément de l'énumeration, insérer un élément aprés générera une erreur de dépassement de tableau
	};

	/**
	 * @brief Paramètres du contrôle général de type booléen
	 */
	enum B_PARAM
	{
		B_PARAM_TITLE_IS_VISIBLE,		/*!< Affichage ou non du titre du graphique */
		B_PARAM_USE_ANTIALIASING,		/*!< Affichage des graphiques avec anti-crénelage */
		B_PARAM_RIGHT_CLICK_ZOOM_FIT,	/*!< Le bouton droit de la souris appel la procedure de calcul de zoom optimal */
		B_PARAM_HIDE_LEGEND,			/*!< A vrai la légende est cachée */
		B_PARAM_LAST				//Dernier élément de l'énumeration, insérer un élément aprés générera une erreur de dépassement de tableau
	};

	/**
	 * @brief Paramètres du contrôle général de type wxFloat32
	 */
	enum F_PARAM
	{
		F_PARAM_X_MIN,		/*!< Valeur X minimal */
		F_PARAM_X_MAX,		/*!< Valeur X maximal */
		F_PARAM_Y_MIN,		/*!< Valeur Y minimal */
		F_PARAM_Y_MAX,		/*!< Valeur Y maximal */
		F_PARAM_X_TIC,		/*!< Espace entre chaque libellé sur l'axe 0=Automatique*/
		F_PARAM_Y_TIC,		/*!< Espace entre chaque libellé sur l'axe 0=Automatique*/
		F_PARAM_MARKER_SIZE,/*!< Taille en % des marqueurs   */
		F_PARAM_LAST		//Dernier élément de l'énumeration, insérer un élément aprés générera une erreur de dépassement de tableau
	};

	/**
	 * @brief Enumeration des zones de dessins
	 * Chaque tracé est associé à une zone de dessin
	 */
	enum AREA_TYPE
	{
		AREA_TYPE_MAINGRAPH,	/*!< Zone de dessin principale */
		AREA_TYPE_X_AXIS,		/*!< Zone de dessin de l'axe des X */
		AREA_TYPE_Y_AXIS		/*!< Zone de dessin de l'axe des Y */
	};

	/**
	 * @brief Paramètres de l'élément d'un objet de dessin
	 */
	enum DRAWING_METHOD
	{
		DRAWING_METHOD_LINE,				/*!< Dessin en courbe (ensemble de lignes) */
		DRAWING_METHOD_COLS,				/*!< Dessin en colonnes (barres horizontales) */
		DRAWING_METHOD_STIPPLE,				/*!< Dessin en nuages de Bitmaps ( défini par le wxPen ) */
		DRAWING_METHOD_LINES_AND_STIPPLE,	/*!< Dessin en nuages de Bitmaps ( défini par le wxPen ) et lié par des lignes */
		DRAWING_METHOD_DOT_CLOUD,			/*!< Dessin en nuages de Points */
		DRAWING_METHOD_ECHOGRAM,			/*!< Dessin en barres verticales */
		DRAWING_METHOD_SPLINE				/*!< Dessin en courbe lissé */
	};

	/**
	 * @brief Paramètres de rendu d'un marqueur de valeur @see SG_Element_Data
	 *
	 * Valable uniquement pour :
	 * - DRAWING_METHOD_LINE
	 * - DRAWING_METHOD_DOT_CLOUD
	 */
	enum DRAWING_MARKER
	{
		DRAWING_MARKER_NONE,		/*!< Pas de Marqueur */
		DRAWING_MARKER_DOT,			/*!< Marqueur en forme de rond */
		DRAWING_MARKER_SQUARE,	/*!< Marqueur en forme de carré */
		DRAWING_MARKER_CROSS_D,	/*!< Marqueur en forme de croix diagonale */
	};


	/**
	 * @brief Paramètres d'insertion de l'élément de légende
	 */
	enum LEGEND_PLACEMENT
	{
		LEGEND_PLACEMENT_HORIZONTAL,		/*!< Placé dans le contrôle wxBoxSizer(Horizontal) principal */
		LEGEND_PLACEMENT_BOTTOM_LEFT,		/*!< Placé dans la cellule (2 ème ligne,1ère colonne) dans le contrôle wxFlexBoxSizer  */
		LEGEND_PLACEMENT_VERTICAL,			/*!< Placé dans le contrôle wxBoxSizer(Vertical) secondaire */
	};

	/**
	 * @brief Classe de définition des paramètres du contrôle général
	 *
	 */
	class SG_Params
	{
	public:
		SG_Params(simpleGraph* parent);
		/**
		 * Affecter une valeur à une propriété
		 * @param indice Indice de la propriété
		 * @param value Nouvelle valeur de la propriété
		 */
		void SetCfg( I_PARAM indice, wxInt32 value);
		/**
		 * Affecter une valeur à une propriété
		 * @param indice Indice de la propriété
		 * @param value Nouvelle valeur de la propriété
		 */
		void SetCfg( F_PARAM indice, wxFloat32 value);
		/**
		 * Obtenir la valeur d'une propriété
		 * @param indice Indice de la propriété
		 * @return value Valeur de la propriété
		 */
		const wxInt32& GetCfg( I_PARAM indice );
		/**
		 * Obtenir la valeur d'une propriété
		 * @param indice Indice de la propriété
		 * @return value Valeur de la propriété
		 */
		const wxFloat32& GetCfg( F_PARAM indice );
		/**
		 * Affecter une valeur à une propriété
		 * @param indice Indice de la propriété
		 */
		void EnableCfg( B_PARAM indice );
		/**
		 * Affecter une valeur à une propriété
		 * @param indice Indice de la propriété
		 */
		void DisableCfg( B_PARAM indice );
		/**
		 * Obtenir une valeur à une propriété
		 * @param indice Indice de la propriété
		 * @return value Valeur de la propriété
		 */
		bool isEnable( B_PARAM indice );

		/**
		 * Initialise les paramètres
		 */
		void Init();
	private:
		wxInt32 integerParamTab[I_PARAM_LAST];
		bool booleanParamTab[B_PARAM_LAST];
		wxFloat32 floatParamTab[F_PARAM_LAST];
		simpleGraph* m_parent;
	};





	/**
	 * @brief Classe de définition des paramètres d'un tracé
	 *
	 */
	class SG_El_Params
	{
	public:
		SG_El_Params();
		/**
		 * Retourne l'élément servant à dessiner la courbe
		 * @return L'objet de dessin
		 */
		const wxPen& GetPen();
		/**
		 * Retourne l'élément servant à dessiner le contenu de la courbe
		 * @return L'objet de dessin
		 */
		const wxBrush& GetBrush();

		/**
		 * Convertie le contenu de cette classe en une chaine de caractères
		 */
		wxString Serialize();
		/**
		 * Affecte le contenu de cette classe en fonction d'une chaine de caractères.
		 * @see Serialize()
		 */
		void FromSerialization(const wxString& data);


		/**
		 * Retourne la police de texte
		 * @return L'objet de dessin
		 */
		const wxFont& GetFont();
		/**
		 * Retourne la police de texte
		 * @return L'objet de dessin
		 */
		void SetFont(const wxFont& newFont);

		/**
		 * Retourne Le type de représentation graphique des données
		 */
		DRAWING_METHOD GetDrawingMethod();

		/**
		 * Affecte l'outil de dessin
		 * @param newPen Outil de dessin a affecter.
		 */
		void SetPen(const wxPen* newPen);
		/**
		 * Affecte l'outil de dessin
		 * @param newPen Outil de dessin a affecter.
		 */
		void SetBrush(const wxBrush* newBrush);
		/**
		 * Affecte le type de représentation graphique des données
		 */
		void SetDrawingMethod(DRAWING_METHOD _drawingMethod);
		/**
		 * Affecte le type de représentation graphique des données
		 */
		void SetDrawingMarker(DRAWING_MARKER _drawingMarker);
		/**
		 * Retourne le marqueur de valeur graphique
		 */
		DRAWING_MARKER GetDrawingMarker();

		/**
		 * Effectue un tirage aléatoire de tout les paramètres de rendu
		 */
		void Randomize();
		/**
		 * Effectue un tirage aléatoire du type de marqueur
		 */
		void RandomizeMarker();

		/**
		 * Applique une rotation au texte
		 * @param textRotation Rotation en degrées
		 */
		void SetTextRotation(const wxFloat32& textRotation);

		/**
		 * Applique une rotation au texte
		 * @return Vrai si le texte est orienté autrement que de gauche à droite.
		 */
		bool IsTextRotated();

		/**
		 * Applique une rotation au texte
		 * @return Vrai si le texte est orienté autrement que de gauche à droite.
		 */
		wxFloat32 GetTextRotation();


		/**
		 * Retourne la couleur du texte
		 */
		const wxColour& GetTextForeGroundColor();
		/**
		 * Retourne la couleur de fond du texte
		 */
		const wxColour& GetTextBackGroundColor();
		/**
		 * Appliquer une couleur au texte
		 */
		void SetTextForeGroundColor(const wxColour& newColour);
		/**
		 * Appliquer une couleur de fond au texte
		 */
		void SetTextBackGroundColor(const wxColour& newColour);

	protected:
		/**
		 * Charge un champ de configuration
		 * @param name Nom du paramètre
		 * @param value Valeur du paramètre
		 * @see FromSerialization()
		 */
		virtual void LoadConfig(const wxString& name, const wxString& value);
	private:
		wxPen drawPen;
		wxBrush drawFill;
		wxFont textFont;
		wxColour textForegroundColor;
		wxColour textBackgroundColor;
		struct t_GenParam
		{
			DRAWING_METHOD drawingMethod;
			DRAWING_MARKER drawingMarker;
			bool isTextOriented;
			wxFloat32 textOrientation;
		} params;
	};


	/**
	 * @brief Cette classe regroupe les appels aux méthode de Drawing Context
	 */
	class SG_Renderer
	{
	public:

		SG_Renderer(wxDC& drawingContext,SG_Params& mainStyle,wxSize drawSize=wxSize(-1,-1));
		/**
		 * Débute la procédure de dessin avec le style passé en paramètre.
		 * @warning Un appel de 2 Begin à la suite sans appel de End entre provoquera l'utilisation du style fourni au premier Begin
		 */
		void Begin(SG_El_Params& el_style);

		/**
		 * Dessine l'élément courant avec le style indiqué dans le dernier appel de Begin()
		 */
		void Draw(const ArrayOfFloats& Yvalues,const ArrayOfFloats& Xvalues=ArrayOfFloats());

		/**
		 * Selon les coordonnées du contexte graphique dessine une ligne
		 */
		void DrawLine(wxPoint from,wxPoint to);
		/**
		 * Selon les coordonnées graphique standart (Logicial) dessine une ligne
		 */
		void DrawLine(wxRealPoint from,wxRealPoint to);
		/**
		 * Retourne la largeur et hauteur de la zone de dessin
		 */
		wxSize GetDeviceSize();
		/**
		 * A partir des coordonnées x,y retourne le pixel correspondant
		 */
		wxPoint LogicalToDeviceCoord( wxFloat32 xx, wxFloat32 yy);

		/**
		 * A partir du pixel x,y retourne les coordonnées x,y
		 */
		wxRealPoint DeviceToLogicalCoord( wxInt32 xx, wxInt32 yy);
		/**
		 * Retourne les paramètres globaux des tracés
		 */
		SG_Params* GetMainStyle();

		/**
		 * Cette méthode permet de connaitre la taille en pixel d'une chaine de caractère
		 * @param textString Chaine de caractère
		 * @return Hauteur, largeur en pixel
		 */
		wxSize GetTextDeviceSize(const wxString& textString);

		/**
		 * Dessine une chaine de caractères aux coordonnées indiquées, en utilisant la police de caractère de l'élément de dessin en cours.
		 * @see SG_Renderer::GetTextDeviceSize()
		 * @param topLeftPosition Position du coin supérieur gauche de dessin du texte @see SG_Renderer::LogicalToDeviceCoord()
		 * @param text Chaine de caractère
		 */
		void DrawText(wxPoint topLeftPosition,const wxString& text);

		/**
		 * Dessine un rectangle avec le style en cours d'utilisation (coordonnées device)
		 */
		void DrawRectangle(const wxRect& rectArea);
		/**
		 * A destination de la légende, dessine un exemple de tracé
		 */
		void DrawSample(const wxRect& drawZone);
		/**
		 * Clos la procédure de dessin. Le prochain appel de Begin sera effectif.
		 */
		void End();
	private:
		/**
		 * @return Une unité Logical équivaut à cette valeur en pixel(Device)
		 */
		wxFloat32 GetXOffset();
		wxFloat32 GetYOffset();
		void DrawLines(const ArrayOfFloats& Yvalues,const ArrayOfFloats& Xvalues);
		void DrawStipples(const ArrayOfFloats& Yvalues,const ArrayOfFloats& Xvalues,bool linkDots);
		void DrawDotsCloud(const ArrayOfFloats& Yvalues,const ArrayOfFloats& Xvalues);
		void DrawHistogram(const ArrayOfFloats& Yvalues,const ArrayOfFloats& Xvalues);
		void DrawEchogram(const ArrayOfFloats& Yvalues,const ArrayOfFloats& Xvalues);
		void DrawMarkers( wxInt32 n, wxPoint points[],DRAWING_MARKER eleMarker);
		void DrawCrossDMarker( wxInt32 n, wxPoint points[], const wxSize& markerSize);
		void DrawRectMarker( wxInt32 n, wxPoint points[], const wxSize& markerSize);
		void DrawDotMarker( wxInt32 n, wxPoint points[], const wxSize& markerSize);
		wxDC* currentContext;
		SG_Params* currentMainStyle;
		SG_El_Params* currentElementStyle;
		wxFloat32 histoGramWidth; //Largeur de chaque barre pour eviter que la superposition d'un
	};


	/**
	 * @brief Tracé de graphique
	 * Cette classe abstraite représente un tracé de graphique
	 */
	class SG_Element : public wxObject
	{
	public:
		SG_El_Params el_style;
		AREA_TYPE drawingArea;

		SG_Element(simpleGraph* _parent, const wxString& elLabel,AREA_TYPE _drawingArea);
		virtual ~SG_Element(){};

		/**
		 * Demande au tracé d'utiliser un objet de rendu pour afficher ses valeurs
		 */
		virtual void Draw(wxWindow* paintArea,SG_Renderer& renderer)=0;

		/**
		 * Demande au tracé de dessiner l'apercu de celui-ci dans une zone (pour la légende)
		 */
		virtual void DrawSample(SG_Renderer& renderer,const wxRect& drawZone);

		/**
		 * Retourne les bornes par défaut du tracé
		 * @param[out] xMin Valeur x minimal
		 * @param[out] xMax Valeur x maximal
		 * @param[out] yMin Valeur y minimal
		 * @param[out] yMax Valeur y maximal
		 */
		virtual void GetMinMax(wxFloat32& xMin,wxFloat32& xMax,wxFloat32& yMin,wxFloat32& yMax)=0;

		/**
		 * Retourne l'état du graphique
		 * @return Vrai si l'élément doit être déssiné
		 */
		bool IsVisible();

		/**
		 * Affecte l'état du graphique
		 * @param _visible Vrai si l'élément doit être déssiné
		 */
		void Hide(bool _visible=false);
		/**
		 * Retourne la taille en pixel qui permet à l'objet d'être dessiné sans superposition des libellés
		 */
		virtual wxSize GetBestSize() const;

		/**
		 * Retourne le libellé du tracé
		 */
		virtual wxString GetLabel() const;
		/**
		 * Affectation d'un nouveau libellé du tracé
		 */
		virtual void SetLabel(const wxString& newLabel);
	protected:
		bool visible;
		wxInt32 legendIndex;
		simpleGraph* parent;
		wxString elementLabel;
	};


	/**
	 * @brief Tracé composé d'une liste de valeur X,Y
	 */
	class SG_Element_Data : public SG_Element
	{
	public:
		/**
		 * Constructeur
		 * @param _parent Contrôle où ce tracé sera logé.
		 * @param sizeHint Pour un gain de performance il est possible de pré-allouer un espace mémoire pour la liste de données.
		 */
		SG_Element_Data(simpleGraph* _parent, const wxString& elLabel,const wxInt32& sizeHint=50);
		/**
		 * Retourne le nombre de valeurs du tracé
		 * @return Nombre d'élément composant le tracé
		 */
		wxInt32 GetSize();

		virtual void Draw(wxWindow* paintArea,SG_Renderer& renderer);
		/**
		 * Alimente le tracé avec une valeur
		 * @param xx Valeur x
		 * @param yy Valeur y
		 */
		void PushValue(const wxFloat32& xx,const wxFloat32& yy);

		/**
		 * Alimente le tracé avec une valeur
		 * @param xx Valeur x en chaine de caractère (sera convertie en nombre)
		 * @param yy Valeur y
		 * @return Conversion de xx
		 */
		wxFloat32 PushStringValue(const wxString& xx,const wxFloat32& yy);
		virtual void GetMinMax(wxFloat32& xMin,wxFloat32& xMax,wxFloat32& yMin,wxFloat32& yMax);
	protected:
		ArrayOfFloats valuesY;
		ArrayOfFloats valuesX;
	};
	/**
	 * @brief Tracé composé d'une liste de valeur sans coordonnées X
	 * Les valeurs sont placé pour X=0 à X=GetSize()
	 */
	class SG_Element_List : public SG_Element_Data
	{
	public:
		/**
		 * Constructeur
		 * @param _parent Contrôle où ce tracé sera logé.
		 * @param sizeHint Pour un gain de performance il est possible de pré-allouer un espace mémoire pour la liste de données.
		 */
		SG_Element_List(simpleGraph* _parent, const wxString& elLabel,const wxInt32& sizeHint=50);

		/**
		 * Alimente le tracé avec une valeur Y (la valeur X est incrémenté de 1 à partir de 0)
		 * @param ElValue Valeur
		 */
		void PushValue(const wxFloat32& yValue);
		virtual void GetMinMax(wxFloat32& xMin,wxFloat32& xMax,wxFloat32& yMin,wxFloat32& yMax);
	private:
		wxInt32 lastXVal;
	};
	/**
	 * @brief Axe standart x ou y
	 */
	class SG_Element_Axis : public SG_Element
	{
	public:
		SG_Element_Axis(simpleGraph* _parent,wxOrientation _axisOrientation,wxFloat32 intervalValue=0.f,wxString unitLabel=wxString(""));
		virtual void Draw(wxWindow* paintArea,SG_Renderer& renderer);
		virtual void GetMinMax(wxFloat32& xMin,wxFloat32& xMax,wxFloat32& yMin,wxFloat32& yMax);
		virtual wxSize GetBestSize() const;
		virtual wxString GetLabel() const;
		virtual void SetLabel(const wxString& newLabel);
	protected:
		void DrawAxeWithTics(SG_Renderer& renderer);
		virtual void AddTic(SG_Renderer& renderer,const wxPoint& from,const wxPoint& to,const wxFloat32& realValue);
		/**
		 * A partir de la valeur réél retourne le texte de l'axe équivalent
		 * Si le texte est trop long pour être affiché et que drawTic est à faux alors le tiret ne sera pas affiché, il sera affiché quand même si drawTic à vrai
		 */
		virtual wxString GetTextValue(const wxFloat32& realValue, bool& drawTic);
		wxOrientation axisOrientation;
		wxFloat32 intervalLabel;
		wxInt32 topRightLabelPixelValue; // Valeur pixel (x ou y) du dernier libellé déssiné, l'objectif est de ne pas superposer les libellés
		wxInt32 maxSizeElement;			 // Valeur en pixel minimal (hauteur ou largeur) afin qu'il n'y ai pas de superposition de libellés
		wxSize sizeofTitle;				  // Valeur en pixel de la taille du libellé du titre
	};


	/**
	 * @brief Axe composé de libellés
	 * Utile pour les histogrammes sur l'axe X
	 */
	class SG_Element_Axis_Labeled : public SG_Element_Axis
	{
	public:
		SG_Element_Axis_Labeled(simpleGraph* _parent,wxOrientation _axisOrientation,wxFloat32 intervalValue=1.f,wxString unitLabel=wxString(""));

		/**
		 * Ajoute un composant de l'axe
		 * @param offset En fonction de l'orientation, désigne le nombre d'unité entre le dernier libellé et ce libellé (x ou y)
		 * @param label Libellé pour cette position
		 */
		void PushLabel(wxString label);
	protected:
		virtual wxString GetTextValue(const wxFloat32& realValue, bool& drawTic);
		wxArrayString labels;
	};
	/**
	 * Zone réservée au dessin des tracés et légendes
	 */
	class SG_Area: public wxControl
	{
	public:

		SG_Area( simpleGraph *parent , AREA_TYPE _areaType=AREA_TYPE_MAINGRAPH, wxSize windowSize=wxDefaultSize);
		~SG_Area();
		void OnPaint( wxPaintEvent &event );
		void OnResize( wxSizeEvent &event );
		virtual void ScrollWindow( int dx, int dy, const wxRect *rect );
		virtual wxSize DoGetBestSize() const;
		void OnMouse( wxMouseEvent &event );


		wxRect lastZoomHintRect;
		SG_El_Params zoomAreaHintStyle;

		void OnBeginSelection(wxPoint mousePos);
		void OnDragging(wxPoint mousePos);
		void OnEndSelection(wxPoint mousePos);
	private:
		void DrawZoomHintRectangle( wxPoint newPos, bool drawNewRect=true);
		void Draw(wxDC& directDc,const wxSize& drawSize);
		void DrawAntiAliased(wxDC& directDc);

		void OnErase(wxEraseEvent& evt);
		AREA_TYPE areaType;
		simpleGraph     *m_owner;
		DECLARE_EVENT_TABLE()
		wxBufferedDC* backingStoreDc;
		wxSize backingStoreSize;
	};




	/**
	 * @brief Gestion des légendes
	 * Cette classe gère les légendes
	 */
	class SG_Legend : public wxControl
	{
	public:
		SG_Legend(simpleGraph* parent,LEGEND_PLACEMENT whereToInsert, wxWindowID id=-1, const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = 0);
        /** Attach Legend with graph before other components */
        static void Prepend(SG_Legend* legend, const wxSizerFlags& sizerParams);
        /** Attach Legend with graph after other components */
        static void Add(SG_Legend* legend, const wxSizerFlags& sizerParams);
		void OnPaint( wxPaintEvent &event );
		virtual wxSize DoGetBestSize() const;
		void OnMouse( wxMouseEvent &event );
        /** @return Legend's sizer */
        LEGEND_PLACEMENT GetLegendPlacement() const;
	protected:
		virtual void drawLegend(wxDC& drawDC);
		void OnErase(wxEraseEvent& evt);
		simpleGraph* m_owner;
		DECLARE_EVENT_TABLE()
		LEGEND_PLACEMENT whereToInsert;
	};



	/**
	 * @brief Classe de rendu 2D de données graphiques
	 *
	 * Basé sur wxWidgets ce contrôle permet l'affichage de données graphiques.
	 */
	class simpleGraph : public wxScrolledWindow
	{
	public:
		SG_Params parameters;
		simpleGraph(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxPanelNameStr);
		virtual ~simpleGraph();

		/**
		 * Supprime un élément de tracé
		 * @param indexEl Indice de l'élément à supprimer.
		 */
		void DeleteDrawingElement( wxInt32 indexEl );

		/**
		 * Retourne un élément de dessin
		 * @warning Ne pas stocker cet élément, il peut être supprimé à tout moment. Peut retourner NULL si l'élément n'existe plus.
		 */
		SG_Element* GetDrawingElement( wxInt32 indexEl );

		/**
		 * Retourne un itérateur pointant vers le premier élément de dessin.
		 */
		ArrayOfGraphEl::iterator Begin();
		/**
		 * Retourne un itérateur pointant vers le dernier élément de dessin.
		 */
		ArrayOfGraphEl::iterator End();

		void OnZoomIn( const wxRealPoint& beginZone,const wxRealPoint& endZone);
		void OnZoomOut( );

		void OnPaint( wxPaintEvent &event );

		/**
		 * Régle les valeurs de bornes x et y afin de pouvoir visualiser tout les tracés dans leurs ensembles
		 */
		void ZoomFit();
	protected:
		friend class SG_Area;	/*!< Permet à la zone de dessin des tracés l'accès aux objets de tracés */
		friend class SG_Legend;
		friend class SG_Element;
		friend class SG_Params;

		/**
		 * Ajoute un tracé au graphique. Cet objet sera supprimé par le destructeur.
		 * Cette méthode est appelée par le contructeur de SG_Element. Vous n'avez donc pas à l'utiliser
		 * @param drawingEl Pointeur vers le tracé à ajouter
		 * @return Indice de l'élément de tracé.
		 */
		wxInt32 AddDrawingElement( SG_Element* drawingEl );

		wxSizer* GetSizer( LEGEND_PLACEMENT whereToInsert );


		ArrayOfGraphEl elements;

		virtual void OnUpdate(I_PARAM indexParam);
		virtual void OnUpdate(F_PARAM indexParam);
		virtual void OnUpdate(B_PARAM indexParam);

		void OnMouseGeneral( wxMouseEvent &event );
	private:
		wxInt32 elCounter;
		wxBoxSizer* m_plotAndTitleSizer;
		wxBoxSizer* mainsizer;
		wxBoxSizer* cellSizer;
		SG_Area* mainGraphArea;
		wxFlexGridSizer* plotsizer;
		bool constructionFinish;
		DECLARE_EVENT_TABLE()
	};


}

#endif
