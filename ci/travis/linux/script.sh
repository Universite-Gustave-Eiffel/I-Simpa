cd build
pwd
make VERBOSE=1
make install
export PATH=$HOME/boost-install:$PATH
CTEST_OUTPUT_ON_FAILURE=1 make test
