cmake_minimum_required(VERSION 3.3)

include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

message(STATUS " Boost path - $ENV{BOOST_INCLUDEDIR}")
message(STATUS " Boost lib path - $ENV{BOOST_LIBRARYDIR}")
set(SOURCE_FILES
    data_manager/core_configuration.cpp
    input_output/reportmanager.cpp
    tools/dotdistribution.cpp
    CalculationCore.cpp
    sppsInitialisation.cpp
    sppsNantes.cpp
    sppsTypes.cpp)

set(Boost_DEBUG 1)
set(BOOST_LIBRARYDIR $ENV{BOOST_LIBRARYDIR})
set(BOOST_INCLUDEDIR $ENV{BOOST_INCLUDEDIR})
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
ADD_DEFINITIONS( -DBOOST_ALL_NO_LIB ) 
find_package(Boost REQUIRED COMPONENTS thread random system)
link_directories( ${Boost_LIBRARY_DIRS} )  
include_directories( ${Boost_INCLUDE_DIRS} )  

# Set Properties->General->Configuration Type to Application(.exe)
# Creates spps.exe with the listed sources
# Adds sources to the Solution Explorer
add_executable(spps ${SOURCE_FILES})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (spps lib_interface)
target_link_libraries(spps ${Boost_LIBRARIES})

# Creates a folder "executables" and adds target
# project (spps.vcproj) under it
set_property(TARGET spps PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy spps.exe to destination directory
install (TARGETS spps
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
