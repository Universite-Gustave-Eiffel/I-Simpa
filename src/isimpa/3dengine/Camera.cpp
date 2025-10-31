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


/*************************************
* Class Camera
*
* Contrôle de la camera du visualisateur
* Differend type de camera, première personne et elliptique(autour et tourné vers un point)
*************************************/

#include "Camera.h"
#include <math.h>
#include "last_cpp_include.hpp"

CameraControler::CameraControler()
:arcBall(640,480)
{
}



void CameraControler::Init(int cameratype,int *wWidth,int *wHeight,vec3 *trans,vec3 *rot,vec3 *focal,vec3 *m_trans,vec3 *m_rot)
{
	camtype=cameratype;
	Camera_Trans=trans;
	Camera_Rot=rot;
	Camera_Focal=focal;
	Model_Trans=m_trans;
	Model_Rot=m_rot;
	windowHeight=wHeight;
	windowWidth=wWidth;
	this->ResetCameraPos();
}

void CameraControler::ResetCameraPos()
{
	Camera_Trans->set(0.00049605915f,0.039786763f,1.7203468f);
	Camera_Rot->set(0.0, 1.0f, 0.0);
	Camera_Focal->set(0.00027848946f,0.022336427f,2.7201939f);
	Model_Trans->set(0.0, 0.0, 0.0);
	Model_Rot->set(0.0, 0.0, 0.0);
}

void CameraControler::ThirdCameraMove(int ansx,int ansy,int nouvx,int nouvy)
{
	if(camtype==_CAMERA_ELLIP)
	{
		this->Model_Trans->y += (float)(ansy - nouvy)*_CAMERA_ELLIP_SENSIBILITE;
		this->Model_Trans->x += (float)(nouvx - ansx)*_CAMERA_ELLIP_SENSIBILITE;
	}
}
void CameraControler::FirstCameraMove(int ansx,int ansy,int nouvx,int nouvy) //bouton gauche souris
{
	if(camtype==_CAMERA_ELLIP)
	{

		vec2 mouvepos((float)(ansx - nouvx),(float)(nouvy - ansy));
		int sign = (ansx - nouvx)+(nouvy - ansy) < 0.0 ? 1 : -1;
        this->Model_Trans->z += mouvepos.length()*_CAMERA_ELLIP_SENSIBILITE*sign;

	}else if(camtype==_CAMERA_FP)
	{
		this->RotationZX((nouvy-ansy)*_CAMERA_FP_SENSIBILITE);
		this->RotationY((nouvx-ansx)*_CAMERA_FP_SENSIBILITE);
	}
}

const CameraControler::t_cameraConfig CameraControler::GetCameraConfiguration()
{
	t_cameraConfig config={*Camera_Trans,*Camera_Rot,*Camera_Focal,*Model_Trans,*Model_Rot};
	return config;
}


void CameraControler::TranslationCameraFace(float distance)
{
	 vec3 Focalisation(*Camera_Focal);
	 vec3 Position(*Camera_Trans);

     float norme;
     norme = sqrt( (Focalisation.x-Position.x)*
                   (Focalisation.x-Position.x)+
                   (Focalisation.y-Position.y)*
                   (Focalisation.y-Position.y)+
                   (Focalisation.z-Position.z)*
                   (Focalisation.z-Position.z));

	 Camera_Focal->set((distance/norme)*(Position.x-Focalisation.x)+Focalisation.x,
					  (distance/norme)*(Position.y-Focalisation.y)+Focalisation.y,
					  (distance/norme)*(Position.z-Focalisation.z)+Focalisation.z);

     vec3 D;
     norme = sqrt( (Focalisation.x-Position.x)*
                   (Focalisation.x-Position.x)+
                   (Focalisation.y-Position.y)*
                   (Focalisation.y-Position.y)+
                   (Focalisation.z-Position.z)*
                   (Focalisation.z-Position.z));
     D.x = (distance/norme)*(Position.x-Focalisation.x)+Position.x;
     D.y = (distance/norme)*(Position.y-Focalisation.y)+Position.y;
     D.z = (distance/norme)*(Position.z-Focalisation.z)+Position.z;

     Camera_Trans->set(D.x,D.y,D.z);
}

void CameraControler::TranslationCameraCote(float distance)
{


}

void CameraControler::SecondCameraMove(int ansx,int ansy,int nouvx,int nouvy)
{
	if(camtype==_CAMERA_ELLIP)
	{

		this->Model_Rot->x += ((nouvy - ansy)/((float)*this->windowHeight))*200;
        this->Model_Rot->y += ((nouvx - ansx)/((float)*this->windowWidth))*200;
		//this->Model_Rot->normalize();
	}else if(camtype==_CAMERA_FP)
	{
		if((nouvy-ansy)!=0.0)
			this->TranslationCameraFace((nouvy-ansy)*-_CAMERA_FP_SENSIBILITE_ZOOM);

		if((nouvx-ansx)!=0.0)
			this->RotationY((nouvx-ansx)*_CAMERA_FP_SENSIBILITE*3);
	}
}

bool CameraControler::KeyDownEvent(int keyCode)
{
	const int speed=5;
	switch(keyCode)
	{
		case CameraControler::KEYCODE_UP:
			this->SecondCameraMove(0,0,0,-1*speed);
			break;
		case CameraControler::KEYCODE_DOWN:
			this->SecondCameraMove(0,0,0,1*speed);
			break;
		case CameraControler::KEYCODE_LEFT:
			this->SecondCameraMove(0,0,-1*speed,0);
			break;
		case CameraControler::KEYCODE_RIGHT:
			this->SecondCameraMove(0,0,1*speed,0);
			break;
	}
	return true;
}

//Retourne vrai si le rendu doit être rafraichie
bool CameraControler::MouseEvent(bool leftdown,bool midledown,bool rightdown, int x,int y,int rotation)
{
    static int leftdragging(0),rightdragging(0),midledragging(0);
    static int last_x(0), last_y(0);
	bool needRedraw=false;
	if(rotation!=0)
	{
		needRedraw=false;
	}
    if(leftdown)
    {
        if(!leftdragging)
        {
            leftdragging = 1;
        }
        else
        {
            this->FirstCameraMove(last_x,last_y,x,y);
			needRedraw=true;
        }
        last_x = x;
        last_y = y;
    }
    else
        leftdragging = 0;

    if(rightdown)
    {
        if(!rightdragging)
        {
            rightdragging = 1;
        }
        else
        {
            this->SecondCameraMove(last_x,last_y,x,y);
			needRedraw=true;
        }
        last_x = x;
        last_y = y;
    }
    else
        rightdragging = 0;
    if(midledown)
    {
        if(!midledragging)
        {
            midledragging = 1;
        }
        else
        {
            this->ThirdCameraMove(last_x,last_y,x,y);
			needRedraw=true;
        }
        last_x = x;
        last_y = y;
    }
    else
        midledragging = 0;
	return needRedraw;

}

void CameraControler::changeCameraMode(int cameraMode)
{
	this->ResetCameraPos();
	camtype=cameraMode;
}

int CameraControler::GetCameraMode()
{
	return camtype;
}
void CameraControler::RotationY(float angle) {
     angle = angle/180.f*(float)M_PI;
	 vec3 Focalisation(*Camera_Focal);
	 vec3 Position(*Camera_Trans);
	 Camera_Trans->x=((Position.x-Focalisation.x)*cosf(angle)-(Position.z-Focalisation.z)*sinf(angle))+Focalisation.x;
	 Camera_Trans->z=((Position.x-Focalisation.x)*sinf(angle)+(Position.z-Focalisation.z)*cosf(angle))+Focalisation.z;
}

void CameraControler::RotationZX(float angle) {
     angle = angle/180.f*(float)M_PI;
	 vec3 Focalisation(*Camera_Focal);
	 vec3 Position(*Camera_Trans);
     vec3 M;
     vec3 P;
     float t, p, r;
     P.x = Position.x - Focalisation.x;
     P.y = Position.z - Focalisation.z;
     P.z = Position.y - Focalisation.y;
     r = sqrt((P.x*P.x) + (P.y*P.y) + (P.z*P.z));
     p = atanf(P.y/P.x);
     t = acosf(P.z/r);
     if (P.x<=0) {
        if (P.y>=0)
           p += M_PI;
        else
           p += -M_PI;
     }
     t += angle;
     P.x = r*sinf(t)*cosf(p);
     P.y = r*sinf(t)*sinf(p);
     P.z = r*cosf(t);
     P.x += Focalisation.x;
     P.y += Focalisation.z;
     P.z += Focalisation.y;
	 Camera_Trans->set(P.x,P.z,P.y);
}
