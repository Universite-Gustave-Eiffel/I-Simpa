'Scene' tab
-----------

In the 'Data' tree of the 'Project' window, user define all needed information, in a acoustic point of view, in order to realize an acoustical calculation. This means, surface material, sound sources, receivers (punctual and plane), as well as other needed data in function of the numerical code (for example, the SPPS code can also take into account fitting zones).

In the 'Project' tree of the 'Project' window, user define all general information (environmental data, configuration, information…) and common database (material and spectrum).

.. warning::
	All data defined in the 'Scene' tree depend on the numerical code that will be used in the tab 'Calculation' (*i.e.* depending on the calculation code, it is not necessary to define all elements. See the user guide/manual of the corresponding code for more information).

.. tip::
	For data that depend on frequency, users need to define only the data for the frequency bands of interest, *i.e.* the frequency bands that will be used for the calculation.

'Data' tree
~~~~~~~~~~~~~~~~~~~~~~
	
Fitting zone
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The element 'Fitting zone' allows to define one or more volumes of the geometry that contain scattering objects, producing acoustics diffusion. This phenomenon can have an important effect on the sound level distribution, as well as on other acoustical parameters (like reverberation time), in rooms such as in industrial hall, with many objects on the floor.

Most of time, the diffusion process is modelled using the concept of mean free path (noted Lambda or λ [m]) with a statistical approach defined by a diffusion law). On the same time, acoustical absorption (noted Alpha or α, between 0 and 1) can also occurred on the scattering object.

Within I-Simpa, the diffusion in a fitting zone, can be defined using two methods (right click on the folder 'Fitting zone' and Select the option in the contextual menu):

	- by creating a rectangular volume in the 3D scene ('Define rectangular fitted zone') and by setting the diffusion parameters in the corresponding zone;

	- by detecting one or more volumes in the 3D scene ('Define scene fitted zone') and by setting the diffusion parameters in the corresponding zones. This can also be done handly, by selecting all the face elements around a given volume and a point inside the volume.

**Right click** on the root 'Fitting zone' folder to define a fitting zone:

- 'Define scene fitted zone'
	Defines a fitted zone from the face elements of the 3D scene.
	
	.. warning::	
		The volume must be a closed space.
	
	* 'Surfaces'
		Contains the scene surfaces that define the fitting zone.
			
			+ 'Surface area'
				|surfacearea|.
		
		The selection of faces can be made:
		
			+ by using the 3D view. |selectsurface|
			+ by drag & drop of all surfaces of a 'Volume' element;
			+ by a direct conversion of a 'Volume' element to a fitting zone (see section `Volumes`_).			
	
	* 'Properties'
		|properties|.
			
			+ 'Active fitting zone'
				(Un)check to (Des)activate the fitting zone.
			
			.. note:: Depending of the parameters of the selected calculation code, all the fitting zones can also be desactivated at the calculation step.
			
			+ 'Description'
				|description|.	
			
	* 'Inside position'
		Defines a point of coordinates (x,y,z) inside the volume. |defineposition|.
	
	* 'Display'
		|displayparameter|
		
			+ 'Color':
				Defines the color of the fitting zone.
			
			+ 'Show name':
				Check to display the name of the fitting zone on the 3D view.
	
	* 'Acoustics parameters'
		Defines the acoustics parameters of the fitting zone, by frequency band.
		
			+ 'Alpha'
				:term:`Absorption coefficient` of the scatterers.
				
			+ 'Lambda'
				:term:`Mean free path` (unit: m).
				
			+ 'Diffusion law'
				Reflection law.

- 'Define rectangular fited zone'
	Defines a parallelepipedic fitted zone.
	
	* 'Acoustics parameters'
		Similar as for 'Define scene fitted zone'.
	
	* 'Display'
		
		+ 'Color'
			Defines color of the fitting zone in the 3D view.
		
		+ 'Opacity'
			Defines opacity of the fitting zone in the 3D view.
		
		+ 'Render mode'
			In the 3D view, the fitting zone can be represented by a zone with face ('Volume') or a contour ('Borders').
		
		+ 'Show name'
			Check to display the name of the fitting zone on the 3D view.
		
	* 'Properties'
		Similar as for 'Define scene fitted zone'.

	* 'Origin'
		Defines the (x,y,z) coordinates of the first point of the rectangular zone. |defineposition|.
	
	* 'Destination'
		Defines the (x,y,z) coordinates of the the point opposite the 'Origin' point of the rectangular zone. |defineposition|.
			

Punctual receivers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Several acoustic indicators must be associated to punctual receivers in the volume of propagation. Besides their position and direction of view, it may also be necessary to define other acoustic properties, such as directivity, background noise... The receivers can be grouped within source groups. A group can contain other groups.

**Right click** on the root folder 'Punctual receivers' to create new receivers or groups of receivers

- 'New receiver'
	Creates a new receiver.
	
- 'New Group'
	Creates a new group of receivers.
	
- 'Create a receiver grid'
	Creates a grid of receivers in a new group.
	
	* 'Number of receivers (column)'
		Number of receivers along a column.
		
	* 'Number of receivers (row)'
		Number of receivers along a row.
	
	* 'Column vector'
		Defines the (x,y,z) coordinates of the vector along the column.
		
	* 'Row vector'
		Defines the (x,y,z) coordinates of the vector along the row.
		
	* 'Grid origin'
		Defines the (x,y,z) coordinates of the grid origin.
	
- 'Rotation'
	Rotate all receivers in a given group.
	
	* 'Angle (degree)'
		Defines the rotation angle in degree.
	
	* 'Rotation center'
		Defines the (x,y,z) coordinates of the point of rotation.
	
	* 'Rotation vector'
		Defines the (x,y,z) coordinates of the vector of rotation.
	
- 'Translation'
	Translate all receivers in a given group.
	
	* 'Translation vector'
		Defines the (x,y,z) coordinates of the vector of translation.
	
- 'Orientation'
	Orient all receivers in a given group to a target point.
	
	* 'Target point'
		Defines the (x,y,z) coordinates of the target point.
	
Receiver parameters:

- 'Background noise'
	Defines the existing background noise at the receiver location. This parameters can be useful, for example, to calculate some acoustics indicator for speech intelligibility. This parameter is defined by a background noise 'Spectrum'. |usingspectrum|.

- 'Direction'
	Defines the orientation of the receivers, using a vector of coordinates (x,y,z). This parameters can be useful, for example, to associate a directivity to the receiver. |defineposition|. The receiver can also be automatically oriented to a given sound source with a dynamic link: choose the corresonding sound source in the list.
	
- 'Display'
	|displayparameter|.
	
		+ 'Color':
				Defines the color of the receiver.
			
		+ 'Show name':
				Check to display the name of the receiver on the 3D view.
	
- 'Position'
	Defines the (x,y,z) coordinate of the receiver. |defineposition|.
	
- 'Properties'
	|properties|.
					
			+ 'Description'
				|description|.	
			
			+ 'Direction x'
				Coordinate x of the orientation vector (unit vector).
			
			+ 'Direction y'
				Coordinate y of the orientation vector (unit vector).
				
			+ 'Direction z'
				Coordinate z of the orientation vector (unit vector).
				
			+ 'Directivity'
				Directivity of the receiver. See the Directivity_ section for details.
	
	
Sound sources
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This element allows to define the acoustic properties of sound sources, such as the acoustic power, the directivity, the position of the source and its orientation. The sound sources can be grouped
within source groups. A group can contain other groups.

**Right click** on the root folder 'Sound source' to create new sound sources or groups of sound sources

- 'New source'
	Creates a new source.
	
- 'New Group'
	Creates a new group of receivers.
	
- 'Create a line of sound source'
	Creates a line of sound source in a new group.
	
- 'All sources'
	Possible actions on a group of sound sources.
	
			+ 'Enable'
				Enable all sound surces in a group.	
			
			+ 'Disable'
				Disable all sound surces in a group.	
			
			+ 'Rotation'
				Same action as for sound sources.
				
			+ 'Translation'
				Same action as for sound sources.

Sound source parameters:

- 'Display'
	|displayparameter|.
	
- 'Position'
	Defines the (x,y,z) coordinate of the sound source. |defineposition|.
	
- 'Properties'
	|properties|.
					
			+ 'Active source'
				(Un)check to (Des)activate the sound source.

			+ 'Description'
				|description|.				
			
			+ 'Direction x'
				Coordinate x of the orientation vector (unit vector).
			
			+ 'Direction y'
				Coordinate y of the orientation vector (unit vector).
				
			+ 'Direction z'
				Coordinate z of the orientation vector (unit vector).
				
			+ 'Directivity'
				Selects the directivity of the sound source in a list. Several 'theoretical' directivities are proposed (omnidrectionnal, unidirectionnal, XY, YZ, ZY) as well as 'balloon' directivities (*i.e.* as measured). In this last case, the 'Balloon' parameter must be given.
			
			+ 'Directivity Balloon'
				Selects the directivity balloon of the sound source in a list. This parameter is enable only if the 'Balloon' option is selected in the 'Directivity' parameter.
				
			+ 'Time delay'
				Defines a delay (in second) for activating the sound source.			

- 'Sound power'
	Defines the sound power of the sound source. This parameter is defined by a power 'Spectrum'. |usingspectrum|.
				
Surface receivers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
It is common in a acoustics study to focus on the distribution of sound levels on a surface (noise mapping). The concept of mapping can also be generalized to the surface representation of any acoustic parameter, like reverberation time, clarity... The I-Simpa interface enables to define surfaces for the representation of maps.

By right clicking on the root 'Surface receiver' folder, it allows define two types of surface receiver:

- 'New scene receiver'
	Surface elements of the 3D scene can be selected.
	
	+ 'Surfaces'
		Defines the surface element of the 3D scene to be considered for the surface receiver. |selectsurface|.
		
		+ 'Surface area'
			|surfacearea|.

	+ 'Properties'
		|properties|.			
			
		* 'Description'
			|description|.
	

- 'New plan receiver'
	The surface receiver is defined by a grid cutting plane, made of 3 vertices A, B and C.
	
	+ 'Display'
		|displayparameter|.

		* 'Grid color'
			Defines the color for displaying the grid.			
			
		* 'Show grid'
			Check to display the grid on the 3D view.
		
		* 'Show vertice names'
			Check to display the name of the vertices A, B, C.
			
	+ 'Properties'
		|properties|.

		* 'Description'
			|description|.

		* 'Enable'
			Check to enable the plan receiver in the calculation.
		
		* 'Grid resolution'
			Defines the resolution of the grid (in m).
			
	+ 'Vertex A'
			Defines the (x,y,z) coordinates of the vertex A. |defineposition|.	

	+ 'Vertex B'
			Defines the (x,y,z) coordinates of the vertex B. |defineposition|.
			
	+ 'Vertex C'
			Defines the (x,y,z) coordinates of the vertex C. |defineposition|.
			
Surfaces
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
This element contains all surfaces of the scene. These surfaces are either created within I-Simpa in the case of a the creation of a parallelepipedic geometry or imported. In the last case, depending on the software used to create the 3D scene, it is possible to import faces that are already organized into groups. This group organisation allows to easily affect specific material (with specific acoustic properties) for each surface group.

**Right click** on the root folder 'Surfaces' to organize the face elements:

- 'Add a group'
	Creates a new group.
	
**Right click** on a group to perform actions:

- 'Inverse face normal'
	Change the orientation of the normal of all face elements within the group. This can useful if, at the importation of 3D scene, some original faces have a wrong orientation.

- 'Properties'
	|properties|
	
	+ 'Material'
		Select the material to affect to the surfaces group, from a list. The list is defined in the 'Material database' of the 'Project tree'.
		
	+ 'Surface area'
		|surfacearea|.	

Volumes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The volume element allows to define volumes of interest in the 3D scene. You can 'handly' create a volume from face elements in the 3D scene or automatically detect all existing volume in the 3D scene. This can useful to create or detect fitting zones.

**Right click** on the root folder 'Volumes' to manage 'Volumes':

- 'Create a volume'
	Creates a volume from the face elements of the 3D scene.
	
	+ 'Display'
		|displayparameter|.

		* 'Domain color'
			Defines the color for displaying the volume.			
			
	+ 'Inside position'
		Defines a point of coordinates (x,y,z) inside the volume. |defineposition|.
		
	+ 'Properties'
		|properties|
	
		+ 'Calculate the mean free path'
			Calculate the :term:`mean free path` using the classical formulae from statistical room acoustics (λ=4V/S, with V the volume and S the surface of the volume).
		
		+ 'Mean free path'
			Value (in m) of the calculated mean free path.	

- 'Volume auto-detect'
	Automatically detect all volumes within the 3D scene. Same paremeters as for 'Create a volume'.

'Project' tree
~~~~~~~~~~~~~~~~~~~~~~
	
Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This element of the 'Project' tree defines some useful information concerning the project. These element are not used for the calculation.

Property elements: these elements are all optional.

- 'Author'
	Name or information concerning the author of the project.

- 'Date'
	Date of the project. By default, it is the date of creation of the project (text).

- 'Project description'
	Quick description of the project.
	
- 'Project name'
	Name of the project, given by the author.

Display
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
This element of the 'Project' tree defines some useful parameter for the 3D display.

Property elements:

- 'XYZ axis'
	Define some parameters for the graphical representation of the geometric coordinates.
	
- 'Arrow color (x)'
	Open a dialog box for selecting the color of the arrow of the x-axis.
	
- 'Arrow color (y)'
	Open a dialog box for selecting the color of the arrow of the y-axis.
	
- 'Arrow color (z)'
	Open a dialog box for selecting the color of the arrow of the z-axis.
	
- 'Arrow length'
	Length (in m) of the arrow of the xyz-axis.
	
- 'Arrow widh'
	Width (in m) of the arrow of the xyz-axis.
	
- 'Grid size'
	Size (in m) of a grid element.
	
- 'Grid color'
	Open a dialog box for selecting the color of the XYZ grid.
	
- 'XY Grid'
	Enable/disbale the display of the XY grid.
	
- 'XZ Grid'
	Enable/disbale the display of the XZ grid.
	
- 'YZ Grid'
	Enable/disbale the display of the YZ grid.

Environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This element of the 'Project' tree defines some useful environment data.

.. warning:: The use of these information depend on the calculation code. See the reference guide of the numerical code for more details. The code can enable the calculation of the atmospheric absorption and of the meteorological effect (considerig a log_lin celerity gradient) with a ground description using the roughness parameters.

Property elements:

- 'Atmospheric absorption - User defined'
	Enable/disable the selection of the value of the atmospheric absorption.
	
- 'Atmospheric absorption - Value'
	User value of the atmospehic absorption. This option is available only if the option 'Atmospheric absorption - User defined' is enable.
	
- 'Atmospheric pressure (Pa)'
	Atmospheric pressure(Pa).
	
- 'Meteorolical effect - Celerity gradient a_log'
	Value of the a_log paremeter of the log_lin profile celerity. The default value is function of the option given by the 'Meteorolical effect - Profile', but can be modified by the user.
	
- 'Meteorolical effect - Celerity gradient b_lin'
	Value of the b_lin paremeter of the log_lin profile celerity. The default value is function of the option given by the 'Meteorolical effect - Profile', but can be modified by the user.
	
- 'Meteorolical effect - Profile'
	Defines a log_lin gradient profile. By selecting one of the following options, it sets the 'Meteorolical effect - Celerity gradient a_log' and 'Meteorolical effect - Celerity gradient b_lin' parameters. However, these values can be modified by the user.
	
		+ 'Very favorable'
			a_log=+1, b_lin=+0.12
			
		+ 'Favorable'
			a_log=+0.4, b_lin=+0.04
			
		+ 'Homogenous':
			a_log=0, b_lin=0
		
		+ 'Unfavorable':
			a_log=-0.4, b_lin=-0.04
		
		+ 'Veryfavorable':
			a_log=-1, b_lin=-0.12		
		
- 'Relative humidity (%)'
	Relative humidity of air (%).
	
- 'Roughness - z0 (m)'
	Value of the ground roughness (m).
	
- 'Roughness - Ground type (m)'
	Defines the ground type. By selecting one of the following options, it sets the 'Roughness - z0 (m)' parameter. However, this value can be modified by the user.
	
		+ 'Water'
			z0=0.006
		
		+ 'Ground'
			z0=0.02
		
		+ 'Short lawn'
			z0=0.001
		
		+ 'Dense lawn'
			z0=0.02
		
		+ 'Wheat (1m height)'
			z0=0.16
		
		+ 'Sparse habitat (farm, trees, hedges)'
			z0=0.6
		
		+ 'Low concentration Suburb (residential areas, gardens)'
			z0=1.2
		
		+ 'Dense urban'
			z0=10
		
		+ 'Dense suburb'
			z0=1.8
		

- 'Temperature (°C)'
	Temperature of air (°C).

Informations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
This element of the 'Project' tree gives some useful information concerning the I-Simpa project.

Property elements:

- 'Model face count'
	Number of face elements.

- 'Number of active fitting zones'
	Number of active fitting zones.

- 'Number of active sound sources'
	Number of active sound sources.

- 'Number of fitting zones'
	Number of fitting zones.

- 'Number of punctual receivers'
	Number of punctual receivers.

- 'Number of sound sources'
	Number of sound sources.

- 'Number of surface groups'
	Number of surface groups.

- 'Scene surface (m2)'
	Scene surface (m²).

- 'Scene volume (m3)'
	Scene volume (m3).

Project database
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

See a detail descritpion of the `Project database`_ 



.. _`Using spectrum`: using_spectrum.html
.. _Directivity: using_directivity.html
.. _`Defines position`: define_position.html
.. _`Surface selection`: surface_selection.html
.. _`Project database`: project_database.html

.. |usingspectrum| replace:: See the section `Using spectrum`_ for understanding how to use a spectrum in the application
.. |defineposition| replace:: The position can be defined by filling the form or using the pointer on the 3D view. See the section `Defines position`_ for understanding how to define a location with the 3D view
.. |selectsurface| replace:: See the section `Surface selection`_ for understanding how to select surface elements
.. |displayparameter| replace:: Defines the parameters for displaying the element on the 3D view
.. |properties| replace:: Defines the properties of the element
.. |description| replace:: Gives a short text description of the element
.. |surfacearea| replace:: Total surface (in m²) of all face elements in the folder