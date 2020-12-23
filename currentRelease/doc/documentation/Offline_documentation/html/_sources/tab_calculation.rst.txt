'Calculation' tab
-----------------

This tab allows to choose and to define the acoustic calculation. This tab contains all calculation codes that are linked to I-Simpa. By default, two numerical codes are given:

- `TCR calculation code`_
	Apply the Classical Theory of Reverberation for room acoustics. It applies Sabine and Eyring formulae. This code must be used only for closed spaces.

- `SPPS calculation code`_
	This code is based on a particle tracing method, in respect with the geometrical and energetical hypothesis. This code can be applied in most of situations in room acoustics, as well as in open field, including urban area.

.. important::

	Read the corresponding documentation before using these codes.


.. include:: code_configuration_SPPS.rst

.. include:: code_configuration_TCR.rst

.. include:: code_configuration_run.rst

.. include:: code_configuration_job.rst
