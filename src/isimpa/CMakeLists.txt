include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")
set(CMAKE_INSTALL_MFC_LIBRARIES ON)
include( InstallRequiredSystemLibraries )

MACRO(ADDMULTIPLE_LANG curdir bindir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        MESSAGE(STATUS "Add lang ${curdir}/${child}  ${bindir}${child}")
        add_subdirectory(${curdir}/${child} ${bindir}${child})
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
    3dengine/Core/ply.cpp
    3dengine/Core/ply.h
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
    GL/opengl_inc.h
    GL/wglext.h)

if(WIN32) # Check if we are on Windows
  set(WIN32_SOURCES
    GL/offScreenRendering.cpp
    GL/offScreenRendering.h
	resources/isimpa.rc
      )
else()
  set(WIN32_SOURCES )
endif()

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
    IHM/applicationPreferences.cpp
    IHM/applicationPreferences.hpp
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
    IHM/uiTreeCtrl.h)

set(LANGUAGE_SOURCES
    Language/additionnal_univ.h)

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
    data_manager/tree_core/e_core_sppsoctreecore.h
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
source_group( "3dengine\\Core" FILES ${3DENGINE_CORE_SOURCES} )
source_group( "3dengine\\tools" FILES ${3DENGINE_TOOLS_SOURCES} )

source_group( "GL" FILES ${GL_SOURCES} )
source_group( "WIN32" FILES ${WIN32_SOURCES})
source_group( "IHM" FILES ${IHM_SOURCES} )
source_group( "Language" FILES ${LANGUAGE_SOURCES} )

source_group( "data_manager" FILES ${DATA_MANAGER_SOURCES} )
source_group( "data_manager\\customEditor" FILES ${DATA_MANAGER_CUSTOMEDIT_SOURCES} )
source_group( "data_manager\\generic_element" FILES ${DATA_MANAGER_GENERICELEM_SOURCES} )
source_group( "data_manager\\grpInfo" FILES ${DATA_MANAGER_GRPINFO_SOURCES} )
source_group( "data_manager\\python_interface" FILES ${DATA_MANAGER_PYINTERFACE_SOURCES} )
source_group( "data_manager\\tree_core" FILES ${DATA_MANAGER_TREECORE_SOURCES} )
source_group( "data_manager\\tree_rapport" FILES ${DATA_MANAGER_TREERAPPORT_SOURCES} )
source_group( "data_manager\\tree_scene" FILES ${DATA_MANAGER_TREESCENE_SOURCES} )
source_group( "data_manager\\tree_userpref" FILES ${DATA_MANAGER_TREEUSER_SOURCES} )

source_group( "main" FILES ${MAIN_SOURCES} )
source_group( "manager" FILES ${MANAGER_SOURCES} )
source_group( "" FILES ${ISIMPA_SOURCES} )

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

# Build auto-generated source files
# Find package for generation
FIND_PACKAGE(PythonInterp 2.7 REQUIRED)

# Ask make to call python in order to use element.h to produce enum_def.cpp
ADD_CUSTOM_COMMAND(
  OUTPUT "enum_def.cpp"
  COMMAND "${PYTHON_EXECUTABLE}" ${PROJECT_SOURCE_DIR}/src/isimpa/data_manager/python_interface/py_ui_module/generate_enum_def.py
  ARGS    "${PROJECT_SOURCE_DIR}/src/isimpa/data_manager/element.h" > "enum_def.cpp"
  DEPENDS "data_manager/element.h"
  )

# Before using element_pywrap.cpp build enum_def.cpp
set_property(SOURCE data_manager/python_interface/py_ui_module/element_pywrap.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/enum_def.cpp)

configure_file(data_manager/appconfig_version.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/generated/appconfig_version.cpp )

set(Boost_USE_STATIC_LIBS   OFF)
find_package(Boost REQUIRED COMPONENTS system python filesystem regex)
link_directories( ${Boost_LIBRARY_DIRS} )

# Set Properties->General->Configuration Type to Application
# Creates isimpa with the listed sources
# Adds sources to the Solution Explorer
add_executable(isimpa
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
    ${CMAKE_CURRENT_BINARY_DIR}/enum_def.cpp
	${CMAKE_CURRENT_BINARY_DIR}/generated/appconfig_version.cpp)

# Creates a folder "executables" and adds target
# project (isimpa.vcproj) under it
set_property(TARGET isimpa PROPERTY FOLDER "executables")

# to convert .po files into .mo files
FIND_PACKAGE(Gettext)
IF(GETTEXT_FOUND )
  MESSAGE(STATUS "Found Gettext")
  # Main isimpa locale
  add_subdirectory (lang/fr)
  add_subdirectory (lang/pl)
  # Plugins locale
  ADDMULTIPLE_LANG(${PROJECT_SOURCE_DIR}/currentRelease/SystemScript/job_tool/locale ${CMAKE_BINARY_DIR}/src/job_tool)
  ADDMULTIPLE_LANG(${PROJECT_SOURCE_DIR}/currentRelease/SystemScript/recp_tool/locale ${CMAKE_BINARY_DIR}/src/recp_tool)
  ADDMULTIPLE_LANG(${PROJECT_SOURCE_DIR}/currentRelease/SystemScript/source_tools/locale ${CMAKE_BINARY_DIR}/src/source_tools)
  ADDMULTIPLE_LANG(${PROJECT_SOURCE_DIR}/currentRelease/SystemScript/moveto_vertex/locale ${CMAKE_BINARY_DIR}/src/moveto_vertex)
  ADDMULTIPLE_LANG(${PROJECT_SOURCE_DIR}/currentRelease/SystemScript/preceiv_sourceTracker/locale ${CMAKE_BINARY_DIR}/src/preceiv_sourceTracker)
  #disabled ADDMULTIPLE_LANG(${PROJECT_SOURCE_DIR}/currentRelease/SystemScript/SppsReportSample/locale ${CMAKE_BINARY_DIR}/src/SppsReportSample)
  # Building isimpa require building pofiles
  add_dependencies(isimpa pofiles)
ENDIF()

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(wxWidgets_USE_DEBUG ON)
    SET(wxWidgets_USE_STATIC OFF)
    MESSAGE(STATUS "Using debug version of wxWidgets")
ELSE()
    SET(wxWidgets_USE_DEBUG OFF)
    SET(wxWidgets_USE_STATIC OFF)
    MESSAGE(STATUS "Using dynamic wxWidgets library  lib:${wxWidgets_LIB_DIR} include:${wxWidgets_ROOT_DIR}")
ENDIF()

find_package(wxWidgets COMPONENTS core base xml gl aui adv REQUIRED)
include(${wxWidgets_USE_FILE})
# Find package for building
FIND_PACKAGE(PythonLibs)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories(${PYTHON_INCLUDE_PATH})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries(isimpa lib_interface)
target_link_libraries(isimpa ${Boost_LIBRARIES})
target_link_libraries(isimpa ${PYTHON_LIBRARIES})
target_link_libraries(isimpa ${wxWidgets_LIBRARIES})
target_link_libraries(isimpa ${OPENGL_LIBRARIES})

add_definitions(-DUSE_PYTHON=${PYTHONLIBS_FOUND})

if(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set_target_properties(isimpa PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(isimpa PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
ENDIF()

#--------------#
#    INSTALL
#--------------#

# Adds logic to INSTALL.vcproj to copy isimpa to destination directory
install (TARGETS isimpa
        RUNTIME DESTINATION .)

set(ISIMPA_RESSOURCES ${PROJECT_SOURCE_DIR}/currentRelease)

install (FILES
${ISIMPA_RESSOURCES}/appconst.xml
${ISIMPA_RESSOURCES}/Licence.pdf
${ISIMPA_RESSOURCES}/Licence.rtf
isimpa.ico
        DESTINATION .)

install(DIRECTORY
${ISIMPA_RESSOURCES}/Bitmaps
${ISIMPA_RESSOURCES}/doc
${ISIMPA_RESSOURCES}/SystemScript
${ISIMPA_RESSOURCES}/UserScript
${ISIMPA_RESSOURCES}/Directivities
DESTINATION .)

if(WIN32) # Check if we are on Windows
    # Copy python libraries
    get_filename_component(PYTHON_ROOT_FOLDER ${PYTHON_EXECUTABLE} PATH)
	set(PYTHON_LIB_FOLDER ${PYTHON_ROOT_FOLDER}/Lib)

    #Fetch required Dll's for GUI
    string(REPLACE "\\" "/" wxWidgets_LIB_DIR_SLASH ${wxWidgets_LIB_DIR})
    set(DIRS ${Boost_LIBRARY_DIR_DEBUG}
             ${Boost_LIBRARY_DIR_RELEASE}
             ${wxWidgets_LIB_DIR_SLASH}
    )

    install(CODE "
    include(BundleUtilities)

    # Print all dependencies
    #GET_PREREQUISITES(\"\${CMAKE_CURRENT_LIST_DIR}/\${BUILD_TYPE}/isimpa.exe\" ISIMPA_DEPENDENCIES 1 0 \${CMAKE_INSTALL_PREFIX} \"${DIRS}\" )
    #    MESSAGE(STATUS \"Dependencies \${ISIMPA_DEPENDENCIES}\")

    # Use this to print all available variables
    #get_cmake_property(_variableNames VARIABLES)
    #foreach (_variableName \${_variableNames})
    #    message(STATUS \"\${_variableName}=\${\${_variableName}}\")
    #endforeach()

    FIXUP_BUNDLE(\"\${CMAKE_CURRENT_LIST_DIR}/\${BUILD_TYPE}/isimpa.exe\" \"\"   \"${DIRS}\")

	# Install copied dlls
	FILE(GLOB children \"\${CMAKE_CURRENT_LIST_DIR}/\${BUILD_TYPE}/*.dll\")
	file(INSTALL DESTINATION \"\${CMAKE_INSTALL_PREFIX}/.\" TYPE PROGRAM FILES
    \${children}
    )
")



	install(DIRECTORY
	${PYTHON_ROOT_FOLDER}/Lib
	${PYTHON_ROOT_FOLDER}/DLLs
	DESTINATION .
    PATTERN "Lib/site-packages" EXCLUDE
    PATTERN "*.pdb" EXCLUDE)

    install(FILES "${PYTHON_ROOT_FOLDER}/python.exe" DESTINATION .)

    if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
        install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION . COMPONENT Runtime )
    endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    # Try to fetch x64 version of python dll
    get_filename_component(PYTHON_DLL_FILENAME ${PYTHON_LIBRARY}  NAME_WE)

    SET(CMAKE_FIND_LIBRARY_PREFIXES "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

    find_library(
        PYTHON_DLL
        NAMES ${PYTHON_DLL_FILENAME}
        PATHS ${PYTHON_ROOT_FOLDER}
        NO_DEFAULT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
    )
    if(NOT EXISTS ${PYTHON_DLL})
        MESSAGE(FATAL_ERROR "Python not found here ${PYTHON_DLL_FILENAME}.DLL")
    else()
        MESSAGE(STATUS "Python dll found here ${PYTHON_DLL}")
    endif()

    install(FILES ${PYTHON_DLL} DESTINATION .)

    install(CODE "
    set(INSTALL_DLL \"\${CMAKE_INSTALL_PREFIX}/${PYTHON_DLL_FILENAME}.DLL\")
    file(SHA512 \${INSTALL_DLL} INSTALL_DLL_HASH)
    MESSAGE(STATUS \"INSTALL_DLL_HASH \${INSTALL_DLL_HASH}\")
    ")
endif()

# Package creation instructions, NSIS under windows
if(WIN32 AND NOT DEFINED CPACK_GENERATOR) # Check if we are on Windows
	SET(CPACK_GENERATOR "NSIS;ZIP")
endif()
SET(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "I-SIMPA")
SET(CPACK_PACKAGE_VENDOR "Ifsttar")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/DESCRIPTION.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
SET(CPACK_PACKAGE_VERSION_MAJOR ${isimpa_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${isimpa_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${isimpa_VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "I-SIMPA")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.    # Create the desktop link
  set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  CreateShortCut '$DESKTOP\\\\I_SIMPA.lnk' '$INSTDIR\\\\isimpa.exe' ")
  set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$DESKTOP\\\\I_SIMPA.lnk'")
  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\src\\\\isimpa\\\\isimpa.ico")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "isimpa.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "I-SIMPA")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\i-simpa-wiki.readthedocs.io")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\i-simpa.ifsttar.fr")
  SET(CPACK_NSIS_CONTACT "i-simpa@ifsttar.fr")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES TRUE)
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "isimpa" "I-SIMPA")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nicolas Fortin")

INCLUDE(CPack)
