## Building I-Simpa

I-Simpa is developed in C/C++ using Visual Studio 2008.
For the rest of the installation procedure, we assume that you got already Visual Studio 2008.

#### Minimal Hardware requirements
* Processor 1.5 GHz Intel Pentium® or equivalent / higher
* Graphic card with OpenGl® (1.4 or higher) with display resolution 1024 x 728 or more
* Hard disk: 20 Go free space
* RAM: 512 Mo minimum required

#### Libraries installation
Following step by step the configuration/installation of all libraries required for I-Simpa:
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/boost">Install Boost</a>
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/Git-Extensions">Install Git Extensions</a>
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/wxWidgets">Install wxWidgets</a>
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/python">Install Python</a>

### Building I-Simpa
You could be able to build I-Simpa using the `I-Simpa.sln` file in `I-Simpa\Source\isimpa\trunk`:

If you experience any error/unsuccess compilation:
* first clean all the solution;
* then, remove all data from your `X:\Users\U\AppData\Local\Temp` I-Simpa folder, where `X:` is your hard disk letter, and `U` your name user (NB: AppData is an hidden directory, change display configuration on your windows explorer to see it);
* try to rebuild all the solution.

### Running I-Simpa

After building in the "Release" mode, the executable file `I-Simpa.exe` will be localized in the `currentRelease` folder at the root folder of the local GitHub repository. The `currentRelease` folder contain all necessary files for running I-Simpa.

In the "Debug" building mode, if you want to run I-Simpa, you will need two copy two folders (from the `currentRelease` folder) into the debug folder that Visual Studio will create:
* `Bitmap` that contain all images for the software;
* `Locale` that contain some `.po` files.

### Need information
If you have some problem during the building procedure, please <a href="mailto:i-simpa@ifsttar.fr">contact us by mail</a>.

#### Other installation configuration

Contributors can try other configurations, using more recent libraries (wxWidgets for example), other IDE (Code::Blocks for example) or a more recent version of Visual Studio. If you success in building and running I-Simpa, we will appreciate that you share your experience, adding it in this wiki. Please <a href="mailto:i-simpa@ifsttar.fr">contact us by mail</a>.