macro(InitializeISIMPABuildOptions)
    # ==============================================================================
    # =================   All the options for the build process    =================
    # ==============================================================================

    option(BUILD_FORCE_DIRECTORY "The build directory must be different to the source directory." OFF)
    option(BUILD_GUI "Build ISIMPA Gui. Otherwise you have only the command line and the Python import module." ON)
    option(ISIMPA_USE_EXTERNAL_ZIPIOS "Use system installed zipios++ instead of the bundled." OFF)
    option(ISIMPA_USE_EXTERNAL_SMESH "Use system installed smesh instead of the bundled." OFF)
    option(ISIMPA_USE_EXTERNAL_KDL "Use system installed orocos-kdl instead of the bundled." OFF)
    option(ISIMPA_USE_EXTERNAL_FMT "Use system installed fmt library if available instead of fetching the source." ON)
    option(ISIMPA_USE_EXTERNAL_ONDSELSOLVER "Use system installed OndselSolver instead of git submodule." OFF)
    option(ISIMPA_USE_FREETYPE "Builds the features using FreeType libs" ON)
    option(ISIMPA_BUILD_DEBIAN "Prepare for a build of a Debian package" OFF)
    option(BUILD_WITH_CONDA "Set ON if you build ISIMPA with conda" OFF)
    option(BUILD_DYNAMIC_LINK_PYTHON "If OFF extension-modules do not link against python-libraries" ON)
    option(INSTALL_TO_SITEPACKAGES "If ON the ISIMPA root namespace (python) is installed into python's site-packages" ON)
    option(OCCT_CMAKE_FALLBACK "disable usage of occt-config files" OFF)
    if (WIN32 OR APPLE)
        option(ISIMPA_USE_QT_FILEDIALOG "Use Qt's file dialog instead of the native one." OFF)
    else()
        option(ISIMPA_USE_QT_FILEDIALOG "Use Qt's file dialog instead of the native one." ON)
    endif()

    # == Win32 is default behaviour use the LibPack copied in Source tree ==========
    if(MSVC)
        option(ISIMPA_RELEASE_PDB "Create PDB files for Release version." ON)
        option(ISIMPA_RELEASE_SEH "Enable Structured Exception Handling for Release version." ON)
        option(ISIMPA_LIBPACK_USE "Use the LibPack to Build ISIMPA (only Win32 so far)." ON)
        option(ISIMPA_USE_PCH "Activate precompiled headers where it's used." ON)

        if (DEFINED ENV{ISIMPA_LIBPACK_DIR})
            set(ISIMPA_LIBPACK_DIR $ENV{ISIMPA_LIBPACK_DIR} CACHE PATH  "Directory of the ISIMPA LibPack")
            message(STATUS "Found libpack env variable: ${ISIMPA_LIBPACK_DIR}")
        else()
            set(ISIMPA_LIBPACK_DIR ${CMAKE_SOURCE_DIR} CACHE PATH  "Directory of the ISIMPA LibPack")
        endif()

        set(LIBPACK_FOUND OFF )
        if (EXISTS ${ISIMPA_LIBPACK_DIR}/plugins/imageformats/qsvg.dll)
            set(LIBPACK_FOUND ON )
            set(COPY_LIBPACK_BIN_TO_BUILD OFF )
            # Create install commands for dependencies for INSTALL target in ISIMPA solution
            option(ISIMPA_INSTALL_DEPEND_DIRS "Create install dependency commands for the INSTALL target found
                in the ISIMPA solution." ON)
            # Copy libpack smaller dependency folders to build folder per user request - if non-existent at destination
            if (NOT EXISTS ${CMAKE_BINARY_DIR}/bin/imageformats/qsvg.dll)
                option(ISIMPA_COPY_DEPEND_DIRS_TO_BUILD "Copy smaller libpack dependency directories to build directory." OFF)
            endif()
            # Copy libpack 'bin' directory contents to build 'bin' per user request - only IF NOT EXISTS already
            if (NOT EXISTS ${CMAKE_BINARY_DIR}/bin/DLLs)
                set(COPY_LIBPACK_BIN_TO_BUILD ON )
                option(ISIMPA_COPY_LIBPACK_BIN_TO_BUILD "Copy larger libpack dependency 'bin' folder to the build directory." OFF)
                # Copy only the minimum number of files to get a working application
                option(ISIMPA_COPY_PLUGINS_BIN_TO_BUILD "Copy plugins to the build directory." OFF)
            endif()
        else()
            message(WARNING Could not locate ${ISIMPA_LIBPACK_DIR}/plugins/imageformats/qsvg.dll)
            message("Libpack NOT found.\nIf you intend to use a Windows libpack, set the ISIMPA_LIBPACK_DIR to the libpack directory.")
            message(STATUS "Visit: https://github.com/ISIMPA/ISIMPA-Libpack/releases/ for Windows libpack downloads.")
        endif()
    elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
        option(ISIMPA_WARN_ERROR "Make all warnings into errors. " OFF)
    else(MSVC)
        option(ISIMPA_LIBPACK_USE "Use the LibPack to Build ISIMPA (only Win32 so far)." OFF)
        set(ISIMPA_LIBPACK_DIR ""  CACHE PATH  "Directory of the ISIMPA LibPack")
    endif(MSVC)

    ChooseQtVersion()

    # https://blog.kitware.com/constraining-values-with-comboboxes-in-cmake-cmake-gui/
    set(ISIMPA_USE_OCC_VARIANT "Community Edition"  CACHE STRING  "Official OpenCASCADE version or community edition")
    set_property(CACHE ISIMPA_USE_OCC_VARIANT PROPERTY STRINGS
                 "Official Version"
                 "Community Edition"
    )

    configure_file(${CMAKE_SOURCE_DIR}/src/QtOpenGL.h.cmake ${CMAKE_BINARY_DIR}/src/QtOpenGL.h)

    option(BUILD_DESIGNER_PLUGIN "Build and install the designer plugin" OFF)

    if(APPLE)
        option(ISIMPA_CREATE_MAC_APP "Create app bundle on install" OFF)

        if(ISIMPA_CREATE_MAC_APP)
            install(
                DIRECTORY ${CMAKE_SOURCE_DIR}/src/MacAppBundle/ISIMPA.app/
                DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app
            )

            # It should be safe to assume we've got sed on OSX...
            install(CODE "
                execute_process(COMMAND
                    sed -i \"\" -e s/VERSION_STRING_FROM_CMAKE/${PACKAGE_VERSION}/
                    -e s/NAME_STRING_FROM_CMAKE/${PROJECT_NAME}/
                    ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app/Contents/Info.plist)
                   ")

            set(CMAKE_INSTALL_PREFIX
                ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app/Contents)
            set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib )
        endif(ISIMPA_CREATE_MAC_APP)
        set(CMAKE_MACOSX_RPATH TRUE )
    endif(APPLE)

    option(BUILD_FEM "Build the ISIMPA FEM module" ON)
    option(BUILD_SANDBOX "Build the ISIMPA Sandbox module which is only for testing purposes" OFF)
    option(BUILD_TEMPLATE "Build the ISIMPA template module which is only for testing purposes" OFF)
    option(BUILD_ADDONMGR "Build the ISIMPA addon manager module" ON)
    option(BUILD_BIM "Build the ISIMPA BIM module" ON)
    option(BUILD_DRAFT "Build the ISIMPA draft module" ON)
    option(BUILD_DRAWING "Build the ISIMPA drawing module" OFF)
    option(BUILD_HELP "Build the ISIMPA help module" ON)
    option(BUILD_IDF "Build the ISIMPA idf module" ON)
    option(BUILD_IMPORT "Build the ISIMPA import module" ON)
    option(BUILD_INSPECTION "Build the ISIMPA inspection module" ON)
    option(BUILD_JTREADER "Build the ISIMPA jt reader module" OFF)
    option(BUILD_MATERIAL "Build the ISIMPA material module" ON)
    option(BUILD_MESH "Build the ISIMPA mesh module" ON)
    option(BUILD_MESH_PART "Build the ISIMPA mesh part module" ON)
    option(BUILD_FLAT_MESH "Build the ISIMPA flat mesh module" ON)
    option(BUILD_OPENSCAD "Build the ISIMPA openscad module" ON)
    option(BUILD_PART "Build the ISIMPA part module" ON)
    option(BUILD_PART_DESIGN "Build the ISIMPA part design module" ON)
    option(BUILD_CAM "Build the ISIMPA CAM module" ON)
    option(BUILD_ASSEMBLY "Build the ISIMPA Assembly module" ON)
    option(BUILD_PLOT "Build the ISIMPA plot module" ON)
    option(BUILD_POINTS "Build the ISIMPA points module" ON)
    option(BUILD_REVERSEENGINEERING "Build the ISIMPA reverse engineering module" ON)
    option(BUILD_ROBOT "Build the ISIMPA robot module" ON)
    option(BUILD_SHOW "Build the ISIMPA Show module (helper module for visibility automation)" ON)
    option(BUILD_SKETCHER "Build the ISIMPA sketcher module" ON)
    option(BUILD_SPREADSHEET "Build the ISIMPA spreadsheet module" ON)
    option(BUILD_START "Build the ISIMPA start module" ON)
    option(BUILD_TEST "Build the ISIMPA test module" ON)
    option(BUILD_MEASURE "Build the ISIMPA Measure module" ON)
    option(BUILD_TECHDRAW "Build the ISIMPA Technical Drawing module" ON)
    option(BUILD_TUX "Build the ISIMPA Tux module" ON)
    option(BUILD_WEB "Build the ISIMPA Web module" ON)
    option(BUILD_SURFACE "Build the ISIMPA surface module" ON)
    option(BUILD_VR "Build the ISIMPA Oculus Rift support (need Oculus SDK 4.x or higher)" OFF)
    option(BUILD_CLOUD "Build the ISIMPA cloud module" OFF)
    option(ENABLE_DEVELOPER_TESTS "Build the ISIMPA unit tests suit" ON)

    if(MSVC)
        set(ISIMPA_3DCONNEXION_SUPPORT "NavLib" CACHE STRING "Select version of the 3Dconnexion device integration")
        set_property(CACHE ISIMPA_3DCONNEXION_SUPPORT PROPERTY STRINGS "NavLib" "Raw input")
    else(MSVC)
        set(ISIMPA_3DCONNEXION_SUPPORT "Raw input")
    endif(MSVC)

    if(MSVC)
        option(BUILD_FEM_NETGEN "Build the ISIMPA FEM module with the NETGEN mesher" ON)
        option(ISIMPA_USE_PCL "Build the features that use PCL libs" OFF) # 3/5/2021 current LibPack uses non-C++17 FLANN
        option(ISIMPA_USE_3DCONNEXION "Use the 3D connexion SDK to support 3d mouse." ON)
    elseif(APPLE)
        find_library(3DCONNEXIONCLIENT_FRAMEWORK 3DconnexionClient)
        if(IS_DIRECTORY ${3DCONNEXIONCLIENT_FRAMEWORK})
            option(ISIMPA_USE_3DCONNEXION "Use the 3D connexion SDK to support 3d mouse." ON)
        else(IS_DIRECTORY ${3DCONNEXIONCLIENT_FRAMEWORK})
            option(ISIMPA_USE_3DCONNEXION "Use the 3D connexion SDK to support 3d mouse." OFF)
        endif(IS_DIRECTORY ${3DCONNEXIONCLIENT_FRAMEWORK})
    else(MSVC)
        set(ISIMPA_USE_3DCONNEXION OFF )
    endif(MSVC)
    if(NOT MSVC)
        option(BUILD_FEM_NETGEN "Build the ISIMPA FEM module with the NETGEN mesher" OFF)
        option(ISIMPA_USE_PCL "Build the features that use PCL libs" OFF)
    endif(NOT MSVC)

    if(ISIMPA_3DCONNEXION_SUPPORT STREQUAL "NavLib" AND ISIMPA_USE_3DCONNEXION)
        set(ISIMPA_USE_3DCONNEXION_NAVLIB ON)
        set(ISIMPA_USE_3DCONNEXION OFF)
    endif()

    # if this is set override some options
    if (ISIMPA_BUILD_DEBIAN)
        set(ISIMPA_USE_EXTERNAL_ZIPIOS ON )
        # A Debian package for SMESH doesn't exist
        #set(ISIMPA_USE_EXTERNAL_SMESH ON )
    endif (ISIMPA_BUILD_DEBIAN)

    if(BUILD_FEM)
        set(BUILD_SMESH ON )
    endif()

    # for Windows the minimum required cmake version is 3.4.3 to build the CAM module
    if(WIN32 AND CMAKE_VERSION VERSION_LESS 3.4.3)
        message(WARNING "Disable CAM, requires cmake >= 3.4.3 in order to build this module")
        set(BUILD_CAM OFF )
    endif()

    # force build directory to be different to source directory
    if (BUILD_FORCE_DIRECTORY)
        if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
            message(FATAL_ERROR "The build directory (${CMAKE_BINARY_DIR}) must be different to the source directory (${CMAKE_SOURCE_DIR}).\n"
                                "Please choose another build directory! Or disable the option BUILD_FORCE_DIRECTORY.")
        endif()
    endif()
endmacro(InitializeISIMPABuildOptions)
