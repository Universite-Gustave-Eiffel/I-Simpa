mkdir build
cd build

# wxWidget path (only in script scope)
export PATH=$HOME/wxWidgets-install/bin/:$PATH

cmake ..
