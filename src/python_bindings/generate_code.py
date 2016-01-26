#! /usr/bin/python
# -*- coding: cp1252 -*-
# Copyright 2004-2008 Roman Yakovenko.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

"""
This file contains example to custom code creator.
My class point_t has private member variable 'x' and two accessors: get_x and 
set_x. I want to export those accessors as Python property.

In order to do this I created new class property_creator_t. In __init__ it takes
3 arguments:
    reference to get function declaration
    reference to set function declaration
    reference to parent code creator ( we can safely omit this argument )
"""

import os
import sys

#from environment import gccxml
from pygccxml import declarations
from pygccxml.declarations.matchers import access_type_matcher_t

from pyplusplus import code_creators
from pyplusplus import module_builder
from pyplusplus import function_transformers as FT
from doxygen import doxygen_doc_extractor
from pyplusplus.module_builder import call_policies
"""


"""

myextractor=doxygen_doc_extractor()

if __name__ == '__main__':
    module_name = '_libsimpa'
    #1. creating module builder
    mb = module_builder.module_builder_t( files=[
                                                     os.path.abspath('../dummy.hpp')
                                                 ]
                                          , gccxml_path=os.environ["GCCXML"]
                                          , encoding='iso-8859-1')

    #mb.namespace("core_mathlib").exclude()
    mb.calldefs( access_type_matcher_t( 'protected' ) ).exclude()
    mb.calldefs( access_type_matcher_t( 'private' ) ).exclude()
    # ####################################################
    # SCRIPT POUR ../input_output/bin.h
    mb.class_("t_pos").decl("operator float *").exclude()
    # ####################################################
    # SCRIPT POUR ../input_output/gabe/stdgabe.h
    mb.namespace("formatGABE").exclude()
    mb.namespace("formatGABE").decls("GABE_OBJECTTYPE").include()
    mb.decls( lambda decl: 'impl_' in decl.name ).exclude()
    io_gabe=mb.class_("Gabe_rw")
    io_gabe.mem_fun("size").alias="__len__"
    #transforme le parametre en valeur de retour des fonctions commencant par ReadCol
    io_gabe.decls(lambda decl: decl.name.startswith( 'ReadCol' )).add_transformation( FT.output(1) )

    # ####################################################
    # SCRIPT POUR ../input_output/mbin.h
    cmbin=mb.namespace("formatMBIN").class_("CMBIN")
    cmbin.mem_fun("ImportBIN").exclude()
    cmbin.mem_fun("ExportBIN").exclude()
    
    # ####################################################
    # SCRIPT POUR     ../input_output/cxml.h
    # mb.namespace("BasicXmlProcess").exclude()
    # cxmlnode=mb.class_("CXmlNode")
    # cxmlnode.mem_fun("GetChild").call_policies =  call_policies.return_value_policy( call_policies.return_opaque_pointer )
    # cxmlnode.decls("t_nodeData").exclude()
    # cxml=mb.class_("CXml")
    # cxml.mem_fun("GetRoot").call_policies =  call_policies.return_value_policy( call_policies.return_opaque_pointer )
    # mb.class_("CoreString").include()
    # for cons in cxmlnode.constructors("CXmlNode"):
	# for el in cons.arguments:
	# 	if el.name=="rowData":
    #                 cons.exclude()

    # ####################################################
    # SCRIPT POUR     ../input_output/progressionInfo.h
    mb.class_("progressionInfo").mem_fun("GetMainOperation").call_policies = call_policies.return_internal_reference( 1 )
    mb.class_("progressOperation").mem_fun("GetState").exclude()

    # ####################################################
    # SCRIPT POUR     ../input_output/exportRecepteurSurf/std_rsbin.h
    rsbin=mb.namespace("formatRSBIN").class_("rsurf_data")
    rsbin.mem_fun("GetFileInfos").add_transformation( FT.output('rs_size'),FT.output('nodes_size') ,FT.output('nbtimestep'),FT.output('timestep'),FT.output('recordType'))
    rsbin.mem_fun("GetNodePositionValue").add_transformation( FT.output('x'),FT.output('y'),FT.output('z'))
    rsbin.mem_fun("GetRsInfo").add_transformation( FT.output('nbfaces'),FT.output('rs_name'),FT.output('xmlid'))
    rsbin.mem_fun("GetFaceInfo").add_transformation( FT.output('vertexA'),FT.output('vertexB'),FT.output('vertexC'),FT.output('recordCount'))
    rsbin.mem_fun("GetFaceEnergy").add_transformation( FT.output('idstep'),FT.output('energy'))
    rsbin.mem_funs("SetRsBinData").exclude()
    rsiso=mb.namespace("formatRSBIN").class_("rsurf_iso_contouring")
    rsiso.mem_fun("GetMinMax").add_transformation( FT.output('min'),FT.output('max'))
    rsiso.mem_fun("GetIsoLevelCurves").add_transformation( FT.output('curvlst'))

    # #####################################################
    # ../data_manager/data_calculation/calculsPropagation.h
    mb.namespace("CGTconst").exclude()

    # #####################################################
    # ../input_output/particles/part_io.hpp
    ParticuleIO=mb.namespace("particleio").class_("ParticuleIO")
    ParticuleIO.mem_fun("GetHeaderData").add_transformation( FT.output('timeStep'),FT.output('nbParticles'),FT.output('nbStepMax'))
    ParticuleIO.mem_fun("NextParticle").add_transformation( FT.output('firstTimeStep'),FT.output('nbTimeStep'))
    ParticuleIO.mem_fun("NextTimeStep").add_transformation(  FT.output('x'),FT.output('y'),FT.output('z'),FT.output('energy'))
    
    # #####################################################
    # ../tools/vol_splitter.hpp
    VolumesSplitter=mb.namespace("volumes_splitter").class_("VolumesSplitter")
    VolumesSplitter.mem_fun("GetVolumeInformation").add_transformation( FT.output('volumeXmlId'),FT.output('volume'),FT.output('faceIds'),FT.output('volNeigh'))
    VolumesSplitter.mem_fun("GetVolumeInformation").alias="__getitem__"
    VolumesSplitter.mem_fun("GetVolumes").alias="__len__"
    VolumesSplitter.mem_fun("GetInternalFaces").add_transformation( FT.output('internalFaceIds'))

    # #####################################################
    # ../tools/surf_merging.hpp
    SurfaceMerging=mb.namespace("surf_merge").class_("SurfaceMerging")
    SurfaceMerging.mem_fun("GetGroupInformation").add_transformation( FT.output('faceIds'))
    
    #2. creating module code creator
    mb.build_code_creator( module_name=module_name, doc_extractor=myextractor)    
    mb.code_creator.user_defined_directories.append( os.path.abspath( '../' ))       

    #4. writing module to disk
    mb.write_module( module_name + '.py.cpp' )
    
    
    print 'done'
