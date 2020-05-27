
cmake --version
${CC} --version
${CXX} --version

mkdir build
cd build

CLANG_WARNINGS=""

# wxWidget path (only in script scope)
export PATH=$HOME/wxWidgets-install/bin/:$HOME/swig-install/bin/:$HOME/cmake-install/bin/:$PATH
export BOOST_LIBRARYDIR=$HOME/boost-install/lib/
export BOOST_INCLUDEDIR=$HOME/boost-install/include/
export BOOST_ROOT=$HOME/boost-install/

cmake ..
