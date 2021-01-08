Calculation code characteristics
------------------------------------

.. list-table:: Physical phenomena
   :widths: 70 15 15
   :header-rows: 1

   * - Physical phenomena
     - `SPPS code`_
     - `TCR code`_
   * - Direct field
     - ✅
     - ✅
   * - Specular field (specular reflection)
     - ✅
     - ❌
   * - Diffuse field
     - ✅ (using scattering coefficient)
     - ✅ (Sabine, Eyring)
   * - Scattered field (edge scattering)
     - ❌
     - ❌
   * - Surface absorption
     - ✅ (using absorption coefficient)
     - ✅ (using absorption coefficient)
   * - Diffusion by fittings
     - ✅
     - ✅
   * - Atmospheric absorption
     - ✅
     - ✅
   * - Meteorological effects
     - ✅
     - ❌

.. list-table:: Receiver type and characteristics
   :widths: 70 15 15
   :header-rows: 1

   * - Type/characteristics
     - `SPPS code`_
     - `TCR code`_
   * - Punctual receiver
     - ✅
     - ✅
   * - Surface receiver (any face element of the 3D scene)
     - ✅
     - ✅
   * - Surface receiver (any defined plane receivers)
     - ✅
     - ✅

.. list-table:: Sound source type and characteristics
   :widths: 70 15 15
   :header-rows: 1

   * - Type/characteristics
     - `SPPS code`_
     - `TCR code`_
   * - Punctual sound source : omnidirectionnal
     - ✅
     - ✅
   * - Punctual sound source : complex directivity
     - ✅
     - ❌
   * - Surface sound source
     - ❌
     - ❌
   * - Line sound source
     - as a line of punctual sources
     - as a line of punctual sources

.. list-table:: Acoustical parameters
   :widths: 70 15 15
   :header-rows: 1

   * - Type/characteristics
     - `SPPS code`_
     - `TCR code`_
   * - Sound level
     - ✅ (dB, dBA)
     - ✅ (Sabine, Eyring, Direct field)
   * - Lateral Sound level
     - ✅ (cos, cos²)
     - ❌
   * - Clarity
     - ✅ (C50, C80, user defined)
     - ❌
   * - Definition
     - ✅ (D50, user defined)
     - ❌
   * - Ts
     - ✅
     - ❌
   * -  RT
     - ✅ (RT15, RT30, user defined)
     - ✅ (RT Sabine, RT Eyring)
   * - EDT
     - ✅
     - ✅
   * - ST
     - ✅
     - ❌
   * - LF
     - ✅ (LF80, user defined)
     - ❌
   * - LFC
     - ✅ (LFC80, user defined)
     - ❌
   * - LG
     - ❌
     - ❌
   * - G
     - ❌
     - ❌

.. list-table:: Graphical representations
   :widths: 70 15 15
   :header-rows: 1

   * - Type/characteristics
     - `SPPS code`_
     - `TCR code`_
   * - Echogramm
     - ✅
     - ❌
   * - Spectrum
     - ✅ (total, per sound source)
     - ❌
   * - Sound level map (on surface receivers) : instantaneous
     - ✅
     - ❌
   * - Sound level map (on surface receivers) : cumulative
     - ✅
     - ❌
   * - Sound level map (on surface receivers) : cumulated
     - ✅
     - ✅
   * - Acoustical parameters map (on surface receivers)
     - ✅
     - ❌
   * - Schroeder curves
     - ✅ (data to display)
     - ❌
   * - Intensity
     - ✅ (animation)
     - ❌

.. _`TCR code`: code_TCR.html
.. _`SPPS code`: code_SPPS.html
