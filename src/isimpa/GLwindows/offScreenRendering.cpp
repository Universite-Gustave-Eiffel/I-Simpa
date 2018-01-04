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


#include "offScreenRendering.h"
#include "wglext.h"      // Sample's header file
#include "last_cpp_include.hpp"


namespace OffScreenRendering
{


	OffScreen::OffScreen(int width,int height)
	{
		// WGL_ARB_extensions_string
		wglGetExtensionsStringARB = NULL;

		// WGL_ARB_pbuffer
		wglCreatePbufferARB    = NULL;
		wglGetPbufferDCARB     = NULL;
		wglReleasePbufferDCARB = NULL;
		wglDestroyPbufferARB   = NULL;
		wglQueryPbufferARB     = NULL;

		// WGL_ARB_pixel_format
		wglGetPixelFormatAttribivARB = NULL;
		wglGetPixelFormatAttribfvARB = NULL;
		wglChoosePixelFormatARB      = NULL;

		//-----------------------------------------------------------------------------
		// GLOBALS
		//-----------------------------------------------------------------------------
		g_hWnd             = NULL;
		g_hDC              = NULL;
		g_hRC              = NULL;
		g_testTextureID    = -1;
		g_dynamicTextureID = -1;


		PBUFFER_WIDTH  = width;
		PBUFFER_HEIGHT = height;


	}

	//-----------------------------------------------------------------------------
	// Name: loadTexture()
	// Desc: 
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// Name: init()
	// Desc: 
	//-----------------------------------------------------------------------------
	void OffScreen::init( HWND _g_hWnd )
	{
		g_hWnd=_g_hWnd;
		GLuint PixelFormat;

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 16;
		pfd.cDepthBits = 16;

		
		g_hDC = GetDC( g_hWnd );
		PixelFormat = ChoosePixelFormat( g_hDC, &pfd );
		SetPixelFormat( g_hDC, PixelFormat, &pfd);
		g_hRC = wglCreateContext( g_hDC );
		wglMakeCurrent( g_hDC, g_hRC );

		//
		// If the required extensions are present, get the addresses for the
		// functions that we wish to use...
		//
	
		wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
		char *ext = NULL;
		
		if( wglGetExtensionsStringARB )
			ext = (char*)wglGetExtensionsStringARB( wglGetCurrentDC() );
		else
		{
			throw(OUTSCREENERROR_wglGetExtensionsStringARB_miss);
		}

		//
		// WGL_ARB_pbuffer
		//

		if( strstr( ext, "WGL_ARB_pbuffer" ) == NULL )
		{
			throw(OUTSCREENERROR_WGL_ARB_pbuffer_extension_not_found);
		}
		else
		{
			wglCreatePbufferARB    = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
			wglGetPbufferDCARB     = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
			wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
			wglDestroyPbufferARB   = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
			wglQueryPbufferARB     = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");
			PFNWGLGETPIXELFORMATATTRIBFVEXTPROC wglGetPixelFormatAttribfvEXT = (PFNWGLGETPIXELFORMATATTRIBFVEXTPROC)wglGetProcAddress("wglGetPixelFormatAttribfvEXT");

			
			if( !wglCreatePbufferARB || !wglGetPbufferDCARB || !wglReleasePbufferDCARB ||
				!wglDestroyPbufferARB || !wglQueryPbufferARB  ) //|| !wglGetPixelFormatAttribfvEXT
			{
				throw(OUTSCREENERROR_WGL_ARB_pbuffer_functions_not_found);
			}
		}

		//
		// WGL_ARB_pixel_format
		//

		if( strstr( ext, "WGL_ARB_pixel_format" ) == NULL )
		{
			throw(OUTSCREENERROR_WGL_ARB_pixel_format_extension_was_not_found);
		}
		else
		{
			wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
			wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
			wglChoosePixelFormatARB      = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

			if( !wglGetExtensionsStringARB || !wglCreatePbufferARB || !wglGetPbufferDCARB )
			{
				throw(OUTSCREENERROR_WGL_ARB_pixel_format_functions_not_found);
			}
		}

		//-------------------------------------------------------------------------
		// Create a p-buffer for off-screen rendering.
		//-------------------------------------------------------------------------

		g_pbuffer.hPBuffer = NULL;
		g_pbuffer.nWidth   = PBUFFER_WIDTH;
		g_pbuffer.nHeight  = PBUFFER_HEIGHT;


		//
		// Define the minimum pixel format requirements we will need for our 
		// p-buffer. A p-buffer is just like a frame buffer, it can have a depth 
		// buffer associated with it and it can be double buffered.
		//

		int pf_attr[] =
		{
			WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
			WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
			WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
			WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
			WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
			WGL_ALPHA_BITS_ARB, 8,              // At least 8 bits for ALPHA channel
			WGL_DEPTH_BITS_ARB, 16,             // At least 16 bits for depth buffer
			WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
			0                                   // Zero terminates the list
		};

		unsigned int count = 0;
		int pixelFormat;
		wglChoosePixelFormatARB( g_hDC,(const int*)pf_attr, NULL, 1, &pixelFormat, &count);

		if( count == 0 )
		{
			throw(OUTSCREENERROR_not_find_acceptable_pixel_format);
		}

		//
		// Create a generic p-buffer...
		//

		g_pbuffer.hPBuffer = wglCreatePbufferARB( g_hDC, pixelFormat, g_pbuffer.nWidth, g_pbuffer.nHeight, NULL );
		g_pbuffer.hDC      = wglGetPbufferDCARB( g_pbuffer.hPBuffer );
		g_pbuffer.hRC      = wglCreateContext( g_pbuffer.hDC );

		if( !g_pbuffer.hPBuffer )
		{
			throw(OUTSCREENERROR_Could_not_create_the_pbuffer);
		}

		//
		// We were successful in creating a p-buffer. We can now make its context 
		// current and set it up just like we would a regular context 
		// attached to a window.
		//

		if( !wglMakeCurrent( g_pbuffer.hDC, g_pbuffer.hRC ) ) 
		{
			throw(OUTSCREENERROR_Could_not_make_the_pbuffer_context_current);
		}
		

	}

	//-----------------------------------------------------------------------------
	// Name: shutDown()
	// Desc: 
	//-----------------------------------------------------------------------------
	void OffScreen::shutDown( void )
	{
		
	}

	//-----------------------------------------------------------------------------
	// Name: render()
	// Desc: 
	//-----------------------------------------------------------------------------

	GLubyte* OffScreen::render( int &datasize )
	{

		//
		// Read out the pixel data from the p-buffer so we can make a texture 
		// later. We have to do this step now because glReadPixels() reads from
		// the buffer of the current context.
		//
		
		datasize = PBUFFER_WIDTH*PBUFFER_HEIGHT*3;
		GLubyte* imgdata = new GLubyte[datasize];
		
		glReadPixels( 0, 0, g_pbuffer.nWidth, g_pbuffer.nHeight, GL_RGB, GL_UNSIGNED_BYTE, imgdata );

		if( g_hRC != NULL )
		{
			wglMakeCurrent( NULL, NULL );
			wglDeleteContext( g_hRC );
			g_hRC = NULL;
		}

		if( g_hDC != NULL )
		{
			ReleaseDC( g_hWnd, g_hDC );
			g_hDC = NULL;
		}

		//
		// Don't forget to clean up after our p-buffer...
		//

		if( g_pbuffer.hRC != NULL )
		{
			wglMakeCurrent( g_pbuffer.hDC, g_pbuffer.hRC );
			wglDeleteContext( g_pbuffer.hRC );
			wglReleasePbufferDCARB( g_pbuffer.hPBuffer, g_pbuffer.hDC );
			wglDestroyPbufferARB( g_pbuffer.hPBuffer );
			g_pbuffer.hRC = NULL;
		}

		if( g_pbuffer.hDC != NULL )
		{
			ReleaseDC( g_hWnd, g_pbuffer.hDC );
			g_pbuffer.hDC = NULL;
		}


		return imgdata;

	}



}