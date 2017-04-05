cd build
pwd
make VERBOSE=1
make install
CTEST_OUTPUT_ON_FAILURE=1 make test

