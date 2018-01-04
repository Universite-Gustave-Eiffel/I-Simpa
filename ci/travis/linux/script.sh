cd build
pwd
make VERBOSE=1
make install
export LD_LIBRARY_PATH=$HOME/boost-install/lib:$LD_LIBRARY_PATH
CTEST_OUTPUT_ON_FAILURE=1 make test
