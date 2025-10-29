include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")
set(CMAKE_INSTALL_MFC_LIBRARIES TRUE) 
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE )
include( InstallRequiredSystemLibraries )
#--------------#
#    SOURCES
#--------------#

set(PREPROCESS_SOURCE_FILES
		"computations.cpp"
        "tools/intersection_tri.cpp"
        "Preprocess.cpp")

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

link_directories( ${Boost_LIBRARY_DIRS} ) 

# Set Properties->General->Configuration Type to Application
# Creates spps with the listed sources
# Adds sources to the Solution Explorer
add_executable(preprocess ${PREPROCESS_SOURCE_FILES})

# Creates a folder "executables" and adds target
set_property(TARGET preprocess PROPERTY FOLDER "executables")

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (preprocess lib_interface)

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


# Adds logic to INSTALL.vcproj to copy preprocess to destination directory
install (TARGETS preprocess
        RUNTIME DESTINATION ${ISIMPA_BIN_DIR}/meshing/tetgen)

if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${ISIMPA_BIN_DIR}/meshing/tetgen COMPONENT System )
endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )