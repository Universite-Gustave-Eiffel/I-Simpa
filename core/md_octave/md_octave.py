#! /usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function #compatibilité python 3.0
import sys
import os
import coreConfig as cc
import subprocess as sp
import libsimpa as ls
import math
from subprocess import call
import shutil
import glob

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
    
def processFace(tetraface, modelImport, fileOut):
    if tetraface.marker >= 0:
        fileOut.write('{0:>6} {1:>6} {2:>6} {3:>6}'.format(*(tetraface.vertices[0] + 1, tetraface.vertices[1] + 1, tetraface.vertices[2] + 1, modelImport.faces[tetraface.marker].idMat)) + "\n")
        
def writeInputFiles(cbinpath,cmbinpath,materials, sources_lst, outfolder):
    # Import 3D model
    # This model contains the associated material link to the XML file
    modelImport = ls.ioModel()
    if not ls.CformatBIN().ImportBIN( modelImport, cbinpath):
        print("Error can not load %s model !\n" % cbinpath)
        return -1    
    
    # Import 3D mesh file builded from tetgen output
    meshImport = ls.CMBIN().LoadMesh(cmbinpath)
    if not meshImport:
        print("Error can not load %s mesh model !\n" % cmbinpath)
        return -1
        
    # Write NODES file
    with open(outfolder+"scene_nodes.txt", "w") as f:
        for node in meshImport.nodes:
            f.write('{0:>15} {1:>15} {2:>15}'.format(*(node[0], node[1], node[2])) + "\n")
        
    # Write elements file
    with open(outfolder+"scene_elements.txt", "w") as f:
        for tetra in meshImport.tetrahedres:
            f.write('{0:>6} {1:>6} {2:>6} {3:>6} {4:>6}'.format(*(tetra.vertices[0]+1, tetra.vertices[1]+1, tetra.vertices[2]+1, tetra.vertices[3]+1, tetra.idVolume)) + "\n")
        
    # Write tetra face file
    with open(outfolder+"scene_faces.txt", "w") as f:
        for tetra in meshImport.tetrahedres:
            processFace(tetra.getFace(0), modelImport, f)
            processFace(tetra.getFace(1), modelImport, f)
            processFace(tetra.getFace(2), modelImport, f)
            processFace(tetra.getFace(3), modelImport, f)

    # Write boundary material file
    with open(outfolder+"scene_materials_absorption.txt", "w") as f:
        for xmlid, mat in materials.iteritems():
            f.write('{0:>6} '.format(xmlid))
            # for each frequency band
            for freqAbs in mat["q"]:
                f.write(' {0:>6.2g}'.format(freqAbs))            
            #end of line
            f.write("\n")
            
    # Write boundary material transmission file
    with open(outfolder+"scene_materials_transmission.txt", "w") as f:
        for xmlid, mat in materials.iteritems():
            f.write('{0:>6} '.format(xmlid))
            # for each frequency band
            for freqTransm in mat["g"]:
                f.write(' {0:>6.2g}'.format(freqTransm))            
            #end of line
            f.write("\n")
    
    # Write source position and power files
    with open(outfolder+"scene_sources.txt", "w") as f:
        for src in sources_lst:
            f.write('{0:>15} {1:>15} {2:>15} '.format(src.pos[0], src.pos[1], src.pos[2]))
            # for each frequency band
            for spl in src.db:
                f.write(' {0:>6.4g}'.format(spl))            
            #end of line
            f.write("\n")
    
    
def main(callOctave=True):

    #find core folder
    scriptfolder=sys.argv[0][:sys.argv[0].rfind(os.sep)]+os.sep
    #Read I-SIMPA XML configuration file
    coreconf=cc.coreConfig(sys.argv[1])
    outputdir=coreconf.paths["workingdirectory"]
    #Translation CBIN 3D model and 3D tetrahedra mesh into Octave input files
    writeInputFiles(outputdir+coreconf.paths["modelName"], outputdir+coreconf.paths["tetrameshFileName"], coreconf.materials, coreconf.sources_lst ,outputdir)
    # Copy octave script to working dir
    matscript_folder = os.path.join(os.path.abspath(os.path.join(os.path.realpath(__file__), os.pardir)), "script")
    files = glob.iglob(os.path.join(matscript_folder, "*.m"))
    print(os.path.join(matscript_folder, "*.m"))
    for filep in files:
        if os.path.isfile(filep):
            shutil.copy2(filep, outputdir)
    if callOctave:
        # Check if octave program are accessible in path
        octave = which("octave.bat")
        if octave is None:
            print("Octave program not in system path, however input files are created", file=sys.stderr)
        else:
            command = ["octave.bat","--persist", outputdir+"MVCEF3D.m"]
            print("Run " + " ".join(command))
            call(command, cwd=outputdir, shell=True)
if __name__ == '__main__':
    main()
