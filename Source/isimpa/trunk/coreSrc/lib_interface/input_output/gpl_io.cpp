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

#include "gpl_io.hpp"
#include <fstream>
#include <iostream>

namespace GPL_IO
{

	GPL_FileExchange::GPL_FileExchange()
	{


	}
	GPL_FileExchange::GPL_FileExchange(const std::string& gpl_filename)
	{
		Read(gpl_filename);
	}


	int GPL_FileExchange::Read(const std::string& gpl_filename)
	{
		std::ifstream gplFile; // On the stack
		gplFile.open( gpl_filename.c_str(),std::ios::in  );
		if(!gplFile.is_open())
			return -1;

		std::string line;
		//////////////////////////////
		//Read of the first line
		std::getline(gplFile,line);
		if(line!="GIMP Palette")
		{
			std::cerr<<"Not a GIMP palette file !"<<std::endl;
			return -2;
		}
		while(std::getline(gplFile,line))
		{
			std::string::size_type posSep=line.find(": ");
			if(posSep != std::string::npos)
			{
				std::string fieldName;
				std::string fieldValue;
				fieldName=line.substr(0,posSep);
				fieldValue=line.substr(posSep+2,std::string::npos);
				this->fieldLst.push_back(t_field(fieldName,fieldValue));
			}else{
				//Finish with fields
				break;
			}
		}
		//////////////////////////////
		//Reading of comments
		while(line.size()>0 && (*line.begin()) ==35)
		{
			if(!std::getline(gplFile,line))
				break;
		}
		//////////////////////////////
		//Reading of colors
		while(line.length()>0)
		{
			t_color newcolor;
			char namecolor[350];
			int r,g,b;
			int retval=sscanf(line.c_str(),"%i %i %i	%350s\n",&r,&g,&b,namecolor);
			if(retval!=EOF && retval!=0)
			{
				newcolor.colorName=std::string(namecolor,350);
				newcolor.lvl[0]=(unsigned short)r;
				newcolor.lvl[1]=(unsigned short)g;
				newcolor.lvl[2]=(unsigned short)b;
				this->colorLst.push_back(newcolor);
			}
			if(!std::getline(gplFile,line))
				break;
		}
		this->managerColorIterator=this->colorLst.begin();
		this->managerFieldIterator=this->fieldLst.begin();
		gplFile.close();
		return 0;
	}
	void GPL_FileExchange::SetToFirstField()
	{
		this->managerFieldIterator=this->fieldLst.begin();
	}
	void GPL_FileExchange::SetToFirstColor()
	{
		this->managerColorIterator=this->colorLst.begin();
	}
	bool GPL_FileExchange::Ok()
	{
		return this->colorLst.size()>0;
	}
	bool GPL_FileExchange::Write(const std::string& gpl_filename)
	{
		std::ofstream gplFile; // On the stack
		gplFile.open( gpl_filename.c_str(),std::ios::out  );
		if(!gplFile.is_open())
			return false;
		gplFile<<"GIMP Palette"<<std::endl;
		for(t_field_iterator itfield=this->fieldLst.begin();itfield!=this->fieldLst.end();itfield++)
		{
			gplFile<<(*itfield).first<<": "<<(*itfield).second<<std::endl;
		}
		gplFile<<"#"<<std::endl;
		for(t_color_iterator itcolor=this->colorLst.begin();itcolor!=this->colorLst.end();itcolor++)
		{
			gplFile<<(*itcolor).lvl[0]<<" "<<(*itcolor).lvl[1]<<" "<<(*itcolor).lvl[2]<<"	"<<(*itcolor).colorName<<std::endl;
		}
		gplFile<<std::endl;
		gplFile.close();
		return true;
	}
	bool GPL_FileExchange::GetFieldByName(const std::string& fieldName, std::string& fieldValue)
	{
		for(t_field_iterator itfield=this->fieldLst.begin();itfield!=this->fieldLst.end();itfield++)
		{
			if((*itfield).first==fieldName)
			{
				fieldValue=(*itfield).second;
				return true;
			}
		}
		return false;
	}
	bool GPL_FileExchange::GetNextField(std::string& fieldName, std::string& fieldValue)
	{
		if(this->managerFieldIterator!=this->fieldLst.end())
		{
			fieldName=(*this->managerFieldIterator).first;
			fieldValue=(*this->managerFieldIterator).second;
			this->managerFieldIterator++;
		}
		return this->managerFieldIterator!=this->fieldLst.end();
	}
	bool GPL_FileExchange::GetNextColor(unsigned short& red,unsigned short& green,unsigned short& blue, std::string& colorName)
	{
		if(this->managerColorIterator!=this->colorLst.end())
		{
			red=(*this->managerColorIterator).lvl[0];
			green=(*this->managerColorIterator).lvl[1];
			blue=(*this->managerColorIterator).lvl[2];
			colorName=(*this->managerColorIterator).colorName;
			this->managerColorIterator++;
		}
		return this->managerColorIterator!=this->colorLst.end();
	}

	unsigned int GPL_FileExchange::GetFieldsCount()
	{
		return this->fieldLst.size();
	}
	unsigned int GPL_FileExchange::GetColorsCount()
	{
		return this->colorLst.size();
	}
	void GPL_FileExchange::RecordNewField(const std::string& fieldName, const std::string& fieldValue)
	{
		this->fieldLst.push_back(t_field(fieldName,fieldValue));
	}
	void GPL_FileExchange::AddNewColor(const unsigned short& red,const unsigned short& green,const unsigned short& blue,const std::string& colorName)
	{
		t_color newcolor;
		newcolor.lvl[0]=red;
		newcolor.lvl[1]=green;
		newcolor.lvl[2]=blue;
		newcolor.colorName=colorName;
		this->colorLst.push_back(newcolor);
	}

}
