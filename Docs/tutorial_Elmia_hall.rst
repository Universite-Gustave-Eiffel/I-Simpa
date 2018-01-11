Study of the Elmia hall
------------------------

In this example, we are interested in the case of a theater. This hall, the Elmia hall, has been the subject of measurements and a comparative study of several room acoustics software. The user can refer to the following reference for more information: `A Comparison of Room Simulation Software - The 2nd Round Robin on Room Acoustical Computer Simulation, Bork Ingolf, Acta Acustica united with Acustica, Volume 86, Number 6, November/December 2000, pp. 943-956(14) <http://www.ingentaconnect.com/content/dav/aaua/2000/00000086/00000006/art00008?utm_source=TrendMD&utm_medium=cpc&utm_campaign=Acta_Acustica_united_with_Acustica_TrendMD_0>`_.

Resources for this tutorial are located in the following folder:
::

	<I-Simpa installation folder>\doc\tutorial\tutorial 2

.. important::

	If not already done, we suggest you to follow the two previous tutorials, before the present tutorial:
	
	- `Study of a teaching room`_
	
.. _`Study of a teaching room`: tutorial_teaching_room.html	
	
Geometry import
~~~~~~~~~~~~~~~~

The 3D model that we have at our disposal presents numerous irregularities (surfaces in intersection in particular). According to the calculation code used, such as the SPPS code, it is possible that this geometry can not give rise to calculations. In this case, the use of the advanced model correction tools proposed by I-Simpa, at the scene importation, makes it possible to create a new model.

Import the 'bad' 3D model
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The 'bad' 3D model is already divided into 10 surface groups corresponding to the materials to be combined (audience, ceiling, floor, stages, stairs...). The first step is to load this model without performing geometric approximation processing in order to recover the existing surface groups:

1. **Create** a new project.
2. In the main menu, **Select** 'File' and 'Import new scene' and **Select** the ``Elmia.ply`` file located in ressource folder. The "Loading 3D Scene" window opens.
3. **Uncheck** all options in the window and **Select** 'OK'. The model is displayed in the 3D view, some misaligned faces give the impression that surfaces are missing. You can notice that 10 groups of surfaces are visible in the tree of the 'Scene' tab. This first operation was only intended to load these groups of surfaces in order to automatically re-associate each surface from the model that will be generated in the next task.

Create an approximated 3D model
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. **Choose** 'Import new scene' in the 'File' menu and **Select** the same 3D scene ``Elmia.ply``. The "Loading 3D Scene" window opens.
2. **Select** the 'Average model remesh' option and **Set** '1.' (1 m) for the parameter 'Association maximum distance'. Valid with 'OK'. The 'Mesh correction computation' window is displayed.
3. **Set** to 6 for the 'Model solving' paremeter. Using this value, the original 3D scene will be filled with :math:`2^6` cubes and **Valid** all options with 'OK'. A new windows is displayed, showing all closed 3D volume in the scene.
4. **Select** the volume that corresponds to the interior volume of the scene. In the present example, select the volume '9624 m3' and valid with 'Finish'. A new 3D scene, an approximate scene, is created and displays n the 3D view.

.. warning::

	The Average model remesh operation can only be used simultaneously on a single volume of the scene. If the scene contains several closed volumes, the user must retain only one.

5. In this operation, somes onf the faces of the new scene are not automatically assigned to surface groups. A new surfaces group 'model' is created and contains unassigned surfaces, which must then be redistributed to the corresponding groups. **Select** and **Hold** the 'model' group and **Move** it to the 'interiorwall' surfaces group. You can delete the group 'model' once it is empty.
6. To verify that the model correction has been effective, you can generate a meshing of the scene: in the 'Calculation' tab, **Open** the properties of the 'Meshing' element of the SPPS code and **Uncheck** the 'Scene correction before meshing' (since the 3D scene is already optimized). In the `'Meshing' toolbar_`, **Click** on the specific icon to generate the meshing. **Choose** SPPS in the new windows that is displayed and **Valid** with 'OK'. The meshing is displayed on the 3D view: you can check the 'x', 'y' or 'z' boxes in the Meshing toolbar to choose a axis and use the 'Slider' cursor to move the meshing.

.. _`'Meshing' toolbar`: toolbars.html#toolbar-meshing

Define sound sources
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Following the same procedure as for the tutorial 'Study of a teaching room' (`define sound sources`_), **Create** three omnidirectional sound sources at S01(8.5,0.0,2.1), S02(3.0,3.5,2.1), S03(1.0,1.0,0.9), with an arbitrary global sound power (for example a white noise @80dB).

.. _`define sound sources`: tutorial_teaching_room.html#define-a-sound-source

Define punctual receivers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Following the same procedure as for the tutorial 'Study of a teaching room' (`define punctual receivers`_), **Create** one or more punctual receivers, for example at R01(13.8,0.0,1.45), R02(12.9,10.5,5.3), R03(19.9,5.1,2.7), R04(25.5;-4.9,4.35), R05(24.8,11.9,5.7), R06(37.8,6.4,8.45).

.. _`define punctual receivers`: tutorial_teaching_room.html#define-two-punctual-receivers

Define a plane receiver
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The two main locations of the audience will be used to define plan receiver with a height of 50 cm from the audience surface:

1. For the first plane, **Click** on the 'Scene' tab and **Select** the 'New plane receiver' action from the context menu of the 'Surface receivers' root folder. A new plane receiver is added and a grid appears in the 3D view.

2. The 3 points defining the plane receiver must then be placed at the level of the two main locations of the audience, defined by the surfaces group "audience". To do this, **Use** the 'Define via 3D view' action of the context menu associated with the 'Vertex A', 'Vertex B' and 'Vertex C' elements of the surface receiver property. **Set** the positions of the vertices of the plane receiver to: A(11.93,-5.66,1.16), B(11.93,5.73,1.13), C(29.56,5.79,5.25).

3. Fo the second plane receiver, **Repeat** the previous 1-2 steps, with A(33.25,-8.22,6.26), B(33.17,8.58,6.23), C(40.73,8.58,9.47).

Define surface materials
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The present study from the Round Robin II, provides the materials in CATT-Acoustic format. The corresponding ``.txt`` file is available in the same folder as the 3D scene: ``material_catt.txt``.

1. **Right click** on the 'Project/Project Database/Materials/User' element and **Select** 'Import from File'.

2. **Select** the corresponding format from the drop-down list and **Open** the file on your computer. A new material folder is created named with the material file name.

3. **Assign** each material to the corresponding surfaces group following the same procedure as for the tutorial 'Study of a teaching room' (`define surfaces`_).

.. _`define surfaces`: tutorial_teaching_room.html#define-surfaces

SPPS calculation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Imported materials are only given for octaves between 125 Hz and 4000 Hz. The calculation must therefore be limited to these frequencies. In the 'Calculation' tab of the project, **Unfold** the 'SPPS' element, then **Right click** the 'Frequency bands' element and **Select** the option 'Automatic selection', 'Octave', 'Building/Road [125-4000Hz]'.
2. In the 'Meshing' element, **Uncheck** the element 'Scene correction before meshing'.
3. In the 'Properties' element, **Set**:
	a. '0.005' to the 'Time step (s)' element;
	b. 'Energetic' to the 'Calculation' element;
	c. '100 000' to the 'Number of sound particles per source'.
4. **Uncheck** the 'Export surface receivers for each frequency band' (to limit the size of the resulting files).
5. **Right click** on the 'SPPS' and **Select** 'Run calculation' to start the simulation.

Exploring the results
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Follow the same procedures as the 'Study of a teaching room' tutorial for `exploring the results`_.

.. _`exploring the results`:tutorial_teaching_room.html#results