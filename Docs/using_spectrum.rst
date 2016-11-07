Define and use spectrum
""""""""""""""""""""""""""""""""

 .. important::
	It is extremely important that users experiment the manipulation of spectrum before using I-Simpa. The interdependence of sound level in dB and in dB(A), as well as the interdependence of global sound level and sound level for each frequency band, although it is conformed to physical principles, is not obvious.

General information
--------------------

-  Spectrum are defined in the same way for receivers and sound sources
-  Spectrum are defined only within in 'Spectrum' database folder of 'Project database' folder, in the 'Project' tree,
-  the 'Spectrum' database folder contains 2 subfolders, the 'Reference' subfolder and the 'User' subfolder:
-  Spectrum within the 'Reference' subfolders can not be modified. This folder contains only reference built-in spectrum.
-  User can create spectrums in the 'User' subfolder. Spectrum can be modified later if necessary.

Definition
----------

Spectrum creation in the 'Project' tree
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the 'Spectrum' database folder, spectrum are defined simultaneously by a sound level in decibels (dB) or in decibels with the A-weigthed (dB(A)), for each frequency band of interest.

**Very important rules:**

- at the spectrum creation, the global sound level (in dB) is set to 0 dB;
- modifying the global sound level (in dB) updates automatically the ponderate global sound level (in dB(A));
- modifying the ponderate global sound level (in dB(A)) updates automatically the global sound level (in dB);
- modifying the global sound level, either in dB and in dB(A), updates automatically the value for all frequency band, in dB and in dB(A), by applying an uniform ponderation on all frequency band;
- modifying one or more values for a given frequency band, affects automatically the global sound level, both in dB and in dB(A);
- the global sound level, both in dB and in dB(A), are calculated over the 27 frequency bands between 50 and 20000 Hz.

.. warning::
	User can however select only few frequency bands for the calculation. In this case, the user must    set the correct sound level values for each corresponding frequency band. The global sound level that will be calculated by I-Simpa (for source spectrum) will not be the global sound level that should be  obtained over the selected frequency bands. However, in the results data, since the calculation will be been done with the selected frequency bands, the global sound level at a receiver will be correct.

**Examples:**

1. **A first spectrum is created**:

	-  By default, the global sound level is automatically set to 0 dB;
	-  By default, the global sound level is distributed over all frequency bands (27 third octave bands, from 50 Hz to 20 000 Hz), which gives -14.31 dB for each band (*i.e.* -10log(27)=-14.31 dB);
	-  The A-weigthed is automatically calculated for each frequency band, leading a global sound level of -2.38 dB(A).

2. **The global sound level is set to 100 dB**:

	-  The sound level for each frequency band is uniformly modified and sets to 85.69 dB (*i.e.* 100-14.31 dB);
	-  The ponderate sound level in dB(A) is automatically modified, both for each frequency band and for the global sound level.

3. **The ponderate sound level is set to 100 dB(A)**:

	-  The ponderate sound level for each frequency band is modified, according to the pattern of the A ponderation;
	-  The ponderate sound level in dB is automatically modified, both for each frequency band and for the global sound level.

4. **from the last spectrum, the sound level is modified to 50 dB for the third octave band between 500Hz and 2000Hz**, and then the global sound level is set to 80 dB:

	-  All sound level, both in dB and dB(A) are automatically updated.


Using spectrum in the 'Data' tree
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In I-Simpa, spectrum are used either for punctual receivers (element 'Background noise') and for sound sources (element 'Sound power').

-  in the 'Properties' window, **select** the 'Spectrum' in the list (the list contains all 'Reference' and 'User' spectrums)
-  **defines** the total sound power 'Lw' (in dB);
-  if necessary, **add** an 'Attenuation' (in dB) either in global or in frequency band: this attenuation will be applied on the corresponding values of the sound power of the source.

.. tip::
	The 'Attenuation' value can be useful to simulate an attenuation device on the source, without changing the source spectrum, just by adjusting the attenuation in global or in frequency band.

.. warning::
	A global attenuation of 0 dB will apply an uniform attenuation of -14.31 dB for each frequency band. In you do not want to take an attenuation into account, the attenuation must be set to 0 dB for each frequency band.