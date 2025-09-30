## Building I-Simpa

#### Minimal Hardware requirements

* Processor 1.5 GHz Intel Pentium® or equivalent / higher
* Graphic card with OpenGl® (1.4 or higher) with display resolution 1024 x 728 or more
* Hard disk: 20 Go free space
* RAM: 512 Mo minimum required

#### Software & Dependencies requirements

* CMake
* Boost 1.73
* wxWidget 3.1.4
* Swig 3.0.11
* Python 3.8

:warning: This project uses OpenGL, check if your graphic driver is up to date !

---

### On Windows with Microsoft Visual Studio (64bits)

Last (free) version in date :  [Visual Studio Community 2019](https://www.visualstudio.com/)

#### Libraries installation

We recommend you to create a lib folder in `C:` and to use it when extracting extracting externals libraries source.

* **Boost**
	* [Download Boost 1.73](http://www.boost.org/users/history/)
	* Extract the archive
	* To build Boost : from the Visual Studio shell (Menu > All Programs > Visual Studio Tools), in the Boost folder
	
			.\bootstrap.bat
			# build just what we need
			.\b2. toolset=msvc-16.0 --with-thread --with-random --with-python --with-date_time --with-test --with-filesystem --with-regex --build-type=complete stage
			# or everything
			.\b2. toolset=msvc-16.0 --build-type=complete stage
	
	* Move the lib/ folder (inside stage/) to the Boost folder root
	* Create/check environment variable

			BOOST_INCLUDEDIR		path\to\boost\
			BOOST_LIBRARYDIR		path\to\boost\lib\

* **Swig**
	* [Download Swig](http://www.swig.org/)
	* Extract the archive
	* Add to Path the Swig folder path
* **wxWidgets**
	* [Download the wxWidget 3.1.4 Windows Installer](http://www.wxwidgets.org/downloads/)
	* The installer extract the source in a folder
	* In the wxWidget folder, in build/msw, launch wx_vc16.sl
	* Choose the "debug" mode and generate the solution
	* Choose the "release" mode and generate the solution again
	* Quit Visual Studio
	* Create/check environment variable

			WXWIN		path\to\wxwidget
	
* **Python**
	* [Download Python 3.8](https://www.python.org/downloads/)
	* Install it (the installer has an option that can do the two next steps for you)
	* Add to Path the Python installation folder path
	* Create/check environment variable
	
			PYTHONPATH		path\to\python\Lib

#### Building I-Simpa

1. Generate the Visual Studio project with CMake (GUI or command line), be careful to choose a different and empty build folder. Normally, all libraries will be found by CMake (if not, good luck).
2. In the build folder, launch isimpa.sln
3. Define i-simpa as the start project (right clic)
4. Generate ALL_BUILD in debug mode and in release mode, then generate INSTALL
6. You will find I-Simpa ready to launch in the bin folder of your build folder

### On GNU/Linux (GCC or Clang)

Travis will tell you if our Linux build work : [![Build Status](https://travis-ci.org/Ifsttar/I-Simpa.svg?branch=master)](https://travis-ci.org/Ifsttar/I-Simpa)

The travis build commands under linux are here https://github.com/Ifsttar/I-Simpa/blob/master/.travis.yml 

We have currently issues on OpenGL calls under linux. Display of particles and surface receivers are not working in Gnu/Linux. However all the other features appears to work.

#### Dependencies

In order to build under linux you have to download/install the following libraries:

- gcc-4.8
- g++-4.8
- python 3.8
- freeglut3-dev
- libxmu-dev
- libxi-dev
- libpng-dev
- libjpeg-dev
- libxxf86vm1
- libxxf86vm-dev
- libxi-dev
- libxrandr-dev
- mesa-common-dev
- mesa-utils
- libgl1-mesa-dev
- libglapi-mesa
- libgtk-3-dev
- byacc
- gettext

Swig from here:
https://github.com/swig/swig/archive/rel-3.0.10.tar.gz

Boost from here:
https://sourceforge.net/projects/boost/files/boost/1.73.0/boost_1_73_0.tar.bz2/download

wxWidgets from here:
https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.4/wxWidgets-3.1.4.tar.bz2

Cmake from here (or from apt-get)
https://cmake.org/files/LatestRelease/cmake-3.22.0.tar.gz

#### Building instructions

Creating makefile using CMake program (from the I-Simpa source directory):

```sh
export CXX="g++-4.8"
export CC="gcc-4.8"

cmake --version
${CC} --version
${CXX} --version

mkdir build
cd build

CLANG_WARNINGS=""

# wxWidget path (only in script scope)
export PATH=$HOME/wxWidgets-install/bin/:$HOME/swig-install/bin/:$HOME/cmake-install/bin/:$PATH
export BOOST_LIBRARYDIR=$HOME/boost-install/lib/
export BOOST_INCLUDEDIR=$HOME/boost-install/include/
export BOOST_ROOT=$HOME/boost-install/

cmake ..
```

Then you can run the build:

```sh
make
make DESTDIR=/home/john/isimpabuild/ install
```

Then run isimpa program located in the specified destdir.

### Using Docker Development Container

Install [docker](https://docs.docker.com/engine/install/) and [docker compose](https://docs.docker.com/compose/install/)

#### Open dev container in VSCode

Install [remote development](https://marketplace.visualstudio.com/items?itemname=ms-vscode-remote.vscode-remote-extensionpack) extensions for VSCode

Open VSCode and just click in the `reopen in container` button on bottom right corner. If you don't see anything press `Ctrl+Shift+P` and type `Remote-Containers: Rebuild and Reopen in container` or `Docker-images: Build Image` option.

Open a terminal session and build I-Simpa inside the container

```
bash scripts/build.sh
```

You only need to do this once, then you can open I-Simpa running the executable:

```
/home/simpa/isimpabuild/workspace/build/bin/isimpa
```

Save your projects on the `/workspace` directory so you have access to them outside the container.


#### On a terminal session

Open the project and head to the `.devcontainer` directory:

```
cd .devcontainer/
```

Run the container

```
docker compose up -d simpa-container
```

If it is the first time, wait for the container to build and install all the dependencies.

Attach to the container

```
docker attach devcontainer-simpa-container-1
```

Build I-Simpa inside the container

```
bash scripts/build.sh
```

You only need to do this once, then you can open I-Simpa running the executable:

```
/home/simpa/isimpabuild/workspace/build/bin/isimpa
```

Save your projects on the `/workspace` directory so you have access to them outside the container.

### Other installation/configuration

Contributors can try other configurations, using more recent libraries (wxWidgets for example), other IDE (Code::Blocks for example) or a more recent version of Visual Studio... If you success in building and running I-Simpa, we will appreciate that you share your experience, adding it in this wiki. Please <a href="mailto:i-simpa@ifsttar.fr">contact us by mail</a>.

### Need information

If you have some problem during the building procedure, please <a href="mailto:i-simpa@ifsttar.fr">contact us by mail</a>.
