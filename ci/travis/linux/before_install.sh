export DEBIAN_FRONTEND=noninteractive

if [ -d $HOME/wxWidgets-install/include ] ; then
	echo "wxWidget already builded (and in travis cache)"
else
	wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.tar.bz2
	tar -xjf wxWidgets-3.1.0.tar.bz2
	mkdir $HOME/wxWidgets-install
	cd wxWidgets-3.1.0 && ./configure --prefix=$HOME/wxWidgets-install && make && make install
fi

$HOME/wxWidgets-install/bin/wx-config --version
wx-config --version
