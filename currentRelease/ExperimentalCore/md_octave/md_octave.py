#! /usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function  # compatibilité python 3.0

import os
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

def process_face(tetraface, modelImport, sharedVertices, fileOut):
    if tetraface.marker >= 0:
        fileOut.write('{0:>6} {1:>6} {2:>6} {3:>6}'.format(*(
        tetraface.vertices[0] + 1, tetraface.vertices[1] + 1, tetraface.vertices[2] + 1,
        modelImport.faces[tetraface.marker].idMat)) + "\n")
        if tetraface.neighbor >= 0:
            sharedVertices.add(tetraface.vertices[0] + 1)
            sharedVertices.add(tetraface.vertices[1] + 1)
            sharedVertices.add(tetraface.vertices[2] + 1)


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
    tcpath = os.path.normpath(os.path.join(os.getcwd(), "core", "classical_theory", "classicalTheory.exe"))

    if not os.path.exists(tcpath):
        print("Cant find classical theory program!\n %s" % tcpath, file=sys.stderr)
        exit()
    # One configuration file per source with different output path
    doc = xmlWriter.parse(xmlPathTc)
    simunode=doc.getElementsByTagName('simulation')[0]
    freqnode = doc.createElement("freq_enum")
    for freq in coreconf.const["allfrequencies"]:
        bfreq = doc.createElement("bfreq")
        bfreq.setAttribute("freq", str(freq))
        bfreq.setAttribute("docalc", "1" if 100 <= freq <= 5000 else "0")
        freqnode.appendChild(bfreq)
    simunode.appendChild(freqnode)
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
     XML volume index to octave index
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
    if not ls.CformatBIN().ImportBIN(modelImport, cbinpath):
        print("Error can not load %s model !\n" % cbinpath)
        return None

    # Import 3D mesh file builded from tetgen output
    mesh_import = ls.CMBIN().LoadMesh(cmbinpath)
    if not mesh_import:
        print("Error can not load %s mesh model !\n" % cmbinpath)
        return None

    # Write NODES file
    with open(os.path.join(outfolder, "scene_nodes.txt"), "w") as f:
        for node in mesh_import.nodes:
            f.write('{0:>15} {1:>15} {2:>15}'.format(*(node[0], node[1], node[2])) + "\n")

    ret["model"] = mesh_import
    ret["mesh"] = mesh_import
    # Write elements file
    with open(os.path.join(outfolder, "scene_elements.txt"), "w") as f:
        for tetra in mesh_import.tetrahedres:
            volindex = idVolumeIndex.get(tetra.idVolume)
            if volindex is None:
                volindex = len(idVolumeIndex) + 1
                idVolumeIndex[tetra.idVolume] = len(idVolumeIndex) + 1
            f.write('{0:>6} {1:>6} {2:>6} {3:>6} {4:>6}'.format(*(
            tetra.vertices[0] + 1, tetra.vertices[1] + 1, tetra.vertices[2] + 1, tetra.vertices[3] + 1,
            volindex)) + "\n")

    # Write tetra face file
    with open(os.path.join(outfolder,"scene_faces.txt"), "w") as f:
        for tetra in mesh_import.tetrahedres:
            process_face(tetra.getFace(0), modelImport, sharedVertices, f)
            process_face(tetra.getFace(1), modelImport, sharedVertices, f)
            process_face(tetra.getFace(2), modelImport, sharedVertices, f)
            process_face(tetra.getFace(3), modelImport, sharedVertices, f)

    # Write boundary material file
    with open(os.path.join(outfolder,"scene_materials_absorption.txt"), "w") as f:
        for xmlid, mat in materials.iteritems():
            f.write('{0:>6} '.format(xmlid))
            # for each frequency band
            for freqAbs in mat["q"]:
                f.write(' {0:>6.2g}'.format(freqAbs))
            # end of line
            f.write("\n")

    # Write boundary material transmission file
    with open(os.path.join(outfolder, "scene_materials_transmission.txt"), "w") as f:
        for xmlid, mat in materials.iteritems():
            f.write('{0:>6} '.format(xmlid))
            # for each frequency band
            for freqTransm in mat["g"]:
                f.write(' {0:>6.2g}'.format(freqTransm))
            # end of line
            f.write("\n")

    # Write source position and power files
    with open(os.path.join(outfolder, "scene_sources.txt"), "w") as f:
        for src in sources_lst:
            f.write('{0:>15} {1:>15} {2:>15} '.format(src.pos[0], src.pos[1], src.pos[2]))
            # for each frequency band
            for spl in src.db:
                f.write(' {0:>6.4g}'.format(spl))
            # end of line
            f.write("\n")

    # Write shared vertices index
    with open(os.path.join(outfolder, "scene_shared_vertices.txt"), "w") as f:
        for ptindex in sharedVertices:
            f.write(str(ptindex) + "\n")

    return ret


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


def process_output_files(outfolder, coreconf, import_data, resultsModificationLayers):
    # sound velocity
    c0 = 342.0
    rhoco2 = 1.2 * c0 * c0
    # Read mesh file information from octave
    mesh_path = os.path.join(outfolder, "scene_XYZ_TETRA.hdf5")
    if os.path.exists(mesh_path):
        mesh_data = h5py.File(mesh_path, "r")
        nodes = mesh_data["topo"]["value"]["XYZ"]["value"].value.transpose()
        rooms = set(mesh_data["topo"]["value"]["TetDOF"]["value"].keys()) - {"dims"}
        tetrahedrons = numpy.concatenate([mesh_data["topo"]["value"]["TetDOF"]["value"][key]["value"].value.astype(int).transpose() for key in rooms])

        # Create spatial index for receivers points
        receivers_index = kdtree.create(dimensions=3)
        # For each surface receiver
        pt_count = 0
        for idrs, surface_receivers in coreconf.recsurf.iteritems():
            # For each vertex of the grid
            for faceid, receiver in enumerate(surface_receivers.GetSquaresCenter()):
                receivers_index.add(ReceiverSurf(idrs, faceid, receiver[0], receiver[1], receiver[2]))
                pt_count += 1
        # For each punctual receiver
        for idrp, rp in coreconf.recepteursponct.iteritems():
            receivers_index.add(ReceiverPunctual(idrp, rp["pos"][0], rp["pos"][1], rp["pos"][2]))
            pt_count += 1

        receivers_index.rebalance()

        # Open data files
        result_matrix = [numpy.array(h5py.File(os.path.join(outfolder, "scene_WInstaFields" + str(freq) + ".hdf5"))["xx"]["value"]["b"]["value"]) for freq in
                         coreconf.const["frequencies"]]
        coreconf.time_step = float(h5py.File(os.path.join(outfolder, "scene_WInstaFields" + str(coreconf.const["frequencies"][0]) + ".hdf5"))["xx"]["value"]["a"]["value"].value)
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
            # nearest_receivers = receivers_index.search_nn_dist([p[0], p[1], p[2]], rmax)
            nearest_receivers = receivers_index.search_nn_dist([p[0], p[1], p[2]], rmax)
            tetra_values = [[numpy.ones(4) for idvert in range(4)] for idfreq in range(len(coreconf.const["frequencies"]))]
            if len(nearest_receivers) > 0:
                tetra_values = [
                    [schroeder_to_impulse(result_matrix[idfreq][:, verts[idvert]]) for idvert in range(4)] for
                    idfreq in range(len(coreconf.const["frequencies"]))]
            # nearest_receivers = [receiver for receiver in res if square_dist(receiver, p) <= rmax]
            new_perc = int((idTetra / float(len(tetrahedrons))) * 100)
            if new_perc != last_perc:
                print("Export receivers %i %%" % new_perc)
                last_perc = new_perc
            # Compute coefficient of the receiver point into the tetrahedron
            for receiver in nearest_receivers:
                coefficient = get_a_coefficients(to_array(receiver), nodes[verts[0]], nodes[verts[1]], nodes[verts[2]], nodes[verts[3]])
                if coefficient.min() > 0:
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
                                        if power > 0:
                                            interpolated_value[steps[source_id]] += power / (rhoco2 * 2.5e-3)
                            coreconf.recsurf[receiver.idrs].face_power[receiver.faceid].append(
                                interpolated_value * rhoco2 * 2.5e-3)
                        else:
                            # Into a punctual receiver
                            # Look for sound source
                            if coreconf.const["with_direct_sound"]:
                                for source_id in steps.keys():
                                    if source_id in resultsModificationLayers and receiver.idrp in resultsModificationLayers[source_id].recp:
                                        power = resultsModificationLayers[source_id].recp[receiver.idrp][current_frequency_id]
                                        if power > 0:
                                            interpolated_value[steps[source_id]] += power / rhoco2

                            coreconf.recepteursponct[receiver.idrp]["power_insta"].append(
                                interpolated_value * rhoco2)
        print("End export receivers values")
    else:
        print("The Octave result file is not found %s" % mesh_path, file=sys.stderr)

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
            ret_tab[src.id] = int(math.floor(dist / (coreconf.const["cel"] * coreconf.time_step)))
    return ret_tab


def write_config_file(coreConf, outputdir):
    with open(os.path.join(outputdir, "Input_parameters.m"), "w") as f:
        f.write("TOB=[50, 63, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500,"
                " 3200, 4000, 5000, 6300, 8000, 10000, 12500, 16000, 20000];\n")
        f.write("BdOct1=4;\n")
        f.write("BdOctend=21;\n")
        f.write("Temperature=%f;\n" % coreConf.const["temperature_celsius"])
        f.write("Humidity=%f;\n" % coreConf.const["humidite"])
        f.write("Pressure=%f;\n" % coreConf.const["pression"])
        f.write("tol=%f;\n" % coreConf.const["tolerance"])
        f.write("maxint=%d;\n" % coreConf.const["maxint"])
        f.write("dt=%f;\n" % coreConf.const["timestep"])
        f.write("duration=%f;\n" % coreConf.const["duration"])
        f.write("atmos_absorption_calculation = %s;\n" % "1" if coreConf.const["do_abs_atmo"] else "0")


def main(call_octave=True):
    # find core folder
    scriptfolder = sys.argv[0][:sys.argv[0].rfind(os.sep)] + os.sep
    # Read I-SIMPA XML configuration file
    coreconf = cc.coreConfig(sys.argv[1])
    # Get direct field values using external computation core
    resultsModificationLayers = runTC(sys.argv[1], coreconf)
    outputdir = os.path.join(coreconf.paths["workingdirectory"], "core")
    if not os.path.exists(outputdir):
        os.mkdir(outputdir)
    # Translation CBIN 3D model and 3D tetrahedra mesh into Octave input files
    import_data = write_input_files(os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["modelName"]), os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["tetrameshFileName"]),
                      coreconf.materials, coreconf.sources_lst, outputdir)
    # Copy octave script to working dir
    matscript_folder = os.path.join(os.path.abspath(os.path.join(os.path.realpath(__file__), os.pardir)), "script")
    files = glob.iglob(os.path.join(matscript_folder, "*.m"))
    print(os.path.join(matscript_folder, "*.m"))
    for filep in files:
        if os.path.isfile(filep):
            shutil.copy2(filep, outputdir)
    # Write configuration file
    write_config_file(coreconf, outputdir)

    if call_octave:
        # Check if octave program are accessible in path
        octave = which("octave-cli.exe")
        if octave is None:
            print("Octave program not in system path, however input files are created", file=sys.stderr)
        else:
            command = ["octave-cli", "--no-window-system", "--verbose", os.path.join(outputdir , "mainDiffusion.m")]
            print("Run " + " ".join(command))
            deb = time.time()
            call(command, cwd=outputdir, shell=True)
            print("Execution in %.2f seconds" % ((time.time() - deb) / 1000.))
    process_output_files(outputdir, coreconf, import_data, resultsModificationLayers)
    sauve_recsurf_results.SauveRecepteurSurfResults(coreconf)
    sauve_recponct_results.SauveRecepteurPonctResults(coreconf)

if __name__ == '__main__':
    main(sys.argv[-1] != "noexec")
