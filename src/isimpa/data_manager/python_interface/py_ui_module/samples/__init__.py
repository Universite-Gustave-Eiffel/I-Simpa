# -*- coding: cp1252 -*-
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

from __future__ import print_function #compatibilité python 3.0
import uictrl
import sys

class manager:
    """
        This class make the user able to enable or disable a group of emitters with one click only
    """
    def __init__(self):
        """
            Constructor. Register the two new menu functions
        """
        self.enable_grp_sourcesid=uictrl.application.register_event(self.enable_grp_sources)
        self.disable_grp_sourcesid=uictrl.application.register_event(self.disable_grp_sources)
    def set_grp_src_activation(self,idgrp,newstate):
        grpsrc=uictrl.element(idgrp)
        all_property=grpsrc.getallelementbytype(uictrl.element_type.ELEMENT_TYPE_SCENE_SOURCES_SOURCE_PROPRIETES)
        for prop in all_property:
            uictrl.element(prop).updateboolconfig("enable",newstate)
    def enable_grp_sources(self,idgrp):
        """
            Called by user interface when the user click on the enable menu item
        """
        self.set_grp_src_activation(idgrp,True)
    def disable_grp_sources(self,idgrp):
        """
            Called by user interface when the user click on the disable menu item
        """
        self.set_grp_src_activation(idgrp,False)
    def getmenu(self,typeel,idel,menu):
        """
            Called by the user interface
            The list menu structure contains the current implemented functions.
        """
        submenu=[(uictrl._("Enable"),self.enable_grp_sourcesid),(uictrl._("Disable"),self.disable_grp_sourcesid)]
        menu.insert(2,(uictrl._("All emitters"),submenu))
        menu.insert(2,())
        return True
#register the menu manager class instance.
uictrl.application.register_menu_manager(uictrl.element_type.ELEMENT_TYPE_SCENE_SOURCES, manager())

