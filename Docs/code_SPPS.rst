Presentation
~~~~~~~~~~~~~~~~~~~~~~~~~~

**SPPS is a sound particles-tracing code, based on geometrical, energetical and probabilistic approaches.**

- `Visit the offical I-Simpa website`_ for additionnal information about SPPS code.

- See the main `characteristics of the code`_ that is embedded in I-Simpa

.. _Visit the offical I-Simpa website: http://i-simpa.ifsttar.fr/presentation/embedded-codes/spps/
.. _`characteristics of the code`: code_characteristics.html

The simulation principle of the SPPS code (from French "Simulation de la Propagation
de Particules Sonores") relies upon tracking sound particles, carrying a amount
of energy ε and emitted from a sound source, within a 3D-domain. Each particle
propagates along a straight line between two time steps Δt (the whole trajectory
may be curved), until collision with an object. At each collision, sound particles
may be absorbed, reflected, scattered, diffused, transmitted, depending on the nature of the object.

Two algorithms can be considered:

	1. The first approach (**Energetic**) is to consider that the energy of the particle is constant. In function of the phenomena, the particle may disappear from the domain or follows its propagation: the number of sound particles decreases along the time.

	2. In the second approach (**Random**), the particle energy is varying according to the physical phenomena occurring during the propagation. In this case, the number of particles in the domain should be constant along the time. Since, in both cases, physical phenomena can be modeled according to probabilistic laws, both approaches are equivalent to Monte-Carlo methods. The accuracy of prediction is then mainly dependent of the initial number of particles.

Detailled information about SPPS code:

- `Using SPPS within I-simpa`_
- `General principle of SPPS`_
- `The physics inside SPPS`_
- `Validation of I-Simpa (online)`_

.. _`Using SPPS within I-simpa`: code_configuration_SPPS.html
.. _`General principle of SPPS`: code_SPPS_principle.html
.. _`The physics inside SPPS`: code_SPPS_modelling.html
.. _`Validation of I-Simpa (online)`: https://i-simpa.ifsttar.fr/support/resources/code-verification/
