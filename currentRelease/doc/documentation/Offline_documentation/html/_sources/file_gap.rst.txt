Punctual receiver file `.gap`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In addition to classical room acoustics parameters, some *advanced* parameters are also calculated, and displayed in the 'Advanced sound level' file. This file display the temporal evolution of the impulse response, for each frequency band, as a data table, weihgted by :math:`cos theta` (LFC and LG) and :math:`cos^2 theta` (LF and LG).

The contextual menu that is associated to this file allows to calculate several room acoustic parameters:

- Early lateral energy fraction LFC (in %)
- Early lateral energy fraction LF (in %)
- Early lateral energy LF (in dB)
- Strength G (in dB)

Data display:

- The **Global** value (*i.e.* the sum of all frequency bands) is calculated and displayed at the bottom of each column.
- The **Total** value (*i.e.* the sum of all time step) is calculated and displayed at the end of each row.

Several parameters can be given by the user in order to calculate user-values of advanced parameters:

- LF: fix the value of the temporal limit of integration, usually 80 ms
- LFC: fix the value of the temporal limit of integration, usually 80 ms

.. tip::
	Multiple calculations are allowed for each paremeter, by using the semicolon ';' bewteen parameters.