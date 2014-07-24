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

/**************************************************
* OpenGL Viewer
* Herite de GLCanvas, permet l'intégration d'un
* vue 3D dans wxWidget et l'objet peut prendre en
* paramètre un fichier de differends formats
*
*
**************************************************/
/*
 * OpenGlViewer implementation
 */
#include "OpenGlViewer.h"
#include "last_cpp_include.hpp"

BEGIN_EVENT_TABLE(OpenGlViewer, wxGLCanvas)
    EVT_SIZE(OpenGlViewer::OnSize)
    EVT_PAINT(OpenGlViewer::OnPaint)
    EVT_MOUSE_EVENTS(OpenGlViewer::OnMouseEvent)
    EVT_ERASE_BACKGROUND(OpenGlViewer::OnEraseBackground)
    EVT_TIMER(wxID_ANY, OpenGlViewer::OnTimer)
	EVT_KEY_DOWN(OpenGlViewer::OnKeyDown )
END_EVENT_TABLE()


OpenGlViewer::OpenGlViewer(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style,
    const wxString& name, int* gl_attrib)
    : wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name, gl_attrib),
	m_Timer(this)
{
	appLoaded=false;
	simulationIsRunning=false;
	particlesTimeStep=30;
	minimalTimeStep=20;
	LastAnimationTimeStep=9999;
	smooth = GL_TRUE;
	modeaff = GL_TRIANGLES;
    parent->Show(true);
    SetCurrent();
	m_GLApp = new OpenGLApp();
	appLoaded=true;
	cutPlaneToUpdate=false;
	eventBinded=false;
	modeSelectionPoint=false;
	eventPositionBinded=false;
	CurrentObject=NULL;
	currentTool=TOOL_MODE_CAMERA;
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	ElementDrawToUpdate=false;
}

void OpenGlViewer::SetCurrentTool(TOOL_MODE newMode)
{
	currentTool=newMode;
}

OpenGlViewer::TOOL_MODE OpenGlViewer::GetCurrentTool()
{
	return currentTool;
}
int OpenGlViewer::GetImage(wxImage& aimage, const int awidth, const int aheight)
{
	if(appLoaded)
	{
	    #ifdef _WIN32
            int retour = m_GLApp->GetImage(aimage,awidth,aheight,(HWND)this->GetHWND());
        #else
            int retour = m_GLApp->GetImage(aimage,awidth,aheight);
        #endif
		return retour;
	}
	else
		return 0;
}

int OpenGlViewer::GetImage(wxImage& aimage)
{
	if(appLoaded)
	{
		Display();
		//int retour = m_GLApp->GetImage(aimage,awidth,aheight,(HWND)this->GetHWND());
		int retour = m_GLApp->GetImage(aimage);
		wxSizeEvent sizeevt;
		OnSize(sizeevt);
		return retour;
	}
	else
		return 0;
}

/*
int OpenGlViewer::GetImage(wxImage& aimage, const int awidth, const int aheight)
{
#ifdef __WXGTK__
 unsigned char* buffer = (unsigned char*) malloc(awidth*aheight*3) ;
 if(!buffer)
   {
     wxLogError("glcanvas/save") ;
     return -1 ;
   } // if

 OSMesaContext ctx = OSMesaCreateContext(GL_RGB, NULL) ;
 OSMesaMakeCurrent(ctx, buffer, GL_UNSIGNED_BYTE, awidth, aheight) ;
 OSMesaPixelStore(OSMESA_Y_UP, 0) ;

 // OpenGL-Commands, no "GetContext" or "SetCurrent" here!!!
 m_GLApp->ChangeWindow(awidth, aheight);
 CurrentObject->backgroundChange=true;
 m_GLApp->InitGl();
 m_GLApp->RunGlCommands(false);

 aimage.Create(awidth, aheight) ;
 aimage.SetData(buffer) ;
 OSMesaDestroyContext(ctx) ;
 return 0 ;
#else
#ifdef __WXMSW__



 HWND hWND = (HWND) GetHWND() ;

 BITMAPINFO bmi ;
 memset(&bmi, 0, sizeof(BITMAPINFO)) ;
 bmi.bmiHeader.biSize           = sizeof(BITMAPINFOHEADER) ;
 bmi.bmiHeader.biWidth          = awidth ;
 bmi.bmiHeader.biHeight         = aheight ;
 bmi.bmiHeader.biPlanes         = 1 ;
 bmi.bmiHeader.biBitCount       = 24 ;
 bmi.bmiHeader.biCompression    = BI_RGB ;
 bmi.bmiHeader.biSizeImage      = awidth * aheight * 3 ;

 HDC hDC = ::GetDC(hWND) ; // war ::GetDC(pView->m_hWnd)
 LPVOID p ;
HBITMAP hDib = ::CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &p, NULL, 0) ;
 if(!hDib)
 {
   wxLogError("CreateDIBSection returned NULL") ;
   return -1 ;
 } // if

 ::ReleaseDC(hWND, hDC) ;

 HDC hMemDC = ::CreateCompatibleDC(NULL) ;
 if(!hMemDC)
 {
   wxLogError("CreateCompatibleDC returned NULL") ;
   DeleteObject(hDib) ;
   return -1 ;
 } // if

 HANDLE hOldDib = SelectObject(hMemDC, hDib) ;

 PIXELFORMATDESCRIPTOR pixelDesc ;
 pixelDesc.nSize           = sizeof(PIXELFORMATDESCRIPTOR) ;
 pixelDesc.nVersion        = 1 ;
 pixelDesc.dwFlags         =
   PFD_DRAW_TO_BITMAP|PFD_SUPPORT_OPENGL|PFD_STEREO_DONTCARE ;
 pixelDesc.iPixelType      = PFD_TYPE_RGBA ;
 pixelDesc.cColorBits      = 24 ;
 pixelDesc.cRedBits        = 8 ;
 pixelDesc.cRedShift       = 16 ;
 pixelDesc.cGreenBits      = 8 ;
 pixelDesc.cGreenShift     = 8 ;
 pixelDesc.cBlueBits       = 8 ;
 pixelDesc.cBlueShift      = 0 ;
 pixelDesc.cAlphaBits      = 0 ;
 pixelDesc.cAlphaShift     = 0 ;
 pixelDesc.cAccumBits      = 64 ;
 pixelDesc.cAccumRedBits   = 16 ;
 pixelDesc.cAccumGreenBits = 16 ;
 pixelDesc.cAccumBlueBits  = 16 ;
 pixelDesc.cAccumAlphaBits = 0 ;
 pixelDesc.cDepthBits      = 32 ;
 pixelDesc.cStencilBits    = 8 ;
 pixelDesc.cAuxBuffers     = 0 ;
 pixelDesc.iLayerType      = PFD_MAIN_PLANE ;
 pixelDesc.bReserved       = 0 ;
 pixelDesc.dwLayerMask     = 0 ;
 pixelDesc.dwVisibleMask   = 0 ;
 pixelDesc.dwDamageMask    = 0 ;
int nPixelIndex = ::ChoosePixelFormat(hMemDC, &pixelDesc) ;
 if(nPixelIndex == 0) // war !
 {
   wxLogMessage("ChoosePixelFormat returned 0, using default") ;
   nPixelIndex = 1 ; // default
   if(!::DescribePixelFormat(hMemDC, nPixelIndex,
     sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc))
   {
     wxLogError("DescribePixelFormat returned 0") ;
     SelectObject(hMemDC, hOldDib) ;
     DeleteObject(hDib) ;
     DeleteDC(hMemDC) ;
     return -1 ;
   } // if
 } // if

 if(!::SetPixelFormat(hMemDC, nPixelIndex, &pixelDesc))
 {
   wxLogError("SetPixelFormat returned 0") ;
   SelectObject(hMemDC, hOldDib) ;
   DeleteObject(hDib) ;
DeleteDC(hMemDC) ; return -1 ;
 } // if

 HGLRC hMemRC = ::wglCreateContext(hMemDC) ; //this->m_glContext->GetGLRC();
 if(!hMemRC)
 {
   wxLogError("wglCreateContext returned NULL") ;
   SelectObject(hMemDC, hOldDib) ;
   DeleteObject(hDib) ;
DeleteDC(hMemDC) ; return -1 ;
 } // if

 HDC hOldDC = ::wglGetCurrentDC() ;
 HGLRC hOldRC = ::wglGetCurrentContext() ;
 ::wglMakeCurrent(hMemDC, hMemRC) ;

 if(m_GLApp)
 {
	 CurrentObject->backgroundChange=true;
	 m_GLApp->InitGl();

	 // OpenGL Commands, no SetCurrent or GetContext!
	 m_GLApp->ChangeWindow(awidth, aheight);

	 m_GLApp->RunGlCommands(false);
	 legendRendering::ForeGroundGlBitmap foregroundRender(legendDrawer);
	 foregroundRender.Draw(awidth, aheight);

 }


 ::wglMakeCurrent(NULL, NULL) ;
 ::wglDeleteContext(hMemRC) ;
 ::wglMakeCurrent(hOldDC, hOldRC) ;

 aimage.Create(awidth, aheight) ;

 wxLogInfo(_("Copie des données vers l'image"));
 COLORREF c ;
 for(int x=0 ; x<awidth ; x++)
   for(int y=0 ; y<aheight ; y++)
   {
     c = GetPixel(hMemDC, x, y) ;
     aimage.SetRGB(x, y, c & 255, (c >> 8) & 255, (c >> 16) & 255) ;
   } // for

 if(hOldDib)
   SelectObject(hMemDC, hOldDib) ;

 DeleteObject(hDib) ;
DeleteDC(hMemDC) ;


return 0 ;
#else
if(appLoaded)
{
	int retour = m_GLApp->GetImage(aimage,awidth,aheight,(HWND)this->GetHWND());
	OnSize(wxSizeEvent());
	return retour;
}
else
	return 0;
#endif
#endif
}
*/

void OpenGlViewer::SetSimulationRefreshRate(const int& refreshrate)
{
	particlesTimeStep=refreshrate;
}
void OpenGlViewer::OpenModel(CObjet3D *m_Object)
{

	if ( m_Timer.IsRunning() )
        m_Timer.Stop();

    int w, h;
    GetClientSize(&w, &h);
	m_GLApp->ChangeWindow((GLint) w, (GLint) h);
	doScreenRefresh=true;

	if(m_Object->IsFontNeedBuilding())
		#ifdef _WIN32
			m_Object->BuildFont(((HDC)this->GetHDC()));
		#else
			m_Object->BuildFont();
		#endif
	m_GLApp->Init(m_Object);

	CurrentObject=m_Object;
	m_Timer.Start(this->minimalTimeStep,true);
}


void OpenGlViewer::RefreshElementDraw()
{
	ElementDrawToUpdate=true;
	doScreenRefresh=true;
}


void OpenGlViewer::SetCutPlane(bool actif,int axe,int signe, float valeur)
{
  if(appLoaded)
  {
	m_GLApp->cutPlane.actif=actif;
	if(axe>=0 && axe <=2)
		m_GLApp->cutPlane.plane=axe;
	if(signe==1 || signe==-1)
		m_GLApp->cutPlane.sign=signe;
	if(valeur>=0 && valeur <=1)
		m_GLApp->SetPlaneValue(valeur,m_GLApp->cutPlane.plane);
	//RealCoords -> GlCoords
    if(axe==1)
	{
		m_GLApp->cutPlane.plane=2;
	}else{
		if(axe==2)
		{
			m_GLApp->cutPlane.plane=1;
		}
	}
	cutPlaneToUpdate=true; //ce booléen indique au Timer de rafraichir la visualisation lors du prochain pas de temps
						   // faire le rafraichissement par l'intermediaire du timer permet de rendre fluide la
  }						   // modification de la coupe du maillage
}

int OpenGlViewer::GetCameraMode()
{
	return m_GLApp->GetUserCam()->GetCameraMode();

}
OpenGlViewer::CAMERA_MODE OpenGlViewer::NextCameraMode()
{
	if(appLoaded)
	{
		if(m_GLApp->GetUserCam()->GetCameraMode()==this->CAMERA_FIRST_PERSON)
			return CAMERA_ROTATION;
		else
			return CAMERA_FIRST_PERSON;
	}
	return CAMERA_ROTATION;
}
void OpenGlViewer::SetCameraMode(int cameraMode)
{
	if(appLoaded)
	{
		if(cameraMode==this->CAMERA_FIRST_PERSON)
			m_GLApp->GetUserCam()->changeCameraMode(_CAMERA_FP);
		else
			m_GLApp->GetUserCam()->changeCameraMode(_CAMERA_ELLIP);
		doScreenRefresh=true;
	}
}

void OpenGlViewer::ResetCamera()
{
	if(appLoaded)
	{
		m_GLApp->GetUserCam()->ResetCameraPos();
		doScreenRefresh=true;
	}
}

void OpenGlViewer::OnTimer( wxTimerEvent& event) //rafraichie le rendu afin d'afficher l'evolution des particules
{
	try
	{
		if(appLoaded)
		{
			if(ElementDrawToUpdate)
			{
				m_GLApp->UpdateGlElementList();
				ElementDrawToUpdate=false;
			}
			if(cutPlaneToUpdate)
			{

				m_GLApp->UpdateGlMaillageList();
				cutPlaneToUpdate=false;
				doScreenRefresh=true;
			}
			if(simulationIsRunning)
			{
				if( wxDateTime::UNow().GetValue() - LastAnimationTimeStep.GetValue() >=particlesTimeStep)
				{
					doScreenRefresh=true;
					m_GLApp->Tic();//pour chaque pas de temps on informe le module opengl
					LastAnimationTimeStep=wxDateTime::UNow();
				}
			}
			if(doScreenRefresh)
			{
				//Display(&wxClientDC(this));
				doScreenRefresh=false;
				wxPaintEvent ptevt;
				GetEventHandler()->ProcessEvent(ptevt);
			}
		}
	}
	catch( ... ) {
		wxLogError(_("Display error unknown"));
		m_Timer.Start(this->minimalTimeStep,true);
		return;
	}
	m_Timer.Start(this->minimalTimeStep,true);

}

void OpenGlViewer::OnPaint( wxPaintEvent& event )
{
    // This is a dummy, to avoid an endless succession of paint messages.
    // OnPaint handlers must always create a wxPaintDC.


	#ifndef __WXMOTIF__
		if (!GetContext()) return;
	#endif
	wxPaintDC(this);
	Display();
}

void OpenGlViewer::Display()
{
	//Initialisation
	int w, h;
    GetClientSize(&w, &h);

	if(IsShown() && this->GetParent()->IsShown())
		SetCurrent();

	//Execution des commandes de rendu 3D

	m_GLApp->ChangeWindow(w,h);
	m_GLApp->Display();

	legendDrawer.Draw(w,h);

	SwapBuffers();

}
void OpenGlViewer::OnSize(wxSizeEvent& event)
{
	if(!this->IsShownOnScreen())
		return;
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
    int w, h;
    GetClientSize(&w, &h);
#ifndef __WXMOTIF__
    if (GetContext())
#endif
    {
        SetCurrent();

		if(appLoaded)
		{
			m_GLApp->ChangeWindow((GLint) w, (GLint) h);
			doScreenRefresh=true;
		}
    }

}

void OpenGlViewer::OnMouseDoubleClick(t_faceIndex vertexSel)
{
	if(currentTool == TOOL_MODE_SELECTION && eventBinded)
	{
		if(vertexSel.f!= -1 && vertexSel.g!=-1)
		{
			#ifdef _DEBUG
			wxDateTime timeBeforeAlgo(wxDateTime::UNow());
			#endif
			//On demande la liste des toutes les faces coplanaire a cette face
			std::vector<t_faceIndex> coFaces;

			CurrentObject->GetAllCoplanarFaces(vertexSel,coFaces);

			#ifdef _DEBUG
			wxDateTime timeAfterAlgo(wxDateTime::UNow());
			wxTimeSpan diff=timeAfterAlgo-timeBeforeAlgo;
			wxLogDebug("Recherche de la surface en %f secondes",(float)(diff.GetValue().ToLong()/1000.));
			#endif
			(*pointeurFonctionEventSelection)(coFaces,true);
			doScreenRefresh=true;

		}

	}
}


void OpenGlViewer::OnMouseEvent(wxMouseEvent& event)
{
	if(appLoaded)
	{
		if(currentTool == TOOL_MODE_CAMERA || !event.LeftIsDown())
		{
			if(m_GLApp->GetUserCam()->MouseEvent(event.LeftIsDown(),event.MiddleIsDown(),event.RightIsDown(), event.GetX(),event.GetY(),event.GetWheelRotation()))
				doScreenRefresh=true;
		}
		if(event.LeftIsDown())
		{
			if(currentTool == TOOL_MODE_SELECTION || modeSelectionPoint)
				doScreenRefresh=true; //Il faut redessiner avant la sélection, l'algorithme de sélection se servant de la derniere scene dessinée
			if(currentTool == TOOL_MODE_SELECTION)
			{
				t_faceIndex vertexSel = m_GLApp->SelectVertex( event.GetX(),event.GetY());

				if(event.LeftDClick())
				{
					OnMouseDoubleClick(vertexSel);
				}else{
					if(vertexSel!=t_faceIndex(-1,-1) && eventBinded)
					{
						std::vector<t_faceIndex> facelst;
						facelst.push_back(vertexSel);
						(*pointeurFonctionEventSelection)(facelst,event.Dragging());
						doScreenRefresh=true;
					}
				}
			}else if(currentTool == TOOL_MODE_RECEPTEURS_SOUNDLEVEL_EXTRACT)
			{
				vec3 posCurseur=m_GLApp->SelectPosition(event.GetX(),event.GetY());
				if(eventPositionBinded)
					(*pointeurFonctionEventSelectionPosition)(posCurseur);
				doScreenRefresh=true;
				/*
				if(CurrentRssRenderer)
				{
					doScreenRefresh=CurrentRssRenderer->UserWantToKnowAcousticLevelAtPosition(posCurseur);
				}
				*/
			}
			if(modeSelectionPoint && CurrentObject!=NULL)
			{
				vec3 posCurseur=m_GLApp->SelectPosition(event.GetX(),event.GetY());
				t_faceIndex vertexSel = m_GLApp->SelectVertex( event.GetX(),event.GetY());
				vec3 posProjection;

				if(vertexSel!=t_faceIndex(-1,-1) && eventPositionBinded && CurrentObject->GetExactCollisionPosition(vertexSel,posCurseur,&posProjection))
				{
					(*pointeurFonctionEventSelectionPosition)(coordsOperation::GlCoordsToCommonCoords(CurrentObject->UnitizeVar, posProjection));
					doScreenRefresh=true;
				}else{
					(*pointeurFonctionEventSelectionPosition)(coordsOperation::GlCoordsToCommonCoords(CurrentObject->UnitizeVar, posCurseur));
					doScreenRefresh=true;
				}
			}
		}
	}
	event.Skip();
}

void OpenGlViewer::OnEraseBackground( wxEraseEvent& event )
{
    // Ne rien faire afin d'éviter le raffraichissement en boucle
}
void OpenGlViewer::RunSimulation()
{
	simulationIsRunning=true;
}

//Met en pause la simulation tout en laissant actif le rendu des particules
void OpenGlViewer::PauseSimulation()
{
	simulationIsRunning=false;
}

void OpenGlViewer::ChangeRenderMode(OpenGLApp::t_renderMode idRenderMode,bool newValue)
{
	m_GLApp->ChangeRenderMode(idRenderMode,newValue);
	doScreenRefresh=true;
}

bool OpenGlViewer::GetCurrentRenderMode(OpenGLApp::t_renderMode idRenderMode)
{
	return m_GLApp->GetCurrentRenderMode(idRenderMode);
}

void OpenGlViewer::OnKeyDown(wxKeyEvent& keyEvt)
{
	if(appLoaded)
	{
		if(m_GLApp->GetUserCam()->KeyDownEvent(keyEvt.GetKeyCode()))
			doScreenRefresh=true;
	}
	keyEvt.Skip();
}
void OpenGlViewer::InitAnimatorLst()
{
	if(appLoaded)
	{
		m_GLApp->InitAnimatorLst();
	}
}
void OpenGlViewer::ClearAnimatorLst()
{
	if(appLoaded)
	{
		m_GLApp->ClearAnimatorLst();
	}
}
void OpenGlViewer::AddAnimator(Animator* newAnimator)
{
	if(appLoaded)
	{
		m_GLApp->AddAnimator(newAnimator);
		newAnimator->InitLegend(legendDrawer);
	}
}
void OpenGlViewer::StopSimulation()
{
	simulationIsRunning=false;
	if(appLoaded)
	{
		m_GLApp->RestartAnimationRendering();
	}
	doScreenRefresh=true;
}

void OpenGlViewer::NextStepSimulation()
{
	simulationIsRunning=false;
	if(appLoaded)
	{
		m_GLApp->Tic();
		doScreenRefresh=true;
	}
}

void OpenGlViewer::PreviousStepSimulation()
{
	simulationIsRunning=false;
	if(appLoaded)
	{
		m_GLApp->DecreaseTimeStep();
		doScreenRefresh=true;
	}
}

void OpenGlViewer::bindSelectionEvent(void (*fevent)PARAM_BOUND_ON_SELECT_FACES)
{
	pointeurFonctionEventSelection=fevent;
	eventBinded=true;
}

void OpenGlViewer::bindSelectionPositionEvent(void (*fevent)(const vec3))
{
	pointeurFonctionEventSelectionPosition=fevent;
	eventPositionBinded=true;
}

OpenGlViewer::~OpenGlViewer( )
{
	if ( m_Timer.IsRunning() )
        m_Timer.Stop();
	if(appLoaded)
		delete m_GLApp;
}
