macro(ISIMPALibpackChecks)
    # ==============================================================================

    if(ISIMPA_LIBPACK_USE)

        # checking for a unique file in LibPack location to make sure the right version of the LibPack is there
        find_file(ISIMPA_LIBPACK_CHECKFILE_CLBUNDLER MANIFEST.db PATHS ${ISIMPA_LIBPACK_DIR} NO_DEFAULT_PATH NO_CACHE)
        find_file(ISIMPA_LIBPACK_CHECKFILE_VERSION ISIMPA_LIBPACK_VERSION PATHS ${ISIMPA_LIBPACK_DIR} NO_DEFAULT_PATH NO_CACHE)

        # don't show them in the GUI
        set(ISIMPA_LIBPACK_CHECKFILE_CLBUNDLER "${ISIMPA_LIBPACK_CHECKFILE_CLBUNDLER}" CACHE INTERNAL "Find libpack")
        set(ISIMPA_LIBPACK_CHECKFILE_VERSION "${ISIMPA_LIBPACK_CHECKFILE_VERSION}" CACHE INTERNAL "Find libpack v3+")

        if (ISIMPA_LIBPACK_CHECKFILE_CLBUNDLER)
            set(ISIMPA_LIBPACK_VERSION "CLbundler" CACHE STRING "Displays if the libpack has been found" FORCE)
            message("Found LibPack v2 (CLBundler) via ${ISIMPA_LIBPACK_CHECKFILE_CLBUNDLER}")
            include(cMake/UseLibPackCLbundler.cmake)
        elseif (ISIMPA_LIBPACK_CHECKFILE_VERSION)
            file(READ ${ISIMPA_LIBPACK_CHECKFILE_VERSION} ISIMPA_LIBPACK_VERSION)
            set(ISIMPA_LIBPACK_VERSION "${ISIMPA_LIBPACK_VERSION}" CACHE STRING "Version of the LibPack, if found" FORCE)
            message("Found LibPack " ${ISIMPA_LIBPACK_VERSION})
            include(cMake/UseLibPack3.cmake)
        else(ISIMPA_LIBPACK_CHECKFILE_CLBUNDLER)
            set(ISIMPA_LIBPACK_VERSION "NOTFOUND" CACHE STRING "Displays if the libpack has been found" FORCE)
            message(SEND_ERROR  "Could not find LibPack in specified location: " ${ISIMPA_LIBPACK_DIR})
            file(GLOB found_files LIST_DIRECTORIES true "${ISIMPA_LIBPACK_DIR}/*")
            foreach(found_file ${found_files})
                message("${found_file}")
            endforeach()
        endif(ISIMPA_LIBPACK_CHECKFILE_CLBUNDLER)
# -------------------------------- PyCXX --------------------------------

        find_package(PyCXX REQUIRED)

# -------------------------------- Swig ----------------------------------

        find_package(SWIG)

        if(NOT SWIG_FOUND)
            message("==================================================\n"
                    "SWIG not found, don't build SWIG binding for pivy.\n"
                    "==================================================\n")
        endif(NOT SWIG_FOUND)

# -------------------------------- Salome SMESH --------------------------

        if(NOT ISIMPA_USE_EXTERNAL_SMESH)
            set(SMESH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/3rdParty/salomesmesh/inc)
        endif()

    endif(ISIMPA_LIBPACK_USE)

endmacro(ISIMPALibpackChecks)
