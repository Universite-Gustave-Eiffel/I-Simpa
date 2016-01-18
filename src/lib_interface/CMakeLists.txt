cmake_minimum_required(VERSION 3.3)

include_directories (.)
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
add_library(lib_interface ${LIBINTERFACE_SOURCE_FILES})

# Creates a folder "libraries" and adds target project (lib_interface.vcproj) under it
set_property(TARGET lib_interface PROPERTY FOLDER "libraries")

# Adds logic to INSTALL.vcproj to copy lib_interface.dll to the destination directory
install (TARGETS lib_interface
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
        ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/libarc
        LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/lib
        COMPONENT library
)
