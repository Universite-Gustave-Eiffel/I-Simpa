#! /usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function  # compatibilité python 3.0

import os
import platform
import sys
# Add I-Simpa folder in lib path
from os.path import dirname
libpath = dirname(os.path.abspath(__file__ + "/../../"))
sys.path.append(libpath)
try:
    import libsimpa as ls
except ImportError:
    print("Couldn't find libsimpa in " + libpath, file=sys.stderr)
    exit(-1)
import numpy as np
import coreConfig as cc
from subprocess import call
import shutil
import glob
import subprocess as sp
import time
import kdtree
import sauve_recsurf_results
import sauve_recponct_results
import math
import codecs
from sound_level_layer import SoundLevelLayer
import xml.dom.minidom  as xmlWriter
from build_recsurf import GetRecepteurSurfList


try:
    import numpy
except ImportError:
    print("numpy python module not found", file=sys.stderr)
    exit(-1)



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

def process_face(tetraface, modelImport, sharedVertices):
    Face=[]
    if tetraface.marker >= 0:
        Face.append([tetraface.vertices[0] + 1, tetraface.vertices[1] + 1, tetraface.vertices[2] + 1,
        modelImport.faces[tetraface.marker].idMat])
        if tetraface.neighbor >= 0:
            sharedVertices.add(tetraface.vertices[0] + 1)
            sharedVertices.add(tetraface.vertices[1] + 1)
            sharedVertices.add(tetraface.vertices[2] + 1)
    return Face


##
# \~english
# This method run the Classical physics core for each source independently and return the results
# @return Dict object with source id in key and sound_level_layer.SoundLevelLayer instance in dict values
def runTC(xmlPathTc, coreconf):
    """
        This method run the Classical physics core for each source independently and return the results
    """
    # TODO option to disable direct field computation
    #if not coreconf.const["ajouter_son_direct"]:
    #    return {}
    # tcpath = os.path.normpath(os.path.join(os.getcwd(), "core", "classical_theory", "classicalTheory.exe"))
    if not coreconf.const["with_direct_sound"]:
        return {}
    bin_name = "classicalTheory.exe" if platform.system() != "Linux" else "classicalTheory"
    tcpath = os.path.normpath(os.path.join(os.getcwd(), "core", "classical_theory", bin_name))
    if not os.path.exists(tcpath):
        print("Cant find classical theory program!\n %s" % tcpath, file=sys.stderr)
        exit()
    # One configuration file per source with different output path
    doc = xmlWriter.parse(xmlPathTc)
    simunode=doc.getElementsByTagName('simulation')[0]
    srclstnode=doc.getElementsByTagName('sources')[0]
    sources=srclstnode.getElementsByTagName('source')
    sub_tc=[]
    rootxml=xmlPathTc[0:xmlPathTc.rfind(os.sep)+1]
    for source in sources:
        srclstnode.removeChild(source)
    for source in sources:
        srclstnode.appendChild(source)
        idsource=source.getAttribute('id')
        s_xmlfile = os.path.join(rootxml, "config_s%s.xml" % idsource)
        s_folder='direct_s%s' % idsource
        if not os.path.exists(os.path.join(rootxml,s_folder)):
            os.mkdir(os.path.join(rootxml,s_folder))
        simunode.setAttribute('output_folder', s_folder)
        simunode.setAttribute('do_angular_weighting','0') #Pas de pondération selon l'angle d'incidence de la source
        writer = codecs.open(s_xmlfile, 'w', encoding="utf-8")
        doc.writexml(writer, encoding="utf-8")
        writer.close()
        sub_tc.append((int(idsource),s_xmlfile,s_folder))
        srclstnode.removeChild(source)

    # For each source we launch classical theory computation
    resultsModificationLayers = {}
    for tc_process in sub_tc:
        print(u"Execute classical theory program :\n%s %s" % (tcpath, tc_process[1]))
        sp.check_call([tcpath, tc_process[1]])
        # os.remove(tc_process[1])
        resultsModificationLayer = SoundLevelLayer()
        resultsModificationLayer.LoadData(tc_process[2], coreconf, ls)
        # os.removedirs(coreconf.paths["workingdirectory"] + tc_process[2] + os.sep)
        resultsModificationLayers[tc_process[0]] = resultsModificationLayer
    return resultsModificationLayers


def write_input_files(cbinpath, cmbinpath, materials, sources_lst, outfolder):
    """
     Import 3D model
     This model contains the associated material link to the XML file
     XML volume index to python index
    :param cbinpath:
    :param cmbinpath:
    :param materials:
    :param sources_lst:
    :param outfolder:
    :return: dictionary of loaded data or None if failed
    """
    ret = {}
    idVolumeIndex = {}
    sharedVertices = set()

    modelImport = ls.ioModel()
    ##########Changement type variables  pour utilisation
    if isinstance(modelImport, bytes):
       modelImport = modelImport.decode('utf-8')
    if isinstance(cbinpath, bytes):
        cbinpath = cbinpath.decode('utf-8')
    if isinstance(cmbinpath, bytes):
        cmbinpath = cmbinpath.decode('utf-8')

    if not ls.CformatBIN().ImportBIN(modelImport, cbinpath):
        print("Error can not load %s model !\n" % cbinpath)
        return None

    # Import 3D mesh file builded from tetgen output
    mesh_import = ls.CMBIN().LoadMesh(cmbinpath)
    if not mesh_import:
        print("Error can not load %s mesh model !\n" % cmbinpath)
        return None

    XYZ=[]
    for node in mesh_import.nodes:
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
        el.append([
        tetra.vertices[0] + 1, tetra.vertices[1] + 1, tetra.vertices[2] + 1, tetra.vertices[3] + 1,
        volindex])
    el=np.array(el)

    # Write tetra face file
    el2Dtypd=[]
    for tetra in mesh_import.tetrahedres:
        el2Dtypd.append(process_face(tetra.getFace(0), modelImport, sharedVertices))
        el2Dtypd.append(process_face(tetra.getFace(1), modelImport, sharedVertices))
        el2Dtypd.append(process_face(tetra.getFace(2), modelImport, sharedVertices))
        el2Dtypd.append(process_face(tetra.getFace(3), modelImport, sharedVertices))
    for k in range(len(el2Dtypd)-1,-1,-1):
        if el2Dtypd[k]==[]:
            el2Dtypd.pop(k)
        else:
            el2Dtypd[k]=el2Dtypd[k][0]
    el2Dtypd=np.array(el2Dtypd)

    # Write boundary material file
    Abs_Mater=[]
    for xmlid, mat in materials.items():
        #f.write('{0:>6} '.format(xmlid))
        Abs_Mater.append([xmlid])
        # for each frequency band
        for freqAbs in reversed(mat["q"]):      #modifs 22/08/2024 remettre à l'endroit la matrice d'absorption. dasn le fichier XML, on vas dans l'ordre décroissant des fréquences
            Abs_Mater[-1].append(freqAbs)
    Abs_Mater=np.array(Abs_Mater)

    # Write boundary material transmission file
    Wall_TL=[]
    for xmlid, mat in materials.items():
        Wall_TL.append([xmlid])
        # for each frequency band
        for freqTransm in reversed(mat["g"]):   #modifs 22/08/2024 idem que juste au dessus
            Wall_TL[-1].append(freqTransm)
    Wall_TL=np.array(Wall_TL)

    # Write source position and power files
    Srce_sonore_I=[]       
    for src in sources_lst:
        Srce_sonore_I.append([src.pos[0], src.pos[1], src.pos[2]])
        # for each frequency band
        for spl in src.db:
            Srce_sonore_I[-1].append(spl)
    Srce_Sonore_I=np.array(Srce_sonore_I)

    # Write shared vertices index
    i_nT=[]
    for ptindex in sharedVertices:
        i_nT.append(ptindex)
    i_nT=np.array(i_nT)

    return el,XYZ,el2Dtypd,Abs_Mater,i_nT,Wall_TL,Srce_Sonore_I,ret


class ReceiverSurf:
    def __init__(self, idrs, faceid, x, y, z):
        self.coords = (x, y, z)
        self.idrs = idrs
        self.isSurfReceiver = True
        self.idrp = None
        self.faceid = faceid
        self.spl = []


    def __iter__(self):
        return self.coords.__iter__()

    def __len__(self):
        return 3

    def __getitem__(self, item):
        return self.coords[item]

    def __str__(self):
        return self.coords.__str__()


class ReceiverPunctual:
    def __init__(self, idrp, x, y, z):
        self.coords = (x, y, z)
        self.isSurfReceiver = False
        self.idrp = idrp
        self.spl = []

    def __iter__(self):
        return self.coords.__iter__()

    def __len__(self):
        return 3

    def __getitem__(self, item):
        return self.coords[item]

    def __str__(self):
        return self.coords.__str__()

def to_vec3(vec):
    return ls.vec3(vec[0], vec[1], vec[2])

def to_array(vec):
    return [vec[0], vec[1], vec[2]]

def square_dist(v1, v2):
    return sum([(v1[axis] - v2[axis])**2 for axis in range(len(v1))])


def schroeder_to_impulse(schroeder):
    # In octave schroeder seems to be truncated so we remove last entry
    return schroeder[:-1] - schroeder[1:]


def process_output_files(outfolder, coreconf, import_data, resultsModificationLayers,nodes,tetrahedrons,w,dt):
    # sound velocity
    c0 = 342.0
    rhoco2 = 1.2 * c0 * c0
    # Read mesh file information from python
    
    #Mise en place quand il y a plusieurs salles pour le post traitement
    if len(tetrahedrons)==1:
        tetrahedrons=tetrahedrons[0]
    elif len(tetrahedrons)==2:
        tetrahedrons=np.concatenate((tetrahedrons[0],tetrahedrons[1]), axis=0)
    else:
        tetrahedrons2=np.concatenate((tetrahedrons[0],tetrahedrons[1]), axis=0)
        for k in range(len(tetrahedrons)-2):
            tetrahedrons2=np.array(tetrahedrons2)
            tetrahedrons2=np.concatenate((tetrahedrons2,np.array(tetrahedrons[2+k])), axis=0)
        tetrahedrons=tetrahedrons2

    # Create spatial index for receivers points
    receivers_index = kdtree.create(dimensions=3)
    # For each surface receiver
    pt_count = 0
    for idrs, surface_receivers in coreconf.recsurf.items():
        # For each vertex of the grid
        for faceid, receiver in enumerate(surface_receivers.GetSquaresCenter()):
            receivers_index.add(ReceiverSurf(idrs, faceid, receiver[0], receiver[1], receiver[2]))
            pt_count += 1
    # For each punctual receiver
    for idrp, rp in coreconf.recepteursponct.items():
        receivers_index.add(ReceiverPunctual(idrp, rp["pos"][0], rp["pos"][1], rp["pos"][2]))
        pt_count += 1

    receivers_index.rebalance()

    # Open data files
    if coreconf.const["stationary"]:
        result_matrix = w
    else:
        result_matrix = w
        coreconf.time_step = dt
    last_perc = 0
    for idTetra in range(0, len(tetrahedrons)):
        verts = [tetrahedrons[idTetra][idvert] - 1 for idvert in range(4)]
        p1 = to_vec3(nodes[verts[0]])
        p2 = to_vec3(nodes[verts[1]])
        p3 = to_vec3(nodes[verts[2]])
        p4 = to_vec3(nodes[verts[3]])
        p = (p1+p2+p3+p4) / 4
        rmax = math.ceil(max([square_dist(p, p1), square_dist(p, p2), square_dist(p, p3), square_dist(p, p4)]))
        # Fetch receivers in the tetrahedron
        nearest_receivers = receivers_index.search_nn_dist([p[0], p[1], p[2]], rmax)
        tetra_values = [[numpy.ones(4) for idvert in range(4)] for idfreq in range(len(coreconf.const["frequencies"]))]
        if len(nearest_receivers) > 0:
            if not coreconf.const["stationary"]:
                tetra_values = [
                    [schroeder_to_impulse(result_matrix[idfreq][verts[idvert],:]) for idvert in range(4)] for
                    idfreq in range(len(coreconf.const["frequencies"]))]
            else:
                tetra_values = [
                    [result_matrix[idfreq][verts[idvert],:] for idvert in range(4)] for
                    idfreq in range(len(coreconf.const["frequencies"]))]
        new_perc = int((idTetra / float(len(tetrahedrons))) * 100)
        if new_perc != last_perc:
            # print("Export receivers %i %%" % new_perc) génère énormément de texte, donc à voir pour le garder
            last_perc = new_perc
        # Compute coefficient of the receiver point into the tetrahedron
        for receiver in nearest_receivers:
            coefficient = get_a_coefficients(to_array(receiver), nodes[verts[0]], nodes[verts[1]], nodes[verts[2]], nodes[verts[3]])
            if coefficient.min() > -1e-6:
                # Point is inside tetrahedron
                for id_freq in range(len(coreconf.const["frequencies"])):
                    # closest freq id using all frequencies
                    current_frequency_id = coreconf.const["allfrequencies"].index(min(coreconf.const["allfrequencies"], key=lambda x: abs(x - coreconf.const["frequencies"][id_freq])))
                    # For each frequency compute the interpolated value
                    interpolated_value = coefficient[0] * tetra_values[id_freq][0] + \
                                         coefficient[1] * tetra_values[id_freq][1] + \
                                         coefficient[2] * tetra_values[id_freq][2] + \
                                         coefficient[3] * tetra_values[id_freq][3]

                    # Compute direct field timestep
                    steps = GetNumStepBySource(to_vec3(receiver), coreconf)
                    # If the receiver belongs to a surface receiver add the value into it
                    if receiver.isSurfReceiver:
                        # Look for sound source
                        if coreconf.const["with_direct_sound"]:
                            for source_id in steps.keys():
                                if source_id in resultsModificationLayers and receiver.idrs in resultsModificationLayers[source_id].recsurf and len(resultsModificationLayers[source_id].recsurf[receiver.idrs][current_frequency_id]) > receiver.faceid:
                                    power = resultsModificationLayers[source_id].recsurf[receiver.idrs][current_frequency_id][receiver.faceid]
                                    if power > 0 and not coreconf.const["stationary"]:
                                        interpolated_value[steps[source_id]] += power / (rhoco2 * 2.5e-3)
                                    else:
                                        interpolated_value += power / (rhoco2 * 2.5e-3)
                        coreconf.recsurf[receiver.idrs].face_power[receiver.faceid].append(
                            interpolated_value * rhoco2 * 2.5e-3)
                    else:
                        # Into a punctual receiver
                        # Look for sound source
                        if coreconf.const["with_direct_sound"]:
                            for source_id in steps.keys():
                                if source_id in resultsModificationLayers and receiver.idrp in resultsModificationLayers[source_id].recp:
                                    power = resultsModificationLayers[source_id].recp[receiver.idrp][current_frequency_id]
                                    if power > 0 and not coreconf.const["stationary"]:
                                        interpolated_value[steps[source_id]] += power / rhoco2
                                    else:
                                        interpolated_value += power / rhoco2
                        coreconf.recepteursponct[receiver.idrp]["power_insta"].append(
                            interpolated_value * rhoco2)
    print("End export receivers values")
    # else:
    #     print("The Python result file is not found %s" % mesh_path, file=sys.stderr)

def get_a_coefficients(p, p1, p2, p3, p4):
    """
        Compute the interpolation coefficient of a point into a tetrahedron
        ex: getACoefficients([2,2,0.2], [1,1,0],[3,2,0], [2,4,0], [2,2.5,3])
        source: Journal of Electronic Imaging / April 2002 / Vol. 11(2) / 161
    :param p: Any point (x,y,z)
    :param p1: p1 of tetrahedron (x,y,z)
    :param p2: p2 of tetrahedron (x,y,z)
    :param p3: p3 of tetrahedron (x,y,z)
    :param p4: p4 of tetrahedron (x,y,z)
    :return (a1,a2,a3,a4) coefficients. If point is inside of tetrahedron so all coefficient are greater than 0
    """
    left_mat = numpy.append(numpy.swapaxes(numpy.array([p1, p2, p3, p4]), 0, 1), numpy.ones((1, 4)), axis=0)
    right_mat = numpy.append(numpy.reshape(p, (3, 1)), [1])
    return numpy.dot(numpy.linalg.inv(left_mat), right_mat)


def GetNumStepBySource(pos, coreconf):
    """
        Return the time step number of the incoming impulse for each sound source
    """
    ret_tab = {}
    if coreconf.const["with_direct_sound"]:
        for src in coreconf.sources_lst:
            dist = (pos - src.pos).length()
            ret_tab[src.id] = int(math.floor(dist / (coreconf.const["cel"] * coreconf.const['timestep'])))
    return ret_tab


def run_model(el,XYZ,el2Dtypd,Abs_Mater,i_nT,Wall_TL,Srce_Sonore_I,coreConf):
    import Diffusion_MD_fil_ao3
    nodes,tetrahedrons,w,dt=Diffusion_MD_fil_ao3.main(el,XYZ,el2Dtypd,Abs_Mater,i_nT,Wall_TL,Srce_Sonore_I,coreConf)
    return nodes,tetrahedrons,w,dt

def main(call_python=True):
    # find core folder
    scriptfolder = sys.argv[0][:sys.argv[0].rfind(os.sep)] + os.sep
    # Read I-SIMPA XML configuration file
    print("paramètres : ",sys.argv[1])
    coreconf = cc.coreConfig(sys.argv[1])
    # Get direct field values using external computation core
    resultsModificationLayers = runTC(sys.argv[1], coreconf)
    
    working_directory = coreconf.paths["workingdirectory"]
    if isinstance(working_directory, bytes):
       working_directory = working_directory.decode('utf-8')
    outputdir = os.path.join(working_directory, "core")
    
    if not os.path.exists(outputdir):
        os.mkdir(outputdir)
    # Translation CBIN 3D model and 3D tetrahedra mesh into Octave input files
    el,XYZ,el2Dtypd,Abs_Mater,i_nT,Wall_TL,Srce_Sonore_I, import_data = write_input_files(os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["modelName"]), os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["tetrameshFileName"]),
                  coreconf.materials, coreconf.sources_lst, outputdir)
    coreconf.recsurf = GetRecepteurSurfList(coreconf, import_data["model"], import_data["mesh"])
    # Copy octave script to working dir
    matscript_folder = os.path.join(os.path.abspath(os.path.join(os.path.realpath(__file__), os.pardir)), "script")
    files = glob.iglob(os.path.join(matscript_folder, "*.py"))
    print(os.path.join(matscript_folder, "*.py"))
    for filep in files:
        if os.path.isfile(filep):
            shutil.copy2(filep, outputdir)

    deb = time.time()
    print("Execution in %.2f seconds" % ((time.time() - deb) / 1000.))
    nodes,tetrahedrons,w,dt=run_model(el,XYZ,el2Dtypd,Abs_Mater,i_nT,Wall_TL,Srce_Sonore_I,coreconf)
    process_output_files(outputdir, coreconf, import_data, resultsModificationLayers,nodes,tetrahedrons,w,dt)
    sauve_recsurf_results.SauveRecepteurSurfResults(coreconf)
    sauve_recponct_results.SauveRecepteurPonctResults(coreconf)
    print("fin test")


if __name__ == '__main__':
    main(sys.argv[-1] != "noexec")