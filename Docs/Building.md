## Building I-Simpa

#### Minimal Hardware requirements

* Processor 1.5 GHz Intel Pentium® or equivalent / higher
* Graphic card with OpenGl® (1.4 or higher) with display resolution 1024 x 728 or more
* Hard disk: 20 Go free space
* RAM: 512 Mo minimum required

#### Software & Dependencies requirements

* CMake
* Boost 1.60
* wxWidget 3.1.0
* Swig
* Python 2.7

:warning: This project uses OpenGL, check if your graphic driver is up to date !

---

### On Windows with Microsoft Visual Studio (64bits)

Last (free) version in date :  [Visual Studio Community 2015](https://www.visualstudio.com/)

#### Libraries installation

We recommend you to create a lib folder in `C:` and to use it when extracting extracting externals libraries source.

* **Boost**
	* [Download Boost 1.60](http://www.boost.org/users/history/)
	* Extract the archive
	* To build Boost : from the Visual Studio shell (Menu > All Programs > Visual Studio Tools), in the Boost folder
	
			.\bootstrap.bat
			# build just what we need
			.\b2. toolset=msvc-14.0 --with-thread --with-random --with-python --with-date_time --with-test --with-filesystem --with-regex --build-type=complete stage
			# or everything
			.\b2. toolset=msvc-14.0 --build-type=complete stage
	
	* Move the lib/ folder (inside stage/) to the Boost folder root
	* Create/check environment variable

			BOOST_INCLUDEDIR		path\to\boost\
			BOOST_LIBRARYDIR		path\to\boost\lib\

* **Swig**
	* [Download Swig](http://www.swig.org/)
	* Extract the archive
	* Add to Path the Swig folder path
* **wxWidgets**
	* [Download the wxWidget 3.1.0 Windows Installer](http://www.wxwidgets.org/downloads/)
	* The installer extract the source in a folder
	* In the wxWidget folder, in build/msw, launch wx_vc14.sl
	* Choose the "debug" mode and generate the solution
	* Choose the "release" mode and generate the solution again
	* Quit Visual Studio
	* Create/check environment variable

			WXWIN		path\to\wxwidget
	
* **Python**
	* [Download Python 2.7](https://www.python.org/downloads/)
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

I-Simpa do not use any platform specific library so in theory it can run on Linux.
In practice it is not that simple, Travis will tell you if our Linux build work : [![Build Status](https://travis-ci.org/Ifsttar/I-Simpa.svg?branch=master)](https://travis-ci.org/Ifsttar/I-Simpa)

### Other installation/configuration

Contributors can try other configurations, using more recent libraries (wxWidgets for example), other IDE (Code::Blocks for example) or a more recent version of Visual Studio... If you success in building and running I-Simpa, we will appreciate that you share your experience, adding it in this wiki. Please <a href="mailto:i-simpa@ifsttar.fr">contact us by mail</a>.

### Need information

If you have some problem during the building procedure, please <a href="mailto:i-simpa@ifsttar.fr">contact us by mail</a>.
