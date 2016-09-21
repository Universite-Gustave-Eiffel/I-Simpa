# Libsimpa
message(STATUS " SWIG_EXECUTABLE     - $ENV{SWIG_EXECUTABLE}")

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

# require swig > v3
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

# Find package for building
FIND_PACKAGE(PythonLibs)
# Find package for testing
FIND_PACKAGE(PythonInterp REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(libsimpa.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(libsimpa python libsimpa.i)
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

install (FILES
        ${ISIMPA_RESSOURCES}/libsimpa/vec3.py
        ${ISIMPA_RESSOURCES}/libsimpa/__init__.py
		${CMAKE_CURRENT_BINARY_DIR}/libsimpa.py
        DESTINATION libsimpa/)
#------------#
#    TEST
#------------#

enable_testing()
# If you add tests, do not forget to add the name to set_property(TEST ..


MESSAGE(STATUS "Run test in ${CMAKE_CURRENT_BINARY_DIR}")

add_test(NAME test_modelio
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/model_io.py ${PROJECT_BINARY_DIR}/Testing/Temporary/
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  
  
set_property(TEST test_modelio
 PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
)