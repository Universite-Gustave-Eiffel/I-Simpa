macro(InitializeISIMPABuildOptions)
    # ==============================================================================
    # =================   All the options for the build process    =================
    # ==============================================================================

    option(BUILD_FORCE_DIRECTORY "The build directory must be different to the source directory." OFF)
    option(SKIPISIMPA "Do not build ISIMPA Gui." OFF)

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
