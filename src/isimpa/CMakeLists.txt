include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

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
    GL/wglext.h
    GL/offScreenRendering.cpp
    GL/offScreenRendering.h)

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
    data_manager/customEditor/wxGridCellTreeEditor.cpp
    data_manager/customEditor/wxGridCellTreeEditor.h)

set(DATA_MANAGER_GENERICELEM_SOURCES
    data_manager/generic_element/e_directivity.h
    data_manager/generic_element/e_directivity.cpp
    data_manager/generic_element/e_directivity_user.h
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
    data_manager/tree_scene/e_scene_sources_source_proprietes.h
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
    ../preprocess/input_output/poly.cpp
    ../preprocess/input_output/poly.h
    UtfConverter.cpp
    UtfConverter.h
    ConvertUTF.c
    ConvertUTF.h
    enum_def.cpp)

source_group( "3dengine" FILES ${3DENGINE_SOURCES} )
source_group( "3dengine\\Core" FILES ${3DENGINE_CORE_SOURCES} )
source_group( "3dengine\\tools" FILES ${3DENGINE_TOOLS_SOURCES} )

source_group( "GL" FILES ${GL_SOURCES} )
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


set(Boost_USE_STATIC_LIBS   ON) 
find_package(Boost REQUIRED COMPONENTS system python)

find_package(wxWidgets COMPONENTS core base xml gl aui adv tiff REQUIRED)
include(${wxWidgets_USE_FILE})
# Find package for building
FIND_PACKAGE(PythonLibs)

find_package(OpenGL REQUIRED)
# Build auto-generated source files
# Find package for generation
FIND_PACKAGE(PythonInterp REQUIRED)

# Ask make to call python in order to use element.h to produce enum_def.cpp
ADD_CUSTOM_COMMAND(
  OUTPUT "enum_def.cpp"
  COMMAND "${PYTHON_EXECUTABLE}" ${PROJECT_SOURCE_DIR}/src/isimpa/data_manager/python_interface/py_ui_module/generate_enum_def.py
  ARGS    "${PROJECT_SOURCE_DIR}/src/isimpa/data_manager/element.h" > "enum_def.cpp"
  DEPENDS "data_manager/element.h"
  )
# Before using element_pywrap.cpp build enum_def.cpp
set_property(SOURCE data_manager/python_interface/py_ui_module/element_pywrap.cpp APPEND PROPERTY OBJECT_DEPENDS enum_def.cpp)


include_directories( ${OPENGL_INCLUDE_DIRS})

link_directories( ${Boost_LIBRARY_DIRS} )  
include_directories( ${Boost_INCLUDE_DIRS} )  
include_directories(${PYTHON_INCLUDE_PATH})

# Set Properties->General->Configuration Type to Application(.exe)
# Creates isimpa.exe with the listed sources
# Adds sources to the Solution Explorer
add_executable(isimpa 
    ${3DENGINE_SOURCES}
    ${3DENGINE_CORE_SOURCES}
    ${3DENGINE_TOOLS_SOURCES}
    ${GL_SOURCES}
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
    ${ISIMPA_SOURCES})

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

# Creates a folder "executables" and adds target
# project (isimpa.vcproj) under it
set_property(TARGET isimpa PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy isimpa.exe to destination directory
install (TARGETS isimpa
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
        
set(ISIMPA_RESSOURCES ${PROJECT_SOURCE_DIR}/currentRelease)         
        
install (FILES 
${ISIMPA_RESSOURCES}/appconst.xml
${ISIMPA_RESSOURCES}/isimpa.ico
${ISIMPA_RESSOURCES}/Licence.pdf
${ISIMPA_RESSOURCES}/Licence.rtf
         DESTINATION  ${PROJECT_BINARY_DIR}/bin)
         
install(DIRECTORY 
${ISIMPA_RESSOURCES}/Bitmaps
${ISIMPA_RESSOURCES}/Doc
${ISIMPA_RESSOURCES}/Locale
${ISIMPA_RESSOURCES}/SystemScript
${ISIMPA_RESSOURCES}/UserScript
DESTINATION  ${PROJECT_BINARY_DIR}/bin)


install (FILES 
${ISIMPA_RESSOURCES}/libsimpa/__init__.py
         DESTINATION  ${PROJECT_BINARY_DIR}/bin/libsimpa)
        