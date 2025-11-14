# Install configuration and documentation files
if(IS_FLATPAK)
    install(FILES
            ${ISIMPA_RESSOURCES}/appconst.xml
            DESTINATION ${ISIMPA_DATA_DIR})

    install(FILES
            ${ISIMPA_RESSOURCES}/license.html
            DESTINATION ${ISIMPA_DOC_DIR})

    install(FILES
            isimpa.ico
            DESTINATION ${ISIMPA_DATA_DIR})

    install(FILES
            isimpa.png
            DESTINATION ${ISIMPA_ICON_DIR}
            RENAME org.noise_planet.i-simpa.png)
else()
    install(FILES
            ${ISIMPA_RESSOURCES}/appconst.xml
            ${ISIMPA_RESSOURCES}/license.html
            isimpa.ico
            DESTINATION ${ISIMPA_DATA_DIR})
endif()


# Flatpak-specific: Install desktop file and appdata
if(IS_FLATPAK)
    install(FILES
            ${PROJECT_SOURCE_DIR}/org.noise_planet.i-simpa.desktop
            DESTINATION share/applications)

    install(FILES
            ${PROJECT_SOURCE_DIR}/org.noise_planet.i-simpa.appdata.xml
            DESTINATION share/metainfo)
endif()

# Package creation instructions
if(NOT IS_FLATPAK)

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
    SET(CPACK_PACKAGE_EXECUTABLES "isimpa" "I-SIMPA")
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nicolas Fortin")
    INCLUDE(CPack)
endif()
