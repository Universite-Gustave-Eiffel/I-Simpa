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

#include <list>
#include <string>

/**
 * @file gpl_io.hpp
 * @brief Gimp Palette file reader
 * With this class you can write or read Gimp Palette with the gpl extension. This class was created from scratch and doesn't come from gimp sources.
 */

#ifndef __GPL_FILE_EXCHANGE_
#define __GPL_FILE_EXCHANGE_

namespace GPL_IO
{
	/**
	 * @brief Gimp Palette file reader
	 * With this class you can write or read Gimp Palette with the gpl extension. This class was created from scratch and doesn't come from gimp sources.
	 */
	class GPL_FileExchange
	{
	private:
		struct t_color
		{
			unsigned short lvl[3]; /*!< Red Green Blue [0-255,0-255,0-255] */
			std::string colorName;

		};
		typedef std::pair<std::string,std::string> t_field;
		std::list<t_color> colorLst;
		std::list<t_field> fieldLst;

		typedef std::list<t_field>::iterator t_field_iterator;
		typedef std::list<t_color>::iterator t_color_iterator;

		t_color_iterator managerColorIterator;
		t_field_iterator managerFieldIterator;
	public:
		GPL_FileExchange();
		/**
		 * Constructor, use it for open a gpl file
		 */
		GPL_FileExchange(const std::string& gpl_filename);

		/**
		 * Read the gpl file
		 * @param gpl_filename Path to the file. Specify the full filename with extension.
		 * @return
		 * - 0 Success
		 * - -1 Can't open file
		 * - -2 Not a GIMP palette file !
		 */
		int Read(const std::string& gpl_filename);
		/**
		 * @return True if the class is not empty.
		 */
		bool Ok();

		/**
		 * Write the gpl file
		 * @param gpl_filename Path to the file. Specify the full filename with extension.
		 * @return True on success
		 */
		bool Write(const std::string& gpl_filename);

		/**
		 * Retrieve data of the named field
		 * @param[in] fieldName The label of the field
		 * @param[out] fieldValue The value of the field
		 * @return True if this field exist
		 */
		bool GetFieldByName(const std::string& fieldName, std::string& fieldValue);
		/**
		 * Set to the first field
		 * @see GetNextField
		 */
		void SetToFirstField();
		/**
		 * Set to the first color
		 * @see GetNextColor
		 */
		void SetToFirstColor();
		/**
		 * Retrieve data of the next field
		 * @param[out] fieldName The label of the field
		 * @param[out] fieldValue The value of the field
		 * @return True if there is another field after
		 */
		bool GetNextField(std::string& fieldName, std::string& fieldValue);
		/**
		 * Retrieve data of the next color
		 * @param[out] red Color compound [0-255]
		 * @param[out] green Color compound [0-255]
		 * @param[out] blue Color compound [0-255]
		 * @param[out] colorName Color name
		 * @return True if there is another color after
		 */
		bool GetNextColor(unsigned short& red,unsigned short& green,unsigned short& blue, std::string& colorName);
		/**
		 * @return The number of fields.
		 */
		unsigned int GetFieldsCount();
		/**
		 * @return The number of colors in the palette
		 */
		unsigned int GetColorsCount();

		/**
		 * Add another field
		 * @param[in] fieldName The label of the field
		 * @param[in] fieldValue The value of the field
		 */
		void RecordNewField(const std::string& fieldName, const std::string& fieldValue);
		/**
		 * Record a new color
		 * @param[in] red Color compound [0-255]
		 * @param[in] green Color compound [0-255]
		 * @param[in] blue Color compound [0-255]
		 * @param[in] colorName Color name
		 */
		void AddNewColor(const unsigned short& red,const unsigned short& green,const unsigned short& blue,const std::string& colorName="Untitled");

	};

}
#endif
