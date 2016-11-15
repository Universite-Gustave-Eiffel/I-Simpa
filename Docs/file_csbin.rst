Surface receiver file file `.csbin`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A `.csbin` file contains the temporal evolution of a quantity that represents an acoustic intensity on a surface.

Depending of the calculation parameters, one can obtain, for each surface receiver, by default, on **Global** value (in the 'Global' folder) and, in addition, the result for each frequency band in a corresponding folder.

A right click on a surface receiver file opens the contextual menu, with specific actions:

- 'Acoustic parameters'
	Allows to compute relevant room acoustics parameters on the surface. Depending of the selection, it creates specific files on the corresponding folder.
	
		- Clarity C (in dB)
		- Definition D (in %)
		- Central Time Ts (in ms)
		- Reverberation time RT (in s)
		- Early decay time EDT (in s)
		- Stage Support ST (in dB)

- 'Load animation'
	It allows to represent the spatial variation of the indicators that is selected. If the indicators contains some time dependent value, it can displayed an animation. On can interact on the animation with the `'Simulation' toolbar`_.

		- 'Instantaneous value'
			Show the value of the given indicator at each time step.
		
		- 'Cumulative instantaneous value'
			Show the value of the given indicator at each time step, by cumulating all past steps.
		
		- 'Total value'
			Show the total value of the given indicator. No animation.
	
.. _`'Simulation' toolbar`: toolbar_simulation.html