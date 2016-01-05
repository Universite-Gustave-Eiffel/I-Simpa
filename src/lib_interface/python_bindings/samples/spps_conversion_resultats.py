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

# -*- coding: cp1252 -*-

# Titre: Script de conversion en fichiers CSV
# Description: Script de conversion des fichiers de sortie créés par SPPS en fichier CSV
# Date: Avril 2009
# Auteur: N. Fortin et J. Picaut
# Contact: Judicael.Picaut@lcpc.fr

# Chargement des librairies
import os
import libsimpa as ls

def GabeToCsv(filepath,csvpath):
    """
        Converti un fichier GABE (Generic Array Binary Exchange) en format CSV (Comma Separated Values)
    """
    # Instanciation du lecteur
    reader=ls.Gabe_rw()
    # Lecture du fichier gabe
    if reader.Load(filepath):
        # Conversion en liste
        data=reader.ToList()
        # Rotation des données (les colonnes deviennent des lignes)
        data=zip(*data)
        # Ecriture des données
        fich=open(csvpath,'w')
        for line in data:
            firstcol=True
            for col in line:
                if not firstcol:
                    fich.write(",")
                else:
                    firstcol=False
                fich.write(str(col))    # Ecriture de la cellule et virgule
            fich.write("\n")            # Retour à la ligne
        fich.close()

