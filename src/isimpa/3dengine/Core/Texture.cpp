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
#include "Texture.h"
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/log.h>
#include "3dengine/tools/opengl_test.hpp"
#include "UtfConverter.h"
#include "last_cpp_include.hpp"
#include <cmath>

CTexture::CTexture()
{

}

CTexture::~CTexture()
{
	Destroy();
	this->_pTexInf.clear();
}
void CTexture::ReloadAllTextures()
{
	std::vector<std::string> texFiles;
	texFiles.reserve(this->_pTexInf.size());
	for(long i=0; i < this->_pTexInf.size() ;i++)
	{
		if(this->_pTexInf[i].id != 0)
		{
			texFiles.push_back(this->_pTexInf[i].path);
		}
	}
	Destroy();
	this->_pTexInf.clear();
	for(std::vector<std::string>::iterator ittext=texFiles.begin();ittext!=texFiles.end();ittext++)
		Load(*ittext);


}
void CTexture::Destroy()
{
	for(long i=0; i < this->_pTexInf.size() ;i++)
		if(this->_pTexInf[i].id != 0)
			glDeleteTextures(1,&this->_pTexInf[i].id);
}
long CTexture::Load(const std::string& filename)
{

	wxFileName texFilePath(filename);
	const std::string texFileName(WXSTRINGTOSTDSTRING(texFilePath.GetFullName()));
	//if(texFileName!="DBSS3.jpg")
	//	return -1;
	TextureInfo Tex;
	Tex.path=filename;
	long id = this->GetId(texFileName);

	if (id < this->_pTexInf.size()) return id;


	wxImage textureimg( filename , wxBITMAP_TYPE_ANY );

	if(!textureimg.IsOk())
        return -1;
	
	

	Tex.name   = texFileName;
	Tex.bpp    = 3;
	Tex.width  = textureimg.GetWidth();
	Tex.height = textureimg.GetHeight();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	GLint maxsize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxsize);
	maxsize= maxsize < 1024 ? maxsize : 1024; //Limit to 1024 pixels
	if(!(Tex.width==pow(2,floor(log((double)Tex.width)/log(2.f))) && Tex.height==pow(2,floor(log((double)Tex.height)/log(2.f))) && Tex.height<=maxsize && Tex.width<=maxsize))
	{
		//If the texture need to be resized
		double ratio=(double)Tex.width/(double)Tex.height;
		unsigned int newWidth= Tex.width< maxsize ? Tex.width:maxsize;
		unsigned int newHeight= Tex.height< maxsize ? Tex.height:maxsize;
		//On redimensionne la texture en puissance de 2 afin qu'elle soit compatible avec tout les systèmes
		float widthnearestpowerof2;
		float heightnearestpowerof2;
		vec2 factor;
		ivec2 firstpasteCoords;
		ivec2 pasteCoords;
		if(Tex.width>Tex.height)
		{
			//Width is greater
			widthnearestpowerof2=pow(2,floor(log((double)newWidth)/log(2.f)));
			heightnearestpowerof2=pow(2,floor(log(widthnearestpowerof2/ratio)/log(2.f)));
			textureimg.Rescale(widthnearestpowerof2,heightnearestpowerof2);
		}else{
			//Height is greater
			heightnearestpowerof2=pow(2,floor(log((double)newHeight)/log(2.f)));
			widthnearestpowerof2=pow(2,floor(log(heightnearestpowerof2*ratio)/log(2.f)));
			textureimg.Rescale(widthnearestpowerof2,heightnearestpowerof2);
			pasteCoords.set((int)(heightnearestpowerof2*ratio)+1,0);
		}
		Tex.width  = textureimg.GetWidth();
		Tex.height = textureimg.GetHeight();
	}
	textureimg=textureimg.Mirror(false);
	unsigned char* _Pixels=textureimg.GetData();
	glGenTextures(1,&Tex.id);
	this->_pTexInf.push_back(Tex);
	glBindTexture(GL_TEXTURE_2D, Tex.id);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);


	GLfloat largest_supported_anisotropy;
	if(gltest::gltIsExtSupported("GL_EXT_texture_filter_anisotropic"))
	{
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
	}



	// A debugger Depassement pile mémoire
	int ret=gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Tex.width, Tex.height, GL_RGB, GL_UNSIGNED_BYTE, _Pixels);
	if(ret==0)
	{
		return this->_pTexInf.size() - 1;	
	}else{
		const GLubyte *mess=gluErrorString(ret);
		wxLogError(wxString(wxGetTranslation("OpenGL error :")) + wxString(mess));
		return -1;
	}
	
}

void CTexture::GetTransformation(const long& id,vec2& scale,vec2& translation)
{
	if(id < 0 || this->_pTexInf.size()<=id)
		return;
	scale=this->_pTexInf[id].scale;
	translation=this->_pTexInf[id].translation;
}
bool CTexture::SetTex(long id)
{
	if(id < 0 || this->_pTexInf.size()<=id)
		return false;
	glBindTexture(GL_TEXTURE_2D, this->_pTexInf[id].id);
	glEnable(GL_TEXTURE_2D);
	return true;
}

long CTexture::GetId(const std::string& name)
{
	for(long i=0; i < this->_pTexInf.size(); i++)
		if (this->_pTexInf[i].name==name)
			return i;
	return -1;
}

void CTexture::FllipVerticale()
{
	/*
	unsigned char pix;
	long y = (this->_Tex.height >> 1) - 1;
	for(long l = this->_Tex.width*this->_Tex.bpp; y >= 0 ;y--)
	{
		for(long x = 0; x < l ;x++)
		{
			pix = this->_Pixels[y*l + x];
			this->_Pixels[y*l + x] = this->_Pixels[(this->_Tex.height - 1 - y)*l + x];
			this->_Pixels[(this->_Tex.height - 1 - y)*l + x] = pix;
		}
	}
	*/
}
