Manipulating sources and receivers
-------------------------------------

Rotate a group of sources/receivers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I-Simpa can automatically rotate a group of punctual receivers or sound
sources. Users have to specify:

-  **'Rotation angle (degree)'**: angle of rotation [degree];
-  **'Rotation center'**: center of the rotation (position (x,y,z) in
   [m]);
-  **'Rotation vector'**: axis of rotation (x,y,z) in [m].

Example: Below, an example of rotation of a receivers grid in a plane XY:

- **'Rotation angle (degree)'**\ =45°
- at center position **'Rotation center'**\ =[1.,1.,0.]
- around axis **'Rotation vector'**\ =[0.,0.,1.]



Translate a group of sources/receivers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I-Simpa can automatically translate of punctual receivers or sound
sources. Users have to specify:


-  **'Direction (m)'**: define the direction (vector) of translation.

Example: Below, an example of a translation of a receivers grid, in the direction [3.,0.,0.]:


Create a grid of receivers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I-Simpa can creates automatically 2D grids of punctual receivers. Users
have to specify:

- a column (col) vector and a row vector to define the grid plane, as well as, the spatial step between two receivers
- the starting position (origin) of the grid,
- and the number of rows and columns of the grid.

Grid Parameters:

-  **'Col step x (m)'**: define the x coordinate of the column vector;
-  **'Col step y (m)'**: define the y coordinate of the column vector;
-  **'Col step z (m)'**: define the z coordinate of the column vector;
-  **'Number of cols'**: define the number of columns;
-  **'Number of rows'**: define the number of rows;
-  **'Row step x (m)'**: define the x coordinate of the row vector;
-  **'Row step y (m)'**: define the y coordinate of the row vector;
-  **'Row step z (m)'**: define the z coordinate of the row vector;
-  **'Starting position x (m)'**: define the x coordinate of the origin of the grid;
-  **'Starting position y (m)'**: define the y coordinate of the origin of the grid;
-  **'Starting position z (m)'**: define the z coordinate of the origin of the grid;
