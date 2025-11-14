# Windows-specific installation

# Install configuration and documentation files
install(FILES
        ${ISIMPA_RESSOURCES}/appconst.xml
        ${ISIMPA_RESSOURCES}/license.html
        isimpa.ico
        DESTINATION ${ISIMPA_DATA_DIR})


get_filename_component(PYTHON_ROOT_FOLDER ${Python3_EXECUTABLE} DIRECTORY)

# Method 1: Use BundleUtilities to automatically find and copy Python DLLs
# This is more reliable than manual copying
install(CODE "
        include(BundleUtilities)

        # Get Python DLL (python3XX.dll or python3XX_d.dll)
        get_filename_component(PYTHON_DLL_DIR \"${Python3_RUNTIME_LIBRARY}\" DIRECTORY)

        # Gather all DLLs that the executable depends on
        set(DIRS
            \"${PYTHON_DLL_DIR}\"
            \"${PYTHON_ROOT_FOLDER}\"
            \"${PYTHON_ROOT_FOLDER}/DLLs\"
            \${CMAKE_INSTALL_PREFIX}/${ISIMPA_BIN_DIR}
        )

#        # Fix up the bundle (this will copy required DLLs)
#        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${ISIMPA_BIN_DIR}/isimpa.exe\" \"\" \"\${DIRS}\")
#    " COMPONENT Runtime)

# Install Python standard library
# Exclude unnecessary files to reduce package size
install(DIRECTORY
        ${PYTHON_ROOT_FOLDER}/Lib
        DESTINATION .
        COMPONENT Runtime
        PATTERN "Lib/site-packages" EXCLUDE
        PATTERN "Lib/test" EXCLUDE
        PATTERN "Lib/__pycache__" EXCLUDE
        PATTERN "*.pyc" EXCLUDE
        PATTERN "*.pyo" EXCLUDE
        PATTERN "*.pdb" EXCLUDE
        PATTERN "*.exp" EXCLUDE
        PATTERN "*.lib" EXCLUDE
)

# Install Python DLLs directory (extension modules)
install(DIRECTORY
        ${PYTHON_ROOT_FOLDER}/DLLs
        DESTINATION .
        COMPONENT Runtime
        PATTERN "*.pdb" EXCLUDE
        PATTERN "*.exp" EXCLUDE
        PATTERN "*.lib" EXCLUDE
)

# Install python.exe and pythonw.exe
install(FILES
        "${PYTHON_ROOT_FOLDER}/python.exe"
        DESTINATION .
        COMPONENT Runtime
        OPTIONAL
)

install(FILES
        "${PYTHON_ROOT_FOLDER}/pythonw.exe"
        DESTINATION .
        COMPONENT Runtime
        OPTIONAL
)

# Install the main Python DLL (python3XX.dll)
if(Python3_RUNTIME_LIBRARY)
    install(FILES
            "${Python3_RUNTIME_LIBRARY}"
            DESTINATION ${ISIMPA_BIN_DIR}
            COMPONENT Runtime
    )
endif()

# Install vcruntime if needed
if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
            DESTINATION ${ISIMPA_BIN_DIR}
            COMPONENT Runtime)
endif()

if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION . COMPONENT Runtime)
endif()

SET(CPACK_GENERATOR "NSIS;ZIP")

set(CPACK_SOURCE_IGNORE_FILES
        /bin/wx*
        /lib/cmake/wxWidgets
        /lib/wx
        /lib/*.a
)

SET(CPACK_BUNDLE_NAME "I-SIMPA")
SET(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/isimpa.ico")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "I-SIMPA")
SET(CPACK_PACKAGE_VENDOR "UMRAE, CEREMA, Univ Gustave Eiffel")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/DESCRIPTION.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
SET(CPACK_PACKAGE_VERSION_MAJOR ${isimpa_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${isimpa_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${isimpa_VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "I-SIMPA")
SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/isimpa.ico")
SET(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  CreateShortCut '$DESKTOP\\\\I_SIMPA.lnk' '$INSTDIR\\\\isimpa.exe' ")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$DESKTOP\\\\I_SIMPA.lnk'")
SET(CPACK_NSIS_INSTALLED_ICON_NAME "isimpa.exe")
SET(CPACK_NSIS_DISPLAY_NAME "I-SIMPA")
SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\i-simpa-wiki.readthedocs.io")
SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\i-simpa.unv-eiffel.fr")
SET(CPACK_NSIS_CONTACT "contact@noise-planet.org")
SET(CPACK_NSIS_MODIFY_PATH ON)
SET(CPACK_STRIP_FILES TRUE)
SET(CPACK_SOURCE_STRIP_FILES "")

SET(CPACK_PACKAGE_EXECUTABLES "isimpa" "I-SIMPA")

INCLUDE(CPack)
