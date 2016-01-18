export DEBIAN_FRONTEND=noninteractive

wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository 'deb http://llvm.org/apt/precise/ llvm-toolchain-precise-3.7 main' -y

sudo add-apt-repository ppa:smspillaz/cmake-3.0.2 -y
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq
sudo apt-get install --force-yes --no-install-recommends --no-install-suggests \
        cmake \
        cmake-data \
        git \
        python \
		libboost-thread-dev \
		libboost-random-dev \
		libboost-system-dev \
		libboost-test-dev


sudo -H pip install autopep8 # TODO when switching to trusty or above: replace python-pip with python-autopep8

#update clang
sudo apt-get install --force-yes llvm-3.7 llvm-3.7-dev clang-3.7 libstdc++-4.9-dev