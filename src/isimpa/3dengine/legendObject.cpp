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

#include "legendObject.h"
#include <memory.h>
#include "last_cpp_include.hpp"


namespace legendRendering
{

	#ifndef MIN
		#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
	#endif
	#ifndef MAX
		#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
	#endif
	legendCfg::legendCfg()
	{
		memset(this,0,sizeof(legendCfg));
		u_text=1.f;
		v_text=1.f;
	}


	legendObject::legendObject()
	{
		foreground_tex=0;
	}
	
	legendObject::legendObject(legendObject& cpyFrom)
	{
		objConfig=cpyFrom.GetCfg();
		Init(objConfig.width,objConfig.height);
		Copy(cpyFrom.GetRawData(),objConfig.width,objConfig.height);
	}
	legendObject::~legendObject()
	{
		delete[] foreground_tex;
	}

	void legendObject::Init(int _width,int _height)
	{
		if(foreground_tex)
			delete[] foreground_tex;
		objConfig.width=_width;
		objConfig.height=_height;
		foreground_tex = new FGL_BYTE[objConfig.width*objConfig.height*4];
		Clear();
	}
	
	void legendObject::PreserveResize(int _newwidth,int _newheight)
	{
		if(foreground_tex && (_newwidth!=objConfig.width || _newheight!=objConfig.height))
		{
			int oldWidth(objConfig.width),oldHeight(objConfig.height);
			FGL_BYTE* old_tex = foreground_tex;
			foreground_tex=(legendRendering::FGL_BYTE *)0;
			Init(_newwidth,_newheight);
			Copy(old_tex,oldWidth,oldHeight);
			delete[] old_tex;
		}else{
			Init(_newwidth,_newheight);
		}
	}
	void legendObject::Clear()
	{
		if(foreground_tex)
			memset(foreground_tex,0,objConfig.width*objConfig.height*4);
	}

	FGL_BYTE* legendObject::GetRawData()
	{
		return foreground_tex;
	}
	const legendCfg& legendObject::GetCfg()
	{
		return objConfig;
	}

	void legendObject::SetCfg(legendCfg& cpyFrom)
	{
		int answidth=objConfig.width;
		int ansheight=objConfig.height;
		objConfig=cpyFrom;
		objConfig.width=answidth;
		objConfig.height=ansheight;
	}

	void legendObject::Copy(const FGL_BYTE* RGBA_src,int _sourcewidth,int _sourceheight)
	{
		if(RGBA_src)
		{
			if(!foreground_tex)
				Init(_sourcewidth,_sourceheight);
			if(objConfig.width==_sourcewidth && objConfig.height==_sourceheight)
			{
				memcpy(foreground_tex,RGBA_src,objConfig.width*objConfig.height*4);
			}else{
				unsigned int range_x(MIN(_sourcewidth,objConfig.width));
				unsigned int range_y(MIN(_sourceheight,objConfig.height));
				unsigned int index,index_src;
				for (int x=0; x<range_x; x++)	
				{
					for (int y=0; y<range_y; y++)
					{
						index=(x+(objConfig.width*y))*4;
						index_src=(x+(_sourcewidth*y))*4;
						foreground_tex[index]=RGBA_src[index_src];
						foreground_tex[index+1]=RGBA_src[index_src+1];
						foreground_tex[index+2]=RGBA_src[index_src+2];
						foreground_tex[index+3]=RGBA_src[index_src+3];
					}
				}
			}
		}
	}

	void legendObject::Copy(const FGL_BYTE* RGB_src,const FGL_BYTE* A_src,int _sourcewidth,int _sourceheight)
	{
		DoCopy(RGB_src,A_src,_sourcewidth,_sourceheight);
	}


	void legendObject::DoCopy(const FGL_BYTE* RGB_src,const FGL_BYTE* A_src, int srcWidth,int srcHeight, bool overwrite,int destCornerX,int destCornerY)
	{
		if(foreground_tex && RGB_src)
		{
			int index=0;
			int indexSrc=0;
			int indexDst=0;
			int beginX(destCornerX),beginY(destCornerY),endX(destCornerX+srcWidth),endY(destCornerY+srcHeight);
			if(endX>objConfig.width)
				endX=objConfig.width;
			if(endY>objConfig.height)
				endY=objConfig.height;
			for (int x=beginX; x<endX; x++)	
			{
				for (int y=beginY; y<endY; y++)
				{
						indexDst=(x+(objConfig.width*y))*4;
						index=(x-beginX)+(srcWidth*(y-beginY));
						if(overwrite || !A_src || 255-A_src[index]!=0)
						{
							indexSrc=index*3;
							foreground_tex[indexDst]=RGB_src[indexSrc];
							foreground_tex[indexDst+1]=RGB_src[indexSrc+1];
							foreground_tex[indexDst+2]=RGB_src[indexSrc+2];
							if(A_src)
								foreground_tex[indexDst+3]=255-A_src[index];
							else
								foreground_tex[indexDst+3]=255;
						}
				}
			}
		}
	}

}