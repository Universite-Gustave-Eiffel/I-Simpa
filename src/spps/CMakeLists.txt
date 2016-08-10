include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

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
    sppsNantes.cpp
    sppsTypes.h
    sppsTypes.cpp)

# Sources groups (folders)
source_group("data_manager" FILES ${DATA_MANAGER_SOURCES})
source_group("input_output" FILES ${IO_SOURCES})
source_group("tools" FILES ${TOOLS_SOURCES})
source_group("" FILES ${SPPS_SOURCES})

#---------------------------------------#
#    DEPENDENCY & EXECUTABLE (OR LIB)
#---------------------------------------#

set(Boost_USE_STATIC_LIBS   ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  find_package(Boost REQUIRED COMPONENTS system random filesystem)
else()
  find_package (Threads)
  find_package(Boost REQUIRED COMPONENTS system random filesystem thread)
endif()

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

# Set Properties->General->Configuration Type to Application(.exe)
# Creates spps.exe with the listed sources
# Adds sources to the Solution Explorer
add_executable(spps
    ${DATA_MANAGER_SOURCES}
    ${IO_SOURCES}
    ${TOOLS_SOURCES}
    ${SPPS_SOURCES})

# Creates a folder "executables" and adds target project (spps.vcproj) under it
set_property(TARGET spps PROPERTY FOLDER "executables")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D_DEBUG)
  message(STATUS " SPPS debug build disable multi threading")
else()
  message(STATUS " SPPS release build enable multi threading")
  target_link_libraries (spps ${CMAKE_THREAD_LIBS_INIT})
endif()

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (spps lib_interface)
target_link_libraries(spps ${Boost_LIBRARIES})

#--------------#
#    INSTALL
#--------------#

# Adds logic to INSTALL.vcproj to copy spps.exe to destination directory
install (TARGETS spps
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/core/spps)

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
    tests/spps_directivity_test.cpp)

add_executable(TEST_spps ${SPPS_TESTS})
target_link_libraries (TEST_spps
    lib_interface
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
set_property(TARGET TEST_spps PROPERTY FOLDER "tests")

set(SPPS_TESTS_PRE
    tests/spps_directivity_pre.cpp)
add_executable(TEST_spps_pre ${SPPS_TESTS_PRE})
target_link_libraries (TEST_spps_pre
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})
set_property(TARGET TEST_spps_pre PROPERTY FOLDER "tests")

# Add to test suite
enable_testing()

add_test( NAME spps_directivity_clean
    WORKING_DIRECTORY ${TEST_DATA_DIR}
    COMMAND TEST_spps_pre )

#add_test( NAME spps_directivity_check
#    WORKING_DIRECTORY ${TEST_DATA_DIR}
#    COMMAND TEST_spps )
