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

- **'Rotation angle (degree)'**\ =45°,
-  at center position **'Rotation center'**\ =[1.,1.,0.],
- around axis**'Rotation vector'**\ =[0.,0.,1.] :



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

-  **'Step (line) [x,y,z]'**: define the space steps (vectors) according
   the line direction;
-  **'Step (row) [x,y,z]'**: define the space steps (vectors) according
   the row direction;
-  **'Number of lines'**: define the number of lines (integer)
-  **'Number of rows'**: define the number of rows (integer);
-  **'Start position (x,y,z)'**: define the starting position (x,y,z) of
   the grid.

Examples:

   - Example 1:

       - **'Step (line)'**\ =[0,1,0],
       - **'Step (row)'**\ =[1,0,0],
       - **'Number of lines'**\ =4,
       -**'Number of rows'**\ =5;

   - Example 2:

      - **'Step (line)'**\ =[0,1.5,0],
      - **'Step (row)'**\ =[0.5,0.5,0],
      - **'Number of lines'**\ =4,
      - **'Number of rows'**\ =5.
