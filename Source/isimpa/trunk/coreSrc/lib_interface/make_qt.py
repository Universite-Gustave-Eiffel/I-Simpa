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

import os
from os.path import join
def doWalk():
    exludedirs=["Debug","Debug Win32","DebugUnix","LibDebugging","python_bindings"]
    headers=""
    sources=""
    template="""
    # -------------------------------------------------
    # Project created by QtCreator 2010-05-03T09:58:56
    # -------------------------------------------------
    QT -= core \
        gui
    TARGET = lib_interface
    TEMPLATE = lib
    DEFINES += LIB_INTERFACE_LIBRARY
    SOURCES += %(sources)s
    HEADERS += %(headers)s
    INCLUDEPATH = ./
    """
    for root, dirs, files in os.walk('./'):
        for excldir in exludedirs:
            if excldir in dirs:
                dirs.remove(excldir)  # don't visit excldir directories
        for filename in files:
            ext=filename[filename.rfind(".")+1:]
            filepath=join(root,filename)
            filepath=filepath.replace("\\","/")[2:]
            if ext in ["h","hpp"]:
                headers+=filepath+"\\\n"
            elif ext in ["c","cpp"]:
                sources+=filepath+"\\\n"
    sources=sources[:-2]
    headers=headers[:-2]
    print template % {"headers" : headers, "sources": sources}
doWalk()