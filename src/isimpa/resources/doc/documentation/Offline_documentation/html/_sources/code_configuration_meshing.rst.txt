Meshing
^^^^^^^^^^^^^^^^^^

Some calculation codes may require a meshing of the domain (SPPS for example). I-Simpa used the `TetGen meshing code (A Quality Tetrahedral Mesh Generator and a 3D Delaunay Triangulator)`_. See the documentation for more information.

Within I-Simpa, TetGen can be paramatrized:

- 'Test mesh topology'
	Check/uncheck for activate the debug mode of TetGen (debug mode).

	.. warning::

		With the debug mode, the meshing is not realized.

-  'Additional parameters'
		Add parameters to the TetGen default parameters.

-  'Radius/Edge ratio'
		Defines the ratio between the radius of the sphere that would contain a mesh and the lenght of the mesh. This parameter could be useful in order to avoid long meshes.

-  'Scene correction before meshing'
		Check/uncheck for trying to repair the 3D scene if possible.

-  'User-defined paramters'
		Defines new parameters by replacing default parameters.

-  'Surface receiver constraint'.
		Check/uncheck for using a given value of the mesh surface 'Surface receiver constraint (m²)'.

-  'Surface receiver constraint (m²)'
		Maximun accepted value of the mesh surface (m²) for a surface receiver. The value can be modified only if the 'Surface receiver constraint' parameter is checked.

-  'Volume constraint'
		Check/uncheck for using a given maximum value of the mesh volume 'Volume constraint (m3)'.

-  'Volume constraint (m3)'
		Maximun accepted value of a mesh volume (m3). The value can be modified only if the 'Volume constraint' parameter is checked.


.. _`TetGen meshing code (A Quality Tetrahedral Mesh Generator and a 3D Delaunay Triangulator)`: http://wias-berlin.de/software/tetgen/
