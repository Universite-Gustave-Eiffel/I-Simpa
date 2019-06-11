Study of an industrial room
----------------------------

In this example, we are interested in the case of an industrial hall coupled with other rooms. This hall is composed of two identical noisy machines and two fitting zones (zones made of many objects that act as acoustics scaterrers). The purpose of this tutorial is:

- to illustrate the creation of machines
- illustrate the creation of fitting zones
- study the effect of changing a material on a wall of the industrial hall, on the sound field

Resources for this tutorial are located in the following folder:
::

	<I-Simpa installation folder>\doc\tutorial\tutorial 3

This folder contains several files:

- ``tutorial_3.proj``:  I-Simpa project of the present tutorial (without Results)
- ``Industrial_hall.ply``: 3D hall geometry

.. important::

	If not already done, we suggest you to follow the two previous tutorials, before the present tutorial:

	- `Study of a teaching room`_
	- `Study of the Elmia hall`_

.. _`Study of a teaching room`: tutorial_teaching_room.html
.. _`Study of the Elmia hall`: tutorial_elmya_hall.html

Import a geometry
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first step is to import the geometry of the project. We are interested here in a large industrial hall coupled to a corridor through a door in a wall, this corridor being itself coupled to another room. We assume that walls and doors allow acoustic transmission.

1. In the 'File' menu, **Select** 'Import new scene', and open the file ``LocalIndustriel.ply`` located in the correspondng tutorial folder.

2. In the next windows, **Select** 'OK' without changing the default values.

.. note::

	When importing the model, several surfaces groups are created. These groups were defined at the 3D scene creation with the CAD software.

Define a machine as a sound source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

At this step, we want to create the sound sources. In industrial environments, sound sources are often machines or workstations made up of several source points. Since the hall can be made up of several identical machines located at different locations, it is simpler to define a machine in the form of a group of sound sources, which group can then be copied and moved within the room. In our example, we will define machines composed of three sound sources, which will be duplicated. Each sound source will be defined by a pink noise spectrum, with a global level of 80 dB.

1. **Right-click** the 'Data/Sound Sources' element in the 'Scene' tab of the 'Project' windows, and **Select** 'New Group'. By default, the name of the new group is 'Sound sources'.
2. **Rename** the group as the following 'Milling machine';
3. **Right-click** on the 'Milling Machine' group and select 'New Source'. **Perform** this operation three times (to create three sources), named 'Source 1', 'Source 2' and 'Source 3'. It is possible to modify the name of the sources by applying the same method as for the name of a group;
4. For each source, **Unfold** the source properties, then **Click** on the 'Position' item, and **Assign** the following coordinates (x,y,z) for the source 1 (2,7,1.2), the source 2 (2,6,0.6) and the source 3 (1,7,0.75).
5. For each source, **Select** the 'Sound power' element, **Choose** 'Pink Noise', and then **Set** the global level 'Lw' to 80dB.

Duplicate the machine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The previous machine can then be duplicated and moved into the room.

1. **Right-click** on the 'Milling Machine' group and **Select** 'Copy';
2. **Right-click** the 'Sound Sources' root folder, then **Select** 'Paste': an item named 'Copy of Sound sources' is created.

.. note::

	The sound sources within the new group have exactly the same properties as the original sources, including the names of the sources. However there is no possible confusion between two sources with the same name, each source having its own internal identifier.

3. **Rename** the group created in 'Milling machine 2';
4. Since the new machine is located in the same place as the original machine, it is necessary to move it. To perform a translation of the new machine, **Right-click** on the 'Milling Machine 2' group and **Select** 'All sources>Translation';
5. **Set** the translation values in each direction (x,y,z) to [5,-2,0], and **Click** on 'OK'. The sources group is translated.

Define a scene volume as a fitting zone
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are two methods of creating a fitting zone in the room:

- when creating the scene with the CAD software. In this case, a closed volume is modeled in the 3D geometric model. It is then a 'Scene fitted zone'
- either in I-Simpa, in the form of a parallelepipedal zone. It is then a 'Rectangular fitted zone'

In our example, a parallelepipedal zone has already been provided at the creation of the 3D scene, in order to be assigned to a 'Scene fitted zone'.

1. In the 'Scene' tree, **Right-click** on the 'Fitting zone' root folder and **Select** 'Define scene fitted zone';

2. Now you have to define the surfaces of the scene for the corresponding zone. **Select** the surface selection mode on the 'Selection' toolbar; **Hold** Ctrl keyboard key and **Double-click** on each face element of the the fitting zone. Each face element is selected in red (or another color depending of the I-Simpa Settings). At the same time, the corresponding face elements are also highlighted in the folder 'Surfaces' of the 'Data' tree.

.. note::

Holding the Ctrl keyboard key allow to select all coplanar face element by a double click. You can also use the same procedure, but with one single click, to select each face element independently.

3. In the folder 'Surfaces' of the 'Data' tree, **Select** all highlighted face elements, and **drag/drop** them to the 'Surfaces' element of the fiiting zone. All face elements are then duplicated in this folder.

.. note::

In this example, all face elements were already identified in a given folder 'fitting' of the 'Data' tree, because this volume was build when preparing the 3D geometry. In this case, it was not necessary to follow the step 2 of this procedure. One can directly drag and drop all face elements of the corresponding surface folder.

4. One must also select the face elements of the fitting zone that are located on the ground. **Open** the 'floor' surface group and find the two face elements that correspond to the floor of the fitting zone (**Select** each face element of the group and identify the ones that correspond to the fitting zone). Once the two face elements are identified, **Select** them and drag/drop them to the 'Surfaces' element of the fitting zone.


Define a parallelipipedic volume as a fitting zone
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



Define surface materials
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Insert a lne of punctual receivers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Define a surface receiver
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SPPS calculation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Compare two simulations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
