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
#include <GL/GLAux.h>
#include "wglext.h"      // Project's header file
//#include <GL/wglext.h> // Your local header file


#ifndef __OFF_SCREEN_RENDER
#define __OFF_SCREEN_RENDER

namespace OffScreenRendering
{


	//-----------------------------------------------------------------------------
	//           Name: ogl_offscreen_rendering_1.cpp
	//         Author: Kevin Harris
	//  Last Modified: 03/01/08
	//   Modification: Nicolas Fortin - LCPC Nantes Bouguenais - FRANCE

	

	//-----------------------------------------------------------------------------
	// FUNCTION POINTERS FOR OPENGL EXTENSIONS
	//-----------------------------------------------------------------------------

	// For convenience, this project ships with its own "wglext.h" extension header 
	// file. If you have trouble running this sample, it may be that this "wglext.h" 
	// file is defining something that your hardware doesn’t actually support. 
	// Try recompiling the sample using your own local, vendor-specific "wglext.h" 
	// header file.

	class OffScreen
	{
	private:
		// WGL_ARB_extensions_string
		PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

		// WGL_ARB_pbuffer
		PFNWGLCREATEPBUFFERARBPROC    wglCreatePbufferARB;
		PFNWGLGETPBUFFERDCARBPROC     wglGetPbufferDCARB;
		PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
		PFNWGLDESTROYPBUFFERARBPROC   wglDestroyPbufferARB;
		PFNWGLQUERYPBUFFERARBPROC     wglQueryPbufferARB;

		// WGL_ARB_pixel_format
		PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
		PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
		PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;

		//-----------------------------------------------------------------------------
		// GLOBALS
		//-----------------------------------------------------------------------------
		HWND   g_hWnd;
		HDC	   g_hDC;
		HGLRC  g_hRC;

		HWND   old_g_hWnd;
		HDC	   old_g_hDC;
		HGLRC  old_g_hRC;

		GLuint g_testTextureID;
		GLuint g_dynamicTextureID;


		int PBUFFER_WIDTH;
		int PBUFFER_HEIGHT;

		// This litttle struct will help to organize our p-buffer's data
		typedef struct {

			HPBUFFERARB hPBuffer;
			HDC         hDC;
			HGLRC       hRC;
			int         nWidth;
			int         nHeight;

		} PBUFFER;


		PBUFFER g_pbuffer;

		float  g_fElpasedTime;
		double g_dCurTime;
		double g_dLastTime;

	public:

		enum OUTSCREENERROR
		{
			OUTSCREENERROR_wglGetExtensionsStringARB_miss,
			OUTSCREENERROR_WGL_ARB_pbuffer_extension_not_found,
			OUTSCREENERROR_WGL_ARB_pbuffer_functions_not_found,
			OUTSCREENERROR_WGL_ARB_pixel_format_extension_was_not_found,
			OUTSCREENERROR_WGL_ARB_pixel_format_functions_not_found,
			OUTSCREENERROR_not_find_acceptable_pixel_format,
			OUTSCREENERROR_Could_not_create_the_pbuffer,
			OUTSCREENERROR_Could_not_make_the_pbuffer_context_current
		};

		OffScreen(int width,int height);
		~OffScreen(){ shutDown();};
		void init(HWND _g_hWnd);
		GLubyte* render(int &datasize);
		void shutDown(void);
	};
}

#endif