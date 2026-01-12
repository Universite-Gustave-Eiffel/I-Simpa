project (ctr VERSION 1.0.1)

# Change scope of theses variables in order to be used by I-SIMPA gui project

set(ctr_VERSION_MAJOR ${ctr_VERSION_MAJOR} PARENT_SCOPE)
set(ctr_VERSION_MINOR ${ctr_VERSION_MINOR} PARENT_SCOPE)
set(ctr_VERSION_PATCH ${ctr_VERSION_PATCH} PARENT_SCOPE)

include_directories (./ "${CMAKE_SOURCE_DIR}/src/lib_interface"  "${CMAKE_CURRENT_BINARY_DIR}/generated/")

#--------------#
#    SOURCES
#--------------#

set(CT_SOURCE_FILES
		TC_CalculationCore.cpp
		data_manager/core_configuration.cpp
        input_output/reportmanager.cpp
        main_tc.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/generated/ctrVersion.h)

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

set(Boost_USE_STATIC_LIBS   OFF)

link_directories( ${Boost_LIBRARY_DIRS} )  
include_directories( ${Boost_INCLUDE_DIRS} )


configure_file(ctrVersion.h.in ${CMAKE_CURRENT_BINARY_DIR}/generated/ctrVersion.h )

# Set Properties->General->Configuration Type to Application
# Creates spps with the listed sources
# Adds sources to the Solution Explorer
add_executable(classicalTheory ${CT_SOURCE_FILES})

# Creates a folder "executables" and adds target
set_property(TARGET classicalTheory PROPERTY FOLDER "executables")

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (classicalTheory lib_interface)
target_link_libraries(classicalTheory ${Boost_LIBRARIES})
