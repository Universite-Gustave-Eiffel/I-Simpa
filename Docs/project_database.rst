Project database
================

The Projet database is an element of the 'Projet' tree. This database is composed of three database:

- `Directivities`_ for sound source and receiver
- `Materials`_ for the scene boundaries
- `Spectrum`_ for receiver and sound sources

For each database, two sub-database are given:

- a 'Reference' database that contains built in information, which can not be edited by the user
- a 'User' database that can be edited bu the user to create new elements

.. tip::
	Note that it is possible to copy an element from the 'Reference' database to the 'User' database.
	Copy/Paste is also possible for folder.

Directivities
-------------
This feature is experimental and is not documented.

Materials
------------

Contextual menu
~~~~~~~~~~~~~~~

**Right click** on the 'User' folder to define a new material or a group of material.

- 'New Material'
	Creates a new material in the corresponding group.

- 'Create group'
	Creates a new group in the corresponding group.

- 'Import from a file'
	Allows to import materials from a file. Several formats are supported:

Importing from CATT-Acoustic Format
			File used by the CATT-Acoustic software: text file with extension '.txt' (see `CATT-Acoustic website`_).

			CATT-Acoustic material file example:
			::

				panelwalls = < 21 12 9 6 9 13 > L < 15 13 11 9 7 5 >
				sidereflector = < 20 12 6 4 7 10 > L < 15 20 25 35 45 50 >
				audience = < 45 60 73 80 75 64 > L < 30 40 50 60 65 70 >
				rearaudience = < 50 66 80 88 83 70 > L < 30 40 50 60 65 70 >
				linoleum = < 2 3 3 4 6 5 > L < 30 27 24 21 18 15 >
				reflectors = < 12 10 4 3 3 2 >
				stage = < 15 8 6 6 6 6 > L < 15 13 11 9 7 5 >
				ceiling = < 20 15 10 8 4 2 > L < 15 13 11 9 7 5 >
				speakerwindow = < 2 6 3 3 2 2 > L < 30 27 24 21 18 15 >
				plasteredconcrete = < 2 2 3 3 4 6 > L < 15 13 11 9 7 5 >
				ventilationgrid = < 8 12 15 15 12 8 > L < 30 27 24 21 18 15 >
				floor = < 15 8 6 6 6 6 > L < 15 13 11 9 7 5 >
				interiorwalls = < 20 12 6 4 7 10 > L < 15 13 11 9 7 5 >
				stairs = < 15 8 6 6 6 6 > L < 15 13 11 9 7 5 >
				exteriorwalls = < 20 12 6 4 7 10 > L < 15 20 25 35 45 50 >

			CATT-Acoustic material file import example in I-Simpa

.. image:: images/Screenshots/CATT_import.PNG
   :align: center
   :width: 500px

.. rst-class::  clear-both

		Importing from Odeon Format
			File used by the software Odeon: text file with extension '.li8' (see `Odeon website`_);

		.. warning::

			The CATT-Acoustic and Odeon data files are defined octave bands, while I-Simpa uses third octave bands. During the importation, values per octave band are attributed to the corresponding third octaves. Furthermore, since I-Simpa takes into accound upper and lower frequency bands than the last software, data are extrapolated from the closest frquency band data.

		.. note::

			During importation, all fields of the original file are not imported from the data files.

Properties
~~~~~~~~~~

-  'Display'
		Opens a dialog box to assign a color to the material, for the representation of the material in the scene (see `Menu view`_).

-  'Description'

	+  'Description'
			String to describe the material.

	+  'Mass density (kg/m3)'
			Density of the material (real).

	+  'Reference'
			Reference associated with the material (text).

	+  'Resistivity (kN.mm-4)'
			Resistivity of the material (real value).

	+  'Side of material effect'
			List for choosing how the material properties are considered in relation to the direction of propagation	   relative to the normal to the material:

				*  'Bilateral'
						The material has the same properties on both sides of the surface to which it is associated.

				*  'Unilateral'
						The material is transparent in one direction. The acoustic properties of the material are considered only in a propagation direction opposite to the normal to the face. See the guide of the numerical code to know how this property is used.

-  'Spectrum'
		Defines the acoustic properties of the material for each frequency band.

-  'Absorption'
		Absorption coefficient of the material (real value between 0 and 1).

-  'Diffusion'
		Scattering coefficient of the material in order to model wall diffusion (real value between 0 and 1).

-  'Transmission'
		Check/uncheck for taking acoustic transmission into account.

-  'Loss'
		Transmission loss of the material (dB).

-  'Diffusion law'
		Reflection law for the diffuse part of the reflection;

	+ 'Lambert'
		Diffuse Lambert reflection law (cosinus of the incident angle).

	+ 'Specular'
		Specular reflection law.

	+ 'Uniform'
		Uniform reflection law, (*i.e.* 'W' reflection law that corresponds to cosinus^2 of the incident angle).

	+ 'W2'
		'W2' reflection law (cosinus^2 of the incident angle).

	+ 'W3'
		'W3' reflection law (cosinus^3 of the incident angle).

	+ 'W4'
		'W4' reflection law (cosinus^4 of the incident angle).


.. note::

	For a scattering coefficient set to 0 (no diffuse reflection), the 'Diffusion law' is set to 'Specular', and can not be modified.

.. note::

	For a scattering coefficient set to 1 (full diffuse reflection), the 'Diffusion law' is set to 'Specular' by default, but can be changed.

.. warning::

	**Absorption and transmission loss are linked together**. Since transmission is a part of the absorption (:math:`\alpha=\beta+\tau`, see for example `SPPS documentation`_), the transmission coefficient :math:`\tau` can not be larger than the absorption coefficient :math:`\alpha`. For example, for a given absorption coefficient of :math:`\alpha=0.2`, the transmission loss TL (:math:`TL=-10\log\tau`) must be greater or equal than 7 dB (*i.e.* :math:`TL \geq -10 \log \alpha=-10 \log 0.2 \approx 7` dB.

	- If the transmission coefficient (:math:`\tau=10^(-TL/10)`) value is larger than the possible absorption, the absorption coefficient is automatically adjusted.

	- If the absorption coefficient is changed, the transmission loss :math:`TL=-10\log\tau` can be automatically adjusted if necessary.

Spectrum
------------

Spectrum are can be used either for the definition of the source emission or for the definition of the background noise at a receiver. |usingspectrum|.

Contextual menu
~~~~~~~~~~~~~~~

**Right click** on the 'User' folder to define a new sepctrum.

Properties
~~~~~~~~~~

User has to define the spectrum value for each frequency band, either in 'dB' or in 'dB(A)'.

.. note::

	Values in in 'dB' and in 'dB(A)' are linked together. Changing one value, for example in dB (respectively in dB(A)), will change the dB(A) value (respectively the dB value).

.. note::

	Changing the 'Global' value will affect the value for each frequency band: all values are shifted in order to respect the global value.


.. _Menu view: Menu_View.html
.. _`Using spectrum`: using_spectrum.html
.. _`SPPS documentation`: code_SPPS_modelling.html#acoustic-transmission-modelling
.. _`CATT-Acoustic website`: http://www.catt.se
.. _`Odeon website`: http://www.odeon.dk
.. |usingspectrum| replace:: See the section `Using spectrum`_ for understanding how to use a spectrum in the application
