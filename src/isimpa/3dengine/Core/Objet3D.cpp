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

#include "GL/opengl_inc.h"
#include "wx/string.h"
#include "wx/log.h"
#include <wx/filename.h>
#include "Objet3D.h"
#include "UtfConverter.h"

#include "3ds.h"	// gestion du format 3DS
#include "mesh.h"	// gestion du format MESH
#include "Asc.h"	// gestion du format ASC
#include "Nff.h"	// gestion du format NFF
#include "bin.h"	// gestion du format BIN propriétaire spps
#include "stl.hpp"  // gestion du format STL

//#include "ply.h"				// gestion du format PLY stanford
#include <input_output/ply/rply_interface.hpp>	// gestion du format PLY stanford (Librairie externe)
#include "../preprocess/input_output/poly.h"	// gestion du format POLY tetgen

#include "data_manager/tree_scene/e_scene_encombrements_encombrement_cuboide.h"
#include "data_manager/appconfig.h"
#include "data_manager/drawable_element.h"
#include <tools/collision.h>

WX_DECLARE_HASH_MAP( std::size_t, bool, wxIntegerHash, wxIntegerEqual, BoolHashMap );


//#include "data_manager/e_position.h"
#include "last_cpp_include.hpp"
CTexture *textures;
CMaterial *materials;



#ifdef _DEBUG
	void DrawBox(const vec3& center,const float& halfsize)
	{
		vec3 HA = center + (vec3(-1, -1, 1) * halfsize);
		vec3 HB = center + (vec3(1, -1, 1) * halfsize);
		vec3 HC = center + (vec3(1, 1, 1) * halfsize);
		vec3 HD = center + (vec3(-1, 1, 1) * halfsize);
		vec3 BA = center + (vec3(-1, -1, -1) * halfsize);
		vec3 BB = center + (vec3(1, -1, -1) * halfsize);
		vec3 BC = center + (vec3(1, 1, -1) * halfsize);
		vec3 BD = center + (vec3(-1, 1, -1) * halfsize);


		glColor3f(1,1,1);
		glBegin(GL_LINE_STRIP);
			glVertex3fv(HA);
			glVertex3fv(HB);
			glVertex3fv(HC);
			glVertex3fv(HD);
			glVertex3fv(HA);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3fv(BA);
			glVertex3fv(BB);
			glVertex3fv(BC);
			glVertex3fv(BD);
			glVertex3fv(BA);
		glEnd();
		glBegin(GL_LINES);
			glVertex3fv(BA);
			glVertex3fv(HA);
			glVertex3fv(BD);
			glVertex3fv(HD);
			glVertex3fv(BC);
			glVertex3fv(HC);
			glVertex3fv(BB);
			glVertex3fv(HB);
		glEnd();

	}
#endif

static GLfloat Max(GLfloat a, GLfloat b)
{
  if (a > b)
    return a;
  return b;
}

static GLfloat Abs(GLfloat f)
{
  if (f < 0)
    return -f;
  return f;
}
t_faceIndex GetSideFace(t_faceIndex currentFace,ivec2 side, const std::vector<std::vector<t_faceIndex> >& dotToFace)
{
	for(int idFaceA=0;idFaceA<dotToFace[side.a].size();idFaceA++)
	{

		for(int idFaceB=0;idFaceB<dotToFace[side.b].size();idFaceB++)
		{
			if(dotToFace[side.a][idFaceA]==dotToFace[side.b][idFaceB] && dotToFace[side.a][idFaceA]!=currentFace)
			{
				//Trouvé face ayant les deux cotés en commun
				return dotToFace[side.a][idFaceA];
			}
		}
	}
	return  t_faceIndex(-1,-1);
}

void SetDiffSideValue( SFace3D* CurrentFace, SFace3D* Neighboor, int sideIndex )
{
		if(CurrentFace->FaceNormals.barelyEqual(Neighboor->FaceNormals))
		{
			CurrentFace->Diff[sideIndex]=false;
		}else{
			CurrentFace->Diff[sideIndex]=true;
		}
}
CObjet3D::CObjet3D()
{
	this->_nbMaterials = 0;
	this->_hasTexture = false;
	tabVertexMaillage = NULL;
	tabVertexMaillageSize=0;
	nodesMaillage=NULL;
	nodesMaillageSize=0;

	textures = new CTexture();
	materials = new CMaterial();
	this->UnselectVertex();
	fontInitialized=false;
	selectedDot.set(0,0,0); //point d'origine
	base=0;
	IntersectionTool=NULL;
}

bool CObjet3D::IsFontNeedBuilding()
{
	return !fontInitialized;
}

void CObjet3D::BuildFont(void* vhDC)								// Build Our Bitmap Font
{
	base = glGenLists(223);								// Storage For 96 Characters
	#ifdef _UNIX
    Display *dpy;
    XFontStruct *fontInfo;  // storage for our font.

    // load the font.  what fonts any of you have is going
    // to be system dependent, but on my system they are
    // in /usr/X11R6/lib/X11/fonts/*, with fonts.alias and
    // fonts.dir explaining what fonts the .pcf.gz files
    // are.  in any case, one of these 2 fonts should be
    // on your system...or you won't see any text.

    // get the current display.  This opens a second
    // connection to the display in the DISPLAY environment
    // value, and will be around only long enough to load
    // the font.
    dpy = XOpenDisplay(NULL); // default to DISPLAY env.

    fontInfo = XLoadQueryFont(dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1");
    if (fontInfo == NULL) {
	fontInfo = XLoadQueryFont(dpy, "fixed");
	if (fontInfo == NULL) {
	    printf("no X font available?\n");
	}
    }

    // after loading this font info, this would probably be the time
    // to rotate, scale, or otherwise twink your fonts.

    // start at character 32 (space), get 223 characters (a few characters past z), and
    // store them starting at base.
    glXUseXFont(fontInfo->fid, 32, 223, base);

    // free that font's info now that we've got the
    // display lists.
    XFreeFont(dpy, fontInfo);

    // close down the 2nd display connection.
    XCloseDisplay(dpy);
	#endif
	#ifdef _WINDOWS
	HDC hDC=(HDC)vhDC;
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping



	font = CreateFont(	-12,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						wxString("Courier New"));					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 223, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
	#endif
	fontInitialized=true;
}

void CObjet3D::KillFont()						// Delete The Font
{
  glDeleteLists(base, 223);								// Delete All 223 Characters
}

void CObjet3D::glPrint(vec3 pos,vec3 color,const char *fmt, ...)			// Custom GL "Print" Routine
{

	glDisable(GL_TEXTURE_2D);
	bool enablelight(false),enableblend(false);
	if(glIsEnabled(GL_LIGHTING))
		enablelight=true;
	if(glIsEnabled(GL_BLEND))
		enableblend=true;
	glColor3fv(color);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glRasterPos3fv(pos);

	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32


	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits


	if(!enableblend)
		glDisable(GL_BLEND);
	if(enablelight)
		glEnable(GL_LIGHTING);
	glLineWidth(.5);

}

inline void CObjet3D::GetPathFromFilename( const char* string, char* result)
{ //extrait le dossier du nom de fichier
    char *path;

    strcpy(result, string);
    path=strrchr(result, wxFileName::GetPathSeparator());
    if (path==NULL)
        return;
    else
        *(path+1)=0;
}

long CObjet3D::GetNumFaces(long g)
{ //retourne le nombre de face pour un groupe, cumul si g=-1
	if (this->_pGroups.size() == 0)
		return 0;
	if (g >= 0)
	{
		if (g < this->_pGroups.size())
		{
			return this->_pGroups[g].pFaces.size();
		}
		else
		{
			return this->_pGroups[this->_pGroups.size() - 1].pFaces.size();
		}
	}
	long n = 0;
	for (g = 0; g < this->_pGroups.size(); g++)
	{
		n += this->_pGroups[g].pFaces.size();
	}

	return n;
}


t_faceIndex CObjet3D::FindFaceGroupWithFaceIndex(long face)
{ //retrouve le groupe et le numéro de face du groupe en fonction du numéro de face
	long facecumul=0;
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		if(facecumul+this->_pGroups[g].pFaces.size()>face)
		{
			return t_faceIndex(face-facecumul,g);
		}else{
			facecumul+=this->_pGroups[g].pFaces.size();
		}
	}
	return t_faceIndex(0,0);
}


void CObjet3D::FillVectorWithRealVertices(std::vector<vec3>* vectorToFeed)
{
	vectorToFeed->reserve(this->_pVertices.size());
	for(int v=0; v < this->_pVertices.size(); v++)
	{
		vectorToFeed->push_back( coordsOperation::GlCoordsToCommonCoords(UnitizeVar,this->_pVertices[v]));
	}
}

void CObjet3D::LoadPolyWithoutLostCurrentModelGroupAndMaterials(const char *filename)
{
	//this->_Destroy();
	char m_filepathname[256];
	this->GetPathFromFilename( filename, m_filepathname);
	using namespace formatPOLY;

	CPoly LoadPoly;
	t_model modelpoly;
	if(!LoadPoly.ImportPOLY(modelpoly, filename)) return;
	//Transfert des vertices
	this->_pVertices.clear();
	for(int idvert=0;idvert<modelpoly.modelVertices.size();idvert++)
		this->_pVertices.push_back(coordsOperation::CommonCoordsToGlCoords(vec4(0,0,0,1), modelpoly.modelVertices[idvert]));

	//Création du tableau de correspondance
	std::vector<t_faceIndex> oldFaceCorrspondance(this->GetNumFaces());
	unsigned int counter=0;
	std::vector<SGroup3D> localGroups;

	for(int g=0; g < this->_pGroups.size(); g++)
	{
		localGroups.push_back(SGroup3D());
		localGroups[g].Name=this->_pGroups[g].Name;
		localGroups[g].Material=this->_pGroups[g].Material;
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			oldFaceCorrspondance[counter]=t_faceIndex(f,g);
			counter++;
		}
	}

	//Transfer des groupes

	unsigned int nbfaces=modelpoly.modelFaces.size();
	for(long base_f=0;base_f<nbfaces;base_f++)
	{
		if(modelpoly.modelFaces[base_f].faceIndex<oldFaceCorrspondance.size())
		{
			t_faceIndex corrFace=oldFaceCorrspondance[modelpoly.modelFaces[base_f].faceIndex];
			SFace3D nouvFace;

			nouvFace.Vertices=modelpoly.modelFaces[base_f].indicesSommets;

			nouvFace.Diff[0]=true;
			nouvFace.Diff[1]=true;
			nouvFace.Diff[2]=true;
			ivec3 sommetsTri=modelpoly.modelFaces[base_f].indicesSommets;
			nouvFace.FaceNormals=FaceNormal(this->_pVertices[sommetsTri.a],this->_pVertices[sommetsTri.b],this->_pVertices[sommetsTri.c]);
			nouvFace.idMaterial=this->_pGroups[corrFace.g].pFaces[corrFace.f].idMaterial;

			//Todo, corriger les coordonnées de textures
			nouvFace.TexCoords=this->_pGroups[corrFace.g].pFaces[corrFace.f].TexCoords;

			localGroups[corrFace.g].pFaces.push_back(nouvFace);
		}
	}
	this->_pGroups=localGroups;
	this->_pNormals=std::vector<vec3>(modelpoly.modelVertices.size());
	this->Unitize();
	delete IntersectionTool;
	IntersectionTool=new ui_tools::IntersectionSeeker(_pVertices,_pGroups);
}

bool CObjet3D::Load(const char *filename)
{ //chargement d'un fichier contenant un modèle 3D
	bool isLoadOk=false;
	this->_Destroy();
	wxString fullName(filename);
	wxString Extension=fullName.Mid(fullName.rfind(".")+1);
	Extension.MakeLower();
	//const char *Extension = &filename[strlen(filename) - 3];
	if(Extension=="3ds")
	{
		if(this->_Load3DS(filename))
			isLoadOk=true;
	}
	else if (Extension=="ply")
	{
		if(this->_LoadPLY(filename))
			isLoadOk=true;
	}
	else if (Extension=="poly")
	{
		if(this->_LoadPOLY(filename))
			isLoadOk=true;
	}
	else if (Extension=="bin")
	{
		if(this->_LoadBIN(filename))
			isLoadOk=true;
	}
	else if (Extension=="stl")
	{
		if(this->_LoadSTL(filename))
			isLoadOk=true;
	}
	if(isLoadOk)
	{
		delete IntersectionTool;
		IntersectionTool=new ui_tools::IntersectionSeeker(_pVertices,_pGroups);
	}
	return isLoadOk;
}
bool CObjet3D::BuildModel(vec3 debCuboide,vec3 finCuboide)
{
	if(debCuboide==finCuboide)
		return false;

	this->_Destroy();


	E_Scene_Encombrements_Encombrement_Cuboide constructeur(NULL);
	constructeur.BuildModel(UnitizeVar,debCuboide,finCuboide);
	std::vector<E_Drawable::t_triangle> allTriangles;
	constructeur.GetConsistentModel(allTriangles);
	SGroup3D nvGroupe;
	for(int i=0;i<allTriangles.size();i++)
	{
		int nextIndex=_pVertices.size();
		SFace3D nvFace;
		nvFace.Vertices.a=nextIndex;
		nvFace.Vertices.b=nextIndex+1;
		nvFace.Vertices.c=nextIndex+2;
		nvFace.FaceNormals=FaceNormal(allTriangles[i].a,allTriangles[i].b,allTriangles[i].c);
		nvGroupe.pFaces.push_back(nvFace);
		_pVertices.push_back(allTriangles[i].a);
		_pVertices.push_back(allTriangles[i].b);
		_pVertices.push_back(allTriangles[i].c);
		_pNormals.push_back(nvFace.FaceNormals);
		_pNormals.push_back(nvFace.FaceNormals);
		_pNormals.push_back(nvFace.FaceNormals);
	}
	_pGroups.push_back(nvGroupe);



	this->Unitize();

	delete IntersectionTool;
	IntersectionTool=new ui_tools::IntersectionSeeker(_pVertices,_pGroups);



	return true;
}
bool CObjet3D::Save(const char *filename)
{ //Sauvegarde d'un fichier, determination du type via l'extension dans le nom du fichier
	const char *Extension = &filename[strlen(filename) - 4];
	if ((!strcmp(Extension, "MESH"))||(!strcmp(Extension, "mesh")))
	{
		if(!this->_SaveMESH(filename))
			return false;
		else return true;
	}
	else if ((!strcmp(Extension, ".ASC"))||(!strcmp(Extension, ".asc")))
	{
		if(!this->_SaveASC(filename))
			return false;
		else return true;
	}
	else if ((!strcmp(Extension, ".NFF"))||(!strcmp(Extension, ".nff")))
	{
		if(!this->_SaveNFF(filename))
			return false;
		else return true;
	}
	else if ((!strcmp(Extension, "POLY"))||(!strcmp(Extension, "poly")))
	{
		if(!this->_SavePOLY(filename))
			return false;
		else return true;
	}
	else if ((!strcmp(Extension, ".PLY"))||(!strcmp(Extension, ".ply")))
	{
		if(wxString(filename).Contains(".mat."))
		{
			return this->_SavePLYProjectGroup(filename);
		}else{
			if(!this->_SavePLY(filename))
				return false;
			else
				return true;
		}
	}
	else if ((!strcmp(Extension, ".BIN"))||(!strcmp(Extension, ".bin")))
	{
		if(!this->_SaveBIN(filename))
			return false;
		else return true;


	}else if ((!strcmp(Extension, "CBIN"))||(!strcmp(Extension, "cbin")))
	{
		if(!this->_SaveCBIN(filename))
			return false;
		else return true;
	}else
		return false;
}


int CObjet3D::Unitize()
{

 //Unitize permet de redimensionner le modèle et de le placer au centre de la scene de visualisation
  // on peut effectuer l'operation inverse grace à UnitizeVar définit ici
  // Cette opération nécessite de parcourir deux fois les vertices du modèle
	GLuint  v;
	GLfloat cx, cy, cz, w, h, d;
	GLfloat scale;

	if(this->_pVertices.size()==0)
		return 0;
	vec3 Lo = this->_pVertices[0];
	vec3 Hi = this->_pVertices[0];

	for(v = 0; v < this->_pVertices.size() -1 ;v++)
	{
		if(this->_pVertices[v].x < Lo.x) Lo.x = this->_pVertices[v].x;
		if(this->_pVertices[v].y < Lo.y) Lo.y = this->_pVertices[v].y;
		if(this->_pVertices[v].z < Lo.z) Lo.z = this->_pVertices[v].z;
		if(this->_pVertices[v].x > Hi.x) Hi.x = this->_pVertices[v].x;
		if(this->_pVertices[v].y > Hi.y) Hi.y = this->_pVertices[v].y;
		if(this->_pVertices[v].z > Hi.z) Hi.z = this->_pVertices[v].z;
	}

	w = Abs(Hi.x - Lo.x);
	h = Abs(Hi.y - Lo.y);
	d = Abs(Hi.z - Lo.z);

	cx = (Lo.x + Hi.x) / 2.0;
	cy = (Lo.y + Hi.y) / 2.0;
	cz = (Lo.z + Hi.z) / 2.0;

	scale = 2.0 / Max(Max(w, h), d);

	for (v = 0; v <= this->_pVertices.size() -1; v++)
	{
		this->_pVertices[v].x = this->_pVertices[v].x - cx;
		this->_pVertices[v].y = this->_pVertices[v].y - cy;
		this->_pVertices[v].z = this->_pVertices[v].z - cz;
		this->_pVertices[v].x = this->_pVertices[v].x * scale;
		this->_pVertices[v].y = this->_pVertices[v].y * scale;
		this->_pVertices[v].z = this->_pVertices[v].z * scale;
	}
	UnitizeVar.set(cx,cy,cz,scale); //On sauvegarde les modificateurs
	HightVar=Hi;
	LowVar=Lo;
	ApplicationConfiguration::SetVecParam(ApplicationConfiguration::PARAM_MODEL_MINBOX,LowVar);
	ApplicationConfiguration::SetVecParam(ApplicationConfiguration::PARAM_MODEL_MAXBOX,HightVar);
	selectedDot.set((selectedDot.x-cx)*scale,(selectedDot.y-cy)*scale,(selectedDot.z-cz)*scale);
	ApplicationConfiguration::GLOBAL_CURRENT_APPLICATION_INFORMATIONS.quant_Faces=this->GetNumFaces();
	return scale;
}


long CObjet3D::RenderModel(std::size_t modeRendu, bool blendRendering,bool hideLines)
{//Execute les commandes openGl afin de donner le rendu du modèle
	modelChange=false;
	long g,total=0,m = -1;

	if(!blendRendering)
	{
		glCullFace(GL_FRONT );
		glEnable(GL_CULL_FACE);
		glShadeModel(GL_SMOOTH);
	}else{
		glDisable(GL_CULL_FACE);
		glShadeModel(GL_SMOOTH);
	}
	//Materiel par défaut

	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	vec3 colorDefault=this->colorModelDefault;

	vec4 couleur_ambient(colorDefault.x,colorDefault.y,colorDefault.z,1);
	vec4 couleur_diffuse(colorDefault.x*.6,colorDefault.y,colorDefault.z,1);
	vec4 couleur_specular(colorDefault.x,colorDefault.y,colorDefault.z,1);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, couleur_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleur_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,couleur_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, 128.f);


	for(g=0; g < this->_pGroups.size() ;g++)
	{
		if(m != this->_pGroups[g].Material)
			m = materials->SetMat(this->_pGroups[g].Material);
		if(modeRendu==GL_TRIANGLES)
			total += this->_RenderGroupTexture(g,blendRendering);
		else if(modeRendu==GL_LINES)
		{
			materials->SetColor(this->_pGroups[g].Material);
			total += this->_RenderGroupLines(g,hideLines);
		}
	}

	return total;

}

int CObjet3D::GetDrawableElementSize()
{
	return ApplicationConfiguration::SizeRef(ApplicationConfiguration::ELEMENT_REF_TYPE_DRAWABLE);
}
void CObjet3D::RenderDrawableElement(int eIndex, bool blendRendering )
{
	if(eIndex>=0 && eIndex<GetDrawableElementSize())
	{
		t_HashElement::iterator it=ApplicationConfiguration::BeginRef(ApplicationConfiguration::ELEMENT_REF_TYPE_DRAWABLE);
		for(int navigateIndex=0;navigateIndex<eIndex;navigateIndex++)
			it++;
		E_Drawable* eleToDraw=dynamic_cast<E_Drawable*>(it->second);
		if(eleToDraw)
		{
			std::vector<E_Drawable::t_el_draw_object> drawObject;
			eleToDraw->DrawItem(this->UnitizeVar);
			eleToDraw->DrawTriangles(this->UnitizeVar,drawObject);
			int nbTextToDraw=eleToDraw->labelInfo.size();
			if(nbTextToDraw>0)
			{
				std::vector<E_Drawable::t_labelInfo>* labelVect=&eleToDraw->labelInfo;
				for(int i=0;i<nbTextToDraw;i++)
				{
					this->glPrint(labelVect->at(i).elementLabelPosition,
						labelVect->at(i).elementLabelColor/255,
						labelVect->at(i).elementLabel);
				}
			}

			if(drawObject.size()>0)
			{
				if(!blendRendering)
				{
					glCullFace(GL_FRONT );
					glEnable(GL_CULL_FACE);
					glShadeModel(GL_SMOOTH);
				}else{
					glDisable(GL_CULL_FACE);
					glShadeModel(GL_SMOOTH);
				}
				bool lastStateCulling=!blendRendering;
				//Render des triangles
				glEnable(GL_BLEND);
				glEnable(GL_LIGHTING);
				glMaterialf(GL_FRONT,GL_SHININESS, 128.f);

				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(2.f,2.f);

				glBegin(GL_TRIANGLES);
				for(int obj=0;obj<drawObject.size();obj++)
				{
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, drawObject[obj].colours.colorA);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, drawObject[obj].colours.colorD);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,drawObject[obj].colours.colorS);
					std::vector<E_Drawable::t_triangle>* triangles=&drawObject[obj].triangles;
					std::vector<t_faceIndex>* trianglesIndex=&drawObject[obj].Scenetriangles;
					// Pour les triangles dont les coordonnées sont fournis
					for(int i=0;i<triangles->size();i++)
					{
						vec3 normFace;
						if(!blendRendering)
							normFace=-FaceNormal((*triangles)[i].a,(*triangles)[i].b,(*triangles)[i].c);
						else
							normFace=FaceNormal((*triangles)[i].a,(*triangles)[i].b,(*triangles)[i].c);
						glNormal3fv(normFace);
						glVertex3fv((*triangles)[i].a);
						glVertex3fv((*triangles)[i].b);
						glVertex3fv((*triangles)[i].c);
					}
					// Pour les triangles dont leur indices correspond à celles de la scène

					for(int i=0;i<trianglesIndex->size();i++)
					{
						if((*trianglesIndex)[i].g<this->_pGroups.size() && (*trianglesIndex)[i].f<this->_pGroups[(*trianglesIndex)[i].g].pFaces.size())
						{
							SFace3D* currentFace=&this->_pGroups[(*trianglesIndex)[i].g].pFaces[(*trianglesIndex)[i].f];
							if(lastStateCulling!=!currentFace->internalFace)
							{
								glEnd();
								if(lastStateCulling)
									glDisable(GL_CULL_FACE);
								else
									glEnable(GL_CULL_FACE);
								lastStateCulling=!lastStateCulling;
								glBegin(GL_TRIANGLES);
							}
							if(!blendRendering)
								glNormal3fv(-currentFace->FaceNormals);
							else
								glNormal3fv(currentFace->FaceNormals);
							glVertex3fv(this->_pVertices[currentFace->Vertices.a]);
							glVertex3fv(this->_pVertices[currentFace->Vertices.b]);
							glVertex3fv(this->_pVertices[currentFace->Vertices.c]);
						}
					}
				}
				glEnd();
				glDisable(GL_POLYGON_OFFSET_FILL);
			}
		}
	}
}

std::string CObjet3D::GetNameGroup(long g)
{ //retourne le nom du groupe g du modèle 3D, un groupe contient 0 ou plusieurs faces
	if(g<this->GetNumGroups() && g>=0)
	{
		return this->_pGroups[g].Name;
	}else{
		return "";
	}
}


long CObjet3D::_RenderGroupTexture(long g, bool blendRendering)  //Le rendu
{ //Rendu d'un modèle texturé
	bool hasTextCoords=this->_pTexCoords.size()>0;
	bool cullingActive=!blendRendering;
	bool lastStateCulling=true;




	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.f,2.f);

	if(cullingActive)
		glEnable(GL_CULL_FACE);




	glBegin(GL_TRIANGLES);
	short lastMaterial=-1;
	vec2 texTrans(0.f,0.f);//Texture transformation due to rescale then translation
	vec2 texScale(1.f,1.f);
	for(std::vector<SFace3D>::iterator itface=this->_pGroups[g].pFaces.begin();itface!=this->_pGroups[g].pFaces.end();itface++)
	{
		if(lastMaterial!=(*itface).idMaterial)
		{
			lastMaterial=(*itface).idMaterial;
			glEnd();
			materials->SetMat(lastMaterial);
			if(hasTextCoords)
			{
				if(materials->HasTexture((*itface).idMaterial))
					textures->GetTransformation(materials->GetTextureId((*itface).idMaterial),texScale,texTrans);
			}
			glBegin(GL_TRIANGLES);
		}
		if(cullingActive && lastStateCulling!=!(*itface).internalFace)
		{
			glEnd();
			if((*itface).internalFace)
				glDisable(GL_CULL_FACE);
			else
				glEnable(GL_CULL_FACE);
			lastStateCulling=!(*itface).internalFace;
			glBegin(GL_TRIANGLES);
		}
		if(!blendRendering)
			glNormal3fv((*itface).FaceNormals*-1); //On inverse les normals pour voir l'interieur de l'objet
		if(hasTextCoords)
		{
			vec2 texCoordA=this->_pTexCoords[(*itface).TexCoords.a];
			texCoordA.set(texCoordA.x*texScale.x,texCoordA.y*texScale.y);
			texCoordA+=texTrans;
			glTexCoord2fv(texCoordA);
		}
		if(blendRendering)
			glNormal3fv(this->_pNormals[(*itface).Normals.a]);
		glVertex3fv(this->_pVertices[(*itface).Vertices.a]);
		if(hasTextCoords)
		{
			
			vec2 texCoordB=this->_pTexCoords[(*itface).TexCoords.b];
			texCoordB.set(texCoordB.x*texScale.x,texCoordB.y*texScale.y);
			texCoordB+=texTrans;
			glTexCoord2fv(texCoordB);
		}
		if(blendRendering)
			glNormal3fv(this->_pNormals[(*itface).Normals.b]);
		glVertex3fv(this->_pVertices[(*itface).Vertices.b]);
		if(hasTextCoords)
		{
			vec2 texCoordC=this->_pTexCoords[(*itface).TexCoords.c];
			texCoordC.set(texCoordC.x*texScale.x,texCoordC.y*texScale.y);
			texCoordC+=texTrans;
			glTexCoord2fv(texCoordC);
		}
		if(blendRendering)
			glNormal3fv(this->_pNormals[(*itface).Normals.c]);
		glVertex3fv(this->_pVertices[(*itface).Vertices.c]);
	}
	glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	/*
		*/
	return this->_pGroups[g].pFaces.size();
}



long CObjet3D::_RenderGroupLines(long g,bool hideLines)
{
	glDisable(GL_LIGHTING);
	vec3 colorLines=this->colorModelLines;
	glColor3fv(colorLines);
	glLineWidth(1);
	glBegin(GL_LINES);

	for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
	{
		if(this->_pGroups[g].pFaces[f].Diff[0] || !hideLines)
		{
			glNormal3fv(this->_pNormals[this->_pGroups[g].pFaces[f].Normals.a]);
			glVertex3fv(this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.a]);
			glNormal3fv(this->_pNormals[this->_pGroups[g].pFaces[f].Normals.b]);
			glVertex3fv(this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.b]);
		}
		if(this->_pGroups[g].pFaces[f].Diff[1] || !hideLines)
		{
			glNormal3fv(this->_pNormals[this->_pGroups[g].pFaces[f].Normals.b]);
			glVertex3fv(this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.b]);
			glNormal3fv(this->_pNormals[this->_pGroups[g].pFaces[f].Normals.c]);
			glVertex3fv(this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.c]);
		}
		if(this->_pGroups[g].pFaces[f].Diff[2] || !hideLines)
		{
			glNormal3fv(this->_pNormals[this->_pGroups[g].pFaces[f].Normals.c]);
			glVertex3fv(this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.c]);
			glNormal3fv(this->_pNormals[this->_pGroups[g].pFaces[f].Normals.a]);
			glVertex3fv(this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.a]);
		}
	}
	glEnd();
	#if 0
		//rendu debug octree
		if(IntersectionTool && g==0)
		{
			octreeTool::boxContainer boxLst;
			IntersectionTool->PushBoxInfos(boxLst);
			for(long idbox=0; idbox < boxLst.size() ;idbox++)
			{
				DrawBox(boxLst[idbox].boxcenter,boxLst[idbox].boxhalfsize);
			}
		}
	#endif
	glEnable(GL_LIGHTING);
	return this->_pGroups[g].pFaces.size();
}

bool CObjet3D::_SaveMESH(const char *filename)
{
	using namespace formatMESH;
	formatMESH::CMesh classExport;
	classExport.ExportMESH(this->UnitizeVar,this->_pGroups,this->_pVertices,filename,this->GetNumVertices(),this->GetNumFaces());
	return true;
}

bool CObjet3D::_SaveASC(const char *filename)
{
	formatASC::CAsc classExport;
	//classExport.ExportASC(this->UnitizeVar,this->tabVertexMaillage, filename, this->tabVertexMaillageNbFace);
	classExport.ExportASC(this->UnitizeVar,this->_pGroups,this->_pVertices,filename,this->GetNumVertices(),this->GetNumFaces());
	return true;
}
bool CObjet3D::_SaveNFF(const char *filename)
{
	formatNFF::CNff classExport;
	classExport.ExportNFF(this->UnitizeVar,this->_pGroups,this->_pVertices,filename,this->GetNumVertices(),this->GetNumFaces());
	return true;
}
bool CObjet3D::_SaveBIN(const char *filename)
{
	formatBIN::CformatBIN classExport;
	return classExport.ExportBIN(filename,this->UnitizeVar,this->_pVertices,this->_pTexCoords,this->_pGroups);
}

bool CObjet3D::_SavePLYProjectGroup(const char *filename)
{

	formatRPLY::t_model modelExport;
	/////////////////////////////////////////////////////////////////////
	//Pour les faces appartenant au modèle de base, association avec les matériaux, les récepteurs de surface, les encombrements (de type scène)
	std::vector<std::vector<ApplicationConfiguration::t_PropFace> > linkFacesAssociations(this->_pGroups.size());
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		std::vector<ApplicationConfiguration::t_PropFace> newVec(this->_pGroups[g].pFaces.size());
		linkFacesAssociations[g]=newVec;
	}
	ApplicationConfiguration::GetFaceLink(linkFacesAssociations);

	for(unsigned int i=0;i<this->_pVertices.size();i++)
	{
		modelExport.modelVertices.push_back(coordsOperation::GlCoordsToCommonCoords(UnitizeVar,this->_pVertices[i]));
	}

	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			modelExport.modelFaces.push_back(formatRPLY::t_face(this->_pGroups[g].pFaces[f].Vertices));
		}
	}
	std::vector<int> LayerIdToIdMat;
	int lastidmat=-1;
	int lastlayerid=-1;
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			int idmat=linkFacesAssociations[g][f].idMaterial;
			if(lastidmat!=idmat)
			{
				//Found appropriate layerid
				std::vector<int>::iterator result=LayerIdToIdMat.end();
				std::size_t layerIndex=0;
				for(std::vector<int>::iterator it=LayerIdToIdMat.begin();it!=LayerIdToIdMat.end();it++)
				{
					if(*it==idmat)
					{
						result=it;
						break;
					}else{
						layerIndex++;
					}
				}
				//result = std::find(LayerIdToIdMat.begin(), LayerIdToIdMat.end(), idmat);
				lastidmat=idmat;
				if (result == LayerIdToIdMat.end()) {
					LayerIdToIdMat.push_back(idmat);
					std::string name("unknown");
					E_Materiau* mat=ApplicationConfiguration::GetMateriau(idmat);
					if(mat)
					{
						name=mat->GetElementParent()->GetElementInfos().libelleElement;
					}
					modelExport.modelLayers.push_back(formatRPLY::t_layer(name));
					lastlayerid=LayerIdToIdMat.size()-1;
				} else {
					lastlayerid=layerIndex;
				}
			}
			modelExport.modelFacesLayerIndex.push_back(lastlayerid);
		}
	}
	
	return formatRPLY::CPly::ExportPly(modelExport,filename);
}
bool CObjet3D::_SavePLY(const char *filename)
{

	formatRPLY::t_model modelExport;

	for(unsigned int i=0;i<this->_pVertices.size();i++)
	{
		modelExport.modelVertices.push_back(coordsOperation::GlCoordsToCommonCoords(UnitizeVar,this->_pVertices[i]));
	}
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			modelExport.modelFaces.push_back(formatRPLY::t_face(this->_pGroups[g].pFaces[f].Vertices));
		}
	}
	if(this->_pGroups.size()>1)
	{
		for(int g=0; g < this->_pGroups.size(); g++)
		{
			modelExport.modelLayers.push_back(formatRPLY::t_layer(this->_pGroups[g].Name));
			for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
			{
				modelExport.modelFacesLayerIndex.push_back(g);
			}
		}
	}
	return formatRPLY::CPly::ExportPly(modelExport,filename);
}




bool CObjet3D::_SaveNFFMaillage(const char *filename)
{
	formatNFF::CNff classExport;
	//classExport.ExportTetraNFF(this->UnitizeVar,this->tabVertexMaillage, filename, this->tabVertexMaillageNbFace);
	return true;
}

bool CObjet3D::_LoadSTL(const char *filename)
{
	char m_filepathname[256];
	this->GetPathFromFilename( filename, m_filepathname);
	using namespace formatSTL;

	CStl LoadStl;
	t_model modelstl;
	if(!LoadStl.ImportSTL(modelstl,filename)) return false;
	//Transfert des vertices
	this->_pVertices.clear();
	this->_pVertices.reserve(modelstl.modelVertices.size());
	for(int idvert=0;idvert<modelstl.modelVertices.size();idvert++)
		this->_pVertices.push_back(coordsOperation::CommonCoordsToGlCoords(vec4(0,0,0,1), modelstl.modelVertices[idvert]));

	//Transfer des groupes
	SGroup3D grp;
	this->_pGroups.push_back(grp);
	int g=0;
	unsigned int nbfaces=modelstl.modelFaces.size();
	SFace3D nvface;
	this->_pGroups[g].Name="model";
	this->_pGroups[g].Material=-1;
	this->_pGroups[g].pFaces.reserve(nbfaces);
	for(long f=0;f<nbfaces;f++)
	{
		this->_pGroups[g].pFaces.push_back(nvface);
		this->_pGroups[g].pFaces[f].Vertices=modelstl.modelFaces[f].indicesSommets;

		this->_pGroups[g].pFaces[f].Diff[0]=true;
		this->_pGroups[g].pFaces[f].Diff[1]=true;
		this->_pGroups[g].pFaces[f].Diff[2]=true;
		ivec3 sommetsTri=modelstl.modelFaces[f].indicesSommets;
		this->_pGroups[g].pFaces[f].FaceNormals=FaceNormal(this->_pVertices[sommetsTri.a],this->_pVertices[sommetsTri.b],this->_pVertices[sommetsTri.c]);
		this->_pGroups[g].pFaces[f].idMaterial=-1;
	}
	//Transfert des matières
	this->_nbMaterials=0;
	this->_pNormals.push_back(vec3());
	this->Unitize();
	return true;
}
bool CObjet3D::_LoadPOLY(const char *filename)
{
	char m_filepathname[256];
	this->GetPathFromFilename( filename, m_filepathname);
	using namespace formatPOLY;

	CPoly LoadPoly;
	t_model modelpoly;
	if(!LoadPoly.ImportPOLY(modelpoly, filename)) return false;
	//Transfert des vertices
	this->_pVertices.clear();
	for(int idvert=0;idvert<modelpoly.modelVertices.size();idvert++)
		this->_pVertices.push_back(coordsOperation::CommonCoordsToGlCoords(vec4(0,0,0,1), modelpoly.modelVertices[idvert]));

	//Transfer des groupes
	SGroup3D grp;
	this->_pGroups.push_back(grp);
	int g=0;
	unsigned int nbfaces=modelpoly.modelFaces.size();
	SFace3D nvface;
	this->_pGroups[g].Name="model";
	this->_pGroups[g].Material=-1;
	this->_pGroups[g].pFaces.reserve(nbfaces);
	for(long f=0;f<nbfaces;f++)
	{
		this->_pGroups[g].pFaces.push_back(nvface);
		this->_pGroups[g].pFaces[f].Vertices=modelpoly.modelFaces[f].indicesSommets;

		this->_pGroups[g].pFaces[f].Diff[0]=true;
		this->_pGroups[g].pFaces[f].Diff[1]=true;
		this->_pGroups[g].pFaces[f].Diff[2]=true;
		ivec3 sommetsTri=modelpoly.modelFaces[f].indicesSommets;
		this->_pGroups[g].pFaces[f].FaceNormals=FaceNormal(this->_pVertices[sommetsTri.a],this->_pVertices[sommetsTri.b],this->_pVertices[sommetsTri.c]);
		this->_pGroups[g].pFaces[f].idMaterial=-1;
	}
	//Transfert des matières
	this->_nbMaterials=0;
	this->_pNormals.push_back(vec3());
	this->Unitize();
	return true;
}
bool CObjet3D::_LoadPLY(const char *filename)
{
	formatRPLY::t_model modelImport;
	if(!formatRPLY::CPly::ImportPly(modelImport,filename))
		return false;

	//Transfert des vertices
	this->_pVertices.clear();
	this->_pVertices.reserve(modelImport.modelVertices.size());
	for(std::list<vec3>::iterator itverts=modelImport.modelVertices.begin();itverts!=modelImport.modelVertices.end();itverts++)
	{
		this->_pVertices.push_back(coordsOperation::CommonCoordsToGlCoords(vec4(0,0,0,1), (*itverts)));
	}
	//Transfer des groupes


	std::size_t layerCount(modelImport.modelLayers.size());
	std::size_t facetLayerInfoCount(modelImport.modelFacesLayerIndex.size());
	unsigned int nbfaces=modelImport.modelFaces.size();

	if(layerCount>0 && facetLayerInfoCount==nbfaces)
	{
		this->_pGroups.reserve(layerCount);
		for(std::list<formatRPLY::t_layer>::iterator itlayer=modelImport.modelLayers.begin();itlayer!=modelImport.modelLayers.end();itlayer++)
		{
			this->_pGroups.push_back((*itlayer).layerName);
		}
		//Compte le nombre de face par groupe et reserve la mémoire
		std::vector<std::size_t> faceCountByLayer(layerCount,0);
		for(std::list<std::size_t>::iterator itface=modelImport.modelFacesLayerIndex.begin();itface!=modelImport.modelFacesLayerIndex.end();itface++)
			faceCountByLayer[*itface]++;
		for(std::size_t idgroup=0;idgroup<layerCount;idgroup++)
		{
			this->_pGroups[idgroup].pFaces.reserve(faceCountByLayer[idgroup]);
		}
		//
		std::list<std::size_t>::iterator faceLayerInfoIt=modelImport.modelFacesLayerIndex.begin();
		for(std::list<formatRPLY::t_face>::iterator itface=modelImport.modelFaces.begin();itface!=modelImport.modelFaces.end();itface++)
		{
			SGroup3D& grp=this->_pGroups[*faceLayerInfoIt];
			grp.pFaces.push_back(SFace3D());
			SFace3D& nvface=grp.pFaces.back();
			nvface.Vertices=(*itface).indicesSommets;
			nvface.FaceNormals=FaceNormal(this->_pVertices[nvface.Vertices.a],this->_pVertices[nvface.Vertices.b],this->_pVertices[nvface.Vertices.c]);
			faceLayerInfoIt++;
		}


	}else{
		// Aucune information de calques
		this->_pGroups.push_back(SGroup3D());
		SGroup3D& grp=this->_pGroups[0];

		grp.Name="model";
		grp.Material=-1;
		SFace3D defaultFace;
		defaultFace.Diff[0]=true;
		defaultFace.Diff[1]=true;
		defaultFace.Diff[2]=true;
		defaultFace.idMaterial=-1;

		grp.pFaces.insert(grp.pFaces.begin(),nbfaces,defaultFace);
		unsigned int idface=0;
		for(std::list<formatRPLY::t_face>::iterator itface=modelImport.modelFaces.begin();itface!=modelImport.modelFaces.end();itface++)
		{
			SFace3D& nvface=grp.pFaces[idface];
			nvface.Vertices=(*itface).indicesSommets;
			nvface.FaceNormals=FaceNormal(this->_pVertices[nvface.Vertices.a],this->_pVertices[nvface.Vertices.b],this->_pVertices[nvface.Vertices.c]);
			idface++;
		}
	}
	//Transfert des matières
	this->_nbMaterials=0;
	this->_pNormals.push_back(vec3());
	this->Unitize();
	return true;
}

//#define OPTIMIZE_TEST
bool CObjet3D::_LoadBIN(const char *filename)
{
	#ifdef OPTIMIZE_TEST
	TIMECAPS resolution;
	DWORD start, finish, duration,duration2, i;
	timeBeginPeriod(1);
	double dif;
    start = timeGetTime ();
	#endif

	char m_filepathname[256];
	this->GetPathFromFilename( filename, m_filepathname);
	using namespace formatBIN;
	CformatBIN loadBin;
	t3DModel modelbin={0};
	try
	{
		if(!loadBin.ImportBIN(&modelbin,filename)) return false;
	}
	catch(...)
	{
		wxLogWarning(_("Unable to load this format"));
		return false;
	}

	//Transfert des vertices
	//Transfert des normals
	this->_pVertices.reserve(modelbin.numOfVerts);
	this->_pNormals.reserve(modelbin.numOfVerts);
	this->_pTexCoords.reserve(modelbin.numOfTexCoords);
	for(long v=0;v<modelbin.numOfVerts;v++)
	{
		this->_pVertices.push_back(vec3(modelbin.pVerts[v]));
		this->_pNormals.push_back(vec3(modelbin.pNormals[v]));
	}
	for(long v=0;v<modelbin.numOfTexCoords;v++)
	{
		this->_pTexCoords.push_back(modelbin.pTexVerts[v]);
	}

	//Transfer des groupes
	this->_pGroups.insert(this->_pGroups.end(),modelbin.pObject.size(),SGroup3D());
	std::vector<SGroup3D>::iterator itgroupdest=this->_pGroups.begin();
	unsigned long nbfaceTot=0;

	//for(int g=0;g<modelbin.pObject.size();g++)
	for(std::vector<t3DObject>::iterator itgroup=modelbin.pObject.begin();itgroup!=modelbin.pObject.end();itgroup++)
	{
		SGroup3D grp;
		(*itgroupdest).Name=(*itgroup).strName;
		(*itgroupdest).Material=(*itgroup).materialID;
		(*itgroupdest).pFaces.insert((*itgroupdest).pFaces.end(),(*itgroup).numOfFaces,SFace3D());
		std::vector<SFace3D>::iterator itfacedest=(*itgroupdest).pFaces.begin();
		for(long f=0;f<(*itgroup).numOfFaces;f++)
		{
			(*itfacedest).Vertices.set((*itgroup).pFaces[f].vertIndex);
			(*itfacedest).Normals.set((*itgroup).pFaces[f].vertIndex);
			(*itfacedest).Diff[0]=(*itgroup).pFaces[f].diff[0];
			(*itfacedest).Diff[1]=(*itgroup).pFaces[f].diff[1];
			(*itfacedest).Diff[2]=(*itgroup).pFaces[f].diff[2];
			(*itfacedest).FaceNormals=(*itgroup).pFaces[f].faceNormals;
			(*itfacedest).idMaterial=(*itgroup).pFaces[f].materialID;
			(*itfacedest).TexCoords=(*itgroup).pFaces[f].vertexTex;
			(*itfacedest).internalFace=(*itgroup).pFaces[f].internalFace;
			itfacedest++;
		}
		itgroupdest++;
	}
	//Transfert des matières
	this->_nbMaterials=modelbin.pMaterials.size();
	for(short idmat=0;idmat<modelbin.pMaterials.size();idmat++)
	{
		SMaterial mat;
		//mat.Name = new char[255];
		//strcpy(mat.Name,modelbin.pMaterials[idmat].strName);
		mat.Name=std::string(modelbin.pMaterials[idmat].strName,255);
		mat.ColorA=modelbin.pMaterials[idmat].colorA;
		mat.ColorD=modelbin.pMaterials[idmat].colorD;
		mat.ColorS=modelbin.pMaterials[idmat].colorS;
		mat.idTex=-1; //modelbin.pMaterials[idmat].textureId;
		mat.Shininess=modelbin.pMaterials[idmat].Shininess;
		mat.Transparency=modelbin.pMaterials[idmat].Transparency;

		if(strlen(modelbin.pMaterials[idmat].strFile) > 0)
		{
			wxString pathTexture=wxString(m_filepathname)+modelbin.pMaterials[idmat].strFile;
			mat.idTex = textures->Load(WXSTRINGTOSTDSTRING(pathTexture));
			this->_hasTexture=true;
			if(mat.idTex!=-1)
			{
				wxString saveToLocalPath=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+modelbin.pMaterials[idmat].strFile;
				//On copie la texture dans le dossier du projet
				if(!wxFileExists(saveToLocalPath))
					wxCopyFile(pathTexture,saveToLocalPath,false);
			}
		} else { mat.idTex = -1;}
		materials->Create(&mat);
	}
	#ifdef OPTIMIZE_TEST
	float diff=timeGetTime () - start;
	wxLogDebug("Chargement du modèle en %.3f s",diff/1000);
	timeEndPeriod(1);
	#endif
	this->Unitize();

	return true;
}

bool CObjet3D::_Load3DS(const char *filename)
{
	wxString m_filepathname;
	wxFileName filename_op(filename);
	m_filepathname=filename_op.GetPath()+wxFileName::GetPathSeparator();
	//this->GetPathFromFilename( filename, m_filepathname);

	using namespace format3DS;
	long g,f,j,m;
	SGroup3D grp;
	SFace3D face;
	CLoad3DS Load3ds;
	t3DModel model3ds;
	if(!Load3ds.Import3DS(&model3ds, filename))
		return false;
	long Voffset = 0, Toffset = 0;
	this->_pGroups.reserve(model3ds.numOfObjects);
	//Creation des matériaux
	long matId;
	for(short idmat=0;idmat<model3ds.numOfMaterials;idmat++)
	{
			SMaterial mat;
			m = idmat;
			//mat.Name = new char[255];
			//strcpy(&(*mat.Name),model3ds.pMaterials[m].strName);
			mat.Name=std::string(model3ds.pMaterials[m].strName,255);
			mat.Shininess = 128.0f - model3ds.pMaterials[m].Shininess[0] * 1.28f;
			if (model3ds.pMaterials[m].Transparency[0] > 0.0f)
				mat.Transparency = 1.0f - model3ds.pMaterials[m].Transparency[0] / 100.0f;
			else
				mat.Transparency = 0.0f;
			mat.ColorA.r = model3ds.pMaterials[m].colorA[0] / 255.0f;
			mat.ColorA.g = model3ds.pMaterials[m].colorA[1] / 255.0f;
			mat.ColorA.b = model3ds.pMaterials[m].colorA[2] / 255.0f;
			mat.ColorA.a = mat.Transparency;

			mat.ColorD.r = model3ds.pMaterials[m].colorD[0] / 255.0f;
			mat.ColorD.g = model3ds.pMaterials[m].colorD[1] / 255.0f;
			mat.ColorD.b = model3ds.pMaterials[m].colorD[2] / 255.0f;
			mat.ColorD.a = mat.Transparency;

			mat.ColorS.r = model3ds.pMaterials[m].colorS[0] / 255.0f;
			mat.ColorS.g = model3ds.pMaterials[m].colorS[1] / 255.0f;
			mat.ColorS.b = model3ds.pMaterials[m].colorS[2] / 255.0f;
			mat.ColorS.a = mat.Transparency;

			if(strlen(model3ds.pMaterials[m].strFile) > 0)
			{
				std::string texturename(model3ds.pMaterials[m].strFile,255);
				wxString pathTexture(m_filepathname+wxString(texturename));
				std::string textureFileName=WXSTRINGTOSTDSTRING(pathTexture);


				mat.idTex = textures->Load(textureFileName);

				/*
				try
				{
					mat.idTex = textures->Load(textureFileName);
				}
				catch(...)
				{
					wxLogDebug("Unable to load texture in opengl");
				}
				*/

				if(mat.idTex!=-1)
				{
					wxString saveToLocalPath=ApplicationConfiguration::GLOBAL_VAR.cacheFolderPath+wxString(texturename);
					//On copie la texture dans le dossier du projet
					wxCopyFile(pathTexture,saveToLocalPath,true);
				}


			} else { mat.idTex = -1;}
			matId=materials->Create(&mat);
	}
	//Creation des objets
	this->_hasTexture=false;
	for(g=0; g < model3ds.numOfObjects ;g++)
	{
		if(model3ds.pObject[g].bHasTexture)
			this->_hasTexture = true;
		this->_pGroups.push_back(grp);
		this->_pGroups[g].Name = model3ds.pObject[g].strName;
		this->_pVertices.reserve(model3ds.pObject[g].numOfVerts);
		this->_pNormals.reserve(model3ds.pObject[g].numOfVerts);
		for(j=0; j < model3ds.pObject[g].numOfVerts ;j++)
		{
			this->_pVertices.push_back(model3ds.pObject[g].pVerts[j]);
			this->_pNormals.push_back(model3ds.pObject[g].pNormals[j]);
		}
		this->_pTexCoords.reserve(model3ds.pObject[g].numTexVertex);
		for(j=0; j < model3ds.pObject[g].numTexVertex ;j++)
		{
			this->_pTexCoords.push_back(model3ds.pObject[g].pTexVerts[j]);
		}
		int numtexcoords(this->_pTexCoords.size());
		this->_pGroups[g].pFaces.reserve(model3ds.pObject[g].numOfFaces);
		for(f=0; f < model3ds.pObject[g].numOfFaces ; f++)
		{
			face.Vertices.a = model3ds.pObject[g].pFaces[f].vertIndex[0] + Voffset;
			face.Vertices.b = model3ds.pObject[g].pFaces[f].vertIndex[1] + Voffset;
			face.Vertices.c = model3ds.pObject[g].pFaces[f].vertIndex[2] + Voffset;

			face.Normals.a = model3ds.pObject[g].pFaces[f].vertIndex[0] + Voffset;
			face.Normals.b = model3ds.pObject[g].pFaces[f].vertIndex[1] + Voffset;
			face.Normals.c = model3ds.pObject[g].pFaces[f].vertIndex[2] + Voffset;

			face.FaceNormals = model3ds.pObject[g].pFaces[f].faceNormals;

			if(this->_hasTexture)
			{
				face.TexCoords.a = model3ds.pObject[g].pFaces[f].vertIndex[0] + Toffset;
				face.TexCoords.b = model3ds.pObject[g].pFaces[f].vertIndex[1] + Toffset;
				face.TexCoords.c = model3ds.pObject[g].pFaces[f].vertIndex[2] + Toffset;
			}
			face.idMaterial=model3ds.pObject[g].pFaces[f].materialID;

			face.Diff[0]=model3ds.pObject[g].pFaces[f].diff[0];
			face.Diff[1]=model3ds.pObject[g].pFaces[f].diff[1];
			face.Diff[2]=model3ds.pObject[g].pFaces[f].diff[2];

			this->_pGroups[g].pFaces.push_back(face);
		}
		Voffset += model3ds.pObject[g].numOfVerts;
		Toffset += model3ds.pObject[g].numTexVertex;
		if((model3ds.numOfMaterials > 0) && (model3ds.pObject[g].materialID >= 0))
		{
			this->_pGroups[g].Material = model3ds.pObject[g].materialID+1;
		} else
			this->_pGroups[g].Material = 0;
	}
	this->Unitize();
	return true;
}
void AppendCoplanarFace( const float& coplanarEpsilon, std::vector<vec3>& verticesLst, const t_faceIndex& faceToProcess, const vec3& normSrc, std::vector<SGroup3D>& lstFaces, ui_tools::IntersectionSeeker& intersectionSeeker,BoolHashMap& alreadyProcessedFaces,std::vector<t_faceIndex> &dstLst)
{
	//Méthode récursive qui parcourt chaque sommet à la recherche de faces coplanaires
	alreadyProcessedFaces[faceToProcess.GetHashValue()]=true;
	dstLst.push_back(faceToProcess);
	std::vector<t_faceIndex> dotToFace;
	for(wxInt16 idsommet=0;idsommet<3;idsommet++)
	{
		dotToFace.clear();
		wxInt32 idPt=lstFaces[faceToProcess.g].pFaces[faceToProcess.f].Vertices[idsommet];
		intersectionSeeker.GetSommetFaceNeighboors(verticesLst[idPt],dotToFace);

		wxInt16 sizeFaceVoisins=dotToFace.size();
		//Pour chaque face lié au sommet
		for(wxUint32 idlface=0;idlface<sizeFaceVoisins;idlface++)
		{
			const t_faceIndex* currentFace=&dotToFace[idlface];
			if(alreadyProcessedFaces.find(currentFace->GetHashValue())==alreadyProcessedFaces.end()) //Si la face n'a pas déjà été traité
			{
				if(fabs(normSrc.dot(lstFaces[currentFace->g].pFaces[currentFace->f].FaceNormals))>coplanarEpsilon)
				{
					AppendCoplanarFace(coplanarEpsilon,verticesLst,currentFace,normSrc,lstFaces,intersectionSeeker,alreadyProcessedFaces,dstLst);
				}
			}
		}
	}
}



void CObjet3D::GetAllCoplanarFaces( t_faceIndex firstFace,std::vector<t_faceIndex> &dstLst)
{

	//Selection des faces coplainaire et joint par la face firstFace
	#ifdef _DEBUG
	wxDateTime timeBeforeAlgo(wxDateTime::UNow());
	#endif
	vec3 normSrc=this->_pGroups[firstFace.g].pFaces[firstFace.f].FaceNormals;
	long g=firstFace.g;
	//SGroup3D* currentGroupe=&this->_pGroups[firstFace.g];
	//int fGsize=currentGroupe->pFaces.size();
	#ifdef _DEBUG
	wxDateTime timeAfterAlgo(wxDateTime::UNow());
	wxTimeSpan diff=timeAfterAlgo-timeBeforeAlgo;
	wxLogDebug("Construction des liens en %f secondes",(float)(diff.GetValue().ToLong()/1000.));
	#endif
	BoolHashMap alreadyProcessedFaces; //Liste des faces déjà parcouru
	AppendCoplanarFace(.998f,_pVertices,firstFace,normSrc,this->_pGroups,*IntersectionTool,alreadyProcessedFaces,dstLst);



	/*
	ivec3 sommets=_pGroups[firstFace.g].pFaces[firstFace.f].Vertices;
	vec3 center=GetGTriangle(_pVertices.at(sommets.a),_pVertices.at(sommets.b),_pVertices.at(sommets.c));

	IntersectionTool->GetRegionFaceLst(center,dstLst);
	*/


}


void CObjet3D::ComputeFacesBorders( bool ResetFaceOrientation )
{

	//Création d'un tableau permettant de connaitre rapidement les faces reliées
	std::vector<std::vector<t_faceIndex> > dotToFace(this->_pVertices.size());
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		int fGsize=this->_pGroups[g].pFaces.size();
		for(long f=0; f < fGsize ;f++)
		{
			SFace3D* currentFace=&this->_pGroups[g].pFaces[f];
			dotToFace[currentFace->Vertices.a].push_back(t_faceIndex(f,g));
			dotToFace[currentFace->Vertices.b].push_back(t_faceIndex(f,g));
			dotToFace[currentFace->Vertices.c].push_back(t_faceIndex(f,g));
		}
	}


	//Procédure principale
	/*
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		int fGsize=this->_pGroups[g].pFaces.size();
		for(long f=0; f < fGsize ;f++)
		{
			SFace3D* currentFace=&this->_pGroups[g].pFaces[f];
			t_faceIndex currentFaceIndex(f,g);
			*/
	for(int idvert=0;idvert<dotToFace.size();idvert++)
	{
		for(int linkedFace=0;linkedFace<dotToFace[idvert].size();linkedFace++)
		{
			t_faceIndex currentFaceIndex=dotToFace[idvert][linkedFace];
			SFace3D* currentFace=&this->_pGroups[currentFaceIndex.g].pFaces[currentFaceIndex.f];

			t_faceIndex NeighAB=GetSideFace( currentFaceIndex,ivec2(currentFace->Vertices.a,currentFace->Vertices.b),dotToFace);
			if(NeighAB.IsOk())
				SetDiffSideValue( currentFace, &this->_pGroups[NeighAB.g].pFaces[NeighAB.f], 0);
			t_faceIndex NeighBC=GetSideFace( currentFaceIndex,ivec2(currentFace->Vertices.b,currentFace->Vertices.c),dotToFace);
			if(NeighBC.IsOk())
				SetDiffSideValue( currentFace, &this->_pGroups[NeighBC.g].pFaces[NeighBC.f], 1);
			t_faceIndex NeighCA=GetSideFace( currentFaceIndex,ivec2(currentFace->Vertices.c,currentFace->Vertices.a),dotToFace);
			if(NeighCA.IsOk())
				SetDiffSideValue( currentFace, &this->_pGroups[NeighCA.g].pFaces[NeighCA.f], 2 );
		}
	}
	//todo On change l'orientation des faces corrigés
	/*
	if(ResetFaceOrientation && false)
	{
		for(int g=0; g < this->_pGroups.size(); g++)
		{
			int fGsize=this->_pGroups[g].pFaces.size();
			for(long f=0; f < fGsize ;f++)
			{
				SFace3D* currentFace=&this->_pGroups[g].pFaces[f];
				if(currentFace->dofaceinvertion)
					currentFace->Vertices.set(currentFace->Vertices.c,currentFace->Vertices.b,currentFace->Vertices.a); //On change l'orientation de la face.
			}
		}
	}
	*/
	modelChange=true;
}
t_faceIndex CObjet3D::SelectVertex(float x,float y, float z)
{ //retourne les coordonnées du triangle qui contient le point x,y,z
	vec3 pointSel(x,y,z);

	return IntersectionTool->GetSingleCollision(pointSel);
	/*
	std::vector<t_faceIndex> zoneClick;

	IntersectionTool->GetCollisionLst(vec3(x-.01,y-.01,z-.01),vec3(x+.01,y+0.01,z+0.01),zoneClick);
	//calcul du determinant afin de savoir si le point est sur le même plan que le vertex
	//L'utilité est de reduire le temps de calcul en ne traitant pas les vertex
	// où le plan de celui-ci ne comprend pas le point

	for(int i=0;i<zoneClick.size();i++)
	{
		int g=zoneClick[i].g;
		int f=zoneClick[i].f;
		if(
		DotIsInVertex( pointSel,
		this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.a],
		this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.b],
		this->_pVertices[this->_pGroups[g].pFaces[f].Vertices.c])
		)
		{
			return  t_faceIndex(f,g);
			break;
		}
	}
	return t_faceIndex(-1,-1);
	*/
}

void CObjet3D::RenderSelection()
{
	glShadeModel(GL_SMOOTH);
	selectionChange=false;

	vec4 couleur_ambient(colorSelection.x,colorSelection.y,colorSelection.z,1);
	vec4 couleur_diffuse(colorSelection.x*.6,colorSelection.y,colorSelection.z,1);
	vec4 couleur_specular(colorSelection.x,colorSelection.y,colorSelection.z,1);

	glDepthFunc(GL_LEQUAL); //si supeposition, on affiche bien la sélection par dessus
	glEnable(GL_CULL_FACE);//Pas de test de direction dans le dessin
	glCullFace(GL_BACK );
	glShadeModel(GL_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, couleur_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleur_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,couleur_specular);
	glBegin(GL_TRIANGLES);
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		int fGsize=this->_pGroups[g].pFaces.size();
		for(long f=0; f < fGsize ;f++)
		{
			SFace3D* ptFace=&this->_pGroups[g].pFaces[f];//Calcul du vecteur de translation permettant de recouvrir les surfaces
			if(ptFace->selected)
			{
				vec3 selectionTransVec=ptFace->FaceNormals*BARELY_EPSILON*2;
				glNormal3fv(ptFace->FaceNormals);
				//Triangle coté 1/2
				glVertex3fv(this->_pVertices[ptFace->Vertices.a]+selectionTransVec);
				glVertex3fv(this->_pVertices[ptFace->Vertices.b]+selectionTransVec);
				glVertex3fv(this->_pVertices[ptFace->Vertices.c]+selectionTransVec);
				selectionTransVec*=-1;
				glNormal3fv(ptFace->FaceNormals*-1);
				//Triangle coté 2/2
				glVertex3fv(this->_pVertices[ptFace->Vertices.c]+selectionTransVec);
				glVertex3fv(this->_pVertices[ptFace->Vertices.b]+selectionTransVec);
				glVertex3fv(this->_pVertices[ptFace->Vertices.a]+selectionTransVec);
			}
		}
	}
	glEnd();
	glDepthFunc(GL_LESS);
}

void CObjet3D::UnselectVertex()
{ //Supprime toutes les sélections
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		int fGsize=this->_pGroups[g].pFaces.size();
		for(long f=0; f < fGsize ;f++)
		{
			this->_pGroups[g].pFaces[f].selected=false;
		}
	}
	selectionChange=true;
}


void CObjet3D::UnselectVertex(long face)
{
	this->UnselectVertex(this->FindFaceGroupWithFaceIndex(face));
}

void CObjet3D::UnselectVertex(long face, long group)
{
	this->UnselectVertex(t_faceIndex(face,group));
}

void CObjet3D::UnselectVertex(t_faceIndex faceid)
{
	selectionChange=true;
	this->_pGroups[faceid.g].pFaces[faceid.f].selected=false;
}



void CObjet3D::SelectVertex(long face)
{
	this->SelectVertex(this->FindFaceGroupWithFaceIndex(face));
}

void CObjet3D::SelectVertex(long face, long group)
{
	this->SelectVertex(t_faceIndex(face,group));
}

bool CObjet3D::IsSelectedFace(long face, long group)
{
	return this->_pGroups[group].pFaces[face].selected;
}
const SFace3D& CObjet3D::GetFace(t_faceIndex faceid)
{
	return this->_pGroups[faceid.g].pFaces[faceid.f];
}

void CObjet3D::GetRealCenterFace(const t_faceIndex& faceid,vec3* center)
{
	ivec3 sommets=this->_pGroups[faceid.g].pFaces[faceid.f].Vertices;
	*center=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,GetGTriangle(_pVertices[sommets.a],_pVertices[sommets.b],_pVertices[sommets.c]));
}

void CObjet3D::GetRealVertice(const unsigned long verticeId,vec3* outVert)
{
	*outVert=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[verticeId]);
}
void CObjet3D::SelectVertex(const t_faceIndex& faceid)
{
	if(faceid.f>=0 && faceid.f<this->GetNumFaces(faceid.g))
	{
		this->_pGroups[faceid.group].pFaces[faceid.face].selected=true;
		selectionChange=true;
	}
}

bool CObjet3D::faceConnecte(t_faceIndex _f)
{ //TODO vrai si trois faces on deux vertex en commun avec lui
  // Un objet peut etre maillé si toutes les faces ont faceConnecte à vrai
  // Cette fonction est une ébauche à la correction automatique de modèle 3D ( tentative )


  // correction auto1 fusion des points dont la distance est égale ou inferieure a epsilon
  // suppression des faces ayant deux des 3 points ayant le même index

	SFace3D ft=this->_pGroups[_f.g].pFaces[_f.f];
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			if(_f.g!=g && _f.f!=f)
			{
			}
		}
	}
	return false;
}










float CObjet3D::GetAireScene( )
{
	float airtot=0;
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		for(long f=0; f < this->_pGroups[g].pFaces.size() ;f++)
		{
			vec3 a=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[_pGroups[g].pFaces[f].Vertices.a]);
			vec3 b=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[_pGroups[g].pFaces[f].Vertices.b]);
			vec3 c=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[_pGroups[g].pFaces[f].Vertices.c]);
			vec3 ab;
			vec3 ac;
			Vector(a,b,ab);
			Vector(a,c,ac);
			ab.cross(ac);
			airtot+=.5*ab.normalize();
		}
	}
	return airtot;
}

float CObjet3D::GetSelectionArea( )
{
	float airtot=0;
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		int fGsize=this->_pGroups[g].pFaces.size();
		for(long f=0; f < fGsize ;f++)
		{
			SFace3D* ptFace=&this->_pGroups[g].pFaces[f];//Calcul du vecteur de translation permettant de recouvrir les surfaces
			if(ptFace->selected)
			{
				vec3 a=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[ptFace->Vertices.a]);
				vec3 b=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[ptFace->Vertices.b]);
				vec3 c=coordsOperation::GlCoordsToCommonCoords(UnitizeVar,_pVertices[ptFace->Vertices.c]);
				vec3 ab;
				vec3 ac;
				Vector(a,b,ab);
				Vector(a,c,ac);
				ab.cross(ac);
				airtot+=.5*ab.normalize();
			}
		}
	}
	return airtot;
}

void CObjet3D::FlipSelectedFaces( )
{
	for(int g=0; g < this->_pGroups.size(); g++)
	{
		int fGsize=this->_pGroups[g].pFaces.size();
		for(long f=0; f < fGsize ;f++)
		{
			SFace3D* ptFace=&this->_pGroups[g].pFaces[f];//Calcul du vecteur de translation permettant de recouvrir les surfaces
			if(ptFace->selected)
			{
				ptFace->Vertices.set(ptFace->Vertices.c,ptFace->Vertices.b,ptFace->Vertices.a);
				ptFace->TexCoords.set(ptFace->TexCoords.c,ptFace->TexCoords.b,ptFace->TexCoords.a);
				ptFace->FaceNormals*=-1;
			}
		}
	}
	modelChange=true;
}
void CObjet3D::_Destroy(void)
{
	this->_pVertices.clear();
	this->_pNormals.clear();
	this->_pTexCoords.clear();
	this->_pGroups.clear();

	delete[] tabVertexMaillage;
	delete[] nodesMaillage;
	tabVertexMaillage=NULL;
	nodesMaillage=NULL;


	delete textures;
	delete materials;
	textures = new CTexture();
	materials = new CMaterial();

	selectionChange=true;
	delete IntersectionTool;
	IntersectionTool=NULL;
	this->UnitizeVar.set(0,0,0,1);
	this->LowVar.set(0,0,0);
	this->HightVar.set(0,0,0);
	ApplicationConfiguration::SetVecParam(ApplicationConfiguration::PARAM_MODEL_MINBOX,LowVar);
	ApplicationConfiguration::SetVecParam(ApplicationConfiguration::PARAM_MODEL_MAXBOX,HightVar);
}
CObjet3D::~CObjet3D()
 {
	 this->_Destroy();KillFont();
	 delete textures;
	 delete materials;
 }
bool CObjet3D::GetExactCollisionPosition( t_faceIndex faceCollision, vec3 aproximateCollisionPosition,vec3* exactPosition )
{
	SFace3D* collisionFace=&_pGroups[faceCollision.g].pFaces[faceCollision.f];
	float t,u,v;
	if(collision_manager::intersect_triangle(aproximateCollisionPosition, collisionFace->FaceNormals,
		_pVertices[collisionFace->Vertices.a], _pVertices[collisionFace->Vertices.b],_pVertices[collisionFace->Vertices.c],
					  &t, &u, &v))
	{
		*exactPosition=aproximateCollisionPosition+(collisionFace->FaceNormals*t);
		return true;
	}else{
		return false;
	}
}
