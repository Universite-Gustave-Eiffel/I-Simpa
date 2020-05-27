export DEBIAN_FRONTEND=noninteractive

#
# Swig install
if [ -f $HOME/swig-install/bin/swig ] ; then
	echo "Swig already built (and in travis cache)"
else
	cd
	wget https://github.com/swig/swig/archive/rel-3.0.10.tar.gz
	tar zxvf rel-3.0.10.tar.gz
	mkdir $HOME/swig-install
	cd  $HOME/swig-rel-3.0.10 && ./autogen.sh && ./configure --prefix=$HOME/swig-install && make && make install
fi

#
# Boost install

if [ -d $HOME/boost-install/boost ] ; then
	echo "Boost already built (and in travis cache)"
else
	cd
	wget https://sourceforge.net/projects/boost/files/boost/1.73.0/boost_1_73_0.tar.bz2
	tar -xjf boost_1_73_0.tar.bz2 -C $HOME/boost-install
	cd  $HOME/boost-install && ./bootstrap.sh link=static variant=release address-model=64 cxxflags="-std=c++11 -fPIC" boost.locale.icu=off --with-libraries=filesystem,system,test,regex,python38,random,thread,date_time --prefix=$HOME/boost-install && ./b2 install
fi

#
# WXWidget install

#if [ -d $HOME/wxWidgets-install/include ] ; then
#	echo "wxWidget already built (and in travis cache)"
#else
#	cd
#	wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.1/wxWidgets-3.1.1.tar.bz2
#	tar -xjf wxWidgets-3.1.1.tar.bz2
#	mkdir $HOME/wxWidgets-install
#	cd $HOME/wxWidgets-3.1.1 && ./configure --prefix=$HOME/wxWidgets-install --disable-shared && make && make install
#fi

# check wxWidget install
export PATH=$HOME/wxWidgets-install/bin/:$PATH
echo "wxWidget version : "
wx-config --version

# Download CMake
cd
wget --no-check-certificate https://cmake.org/files/v3.17/cmake-3.17.2-Linux-x86_64.tar.gz
mkdir $HOME/cmake-install
tar zxvf cmake-3.17.2-Linux-x86_64.tar.gz -C $HOME/cmake-install --strip 1
