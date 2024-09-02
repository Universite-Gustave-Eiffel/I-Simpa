#! /usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function  # compatibilité python 3.0
#te
import os
import sys
# Add I-Simpa folder in lib path
from os.path import dirname
libpath = dirname(os.path.abspath(__file__ + "/../../"))
sys.path.append(libpath)
#libpath='c:\program files\i-simpa'
print("syspath : ",sys.path)


try:
    import libsimpa as ls
except ImportError:
    print("Couldn't find libsimpa in " + libpath, file=sys.stderr)
    exit(-1)

import coreConfig as cc
import shutil
import glob
import time
import math
import xml.dom.minidom  as xmlWriter
from build_recsurf import GetRecepteurSurfList
import numpy as np

try:
    import h5py
except ImportError:
    print("h5py python module not found, cannot read hdf5 files. See www.h5py.org", file=sys.stderr)
    exit(-1)

try:
    import numpy
except ImportError:
    print("numpy python module not found", file=sys.stderr)
    exit(-1)


# todo under windows look for
# HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Octave-4.2.0


# Find octave program utility
def which(program):
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file

    return None


def write_input_files(cbinpath, cmbinpath, outfolder):
    """
     Import 3D model
     XML volume index to python index
    :param cbinpath:
    :param cmbinpath:
    :param sources_lst:
    :param outfolder:
    :return: dictionary of loaded data or None if failed
    """
    ret = {}
    idVolumeIndex = {}

    modelImport = ls.ioModel()
    
    #-------------Code modifié----------------
    if isinstance(modelImport, bytes):
       modelImport = modelImport.decode('utf-8')
    if isinstance(cbinpath, bytes):
        cbinpath = cbinpath.decode('utf-8')
    if isinstance(cmbinpath, bytes):
        cmbinpath = cmbinpath.decode('utf-8')
    #-------------FIN Code modifié------------
    
    if not ls.CformatBIN().ImportBIN(modelImport, cbinpath):
        print("Error can not load %s model !\n" % cbinpath)
        return None

    # Import 3D mesh file builded from tetgen output
    mesh_import = ls.CMBIN().LoadMesh(cmbinpath)
    if not mesh_import:
        print("Error can not load %s mesh model !\n" % cmbinpath)
        return None

    # Write NODES file
    XYZ=[]
    for node in mesh_import.nodes:
        #f.write('{0:>15} {1:>15} {2:>15}'.format(*(node[0], node[1], node[2])) + "\n")
        XYZ.append([node[0], node[1], node[2]])
    XYZ=np.array(XYZ)
    
    ret["model"] = modelImport
    ret["mesh"] = mesh_import
    # Write elements file
    el=[]
    for tetra in mesh_import.tetrahedres:
        volindex = idVolumeIndex.get(tetra.idVolume)
        if volindex is None:
            volindex = len(idVolumeIndex) + 1
            idVolumeIndex[tetra.idVolume] = len(idVolumeIndex) + 1
        #f.write('{0:>6} {1:>6} {2:>6} {3:>6} {4:>6}'.format(*(
        #tetra.vertices[0] + 1, tetra.vertices[1] + 1, tetra.vertices[2] + 1, tetra.vertices[3] + 1,
        #volindex)) + "\n")
        el.append([
        tetra.vertices[0] + 1, tetra.vertices[1] + 1, tetra.vertices[2] + 1, tetra.vertices[3] + 1,
        volindex])
    el=np.array(el)


    return ret,el,XYZ


def write_config_file(coreConf, outputdir):
    with open(os.path.join(outputdir, "Input_parameters.m"), "w") as f:
        f.write("Temperature=%f;\n" % coreConf.const["temperature_celsius"])
        f.write("Humidity=%f;\n" % coreConf.const["humidite"])
        f.write("Pressure=%f;\n" % coreConf.const["pression"])


def run_model(el,XYZ,coreConf,libpath):
    sys.path.append(libpath + '/Lib/import_frequencies')
    import Room_Natural_Frequencies_ao2
    Room_Natural_Frequencies_ao2.main(el,XYZ,coreConf)

def main(call_python=True):
    # find core folder
    scriptfolder = sys.argv[0][:sys.argv[0].rfind(os.sep)] + os.sep
    # Read I-SIMPA XML configuration file
    coreconf = cc.coreConfig(sys.argv[1])

    
    #------------------modif code---------------------
    working_directory = coreconf.paths["workingdirectory"]
    if isinstance(working_directory, bytes):
       working_directory = working_directory.decode('utf-8')
    outputdir = os.path.join(working_directory, "core")
    #------------------FIN modif code-----------------
    
    #outputdir = os.path.join(coreconf.paths["workingdirectory"], "core")
    if not os.path.exists(outputdir):
        os.mkdir(outputdir)
    # Translation CBIN 3D model and 3D tetrahedra mesh into Octave input files
    import_data,el,XYZ = write_input_files(os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["modelName"]), os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["tetrameshFileName"]),
                      outputdir)

    coreconf.recsurf = GetRecepteurSurfList(coreconf, import_data["model"], import_data["mesh"])
    # Copy octave script to working dir
    matscript_folder = os.path.join(os.path.abspath(os.path.join(os.path.realpath(__file__), os.pardir)), "script")
    files = glob.iglob(os.path.join(matscript_folder, "*.py"))
    print(os.path.join(matscript_folder, "*.py"))
    for filep in files:
        if os.path.isfile(filep):
            shutil.copy2(filep, outputdir)
    # Write configuration file
    write_config_file(coreconf, outputdir)

    
    #--------------Modifs code 08/07/2024-------
    script_name = "Room_Natural_Frequencies_ao2.py"
    #script_path = os.path.join("C:/Program Files/I-SIMPA/core/mp_python/Script" , script_name)
    #print("script_path = ", script_path)
    #sys.path.append('C:/ProgramData/anaconda3/Lib/venv/scripts/nt')
    #command = ["--no-window-system", "C:/ProgramData/anaconda3/Lib/venv/scripts/nt"]
    #print("Run " + " ".join(command))
    deb = time.time()
    run_model(el,XYZ,coreconf, libpath)
    print("Execution in %.2f seconds" % ((time.time() - deb) / 1000.))
    #--------------FIN Modifs code--------------
    
    #process_output_files(outputdir, coreconf, import_data, resultsModificationLayers)

if __name__ == '__main__':
    main(sys.argv[-1] != "noexec")
