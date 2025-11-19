I-Simpa overview
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

I-Simpa alone is **not a calculation software**, but is equivalent to a pre and post-processor
for acoustic codes. One or more numerical codes must be added in order to obtain a fully functional system.

At the present time, **I-Simpa is distributed with two codes**:

- `TCR code`_ based on the classical theory of reverberation
- `SPPS code`_ based on particle tracing approach

The resume of `all code characteristics are given here`_

.. _`TCR code`: code_TCR.html
.. _`SPPS code`: code_SPPS.html
.. _`all code characteristics are given here`: code_characteristics.html

Classical applications are room and building acoustics, environmental noise and industrial noise, but it can be easily extend to other applications concerning the sound propagation in 3D environments (interior of vehicle, sound in cavities…).

I-Simpa has been initially developed as a research tool (i.e. for research laboratories), but can also be a very efficient tool for a professional/commercial use, as well as for education.

**Functionnal**
	Elements and components are organized in tree structures, to easily access to all information, parameters and properties. Many features are proposed for helping users

**Open system**
	All information and data are organized in spreadsheets that can be displayed, exported, copied… User can use their own software for processing data and graphical representation.

**Open software**
	User can integrate their own numerical propagation code, develop their own functionalities within the interface, using Python scripts… All have been done in order you can extend I-Simpa easily.

**Open source project**
	The source code of I-Simpa is freely available on the GitHub plateform (https://github.com/Ifsttar/I-Simpa). There are many way for the community to contribute to the development of I-Simpa.

I-Simpa features
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Geometry
------------------


.. image:: images/Features/Import_geometry.png
	:align: left
	:width: 140px


Import 3D files
	Import 3D files with common extensions: 3DS (3D Studio), STL (stereolithography), PLY (Stanford) and POLY (TetGen).

,,,,


.. image:: images/Features/Correction_model.png
	:align: left
	:width: 140px

Geometry correction
	Automatic tools for model corrections and repairs during file importation. Model detection error.

,,,,


.. image:: images/Features/geometry_approximation.png
	:align: left
	:width: 140px

Geometry approximation
	Algorithm for model approximation by the marching cube method. This can be useful for original 3D scene with many problems.

,,,,


.. image:: images/Features/creating_scene.png
	:align: left
	:width: 140px


Parallelepipedic geometry
	Built-in design of parallelepipedic model. Just gives the dimensions of the scene.

,,,,

Surface material
---------------------


.. image:: images/Features/import_material.png
	:align: left
	:width: 140px

Import data material
	Import data from file (import from CATT-Acoustic™ and Odeon room acoustics software).

,,,,


.. image:: images/Features/manipulate_materials.png
	:align: left
	:width: 140px

Reference and User material database
	Use the built-in database (Reference) or create your own materials (User database). Organize and copy material between (sub-)groups of material.

,,,,


.. image:: images/Features/material_properties.png
	:align: left
	:width: 140px

Material properties
	Define acoustic parameters by frequency band: absorption, transmission loss, scattering, scattering reflection law. Add material information (description, physical parameters...).

,,,,

Sound sources
---------------------

.. image:: images/Features/spectrum_data.png
	:align: left
	:width: 140px

Reference and User spectrum database
	Use the built-in database (Reference) or create your own source spectrum (User database).

,,,,

.. image:: images/Features/source_properties.png
	:align: left
	:width: 140px

Sources properties
	Define source properties: spectrum, position, directivity, delay, activate/desactivate sources.

,,,,

.. image:: images/Features/Manipulate_sources.png
	:align: left
	:width: 140px

Sources manipulation
	Organize and copy sources between (sub-)groups of material. Translation and rotation of (sub-)groups of sound sources.

,,,,

Acoustic receivers
---------------------


.. image:: images/Features/Punctual_receiver_properties.png
	:align: left
	:width: 140px

Punctual sound sources
	Defines source properties: position, directivity, orientation to a point or (with a dynamic link) to a source, background noise.

,,,,

.. image:: images/Features/manipulate_punctual_receivers.png
	:align: left
	:width: 140px

Receivers manipulation
	Organize and copy receivers between (sub-)groups of material. Translation and rotation of (sub-)groups of receivers.

,,,,

.. image:: images/Features/surface_receiver.png
	:align: left
	:width: 140px

Surface receivers
	Definition of a surface receiver by selecting surfaces of the 3D model or by creating cut planes.

,,,,


Acoustic calculation
---------------------

.. image:: images/Features/calculation_codes.png
	:align: left
	:width: 140px

Calculation code
	Built-in acoustic calculation code: sound particle tracing (SPPS); Classical theory of reverberation for room acoustics (TCR).

,,,,

.. image:: images/Features/sound_map.png
	:align: left
	:width: 140px

Sound map
	Noisemap (surface receivers) in stationary stationary state, time-varying state, time-cumulated state. Iso-contours.

,,,,

.. image:: images/Features/acoustical_parameters.png
	:align: left
	:width: 140px

Acoustics parameters
	:term:`EN ISO 3382-1` room acoustics parameters: Reverberation times, Clarity, Definition, Early support, Centre time, Strength, Early Lateral Energy Fraction, Late Lateral Sound Level.

,,,,

.. image:: images/Features/result_animation.png
	:align: left
	:width: 140px

Animation
	Intensity vectors animation for punctual receivers. Ray-tracing and particle-tracing animation.

,,,,


Other features
---------------------


.. image:: images/Features/python_script.png
	:align: left
	:width: 140px

Python™ scripting
	Extension of I-Simpa functionalities using Python scripts. Creations of Toolbox.

,,,,

.. image:: images/Features/environmental_data.png
	:align: left
	:width: 140px

Environmental data
	Definition of atmospheric conditions and calculation of absorption coefficient (:term:`ISO 9613-1`). Meteorological parameters (log-lin sound speed velocity profiles).

,,,,

.. image:: images/Features/meshing.png
	:align: left
	:width: 140px

Tetrahedric meshing
	Built-in Delaunay tetrahedric meshing using TetGen code (fully parametrized).

,,,,

.. image:: images/Features/fitting_zone.png
	:align: left
	:width: 140px

Fitting zone
	Definition of volumes with fitting objects characterized by probabilistic parameters (:term:`mean free path`, absorption, diffusion law).
