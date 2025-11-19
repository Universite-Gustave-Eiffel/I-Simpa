
'New project'
	Create a new project (empty project)

'Open project'
	Open an existing project with the I-Simpa file format ``*.proj``

'Import new scene'
	Import a new 3D geometry in the existing project. Some `Import options`_ are available at this step.

	Import file formats:

	-  `.3DS`_:  keep all surface groups and texture
	-  `.PLY`_: ASCII and BIN format, keep all surface groups
	-  `.POLY`_
	-  `.STL`_

.. tip::
	When you import a new scene, except the geometry, all previous data (sources, receivers, materials...) of the current project are unchanged. You can use this option if you want to keep all current data, and just import a new geometry. If you want to remove all data, you need to create a new project.

.. note::
	Numerical codes within I-Simpa (like SPPS) are very sensitive to the quality of the import geometry. Although I-Simpa can correct some defaults during importation of a geometry, it is recommended to pay a particular attention to the creation of the geometry. `Read the recommandations here.`_

'Export scene'
	Export the current 3D geometry from the current project (without additional data)

	Export file formats:

	-  `.PLY`_
	-  .MAT.PLY: same as PLY, with surface groups
	-  `.POLY`_
	-  `.MESH`_
	-  `.NFF`_
	-  .BIN: I-Simpa file format
	-  .ASC

'New scene'
	This option able to create a parallelepipedic scene. This is the simple way to create a 3D scene with I-Simpa. This can be useful for starting with I-Simpa. For more realistic scenes, users have to build 3D scenes using a CAD software, and then, importing the geometry using one the possible import file format (see **'Import new scene'** in the file menu).

.. note::
	Using the built-in option for creating a rectangular geometry, each main face of the geometry (*i.e.* there are 6 main faces), is split into 2 triangular face elements. This mean that, when defining the acoustical material for each face, you will only have the possibility to affect two different materials for each main face (one on each triangular face). Most of times, users will consider a single material for each group.

'Save project'
	Save the current project with the current filename.

'Save project as'
	Save the current project with a new/given filename.

'Save copy of project'
	Save the current project with a new filename, by default, in the same folder as the original project.

'Recent projects'
	Show the more recent projects. Load a project by selecting the file in the list.

'Settings'
	Defines I-Simpa settings.

	- 'Options'
		Defines general options.

			* '3D rendering'
				Defines 3D rendering options.

				+ 'Animations'
					Defines 3D animation options.

					* 'Label color'
						Color of the legend concerning animations.

					* 'Particles display'
						Show/hide particle animation.

					* 'Render frequency'
						Set frequency of animations (Hz). the speed of animations increases with frequency.

					* 'Surface receiver display'
						Show/hide surface receiver animations.

				+ 'Legend'
					Defines legend options.

					* 'Legend font'
						Legend font (Text Dialog box).

					* 'Text background color'
						Background color of the text legend (Color Dialog box).

					* 'Text color'
						Legend text color (Color Dialog box).

					* 'Transparent text background'
						Enable/disable the transparency of the legend background. If selected, the option 'Text background color' has no effect.

				+ 'General'
					Defines general 3D options.

					* 'Default color'
						Default color of an surface element of the geometry (Color Dialog box).

					* 'Scene background'
						Background color of the 3D view (Color Dialog box).

					* 'Scene lines'
						Color of lines for the geometry (Color Dialog box).

					* 'Selection'
						Color of a face selection (Color Dialog box).

				+ 'Particles'
					Defines particles options.

					* 'Color of particles'
						Color of particles (Color Dialog box).

				+ 'Sound map'
					Defines sound map options.

					* 'Iso-lines color'
						Color of lines used for displaying iso-contour representation (Color Dialog box).

					* 'Iso-levels color'
						Select the colormap for sound map representation. Colormaps can be added by users.

			* 'Main preferences'
				Defines the main preferences of the interface.

				+ 'History'
					Defines history options.

					* 'Keep modification history'
						Enable/disable 'Undo'/'Redo' options.

	- 'Language'
		Open a dialog box for selecting the interface language.

		.. note::
			I-Simpa must be restarted in order to apply the selected language.

	- 'Change application data folder'
		Allows to change the default folder for the Application Data

'Exit'
	Exit I-Simpa.

.. note::
	When starting I-Simpa, the last project (if available) will be automatically loaded.



.. _Read the recommandations here.: import_file_recommandations.html
.. _.3DS: http://en.wikipedia.org/wiki/.3ds
.. _.PLY: http://en.wikipedia.org/wiki/PLY_%28file_format%29
.. _.POLY: http://tetgen.berlios.de/fformats.poly.html
.. _.STL: http://en.wikipedia.org/wiki/STL_%28file_format%29
.. _.PLY: http://en.wikipedia.org/wiki/PLY_%28file_format%29
.. _.POLY: http://tetgen.berlios.de/fformats.poly.html
.. _.MESH: http://www.ann.jussieu.fr/frey/software.html
.. _.NFF: http://paulbourke.net/dataformats/nff/nff2.html
