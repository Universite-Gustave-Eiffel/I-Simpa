include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")
include(GNUInstallDirs)
set(CMAKE_INSTALL_MFC_LIBRARIES TRUE )
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE )
include(InstallRequiredSystemLibraries)
string(TIMESTAMP BUILD_DATE "%Y-%m-%dT%H:%M:%S")

# Detect if building for Flatpak
if(DEFINED ENV{FLATPAK_ID})
    set(IS_FLATPAK TRUE)
    message(STATUS "Building for Flatpak environment")
else()
    set(IS_FLATPAK FALSE)
endif()

# Set installation directories based on build type
if(IS_FLATPAK)
    set(ISIMPA_ICON_DIR share/icons/hicolor/256x256/apps)
    set(ISIMPA_BIN_DIR bin)
    set(ISIMPA_DATA_DIR bin)
    set(ISIMPA_DOC_DIR bin)
    set(ISIMPA_DOC_DIR bin)
else()
    set(ISIMPA_ICON_DIR .)
    set(ISIMPA_BIN_DIR "${CMAKE_INSTALL_BINDIR}")
    set(ISIMPA_DATA_DIR "${CMAKE_INSTALL_DATADIR}")
    set(ISIMPA_DOC_DIR "${CMAKE_INSTALL_DOCDIR}")
endif()

MACRO(ADDMULTIPLE_LANG curdir installdir)
  # MESSAGE(STATUS "ADDMULTIPLE_LANG ${curdir} ${installdir}")
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  FOREACH(child ${children})
    IF(EXISTS "${curdir}/${child}"
            AND NOT IS_DIRECTORY "${curdir}/${child}"
            AND "${child}" MATCHES "\\.po$")
        file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}" "${curdir}/${child}")
        # Remove the .po extension (last 3 characters)
        string(REGEX REPLACE "\\.po$" "" lang_name "${child}")
        MESSAGE(STATUS "Add lang ${lang_name} from ${rel_path}")
        set(output_file "${installdir}/${lang_name}/LC_MESSAGES/isimpa.mo")
        get_filename_component(output_directory "${output_file}" DIRECTORY)
        add_custom_command(TARGET isimpa
                POST_BUILD
                COMMENT "Create directory and output ${lang_name} to ${output_file}"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${output_directory}"
                COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} "${curdir}/${child}" -o "${output_file}"
        )
    ENDIF()
  ENDFOREACH()
ENDMACRO()

#--------------#
#    SOURCES
#--------------#

set(3DENGINE_SOURCES
    3dengine/ArcBall.cpp
    3dengine/ArcBall.h
    3dengine/Camera.cpp
    3dengine/Camera.h
    3dengine/GlBitmapSurface.cpp
    3dengine/GlBitmapSurface.h
    3dengine/OpenGLApp.cpp
    3dengine/OpenGLApp.h
    3dengine/OpenGlViewer.cpp
    3dengine/OpenGlViewer.h
    3dengine/legendFonts.cpp
    3dengine/legendFonts.h
    3dengine/legendObject.cpp
    3dengine/legendObject.h)

set(3DENGINE_CORE_SOURCES
    3dengine/Core/3ds.cpp
    3dengine/Core/3ds.h
    3dengine/Core/Animator.cpp
    3dengine/Core/Animator.hpp
    3dengine/Core/Asc.cpp
    3dengine/Core/Asc.h
    3dengine/Core/Material.cpp
    3dengine/Core/Material.h
    3dengine/Core/Mathlib.h
    3dengine/Core/Model3D.cpp
    3dengine/Core/Model3D.h
    3dengine/Core/Nff.cpp
    3dengine/Core/Nff.h
    3dengine/Core/Objet3D.cpp
    3dengine/Core/Objet3D.h
    3dengine/Core/Objet3D_maillage.cpp
    3dengine/Core/Particules.cpp
    3dengine/Core/Particules.h
    3dengine/Core/Recepteurs_ponctuel_intensity.cpp
    3dengine/Core/Recepteurs_ponctuel_intensity.hpp
    3dengine/Core/Recepteurs_surfacique.cpp
    3dengine/Core/Recepteurs_surfacique.h
    3dengine/Core/Texture.cpp
    3dengine/Core/Texture.h
    3dengine/Core/bin.cpp
    3dengine/Core/bin.h
    3dengine/Core/mesh.cpp
    3dengine/Core/mesh.h
    3dengine/Core/stl.cpp
    3dengine/Core/stl.hpp
    3dengine/Core/var.cpp
    3dengine/Core/var.h)

set(3DENGINE_TOOLS_SOURCES
    3dengine/tools/intersection_seeker.cpp
    3dengine/tools/intersection_seeker.h
    3dengine/tools/opengl_test.cpp
    3dengine/tools/opengl_test.hpp
    3dengine/tools/recepteursurf_difference.cpp
    3dengine/tools/recepteursurf_difference.hpp
    3dengine/tools/tesselator.cpp
    3dengine/tools/tesselator.h)

set(GL_SOURCES
    GL/opengl_inc.h)

set(IHM_SOURCES
    IHM/ComboTreeCtrl.cpp
    IHM/ComboTreeCtrl.h
    IHM/DataWindow.cpp
    IHM/DataWindow.h
    IHM/DialogDiagramCreator.cpp
    IHM/DialogDiagramCreator.h
    IHM/GabeDataGrid.cpp
    IHM/GabeDataGrid.hpp
    IHM/MainPropGrid.cpp
    IHM/MainPropGrid.h
    IHM/PropGrid.cpp
    IHM/PropGrid.h
    IHM/RecepteurSOperationDialog.cpp
    IHM/RecepteurSOperationDialog.h
    IHM/WizardRemeshModel.cpp
    IHM/WizardRemeshModel.hpp
    IHM/base64.cpp
    IHM/base64.h
    IHM/customItem/uimenuitem.cpp
    IHM/customRenderer/gridcellgabefloatrenderer.cpp
    IHM/customdialog/customdlg.cpp
    IHM/customdialog/customdlg.h
    IHM/foldpaneltree.cpp
    IHM/foldpaneltree.h
    IHM/languageSelection.cpp
    IHM/languageSelection.hpp
    IHM/loadingSceneDialog.cpp
    IHM/loadingSceneDialog.hpp
    IHM/pyConsole.cpp
    IHM/pyConsole.hpp
    IHM/simpleGraph.cpp
    IHM/simpleGraph.h
    IHM/simpleGraphArrays.h
    IHM/simpleGraphDialogs.cpp
    IHM/simpleGraphDialogs.h
    IHM/simpleGraphEnumerators.cpp
    IHM/simpleGraphEnumerators.h
    IHM/simpleGraphManager.cpp
    IHM/simpleGraphManager.h
    IHM/uiTreeCtrl.cpp
    IHM/uiTreeCtrl.h
    IHM/AboutDialog.cpp
    IHM/AboutDialog.hpp)

set(DATA_MANAGER_SOURCES
    data_manager/appconfig.cpp
    data_manager/appconfig.h
    data_manager/drawable_element.cpp
    data_manager/drawable_element.h
    data_manager/e_data.h
    data_manager/e_data_bool.h
    data_manager/e_data_bool_bfreqchoice.h
    data_manager/e_data_color.h
    data_manager/e_data_entier.h
    data_manager/e_data_float.h
    data_manager/e_data_font.h
    data_manager/e_data_file.h
    data_manager/e_data_list.h
    data_manager/e_data_row.h
    data_manager/e_data_row_bandefreq.h
    data_manager/e_data_row_ext_bandefreq.h
    data_manager/e_data_row_materiau.h
    data_manager/e_data_text.h
    data_manager/e_data_tree.h
    data_manager/e_position.h
    data_manager/element.cpp
    data_manager/element.h
    data_manager/logger_tetgen_debug.hpp
    data_manager/projet.cpp
    data_manager/projet.h
    data_manager/projet_calculation.cpp
    data_manager/projet_maillage.cpp
    data_manager/projet_python.cpp
    data_manager/projet_remesh.cpp
    data_manager/projet_undo_redo.cpp)

set(DATA_MANAGER_CUSTOMEDIT_SOURCES
    data_manager/customEditor/wxGridCellColorEditor.cpp
    data_manager/customEditor/wxGridCellColorEditor.h
    data_manager/customEditor/wxGridCellFontEditor.cpp
    data_manager/customEditor/wxGridCellFontEditor.h
    data_manager/customEditor/wxGridCellFileEditor.cpp
    data_manager/customEditor/wxGridCellFileEditor.h
    data_manager/customEditor/wxGridCellTreeEditor.cpp
    data_manager/customEditor/wxGridCellTreeEditor.h)

set(DATA_MANAGER_GENERICELEM_SOURCES
    data_manager/generic_element/e_directivity.h
    data_manager/generic_element/e_directivity.cpp
    data_manager/generic_element/e_directivity_user.h
    data_manager/generic_element/e_directivity_app.h
    data_manager/generic_element/e_gammeabsorption.cpp
    data_manager/generic_element/e_gammeabsorption.h
    data_manager/generic_element/e_gammefrequence.cpp
    data_manager/generic_element/e_gammefrequence.h
    data_manager/generic_element/e_gammefrequence_app.h
    data_manager/generic_element/e_gammefrequence_user.h
    data_manager/generic_element/e_materiau.cpp
    data_manager/generic_element/e_materiau.h
    data_manager/generic_element/e_materiau_app.h
    data_manager/generic_element/e_materiau_user.h
    data_manager/generic_element/e_property_freq.cpp
    data_manager/generic_element/e_property_freq.h)

set(DATA_MANAGER_GRPINFO_SOURCES
    data_manager/grpInfo/data_group_info.cpp
    data_manager/grpInfo/data_group_info.h)

set(DATA_MANAGER_PYINTERFACE_SOURCES
    data_manager/python_interface/boost_python_header.cpp
    data_manager/python_interface/boost_python_header.h
    data_manager/python_interface/instanceManager.cpp
    data_manager/python_interface/instanceManager.hpp
    data_manager/python_interface/py_ui_module/Application.cpp
    data_manager/python_interface/py_ui_module/Application.hpp
    data_manager/python_interface/py_ui_module/e_pyext.cpp
    data_manager/python_interface/py_ui_module/e_pyext.hpp
    data_manager/python_interface/py_ui_module/element_file_pywrap.cpp
    data_manager/python_interface/py_ui_module/element_file_pywrap.hpp
    data_manager/python_interface/py_ui_module/element_pywrap.cpp
    data_manager/python_interface/py_ui_module/element_pywrap.hpp
    data_manager/python_interface/python_command.h
    data_manager/python_interface/pythonshell.cpp
    data_manager/python_interface/pythonshell.hpp
    data_manager/python_interface/pythonstdioredirect.cpp
    data_manager/python_interface/pythonstdioredirect.hpp)

set(DATA_MANAGER_TREECORE_SOURCES
    data_manager/tree_core/e_core.h
    data_manager/tree_core/e_core_core.h
    data_manager/tree_core/e_core_core_bfreqselection.h
    data_manager/tree_core/e_core_core_config.h
    data_manager/tree_core/e_core_core_tetconf.h
    data_manager/tree_core/e_core_sppscore.h
        data_manager/tree_core/e_core_tccore.h)

set(DATA_MANAGER_TREERAPPORT_SOURCES
    data_manager/tree_rapport/e_report.h
    data_manager/tree_rapport/e_report_file.cpp
    data_manager/tree_rapport/e_report_file.h
    data_manager/tree_rapport/e_report_folder.h
    data_manager/tree_rapport/e_report_gabe.cpp
    data_manager/tree_rapport/e_report_gabe.h
    data_manager/tree_rapport/e_report_gabe_gap.cpp
    data_manager/tree_rapport/e_report_gabe_gap.h
    data_manager/tree_rapport/e_report_gabe_recp.cpp
    data_manager/tree_rapport/e_report_gabe_recp.h
    data_manager/tree_rapport/e_report_gabe_recps.cpp
    data_manager/tree_rapport/e_report_gabe_recps.h
    data_manager/tree_rapport/e_report_partvisualisation.h
    data_manager/tree_rapport/e_report_recepteurssvisualisation.h
    data_manager/tree_rapport/e_report_rpi.h
    data_manager/tree_rapport/e_report_unknown_file.cpp
    data_manager/tree_rapport/e_report_unknown_file.h)

set(DATA_MANAGER_TREESCENE_SOURCES
    data_manager/tree_scene/e_scene.cpp
    data_manager/tree_scene/e_scene.h
    data_manager/tree_scene/e_scene_bdd.h
    data_manager/tree_scene/e_scene_bdd_materiaux.h
    data_manager/tree_scene/e_scene_bdd_materiaux_app.h
    data_manager/tree_scene/e_scene_bdd_materiaux_app_group.h
    data_manager/tree_scene/e_scene_bdd_materiaux_app_materiau.h
    data_manager/tree_scene/e_scene_bdd_materiaux_propmateriau.h
    data_manager/tree_scene/e_scene_bdd_materiaux_rendermateriau.h
    data_manager/tree_scene/e_scene_bdd_materiaux_user.h
    data_manager/tree_scene/e_scene_bdd_materiaux_user_group.h
    data_manager/tree_scene/e_scene_bdd_materiaux_user_materiau.h
    data_manager/tree_scene/e_scene_bdd_spectrums.h
    data_manager/tree_scene/e_scene_bdd_spectrums_app.h
    data_manager/tree_scene/e_scene_bdd_spectrums_user.h
    data_manager/tree_scene/e_scene_bdd_directivities.h
    data_manager/tree_scene/e_scene_bdd_directivities_app.h
    data_manager/tree_scene/e_scene_bdd_directivities_user.h
    data_manager/tree_scene/e_scene_donnees.h
    data_manager/tree_scene/e_scene_encombrements.h
    data_manager/tree_scene/e_scene_encombrements_encombrement_cuboide.h
    data_manager/tree_scene/e_scene_encombrements_encombrement_model.h
    data_manager/tree_scene/e_scene_encombrements_encombrement_proprietes.h
    data_manager/tree_scene/e_scene_encombrements_encombrement_rendu.h
    data_manager/tree_scene/e_scene_groupesurfaces.h
    data_manager/tree_scene/e_scene_groupesurfaces_groupe.cpp
    data_manager/tree_scene/e_scene_groupesurfaces_groupe.h
    data_manager/tree_scene/e_scene_groupesurfaces_groupe_vertex.h
    data_manager/tree_scene/e_scene_projet.h
    data_manager/tree_scene/e_scene_projet_configuration.h
    data_manager/tree_scene/e_scene_projet_environnement.h
    data_manager/tree_scene/e_scene_projet_informations.h
    data_manager/tree_scene/e_scene_projet_rendu.h
    data_manager/tree_scene/e_scene_projet_rendu_origine.h
    data_manager/tree_scene/e_scene_projet_userconfiguration.h
    data_manager/tree_scene/e_scene_recepteursp.h
    data_manager/tree_scene/e_scene_recepteursp_recepteur.h
    data_manager/tree_scene/e_scene_recepteursp_recepteur_proprietes.h
    data_manager/tree_scene/e_scene_recepteursp_recepteur_rendu.h
    data_manager/tree_scene/e_scene_recepteurss.h
    data_manager/tree_scene/e_scene_recepteurss_recepteur.h
    data_manager/tree_scene/e_scene_recepteurss_recepteur_proprietes.h
    data_manager/tree_scene/e_scene_recepteurss_recepteur_rendu.h
    data_manager/tree_scene/e_scene_recepteurss_recepteurcoupe.h
    data_manager/tree_scene/e_scene_recepteurss_recepteurcoupe_proprietes.h
    data_manager/tree_scene/e_scene_recepteurss_recepteurcoupe_rendu.h
    data_manager/tree_scene/e_scene_sources.h
    data_manager/tree_scene/e_scene_sources_source.h
    data_manager/tree_scene/e_scene_sources_source_properties.h
    data_manager/tree_scene/e_scene_sources_source_rendu.h
    data_manager/tree_scene/e_scene_volumes.cpp
    data_manager/tree_scene/e_scene_volumes.h
    data_manager/tree_scene/e_scene_volumes_volume.h
    data_manager/tree_scene/e_scene_volumes_volume_proprietes.h
    data_manager/tree_scene/e_scene_volumes_volume_rendu.h)

set(DATA_MANAGER_TREEUSER_SOURCES
    data_manager/tree_userpref/e_userprefitem.hpp
    data_manager/tree_userpref/e_userprefitemisotemplate.hpp
    data_manager/tree_userpref/e_userprefnode.hpp)

set(MAIN_SOURCES
    main/i_simpa_main.cpp
    main/i_simpa_main.h)

set(MANAGER_SOURCES
    manager/bigfilereader.cpp
    manager/bigfilereader.h
    manager/generic_typedef.h
    manager/lifetimeWitness.hpp
    manager/modelRetriever.cpp
    manager/modelRetriever.hpp
    manager/processManager.cpp
    manager/processManager.h
    manager/sppsString.cpp
    manager/stringParser.h
    manager/stringTools.h)

set(ISIMPA_SOURCES
    first_header_include.hpp
    last_cpp_include.hpp
    UtfConverter.cpp
    UtfConverter.h
    ConvertUTF.c
    ConvertUTF.h)

source_group( "3dengine" FILES ${3DENGINE_SOURCES} )
source_group( "3dengine/Core" FILES ${3DENGINE_CORE_SOURCES} )
source_group( "3dengine/tools" FILES ${3DENGINE_TOOLS_SOURCES} )

source_group( "GL" FILES ${GL_SOURCES} )
source_group( "WIN32" FILES ${WIN32_SOURCES})
source_group( "IHM" FILES ${IHM_SOURCES} )

source_group( "data_manager" FILES ${DATA_MANAGER_SOURCES} )
source_group( "data_manager/customEditor" FILES ${DATA_MANAGER_CUSTOMEDIT_SOURCES} )
source_group( "data_manager/generic_element" FILES ${DATA_MANAGER_GENERICELEM_SOURCES} )
source_group( "data_manager/grpInfo" FILES ${DATA_MANAGER_GRPINFO_SOURCES} )
source_group( "data_manager/python_interface" FILES ${DATA_MANAGER_PYINTERFACE_SOURCES} )
source_group( "data_manager/tree_core" FILES ${DATA_MANAGER_TREECORE_SOURCES} )
source_group( "data_manager/tree_rapport" FILES ${DATA_MANAGER_TREERAPPORT_SOURCES} )
source_group( "data_manager/tree_scene" FILES ${DATA_MANAGER_TREESCENE_SOURCES} )
source_group( "data_manager/tree_userpref" FILES ${DATA_MANAGER_TREEUSER_SOURCES} )

source_group( "main" FILES ${MAIN_SOURCES} )
source_group( "manager" FILES ${MANAGER_SOURCES} )
source_group( "" FILES ${ISIMPA_SOURCES} )

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

# Build auto-generated source files
# Find Python 3
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module Development)

# Debug: Print Python variables to verify they're set correctly
message(STATUS "Python3_FOUND: ${Python3_FOUND}")
message(STATUS "Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3_LIBRARIES: ${Python3_LIBRARIES}")
message(STATUS "Python3_LIBRARY_DIRS: ${Python3_LIBRARY_DIRS}")
message(STATUS "Python3_EXECUTABLE: ${Python3_EXECUTABLE}")

list(APPEND CMAKE_SWIG_FLAGS "-py3" "-DPY3")

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)

# Silence OpenGL deprecation warnings on macOS
add_definitions(-DGL_SILENCE_DEPRECATION)

include(FindPackageMessage)
if(OPENGL_GLU_FOUND)
    find_package_message(OPENGL_GLU
        "Found OpenGLU: ${OPENGL_glu_LIBRARY}"
        "[${OPENGL_glu_LIBRARY}][${OPENGL_INCLUDE_DIR}]")
else(OPENGL_GLU_FOUND)
    message(FATAL_ERROR "======================\n"
                        "GLU library not found.\n"
                        "======================\n")
endif(OPENGL_GLU_FOUND)

# Ask make to call python in order to use element.h to produce enum_def.cpp
ADD_CUSTOM_COMMAND(
  OUTPUT "generated/enum_def.cpp"
  COMMENT "Building enum_def.cpp using python script"
  COMMAND "${Python3_EXECUTABLE}" ${PROJECT_SOURCE_DIR}/src/isimpa/data_manager/python_interface/py_ui_module/generate_enum_def.py
  ARGS    "${PROJECT_SOURCE_DIR}/src/isimpa/data_manager/element.h"
  DEPENDS "data_manager/element.h"
  )

# Before using element_pywrap.cpp build enum_def.cpp
set_property(SOURCE data_manager/python_interface/py_ui_module/element_pywrap.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated/enum_def.cpp)

configure_file(data_manager/appconfig_version.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/generated/appconfig_version.cpp )

set(RESOURCE_FILES "")

if(APPLE)
    # List your resource files (recursively or with globbing)
    file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")


    # Attach resource files to your executable with the correct bundle location
    foreach(RES_FILE ${RESOURCE_FILES})
        # Get the path of the file relative to the resource root
        file(RELATIVE_PATH RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${RES_FILE}")
        # Set the bundle location for this file, preserving directory structure under Resources/
        get_filename_component(filename ${RES_PATH} NAME)
        get_filename_component(dir ${RES_PATH} DIRECTORY)
        if(NOT filename STREQUAL "CMakeLists.txt")
            set_source_files_properties(${RES_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${dir}")
        endif ()
    endforeach()
endif ()

# Set Properties->General->Configuration Type to Application
# Creates isimpa with the listed sources
# Adds sources to the Solution Explorer
add_executable(isimpa WIN32 MACOSX_BUNDLE
    ${3DENGINE_SOURCES}
    ${3DENGINE_CORE_SOURCES}
    ${3DENGINE_TOOLS_SOURCES}
    ${GL_SOURCES}
    ${WIN32_SOURCES}
    ${IHM_SOURCES}
    ${LANGUAGE_SOURCES}
    ${DATA_MANAGER_SOURCES}
    ${DATA_MANAGER_CUSTOMEDIT_SOURCES}
    ${DATA_MANAGER_GENERICELEM_SOURCES}
    ${DATA_MANAGER_GRPINFO_SOURCES}
    ${DATA_MANAGER_PYINTERFACE_SOURCES}
    ${DATA_MANAGER_TREECORE_SOURCES}
    ${DATA_MANAGER_TREERAPPORT_SOURCES}
    ${DATA_MANAGER_TREESCENE_SOURCES}
    ${DATA_MANAGER_TREEUSER_SOURCES}
    ${MAIN_SOURCES}
    ${MANAGER_SOURCES}
    ${ISIMPA_SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/generated/enum_def.cpp
	${CMAKE_CURRENT_BINARY_DIR}/generated/appconfig_version.cpp
    ${RESOURCE_FILES})

if(WIN32)
    set(RESOURCE_DEST_DIR "$<TARGET_FILE_DIR:isimpa>")
elseif(APPLE)
    set(RESOURCE_DEST_DIR "$<TARGET_FILE_DIR:isimpa>/../Resources")
else ()
    set(RESOURCE_DEST_DIR "$<TARGET_FILE_DIR:isimpa>/resources")
endif()

# to convert .po files into .mo files
FIND_PACKAGE(Gettext)
IF(GETTEXT_FOUND )
    MESSAGE(STATUS "Found Gettext")
    # Main isimpa locale
    ADDMULTIPLE_LANG(${CMAKE_CURRENT_SOURCE_DIR}/lang "${RESOURCE_DEST_DIR}/share/locale")
    # Plugins locale
    ADDMULTIPLE_LANG(${CMAKE_CURRENT_SOURCE_DIR}/resources/SystemScript/check_version/locale "${RESOURCE_DEST_DIR}/SystemScript/check_version/locale")
    ADDMULTIPLE_LANG(${CMAKE_CURRENT_SOURCE_DIR}/resources/SystemScript/job_tool/locale "${RESOURCE_DEST_DIR}/SystemScript/job_tool/locale")
    ADDMULTIPLE_LANG(${CMAKE_CURRENT_SOURCE_DIR}/resources/SystemScript/moveto_vertex/locale "${RESOURCE_DEST_DIR}/SystemScript/moveto_vertex/locale")
    ADDMULTIPLE_LANG(${CMAKE_CURRENT_SOURCE_DIR}/resources/SystemScript/preceiv_sourceTracker/locale "${RESOURCE_DEST_DIR}/SystemScript/preceiv_sourceTracker/locale")
    ADDMULTIPLE_LANG(${CMAKE_CURRENT_SOURCE_DIR}/resources/SystemScript/recp_tool/locale "${RESOURCE_DEST_DIR}/SystemScript/recp_tool/locale")
    ADDMULTIPLE_LANG(${CMAKE_CURRENT_SOURCE_DIR}/resources/SystemScript/source_tools/locale "${RESOURCE_DEST_DIR}/SystemScript/source_tools/locale")
ELSE ()
    MESSAGE(WARNING "Gettext not found !")
ENDIF()

# Creates a folder "executables" and adds target
# project (isimpa.vcproj) under it
set_property(TARGET isimpa PROPERTY FOLDER "executables")

set_target_properties(isimpa PROPERTIES MACOSX_BUNDLE_ICON_FILE "isimpa.icns")

SET(wxUSE_GLCANVAS_EGL OFF)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(wxWidgets_USE_DEBUG ON)
    SET(wxWidgets_USE_STATIC ON)
    MESSAGE(STATUS "Using debug version of wxWidgets")
ELSE()
    SET(wxWidgets_USE_DEBUG OFF)
    SET(wxWidgets_USE_STATIC ON)
ENDIF()

# add wxWidgets as dependency

set(USING_CPM false)

find_package(wxWidgets 3.3.1)

if(wxWidgets_FOUND)

    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        if(NOT ${wxWidgets_CONFIGURATION} MATCHES .*d$ )
            set(wxWidgets_CONFIGURATION ${wxWidgets_CONFIGURATION}d)
        endif()
    else()
        if(${wxWidgets_CONFIGURATION} MATCHES .*d$ )
            string(LENGTH ${wxWidgets_CONFIGURATION} BUILD_STR_SIZE)
            MATH(EXPR BUILD_STR_SIZE "${BUILD_STR_SIZE}-1")
            string(SUBSTRING ${wxWidgets_CONFIGURATION} 0 ${BUILD_STR_SIZE} wxWidgets_CONFIGURATION)
        endif()
    endif()

    find_package(wxWidgets COMPONENTS core base xml gl aui adv html REQUIRED)

    include(${wxWidgets_USE_FILE})
else(NOT wxWidgets_FOUND)
    set(USING_CPM true)
    set(BUILD_SHARED_LIBS OFF)
    set(wx_options
            "wxBUILD_SAMPLES OFF"
            "wxBUILD_TESTS OFF"
            "wxBUILD_SHARED OFF"
            "wxUSE_OPENGL ON"
            "wxUSE_UNICODE ON"
            "wxUSE_LIBTIFF OFF"
            "wxUSE_GLCANVAS_EGL ON"
            "wxUSE_LIBWEBP OFF"
            #"wxBUILD_INSTALL OFF"
    )

    if(APPLE)
        # issue https://github.com/wxWidgets/wxWidgets/issues/24687
        list(APPEND wx_options "wxBUILD_PRECOMP OFF")
    endif()
    CPMAddPackage(
            NAME wxWidgets
            URL https://github.com/wxWidgets/wxWidgets/releases/download/v3.3.1/wxWidgets-3.3.1.tar.bz2
            URL_HASH SHA256=f936c8d694f9c49a367a376f99c751467150a4ed7cbf8f4723ef19b2d2d9998d
            OPTIONS ${wx_options}
    )
endif()

execute_process(COMMAND ${Python3_EXECUTABLE} -c "import sys; import sysconfig; print(list(sysconfig.get_paths().values()))"
                    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    FIND_PACKAGE(OpenGL REQUIRED COMPONENTS OpenGL)
else()
    FIND_PACKAGE(OpenGL REQUIRED COMPONENTS OpenGL EGL)
endif()

if(APPLE)
    set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)
endif ()

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${wxWidgets_SOURCE_DIR})
# Include Python headers
include_directories(${Python3_INCLUDE_DIRS})

SetupBoost()

# Properties->Linker->Input->Additional Dependencies
target_link_libraries(isimpa
        lib_interface
        Python3::Python
        Boost::python
        Boost::thread
)

if(APPLE)
    if(CMAKE_OBJCXX_LINK_LIBRARY_USING_FRAMEWORK_SUPPORTED)
        target_link_libraries(isimpa PRIVATE "$<LINK_LIBRARY:FRAMEWORK,CoreText>")
    endif()

    target_include_directories(isimpa PRIVATE ${CMAKE_OSX_SYSROOT}/System/Library/Frameworks/CoreText.framework/Headers)
endif ()

if(USING_CPM)
    target_link_libraries(isimpa wx::core wx::base wx::aui wx::adv wx::xml wx::html wx::gl)
else()
    target_link_libraries(isimpa ${wxWidgets_LIBRARIES})
endif()

target_link_libraries(isimpa OpenGL::GL OpenGL::GLU)

add_definitions(-DUSE_PYTHON=${PYTHONLIBS_FOUND})

if(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	# Source files are encoded in UTF-8
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /source-charset:utf-8")
    set_target_properties(isimpa PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(isimpa PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
ENDIF()

##################################
## Prepare build directory expected dependencies files for binaries and resources


if(NOT APPLE)
    # Set resource source and destination variables
    set(RESOURCE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")

    # Copy resources directory POST_BUILD
    add_custom_command(
            TARGET isimpa POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            "${RESOURCE_SRC_DIR}" "${RESOURCE_DEST_DIR}"
            COMMENT "Copying resources to ${RESOURCE_DEST_DIR}"
    )
endif ()
    if(WIN32)

        # Copy python libraries
        get_filename_component(PYTHON_ROOT_FOLDER ${Python3_EXECUTABLE} PATH)

        # Copy directories after build
        add_custom_command(TARGET isimpa POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PYTHON_ROOT_FOLDER}/Lib"
                "$<TARGET_FILE_DIR:isimpa>/Lib"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PYTHON_ROOT_FOLDER}/DLLs"
                "$<TARGET_FILE_DIR:isimpa>/DLLs"
                COMMAND ${CMAKE_COMMAND} -E copy
                "${PYTHON_ROOT_FOLDER}/python.exe"
                "$<TARGET_FILE_DIR:isimpa>/python.exe"
        )

    # Copy all .dll files from python root folder after build
    file(GLOB PYTHON_DLL_FILES "${PYTHON_ROOT_FOLDER}/*.dll")

    foreach(dll ${PYTHON_DLL_FILES})
        add_custom_command(TARGET isimpa POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll}"
                "$<TARGET_FILE_DIR:isimpa>/"
        )
    endforeach()
endif ()

# Optionally copy runtime libraries after build
if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    add_custom_command(TARGET isimpa POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
            "$<TARGET_FILE_DIR:isimpa>/"
    )
endif()

add_custom_command(TARGET isimpa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_FILE_DIR:libsimpa>"
        "$<TARGET_FILE_DIR:isimpa>/libsimpa"
        COMMAND ${CMAKE_COMMAND} -E rm -f
        "$<TARGET_FILE_DIR:isimpa>/libsimpa/*.cmake"
        COMMAND ${CMAKE_COMMAND} -E rm -rf
        "$<TARGET_FILE_DIR:isimpa>/libsimpa/CMakeFiles"
        COMMENT "Copying libsimpa to isimpa binary directory"
)

add_custom_command(TARGET isimpa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:spps>"
        "$<TARGET_FILE_DIR:isimpa>/"
        COMMENT "Copying spps binary to isimpa binary directory"
)

add_custom_command(TARGET isimpa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:vmr>"
        "$<TARGET_FILE_DIR:isimpa>"
        COMMENT "Copying vmr to isimpa binary directory"
)

add_custom_command(TARGET isimpa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:tetgen>"
        "$<TARGET_FILE_DIR:isimpa>"
        COMMENT "Copying tetgen to isimpa binary directory"
)

add_custom_command(TARGET isimpa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:classicalTheory>"
        "$<TARGET_FILE_DIR:isimpa>"
        COMMENT "Copying classicalTheory to isimpa binary directory"
)

add_custom_command(TARGET isimpa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:preprocess>"
        "$<TARGET_FILE_DIR:isimpa>"
        COMMENT "Copying preprocess to isimpa binary directory"
)

#######################################################################
# Prepare install commands for CPack builder

# Install executable
install(TARGETS isimpa
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${ISIMPA_BIN_DIR}
        LIBRARY DESTINATION ${ISIMPA_BIN_DIR}
        ARCHIVE DESTINATION ${ISIMPA_BIN_DIR})

if(APPLE)
    # CPack bundle plist location after install
    SET(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_BINARY_DIR}/isimpa.app/Contents/Info.plist")
    SET(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_BINARY_DIR}/isimpa.app/Contents/Resources/isimpa.icns")

elseif(WIN32)
    # Install resources
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
            DESTINATION "${ISIMPA_DATA_DIR}"
    )

    # Install Python runtime (Windows)
    get_filename_component(PYTHON_ROOT_FOLDER ${Python3_EXECUTABLE} PATH)
    install(DIRECTORY "${PYTHON_ROOT_FOLDER}/Lib" DESTINATION "${ISIMPA_BIN_DIR}")
    install(DIRECTORY "${PYTHON_ROOT_FOLDER}/DLLs" DESTINATION "${ISIMPA_BIN_DIR}")
    install(PROGRAMS "${PYTHON_ROOT_FOLDER}/python.exe" DESTINATION "${ISIMPA_BIN_DIR}")

    file(GLOB PYTHON_DLL_FILES "${PYTHON_ROOT_FOLDER}/*.dll")
    install(FILES ${PYTHON_DLL_FILES} DESTINATION "${ISIMPA_BIN_DIR}")

    # Install system runtime libraries
    if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
        install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "${ISIMPA_BIN_DIR}")
    endif()
endif()

if(APPLE AND NOT DEFINED CPACK_GENERATOR)
    SET(CPACK_GENERATOR "DragNDrop")
endif()

set(CPACK_SOURCE_IGNORE_FILES
        /bin/wx*
        /lib/cmake/wxWidgets
        /lib/wx
        /lib/*.a
)

SET(CPACK_BUNDLE_NAME "isimpa")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "isimpa-${CMAKE_PROJECT_VERSION}")
SET(CPACK_PACKAGE_DIRECTORY distribution)
SET(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "I-SIMPA")
SET(CPACK_PACKAGE_VENDOR "UMRAE, CEREMA, Univ Gustave Eiffel")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/DESCRIPTION.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
SET(CPACK_PACKAGE_VERSION_MAJOR ${isimpa_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${isimpa_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${isimpa_VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "I-SIMPA")

SET(CPACK_PACKAGE_EXECUTABLES "isimpa" "I-SIMPA")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nicolas Fortin")

INCLUDE(CPack)
