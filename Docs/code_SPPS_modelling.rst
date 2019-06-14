Modelling of physical phenomena in **SPPS**
===================================================

Sound source modelling
--------------------------------

General formalism
~~~~~~~~~~~~~~~~~~

Acoustic directivity and probability density of sound particles emission
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

From a numerical modelling point of view, the problem of sound emission from a point source is to attribute to the sound particles, at a given moment (which can be the origin of time), the exact position of the source, propagation directions in accordance with the directivity of the source. For example, in the case of an omnidirectional sound source, it is necessary to verify that the sound particles, once emitted, are uniformly distributed over a sphere centred on the source (figure[geo\_source]). The first solution that comes to mind is to make a deterministic discretization [1]_ of the source's radiation in spherical coordinates. Nevertheless, such discretization of emission angles causes an artificial concentration of sound particles at the poles of the sphere symbolizing the source.


.. figure:: geo_source.eps
   :alt: [geo\_source]Géométrie élémentaire pour une source sonore. La direction d’émission d’une particule est définie en coordonnées sphériques par les angles :math:`\phi` (azimuth) compris entre :math:`0` et :math:`2\pi`, et :math:`\theta` compris entre :math:`0` et :math:`\pi` (angle polaire), ou par l’angle solide élémentaire :math:`d\Omega`.
   :width: 7.00000cm

   [geo\_source]Géométrie élémentaire pour une source sonore. La direction d’émission d’une particule est définie en coordonnées sphériques par les angles :math:`\phi` (azimuth) compris entre :math:`0` et :math:`2\pi`, et :math:`\theta` compris entre :math:`0` et :math:`\pi` (angle polaire), ou par l’angle solide élémentaire :math:`d\Omega`.

It is in fact necessary to check the respect of the number of particles emitted by elementary solid angle :math:`d\Omega = d\phi\,\sin\theta\,d\theta\,d\theta`, checks the directivity of the source, either in a deterministic or random way (figure[geo\_source]). In our case, we chose a random draw of the initial directions of the particles. It is then necessary to define the probability densities :math:`g(\phi)` and :math:`p(\theta)`, respectively for the angles :math:`\phi\in[0.2\pi]` and :math:`\theta\in[0.\pi]`, such as the probability density :math:`Q(\theta,\phi)` of emission in the direction :math:`(\theta,\phi)` (or probability density :math:`F(\Omega)` of emission in the elementary solid angle :math:`\Omega`) either in accordance with the directivity of the source. In practice, :math:`Q(\theta,\phi)` is nothing more than the directivity of the source. By definition, these probability densities verify the following relationship:

.. math::

   \begin{aligned}
           \int F(\Omega)\, d\Omega &= \iint Q(\theta,\phi) \,\sin\theta \, d\phi\,d\theta \\
           &=\int_{0}^{2\pi} g(\phi) \, d\phi \int_{0}^{\pi} p(\theta)\,\sin\theta\,d\theta\\
           &=1.
           \end{aligned}

From the point of view of sound power, this is tantamount to considering that the power :math:`W` of the source is well distributed over the surface of a sphere centered on the source:

.. math:: \iint W\,Q(\theta,\phi) \,\sin\theta \, d\phi\,d\theta=W.

If the densities :math:`g(\phi)` and :math:`p(\theta)` are independent, they check:

.. math:: \label{cond_g} \int_{0}^{2\pi} g(\phi) \, d\phi = 1,
and

.. math:: \label{cond_p} \int_{0}^{\pi} p(\theta)\,\sin\phi\,d\theta=1.

Source power and elementary particle energy
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Acoustically, for a period of time: math:`\Delta t`, a power source: math:`W` emits an amount of energy :math:`E=W\times \Delta t`. From a particle concept point of view, each particle carries an elementary energy: math:`\varepsilon_0`. If the source emits:math:`N` sound particles, energy conservation between the two approaches therefore requires:

.. math:: N \times \varepsilon_0 = W\times \Delta t,
then

.. math:: \label{energie_elementaire} \varepsilon_0 = \frac{W}{N}\times \Delta t.

Onidirectionnal source
~~~~~~~~~~~~~~~~~~~~~~~~~

Physical description
''''''''''''''''''''

An omnidirectional sound source is a source that radiates in all directions with the same amplitude. The directivity of the source is similar to a sphere. The directivity coefficient :math:`Q` associated with the directivity of the source is equal to :math:`1`.

Modeling
''''''''''''

In the case of an omnidirectional source, the densities :math:`g(\phi)` and :math:`p(\theta)` being uniform, the relationships ([cond\_g]) and ([cond\_p]) check:

.. math:: \label{cond_g_2} \int_{0}^{2\pi} g(\phi) \, d\phi =\int_{0}^{2\pi} A \, d\phi = 1
and

.. math:: \label{cond_p_2} \int_{0}^{\pi} p(\theta)\,\sin\theta\,d\theta=\int_{0}^{\pi} B\,\sin\theta\,d\theta=1,

où :math:`A=1/2\pi` et :math:`B=1/2` sont deux constantes de normalisation. En pratique, la méthode consiste dans un premier temps, à tirer un angle :math:`\theta` entre :math:`0` et :math:`2\pi` de la manière suivante:

.. math:: \label{choix_phi} \phi=2\pi \times u \quad \in [0,2\pi]

où :math:`u` est un nombre aléatoire compris entre :math:`0` et :math:`1`, et défini par une distribution uniforme. Si l’angle :math:`\theta` est choisi de la même manière (**i.e.** :math:`\theta= \pi \times v`, :math:`v` étant un nombre aléatoire suivant une distribution uniforme entre :math:`0` et :math:`1`), la distribution des directions d’émission ne respecte pas l’uniformité par angle solide élémentaire, puisque la condition ([cond\_p]) n’est pas vérifiée. Dans ce cas, les directions autour des pôles seraient privilégiées. Il faut en fait déterminer les angles :math:`\theta` qui vérifient une distribution proportionnelle à :math:`\sin\theta`. Dans ce cas simple, la procédure consiste à appliquer la méthode de la fonction de distribution cumulative inverse. D’après la relation ([cond\_p]), la probabilité :math:`f(\hat{\theta})` de tirer un angle :math:`\theta<\hat{\theta}` est donnée par:

.. math:: \label{cdf_dist_theta} f(\hat{\theta}) = \frac{1}{2} \int_{0}^{\hat{\theta}} \sin\theta \, d\theta=\frac{1}{2} \left[\cos\hat{\theta}-1\right].

Sachant que cette distribution est comprise entre :math:`0` et :math:`1`, le choix de l’angle :math:`\hat{\theta}` est réalisé en tirant aléatoirement un nombre :math:`v\in [0,1]`, suivant une distribution uniforme, tel que:

.. math:: \hat{\theta} = \cos^{-1} \left(2v-1\right)\quad \in [0,\pi].

Les vecteurs de propagation :math:`\mathbf{v}(v_x,v_y,v_z)`, de norme :math:`c` (célérité du son), sont ensuite définis par les relations:

.. math::

   \mathbf{v}=\left\{\begin{array}{l}
               v_x=c\,\cos\theta\,\sin\phi\\
               v_y=c\,\sin\theta\,\sin\phi\\
               v_z=c\,\cos\phi
               \end{array}\right.

Vérification
''''''''''''

La figure [distribution\_source\_omni] présente un exemple de répartition des angles :math:`\theta` et :math:`\phi` obtenus suivant cette méthode de tirage avec :math:`10000` réalisations. On observe aisément que la distribution des angles vérifient la distribution théorique. Il est bien entendu que la qualité du tirage aléatoire est fonction de la méthode de génération des nombres aléatoires, et que le respect des distributions théoriques augmente avec le nombre de tirages.

Source unidirectionnelle
~~~~~~~~~~~~~~~~~~~~~~~~

Description physique
''''''''''''''''''''

Une source sonore unidirectionnelle est une source qui rayonne dans une seule direction de l’espace, définie par les angles :math:`\theta` et :math:`\phi`, au point d’émission et dans le repère absolu. Le coefficient de directivité associé à la directivité de la source est alors défini par:

.. math:: Q\left(\theta,\phi\right)=\delta\left(\theta,\phi\right)=\delta\left(\theta\right)\times\delta\left(\phi\right),

où :math:`\delta` désigne la distribution de Dirac.

Modélisation
''''''''''''

La modélisation de ce type de source est entièrement déterministe et ne pose donc pas de problème. Il suffit de définir un vecteur :math:`\mathbf{s}(s_x,s_y,s_z)` dans le repère absolu de la scène, tel que :

.. math::

   \mathbf{s}=\left\{\begin{array}{l}
               s_x=s\,\cos\theta\,\sin\phi\\
               s_y=s\,\sin\theta\,\sin\phi\\
               s_z=s\,\cos\phi
               \end{array}\right.

Sources « plan » (XY, YZ et XZ)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Description physique
''''''''''''''''''''

Une source sonore de type « plan » est une source qui rayonne dans un plan. Par défaut, on considère trois sources XY, YZ et XZ définis par les trois plans de référence :math:`(xOy)`, :math:`(yOz)` et :math:`(xOz)` respectivement.

Modélisation
''''''''''''

La procédure consiste à déterminer aléatoirement la direction de propagation d’une particule dans un plan. Reprenant la convention d’angle présentée à la figure [geo\_source], on aura:

.. math::

   \left\{\begin{array}{llll}
               \text{plan XY:}& \theta=2\pi \times u\quad &\text{et}&\quad \phi=\pi/2\\
               \text{plan YZ:}& \theta=0 \quad &\text{et}&\quad \phi=2\pi \times u\\
               \text{plan XZ:}& \theta=\pi/2\quad &\text{et}&\quad \phi=2\pi \times u
               \end{array}\right.

où :math:`u` désigne un nombre aléatoire compris entre :math:`0` et :math:`1`. Par la suite, le vecteur de propagation :math:`\mathbf{v}(v_x,v_y,v_z)`, de norme :math:`c` (célérité du son), est défini par les relations:

.. math::

   \mathbf{v}=\left\{\begin{array}{l}
               v_x=c\,\cos\theta\,\sin\phi\\
               v_y=c\,\sin\theta\,\sin\phi\\
               v_z=c\,\cos\phi
               \end{array}\right.

Modélisation de la propagation acoustique
-----------------------------------------

Propagation acoustique
~~~~~~~~~~~~~~~~~~~~~~

Description physique
''''''''''''''''''''

En l’absence d’absorption et de réflexion sur les parois du domaine ou sur des objets, la décroissance de l’intensité sonore issue d’une source omnidrectionnelle s’écrit:

.. math:: I=\frac{Q}{4\pi\,r^2},

où :math:`r` est la distance à la source, et :math:`Q` la directivité de la source dans la direction d’observation (:math:`Q=1` pour une source omnidirectionnelle). Cette décroissance traduit le phénomène de dispersion géométrique, qui décrit l’« étalement » d’une onde sphérique au fur et à mesure de la propagation.

Modélisation
''''''''''''

Compte tenu de la méthode présentée au paragraphe [par:modelisation\_source\_omni] pour une source omnidirectionnelle, la dispersion géométrique est automatiquement respectée. En effet, la méthode numérique proposée permet d’obtenir une répartition uniforme des particules sur un angle solide élémentaire. Sur une sphère, la distribution des particules :math:`n(r)` (en :math:`m^2`) est donc égale à:

.. math:: n(r)=\frac{N}{4\pi\,r^2},

où :math:`N` est le nombre de particules. La distribution des particules vérifie donc bien la même décroissance que l’intensité. Il faut toutefois noter, que plus le point d’observation sera loin de la source, plus il faudra de particules sonores.

Vérification
''''''''''''

La figure [verif\_dispersion] montre les résultats numériques de la propagation en champ libre [2]_, pour une source sonore omnidirectionnelle, sans absorption atmosphérique (nombre de particules :math:`N=20\times 10^6`). L’accord est excellent.

|image|

Absorption atmosphérique
~~~~~~~~~~~~~~~~~~~~~~~~

Description physique
''''''''''''''''''''

Durant sa propagation dans l’air, une onde sonore est en partie atténuée du fait de mécanismes physiques particuliers (processus de transmission « classique », absorption moléculaire due à la relaxation rotationnelle, absorption moléculaire due à la relaxation vibratoire de l’oxygène et de l’azote) (bass1984). Ainsi, après une distance de propagation :math:`r`, l’amplitude :math:`p_t` de la pression acoustique décroît suivant la relation (ISO9613-1:1993):

.. math::

   \label{eq:att_atmos_pression}
           p_t= p_i \exp \left(-\frac{\ln(10)}{20}\alpha_\text{air}\times r\right)

où :math:`p_i` est la pression initiale. Considérant que l’intensité sonore est proportionnelle au carré de la pression acoustique,

.. math::

   \label{eq:att_atmos_pression_I}
           I\propto p_t^2 \propto \exp \left(-2\frac{\ln(10)}{20}\alpha_\text{air}\times r\right)

et écrivant que l’intensité :math:`I` de l’onde sonore décroît suivant la relation:

.. math::

   \label{eq:att_atmos}
           I= I_0 \exp -m\,r,

où :math:`I_0` est l’intensité initiale de l’onde sonore, alors le coefficient d’absorption atmosphérique :math:`m` (en Np/m) peut-être exprimé à partir du coefficient d’absorption atmosphérique :math:`\alpha_\text{air}` (en dB/m), par la relation:

.. math:: m=\frac{\ln 10}{10}\alpha_\text{air}.

Dans le code **SPPS**, le coefficient d’absorption atmosphérique :math:`\alpha_{air}` est calculé suivant la norme ISO 9613-1:1993 *Acoustique. Atténuation du son lors de sa propagation à l’air libre. Partie 1 : calcul de l’absorption atmosphérique* (ISO9613-1:1993), en considérant la fréquence centrale de chaque bande de fréquence [3]_ de calcul conformément à la norme de référence (*cf.* (paragraph 8.2.1, ISO9613-1:1993). Cette approximation est considérée comme valide si le produit de la distance source-récepteur (en km) par le carré de la fréquence centrale (en kHz) ne dépasse pas :math:`6` km/kHz:math:`^2` pour les bandes de tiers d’octave et :math:`3` km/kHz:math:`^2` pour les bandes d’octave. La distance de propagation ne doit néanmoins pas dépasser :math:`6` km pour les bandes de tiers d’octave et :math:`3` km pour les bandes d’octave, quelle que soit la fréquence centrale considérée.

Modélisation « aléatoire »
''''''''''''''''''''''''''

En choisissant le mode de calcul « aléatoire », l’absorption atmosphérique est prise en compte comme une probabilité de disparition de la particule sonore durant son déplacement. La densité de probabilité correspondante peut-être définie à partir de la relation ([eq:att\_atmos]):

.. math:: f(r)=\exp -m\,r.

Cette quantité exprime la probabilité que la particule ne pas soit absorbée durant son déplacement :math:`r`. La densité de probabilité :math:`f(r)` est bien comprise entre :math:`1` et :math:`0`: :math:`f(0)=1`, la probabilité est maximale, la particule ne pouvant pas être absorbée si elle ne déplace pas; :math:`f(\infty)=0`, la probabilité est nulle puisque la particule ne peut pas se propager à l’infini (figure [attenuation]). On vérifie également facilement que la probabilité de propagation est indépendante de la probabilité de propagation précédente:

.. math:: f\left(\Sigma_{n=1}^N r_i\right)=\Pi_{i=1}^N f(r_n).

La prise en compte de l’absorption atmosphérique est relativement simple. Il suffit en effet de tirer un nombre aléatoire uniforme :math:`\zeta` compris entre :math:`0` et :math:`1`, à chaque pas de temps, et de comparer ce nombre à la densité de probabilité :math:`f(d_0)` correspondant à un déplacement élémentaire :math:`d_O=c\Delta t` sur un pas de temps :math:`\Delta t`. Si ce nombre :math:`\zeta` est inférieur à :math:`f(d_o)`, il y aura propagation. Dans le cas contraire, il y aura absorption atmosphérique, donc disparition de la particule. Même sur un faible nombre de particules, cette méthode permet de prendre en compte correctement l’absorption atmosphérique.

.. figure:: attenuation.eps
   :alt: [attenuation]Prise en compte de l’absorption atmosphérique par un processus aléatoire. La courbe :math:`f(r)` sépare le domaine de propagation du domaine d’absorption atmosphérique.
   :width: 8.00000cm

   [attenuation]Prise en compte de l’absorption atmosphérique par un processus aléatoire. La courbe :math:`f(r)` sépare le domaine de propagation du domaine d’absorption atmosphérique.

Modélisation « énergétique »
''''''''''''''''''''''''''''

En choisissant l’option de calcul « énergétique », l’énergie de la particule est pondérée tout au long de son déplacement, en utilisant la relation ([eq:att\_atmos]).

|image|

Vérification
''''''''''''

A titre d’illustration, la figure [illustration\_att\_atmos] présente la décroissance sonore calculée [4]_ par le code **SPPS** à :math:`10` kHz pour des conditions atmosphériques classiques (:math:`T=20`\ °C, :math:`H=50`\ %, :math:`P=101325` Pa, soit :math:`m=0.036` m:math:`^{-1}`), pour les deux types de modélisation, comparée à la décroissance théorique présentée à la relation ([eq:att\_atmos]). Comme attendu, la modélisation « énergétique » donne un meilleur résultat que la modélisation « aléatoire », l’écart moyen par rapport à la courbe théorique étant de :math:`0.17` dB et :math:`0.41` dB respectivement, les temps de calcul étant par ailleurs similaires.

Profil de célérité acoustique
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Description physique
''''''''''''''''''''

En milieu extérieur et pour des hauteurs de propagation limitées à :math:`100` m, la propagation acoustique peut être influencée par les conditions micro-météorologiques régies par des lois thermiques (transfert de chaleur) et aérodynamiques (profils de vent). Les phénomènes qui apparaissent à ces altitudes présentent une interaction très forte avec le sol (topographie, température de surface et de sous-sol, hygrométrie, cultures, forêts, obstacles, bâti…). De plus, ils évoluent rapidement dans le temps et dans l’espace, ce qui rend leur description analytique et leur modélisation numérique complexe. Les facteurs thermiques et aérodynamiques qui influent sur la propagation sont les suivants:

	-  **Facteurs thermiques** : les échanges thermiques entre le sol et la couche basse de l’atmosphère conduisent à une variation de la température de l’air en fonction de la hauteur au dessus du sol, et donc a une variation de la vitesse du son.

	-  **Facteurs aérodynamiques** : compte tenu de l’état de rugosité de surface du sol, la vitesse du vent est toujours plus élevée en hauteur qu’au niveau du sol. Dans une situation donnée, la vitesse du son en présence de vent correspond à la somme algébrique de la vitesse du son en l’absence de vent et de la projection du vecteur vent sur la direction de propagation considérée. Cette vitesse varie donc en fonction de la hauteur au-dessus du sol.

Par analogie avec les lois de l’optique, l’effet des conditions atmosphériques sur la propagation acoustique peut être décrit à travers l’expression de l’indice acoustique :math:`n` du milieu de propagation. Si l’on se place dans une coupe verticale, cet indice est supposé varier avec l’altitude :math:`z` et avec la distance source-récepteur :math:`r`, tel que :

.. math:: n(r,z)=\frac{c(r,z)}{c_0}=\langle n(r,z)\rangle + \mu(r,z),

où :math:`c` est la célérité effective de l’onde sonore dans le milieu traversé et :math:`c_0` celle de référence. Ainsi, on peut distinguer deux phénomènes distincts influant sur la propagation acoustique, la réfraction et la turbulence atmosphérique. Ces phénomènes sont respectivement liés aux parties déterministe :math:`\langle n\rangle` et stochastique :math:`\mu` de l’indice du milieu de propagation. En pratique, ces phénomènes de réfraction et de turbulence co-existent et interagissent, ce qui conduit à des conditions de propagation complexes, ainsi qu’à une très large dispersion des niveaux sonores rencontrés *in situ*, tous paramètres identiques par ailleurs (topographie, nature de sol, géométrie source-récepteur…).

Modèle de profil de célérité
''''''''''''''''''''''''''''

Le profil moyen de célérité du son dépend ainsi des profils moyens de vent et de température. Ce profil de célérité peut être décrit de manière analytique, suivant qu’il suit une loi linéaire (« :math:`\text{lin}` »), logarithmique (« :math:`\text{log}` »), hybrides (« :math:`\text{log-lin}` ») ou autre. Les profils « \ :math:`\text{log}` » présentent ainsi l’avantage de traduire le très fort gradient vertical de célérité du son au voisinage immédiat du sol, mais ne rendent pas bien compte de l’évolution plus modérée avec l’altitude au-delà d’une certaine hauteur. A l’inverse, les profils « \ :math:`\text{lin}` » minimisent les effets au voisinage du sol et ne sont donc pas représentatifs de la réalité lorsque l’on se place à très basse altitude. Un bon compromis consiste donc à utiliser des profils hybrides du type « \ :math:`\text{log-lin}` » (valables surtout pour une atmosphère dite « stable »), exprimés à travers les paramètres :math:`a_\text{log}` et :math:`b_\text{lin}` qui apparaissent dans l’expression analytique du profil vertical de la célérité effective du son :

.. math::

   \label{eq:celerite_log_lin}
           \langle c(z)\rangle=c_0+ a_\text{log}\times \ln\left(1+\frac{z}{z_0}\right)+b_\text{lin}\times \left( z-z_0\right),

où :math:`z_0` est le paramètre de rugosité, dont les valeurs typiques s’étendent de :math:`10^{-2}` m environ pour l’herbe rase à plusieurs mètres en milieu urbain. Le gradient vertical s’exprime alors en dérivant suivant la variable :math:`z`:

.. math:: \frac{\partial\langle c(z)\rangle}{\partial z}=\frac{a_\text{log}}{z_0}+b_\text{lin}.

La propagation dans un milieu à célérité variable a pour principal effet d’incurver les rayons sonores vers le bas ou vers le haut suivant que le gradient vertical de célérité du son soit respectivement positif (*conditions (très) favorables* à la propagation) ou négatif (*conditions (très) défavorables* à la propagation). L’état transitoire entre ces :math:`2` états représente des *conditions homogènes* de propagation.

	-  **Homogène:** la célérité :math:`c` est identique en tout point du domaine et égale à la célérité de référence :math:`c_0`, cette dernière étant calculée en fonction des conditions de température et d’humidité, par la formule:

	   .. math:: c_0=343.2\sqrt{\frac{T}{T_\text{ref}}},

	   où :math:`T` est la température (en K), et :math:`T_\text{ref}=293.15` K la température de référence (ISO9613-1:1993).

	-  **Très défavorable** : :math:`a_\text{log}=-1` et :math:`b_\text{lin}=-0.12`;

	-  **Défavorable** : :math:`a_\text{log}=-0.4` et :math:`b_\text{lin}=-0.04`;

	-  **Favorable**: :math:`a_\text{log}=+0.4` et :math:`b_\text{lin}=+0.04`;

	-  **Très favorable**: :math:`a_\text{log}=+1` et :math:`b_\text{lin}=+0.12`.

La courbure du rayon, à la frontière des zones (I) et (II), est obtenue en appliquant la construction de Huygens-Fresnel, aboutissant à la loi de Snell-Descartes suivante (salomons2001) (figure [geo\_profil\_celerite]):

.. math::

   \label{relation_snell}
           \frac{\cos\gamma_1}{c_1}=\frac{\cos\gamma_2}{c_2},

où :math:`c_1` et :math:`c_2` sont respectivement les normes des vecteurs de propagation :math:`\mathbf{c_1}` et :math:`\mathbf{c_2}`, et où les angles :math:`\gamma_1` et :math:`\gamma_2` sont définis par rapport à l’axe horizontal au plan :math:`(xOy)`. Par construction, la projection de la direction de propagation dans le plan :math:`(xOy)`, définie par l’angle :math:`\phi` en coordonnées sphériques, est conservée (figure [geo\_profil\_celerite]).

|image|

Modélisation
''''''''''''

Quelque soit le mode de calcul choisi, la prise en compte de la célérité est identique. A chaque pas de temps, la norme de célérité est calculée en fonction du profil de célérité retenu, à partir de la relation ([eq:celerite\_log\_lin]). Pour déterminer la nouvelle direction de propagation, dû au changement de célérité, il faut ensuite appliquer la relation ([relation\_snell]). Connaissant l’angle :math:`\theta_1` de la direction de propagation initiale, la nouvelle direction de propagation est définie par:

.. math::

   \label{cosgamma2}
           \cos\gamma_2=\frac{c_2}{c_1}\cos\gamma_1=\frac{c_2}{c_1}\frac{\sqrt{c_{1x}^2+c_{1y}^2}}{c_1}.

 Les coordonnées du vecteur propagation sont ensuite obtenues par:

.. math::

   \mathbf{c_2}=\left\{\begin{array}{l}
           c_{2x}=c_2\cos\gamma_2\cos\phi\\
           c_{2y}=c_2\cos\gamma_2\sin\phi\\
           c_{2z}=c_2\sin\gamma_2
           \end{array}\right.

avec

.. math:: \sin\phi=\frac{c_{1y}}{\sqrt{c_{1x}^2+c_{1y}^2}},

et

.. math:: \cos\phi=\frac{c_{1x}}{\sqrt{c_{1x}^2+c_{1y}^2}}.

D’un point de vue de la simulation numérique, le calcul du :math:`\cos\gamma_2` par la relation ([cosgamma2]) peut donner des valeurs supérieures à :math:`1` ce qui n’est évidemment pas physique. Ce cas se produit lors d’une inversion de la courbure (*turning point*) d’un rayon. Pour éviter ce problème et imposer un changement de courbure, la procédure consiste à imposer la valeur de :math:`\gamma_2` à :math:`1-\epsilon` (:math:`\epsilon` étant une valeur très petite) et à changer l’orientation de la composante :math:`c_{2z}` (*i.e.* :math:`c_{2z}\rightarrow -c_{2z}`).

|image|

A titre d’exemple, la figure [illustration\_refraction] présente deux illustrations de la prise en compte de la réfraction acoustique en utilisant cette méthode. Cette figure peut être comparée directement avec les exemples données dans la référence :raw-latex:`\cite[figures 4.5 et 4.6]{salomons2001}`.

Diffusion par un encombrement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La présence d’un grand nombre d’objets sur le parcours d’une onde sonore peut engendrer un processus de diffusion. Ce processus peut être simulé de manière déterministe, en modélisant chaque objet individuellement. Lorsque le nombre d’objets devient important, et que ces objets sont de tailles similaires (exemple d’un hall industriel avec de nombreuses machines (sans émission acoustique) ou boite similaires), il peut être plus intéressant de modéliser statistiquement cet encombrement.

Description physique
''''''''''''''''''''

Afin de prendre en compte la diffraction et l’absorption des objets diffusants répartis dans le milieu de propagation, nous avons considéré une approche similaire à celle de Ondet et Barbry présentée dans la référence (ondet1989), elle même reprise des travaux de Kuttruff (kuttruff1981), de Auletta (auletta1985, auletta1986), et de Lindquist (lindqvist1982), entre autres. Dans cette approche,

	-  les objets diffusants sont considérés comme ponctuels. Les particules sont renvoyées dans toutes les directions de l’espace à chaque collision avec un objet diffusant (sauf en cas d’absorption). Cette hypothèse est valide en général quand la longueur d’onde est de l’ordre de grandeur de la dimension caractéristique de l’obstacle;

	-  le phénomène de diffusion suit un processus de Poisson, ce qui signifie que la probabilité de collision d’une particule sonore avec un objet diffusant, suit une loi de Poisson. Les probabilités de collision sont indépendantes les unes des autres (la probabilité de collision pendant le temps :math:`t` et :math:`t+dt` est indépendante des collisions avant le temps :math:`t`);

	-  les objets de l’encombrement ne produisent pas de particules (*i.e.* ces objets ne sont pas des sources sonores).

.. figure:: diffusion_encombrement.eps
   :alt: [fig:diffusion\_encombrement]Illustration de la diffusion par un encombrement.
   :width: 7.00000cm

   [fig:diffusion\_encombrement]Illustration de la diffusion par un encombrement.

Une onde sonore se propageant dans le milieu peut être amenée à rentrer en contact avec des objets diffusants, entraînant simultanément la diffraction de l’onde, et, en partie, son absorption. Par analogie, dans l’approche particulaire, une particule entrant en contact avec un objet diffusant peut être, soit absorbée, soit réfléchie dans une autre direction de propagation (figure [fig:diffusion\_encombrement]). A l’échelle macroscopique, c’est-à-dire considérant simultanément l’ensemble des particules sonores, il se produit un processus de diffusion, caractérisé par:

	-  le coefficient d’absorption :math:`\alpha_c` des objets diffusants;

	-  la densité d’encombrement :math:`n_c` du milieu de propagation, défini par le nombre :math:`N_c` d’obstacles présent dans le volume :math:`V_c`:

   .. math:: n_c=\frac{N_c}{V_c}.

	-  la section moyenne de diffusion :math:`q_c`, c’est-à-dire la surface moyenne de l’objet diffusant, vue par une particule dans une direction incidente donnée. En pratique, cette donnée est très difficile à obtenir, voir impossible, dès lors que les objets diffusants sont de formes complexes et différentes. Dans cette condition, il est commun d’assimiler l’objet diffusant à une sphère, ayant la même surface externe :math:`s_c` que l’objet. Quelque soit l’angle d’incidence, la section visible de la sphère (section moyenne de diffusion) sera égale à un quart de la surface totale de la sphère, soit:

   .. math:: q_c=\frac{s_c}{4}.

		-  la section moyenne de diffraction par unité de volume :math:`\nu_c`, encore appelée fréquence de diffusion, par

		   .. math:: \nu_c=n_c\,q_c,

		    si tous les objets diffusants sont identiques, ou

		   .. math:: \nu_c=\frac{1}{V}\sum_{p=1}^{N_c}\frac{s_{c_p}}{4}

		    si chaque objet diffusant :math:`p` est défini par sa surface :math:`s_{c_p}`. En pratique, et dans la suite du document, les objets diffusants seront considérés uniformes dans un même volume de diffusion. Néanmoins dans un même volume de propagation, plusieurs volumes de diffusion disjoints peuvent être considérés.

Puisque le phénomène de diffusion suit une loi de Poisson, la probabilité qu’une particule sonore subisse :math:`k` collisions avec des objets diffusants, au bout d’un temps :math:`t_k` est égale à:

.. math:: W_k (c\,t_k)=\frac{\left(\nu_c \, c\,t_k\right)^k}{k!}\exp\left(-\nu_c\, c\, t_k\right),

où :math:`c\, t_k` est la distance parcourue pendant un temps :math:`t_k`, pouvant être exprimée à partir de la distance :math:`R_p` séparant deux collisions (figure [fig:diffusion\_encombrement]):

.. math:: c\,t_k=\sum_{p=1}^k R_p.

Les probabilités de collision étant indépendantes les unes des autres :raw-latex:`\cite{ondet1989}`, on montre aisément que les variables aléatoires :math:`R_i` (notée :math:`R` par la suite) suivent la densité de probabilité :math:`f(R)` suivante:

.. math:: \label{dispois} f(R)=\nu_c \exp\left(-\nu_c \, R\right).

Le libre parcours moyen :math:`\lambda_c` (distance moyenne entre deux collisions) est simplement obtenu en exprimant le premier moment de la densité de probabilité ci-dessus, à savoir:

.. math:: \lambda_c=\int_0^\infty R\, f(R)\, dR=\frac{1}{\nu_c}.

Modélisation
''''''''''''

Par définition, la fonction de distribution cumulée, associée à cette densité de probabilité, est définie par la relation suivante:

.. math:: \label{fdc} p(\hat{R})=\int_0^{\hat{R}} f(R)\, dR=1-\exp\left(-\nu_c\,\hat{R}\right).

Cette fonction de distribution cumulée exprime simplement la probabilité que la particule subisse une collision avec un objet diffusant durant un trajet de longueur :math:`\hat{R}`. Cette fonction est donc nulle pour :math:`\hat{R}=0` et égale à :math:`1` pour :math:`\hat{R}=\infty`. La simulation numérique du processus de diffusion est réalisée par la méthode de la fonction de distribution cumulée inverse, obtenue en inversant la relation ([fdc]), soit:

.. math:: \label{fdci} \hat{R}=-\frac{1}{\nu_c}\ln \left[ 1-p(\hat{R})\right].

La fonction de distribution cumulée étant comprise entre :math:`0` et :math:`1`, elle peut être assimilée à une variable aléatoire :math:`\xi` comprise entre :math:`0` et :math:`1`. En tirant une succession de variables aléatoires :math:`\xi_i`, on peut ainsi déterminer une succession de trajets de longueur :math:`\hat{R}_i` qui satisfait bien à la fonction de distribution ([dispois]) de notre problème:

.. math:: \label{fdci_2} \hat{R}_n=-\frac{1}{\nu_c}\ln \left[ 1-\xi_n\right].

Un exemple de tirage aléatoire utilisant la méthode de la distribution cumulative inverse, est proposé à la figure [fig:verification\_diffusion\_encombrement]. La comparaison avec la distribution théorique ([dispois]) montre un excellent comportement de la méthode.

|image|

Concrètement, lorsque qu’une particule sonore :math:`n` entre dans une zone d’encombrement, on lui associe une distance de collision :math:`R_n` avec un objet de l’encombrement en appliquant la relation ([fdci\_2]). Au fur et à mesure de la propagation de la particule sonore dans la zone d’encombrement, un test est réalisé pour déterminer si la distance cumulée :math:`d_n` de la particule dans la zone d’encombrement est inférieure ou supérieure à :math:`R_n`. Si la distance parcourue :math:`d_n` est supérieure à la distance de collision :math:`R_n` la particule entre en collision avec un objet. En mode « énergétique », l’énergie de la particule sonore est pondérée par le coefficient d’absorption moyen :math:`\alpha_c` de l’encombrement et poursuit sa propagation dans une direction aléatoire (distribution uniforme [5]_). En mode « aléatoire », un nouveau tirage aléatoire sur une variable uniforme :math:`u` permet de déterminer si la particule est absorbée par l’objet diffusant (procédure identique à l’absorption par une paroi, *cf.* paragraphe [sec:modelisation\_reflexion]), ou réfléchie dans une direction uniforme. Après chaque collision, la distance de propagation cumulée est remise à zéro, et un nouveau tirage est effectué pour déterminer la prochaine distance de collision. Quelque soit le mode de calcul choisi, tant que la distance parcourue par la particule est inférieure à la distance de collision, la particule poursuit sont trajet sans changer de direction.

|image|

Modélisation des parois
-----------------------

Description physique
~~~~~~~~~~~~~~~~~~~~

Absorption, dissipation et transmission acoustique
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Au contact d’une paroi, une onde sonore sera en partie réfléchie vers l’intérieur du domaine pour une part :math:`R`, en partie dissipée par transformation de l’énergie acoustique en chaleur dans le matériau pour une part :math:`\beta`), le reste étant transmis à travers le matériau dans le domaine adjacent pour une part :math:`\tau`. Ce dernier coefficient est défini comme le facteur de transmission. Si :math:`W_i` est la puissance incidente sur une paroi, alors une partie :math:`W_r=R\,W_i` sera réfléchie, une partie :math:`W_d=\beta\,W_i` sera dissipée dans le matériau, et une partie :math:`W_t=\tau\,W_i` sera transmise à travers la cloison. Par construction [6]_, les coefficients :math:`R`, :math:`\beta` et :math:`\tau` sont compris entre 0 et 1. Au final, le bilan énergétique de la paroi s’écrit alors:

.. math:: R+\beta+\tau=1.

.. figure:: principe_paroi.eps
   :alt: [principe\_paroi]Illustration des mécanismes de réflexion, d’absorption, de dissipation et de transmission acoustique par une paroi.
   :width: 6.00000cm

   [principe\_paroi]Illustration des mécanismes de réflexion, d’absorption, de dissipation et de transmission acoustique par une paroi.

Il est usuel de définir le coefficient d’absorption :math:`\alpha` de la paroi comme la somme de la partie transmise :math:`\tau` et de la partie dissipée :math:`\beta`, sous la forme :math:`\alpha=\beta+\tau`, de telle manière que le bilan énergétique ci-dessus s’écrive:

.. math:: R=1-\alpha.

Le coefficient d’absorption d’un matériau peut être mesuré en utilisant les procédures standardisées ISO 354 pour la méthode en chambre réverbérante (ISO354:2003) et ISO 10534 (parties 1 et 2) pour la méthode utilisant un tube d’impédance (ISO10534-1:1996,ISO10534-2:1998). En ce qui concerne le coefficient de transmission acoustique, le lecteur pourra se reporter aux différentes parties de la norme relative la transmission aérienne (parties 1, 3, 4 et 5, (cite{)ISO140-1:1997,ISO140-3:1995,ISO140-4:1998,ISO140-5:1998)).

Diffusion acoustique
^^^^^^^^^^^^^^^^^^^^

D’autre part, suivant la forme, la taille et la distribution des irrégularités de la paroi, l’onde sonore pourra être réfléchie simultanément dans la direction spéculaire et dans d’autres directions. En acoustique des salles, il est commun de considérer qu’une fraction :math:`1-s` de l’énergie sonore sera réfléchie dans la direction de réflexion spéculaire, tandis qu’une fraction :math:`s` de l’énergie sera réfléchie dans les autres directions de l’espace, suivant une loi de réflexion caractérisée par les irrégularités de la paroi (embrechts2001). Dans ce dernier cas, on parle alors de *réflexion diffuse*, :math:`s` étant appelé coefficient de diffraction (*scattering coefficient* en anglais). En acoustique des salles, de nombreuses études théoriques et expérimentales sont actuellement en cours pour caractériser ou mesurer ces lois de réflexion (vorlander2000,cox2004). Toutefois, la pratique courante consiste à utiliser une loi de Lambert pour décrire une réflexion diffuse. La valeur du coefficient de diffraction :math:`s` peut quant à elle être obtenue par une procédure de mesure standardisée (norme ISO 17497-1 de 2004 (ISO17497-1:2004)). [7]_

|image|

Modélisation de la réflexion acoustique
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modélisation « aléatoire »
''''''''''''''''''''''''''

Dans un premier temps, une particule sonore entrant en collision avec une paroi pourra être soit absorbée par la paroi (avec une probabilité :math:`\alpha`), soit réfléchie dans une nouvelle direction de propagation (avec une probabilité :math:`R=1-\alpha`). En pratique, le choix absorption/réflexion est réalisé en tirant aléatoirement un nombre :math:`u` entre :math:`0` et :math:`1`, suivant une distribution uniforme. Si ce nombre est inférieur à :math:`\alpha=1-R` (au point considéré), la particule est absorbée et disparaît du milieu de propagation. Si ce nombre est supérieur à :math:`\alpha=1-R`, la particule est réfléchie et poursuit sa propagation dans une nouvelle direction de l’espace.

Dans un second temps, pour déterminer le type de réflexion (spéculaire ou diffuse), un nouveau tirage aléatoire :math:`v` est réalisé entre :math:`0` et :math:`1`. Si ce nombre est inférieur à la valeur de :math:`1-s` (**i.e.** :math:`v<(1-s)`) au point considéré, la particule est réfléchie spéculairement, conformément aux lois de Snell-Descartes bien connues. Dans le cas contraire (**i.e.** :math:`v>(1-s)`), la réflexion est diffuse. Dans ce dernier cas, il est faut déterminer la direction de réflexion diffuse (*cf.* paragraphe [sec:mod\_reflexion]).

Modélisation « énergétique »
''''''''''''''''''''''''''''

Lorsqu’une particule entre en collision avec une paroi, son énergie :math:`\epsilon` est pondérée par le coefficient de réflexion :math:`R=1-\alpha`. Le choix d’une réflexion spéculaire ou diffuse, en fonction du coefficient de diffraction, est identique à la méthode « aléatoire »: un tirage aléatoire :math:`v` est réalisé entre :math:`0` et :math:`1`. Si ce nombre est inférieur à la valeur de :math:`1-s`, la particule est réfléchie spéculairement. Dans le cas contraire, la réflexion est diffuse, dans une direction à déterminer (*cf.* paragraphe [sec:mod\_reflexion]). Un traitement entièrement « énergétique » serait possible en dupliquant la particule en deux particules, la première étant réfléchie dans la direction spéculaire et la seconde dans la direction diffuse (*cf.* remarque à la page ).

Modélisation des lois de réflexion
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Formalisme
^^^^^^^^^^

.. figure:: geo_reflexion.eps
   :alt: [geo\_reflexion]Géométrie élémentaire d’une réflexion par une paroi, en 3D: :math:`\phi` et :math:`\phi'` désignent respectivement les angles d’incidences et de réflexion par rapport à la normale à la paroi. Les angles :math:`\theta` et :math:`\theta'` dans le plan tangent, des directions d’incidence et de réflexion, ne sont pas représentés (:math:`\phi,\phi'\in[0,\pi/2]` et :math:`\theta,\theta'\in[0,2\pi]`).
   :width: 7.50000cm

   [geo\_reflexion]Géométrie élémentaire d’une réflexion par une paroi, en 3D: :math:`\phi` et :math:`\phi'` désignent respectivement les angles d’incidences et de réflexion par rapport à la normale à la paroi. Les angles :math:`\theta` et :math:`\theta'` dans le plan tangent, des directions d’incidence et de réflexion, ne sont pas représentés (:math:`\phi,\phi'\in[0,\pi/2]` et :math:`\theta,\theta'\in[0,2\pi]`).

Considérons une particule sonore incidente sur une paroi, dont la direction incidente est définie par les coordonnées sphériques :math:`(\theta,\phi)` (figure [geo\_reflexion]). Cette particule a une probabilité :math:`P(\theta,\phi;\theta',\phi')\equiv P(\Omega,\Omega')` d’être réfléchie dans l’angle solide élémentaire :math:`d\Omega'=\sin\phi'\,d\phi'\,d\theta'` :raw-latex:`\cite{joyce1974,joyce1975,joyce1978,lepolles2003}`. :math:`P(\Omega,\Omega')\,d\Omega'` représente en fait la fraction de l’intensité sonore incidente qui est réfléchie dans l’angle solide :math:`d\Omega'`. Soit :math:`j(\theta,\phi)` le flux incident de particules. Le flux de particules réfléchies :math:`j'(\theta',\phi')` a pour expression:

.. math::

   \label{eq_flux_1} j'(\theta',\phi') \cos \phi'=\int
               P(\theta,\phi;\theta',\phi')\, j(\theta,\phi) \cos \phi \,d\Omega.

En définissant la loi de réflexion :math:`R(\theta,\phi;\theta',\phi')\equiv R(\Omega,\Omega')` de la manière suivante

.. math:: R(\theta,\phi;\theta',\phi')=\frac{P(\theta,\phi;\theta',\phi')}{\cos(\phi')},

la relation ([eq\_flux\_1b]) s’écrit encore

.. math::

   \label{eq_flux_1b} j'(\theta',\phi') =\int
               R(\theta,\phi;\theta',\phi')\, j(\theta,\phi) \cos \phi \,d\Omega.

Il est important de souligner la différence entre la loi de réflexion :math:`R` et la probabilité :math:`P`. :math:`P` désigne une probabilité de réflexion d’une particule sonore par angle solide élémentaire, tandis que :math:`R` désigne le flux d’énergie réfléchie dans une direction :math:`\phi'`, pour un flux incident dans la direction :math:`\phi`.

La première loi de la thermodynamique imposent la conservation du flux sur la surface (en l’absence d’absorption). La loi de réflexion :math:`R(\Omega,\Omega')` doit donc vérifier la condition suivante:

.. math::

   \label{loi_2} \int P(\Omega,\Omega') \,d\Omega'=\int
               R(\Omega,\Omega')\cos\phi' \,d\Omega'=1,

ou encore en coordonnées sphériques,

.. math::

   \label{loi_2b} \iint R(\theta,\phi;\theta',\phi')\cos\phi'
               \,\sin\phi'\,d\phi'\,d\theta'=1.

La seconde loi de la thermodynamique impose cette fois:

.. math::

   \label{loi_1} \int P(\Omega,\Omega') \,d\Omega=\int
               R(\Omega,\Omega')\cos\phi \,d\Omega=1,

ou encore en coordonnées sphériques

.. math::

   \label{loi_1b} \iint R(\theta,\phi;\theta',\phi')\cos\phi
               \,\sin\phi\,d\phi\,d\theta=1.

D’autre part, le principe de réciprocité impose que

.. math:: \label{loi_3} R(\Omega,\Omega')=R(\Omega',\Omega).

Malgré quelques études récentes, et hormis la loi de Lambert largement utilisée pour modéliser des réflexions diffuses :raw-latex:`\cite{kuttruff2000}`, il n’existe pas à notre connaissance d’autres lois de réflexion pour des parois réelles. Dans le code **SPPS**, nous proposons toutefois des modes de réflexion arbitraires, plus ou moins réels, présentés à la figure [fig\_ref] et détaillées ci-dessous.

Réflexion spéculaire
^^^^^^^^^^^^^^^^^^^^

Description physique
''''''''''''''''''''

Le mode de réflexion le plus simple est défini par la réflexion spéculaire (figure [fig\_ref\_specul]), et peut s’écrire en trois dimensions:

.. math::

   R(\theta,\phi;\theta',\phi')=2 \delta(\theta-\theta'\pm \pi)
               \,\delta(\sin^2 \phi-\sin^2 \phi'),

où les couples :math:`(\theta,\phi)` et :math:`(\theta',\phi')` désignent respectivement les directions incidentes et réfléchies des particules sonores sur la paroi. Bien que la forme de cette expression ne soit pas conventionnelle, cette relation vérifie les lois de Snell-Descartes, ainsi que les conditions ([loi\_1]) à ([loi\_3]).

Modélisation
''''''''''''

D’un point de vue numérique, la simulation de cette loi de réflexion ne pose pas de problème majeur, puisque les angles d’incidence de chaque particule sur une paroi sont connus. La modélisation est identique pour les approches « aléatoire » et « énergétique ».

Réflexion uniforme
^^^^^^^^^^^^^^^^^^

Description physique
''''''''''''''''''''

Une loi de réflexion uniforme produit une distribution des angles de réflexion :math:`P(\Omega')` est équiprobable. Il ne faut pas confondre avec la loi de de réflexion de Lambert, pour lequel l’uniformité est vérifiée par la loi de réflexion. Dans le cas d’une loi uniforme, les particules sont réfléchies uniformément dans tout le demi-espace, indépendamment de l’angle d’incidence. Dans ces conditions, la densité de réflexion après normalisation s’écrit:

.. math:: P(\Omega')d\Omega'= P(\theta',\phi')d\Omega'=\left[\frac{1}{2\pi}\,d\theta'\right] \,\left[\sin \phi' \,d\phi'\right],

ou encore

.. math:: R(\theta',\phi')=\frac{1}{2\pi\cos\phi'},

 pour :math:`\theta'\in[0,2\pi]` et :math:`\phi'\in[0,\pi/2]`.

Modélisation
''''''''''''

D’un point de vue numérique, la détermination de l’angle de réflexion est obtenue, là encore, par la méthode de la fonction de distribution cumulative inverse,

.. math:: f(\hat{\phi})= \int_{0}^{\hat{\phi}} \sin\phi'\,d\phi'=1-\cos\hat{\phi},

ce qui donne

.. math:: \hat{\phi}=\cos^{-1}\left(1-u\right),

où :math:`u` est un nombre aléatoire compris entre :math:`0` et :math:`1`.

Réflexion Lambert
^^^^^^^^^^^^^^^^^

Description physique
''''''''''''''''''''

Au sens de la lumière, une surface parfaitement diffuse est une surface qui parait aussi lumineuse quelque soit l’angle d’observation, et quelque soit l’angle d’incidence. Au sens de l’acoustique, une surface parfaitement diffuse réfléchira la même énergie dans toutes les directions quelque soit l’angle d’incidence. D’un point de vue mathématique, cette condition impose que la loi de réflexion :math:`R` soit indépendante de la direction de réflexion, donc de la direction d’incidence (par réciprocité). Après normalisation la loi de réflexion s’écrit:

.. math:: R(\theta,\phi;\theta',\phi')=\frac{1}{2\pi}\times 2.

| Le coefficient :math:`1/2\pi` est lié à la normalisation suivant l’angle :math:`\theta'` (distribution uniforme entre :math:`0` et :math:`2\pi`). Le second coefficient (facteur :math:`2`) est lié à la normalisation suivant l’angle :math:`\phi'`. La probabilité :math:`P(\Omega')\, d\Omega'` se réduit donc à

  .. math::

     \begin{aligned}
                 P(\Omega') \, d\Omega' & = & R\,\cos\phi'\,\sin\phi'\,d\theta'\,d\phi'\\
                 & =& \left[\frac{1}{2\pi}\,d\theta'\right]\, \left[2 \cos\phi'\,\sin\phi'\,d\phi'\right],
                 \end{aligned}

  où l’expression :math:`\cos\phi'` est relative à la loi de Lambert. Il est aisé de vérifier que :math:`R` vérifie les conditions ([loi\_1]) à ([loi\_3]). Il est important de souligner la différence entre une surface aléatoire et une surface « uniforme ». La première est conditionnée par une loi de réflexion :math:`R` aléatoire uniforme, tandis que la seconde (cf. le paragraphe suivant) est définie par une probabilité :math:`P` uniforme.

Modélisation
''''''''''''

D’un point de vue numérique, le tirage aléatoire des angles de réflexion (en trois dimensions) doit être réalisé en accord avec la distribution :math:`P(\Omega')`, à savoir :math:`P(\phi')` dans notre cas. Appliquant la méthode de la fonction de distribution cumulative inverse, la probabilité :math:`f(\hat{\phi})` qu’une particule sonore soit réfléchie dans un angle :math:`\phi'` compris entre :math:`0` et :math:`\hat{\phi}` est donnée par la relation suivante [8]_:

.. math:: f(\hat{\phi})=2\int_{0}^{\hat{\phi}} \cos\phi'\sin\phi'\,d\phi'=\sin^2 \hat{\phi}.

Cette probabilité étant comprise entre :math:`0` et :math:`1`, le choix de l’angle :math:`\hat{\phi}` est réalisé en tirant aléatoirement un nombre :math:`u\in[0,1]`, tel que:

.. math:: \hat{\phi}=\sin^{-1}\sqrt{u}=\cos^{-1}\left(1-u\right)^\frac{1}{2}.

Réflexion normale en :math:`w^n`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Description physique
''''''''''''''''''''

| Considérons une loi de réflexion indépendante de la direction incidente et définie uniquement par l’angle de réflexion :math:`\phi'` autour de la normale à la paroi (:math:`\theta'` étant uniforme entre :math:`0` et :math:`2\pi`), de la forme:

  .. math::

     \begin{aligned}
                 R(\Omega')&=&R(\theta',\phi')\\
                 &=&\frac{1}{2\pi}\times (n+1)\cos^{n-1}\phi'\\
                 &=&\frac{n+1}{2\pi}\,w^{n-1}.
                 \end{aligned}

  où :math:`n` est un entier positif. La quantité notée :math:`w=\cos\phi'` n’est autre que la projection de la direction de réflexion sur la normale à la paroi. Conformément aux notations de ce document, la probabilité :math:`P` aura donc pour expression:

  .. math::

     \begin{aligned}
                 P(\Omega')\,d\Omega'&=&P(\phi')\,\sin\phi'\,d\theta'\,d\phi'\\
                 &=&\left[\frac{1}{2\pi}d\theta'\right]\\
                 & & \quad\times
                 \left[(n+1)\cos^{n}\phi'\,d\phi'\right]\\
                 &=&\frac{n+1}{2\pi}\,w^{n}\,d\theta'\,d\phi'
                 \end{aligned}

ce qui justifie l’appellation en *loi en :math:`w^n`*. Cette forme de réflexion est identique à celle introduite dans (lepolles2003)`. On peut remarquer que ce type de loi est une forme généralisée de la loi de Lambert (:math:`n=1`) et de la loi uniforme (:math:`n=0`).

Modélisation
''''''''''''

D’un point de vue numérique, le tirage aléatoire des angles de réflexion est réalisé en appliquant la méthode de la fonction de distribution cumulative inverse. La probabilité :math:`f(\hat{\Omega})` (c’est-à-dire la probabilité :math:`f(\hat{\phi})` puisque la direction de réflexion ne dépend que l’angle par rapport à la normale) qu’une particule sonore soit réfléchie dans un angle solide élémentaire compris entre :math:`0` et :math:`\hat{\Omega}` est alors donnée par la relation

.. math:: f(\hat{\phi})= (n+1)\int_{0}^{\hat{\phi}} \cos^n\phi'\sin\phi'\,d\phi'=1-\cos^{n+1}\hat{\phi}.

Cette probabilité étant comprise entre :math:`0` et :math:`1`, le choix de l’angle :math:`\hat{\phi}` est réalisé en tirant aléatoirement un nombre :math:`u\in[0,1]`, tel que:

.. math:: \hat{\phi}=\cos^{-1}\left(1-u\right)^\frac{1}{n+1}.

Modélisation de la transmission acoustique
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Description physique
''''''''''''''''''''

Comme nous l’avons indiqué plus haut, la partie de la puissance qui n’est pas réfléchie par la paroi est soit dissipée dans la paroi, soit transmise. La transmission acoustique est alors définie par le facteur de transmission :math:`\tau`, défini comme le rapport de la puissance transmise :math:`W_t` par la paroi sur la puissance incidente :math:`W_i`. S’il n’y a pas de dissipation dans la paroi (**i.e.** tout est transmis à travers la paroi), alors :math:`\tau=\alpha`. Dans le cas contraire, :math:`\tau<\alpha`. En pratique, la transmission acoustique est plutôt définie par l’indice d’affaiblissement de la paroi, qui est fonction du facteur de transmission à travers la relation suivante:

.. math:: R=10\,\log\left(\frac{W_i}{W_t}\right)=-10\,\log{\tau}.

Modélisation « aléatoire »
''''''''''''''''''''''''''

La modélisation est similaire à celle utilisée pour la réflexion acoustique. Pour déterminer si la particule sonore est dissipée ou transmise par la cloison, il est nécessaire de tirer un nombre :math:`w` entre :math:`0` et :math:`\alpha`. Si ce nombre est inférieur à :math:`\tau`, la particule est transmise et conserve sa direction de propagation. Dans le cas contraire la particule est dissipée et disparaît du milieu de propagation.

Modélisation « énergétique »
''''''''''''''''''''''''''''

La modélisation énergétique est réalisée pondérant l’énergie de la particule une fois transmise par la cloison, par le coefficient :math:`\tau`.

Vérification de la modélisation des parois
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La figure [figure\_test\_reflexion] illustre le résultat de la procédure de réflexion (réflexion=spécualire/diffus, absorption=disspation/transmission) en mode « aléatoire », avec les paramètres acoustiques suivants: coefficient de diffraction :math:`s=0.6`, coefficient d’absorption :math:`\alpha=0.8`, coefficient de transmission :math:`\tau=10^{-R/10}=0.1` (indice d’affaiblissement :math:`R=10` dB). Avec :math:`10000` réalisations, les différents phénomènes sont retrouvés (en termes de nombre de réalisations) avec les même proportions que les paramètres acoustiques imposés.

|image|

Calcul des niveaux sonores aux points d’observation
---------------------------------------------------

Dans le code **SPPS** deux types de récepteurs sont considérés:

	-  les récepteurs « volumiques » modélisent les récepteurs « classiques » dits ponctuels. La notion de « point récepteur » n’étant pas applicable dans le code **SPPS**, puisqu’en théorie la probabilité qu’une particule sonore traverse un récepteur ponctuel est nul, il est nécessaire de donner un volume au point récepteur, pour comptabiliser le nombre de particules l’ayant traversé, et en déduire ainsi la densité d’énergie au point d’observation. Pour un récepteur ponctuel, le code **SPPS** retourne le niveau de pression acoustique, à chaque pas de temps du calcul et pour chaque bande de fréquence considérée;

	-  les récepteurs « surfaciques » sont des éléments de surface (en principe des faces de la scène) sur lesquels sont calculés les intensités sonores incidentes, ce qui permet ensuite de construire des cartographies acoustiques. Pour un récepteur surfacique, le code **SPPS** retourne le niveau d’intensité sonore, à chaque pas de temps du calcul et pour chaque bande de fréquence considérée.

En parallèle, le code de calcul détermine également le niveau de pression acoustique global dans le modèle, en sommant les contributions de chaque particule, à chaque pas de temps et pour chaque bande de fréquence.

Calcul du niveau de pression acoustique au niveau d’un récepteur « volumique »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La quantité d’\ **énergie** :math:`E_\text{rec}^{j}(n)` (en J ou W.s) dans la bande de fréquence :math:`j` reçue au niveau d’un récepteur volumique, au pas de temps :math:`n` (*i.e.* au temps :math:`n \Delta t`) est égale à la somme des énergies :math:`\varepsilon_i^{j}` apportées par chaque particule :math:`i` dans la bande de fréquence :math:`j`, traversant le volume récepteur au cours du pas de temps :math:`n` (figure [principe\_recepteur\_volumique]) :

.. math:: E_\text{rec}^{j}(n)=\sum_i^{N_0} \varepsilon_i^{j} = \sum_i^{N_0} \frac{W}{N} \epsilon_i^{j} \times \Delta t_i,

où :math:`{N_0}` est le nombre total de particules traversant le volume récepteur et :math:`\Delta t_i` le temps de présence de la particule :math:`i` dans le volume récepteur (:math:`\Delta t_i<\Delta t`), et :math:`\epsilon_i^{j}` le coefficient de pondération (compris entre 0 et 1) associé à la particule :math:`i` dans la bande de fréquence :math:`j`. Si le mode de calcul est « aléatoire », le coefficient :math:`\epsilon_i^{j}` est constant et égale à l’unité (1). Si le mode de calcul est « énergétique », le coefficient :math:`\epsilon_i^{j}` traduit la perte cumulée d’énergie dans la bande de fréquence :math:`j` de la particule :math:`i` tout au long de son trajet du fait des phénomènes physiques rencontrés (absorption par les parois et par les encombrements, absorption atmosphérique, transmission acoustique…). Le temps de présence :math:`\Delta t_i` peut également être exprimé en fonction de la longueur du trajet de la particule :math:`i` dans le volume récepteur, soit :math:`\ell_i`, tel que :math:`\Delta t_i=\ell_i/c`, :math:`c` étant la vitesse de la particule au niveau du point d’observation. En considérant un récepteur « volumique » [9]_ est défini par un volume sphérique de rayon :math:`r_\text{rec}` (et de volume :math:`V_\text{rec}`), la **densité d’énergie** :math:`w_\text{rec}^{j}(n)` (en J/m\ :math:`^3`) dans le volume récepteur, pour la bande de fréquence :math:`j`, est donné par:

.. math::

   \label{densite_energie_recepteur_volumique}
           w_\text{rec}^{j}(n)=\frac{E_\text{rec}(n)}{V_\text{rec}}= \frac{W}{N} \frac{1}{V_\text{rec}} \sum_i^{N_0} \epsilon_i^{j} \frac{\ell_i}{c}.

.. figure:: principe_recepteur_volumique.eps
   :alt: [principe\_recepteur\_volumique]Principe du calcul du niveau de pression acoustique pour un récepteur « volumique ». La densité d’énergie dans le volume récepteur est calculée en sommant les contributions énergétiques de chaque particule traversant le récepteur. La contribution de la particule :math:`i` est calculée à partir du trajet :math:`\ell_i` de la particule dans le volume récepteur (relation ([densite\_energie\_recepteur\_volumique])).
   :width: 8.00000cm

   [principe\_recepteur\_volumique]Principe du calcul du niveau de pression acoustique pour un récepteur « volumique ». La densité d’énergie dans le volume récepteur est calculée en sommant les contributions énergétiques de chaque particule traversant le récepteur. La contribution de la particule :math:`i` est calculée à partir du trajet :math:`\ell_i` de la particule dans le volume récepteur (relation ([densite\_energie\_recepteur\_volumique])).

L’\ **intensité sonore** :math:`I_\text{rec}^{j}(n)` (en W/m\ :math:`^2`) au point récepteur est quant à elle donnée par:

.. math:: I_\text{rec}^{j}(n)=c \times w^{j}_\text{rec}(n)= \frac{W}{N} \frac{1}{V_\text{rec}} \sum_i^{N_0} \epsilon_i^{j}\ell_i.

Le **niveau d’intensité sonore** :math:`L_\text{I,rec}^{j}(n)` et le **niveau de pression acoustique** :math:`SPL_\text{rec}(n)` (en dB) peuvent ensuite être déduits de l’intensité sonore par les relations suivantes:

.. math::

   \label{LI_recepteur_volumique}
           L_\text{I,rec}^{j}(n)=10\log\left(\frac{P2_\text{rec}^{j}(n)}{\rho_0 c I_0}\right),

et

.. math::

   \label{SPL_recepteur_volumique}
           SPL_\text{rec}^{j}(n)=10\log\left( \frac{P2_\text{rec}^{j}(n)}{p_0^2}\right),

où :math:`I_0=10^{-12}` W/m:math:`^2` et :math:`p_0=20 \times 10^{-6}` Pa désignent l’intensité sonore et la pression acoustique de référence. Chacune des quantités précédentes (énergie, densité d’énergie, intensité et niveaux sonores) sont calculées pour chaque bande de fréquence.

Vecteur intensité au niveau d’un récepteur « volumique »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le vecteur **intensité sonore** :math:`\mathbf{I}_\text{rec}^{j}(n)` (en W/m\ :math:`^2` ou J/m\ :math:`^2`.s) au point récepteur, pour la bande de fréquence :math:`j`, est défini comme la somme des densités d’énergie portées par le vecteur vitesse :math:`\mathbf{v}_i` des particules (de norme :math:`c_i`) traversant le volume récepteur:

.. math:: \textbf{I}_\text{rec}^{j}(n)= \frac{W}{N} \frac{1}{V_\text{rec}} \sum_i^{N_0} \epsilon_i^{j}\ell_i\frac{\mathbf{v}_i}{c_i}.

Calcul du niveau de pression acoustique « latéral » au niveau d’un récepteur « volumique »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour le calcul de certains paramètres acoustiques, tels que ceux basés sur l’énergie latérale (LF avec une pondération en :math:`|\cos\theta|` et LFC avec une pondération en :math:`\cos^2\theta`), il est nécessaire de considérer une pondération de l’intensité sonore en fonction de l’angle :math:`\theta` entre la direction d’observation du récepteur ponctuel (en principe orienté vers la source sonore) et de la direction incidente des particules au niveau du récepteur. De ce fait, le code de calcul **SPPS** retourne également les deux quantités suivantes, homogène au carré de la pression acoustique (*i.e.* en Pa\ :math:`^2`):

.. math::

   \begin{aligned}
           P2_{\text{rec},\cos\theta}^{j}(n)&=& \rho_0 c^2 \times w^{j}_{\text{rec},\cos\theta}(n)\\
           &=& \rho_0 c \frac{W}{N} \frac{1}{V_\text{rec}}\\
           & & \quad \times \sum_i^{N_0} \epsilon_i^{j}\ell_i|\cos\theta_i|,
           \end{aligned}

et

.. math::

   \begin{aligned}
            P2_{\text{rec},\cos^2\theta}^{j}(n)&=& \rho_0 c^2 \times w^{j}_{\text{rec},\cos^2\theta}\\
            &=& \rho_0 c \frac{W}{N} \frac{1}{V_\text{rec}}\\
            & & \quad \times \sum_i^{N_0} \epsilon_i^{j}\ell_i\cos^2\theta_i.
           \end{aligned}

où :math:`\theta_i` est l’angle correspondant pour la particule :math:`i`.

Calcul du niveau d’intensité sonore sur un récepteur « surfacique »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La **puissance** :math:`W_\text{surf}^{j}` (en W) reçue par un élément de surface de taille :math:`\Delta S` de normal :math:`\mathbf{n}` est égale à la somme de l’énergie apportée par chaque particule :math:`i` dans la bande de fréquence :math:`j` par unité de temps :math:`\Delta t`, au pas de temps :math:`n` (*i.e.* au temps :math:`n\Delta t`), soit:

.. math:: W_\text{surf}^{j}(n)=\sum_i^{N_0} \frac{\varepsilon_i^{j}}{\Delta t} \frac{\mathbf{v_i}}{c}\cdot \mathbf{n}=\frac{W}{N} \sum_i^{N_0}  \epsilon_i\cos\theta_i,

où :math:`\mathbf{v_i}` désigne le vecteur vitesse (de norme :math:`c`) de la particule :math:`i`, :math:`\theta_i` l’angle entre la normale :math:`\mathbf{n}` de la surface et la direction de la particule, et où :math:`{N_0}` est le nombre total de particules sonores entrant en collision avec l’élément de surface :math:`\Delta S`.

L’\ **intensité sonore** :math:`I_\text{surf}^{j}(n)` (en W/m\ :math:`^2`) reçue par l’élément de surface :math:`\Delta S` au pas de temps :math:`n` est égale à la puissance reçue divisée par la surface, soit:

.. math:: I_\text{surf}^{j}(n)=\frac{W}{N} \frac{1}{\Delta S}\sum_i^{N_0} \epsilon_i^{j} \cos\theta_i.

Le **niveau d’intensité sonore** :math:`L_\text{surf}^{j}(n)` (en dB) peut ensuite être calculé par la relation suivante:

.. math::

   \label{L_recepteur_surfacique}
           L_\text{surf}^{j}(n)= 10\log\frac{I_\text{surf}^{j}(n)}{I_0},

où :math:`I_0=10^{-12}` W/m:math:`^2` désigne l’intensité de référence.

.. figure:: principe_recepteur_surfacique.eps
   :alt: [principe\_recepteur\_surfacique]Principe du calcul du niveau d’intensité acoustique au niveau d’un récepteur « surfacique ».
   :width: 8.00000cm

   [principe\_recepteur\_surfacique]Principe du calcul du niveau d’intensité acoustique au niveau d’un récepteur « surfacique ».

Calcul du niveau de pression acoustique sur un récepteur « surfacique »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

De façon équivalente au calcul du niveau d’intensité sonore sur un récepteur « surfacique », la pression acoustique quadratique :math:`P2_\text{surf}^{j}(n)` (en Pa) reçue par l’élément de surface :math:`\Delta S` au pas de temps :math:`n` est égale au produit de l’intensité sonore (sans pondération de l’angle d’incidence sur la surface) par l’impédance caractéristique de l’air :math:`\rho_0\,c`, soit:

.. math:: P2_\text{surf}^{j}(n)=\rho_0\,c \frac{W}{N} \frac{1}{\Delta S}\sum_i^{N_0} \epsilon_i^{j}.

Le **niveau de pression acoustique** :math:`L_\text{SPL,surf}^{j}(n)` (en dB) peut ensuite être calculé par la relation suivante:

.. math::

   \label{SPL_recepteur_surfacique}
           L_\text{SPL,surf}^{j}(n)= 10\log\frac{P2_\text{surf}^{j}(n)}{p_0^2},

où :math:`p_0=20\times 10^{-6}` Pa désigne la pression acoustique de référence.

Calcul du niveau de pression acoustique global dans le modèle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L’intensité acoustique globale :math:`I_\text{global}^{j}(n)` dans le modèle, pour la bande de fréquence :math:`j`, est calculée en sommant les intensités portées par toutes les particules sonores présentes :math:`N_n` dans le modèle au pas de temps :math:`n` (*i.e.* au temps :math:`n\Delta t`):

.. math:: I_\text{global}^{j}(n)= \frac{W}{N} \frac{1}{V_\text{rec}} \sum_i^{N_n} \epsilon_i^{j}\ell_i.

Le **niveau de pression acoustique** :math:`SPL_\text{global}^{j}(n)` (en dB) peut ensuite être calculé par la relation suivante:

.. math::

   \label{SPL_global}
           SPL_\text{global}^{j}(n)=10\log\left( \frac{P2_\text{global}^{j}(n)}{p_0^2}\right),

où :math:`p_0=20\times 10^{-6}` Pa désigne la pression acoustique de référence.

.. [1]
   La méthode consiste à considérer des angles d’emission :math:`\theta` et :math:`\phi` par pas constant.

.. [2]
   Le champ libre est simulé en considérant un long couloir avec des limites parfaitement absorbantes.

.. [3]
   Pour des raisons de simplicité, nous considérons la fréquence normale des bandes de fréquence (ISO266:1997), et non la fréquence centrale exacte.

.. [4]
   La simulation est identique à celle présentée au paragraphe [sec:propagation\_acoustique] pour la vérification du respect de la dispersion géométrique.

.. [5]
   Comme pour une source omnidirectionnelle, *cf.* paragraphe [sec:source\_omni].

.. [6]
   Il ne peut pas y avoir de création d’énergie, ni plus d’absorption que d’énergie incidente.

.. [7]
   Il est important de signaler que ce coefficient de diffraction est différent du coefficient de diffusion :math:`\delta` (*diffusion coefficient* en anglais) proposé par l’, et qui définit quant à lui la « qualité de diffusion » d’une surface en comparant la réponse polaire (loi de réflexion) de la surface en question à une distribution uniforme. La mesure de ce coefficient fait actuellement l’objet de la rédaction de la partie 2 de la norme ISO 17497). Dans certains logiciels de simulation acoustique, le coefficient de diffraction :math:`s` est parfois nommé à tord, coefficient de diffusion.

.. [8]
   Il est important de noter que d’autres auteurs (burns1990,hodgson1991,lam1996,xiangyang2002) mentionnent d’autres relations. La bonne formulation est fonction de la convention des angles. Il est également possible que certaines relations qui sont proposées ne soient pas exactes.

.. [9]
   La définition du volume récepteur est un élément crucial pour la qualité et la représentativité des résultats. Il doit être suffisamment grand pour comptabiliser des particules sonores lors de leur propagation, mais pas trop pour que la densité d’énergie calculée au point d’observation soit représentative.

.. |image| image:: illustration_dispersion_geometrique.eps
   :width: 14.00000cm
.. |image| image:: organigramme_att_atmos.eps
.. |image| image:: geo_profil_celerite.eps
   :width: 12.00000cm
.. |image| image:: organigramme_refraction.eps
.. |image| image:: verification_diffusion_encombrement.eps
   :width: 12.00000cm
.. |image| image:: organigramme_encombrement.eps
.. |image| image:: organigramme_reflexion.eps
.. |image| image:: figure_test_reflexion.eps
   :width: 12.00000cm
