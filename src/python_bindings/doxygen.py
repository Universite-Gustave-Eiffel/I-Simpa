#----------------------------------------------------------------------
# I-SIMPA (http://i-simpa.ifsttar.fr). This file is part of I-SIMPA.
#
# I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
# to scientific acoustic simulations.
# Copyright (C) 2007-2014 - IFSTTAR - Judicael Picaut, Nicolas Fortin
#
# I-SIMPA is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# I-SIMPA is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA or 
# see <http://ww.gnu.org/licenses/>
#
# For more information, please consult: <http://i-simpa.ifsttar.fr> or 
# send an email to i-simpa@ifsttar.fr
#
# To contact Ifsttar, write to Ifsttar, 14-20 Boulevard Newton
# Cite Descartes, Champs sur Marne F-77447 Marne la Vallee Cedex 2 FRANCE
# or write to scientific.computing@ifsttar.fr
# ----------------------------------------------------------------------

"""
Fixed and improved version based on "extracting from C++ doxygen documented file Author G.D." and py++ code.

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
"""

class doxygen_doc_extractor:
	"""
	Extracts Doxigen styled documentation from source
	or generates from description.
	"""
	def __init__(self):
		#for caching source
		self.file_name = None
		self.source = None
	#__init__

	def __call__(self, declaration):
		try:
			if self.file_name != declaration.location.file_name:
				self.file_name = declaration.location.file_name
				self.source = open(declaration.location.file_name).readlines()

			find_block_end = False
			doc_lines = []
			for lcount in xrange(declaration.location.line-2, -1, -1):
				line = self.source[lcount]
				if not find_block_end:
					try:
						#print line.rstrip()[-2:]
						if line.rstrip()[-2:] == "*/":
							find_block_end = True
					except:
						pass
				if find_block_end:
					try:
						if line.lstrip()[:2] == "/*":
							find_block_end = False
					except:
						pass
				final_str = self.clear_str(line)
				if not find_block_end and self.is_code(line):
					break
				if final_str:
					doc_lines.insert(0, final_str)
		except:
			pass
		finally:
			if doc_lines:
				final_doc_lines = [ line.replace("\n",r"\n").replace("\r",r"\r") for line in doc_lines ]
				#final_doc_lines.append(doc_lines[-1].replace("\n",""))
				#final_doc_lines.insert(0, self.get_generic_doc(declaration))
				return '\"' + ''.join(final_doc_lines) + '\"'
			else:
				return '\"\"'
				#return '\"'+self.get_generic_doc(declaration)+'\"'
	#__call__()

	#def get_generic_doc(self, declaration):
		#"""
		#Generate call information about function or method
		#"""
		#try:
			#return "Help on %s" % str(declaration)
		#except:
			#pass
		#return ''
	##get_generic_doc()

	def clear_str(self, tmp_str):
		"""
		Replace */! by Space and \breaf, \fn, \param, ...
		"""
		clean = lambda tmp_str, sym, change2 = '': tmp_str.replace(sym, change2)
	
		tmp_str = reduce(clean, [tmp_str, '/', '*', '!', "\\brief", "\\fn",\
		"@brief", "@fn", "@ref", "\\ref", "\"", "\'", "\\c"])
			
		tmp_str = clean(tmp_str, "@param", "Param:")
		tmp_str = clean(tmp_str, "@see", "Voir:")
		tmp_str = clean(tmp_str, "@pre", "Pre-condition:")
		tmp_str = clean(tmp_str, "@throws", "Throws:")
		tmp_str = clean(tmp_str, "@throw", "Throw:")
		tmp_str = clean(tmp_str, "@todo", "A faire:")
		tmp_str = clean(tmp_str, "\param", "Param:")
		tmp_str = clean(tmp_str, "@ingroup", "Group")
		tmp_str = clean(tmp_str, "\ingroup", "Group")
		tmp_str = clean(tmp_str, "@return", "Retourne")
		tmp_str = clean(tmp_str, "\\return", "Retourne")
		tmp_str = clean(tmp_str, "\\warning", "Attention:")
		tmp_str = clean(tmp_str, "\\WARNING", "Attention:")
		tmp_str = clean(tmp_str, "@dot", "[Dot]")
		tmp_str = clean(tmp_str, "@enddot", "[/Dot]")
		tmp_str = clean(tmp_str, "@code", "[Code]")
		tmp_str = clean(tmp_str, "@endcode", "[/Code]")
		tmp_str = clean(tmp_str, "\\~", "")
		return tmp_str.lstrip()
	#clean_str()
	
	def is_code(self, tmp_str):
		"""
		Detect if tmp_str is code
		"""
		try:
			beg = tmp_str.lstrip()[:2]
			return  beg != "//" and beg != "/*"
		except:
			pass
		return False
	#is_code()

#class doxygen_doc_extractor
