FROM ubuntu:jammy

ARG USERNAME=simpa
ARG USER_UID=1000
ARG USER_GID=$USER_UID
ENV DEBIAN_FRONTEND=noninteractive
ENV HOME=/home/${USERNAME}

# Create user
RUN groupadd -g ${USER_GID} ${USERNAME} \
 && useradd -m -u ${USER_UID} -g ${USER_GID} -s /bin/bash ${USERNAME}

# Basic tools + build deps
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    build-essential \
    software-properties-common \
    wget \
    curl \
    ca-certificates \
    git \
	g++ \
    python3 \
    python3-pip \
	python3-dev \
	autotools-dev \
	build-essential \
	libicu-dev \
	libbz2-dev \
	libboost-all-dev \
    vim \
    pkg-config \
    autoconf \
    automake \
    libtool \
    m4 \
    unzip \
    bzip2 \
    xz-utils \
    libxmu-dev \
    libxi-dev \
    libpng-dev \
    libjpeg-dev \
    libxxf86vm-dev \
    libxrandr-dev \
    mesa-common-dev \
    mesa-utils \
    libgl1-mesa-dev \
    libglapi-mesa \
    libgtk-3-dev \
    freeglut3-dev \
    byacc \
    gettext \
    cmake \
    ca-certificates \
 && rm -rf /var/lib/apt/lists/*

# Try installing gcc-4.8/g++-4.8 (best-effort). If package unavailable, fallback to default gcc.
RUN set -eux; \
    if apt-get update && apt-get install -y gcc-4.8 g++-4.8 ; then \
      update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 90 || true; \
      update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 90 || true; \
      echo "Installed gcc-4.8 successfully"; \
    else \
      echo "gcc-4.8 not available on this distro; using system gcc/g++ as fallback"; \
    fi

# Switch to non-root user context for building heavy packages in /home
USER ${USERNAME}
WORKDIR ${HOME}

ENV PATH=${HOME}/swig-install/bin:${HOME}/wxWidgets-install/bin:${HOME}/cmake-install/bin:${PATH}
ENV BOOST_ROOT=${HOME}/boost-install
ENV BOOST_INCLUDEDIR=${HOME}/boost-install/include
ENV BOOST_LIBRARYDIR=${HOME}/boost-install/lib
ENV CC=/usr/bin/gcc
ENV CXX=/usr/bin/g++

# Download & build SWIG 3.0.10 (as requested)
RUN set -eux; \
    SWIG_VER=3.0.10; \
    mkdir -p ${HOME}/buildsrc && cd ${HOME}/buildsrc; \
    if [ ! -f swig-${SWIG_VER}.tar.gz ]; then \
      wget -q https://github.com/swig/swig/archive/rel-${SWIG_VER}.tar.gz -O swig-${SWIG_VER}.tar.gz; \
    fi; \
    tar -xf swig-${SWIG_VER}.tar.gz; \
    cd swig-rel-${SWIG_VER}; \
    ./autogen.sh || true; \
    ./configure --prefix=${HOME}/swig-install; \
    make -j$(nproc) && make install

# Download & build Boost 1.74.0
RUN set -eux; \
    mkdir -p ${HOME}/buildsrc && cd ${HOME}/buildsrc; \
    if [ ! -f boost_1_74_0.tar.bz2 ]; then \
      wget -q https://sourceforge.net/projects/boost/files/boost/1.74.0/boost_1_74_0.tar.bz2/download -O boost_1_74_0.tar.bz2; \
    fi; \
    tar --bzip2 -xf boost_1_74_0.tar.bz2; \
    cd boost_1_74_0; \
    ./bootstrap.sh --with-libraries=system,filesystem,regex,thread,chrono,date_time,python,test,timer,random \
                   --prefix=$HOME/boost-install; \
    ./b2 install -j"$(nproc)"

# Download & build wxWidgets 3.1.4
RUN set -eux; \
    WX_VER=3.1.4; \
    mkdir -p ${HOME}/buildsrc && cd ${HOME}/buildsrc; \
    if [ ! -f wxWidgets-${WX_VER}.tar.bz2 ]; then \
      wget -q https://github.com/wxWidgets/wxWidgets/releases/download/v${WX_VER}/wxWidgets-${WX_VER}.tar.bz2 -O wxWidgets-${WX_VER}.tar.bz2; \
    fi; \
    tar -xf wxWidgets-${WX_VER}.tar.bz2; \
    cd wxWidgets-${WX_VER}; \
    mkdir -p build_unix && cd build_unix; \
	../configure --prefix=${HOME}/wxWidgets-install --enable-unicode --enable-compat28=no --with-gtk=3; \
    make -j$(nproc) && make install

# Add Boost & wxWidgets to runtime library search path
ENV LD_LIBRARY_PATH=${HOME}/wxWidgets-install/lib:${HOME}/boost-install/lib:$LD_LIBRARY_PATH

CMD ["/bin/bash"]
