cmake_minimum_required(VERSION 3.3)
project(lib_interface)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(LIBINTERFACE_SOURCE_FILES coreinitialisation.cpp coreString.cpp coreTypes.cpp dummy.cpp en_numeric.cpp std_tools.cpp
        Core/mathlib.cpp data_manager/base_core_configuration.cpp  data_manager/data_calculation/Celerite_du_son.cpp
        data_manager/data_calculation/Coef_Att_Atmos.cpp data_manager/data_calculation/Masse_volumique_air.cpp
        input_output/baseReportManager.cpp input_output/bin.cpp input_output/cxml.cpp input_output/gpl_io.cpp
        input_output/exportRecepteurSurf/rsbin.cpp input_output/exportRecepteurSurf/std_rsbin.cpp
        input_output/gabe/gabe.cpp input_output/gabe/stdgabe.cpp input_output/importExportMaillage/mbin.cpp
        input_output/importExportMaillage/m_report_bin.cpp input_output/particles/part_io.cpp
        input_output/ply/rply.c input_output/ply/rply_interface.cpp tools/octree44.cpp
        tools/octree44_dotElement.cpp tools/octree44_segmentElement.cpp tools/octree44_sphereElement.cpp
        tools/octree44_triangleElement.cpp tools/surf_merging.cpp tools/vol_identifier.cpp tools/vol_splitter.cpp
        tools/octree44_cylinderElement.cpp )
add_library(lib_interface SHARED ${LIBINTERFACE_SOURCE_FILES})
target_include_directories (lib_interface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})