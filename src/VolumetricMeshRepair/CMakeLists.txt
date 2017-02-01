include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")
set(CMAKE_INSTALL_MFC_LIBRARIES ON) 
include( InstallRequiredSystemLibraries )

#--------------#
#    SOURCES
#--------------#

set(VMR_SOURCE_FILES
    "adaptative_mesh_guide.cpp"
    "tools/intersection_tri.cpp"
    "main_remesh.cpp"
    "point_feeder.cpp"
    "scalar_field_creator.cpp"
    "spatial_discretization.cpp"
    "triang_cubic_meshing.cpp"
    "triang_marching_cube.cpp"
    "triang_marching_tetra.cpp"
    "triangle_feeder.cpp"
    "triangulator.cpp")

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

find_package(Boost COMPONENTS system filesystem regex REQUIRED)
link_directories( ${Boost_LIBRARY_DIRS} ) 

# Set Properties->General->Configuration Type to Application
# Creates spps with the listed sources
# Adds sources to the Solution Explorer
add_executable(vmr ${VMR_SOURCE_FILES})

# Creates a folder "executables" and adds target
set_property(TARGET vmr PROPERTY FOLDER "executables")

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (vmr lib_interface)

#--------------#
#    INSTALL
#--------------#


# Adds logic to INSTALL.vcproj to copy vmr to destination directory
install (TARGETS vmr
        RUNTIME DESTINATION meshing/vmr)

if(WIN32)
    # Resolve symbolic link
    LIB_TO_DLL(${Boost_FILESYSTEM_LIBRARY_RELEASE} Boost_FILESYSTEM_LIBRARY_RELEASE_REAL)
    LIB_TO_DLL(${Boost_SYSTEM_LIBRARY_RELEASE} Boost_SYSTEM_LIBRARY_RELEASE_REAL)
    LIB_TO_DLL(${Boost_REGEX_LIBRARY_RELEASE} Boost_REGEX_LIBRARY_RELEASE_REAL)
    install (FILES
            ${Boost_FILESYSTEM_LIBRARY_RELEASE_REAL}
            ${Boost_SYSTEM_LIBRARY_RELEASE_REAL}
            ${Boost_REGEX_LIBRARY_RELEASE_REAL}
                DESTINATION meshing/vmr)
endif()
		
if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION meshing/vmr COMPONENT System )
endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )