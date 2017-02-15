include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")
set(CMAKE_INSTALL_MFC_LIBRARIES ON) 
include( InstallRequiredSystemLibraries )
message(STATUS " Boost path - $ENV{BOOST_INCLUDEDIR}")
message(STATUS " Boost lib path - $ENV{BOOST_LIBRARYDIR}")

#--------------#
#    SOURCES
#--------------#

# Sources files
set(DATA_MANAGER_SOURCES
    data_manager/core_configuration.h
    data_manager/core_configuration.cpp)

set(IO_SOURCES
    input_output/reportmanager.h
    input_output/reportmanager.cpp)

set(TOOLS_SOURCES
    tools/dotreflection.h
    tools/dotdistribution.h
    tools/dotdistribution.cpp)

set(SPPS_SOURCES
    CalculationCore.h
    CalculationCore.cpp
    sppsInitialisation.h
    sppsInitialisation.cpp
    sppsTypes.h
    sppsTypes.cpp)

set(SPPS_MAIN_SOURCES
    sppsNantes.cpp)

# Sources groups (folders)
source_group("data_manager" FILES ${DATA_MANAGER_SOURCES})
source_group("input_output" FILES ${IO_SOURCES})
source_group("tools" FILES ${TOOLS_SOURCES})
source_group("" FILES ${SPPS_SOURCES} $(SPPS_MAIN_SOURCES))

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

set(Boost_USE_STATIC_LIBS   OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  find_package(Boost REQUIRED COMPONENTS system random filesystem regex)
else()
  find_package (Threads)
  find_package(Boost REQUIRED COMPONENTS system random filesystem regex thread chrono date_time)
endif()

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

# Check for clock_gettime function
if (UNIX)
  include(CheckLibraryExists)
  check_library_exists(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME )
  if(NOT HAVE_CLOCK_GETTIME)
    message(FATAL_ERROR "clock_gettime not found")
  endif(NOT HAVE_CLOCK_GETTIME)
endif(UNIX)

# Set Properties->General->Configuration Type to Application
# Creates spps with the listed sources
# Adds sources to the Solution Explorer
add_executable(spps
    ${DATA_MANAGER_SOURCES}
    ${IO_SOURCES}
    ${TOOLS_SOURCES}
    ${SPPS_SOURCES}
	${SPPS_MAIN_SOURCES})

# Creates a folder "executables" and adds target project (spps.vcproj) under it
set_property(TARGET spps PROPERTY FOLDER "executables")


# Properties->Linker->Input->Additional Dependencies
target_link_libraries (spps lib_interface ${Boost_LIBRARIES})


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D_DEBUG)
  message(STATUS " SPPS debug build disable multi threading")
else()
  message(STATUS " SPPS release build enable multi threading")
  target_link_libraries (spps ${CMAKE_THREAD_LIBS_INIT})
  if(UNIX)
    target_link_libraries (spps rt)
  endif(UNIX)
endif()

#--------------#
#    INSTALL
#--------------#



# Adds logic to INSTALL.vcproj to copy spps to destination directory
install (TARGETS spps
        RUNTIME DESTINATION core/spps)


if(WIN32)
	# Resolve symbolic link
	LIB_TO_DLL(${Boost_FILESYSTEM_LIBRARY_RELEASE} Boost_FILESYSTEM_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_SYSTEM_LIBRARY_RELEASE} Boost_SYSTEM_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_RANDOM_LIBRARY_RELEASE} Boost_RANDOM_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_REGEX_LIBRARY_RELEASE} Boost_REGEX_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_THREAD_LIBRARY_RELEASE} Boost_THREAD_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_CHRONO_LIBRARY_RELEASE} Boost_CHRONO_LIBRARY_RELEASE_REAL)
	LIB_TO_DLL(${Boost_DATE_TIME_LIBRARY_RELEASE} Boost_DATE_TIME_LIBRARY_RELEASE_REAL)
	install (FILES
        ${Boost_FILESYSTEM_LIBRARY_RELEASE_REAL}
        ${Boost_SYSTEM_LIBRARY_RELEASE_REAL}
        ${Boost_REGEX_LIBRARY_RELEASE_REAL}
        ${Boost_RANDOM_LIBRARY_RELEASE_REAL}
        ${Boost_THREAD_LIBRARY_RELEASE_REAL}
        ${Boost_CHRONO_LIBRARY_RELEASE_REAL}
        ${Boost_DATE_TIME_LIBRARY_RELEASE_REAL}
			DESTINATION core/spps)
endif()

if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION core/spps COMPONENT System )
endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
#------------#
#    TEST
#------------#
# Data files
set(TEST_DATA_DIR "${PROJECT_BINARY_DIR}/Testing")
set(SPPS_TEST_DIR "${TEST_DATA_DIR}/")
configure_file(tests/config.xml ${SPPS_TEST_DIR})
file(COPY
    tests/speaker-test3.txt
    DESTINATION ${TEST_DATA_DIR})
	

# Test executable and folder
set(SPPS_TESTS
    tests/spps_core_test.cpp)
	
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost COMPONENTS filesystem regex unit_test_framework timer chrono REQUIRED)

add_executable(TEST_spps ${SPPS_TESTS}
    ${DATA_MANAGER_SOURCES}
    ${IO_SOURCES}
    ${TOOLS_SOURCES}
    ${SPPS_SOURCES})

target_link_libraries (TEST_spps
    lib_interface
    ${Boost_LIBRARIES})
set_property(TARGET TEST_spps PROPERTY FOLDER "tests")

# Add to test suite
enable_testing()

add_test( NAME spps_core_test
    WORKING_DIRECTORY ${TEST_DATA_DIR}
    COMMAND TEST_spps )
