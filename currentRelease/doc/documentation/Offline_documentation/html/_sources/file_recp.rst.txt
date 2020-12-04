Punctual receiver file `.recp`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This file contains the temporal evolution, by frequency band, of a given quantity that is homogeneous to instantaneous sound squared pressure (:math:`p^2`). Most of time, it should correspond to a punctual eceiver at the point. Typically, this type of file can
represent an impulse response (RI). The actions associated with I-Simpa arise directly from the file format.

.. important::
	The operations performed by the interface I-Simpa with this file type make sense only if the data in the file are equivalent to an instantaneous squared sound pressure (:math:`p^2`).

Data Display
^^^^^^^^^^^^^^^^^^^

Double left click on a `.recp` file open a new winodw with three tabs

-  'Sound Level SPL (dB)' tab
		It contains the temporal evolution of the quantity, for each frequency band, as a data table.

			- This tab is opened by default.
			- The values are represented in sound pressure levels (SPL).
			- The **Global** value (*i.e.* the sum of all frequency bands) is calculated and displayed at the bottom of each column.
			- The **Total** value (*i.e.* the sum of all time step) is calculated and displayed at the end of each row.

- 'Sound Level SPL (dB)' tab
		It provides a graphic display of the **Global** value.

			- The temporal evolution of the **Global** value is displayed, as an echogram.
			- The cumulative quantity of the **Global** value is displayed, according to the Schroeder's backward integration :cite:`Schroeder_1965`.

- 'Spectrum' tab
		It displays a spectrum at the punctual receiver.

Room acoustics parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Given the nature of the file (*i.e.* an echogram), the contextual menu that is associated to this file allows to calculate several room acoustic parameters:

- Sound Pressure Level (SPL) in dB
- Sound Pressure Level (SPL) in dB(A)
- Clarity C (in dB)
- Definition D (in %)
- Central Time Ts (in ms)
- Reverberation time RT (in s)
- Early decay time EDT (in s)
- Stage Support ST (in dB)

Several parameters can be given by the user in order to calculate user-values of some room acoustics parameters:

- Clarity: fix the value of the temporal limit of integration, usually 50 ms
- Definition: fix the value of the temporal limit of integration, usually 50 ms
- Reverberation time: fix the value of the sound level limit of integration, usually 30dB

.. tip::
	Multiple calculations are allowed for each paremeter, by using the semicolon ';' between parameters.

After the calculation parameters, two files are created in the corresponding folder:

- 'Acoustic parameters'
	This file provides access to the room acoustics parameters in the form of a data table.

		- The parameters are given for each frequency band of interest.
		- When allowed, the **Global** value (*i.e.* the sum of all frequency bands) is calculated and displayed at the bottom of each column.
		- When allowed, the **Average** value (*i.e.* the mean value on all frequency bands) is calculated and displayed at the bottom of each column.


- 'Schroeder curves'
	It displays the temporal evolution of the Schroeder's curves :cite:`Schroeder_1965`, for each frequency band, as a data table.

.. tip::
	User can create charts for representing data from the data table. See `charts creation`_

.. _`charts creation`: create_charts.html
