mkdir build
cd build

export CXX="clang++-3.7"
export CC="clang-3.7"

cmake --version
${CC} --version

cmake ..
