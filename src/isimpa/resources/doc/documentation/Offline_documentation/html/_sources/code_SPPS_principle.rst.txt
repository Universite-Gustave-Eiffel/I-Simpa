General principle
========================

Sound particle concept
------------------------------

In contrast to classical acoustic theory, where the study of the sound field is based on the propagation of a wave in a continuous material medium, the approach used in the **SPPS** code is geometric, and more particularly particulate: (joyce1974). In a field of complex propagation, the sound field is then decomposed into a multitude of elementary particles, called sound particles or phonons, without mutual interaction and carrying an infinitesimal energy and constant over time. These particles propagate at the speed of sound, either in straight line (homogeneous atmosphere), or with curved trajectories (in the presence of a velocity profile, atmospheric turbulence...), between two successive shocks with the obstacles and limits of the propagation medium. In a collision with an obstacle or a boundary of the propagation domain, the particles can be absorbed or reflected in a new direction of propagation. Geometric acoustics thus become a special case of particle dynamics, so that a sound field can be likened to a gas of sound particles. Under these conditions, the distribution of the energy of the sound field is assimilated to the distribution of the sound particles. Since the local density of sound energy is proportional to the local density of phonons, the only difficulty lies in determining the distribution of these sound particles over time. Although analytical approaches may be considered under certain conditions (lepolles2003), we have chosen an all-digital approach here. With the evolution of computing powers, the numerical simulation of a system composed of several hundred thousand particles is no longer a major difficulty.

Sound particles vs. sound rays
------------------------------------

This concept is relatively similar to the traditional methods of sound beam tracing, implemented in most current closed or open environment noise prediction software. Nevertheless, even if in form these two methods are comparable, the major differences lie in the management of the sound energy carried by sound particles and sound rays. For the latter method, each sound ray carries an intensity whose amplitude decreases proportionally with the square of the propagation distance, thus simulating the acoustic radiation of a spherical source (geometric dispersion). On the other hand, in the concept of sound particles, each particle carries an elementary energy :math:`\epsilon`, whose amplitude does not vary according to the propagation distance.

On the other hand, all the physical phenomena occurring during propagation (reflection, absorption, diffusion, atmospheric absorption) are not considered in the same way in the two approaches. In the "sound rays" approach, these phenomena occur in the form of a weighting applied to the sound intensity carried by a ray. For example, when in contact with an absorption coefficient wall :math:`\alpha`, the intensity of the beam after reflection will be weighted by the coefficient :math:`(1-\alpha)`. In the "sound particles" approach, some of these physical phenomena can be considered probabilistically. When in contact with an absorption coefficient wall :math:`\alpha`, the particle could for example have a probability :math:`(1-\alpha)` of being reflected, and a probability :math:`\alpha` of being absorbed. In the latter case, it disappears from the propagation domain.

One of the major advantages of the sound particle concept lies above all in the consideration of diffuse reflections on the walls and diffusion by encumbrances. In sound ray methods, wall reflections are determined by image source methods that allow only specular reflections, perfectly deterministic, and prohibit the consideration of diffuse reflection phenomena. In the particulate approach, and as we will see below, these diffuse reflections are considered in a probabilistic way, the angle of reflection being able to be chosen according to any arbitrary law of reflection. The same applies to the consideration of diffusion by objects distributed in the propagation medium (machines in industrial premises for example). In the sound ray approach, it is necessary to know exactly the shape and position of the object in order to calculate in a deterministic way the angle of reflection of a sound ray on it. In the particle approach, scattering objects can be considered statistically, with no knowledge other than their spatial distribution and average shape, which significantly reduces costs in terms of calculating and defining the field of study. This statistical (or probabilistic) description, which can be generalized to most of the physical phenomena involved in sound propagation, is therefore the main strength of the sound particles approach compared to the "sound rays" approach.


Code principle
----------------

The principle of simulations is therefore based on the tracking of sound particles, carrying an initial energy :math: `\ epsilon`, emitted from one or more sound sources, in a volume chamber :math:` V` totally or partially closed. Each particle is propagated along rectilinear or curved paths, until it collides with a wall or scattering object. At each collision, the sound particle (or part of its energy) can be absorbed, reflected or transmitted, depending on the absorption and transmission coefficient of the wall or object. As the processing of certain physical phenomena can be performed by random number draws, this simulation procedure can therefore be likened to a Monte Carlo method: raw-latex: `\ cite {burns1990}`. The physical phenomena simulated by these draws of random numbers will be all the better respected as the number of random draws will be large, that is to say that the initial number :math: `N` of sound particles will also be very large. Nevertheless, more :math: `N` will be large, longer will be the duration of the simulations. The choice of :math: `N` is therefore a compromise between the computation time and the accuracy of the results, but is also a function of the geometry and acoustic characteristics of the propagation domain. For example, the more the propagation domain will be absorbing (at the walls, scattering objects, atmospheric absorption), the more it will be necessary to consider sound particles to have a satisfactory description of the physical phenomena.

In the current version of the `` SPPS`` code, two calculation modes are proposed:

- **Random Modeling:** In this mode, the energy of the particle is constant. The phenomena of absorption and atmospheric absorption are considered statistically: depending on the values ​​of the atmospheric absorption and the absorption coefficients of the materials, the particles can be made to disappear completely from the propagation domain, or to remain in the domain with the same energy. The other physical phenomena (diffusion by a congestion, diffuse reflection) are also treated statistically. As the number of sound particles decreases over time, the calculation time decreases gradually. Moreover, the density of sound energy at a point of the domain is then proportional to the number of sound particles at the same point.

- **Energetic modeling:** In this mode, the energy of the particle is weighted according to the values ​​of the atmospheric absorption and the absorption coefficients of the materials. The other physical phenomena (diffusion by congestion, diffuse reflection) are also treated randomly. Since in this mode, the number of sound particles is constant, the duration of the numerical simulations is longer than for the first mode. Moreover, the sound energy density at a point of the domain is then proportional to the sum of the energy of the sound particles at this same point.

Spatial meshing
----------------

Unlike conventional calculation codes based on ray and sound beam throws, the **SPPS** code is based on a volumetric mesh of the propagation domain.

**Benefits:**

	- the mesh of the propagation domain allows to optimize the propagation of sound particles in the domain. Indeed, a pre-processing makes it possible to determine for each mesh of the domain its position with respect to the limits of the domain, its position inside or outside a space requirement, as well as general properties, such as the speed of sound in the mesh and the meteorological conditions (*via* the height with respect to the ground for example). The tracking of a sound particle in the domain is therefore accelerated by the knowledge of the properties of the meshes adjacent to the mesh containing the particle at the current time step;

	- the volume mesh allows to realize a surface mesh of the domain boundaries, which can be used to build surface receivers, on any boundary of the domain;

	- the realization of a volumetric mesh requiring that the geometric model be perfectly closed, thus ensuring that no sound particles can disappear from the propagation domain due to an opening (faces of the model not perfectly joined for example).

**Disadvantages:**

	- the realization of the mesh requiring that the geometric model is perfectly closed, this supposes that the model is perfectly constructed. Calculation codes based on sound rays do not have this constraint, since they "allow" a ray to "leave" the domain and propagate to infinity.

Temporal meshing
-----------------

In parallel, the monitoring of sound particles is carried out in constant time steps, and not in continuous time. It is therefore necessary to choose a sufficiently small time step so that the physical phenomena present in the sound emission and acoustic propagation processes can be applied to a given time step.

**Mesh supported by the code SPPS:**

	- tetrahedral mesh;
