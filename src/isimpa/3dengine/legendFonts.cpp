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

#include "legendFonts.h"
#include <wx/dcmemory.h>
#include <wx/image.h>
#include <wx/log.h>
#include "last_cpp_include.hpp"


namespace legendRendering
{

	legendText::legendText()
	{
		textForeground.Set(0,0,0);
		textBackground.Set(0,0,0,0);
	}
	void legendText::SetTextForeground(const wxColour& colour)
	{
		textForeground=colour;
	}
	void legendText::SetTextBackground(const wxColour& colour)
	{	
		textBackground=colour;
	}
	void legendText::SetFont(const wxFont& font)
	{
		textFont=font;
	}
	void legendText::InitText(const wxString& text,int maxLength)
	{
		DoAppendText(text,0,0,true,maxLength);
	}
	/*
	void legendText::DoAppendText(const wxString& text,int x,int y, bool overwrite,int maxLength)
	{
		//Todo utiliser uniquement l'espace nécessaire à la chaine de caractère
		wxMemoryDC textDrawer;
		textDrawer.SetBrush( *wxGREEN_BRUSH );
		textDrawer.SetTextBackground(textBackground);
		textDrawer.SetTextForeground(textForeground);
		textDrawer.SetFont(textFont);

		wxSize sizeText;
		if(maxLength==-1 || maxLength<text.size())
			sizeText=textDrawer.GetTextExtent(text);
		else
			sizeText=textDrawer.GetTextExtent(wxString("0",maxLength));		
		if(!foreground_tex)
		{
			legendObject::Init(sizeText.GetWidth(),sizeText.GetHeight());
		}

		unsigned char* color=new unsigned char[objConfig.width*objConfig.height*3];
		unsigned char* alpha=new unsigned char[objConfig.width*objConfig.height];
		memset(color,0,objConfig.width*objConfig.height*3);
		memset(alpha,255,objConfig.width*objConfig.height);

		wxImage emptyImage(objConfig.width,objConfig.height,color,alpha);

		wxBitmap tmpBitmap(emptyImage);
		textDrawer.SelectObject(tmpBitmap);
		if(textBackground.Alpha()==255)
			textDrawer.SetBackgroundMode(wxSOLID);
		else
			textDrawer.SetBackgroundMode(wxTRANSPARENT);
	
		textDrawer.DrawText(text,x,y);
		textDrawer.SelectObject(wxNullBitmap);
		wxImage imageTxt=tmpBitmap.ConvertToImage();
		CopyFont(imageTxt.GetData(),imageTxt.GetAlpha(),overwrite);	
	}
	*/
	void legendText::DoAppendText(const wxString& text,int x,int y, bool overwrite,int maxLength)
	{
		//Todo utiliser uniquement l'espace nécessaire à la chaine de caractère
        wxBitmap bmp(1,1);
		wxMemoryDC textDrawer(bmp);
		textDrawer.SetBrush( *wxGREEN_BRUSH );
		textDrawer.SetTextBackground(textBackground);
		textDrawer.SetTextForeground(textForeground);
		textDrawer.SetFont(textFont);

		wxSize sizeText;
		if(maxLength==-1 || maxLength<text.size())
		{
			sizeText=textDrawer.GetTextExtent(text);
		}else{
			sizeText=textDrawer.GetTextExtent(wxString("0").Pad(maxLength));
		}
		if(!foreground_tex)
		{
			legendObject::Init(sizeText.GetWidth(),sizeText.GetHeight());
		}

		unsigned char* color=new unsigned char[sizeText.GetWidth()*sizeText.GetHeight()*3];
		unsigned char* alpha=new unsigned char[sizeText.GetWidth()*sizeText.GetHeight()];
		memset(color,0,sizeText.GetWidth()*sizeText.GetHeight()*3);
		memset(alpha,255,sizeText.GetWidth()*sizeText.GetHeight());

		wxImage emptyImage(sizeText.GetWidth(),sizeText.GetHeight(),color,alpha);

		wxBitmap tmpBitmap(emptyImage);
		textDrawer.SelectObject(tmpBitmap);
		if(textBackground.Alpha()==255)
			textDrawer.SetBackgroundMode(wxSOLID);
		else
			textDrawer.SetBackgroundMode(wxTRANSPARENT);
	
		textDrawer.DrawText(text,0,0);
		textDrawer.SelectObject(wxNullBitmap);
		wxImage imageTxt=tmpBitmap.ConvertToImage();
		DoCopy(imageTxt.GetData(),imageTxt.GetAlpha(),sizeText.GetWidth(),sizeText.GetHeight(),overwrite,x,y);	
	}
	void legendText::AppendText(const wxString& text,int x,int y)
	{
		/*
		wxFont oldfont=textFont;
		//textFont.SetPointSize(textFont.GetPointSize()+2);
		textFont.SetFamily(
		x-=1;
		y-=1;
		wxPen::set
		wxColour oldcolour=textForeground;
		textForeground=*wxBLACK;
		DoAppendText(text,x,y,false);
		textFont=oldfont;
		textForeground=oldcolour;
		*/
		DoAppendText(text,x,y,false);
	}
	
	wxSize legendText::GetTextSize(const wxString& text)
	{
		wxBitmap bmp( 1, 1 );
		wxMemoryDC textDrawer(bmp);
		textDrawer.SetBrush( *wxGREEN_BRUSH );
		textDrawer.SetTextBackground(textBackground);
		textDrawer.SetTextForeground(textForeground);
		textDrawer.SetFont(textFont);
		wxSize sizeText;
		sizeText=textDrawer.GetTextExtent(text);
		return sizeText;
	}
}