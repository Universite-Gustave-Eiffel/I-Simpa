if [ "$CXX" = "g++" ]; then export CXX="g++-5" CC="gcc-5"; fi

mkdir build
cd build

# wxWidget path (only in script scope)
export PATH=$HOME/wxWidgets-install/bin/:$PATH
export BOOST_LIBRARYDIR=$HOME/boost-install/lib/
export BOOST_INCLUDEDIR=$HOME/boost-install/include/
export BOOST_ROOT=$HOME/boost-install/

cmake ..
