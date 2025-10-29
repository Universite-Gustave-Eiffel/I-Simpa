set(CMAKE_INSTALL_MFC_LIBRARIES TRUE) 
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE )
include( InstallRequiredSystemLibraries )

# Add an executable to the project using the specified source files.
add_executable(tetgen tetgen.cxx predicates.cxx)

# Creates a folder "executables" and adds target
set_property(TARGET tetgen PROPERTY FOLDER "executables")

#--------------#
#    INSTALL
#--------------#

# Detect if building for Flatpak
if(DEFINED ENV{FLATPAK_ID})
    set(IS_FLATPAK TRUE)
    message(STATUS "Building for Flatpak environment")
else()
    set(IS_FLATPAK FALSE)
endif()

# Set installation directories based on build type
if(IS_FLATPAK)
    set(ISIMPA_BIN_DIR bin)
else()
    set(ISIMPA_BIN_DIR .)
endif()

# Adds logic to INSTALL.vcproj to copy tetgen to destination directory
install (TARGETS tetgen
        RUNTIME DESTINATION ${ISIMPA_BIN_DIR}/meshing/tetgen)

install (FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/README
        ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
        DESTINATION ${ISIMPA_BIN_DIR}/meshing/tetgen)
        
        
if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${ISIMPA_BIN_DIR}/meshing/tetgen COMPONENT System )
endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )