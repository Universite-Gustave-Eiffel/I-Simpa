#! /usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function  # compatibilité python 3.0
import libsimpa as ls
import math
import os


def MakeFolderIfNeeded(path):
    #modifs 27/08/2024
    if type(path)==bytes:
        path=str(path)
        path=path[2:-1]
    #fin modifs
    list = path.split(os.path.sep)
    complete = ""
    if os.path.isabs(path):
        complete = list.pop(0) + os.sep
    while len(list) != 0:
        fold = list.pop(0)
        if not os.path.exists(os.path.join(complete, fold)):
            mkpath = os.path.join(complete, fold)
            os.mkdir(mkpath)
        complete = os.path.join(complete, fold)


def SauveRecepteurSurfResults(coreconf):
    
    #-------------Modifs code---------------------
    working_directory = coreconf.paths["workingdirectory"]    
    if isinstance(working_directory, bytes):
           working_directory = working_directory.decode('utf-8')
    recepteurss_directory = coreconf.paths["recepteurss_directory"]    
    if isinstance(recepteurss_directory, bytes):
           recepteurss_directory = recepteurss_directory.decode('utf-8')
    rootpath = os.path.join(working_directory, recepteurss_directory + os.sep)
    #-------------FIN Modifs code-----------------
    
    #rootpath = os.path.join(coreconf.paths["workingdirectory"], coreconf.paths["recepteurss_directory"] + os.sep)
    for (idrs, surface_receiver) in coreconf.recsurf.items():
        if len(surface_receiver.face_power) == 0:
            continue
        freqCount = len(coreconf.const["frequencies"])
        for id_freq in range(freqCount):
            rsdata = ls.rsurf_data()
            # File Header
            rsdata.Make(len(surface_receiver.vertices), 1,
                        int(math.ceil(coreconf.const['duration'] / coreconf.const['timestep'])),
                        float(coreconf.const['timestep']))
            # Vertices
            for nodeid, vertex in enumerate(surface_receiver.vertices):
                rsdata.SetNodeValue(nodeid, vertex[0], vertex[1], vertex[2])
            triangle_based = len(surface_receiver.faceindex) > 0 and len(surface_receiver.faceindex[0]) <= 3
            triangle_count = len(surface_receiver.faceindex)
            if not triangle_based:
                triangle_count = len(surface_receiver.faceindex) * 2
            rsdata.MakeRs(0, triangle_count, str(surface_receiver.label), surface_receiver.index)
            facecount = 0
            for idface, face in enumerate(surface_receiver.faceindex):
                has_levels = len(surface_receiver.face_power[idface]) > id_freq
                nbTimeStep = 1
                if has_levels:
                    nbTimeStep = len(surface_receiver.face_power[idface][id_freq])
                # Triangle 1
                rsdata.SetFaceInfo(0, facecount, face[0], face[1], face[2], nbTimeStep)
                for recordId in range(nbTimeStep):
                    if has_levels:
                        rsdata.SetFaceEnergy(0, facecount, recordId, recordId, float(surface_receiver.face_power[idface][id_freq][recordId]))
                    else:
                        rsdata.SetFaceEnergy(0, facecount, recordId, recordId, float(0))
                facecount += 1
                if len(face) > 3:
                    # Triangle 2
                    rsdata.SetFaceInfo(0, facecount, face[0], face[2], face[3], nbTimeStep)
                    for recordId in range(nbTimeStep):
                        if has_levels:
                            rsdata.SetFaceEnergy(0, facecount, recordId, recordId, float(surface_receiver.face_power[idface][id_freq][recordId]))
                        else:
                            rsdata.SetFaceEnergy(0, facecount, recordId, recordId, float(0))
                    facecount += 1
            rspath = os.path.join(os.path.join(rootpath, str(surface_receiver.label) + os.sep), ("%d Hz" % (coreconf.const["frequencies"][id_freq])) + os.sep)
            MakeFolderIfNeeded(rspath)
            #print("rsdata = ", rsdata)
            #print("coreconf.paths['recepteurss_filename'] = ", coreconf.paths["recepteurss_filename"], type(coreconf.paths["recepteurss_filename"]))
            ls.rsurf_io.Save(rspath + str(coreconf.paths["recepteurss_filename"]), rsdata)
