Import options
"""""""""""""""""""

'Average model remesh'
----------------------

Allow to build an approximate model from the 3D scene which is imported.

Indeed, due to the sensitivity of some numerical codes within I-Simpa in
regards with the 'quality' of the 3D geometries, it may be possible that
some calculation codes will not be functional. A possibility that is
offers by I-Simpa, is to use the 'original' 3D geometry to create an
approximate 3D model, with a 'quality' in accordance with the
calculation code.

.. note::
	Please note, that research works on the sound field modelling in room acoustics, have shown that it is preferable to use a simple geometry with a good evaluation of the acousticalproperties of surface materials, instead of a complicated geometry. In other word, if you need to build an approximate geometry, you don't need to choose a high level of approximation.

.. warning:: 
	Depending of the chosen options and the size of the volume, the 'Average model remesh' option can require large computational times.

The realization of the approximate model is based on the marching cube
technique, and will try to propose a model as close as possible from the
original one. When choosing the 'Average model remesh' options, two
dialog boxes will be successively proposed:

Mesh correction options:
~~~~~~~~~~~~~~~~~~~~~~~~

-  'Model solving'
		This option defines the resolution n of the approximation. The volume of the original model is decomposed in 2^n elements in each 3D axis. For example, for n=7, we will have 128x128x128 cubic ceils in the original volume.

-  'Triangulation method'
		This option defines the type of method used for approximating model. There is currently only one possible option 'Marching cube'.
		
-  'Remesh adaptation'
		When cubic ceils are created in the original volume, you can decide to 'deform' the ceils close to the surface of the volume in order to fit as well as possible the delimiting surface   of the original model (option 'Vertices 6translation'). If deformation is required, choose the 'None' option. important The 'Vertices translation' option can require large computational time and can also produced some unwanted effects.
		
-  'Reduction of the number of surfaces'
		This option able to reduce the total number of faces in the approximate model, by evaluating the quality of each face. There is currently only one possible option 'Edge collapse'. This method is based on the size and the shape of each face, reduced to a 'Quality Constraint', noted q, and on the 'Approximation constraint'. See the following book_ for details: PLTMG: A Software Package for Solving Elliptic Partial Differential Equations by Randolph E. Bank. 
   

'Repair model'
----------------------

With this option, I-Simpa try to repair some elements of the 3D model (fusion of vertices, correction of non-planar surfaces...). This will assure a better compatibility with the numerical code within I-Simpa, but is still limited.


'Surface meshing'
----------------------

This option allows to enhance the quality of surface elements, by re-meshing original surfaces. Default parameter are proposed (it seems to offer a good quality), but can be changed. This procedure is based on the TetGen_ mesh generator.

.. warning::

	By using this option, it increase the numbers of face elements. The assignation of materials within I-Simpa will be more time consumer. This option should be used only if the mesh generation within I-Simpa can not be realized due to a poor quality of surface elements.


'Keep existing group'
----------------------

This option should be used only when importing a new 3D geometry in the current project.

This allows to associate *new* faces of the new geometry to *old* surface groups. The method compares the location of the *new* surface elements from the *old* one, in regard with a given precision ('Association maximum distance').

.. tip::

	This can be useful, if users want to load a modified geometry (with minor changes from the previous one) without re-affecting all surface materials within I-Simpa


.. _TetGen: http://tetgen.berlios.de
.. _book: http://www.google.fr/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=0CFIQFjAC&url=http%3A%2F%2Fbooks.google.com%2Fbooks%2Fabout%2FPLTMG\_A\_Software\_Package\_for\_Solving\_Ell.html%3Fid%3D6C-RaZVo5TgC&ei=3OJLUum7Corb0QX9jICIAw&usg=AFQjCNG6ebGhEXo7pLGyfI6ApR9
