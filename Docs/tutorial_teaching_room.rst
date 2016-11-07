Study of teaching room
------------------------

This tutorial concerns the simple study of a teaching room. The geometry is rectangular of size (6m x 10m x 3m). The goal of this tutorial is to manipulate:

-  face elements
-  surface material
-  punctual receivers
-  omnidirectional sound sources
-  calculation with the TCR and SPPS codes
-  simple representation of results

Scene creation
~~~~~~~~~~~~~~~~

Creates a new geometry
^^^^^^^^^^^^^^^^^^^^^^

The first step is to create the geometry. As we consider a simple rectangular geometry with the same material of each mean face, the built-in option for creating a rectangular room can be used.

.. note::

	Remember that for more complicate room shapes of for rectangular room with different materials on a same main face, user will have to create the geometry with a CAD software.

1. **Start** I-Simpa
2. When starting I-Simpa, the default project is the last saved project. To clear I-Simpa, **select** 'New project' in the File menu or **click** on the 'New project' icon on the 'File' toolbar to clear I-Simpa
3. In the menu file, **select** 'New scene'
4. **Enter** the given size of the room (width:6m, length: 10m and height: 3m), then **click** on 'OK'
5. The 'Loading 3D scene' windows appears. Leave the default values (you can also deselect all options; this will not change the result) and **click** on 'OK'

Defines surfaces
^^^^^^^^^^^^^^^^^^^^^^

The second step is to affect one material for each main faces. Here, we will consider 3 surface groups: the floor, the walls and the ceiling.

1. **select** the mode 'Face selection' on the toolbar 'Selection'
2. **hold** the CTRL key and **double-click** on the floor in the '3D view':

   -  this will select all co-planar faces (i.e. both triangular faces of the floor);
   -  the 2 corresponding elements will be underlined in the 'Surface' folder of the tree 'Data';

3. **uses** the right click on one of the 2 selected 'Surface' elements: a contextual menu will appear, and **select** 'Send to a new face group'
4. a new surface group, named group, is created (info a new group is created with the default name group. Group surfaces can have the same name, but there is no problem, since each group is identified with its own internal Id)
5. **right click** on the new corresponding group and **select** 'Rename' to rename the group with the name floor (you can also use the left click on the group name to change the name)

Calculation
~~~~~~~~~~~~~~~~

TCR code
^^^^^^^^^^^^^^^^^^^^^^

SPPS code
^^^^^^^^^^^^^^^^^^^^^^

Results
~~~~~~~~~~~~~~~~