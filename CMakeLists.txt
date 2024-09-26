# begin basic metadata
# minimum CMake version required for C++20 support, among other things
cmake_minimum_required(VERSION 3.15)

# Maps to a solution file (isimpa.sln). The solution will
# have all targets (exe, lib, dll) as projects (.vcproj)
project (isimpa VERSION 1.3.4)

# a better way to load dependencies
include(cmake/CPM.cmake)

# convert path of a .lib file into a .dll file
# findboost does not store .dll locations
macro(LIB_TO_DLL LIBPATH DLL_PATH)
    get_filename_component(DLL_FILENAME ${LIBPATH}  NAME_WE)
    get_filename_component(DLL_DIR ${LIBPATH} DIRECTORY)
	set(${DLL_PATH} "${DLL_DIR}/${DLL_FILENAME}.dll")
	if(NOT EXISTS "${DLL_DIR}/${DLL_FILENAME}.dll")
      MESSAGE(ERROR " Dll file does not exists ${DLL_DIR}/${DLL_FILENAME}.dll")
	endif()
endmacro(LIB_TO_DLL)

macro(COPY_LIB LIBPATH DESTINATION)
    get_filename_component(UT_SHARED_FILE ${LIBPATH} NAME)
    configure_file(${LIBPATH}
            ${DESTINATION}/${UT_SHARED_FILE} COPYONLY)
endmacro(COPY_LIB)

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
        set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++11 -O3 -fPIC" )        
        set( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -O3 -fPIC" )
    ENDIF()
ELSEIF (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    IF ((NOT APPLE AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "3.2")
            OR (APPLE AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.1"))
        SET(USE_CXX_11 TRUE)
        set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++11  -Wno-error=c++11-narrowing -O3 -fPIC" )    
        set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -O3 -fPIC" )
    ENDIF()
ELSEIF (MSVC AND MSVC_VERSION GREATER 1600)
    SET(USE_CXX_11 TRUE)
ELSE()
    SET(USE_CXX_11 FALSE)
ENDIF()

# set cmake policy

cmake_policy(SET CMP0086 NEW)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0074 NEW)

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message (STATUS "cxx Flags: " ${CMAKE_CXX_FLAGS})

# Sub-directories where more CMakeLists.txt exist
add_subdirectory (src/lib_interface)
add_subdirectory (src/python_bindings)
add_subdirectory (src/ctr)
add_subdirectory (src/VolumetricMeshRepair)
add_subdirectory (src/preprocess)
add_subdirectory(src/tetgen)
add_subdirectory (src/spps)
if(NOT SKIPISIMPA)
   add_subdirectory (src/isimpa)
endif()

# custom target for CLion "make install"
if(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    add_custom_target(install_${PROJECT_NAME}
            $(MAKE) install
            DEPENDS ${PROJECT_NAME}
            COMMENT "Installing ${PROJECT_NAME}")
endif()

# Turn on CMake testing capabilities
set(TEST_DATA_DIR "${PROJECT_BINARY_DIR}/Testing")
file(MAKE_DIRECTORY ${TEST_DATA_DIR})
enable_testing()
