export DEBIAN_FRONTEND=noninteractive
sudo apt-get install --force-yes --no-install-recommends --no-install-suggests \
    cmake \
    cmake-data \
    python \
    libboost-thread-dev \
    libboost-random-dev \
    libboost-system-dev \
    libboost-filesystem-dev \
    libboost-test-dev \
	libboost-python-dev \
	freeglut3-dev \
	libxmu-dev \
    libxi-dev \
	swig \
	libpng-dev \
	libjpeg-dev \
	libxxf86vm1 \
	libxxf86vm-dev \
	libxi-dev \
	libxrandr-dev \
	mesa-common-dev \
	mesa-utils-extra \
    libgl1-mesa-dev \
    libglapi-mesa \
    libgtk-3-dev

if [ -d /wxWidgets-install ] ; then
	echo "wxWidget already build (and in travis cache)"
else
	wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.tar.bz2
	tar -xjf wxWidgets-3.1.0.tar.bz2
	sudo mkdir /wxWidgets-install
	cd wxWidgets-3.1.0 && ./configure --prefix=/wxWidgets-install && make && sudo make install
fi
