set(ENV{PATH} "${ISIMPA_LIBPACK_DIR};$ENV{PATH}")
list(PREPEND CMAKE_PREFIX_PATH "${ISIMPA_LIBPACK_DIR}")

set (Python3_EXECUTABLE ${ISIMPA_LIBPACK_DIR}/bin/python.exe)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Make sure we are using the shared versions of Boost here: the LibPack includes both
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS system REQUIRED PATHS ${ISIMPA_LIBPACK_DIR}/lib/cmake NO_DEFAULT_PATH)

set(OCE_DIR ${ISIMPA_LIBPACK_DIR}/lib/cmake CACHE PATH "" FORCE)

set(SWIG_EXECUTABLE ${ISIMPA_LIBPACK_DIR}/bin/swig.exe CACHE FILEPATH "Swig" FORCE)

set (HDF5_DIR ${ISIMPA_LIBPACK_DIR}/share/cmake)

link_directories (${ISIMPA_LIBPACK_DIR}/lib)
