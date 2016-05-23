CMAKE_MINIMUM_REQUIRED(VERSION 2.8.6)

# Maps to a solution file (isimpa.sln). The solution will
# have all targets (exe, lib, dll) as projects (.vcproj)
project (isimpa)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Add an option in order to build only the computation cores and libraries
option(SKIPISIMPA "skip-gui" FALSE)

# Set compiler flags and options. 
# Here it is setting the Visual Studio warning level to 4
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message (STATUS "cxx Flags: " ${CMAKE_CXX_FLAGS})

# Sub-directories where more CMakeLists.txt exist
add_subdirectory (src/lib_interface)
add_subdirectory (src/python_bindings)
add_subdirectory (src/theorie_classique)
add_subdirectory (src/VolumetricMeshRepair)
add_subdirectory (src/preprocess)
add_subdirectory (src/spps)
if(NOT SKIPISIMPA)
   add_subdirectory (src/isimpa)
endif()

# Turn on CMake testing capabilities
set(TEST_DATA_DIR "${PROJECT_BINARY_DIR}/Testing")
file(MAKE_DIRECTORY ${TEST_DATA_DIR})
enable_testing()
