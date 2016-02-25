include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

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

# Set Properties->General->Configuration Type to Application(.exe)
# Creates spps.exe with the listed sources
# Adds sources to the Solution Explorer
add_executable(vmr ${VMR_SOURCE_FILES})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (vmr lib_interface)

# Creates a folder "executables" and adds target
set_property(TARGET vmr PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy vmr.exe to destination directory
install (TARGETS vmr
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/meshing/vmr)
