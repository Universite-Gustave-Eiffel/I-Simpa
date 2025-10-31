/* ----------------------------------------------------------------------
* I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
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
* For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or
* send an email to contact@noise-planet.org
*
* ----------------------------------------------------------------------*/

#include "GlBitmapSurface.h"
#include <cstring> // keep it for memset into linux build
#include "GL/opengl_inc.h"
#include "3dengine/tools/opengl_test.hpp"
#include <cmath>
#include "last_cpp_include.hpp"

#ifndef MIN
	#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
	#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

namespace legendRendering
{
enum Edge
{
    edLeft, edTop, edRight, edBottom
};
ForeGroundGlBitmap::ForeGroundGlBitmap()
{
}

void ForeGroundGlBitmap::Push(legendObject* newLegend)
{
	legendCfg updatedConf=newLegend->GetCfg();
	if(!gltest::gltIsExtSupported("GL_ARB_texture_non_power_of_two"))
	{
		//La carte graphique ne supporte pas les textures de dimensions qui ne sont pas d'une puissance de 2
		//On redimensionne la texture en puissance de 2 afin qu'elle soit compatible
		float log2ofwidth=log10f((float)updatedConf.width)/log10f(2.f);
		float modRest,tmp;
		modRest=modff(log2ofwidth,&tmp);
		if(updatedConf.width!=updatedConf.height || modRest!=0.f)
		{
			//La texture n'est pas un multiple de 2
			int maxDim=MAX(updatedConf.width,updatedConf.height);
			log2ofwidth=log10f((float)maxDim)/log10f(2.f);
			int newdim=(int)(pow(2,ceilf(log2ofwidth)));
			newLegend->PreserveResize(newdim,newdim);
			legendCfg oldConf=updatedConf;
			updatedConf=newLegend->GetCfg();
			updatedConf.u_text=(float)oldConf.width/(float)newdim;
			updatedConf.v_text=(float)oldConf.height/(float)newdim;
			newLegend->SetCfg(updatedConf);
		}
	}

	glGenTextures(1,(GLuint *)&updatedConf.idTex);
	glBindTexture(GL_TEXTURE_2D,(GLuint)updatedConf.idTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, updatedConf.width, updatedConf.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newLegend->GetRawData());
	newLegend->SetCfg(updatedConf);
	legendsList.push_back(newLegend);
}
ForeGroundGlBitmap::~ForeGroundGlBitmap()
{
	for(int idLegend=0;idLegend<legendsList.size();idLegend++)
	{
		if(legendsList[idLegend]->GetCfg().idTex!=0)
			glDeleteTextures(1,(GLuint *)&legendsList[idLegend]->GetCfg().idTex);
		delete legendsList[idLegend];
	}
}

ForeGroundGlBitmap::ForeGroundGlBitmap(ForeGroundGlBitmap& cpyFrom)
{
	const std::vector<legendObject*>* fromLegendList = cpyFrom.GetLegendObjects();
	for(int idLegend=0;idLegend<fromLegendList->size();idLegend++)
	{
		this->Push(new legendObject(*(*fromLegendList)[idLegend]));
	}
}

const std::vector<legendObject*>* ForeGroundGlBitmap::GetLegendObjects()
{
	return &legendsList;
}

void ForeGroundGlBitmap::Draw(int view_width,int view_height)
{												// Enable Texture Mapping
	int reserved[4];
	memset(reserved,0,sizeof(reserved));
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glViewport (0, 0, view_width, view_height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0, view_width, view_height, 0);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glClear (GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);

	glColor3f(1.f,1.f,1.f);
	glEnable(GL_TEXTURE_2D);

	const int viewHeight_div2=(const int)(view_height/2.f);
	const int viewWidth_div2=(const int)(view_width/2.f);

	for(int idLegend=0;idLegend<legendsList.size();idLegend++)
	{
		int left(0),right(0),top(0),bottom(0);
		legendObject* currentObj=legendsList[idLegend];
		const legendCfg* currentObjConf=&currentObj->GetCfg();

		glBindTexture(GL_TEXTURE_2D,currentObjConf->idTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, currentObjConf->width, currentObjConf->height, GL_RGBA, GL_UNSIGNED_BYTE, currentObj->GetRawData());

		int objWidth, objHeight;

		//TAILLE DE LA LEGENDE
		if(currentObjConf->redim)
		{
			objWidth=(int)(view_width*currentObjConf->redim_width_perc);
			objHeight=(int)(view_height*currentObjConf->redim_height_perc);
		}else{
			objWidth=(int)(currentObjConf->width*currentObjConf->u_text);
			objHeight=(int)(currentObjConf->height*currentObjConf->v_text);
		}
		//POSITION DE LA LEGENDE
		if(currentObjConf->alignementH==ALIGNEMENT_H_CENTER)
		{
			left=(int)(viewWidth_div2-objWidth/2.f+currentObjConf->border_left);
		}else if(currentObjConf->alignementH==ALIGNEMENT_H_LEFT)
		{
			left=currentObjConf->border_left;
		}else if(currentObjConf->alignementH==ALIGNEMENT_H_RIGHT)
		{
			left=view_width-currentObjConf->border_right-objWidth-currentObjConf->border_left;
		}
		right=left+objWidth;
		if(currentObjConf->alignementV==ALIGNEMENT_V_TOP)
		{
			top=reserved[edTop]+currentObjConf->border_top;
			reserved[edTop]+=currentObjConf->border_top+objHeight;
		}else if(currentObjConf->alignementV==ALIGNEMENT_V_CENTER)
		{
			top=(int)(currentObjConf->border_top+viewHeight_div2-objHeight/2.f-currentObjConf->border_bottom);

		}else if(currentObjConf->alignementV==ALIGNEMENT_V_BOTTOM)
		{
			top=view_height-reserved[edBottom]-currentObjConf->border_bottom-objHeight-currentObjConf->border_top;
			reserved[edBottom]+=objHeight+currentObjConf->border_bottom+currentObjConf->border_top;
		}
		bottom=top+objHeight;




		glBegin(GL_QUADS);
			glTexCoord2f(currentObjConf->u_text, 0.0f); glVertex2i(right, top);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(left,top);
			glTexCoord2f(0.0f, currentObjConf->v_text); glVertex2i(left,bottom);
			glTexCoord2f(currentObjConf->u_text, currentObjConf->v_text); glVertex2i(right,bottom);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);


}

void ForeGroundGlBitmap::Delete(legendObject* oldLegend)
{
	for(int idLegend=0;idLegend<legendsList.size();idLegend++)
	{
		if(legendsList[idLegend]==oldLegend)
		{
			if(legendsList[idLegend]->GetCfg().idTex!=0)
				glDeleteTextures(1,(GLuint *)&legendsList[idLegend]->GetCfg().idTex);
			legendsList.erase(legendsList.begin()+idLegend);
			delete oldLegend;
			break;
		}
	}


}

void ForeGroundGlBitmap::Clear()
{
	for(int idLegend=0;idLegend<legendsList.size();idLegend++)
	{
		legendsList[idLegend]->Clear();
	}
}

}
