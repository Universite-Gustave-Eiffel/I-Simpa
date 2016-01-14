cmake_minimum_required(VERSION 3.3)
project(spps)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
message(STATUS " Boost path - $ENV{BOOST_ROOT}")
message(STATUS " Boost lib path - $ENV{BOOST_LIBRARYDIR}")
set(SOURCE_FILES
    data_manager/core_configuration.cpp
    input_output/reportmanager.cpp
    tools/dotdistribution.cpp
    CalculationCore.cpp
    sppsInitialisation.cpp
    sppsNantes.cpp
    sppsTypes.cpp)
include_directories(../lib_interface ./)
set(Boost_DEBUG 1)
#set(BOOST_ROOT $ENV{BOOST_ROOT})
set(BOOST_LIBRARYDIR $ENV{BOOST_LIBRARYDIR})
set(BOOST_INCLUDEDIR $ENV{BOOST_INCLUDEDIR})
find_package(Boost REQUIRED COMPONENTS thread random)
#find_package(Boost
#        #1.39 # Minimum or EXACT version e.g. 1.36.0
#        REQUIRED             # Fail with error if Boost is not found
#        COMPONENTS thread bind # Boost libraries by their canonical name
#        )

add_executable(spps ${SOURCE_FILES})
