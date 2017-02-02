include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")
set(CMAKE_INSTALL_MFC_LIBRARIES ON) 
include( InstallRequiredSystemLibraries )
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

set(Boost_USE_STATIC_LIBS   OFF) 
find_package(Boost COMPONENTS system filesystem regex REQUIRED)
link_directories( ${Boost_LIBRARY_DIRS} )  
include_directories( ${Boost_INCLUDE_DIRS} ) 

# Set Properties->General->Configuration Type to Application
# Creates spps with the listed sources
# Adds sources to the Solution Explorer
add_executable(classicalTheory ${CT_SOURCE_FILES})

# Creates a folder "executables" and adds target
set_property(TARGET classicalTheory PROPERTY FOLDER "executables")

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (classicalTheory lib_interface)
target_link_libraries(classicalTheory ${Boost_LIBRARIES})

#--------------#
#    INSTALL
#--------------#



# Adds logic to INSTALL.vcproj to copy spps to destination directory
install (TARGETS classicalTheory
        RUNTIME DESTINATION core/classical_theory)

if (WIN32)
	# Resolve symbolic link
	LIB_TO_DLL(${Boost_FILESYSTEM_LIBRARY_RELEASE} Boost_FILESYSTEM_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_SYSTEM_LIBRARY_RELEASE} Boost_SYSTEM_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_REGEX_LIBRARY_RELEASE} Boost_REGEX_LIBRARY_RELEASE_REAL)
	install (FILES
			${Boost_FILESYSTEM_LIBRARY_RELEASE_REAL}
			${Boost_SYSTEM_LIBRARY_RELEASE_REAL}
			${Boost_REGEX_LIBRARY_RELEASE_REAL}
			DESTINATION core/classical_theory)
endif ()

if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION core/classical_theory COMPONENT System )
endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )