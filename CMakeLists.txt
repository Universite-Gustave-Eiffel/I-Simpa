CMAKE_MINIMUM_REQUIRED(VERSION 2.8.6)

# Maps to a solution file (isimpa.sln). The solution will
# have all targets (exe, lib, dll) as projects (.vcproj)
project (isimpa)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/bin" CACHE PATH "default install path" FORCE )
	message (STATUS "Default install path is set: " ${CMAKE_INSTALL_PREFIX})
else()
    message (STATUS "Install path already defined: " ${CMAKE_INSTALL_PREFIX})
endif()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Add an option in order to build only the computation cores and libraries
option(SKIPISIMPA "skip-gui" FALSE)

# Set compiler flags and options.
#############################################################
# enable use of c++11 features where available
# full c++11 support in clang 3.3+: http://clang.llvm.org/cxx_status.html
# for Mac, this is probably Apple LLVM 4.2 (based on LLVM 3.2svn, in XCode 4.6+)
#   or definitely Apple LLVM 5.0 (based on LLVM 3.3svn, in Xcode 5+):
#   https://gist.github.com/yamaya/2924292

IF (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    IF (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        SET(USE_CXX_11 TRUE)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ENDIF()
ELSEIF (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    IF ((NOT APPLE AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "3.2")
            OR (APPLE AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.1"))
        SET(USE_CXX_11 TRUE)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-error=c++11-narrowing")
    ENDIF()
ELSEIF (MSVC AND MSVC_VERSION GREATER 1600)
    SET(USE_CXX_11 TRUE)
ELSE()
    SET(USE_CXX_11 FALSE)
ENDIF()


# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message (STATUS "cxx Flags: " ${CMAKE_CXX_FLAGS})

# Sub-directories where more CMakeLists.txt exist
add_subdirectory (src/lib_interface)
add_subdirectory (src/python_bindings)
add_subdirectory (src/theorie_classique)
add_subdirectory (src/VolumetricMeshRepair)
add_subdirectory (src/preprocess)
add_subdirectory(src/tetgen)
add_subdirectory (src/spps)
if(NOT SKIPISIMPA)
   add_subdirectory (src/isimpa)
endif()

# Turn on CMake testing capabilities
set(TEST_DATA_DIR "${PROJECT_BINARY_DIR}/Testing")
file(MAKE_DIRECTORY ${TEST_DATA_DIR})
enable_testing()