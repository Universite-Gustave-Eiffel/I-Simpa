Room acoustics parameters
=========================

The acoustic quality of an environment, particularly in room acoustics, can be evaluate on the basis of a number of objective acoustic parameters. Most of the time, these parameters are obtained from the knowledge of the instantaneous sound pressure of the impulse response (IR), noted :math:`p(t)`, measured or calculated at a receiving point in the propagation domain. Most of the parameters presented below are proposed by the standard NF EN ISO 3382-1.


Sound level parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Sound pressure level :math:`L_\text{SPL}`
--------------------------------------------------

The **sound pressure level** :math:`L_\text{SPL}` (in dB) expresses the energy of the IR (quadratic sound pressure), in integrated form (steady-state sound level):

.. math:: L_\text{SPL}=10\log \frac{\int_0^\infty p^2(t)\text{d}t}{p_0^2}

or, in time-varying state:

.. math:: L_\text{SPL}(t)=10\log \frac{p^2(t)}{p_0^2},

where :math:`p_0=20\times10^{-6}` Pa is the reference acoustics pressure.

Sound strength :math:`G` (single sound source)
------------------------------------------------

The **Sound strength**, noted :math:`G` (in dB), is the logarithmic ratio of the acoustic energy of the IR obtained at the point of measurement, to that equal to a distance of 10 m from the sound source (NF EN ISO 3382-1):

.. math:: G=10\log \frac{\int_0^\infty p^2(t)\text{d}t}{\int_0^\infty p^2_{10\,m}(t)\text{d}t},

where :math:`p^2_{10\,m}(t)` is the instantaneous acoustics pressure of the IR at the refernce observation point at 10 m from the source.

Sound strength :math:`G` (multiple sound sources)
---------------------------------------------------

When considering simultaneous sound sources, the last relation can be generalized:

.. math::

   \begin{aligned}
   G=&10\log \sum_{s=1}^{N_s}\frac{\int_0^\infty p^2(t)\text{d}t}{\int_0^\infty p^2_{s,10\,m}(t)\text{d}t}\\\nonumber
   =&10\log\sum_{s=1}^{N_s} 10^{G_n/10},\end{aligned}

where :math:`s` is the source number et :math:`N_s` the number if sound sources.


Reverberation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Reverberation time :math:`T`
--------------------------------

The Reverberation time, noted :math:`T` (in s), is obtained from the slope of the decay :math:`d` (or decay rate, in dB/s) of the sound energy, between the points located at :math:`-5` dBand :math:`-(5+L)` dB under steady state conditions (NF EN ISO 3382-1). The value of :math:`L` is classically chosen at :math:`30` (:math:`T_{30}`or :math:`RT30`) and :math:`15` (:math:`T15` or :math:`RT15`). This L value can be set under I-Simpa. The slope of the decay :math:`d` (or rate of decay) is determined by linear regression on the background integration of the quadratic IR:

.. math:: E(t)=\int_t^\infty p^2(\tau)\text{d}\tau.

The reverberation time :math:`T` is obtained using the follwing relation (in s):

.. math:: T=\frac{60}{d}.

Early decay time :math:`EDT`
------------------------------------------

The **Early decay time**, noted :math:`EDT` (in s), is obtained, like the classical reverberation time, by calculating the slope of the background integration decay curve of the quadratic IR, but this time corresponding to the first 10 decibels of the decay under the stationary state (between :math:`0` and :math:`-10` dB).

Clarity and intelligibility parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Clarity :math:`C`
--------------------

**Clarity** :math:`C` is obtained by calculating the ratio between the early energy
(on the time intervals :math:`[0,t_e]`) and the late energy (on the time intervals :math:`[t_e,\infty]`). Time value :math:`t_e` is defined as the early temporal limit, which is choosen as :math:`50` ms (:math:`C50`) or :math:`80` ms
(:math:`C80`) (NF EN ISO 3382-1):

.. math:: C_{t_e}=10\log\frac{\int_0^{t_e} p^2(t)\text{d}t}{\int_{t_e}^\infty p^2(t)\text{d}t}.

Within I-Simpa, early temporal limit :math:`t_e` is a parameter.

Centre time :math:`t_s`
-------------------------

**Centre time**, noted :math:`t_S` (in s), is the centre of gravity of the quadratic IR (NF EN ISO 3382-1:2010):

.. math:: t_s=\frac{\int_0^\infty t\times p^2(t)\text{d}t}{\int_0^\infty p^2(t)\text{d}t}.

If the time :math:`t` is given in ms, thus, centre time :math:`t_s`
is also give in ms.

Definition :math:`D`
--------------------

**Definition**, noted :math:`D` (in %), is the ratio between the early energy (on time interval :math:`[0,t_e]`) and the total energy
(NF EN ISO 3382-1):

.. math:: D=100\times \frac{\int_0^{t_e} p^2(t)\text{d}t}{\int_{0}^\infty p^2(t)\text{d}t}.

**Definition** :math:`D` can also be obtained the **Clarity** (for a time value :math:`t_e`):

.. math:: C_{t_e}=10\log \left( \frac{D_{t_e}}{1-D_{t_e}}\right).

The value of :math:`t_e` is classically taken as 50 ms (:math:`D50`).

Spatialisation parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Apparent Source Width (ASW)
---------------------------------

Early lateral energy fraction :math:`LF`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Lateral fraction** noted :math:`LF` (in %) or
:math:`J_{LF}` (linear) is obtained by considering the orientation of the observation point to the sound source:

.. math:: LF=100\times J_{LF}=100\times \frac{\int_{0.005}^{t_e} |\cos\theta|^2\times p^2(t)\text{d}t}{\int_{0}^{t_e} |\cos\theta|\times p^2(t)\text{d}t},

where :math:`\theta` refers to the angle between the axis normally passing between the two ears of a listener (perpendicular to the orientation of the receiver) and the source-receiver axis.

The value of :math:`t_e` is classically chosen at 80 ms (:math:`LF80`), but can be parameterized in I-Simpa.

The receiving point is normally oriented towards the center of the stage or towards each source position, so that this criterion favors above all the acoustic energy coming from lateral directions.

Lateral energy fraction :math:`LFC`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A weighting of the energy with the cosine of the angle, instead of
square of the cosine (:math:`LF`) is considered subjectively more
(NF EN ISO 3382-1). One then also define the
**lateral energy fraction** noted :math:`LF` (in %) or
:math:`J_{LF}` (in linear) by the following relation:

.. math:: LFC=100\times J_{LFC}=100\times \frac{\int_{0.005}^{t_e} |\cos\theta|\times p^2(t)\text{d}t}{\int_{0}^{t_e} |\cos\theta|\times p^2(t)\text{d}t},

The value of :math:`t_e` is normally set to 80 ms (:math:`LFC80`) but can be set under -Simpa. As with :math:`LF`, the receiving point is normally oriented towards the center of the stage or towards each source position, so that this criterion favors above all the acoustic energy coming from lateral directions.

Auditor Envelope (LEV)
---------------------------------

Late lateral sound level
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In addition, the **Late lateral sound level**, noted :math:`L_J` or :math:`LG` in dB, is obtained by combining the **Sound energy** :math:`G` and the **Early lateral energy fraction**. :math:`LF` (NF EN ISO 3382-1):

.. math:: L_J=10\log \frac{\int_{t_e}^\infty |\cos^2\theta|\times p^2(t)\text{d}t}{\int_0^\infty p^2_{10\,m}(t)\text{d}t}.

As for the :math:`LF` and the :math:`LFC`, the receiving point is normally oriented towards the center of the stage or towards each position source, so that this criterion favours above all the energy acoustics from lateral directions.

Scene Criteria
---------------------------------

Early stage support :math:`\text{ST}_\text{Early}`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The **Early stage support** noted :math:`\text{ST}_\text{Early}` (in dB) is
the ratio of the energy reflected in the initial 0.1 s (100 ms) to the direct sound (including ground reflection) measured at a distance of 1 m from the
acoustic center of an omnidirectional sound source, the others reflective surfaces and objects being located more than 2 m from the
position of the observation point.

.. math:: \text{ST}_\text{Early}=10\log\frac{\int_{0.020}^{0.100} p^2(t)\text{d}t}{\int_{0}^{0.010} p^2(t)\text{d}t}


Expected values
---------------------------

Table A.1 of the NF EN ISO 3382-1 standard proposes recommended values for most of these acoustic parameters, for
concert halls and unoccupied multi-purpose halls up to 25000 m3:

.. table:: Expected values for few room acoustics parameters. From Table A.1 of the NF EN ISO 3382-1 standard.

   +----------------------+----------------------+----------------------+
   | Parameter            | Frequency range      |    Values            |
   +======================+======================+======================+
   | :math:`G`            | 500 à 1000 Hz        | :math:`-`\ 2 to      |
   |                      |                      | :math:`+`\ 10 dB     |
   +----------------------+----------------------+----------------------+
   | :math:`EDT`          | 500 à 1000 Hz        | 1 to 3 s             |
   +----------------------+----------------------+----------------------+
   | :math:`C80`          | 500 à 1000 Hz        | :math:`-`\ 5 to      |
   |                      |                      | :math:`+`\ 5 dB      |
   +----------------------+----------------------+----------------------+
   | :math:`D50`          | 500 à 1000 Hz        | 30 to70%             |
   +----------------------+----------------------+----------------------+
   | :math:`t_s`          | 500 à 1000 Hz        | 60 to 260 ms         |
   +----------------------+----------------------+----------------------+
   | :math:`LF` or        | 125 à 1000 Hz        | 5 to 35%             |
   | :math:`LFC`          |                      |                      |
   +----------------------+----------------------+----------------------+
   | :math:`L_J`          | 125 à 1000 Hz        | :math:`-`\ 14 to     |
   |                      |                      | :math:`+`\ 1 dB      |
   +----------------------+----------------------+----------------------+
