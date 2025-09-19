# set compilers (if you installed gcc-4.8 you can use those names)
export CC=$(which gcc)    # or /usr/bin/gcc-4.8 if available
export CXX=$(which g++)
export CXXFLAGS="-O2 -g"
export CFLAGS="-O2 -g"

# ensure the installed package prefixes are on PATH
export PATH=$HOME/wxWidgets-install/bin:$HOME/swig-install/bin:$HOME/cmake-install/bin:$PATH
export BOOST_LIBRARYDIR=$HOME/boost-install/lib/
export BOOST_INCLUDEDIR=$HOME/boost-install/include/
export BOOST_ROOT=$HOME/boost-install/

# build I-Simpa
mkdir -p build && cd build
cmake ..
make -j$(nproc)

# install to a DESTDIR (optional)
make DESTDIR=/home/simpa/isimpabuild install
# the final binary will be under /home/simpa/isimpabuild/...
