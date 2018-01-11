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

//#include <mmsystem.h>
#include <wx/log.h>
#include "OpenGLApp.h"

#include "GL/opengl_inc.h"
#include <wx/progdlg.h>
#ifdef _WIN32
	#include "GL\offScreenRendering.h"
#endif
#include "last_cpp_include.hpp"

extern CTexture *textures;
extern CMaterial *materials;


#define FPS_CONSTANT		10.0
#define VIEW_LEFT			-2.0
#define VIEW_RIGHT			2.0
#define VIEW_BOTTOM			-2.0
#define VIEW_TOP			2.0
#define VIEW_NEAR			0.08 //0.1
#define VIEW_FAR			10.0
#define VIEW_PERSPECTIVE	45.0
//#define OPTIMIZE_TEST	//TEST DE COLLISION
AnimatorManager::AnimatorManager()
{
	this->gl_compilation_array_size=0;
	this->gl_compilation_array=NULL;
	this->ctrl_animator=NULL;
	lastCompilationTime=wxDateTime::UNow();
}
void AnimatorManager::FreeGlCompilationArray()
{
	for(unsigned int idstep=0;idstep<this->gl_compilation_array_size;idstep++)
		glDeleteLists(this->gl_compilation_array[idstep],1);
	this->gl_compilation_array_size=0;
	delete[] this->gl_compilation_array;
	this->gl_compilation_array=NULL;
}
void AnimatorManager::SetAnimator(Animator* _animator)
{
	this->ctrl_animator=_animator;
}
AnimatorManager::~AnimatorManager()
{
	FreeGlCompilationArray();
}
void AnimatorManager::ResetCompilationArray()
{
	FreeGlCompilationArray();
	if(ctrl_animator)
	{
		this->gl_compilation_array=new GLuint[ctrl_animator->GetNbTimeStep()];
		gl_compilation_array_size=ctrl_animator->GetNbTimeStep();
	}
}

OpenGLApp::OpenGLApp()
{
	m_IsObjetLoaded = false;
	winHeight=600;
	winWidth=800;
	//Initialisation de la camera
	userCamera.Init(_CAMERA_ELLIP,&winWidth,&winHeight,&m_Trans,&m_Rot,&m_Focal,&m_Model_Trans,&m_Model_Rot);
	InitCutPlane();
	memset(m_list, -1, sizeof(GLuint)*4);
	CurrentTimeStep=0;


	memset(renderMode, 0, sizeof(bool)*GL_APP_QUANT_RENDER_TYPE);

	renderMode[renderModelFaces]=true;
	renderMode[renderModelLines]=false;
	renderMode[renderModelLinesAndConstruction]=false;
	renderMode[renderModelInside]=true;
	renderMode[renderMaillageFaces]=true;
	renderMode[renderMaillageLines]=false;
	renderMode[renderSelection]=true;
}

void OpenGLApp::InitCutPlane()
{
	this->cutPlane.actif=false;
	this->cutPlane.plane=0;
	this->cutPlane.sign=1;
	this->cutPlane.value=-1;
}

void OpenGLApp::SetPlaneValue(float valeur,int axe)
{
	if(IsObjetLoaded())
	{
		float min=m_Object->LowVar[axe]-0.1;
		float max=m_Object->HightVar[axe]+0.1;
		this->cutPlane.value=((((max-min)*valeur)+min)-m_Object->UnitizeVar[axe])*m_Object->UnitizeVar[3];
	}
}

void OpenGLApp::RestartAnimationRendering()
{
	SetTimeStep(0);
}


CameraControler* OpenGLApp::GetUserCam(void)
{
	return &userCamera;
}

void OpenGLApp::ResetAll(void)
{
	if(IsObjetLoaded())
	{
		Destroy();
	}

	//ClearAnimatorLst();

	m_Rot      = vec3(0.0, 1.0, 0.0);
	m_Trans    = vec3(0.00049605915f,0.039786763f,1.7203468f);
	m_Focal    = vec3(0.00027848946f,0.022336427f,2.7201939f);

	m_Model_Rot = vec3(0.0, 0.0, 0.0);
	m_Model_Trans = vec3(0.0, 0.0, 0.0);

	m_RotLight = vec3(0.0, 0.0, 0.0);

	m_AmbientLight   = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	m_DiffuseLight   = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	m_SpecularLight  = vec4(0.8f, 0.8f, 0.8f, 1.0f);

	m_PosLight       = vec4(0.0f, 0.0f, 4.0f, 1.0f);
	m_DirectionLight = vec3(0.0f, 0.0f, -1.0f);
	m_SpotAngle =  45.0f;

}

void OpenGLApp::Init(CObjet3D *im_Object)
{
    if(m_Object != im_Object) {
	    m_Object=im_Object;
	    ResetAll();
        m_IsObjetLoaded = true;
	    InitCutPlane();
	    InitGl();
		LoadGlMaillageList();
		LoadGlModelList();
    }
}


void OpenGLApp::UpdateGlList()
{
	if(!m_IsObjetLoaded)
		return;
	LoadGlModelList(); //on continu le chargement de la liste model si on doit dessiner les lignes Après
    LoadGlMaillageList();
}


void OpenGLApp::UpdateGlElementList(bool useLists)
{
	if(!m_IsObjetLoaded)
		return;
	if(useLists)
	{
		InitLst(2);
		glNewList(m_list[2], GL_COMPILE);
	}
	int nbEl=m_Object->GetDrawableElementSize();
	for(int idEl=0;idEl<nbEl;idEl++)
		m_Object->RenderDrawableElement(idEl,!renderMode[renderModelInside]);
	if(useLists)
		glEndList();
}

void OpenGLApp::UpdateGlSelectionList(bool useLists)
{
	if(!m_IsObjetLoaded)
		return;
	if(useLists)
	{
		InitLst(3);
		glNewList(m_list[3], GL_COMPILE);
	}
	m_Object->RenderSelection();
	if(useLists)
		glEndList();
}

void OpenGLApp::UpdateGlMaillageList()
{
	if(!m_IsObjetLoaded)
		return;
	LoadGlMaillageList();
}

void OpenGLApp::ChangeRenderMode(t_renderMode idRenderMode,bool newValue)
{
	if(renderMode[idRenderMode]!=newValue)
	{
		renderMode[idRenderMode]=newValue;
	}
	//	On réactualise les listes de rendu en fonction des types de mise à jour
	if(idRenderMode== renderModelFaces || idRenderMode== renderModelLines || idRenderMode==renderModelInside|| idRenderMode== renderModelLinesAndConstruction)
		this->UpdateGlList();
}

bool OpenGLApp::GetCurrentRenderMode(t_renderMode idRenderMode)
{
	return renderMode[idRenderMode];
}

void OpenGLApp::LoadGlModelList(bool useLists)
{

	if(useLists)
	{
		InitLst(0);
		glNewList(m_list[0], GL_COMPILE);
	}
	if(renderMode[renderModelFaces] && !renderMode[renderModelMaterialFaces])
		m_Object->RenderModel(GL_TRIANGLES,!renderMode[renderModelInside],!renderMode[renderModelLinesAndConstruction]);
	if(renderMode[renderModelLines])
		m_Object->RenderModel(GL_LINES,!renderMode[renderModelInside],!renderMode[renderModelLinesAndConstruction]);
	if(useLists)
		glEndList();
}

void OpenGLApp::LoadGlMaillageList(bool useLists)
{
	if(useLists)
	{
		InitLst(1);
		glNewList(m_list[1], GL_COMPILE);
	}
	if(renderMode[renderModelFaces] || renderMode[renderModelMaterialFaces])
		m_Object->RenderMaillage(GL_TRIANGLES,this->cutPlane);
	if(renderMode[renderModelLines])
		m_Object->RenderMaillage(GL_LINES,this->cutPlane);

	if(useLists)
		glEndList();
}

t_faceIndex OpenGLApp::SelectVertex(unsigned int x, unsigned int y) //retourne le n° de vertex sous le curseur
{
	#ifdef OPTIMIZE_TEST
	TIMECAPS resolution;
	DWORD start, finish, duration,duration2, i;
	timeBeginPeriod(1);
	double dif;
    start = timeGetTime ();
	#endif
	if(!m_IsObjetLoaded)
		return t_faceIndex(-1,-1);
	vec3 posCurseur=SelectPosition(x,y);
	t_faceIndex faceSelectionne;
	#ifdef OPTIMIZE_TEST
	duration2 = timeGetTime () - start;
	//for(int i=0;i<10000;i++)
	#endif
	faceSelectionne=m_Object->SelectVertex(posCurseur.x, posCurseur.y, posCurseur.z); //vertex a partir de x,y,z (sur le plan du triangle)
	#ifdef OPTIMIZE_TEST
    duration = timeGetTime () - start;
	#endif
	ChangeWindow(winWidth,winHeight); //permet de ne pas etre affecté par les modifications openGl apporté par le moteur de collision
	#ifdef OPTIMIZE_TEST
	float diff=duration;
	float diff2=duration2;
	wxLogMessage("Test Collision en %.3f s (Collision vecteur en %.3f s)",diff,diff2);
	timeEndPeriod(1);
	#endif
	return faceSelectionne;
}

vec3 OpenGLApp::SelectPosition(unsigned int x, unsigned int y)
{
	this->ChangeWindow(winWidth,winHeight);
	Display();

	if(!m_IsObjetLoaded)
		return vec3(0,0,0);
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return vec3(posX, posY, posZ); //x,y,z (sur le plan du triangle)
}
void OpenGLApp::InitGl()
{
	//glClearColor(0.,0.,0.,0);
	if(m_Object)
		m_Object->backgroundChange=true;

	glLightfv(GL_LIGHT0, GL_AMBIENT, m_AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, m_PosLight);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, m_DirectionLight);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, m_SpotAngle);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glIsEnabled(GL_ALPHA_TEST)) glDisable(GL_ALPHA_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

int OpenGLApp::GetImage(wxImage& aimage, const int awidth, const int aheight,void* _contextv)
{
	#ifdef _WIN32
	HWND _context=(HWND)_contextv;
	GLubyte* pixels=NULL;
	bool isOk=false;
	try
	{
		OffScreenRendering::OffScreen GlShotOffScreen(awidth,aheight);

		GlShotOffScreen.init(_context);
		ChangeWindow(awidth,aheight);

		m_Object->backgroundChange=true;
		m_Object->BuildFont(GetDC( _context ));
		InitGl();
		textures->ReloadAllTextures();
		RunGlCommands(false);
		int dataSize=0;

		pixels = GlShotOffScreen.render(dataSize);

		isOk=aimage.Create(awidth, aheight,pixels);

		aimage=aimage.Mirror(false);
		textures->Destroy();
	}
	catch( OffScreenRendering::OffScreen::OUTSCREENERROR idError )
	{
		switch(idError)
		{
			case OffScreenRendering::OffScreen::OUTSCREENERROR_Could_not_create_the_pbuffer:
				wxLogMessage(_("Resolution is too high, your graphic card is enable to manage this size. Please choose a lower export resolution"));
				break;
			case OffScreenRendering::OffScreen::OUTSCREENERROR_Could_not_make_the_pbuffer_context_current:
				break;
			case OffScreenRendering::OffScreen::OUTSCREENERROR_not_find_acceptable_pixel_format:
				break;
			case OffScreenRendering::OffScreen::OUTSCREENERROR_WGL_ARB_pbuffer_extension_not_found:
				break;
			case OffScreenRendering::OffScreen::OUTSCREENERROR_WGL_ARB_pbuffer_functions_not_found:
				break;
			case OffScreenRendering::OffScreen::OUTSCREENERROR_WGL_ARB_pixel_format_extension_was_not_found:
				break;
			case OffScreenRendering::OffScreen::OUTSCREENERROR_WGL_ARB_pixel_format_functions_not_found:
				break;
			case OffScreenRendering::OffScreen::OUTSCREENERROR_wglGetExtensionsStringARB_miss:
				break;
			default:
				wxLogError(_("Unknown OpenGL error, try to set a lower resolution or use another exportation method."));
		}
		wxLogMessage(_("Error code : %i"),(int)idError);
		isOk=false;
	}
	catch( ... )
	{
		wxLogError(_("OpenGL exportation error. Try to use the window exportation method instead"));
		isOk=false;
	}

	if(isOk)
		return 1;
	else
	{
		return 0;
	}
	#else
	return 0;
	#endif
}



int OpenGLApp::GetImage(wxImage& aimage)
{
	GLint view[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv(GL_VIEWPORT, view);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glReadBuffer( GL_BACK_LEFT ); //GL_BACK_LEFT

	unsigned char *pixels;

	pixels = (unsigned char *) malloc(3 * winWidth * winHeight);

	glReadPixels( 0, 0, winWidth , winHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels );

	aimage.Create( winWidth , winHeight);
	aimage.SetData( pixels );


    aimage=aimage.Mirror(false);


	return 1;
}




void OpenGLApp::Tic()
{

	int nbStep=0;

	for (const ptAnimatorManager& curManager : animators)
	{
		if(curManager->gl_compilation_array_size>nbStep)
			nbStep=curManager->gl_compilation_array_size;
	}

	if(this->CurrentTimeStep<nbStep-1)
		SetTimeStep(CurrentTimeStep+1);
	else
		SetTimeStep(0);
}
void OpenGLApp::DecreaseTimeStep()
{
	int nbStep=0;

	for (const ptAnimatorManager& curManager : animators)
	{
			if(curManager->gl_compilation_array_size>nbStep)
				nbStep=curManager->gl_compilation_array_size;
	}

	if(this->CurrentTimeStep>0)
		SetTimeStep(CurrentTimeStep-1);
	else
		SetTimeStep(nbStep-1);
}
void OpenGLApp::RunGlCommands(bool useLists) //useLists à vrai si l'on doit utiliser les listes openGl compilés
{

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(IsObjetLoaded())
	{
		if(m_Object->backgroundChange)
		{
			m_Object->backgroundChange=false;

			vec3 backgroundColor=m_Object->GetBackgroundColor();
			glClearColor(backgroundColor[0],backgroundColor[1],backgroundColor[2],0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt( m_Focal.x, m_Focal.y, m_Focal.z,
				   m_Trans.x, m_Trans.y, m_Trans.z,
				   m_Rot.x, m_Rot.y, m_Rot.z );
		glTranslatef(m_Model_Trans.x, m_Model_Trans.y, m_Model_Trans.z );
		glRotatef( m_Model_Rot.x, 1.0f, 0.0f, 0.0f);
		glRotatef( m_Model_Rot.y, 0.0f, 1.0f, 0.0f);
		glRotatef( m_Model_Rot.z, 0.0f, 0.0f, 1.0f);

		glPushMatrix();

		if(m_Object->modelChange || !useLists)
			this->LoadGlModelList(useLists);
		if(useLists)
			glCallList(m_list[0]);
		if((renderMode[renderMaillageFaces]) || renderMode[renderMaillageLines])
			if(useLists)
				glCallList(m_list[1]);

		if(m_Object->selectionChange || !useLists)
			this->UpdateGlSelectionList(useLists);

		if(useLists)
			glCallList(m_list[3]);//Rendu des vertex sélectionnés
		if(!useLists)
			this->UpdateGlElementList(useLists);
		if(useLists)
			glCallList(m_list[2]); //e_drawable

		//////////////////////////////////////////////////////
		// Rendu des animations
		for (ptAnimatorManager& curManager : animators)
		{
			Animator* curAnimator=curManager->ctrl_animator;
			if(curAnimator!=NULL && curAnimator->IsRenderingEnable())
			{
				if(curAnimator->IsNeedUpdate(curManager->lastCompilationTime))
					LoadAnimatorLst(curManager);
				if(useLists)
				{
					if(curManager->gl_compilation_array_size > CurrentTimeStep)
					{
						int firstLoopIndice=0;
						if(!curAnimator->DoLoopFromZero())
							firstLoopIndice=CurrentTimeStep;
						if(firstLoopIndice<0)
							firstLoopIndice=0;
						for(int loopI=firstLoopIndice;loopI<=CurrentTimeStep;loopI++)
							glCallList(curManager->gl_compilation_array[loopI]);
					}else{
						if(curManager->gl_compilation_array_size>0)
							glCallList(curManager->gl_compilation_array[curManager->gl_compilation_array_size-1]);
					}
				}else{
					curAnimator->Render(CurrentTimeStep);
				}
			}
		}
		glPopMatrix();

	}
}

//void OpenGLApp::Display(HDC hDC)
void OpenGLApp::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RunGlCommands();
	//SwapBuffers(hDC);
}

void OpenGLApp::ChangeWindow(int w, int h)
{
	winWidth=w;
	winHeight=h;
	if (h==0) h=1;
	glViewport(0,0,w,h);
	Ratio = (GLfloat)w/(GLfloat)h;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(VIEW_PERSPECTIVE, Ratio, VIEW_NEAR, VIEW_FAR);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


void OpenGLApp::InitLst(int index)
{
	DeleteLst(index);
	m_list[index] = glGenLists(1);
	m_list_isinit[index]=true;
}
void OpenGLApp::DeleteLst(int index)
{
	if(m_list_isinit[index])
		glDeleteLists(m_list[index],1);
}
void OpenGLApp::LoadAnimatorLst(ptAnimatorManager& managerToCompile)
{
	managerToCompile->lastCompilationTime=wxDateTime::UNow();
	managerToCompile->ResetCompilationArray();
	unsigned int nbStep=managerToCompile->gl_compilation_array_size;
	if(nbStep>0)
	{
		wxProgressDialog progDialog(_(wxString::Format("Compilation du rendu de l'animation '%s'",managerToCompile->ctrl_animator->GetRendererLabel())),_("Preparing display of surfaces receivers"),100,NULL,wxPD_CAN_ABORT | wxPD_REMAINING_TIME |wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL );
		progDialog.Update(0);
		for(int tStep=0;tStep<nbStep;tStep++)
		{
			int prog=int(((float)tStep/nbStep)*100);
			if(prog>0 && prog<100)
			{
				progDialog.Update(prog,wxString::Format(_("Loading time step %i/%i"),tStep+1,nbStep));
			}
			managerToCompile->gl_compilation_array[tStep]=glGenLists(1);
			glNewList(managerToCompile->gl_compilation_array[tStep], GL_COMPILE);
			managerToCompile->ctrl_animator->Render(tStep);
			glEndList();
		}
	}
}

void OpenGLApp::Destroy()
{
	DeleteLst(0);
	DeleteLst(1);
	DeleteLst(2);
	DeleteLst(3);
	memset(m_list_isinit, 0, sizeof(bool)*4);
	ClearAnimatorLst();
}


void OpenGLApp::SetTimeStep(int idTimeStep)
{
	CurrentTimeStep=idTimeStep;
	for (const ptAnimatorManager& curManager : animators)
	{
		curManager->ctrl_animator->SetTimeStep(CurrentTimeStep);
	}
}
void OpenGLApp::InitAnimatorLst()
{
	for (ptAnimatorManager& curManager : animators)
	{
		curManager->FreeGlCompilationArray();
		curManager->ctrl_animator->Init();
	}
}
void OpenGLApp::ClearAnimatorLst()
{
		for(int idanimatormanager=0;idanimatormanager<animators.size();idanimatormanager++)
		{
			ptAnimatorManager curManager=animators[idanimatormanager];
			for(int idstep=0;idstep<curManager->gl_compilation_array_size;idstep++)
				glDeleteLists(curManager->gl_compilation_array[idstep],1);
	    }
		animators.clear();
}
void OpenGLApp::AddAnimator(Animator* newAnimator)
{
	AnimatorManager* newManager=new AnimatorManager;
	newManager->SetAnimator(newAnimator);
	animators.push_back(newManager);
}
