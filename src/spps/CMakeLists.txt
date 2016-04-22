include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

message(STATUS " Boost path - $ENV{BOOST_INCLUDEDIR}")
message(STATUS " Boost lib path - $ENV{BOOST_LIBRARYDIR}")

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

source_group("data_manager" FILES ${DATA_MANAGER_SOURCES})
source_group("input_output" FILES ${IO_SOURCES})
source_group("tools" FILES ${TOOLS_SOURCES})
source_group("" FILES ${SPPS_SOURCES})

set(Boost_USE_STATIC_LIBS   ON) 
find_package(Boost REQUIRED COMPONENTS thread random system)
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

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (spps lib_interface)
target_link_libraries(spps ${Boost_LIBRARIES})

# Creates a folder "executables" and adds target
# project (spps.vcproj) under it
set_property(TARGET spps PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy spps.exe to destination directory
install (TARGETS spps
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/core/spps)
