Recommendations to import a 3D scene
-------------------------------------

I-Simpa is very sensitive to the quality of 3D files. It will not accept
a ‘bad’ geometry, with faces intersections, holes between faces… (in
fact, the model will be imported, but some codes like SPPS will not be
functional).

It is recommended that users try to export their model in I-Simpa during
the different steps of the model building:

1. **Start to built your 3D model** with your CAD software (the main close surface of the model);
2. **Export the model** in a 3DS file with your CAD software;
3. **Import the 3D model** within I-Simpa;
4. **Generate a meshing** by clicking on the ‘Meshing’ button on the icon toolbar: this will verify that your model is correctly built;
5. if user can see the mesh inside the model, it means that the model is correct for I-Simpa and that the calculation with SPPS will be functional;
6. if the model is not correct, return to the CAD software and try to see what is the problem (the problem is due to something in the last step of the built of the 3D model);
7. if the model is correct, return to your CAD software (step 0) and continue your 3D modelling, by adding objects, faces… Then, continue using the same procedure.

.. note::
    At the importation step, I-Simpa can correct some errors. During importation check the ‘Repair model’ `Import options`_ (if not already done).

.. note::
    If you still have trouble during importation, you can also create an approximate model by using the `Import options`_ ‘Average model remesh’. It will create a simple (or simplified) model.

.. tip::
    Note that, when creating a 3D model for acoustic calculations, it is not necessary to create a very complex 3D model with too many faces. You will have better results with the simple shape of your 3D model, and by adjusting correctly all acoustical characteristics of the material on the face. Make simple.

.. _Import options: import_options.html


Use TetGen to identify problemns in 3D models
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There is a way inside I-Simpa to identify problems in 3D geometries, after imprting the 3D scene.

1. Import your 3D model in I-simpa
2. Go to the 'Calculation' tab of the `'Project' window`_
3. Got to 'Mesh' `'Properties' window`_ of the SPPS code
4. Check the 'Test mesh topology' option (see `SPPS meshing options`_)
5. Run the SPPS calculations
6. Faces in error will be colorized in the  `'Main' window`_. You can find information about indexes and coordinates in the `'Console' window`_

.. figure:: images/addition_illustrations/see_errors_meshing.png
   :align: center
   :width: 400px
   :alt: Displaying meshing errors within I-Simpa console

   Displaying meshing errors within I-Simpa `'Console' window`_


.. _`'Project' window`: main_windows.html
.. _`'Main' window`: project_windows.html
.. _`'Properties' window`: properties_window.html
.. _`SPPS meshing options`: code_configuration_SPPS.html#meshing
.. _`'Console' window`: console_window.html
