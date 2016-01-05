# -------------------------------------------------
# Project created by QtCreator 2010-05-03T09:58:56
# -------------------------------------------------
QT -= core \
    gui
TARGET = lib_interface
TEMPLATE = lib
CONFIG = staticlib
DEFINES += LIB_INTERFACE_LIBRARY
SOURCES += coreinitialisation.cpp \
    coreString.cpp \
    coreTypes.cpp \
    dummy.cpp \
    en_numeric.cpp \
    std_tools.cpp \
    Core/mathlib.cpp \
    data_manager/base_core_configuration.cpp \
    data_manager/data_calculation/Celerite_du_son.cpp \
    data_manager/data_calculation/Coef_Att_Atmos.cpp \
    data_manager/data_calculation/Masse_volumique_air.cpp \
    input_output/baseReportManager.cpp \
    input_output/bin.cpp \
    input_output/cxml.cpp \
    input_output/gpl_io.cpp \
    input_output/exportRecepteurSurf/rsbin.cpp \
    input_output/exportRecepteurSurf/std_rsbin.cpp \
    input_output/gabe/gabe.cpp \
    input_output/gabe/stdgabe.cpp \
    input_output/importExportMaillage/mbin.cpp \
    input_output/importExportMaillage/m_report_bin.cpp \
    input_output/particles/part_io.cpp \
    input_output/ply/rply.c \
    input_output/ply/rply_interface.cpp \
    tools/octree44.cpp \
    tools/octree44_dotElement.cpp \
    tools/octree44_segmentElement.cpp \
    tools/octree44_sphereElement.cpp \
    tools/octree44_triangleElement.cpp \
    tools/surf_merging.cpp \
    tools/vol_identifier.cpp \
    tools/vol_splitter.cpp \
    tools/octree44_cylinderElement.cpp
HEADERS += coreinitialisation.h \
    coreString.h \
    coreTypes.h \
    dummy.hpp \
    en_numeric.hpp \
    std_tools.hpp \
    Core/mathlib.h \
    Core/Model3D.h \
    data_manager/base_core_configuration.h \
    data_manager/data_calculation/calculsPropagation.h \
    input_output/baseReportManager.h \
    input_output/bin.h \
    input_output/cxml.h \
    input_output/gpl_io.hpp \
    input_output/part_binary.h \
    input_output/progressionInfo.h \
    input_output/utf8.h \
    input_output/exportRecepteurSurf/rsbin.h \
    input_output/exportRecepteurSurf/std_rsbin.hpp \
    input_output/gabe/gabe.h \
    input_output/gabe/stdgabe.h \
    input_output/importExportMaillage/mbin.h \
    input_output/importExportMaillage/m_report_bin.h \
    input_output/particles/part_binary.h \
    input_output/particles/part_io.hpp \
    input_output/ply/rply.h \
    input_output/ply/rply_interface.hpp \
    input_output/utf8/checked.h \
    input_output/utf8/core.h \
    input_output/utf8/unchecked.h \
    tools/collision.h \
    tools/octree44.hpp \
    tools/octree44_dotElement.hpp \
    tools/octree44_segmentElement.hpp \
    tools/octree44_sphereElement.hpp \
    tools/octree44_triangleElement.hpp \
    tools/surf_merging.hpp \
    tools/vol_identifier.hpp \
    tools/vol_splitter.hpp \
    tools/octree44_cylinderElement.hpp
INCLUDEPATH = ./
