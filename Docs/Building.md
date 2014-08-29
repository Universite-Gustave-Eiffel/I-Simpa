## Building I-Simpa

I-Simpa is developed in C/C++ using Visual Studio 2008.
For the rest of the tutorial, we assume that you got already Visual Studio 2008.

#### Minimal Hardware requirements
* Processor 1.5 GHz Intel Pentium® or equivalent / higher
* Graphic card with OpenGl® (1.4 or higher) with display resolution 1024 x 728 or more
* Hard disk: 20 Go free space
* RAM: 512 Mo minimum required

Following step by step the configuration / installation of all libraries required for I-Simpa:
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/boost">Install Boost</a>
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/GITEXTENSION">Install Git Extensions</a>
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/wxWidgets">Install wxWidgets</a>
* <a href="https://github.com/Ifsttar/I-Simpa/wiki/python">Install Python</a>

You could be able to build I-Simpa using the `I-Simpa.sln` file in `I-Simpa\Source\isimpa\trunk`:

Please note that in debug building you will need to put two folders into the debug folder that Visual Studio create:
* `Bitmap` that contain all images for the software
* `Locale` that contain some `.po` files

If you get any error/unsuccess compilation:
* first clean all the solution
* then, remove all data from your `X:\Users\U\AppData\Local\Temp` I-Simpa folder, where `X:` is your hard disk letter, and `U` your name user (NB: AppData is an hidden directory, change display configuration on your windows explorer to see it)
* try to rebuild all the solution

If you still get an error, <a href="mailto:i-simpa@ifsttar.fr">Contact us</a>.
