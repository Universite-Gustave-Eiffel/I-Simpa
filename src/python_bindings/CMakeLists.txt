include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

set(BINDINGS_SOURCE_FILES
    __array_1.pypp.hpp
    __call_policies.pypp.hpp
    __convenience.pypp.hpp
    _libsimpa.py.cpp)

set(Boost_USE_STATIC_LIBS ON) 

find_package(Boost REQUIRED COMPONENTS python)
message(STATUS "Include dirs of boost: " ${Boost_INCLUDE_DIRS} )
message(STATUS "Libs of boost: " ${Boost_LIBRARIES} )

find_package(PythonLibs REQUIRED)
message(STATUS "Include dirs of Python: " ${PYTHON_INCLUDE_DIRS} )
message(STATUS "Libs of Python: " ${PYTHON_LIBRARIES} )

# Include external dependency
include_directories( ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

# Link with external dependency
link_directories( ${Boost_LIBRARY_DIRS} ${Python_LIBRARY_DIRS} )

add_library(python_bindings ${BINDINGS_SOURCE_FILES})

SET_TARGET_PROPERTIES(python_bindings PROPERTIES SUFFIX pyd)

target_link_libraries (python_bindings lib_interface ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

# Creates a folder "libraries" and adds target project (python_bindings.vcproj) under it
set_property(TARGET python_bindings PROPERTY FOLDER "libraries")

# Adds logic to INSTALL.vcproj to copy lib_interface.pyc to the destination directory
install (TARGETS python_bindings
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
        ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin
        LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/lib
        COMPONENT library
)
