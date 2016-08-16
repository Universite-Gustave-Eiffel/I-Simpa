include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

#--------------#
#    SOURCES
#--------------#

set(CT_SOURCE_FILES
		"TC_CalculationCore.cpp"
		"data_manager/core_configuration.cpp"
        "input_output/reportmanager.cpp"
        "main_tc.cpp")

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

set(Boost_USE_STATIC_LIBS   ON) 
find_package(Boost REQUIRED)
link_directories( ${Boost_LIBRARY_DIRS} )  
include_directories( ${Boost_INCLUDE_DIRS} ) 

# Set Properties->General->Configuration Type to Application
# Creates spps with the listed sources
# Adds sources to the Solution Explorer
add_executable(TheorieClassique ${CT_SOURCE_FILES})

# Creates a folder "executables" and adds target
set_property(TARGET TheorieClassique PROPERTY FOLDER "executables")

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (TheorieClassique lib_interface)
target_link_libraries(TheorieClassique ${Boost_LIBRARIES})

#--------------#
#    INSTALL
#--------------#

# Adds logic to INSTALL.vcproj to copy spps to destination directory
install (TARGETS TheorieClassique
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/core/theorie_classique)
