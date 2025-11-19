I-Simpa errors and messages
================================

'Tetrahedron file is empty, the calculation can't be done !'
------------------------------------------------------------------

Description
~~~~~~~~~~~~~~~~~~

This errors occurs when the meshing process fails. The meshing process is required for specific numerical code within I-Simpa, particularly using the SPPS code. Most of time, the problem is that the 3D scene that is used for the meshing process, contains several errors such as holes, intersecting faces...

Solution
~~~~~~~~~~~~~~~~~~

The 3D scene must be created and imported with a great attention in order to avoid such problems.
See `our recommandation for creating and importing a 3D scene`_

.. _`our recommandation for creating and importing a 3D scene`: import_file_recommandations.html
