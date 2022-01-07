.. warning:: Paid attention to the limitations of this validation. `Goto this page`_

.. _`Goto this page`: validation_notice.html


Atmospheric absorption implementation verification
-------------------------------------------------------

Presentation
#######################

This section proposes a validation of the implementation of atmospheric absorption in the numerical codes (SPPS, TCR, MD) within I-Simpa, according to ISO 9613-1, mainly in terms of reverberation time.


- `SPPS`_
- `TCR`_
- MD

.. _`SPPS`: ../code_SPPS.html
.. _`TCR`: ../code_TCR.html

Geometry
^^^^^^^^^^^^^^^^^^^^^^^^

- Quasi cubic room (5m x 4m x 3m)
- Uniform wall diffuse reflection (100%)
- 10% uniform surface absorption
- Single source at the room center (100dB per each frequency band)
- Punctual receiver at location (1,1,1)
- Atmospheric conditions: Temperature: 20°C; Humidity:50%


References
^^^^^^^^^^^^^^^^^^^^^^^^

ISO 9613-1:1993, Acoustics -- Attenuation of sound during propagation outdoors -- Part 1: Calculation of the absorption of sound by the atmosphere
http://www.iso.org/iso/catalogue_detail.htm?csnumber=17426

Files and data
^^^^^^^^^^^^^^^^^^^^^^^^

Files/data are located in the following folder:

::

	<I-Simpa installation folder>\doc\validation\atmospheric_absorption

- ``Validation_atmospheric_absorption.proj``:  I-Simpa project file
- ``Validation_atmospheric_absorption.xlsx``:  Results file
- ``Atmospheric_absorption_geometry.png``:  Geometry image file


.. figure:: ../images/Validation/Atmospheric_absorption_geometry.png
   :align: center
   :width: 500px

Results
#######################

What is expected?
^^^^^^^^^^^^^^^^^^^^^^^^

- a decrease of sound level and reverberation time when increasing the frequency du ti the increase of atmospheric absorption coefficient with the Frequency
- an agreement between the TCR (Classical theory of reverberation) and the SPPS/MD codes, since the room is choosen to be as closed as possible with the hypothesis of diffuse sound field.


Results
^^^^^^^^^^^^^^^^^^^^^^^^

The numerical simulations (see figures below) show a very good behavior of the SPPS/MD codes when considering the effect of atmospheric absorption on the reverberation time (RT30) and the sound level (SL). In terms of sound level, the deviation between simulations is around 2 dB, for higher frequencies, and are due to the inherent implementation and hypothesis of each model.

.. figure:: ../images/Validation/Atmospheric_absorption_SL.png
   :align: center
   :width: 500px

.. figure:: ../images/Validation/Atmospheric_absorption_RT.png
   :align: center
   :width: 500px
