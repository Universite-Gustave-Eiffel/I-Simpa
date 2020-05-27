mkdir build
cd build

CLANG_WARNINGS=""

# wxWidget path (only in script scope)
export PATH=$HOME/wxWidgets-install/bin/:$HOME/swig-install/bin/:$HOME/cmake-install/bin/:$PATH
export BOOST_LIBRARYDIR=$HOME/boost-install/lib/
export BOOST_INCLUDEDIR=$HOME/boost-install/
export BOOST_ROOT=$HOME/boost-install/

cmake --version
${CC} --version
${CXX} --version

ls $HOME/boost-install/lib/

cmake ..
