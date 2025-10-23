# Libsimpa
set(CMAKE_INSTALL_MFC_LIBRARIES TRUE) 
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE ) 

include( InstallRequiredSystemLibraries )
#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

# require swig > v3

find_package(SWIG REQUIRED)
include(UseSWIG)

list(APPEND CMAKE_SWIG_FLAGS "-doxygen")

if(UNIX AND NOT APPLE)
  list(APPEND CMAKE_SWIG_FLAGS "-DSWIGWORDSIZE64")
endif()

# Find Python 3
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
list(APPEND CMAKE_SWIG_FLAGS "-py3" "-DPY3")


INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/lib_interface")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET_SOURCE_FILES_PROPERTIES(libsimpa.i PROPERTIES CPLUSPLUS ON)

SWIG_ADD_LIBRARY(libsimpa LANGUAGE python SOURCES libsimpa.i)

SWIG_LINK_LIBRARIES(libsimpa lib_interface)

target_link_libraries(${SWIG_MODULE_libsimpa_REAL_NAME} Boost::python Boost::thread)

# Creates a folder "libraries" and adds target project (lib_interface.vcproj) under it
set_property(TARGET ${SWIG_MODULE_libsimpa_REAL_NAME} PROPERTY FOLDER "libraries")

#--------------#
#    INSTALL
#--------------#

set(ISIMPA_RESSOURCES ${PROJECT_SOURCE_DIR}/currentRelease)

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


# Adds logic to INSTALL.vcproj to copy _libsimpa.pyd to the destination directory
install (TARGETS ${SWIG_MODULE_libsimpa_REAL_NAME}
      LIBRARY DESTINATION ${ISIMPA_BIN_DIR}/libsimpa/
)

if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${ISIMPA_BIN_DIR}/libsimpa COMPONENT System )
endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )

install (FILES
        ${ISIMPA_RESSOURCES}/libsimpa/__init__.py
		${CMAKE_CURRENT_BINARY_DIR}/libsimpa.py
        DESTINATION ${ISIMPA_BIN_DIR}/libsimpa/)
        
        

if(WIN32)
    get_filename_component(PYTHON_PATH ${Python3_EXECUTABLE} DIRECTORY)
    #Fetch required Dll's 
    set(DIRS ${Boost_LIBRARY_DIR_DEBUG}
             ${Boost_LIBRARY_DIR_RELEASE}
             ${PYTHON_PATH}
    )
    
    install(CODE "
    include(BundleUtilities)   
    # Rename to .exe as cmake check if the file ends with .exe in windows before gathering dependencies..
    file(RENAME \"\${CMAKE_INSTALL_PREFIX}/libsimpa/_libsimpa.pyd\" \"\${CMAKE_INSTALL_PREFIX}/libsimpa/_libsimpa.exe\")    
    fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/libsimpa/_libsimpa.exe\" \"\"   \"${DIRS}\")
    file(RENAME \"\${CMAKE_INSTALL_PREFIX}/libsimpa/_libsimpa.exe\" \"\${CMAKE_INSTALL_PREFIX}/libsimpa/_libsimpa.pyd\") 
")
endif()

#------------#
#    TEST
#------------#

enable_testing()
# If you add tests, do not forget to add the name to set_property(TEST ..


MESSAGE(STATUS "Run test in ${CMAKE_CURRENT_BINARY_DIR}")

add_test(NAME test_modelio
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/model_io.py ${PROJECT_BINARY_DIR}/Testing/Temporary/
        WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/libsimpa/)

add_test(NAME check_retrocompat
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_retrocompat.py ${CMAKE_CURRENT_SOURCE_DIR}/tests/mesh.cbin ${PROJECT_BINARY_DIR}/Testing/Temporary/
        WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

add_test(NAME check_retrocompat_mesh
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_retrocompat_mesh.py ${CMAKE_CURRENT_SOURCE_DIR}/tests/tetramesh.mbin ${PROJECT_BINARY_DIR}/Testing/Temporary/
        WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

add_test(NAME check_retrocompat_gabe
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_retrocompat_gabe.py ${PROJECT_BINARY_DIR}/Testing/Temporary/
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/)

add_test(NAME test_math
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_math.py
        WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

add_test(NAME check_retrocompat_recsurf
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_retrocompat_recsurf.py
        WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

set_property(TEST test_modelio check_retrocompat check_retrocompat_mesh check_retrocompat_gabe test_math check_retrocompat_recsurf
 PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_INSTALL_PREFIX}"
)
