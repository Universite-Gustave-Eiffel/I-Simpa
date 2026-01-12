#!/usr/bin/python
# -*- coding: utf-8 -*-

# ----------------------------------------------------------------------
# I-SIMPA (https://i-simpa.univ-gustave-eiffel.fr). This file is part of I-SIMPA.
#
# I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
# to scientific acoustic simulations.
# Copyright (C) UMRAE, CEREMA, Univ Gustave Eiffel - Judicael Picaut, Nicolas Fortin
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
# For more information, please consult: <https://i-simpa.univ-gustave-eiffel.fr> or 
# send an email to i-simpa@ifsttar.fr
#
# To contact Ifsttar, write to Ifsttar, 14-20 Boulevard Newton
# Cite Descartes, Champs sur Marne F-77447 Marne la Vallee Cedex 2 FRANCE
# or write to scientific.computing@ifsttar.fr
# ----------------------------------------------------------------------

import os
import sys
sys.path.append(os.getcwd())
import libsimpa as ls
import sys

def ToList(self):
    """
        Return data using python array
    """
    lstret=[]
    coltypes=list(self.GetTabTypes())
    func_binding={ ls.GABE_OBJECTTYPE_SHORTSTRING : self.ReadColStr,
                   ls.GABE_OBJECTTYPE_INT : self.ReadColInt,
                   ls.GABE_OBJECTTYPE_FLOAT : self.ReadColFloat
                   }
    for idcol in range(0,len(self)):
        lstret.append([ self.GetColTitle(idcol)] + list(func_binding[coltypes[idcol]](idcol)))
    return lstret

setattr(ls.Gabe_rw,"ToList",ToList)

driver = ls.Gabe_rw()

expected = [['', "Particules absorb\xe9es par l'atmosph\xe8re", 'Particules absorb\xe9es par les mat\xe9riaux', 'Particules perdues d\xfb aux boucles infinies', 'Particules perdues d\xfb au maillage incorrect', 'Particules restantes', 'Total'], ['50 Hz', 0, 19999966, 0, 2, 32, 20000000]]

gabe_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "retrocompat_test.gabe")

if not os.path.exists(gabe_path):
    sys.exit("File not exists: " + gabe_path)

if not driver.Load(gabe_path):
    sys.exit("Cannot load gabe file !")


grid = driver.ToList()
grid = [[x.encode('utf-8', errors='surrogateescape').decode('cp1252') if isinstance(x,str) else x for x in ls] for ls in grid]

if not grid == expected:
    exit("Wrong data " + str(grid))

