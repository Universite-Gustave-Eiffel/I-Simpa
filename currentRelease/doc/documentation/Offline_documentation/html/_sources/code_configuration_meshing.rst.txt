Meshing
^^^^^^^^^^^^^^^^^^

Some calculation codes may require a meshing of the domain (SPPS for example). I-Simpa used the `TetGen meshing code (A Quality Tetrahedral Mesh Generator and a 3D Delaunay Triangulator)`_. See the documentation for more information.

Within I-Simpa, TetGen can be paramatrized:

- 'debugmode'
	Check/uncheck for activate the debug mode of TetGen;

	.. warning::

		With the debug mode, the meshing is not realized.

-  'minratio'
		Defines the ratio between the radius of the sphere that would contain a mesh and the lenght of the mesh. This parameter could be useful in order to avoid long meshes.
		
-  'userdefineparams'
		Defines new parameters by replacing default parameters.
		
-  'appendparams'
		Add parameters to the dafault parameters.
		
-  'preprocess'
		Check/uncheck for trying to repair the 3D scene if possible.

-  'maxvol'
		Maximun accepted value of a mesh volume (m3). The value can be modified only if the 'ismaxvol' parameter is check.
		
-  'ismaxvol'
		Check/uncheck for using a given maximum value of the mesh volume 'maxvol'.
		
-  'constraintrecepteurss'
		Maximun accepted value of the mesh surface (m2) for a surface receiver. The value can be modified only if the 'isareaconstraint' parameter is checked.
		
-  'isareaconstraint'.
		Check/uncheck for using a given value of the mesh surface 'constraintrecepteurss'.
		
.. _`TetGen meshing code (A Quality Tetrahedral Mesh Generator and a 3D Delaunay Triangulator)`: http://wias-berlin.de/software/tetgen/