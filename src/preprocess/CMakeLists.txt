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
