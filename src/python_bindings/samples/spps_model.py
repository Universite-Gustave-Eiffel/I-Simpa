#----------------------------------------------------------------------
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

# -*- coding: cp1252 -*-
import libsimpa as ls

##############################################
# Tableaux
 
#D�finition des 8 sommets du cube           
sommets=[   [5.0, 0.0, 0.0],
            [0.0, 0.0, 0.0],
            [0.0, 5.0, 0.0],
            [5.0, 5.0, 0.0],
            [0.0, 5.0, 5.0],
            [5.0, 5.0, 5.0],
            [0.0, 0.0, 5.0],
            [5.0, 0.0, 5.0]
        ]
#D�finition des 12 faces triangulaire du cube
# [ sommetA, sommetB, sommetC, idencombrement, idmateriau, idrecepteursurf ]
faces=  [   [0, 1, 2, -1, 66, -1],
        [0, 2, 3, -1, 66, -1],
        [2, 4, 5, -1, 100, -1],
        [2, 5, 3, -1, 100, -1],
        [2, 6, 4, -1, 100, -1],
        [2, 1, 6, -1, 100, -1],
        [1, 0, 7, -1, 100, -1],
        [6, 1, 7, -1, 100, -1],
        [0, 3, 5, -1, 100, -1],
        [7, 0, 5, -1, 100, -1],
        [7, 5, 4, -1, 66, -1],
        [6, 7, 4, -1, 66, -1]
        ]
############################################
## Cr�ation du mod�le 3D

def CreerModele(filepath):
    """
        Cr�ation du mod�le 3D
    """
    model=ls.ioModel()
    #Ajout des sommets
    for sommet in sommets:
        model.vertices.append(ls.t_pos(sommet[0],sommet[1],sommet[2]))
    #Ajout des faces
    for face in faces:
        newface=ls.ioFace()
        newface.a=face[0]
        newface.b=face[1]
        newface.c=face[2]
        newface.idEn=face[3]
        newface.idMat=face[4]
        newface.idRs=face[5]
        model.faces.append(newface)
    #sauvegarde du mod�le
    ls.CformatBIN().ExportBIN(filepath,model)
