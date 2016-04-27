include_directories (.)

set(LIBINTERFACE_SOURCES 
    coreinitialisation.h
    coreinitialisation.cpp
    coreString.h
    coreString.cpp
    coreTypes.h
    coreTypes.cpp
    en_numeric.hpp
    en_numeric.cpp
    std_tools.hpp
    std_tools.cpp)

set(CORE_SOURCES
    Core/mathlib.h
    Core/mathlib.cpp)

set(DATA_MANAGER_SOURCES
    data_manager/base_core_configuration.h
    data_manager/base_core_configuration.cpp
    data_manager/data_calculation/Celerite_du_son.cpp
    data_manager/data_calculation/Coef_Att_Atmos.cpp
    data_manager/data_calculation/Masse_volumique_air.cpp)

set(IO_SOURCES
    input_output/baseReportManager.h
    input_output/baseReportManager.cpp
    input_output/bin.h
    input_output/bin.cpp
    input_output/cxml.h
    input_output/cxml.cpp
    input_output/gpl_io.hpp
    input_output/gpl_io.cpp)

set(IO_RECSURF_SOURCES
    input_output/exportRecepteurSurf/rsbin.h
    input_output/exportRecepteurSurf/rsbin.cpp
    input_output/exportRecepteurSurf/std_rsbin.hpp
    input_output/exportRecepteurSurf/std_rsbin.cpp)

set(IO_GABE_SOURCES
    input_output/gabe/gabe.h
    input_output/gabe/gabe.cpp
    input_output/gabe/stdgabe.h
    input_output/gabe/stdgabe.cpp)

set(IO_MAILLAGE_SOURCES
    input_output/importExportMaillage/mbin.h
    input_output/importExportMaillage/mbin.cpp
    input_output/importExportMaillage/m_report_bin.h
    input_output/importExportMaillage/m_report_bin.cpp)

set(IO_PARTICLES_SOURCES
    input_output/particles/part_io.hpp
    input_output/particles/part_io.cpp)

set(IO_PLY_SOURCES
    input_output/ply/rply.h
    input_output/ply/rply.c
    input_output/ply/rply_interface.hpp
    input_output/ply/rply_interface.cpp)

set(TOOLS_SOURCES
    tools/octree44.hpp
    tools/octree44.cpp
    tools/octree44_dotElement.hpp
    tools/octree44_dotElement.cpp
    tools/octree44_segmentElement.hpp
    tools/octree44_segmentElement.cpp
    tools/octree44_sphereElement.hpp
    tools/octree44_sphereElement.cpp
    tools/octree44_triangleElement.hpp
    tools/octree44_triangleElement.cpp
    tools/surf_merging.hpp
    tools/surf_merging.cpp
    tools/vol_identifier.hpp
    tools/vol_identifier.cpp
    tools/vol_splitter.hpp
    tools/vol_splitter.cpp
    tools/octree44_cylinderElement.hpp
    tools/octree44_cylinderElement.cpp)

source_group( "" FILES ${LIBINTERFACE_SOURCES} ) 
source_group( "Core" FILES ${CORE_SOURCES} )
source_group( "data_manager" FILES ${DATA_MANAGER_SOURCES} )

source_group( "input_output" FILES ${IO_SOURCES} )
source_group( "input_output\\exportRecepeteurSurf" FILES ${IO_RECSURF_SOURCES} )
source_group( "input_output\\gabe" FILES ${IO_GABE_SOURCES} )
source_group( "input_output\\importExportMaillage" FILES ${IO_MAILLAGE_SOURCES} )
source_group( "input_output\\particles" FILES ${IO_PARTICLES_SOURCES} )
source_group( "input_output\\ply" FILES ${IO_PLY_SOURCES} )

source_group( "tools" FILES ${TOOLS_SOURCES} )

add_library(lib_interface 
    ${LIBINTERFACE_SOURCES}
    ${CORE_SOURCES}
    ${DATA_MANAGER_SOURCES}
    ${IO_SOURCES}
    ${IO_RECSURF_SOURCES}
    ${IO_GABE_SOURCES}
    ${IO_MAILLAGE_SOURCES}
    ${IO_PARTICLES_SOURCES}
    ${IO_PLY_SOURCES}
    ${TOOLS_SOURCES})

#-----------------------------------------------------------------------------
#
# To fix compilation problem: relocation R_X86_64_32 against `a local symbol' can not be
# used when making a shared object; recompile with -fPIC
# See http://www.cmake.org/pipermail/cmake/2007-May/014350.html
#
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(lib_interface PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )



# Creates a folder "libraries" and adds target project (lib_interface.vcproj) under it
set_property(TARGET lib_interface PROPERTY FOLDER "libraries")

