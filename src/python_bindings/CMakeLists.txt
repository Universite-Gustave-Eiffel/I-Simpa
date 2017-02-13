# Libsimpa
message(STATUS " SWIG_EXECUTABLE     - $ENV{SWIG_EXECUTABLE}")
set(CMAKE_INSTALL_MFC_LIBRARIES ON) 
include( InstallRequiredSystemLibraries )
#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

# require swig > v3
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

# Find package for building
FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
# Find package for testing
FIND_PACKAGE(PythonInterp 2.7 REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(libsimpa.i PROPERTIES CPLUSPLUS ON)

set(Boost_USE_STATIC_LIBS   OFF)
find_package(Boost COMPONENTS system filesystem regex REQUIRED)
link_directories( ${Boost_LIBRARY_DIRS} ) 

IF ((${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.8) OR (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} EQUAL 3.8))
  SWIG_ADD_LIBRARY(libsimpa LANGUAGE python SOURCES libsimpa.i)
ELSE()
  SWIG_ADD_MODULE(libsimpa python libsimpa.i)
ENDIF ()



SWIG_LINK_LIBRARIES(libsimpa lib_interface ${PYTHON_LIBRARIES})

# Creates a folder "libraries" and adds target project (lib_interface.vcproj) under it
set_property(TARGET ${SWIG_MODULE_libsimpa_REAL_NAME} PROPERTY FOLDER "libraries")

if(WIN32) # Check if we are on Windows
 if(MSVC) # Check if we are using the Visual Studio compiler
   # A 'Debug' build of the wrappers requires a debug build of the Python interpreter.
   # http://www.swig.org/Doc2.0/Python.html#Python_nn12
   # Fix linking with non existing python27_d.lib
   add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
 endif()
endif()

#--------------#
#    INSTALL
#--------------#

set(ISIMPA_RESSOURCES ${PROJECT_SOURCE_DIR}/currentRelease)

# Adds logic to INSTALL.vcproj to copy _libsimpa.pyd to the destination directory
install (TARGETS ${SWIG_MODULE_libsimpa_REAL_NAME}
      LIBRARY DESTINATION libsimpa/
)

if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION libsimpa COMPONENT System )
endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )

install (FILES
        ${ISIMPA_RESSOURCES}/libsimpa/vec3.py
        ${ISIMPA_RESSOURCES}/libsimpa/__init__.py
		${CMAKE_CURRENT_BINARY_DIR}/libsimpa.py
		${CMAKE_CURRENT_SOURCE_DIR}/tests/model_io.py
        DESTINATION libsimpa/)
        
        

if(WIN32)
	# Resolve symbolic link
	LIB_TO_DLL(${Boost_FILESYSTEM_LIBRARY_RELEASE} Boost_FILESYSTEM_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_SYSTEM_LIBRARY_RELEASE} Boost_SYSTEM_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_REGEX_LIBRARY_RELEASE} Boost_REGEX_LIBRARY_RELEASE_REAL)
	install (FILES
        ${Boost_FILESYSTEM_LIBRARY_RELEASE_REAL}
        ${Boost_SYSTEM_LIBRARY_RELEASE_REAL}
        ${Boost_REGEX_LIBRARY_RELEASE_REAL}
			DESTINATION libsimpa/)
endif()

#------------#
#    TEST
#------------#

enable_testing()
# If you add tests, do not forget to add the name to set_property(TEST ..


MESSAGE(STATUS "Run test in ${CMAKE_CURRENT_BINARY_DIR}")

add_test(NAME test_modelio
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_INSTALL_PREFIX}/libsimpa/model_io.py ${PROJECT_BINARY_DIR}/Testing/Temporary/)
  
  
set_property(TEST test_modelio
 PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
)