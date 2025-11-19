.. warning:: Paid attention to the limitations of this validation. `Goto this page`_

.. _`Goto this page`: validation_notice.html


Clarity calculation implementation validation
-------------------------------------------------------

Presentation
#######################

This section proposes a validation of the implementation of Clarity calculation room parameter `C80`_. Several room configurations are considered, with several source-receiver distances and time step (for SPPS calculation). Since numerical results are compared to theoretical results when considering diffuse sound field, room walls are totally diffusing (sctarring coefficient equal to 1)

.. _`SPPS`: ../code_SPPS.html
.. _`C80`: ../room_acoustics_parameters.html#clarity-c

Theoretical model
^^^^^^^^^^^^^^^^^^^^^^^^

Using the diffuse field theory of Barron :cite:`barron1988`, the clarity room acoustics parameters C80, can be estimated by the following relation:

.. math::

   C_{80} = 10 \log \left(  \frac{E_\text{direct}+E_\text{early}}{E_\text{late}}\right)

where the direct field is given by

.. math::

   E_\text{direct}=\frac{100}{R^2},

and the early and late field by

.. math::

   E_\text{early}=\left( \frac{31200\times RT}{V} \right) \times \exp\left( -\frac{0.04}{RT}\right) \times \left( 1- \exp \left( -\frac{1.11}{RT}\right) \right)

.. math::

      E_\text{late}=\left( \frac{31200\times RT}{V} \right) \times \exp\left( -\frac{0.04}{RT}\right) \times \exp \left( -\frac{1.11}{RT}\right).

where :math:`RT` is the reverberation time, :math:`V` the volume and :math:`R` the distance between the source and the receiver.

.. _`TCR`: ../code_TCR.html

Geometry
^^^^^^^^^^^^^^^^^^^^^^^^

- Cubic and quasi cubic rooms: 5m x 5m x 5m (125m³), 7m x 8m x 9m (504 m³), 10m x 10m x 10m (1000 m³), 20m x 20m x 20m (8000 m³) and 20m x 20m x 30m (1200 m³)
- Uniform wall diffuse reflection (100%)
- 0 to 75% uniform surface absorption (in the ``.proj`` file absorption coefficient values are changed for each frequency band between 50 and 1250Hz in order to compute several room absorption configuration at once)
- Punctual receiver at location at 2m or 8m from the source
- No atmospheric absorption


References
^^^^^^^^^^^^^^^^^^^^^^^^

ISO 3382-1:2009, Acoustics -- Measurement of room acoustic parameters -- Part 1: Performance spaces
http://www.iso.org/iso/iso_catalogue/catalogue_tc/catalogue_detail.htm?csnumber=40979

Files and data
^^^^^^^^^^^^^^^^^^^^^^^^

Files/data are located in the following folder:

::

	<I-Simpa installation folder>\doc\validation\clarity_calculation

- ``Clarity_room_125_m3.proj``:  I-Simpa project file
- ``Clarity_room_504_m3.proj``:  I-Simpa project file
- ``Clarity_room_1000_m3.proj``:  I-Simpa project file
- ``Clarity_room_8000_m3.proj``:  I-Simpa project file
- ``Clarity_room_12000_m3.proj``:  I-Simpa project file
- ``Clarity_validation.xlsx``:  Results file


Results
#######################

What is expected?
^^^^^^^^^^^^^^^^^^^^^^^^

- An agreement between the theoretical results fo Clarity C80 and the one calculated with I-Simpa using data from SPPS code (assuming of course that the SPPS code is validated for such configuration).


Results
^^^^^^^^^^^^^^^^^^^^^^^^

The two figures below show a comparison between the numerical results and the theoretical ones, when condering the reverberation time :math:`RT` as the theoretical one (using Eyring formula) or the numerical one (using the RT30 calculated with I-Simpa at the receiver position, i.e. a 'hybrid' calculation).

.. _`TCR`: ../code_TCR.html

In order to evaluate the effect of receiver position, an additional calculation is performed at a distance of 8m of the source (2m for the other calculation). Since the time step (calculation parameter of SPPS code) could have an impact, an additional comparison is also given for a time step of 5ms (10ms for the other).

It is quite difficult to analyse the results individually (_i.e._ for each room, for each surface absorption...). Regarding the whole results, the calculation of the clarity C80 room acoustics parameter seems quite in agreement with the expected results. The mean error is around 20%, most of time, depending of the geometry.

Additional comparisons, with measurements seem necessary, to go deeper in the validation.

.. figure:: ../images/Validation/clarity_calculation_comparison.png
   :align: center
   :width: 500px

.. figure:: ../images/Validation/clarity_calculation_comparison_hyb.png
   :align: center
   :width: 500px
