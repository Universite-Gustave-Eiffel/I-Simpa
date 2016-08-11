
export CXX="g++-4.8"
export CC="gcc-4.8"

cmake --version
${CC} --version
${CXX} --version

mkdir build
cd build

CLANG_WARNINGS=""

# wxWidget path (only in script scope)
export PATH=$HOME/wxWidgets-install/bin/:$PATH
export BOOST_LIBRARYDIR=$HOME/boost-install/lib/
export BOOST_INCLUDEDIR=$HOME/boost-install/include/
export BOOST_ROOT=$HOME/boost-install/
export SWIG_EXECUTABLE=$HOME/swig-install/bin/swig
export SWIG_DIR=$HOME/swig-install/

cmake ..
