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

#include "GL/opengl_inc.h"
#include "Material.h"
#include "manager/smart_ptr.h"
#include "last_cpp_include.hpp"

extern CTexture *textures;

unsigned long CMaterial::Create(char *name,const vec4 &colorA, vec4 &colorD, vec4 &colorS) 
{
	long id = this->GetId(name);
	if(id < this->_pMaterial.size()) return id;

	SMaterial mTmp;
	mTmp.Name = name;
	mTmp.ColorA = colorA;
	mTmp.ColorD = colorD;
	mTmp.ColorS = colorS;
	mTmp.idTex = -1;
	this->_pMaterial.push_back(mTmp);
	return id;
}

unsigned long CMaterial::Create(const SMaterial *strm) 
{
	long id = this->GetId(strm->Name.c_str());
	if(id < this->_pMaterial.size()) return id;
	this->_pMaterial.push_back(*strm);
	return id;
}

unsigned long CMaterial::GetId(const std::string& name) 
{	

	long i=0;
	for(i=0; i < this->_pMaterial.size(); i++)
		if(!this->_pMaterial[i].Name.compare(name))
			break;
	return i;
}

unsigned long CMaterial::SetColor(unsigned long m,float forcetransparency) 
{	
	if(m >= this->_pMaterial.size()) m=0;

	if(this->_pMaterial.size()==0)
		return 0;

	vec4 couleur_ambient(this->_pMaterial[m].ColorA);
	vec4 couleur_diffuse(this->_pMaterial[m].ColorD);
	vec4 couleur_specular(this->_pMaterial[m].ColorS);
	

	if(forcetransparency>-1) //si l'on force une transparence on change la valeur alpha de la matière
	{
		couleur_ambient.a=forcetransparency;
		couleur_diffuse.a=forcetransparency;
		couleur_specular.a=forcetransparency;
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, couleur_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, couleur_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,couleur_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, this->_pMaterial[m].Shininess);
	if(this->_pMaterial[m].Transparency > 0.0f || forcetransparency>-1) 
		glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	return m;
}

unsigned long CMaterial::SetMat(unsigned long m) 
{	
	if(m >= this->_pMaterial.size()) return 0;
	if(this->_pMaterial[m].Transparency > 0.0f) 
		glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
	if(this->_pMaterial[m].idTex != -1)
	{
		textures->SetTex(this->_pMaterial[m].idTex);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vec4(1.,1.,1.,1.));
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vec4(1.,1.,1.,1.));
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,vec4(1.,1.,1.,1.));
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, 1.);
	}else{
		glDisable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, this->_pMaterial[m].ColorA);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, this->_pMaterial[m].ColorD);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,this->_pMaterial[m].ColorS);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, this->_pMaterial[m].Shininess);
	}
	return m;
}

bool CMaterial::HasTexture(unsigned long m)
{
	if(m >= this->_pMaterial.size())
		return false;
	if(this->_pMaterial[m].idTex != -1)
		return true;
	else
		return false;
}

long CMaterial::GetTextureId(unsigned long m)
{
	if(m >= this->_pMaterial.size())
		return -1;
	return this->_pMaterial[m].idTex;
}