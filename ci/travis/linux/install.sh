if [ "$CXX" = "g++" ]; then export CXX="g++-5" CC="gcc-5"; fi

mkdir build
cd build

# wxWidget path (only in script scope)
export PATH=$HOME/wxWidgets-install/bin/:$PATH

cmake ..
