## Copyright [C] 2017 Christian Prax
## Institut PPRIME, CNRS - Universite de Poitiers  ENSMA, UPR 3346
## Ecole Nationale Superieure d'ingenieurs de Poitiers, ENSIP
##
## This file is part of MD [Room Acoustics Diffusion Model].
##
## MD is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public
## License as published by the Free Software Foundation;
## either version 3 of the License, or [at your option] any
## later version.
##
## MD is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public
## License along with Octave; see the file COPYING.  If not,
## see <http://www.gnu.org/licenses/>.
##
## Author: Christian Prax <Christian.prax@univ-poitiers.fr>
## Keywords: Room acoustics, diffusion model
## Adapted-By: Ifsttar <I-Simpa@ifsttar.fr>
#
# Version Python EF IV 2020
#
#
################### Python ####################################
import numpy as np


# ___________________________________
def line3d(xn, yn, zn):  # j'ai gardé les indices des tableaux comme sous Octave (ie commencent à 1)
    # usage: a,b,c,d,v=line3d(xn,yn,zn)
    # Calculation of the lnear interpolation coefficients [a,b,c,d] of a variable P
    # in a 3D-space
    #
    # In:
    #	- xn,yn,zn : Coordinates of the nodes
    #
    # Out:
    #	- a,b,c,d : Linear interpolation coefficients
    #	- v : Volume of a tetreahedric mesh
    x21 = xn[2] - xn[1];
    x31 = xn[3] - xn[1];
    x41 = xn[4] - xn[1];
    y21 = yn[2] - yn[1];
    y31 = yn[3] - yn[1];
    y41 = yn[4] - yn[1];
    z21 = zn[2] - zn[1];
    z31 = zn[3] - zn[1];
    z41 = zn[4] - zn[1];

    z43 = zn[4] - zn[3];
    z14 = zn[1] - zn[4];
    z32 = zn[3] - zn[2];
    z24 = zn[2] - zn[4];
    z42 = zn[4] - zn[2];
    z13 = zn[1] - zn[3];
    z34 = zn[3] - zn[4];
    z12 = zn[1] - zn[2];
    z23 = zn[2] - zn[3];

    y43 = yn[4] - yn[3];
    y14 = yn[1] - yn[4];
    y32 = yn[3] - yn[2];
    y24 = yn[2] - yn[4];
    y42 = yn[4] - yn[2];
    y13 = yn[1] - yn[3];
    #
    v = (x21 * (y31 * z41 - y41 * z31) - x31 * (y21 * z41 - y41 * z21) + x41 * (y21 * z31 - y31 * z21)) / 6.

    a = np.zeros(5)  # j'ai gardé les indices donc il n'y a rien à l'indice 0
    a[1] = (xn[2] * (yn[3] * zn[4] - yn[4] * zn[3]) + xn[3] * (yn[4] * zn[2] - yn[2] * zn[4]) \
            + xn[4] * (yn[2] * zn[3] - yn[3] * zn[2])) / (6. * v);
    a[2] = (-xn[3] * (yn[4] * zn[1] - yn[1] * zn[4]) - xn[4] * (yn[1] * zn[3] - yn[3] * zn[1]) \
            - xn[1] * (yn[3] * zn[4] - yn[4] * zn[3])) / (6. * v);

    a[3] = (xn[4] * (yn[1] * zn[2] - yn[2] * zn[1]) + xn[1] * (yn[2] * zn[4] - yn[4] * zn[2]) \
            + xn[2] * (yn[4] * zn[1] - yn[1] * zn[4])) / (6. * v);

    a[4] = (-xn[1] * (yn[2] * zn[3] - yn[3] * zn[2]) - xn[2] * (yn[3] * zn[1] - yn[1] * zn[3]) \
            - xn[3] * (yn[1] * zn[2] - yn[2] * zn[1])) / (6. * v);

    b = np.zeros(5)  # j'ai gardé les indices donc il n'y a rien à l'indice 0
    b[1] = [yn[2] * z43 + yn[3] * z24 + yn[4] * z32] / (6. * v);
    b[2] = [-yn[3] * z14 - yn[4] * z31 - yn[1] * z43] / (6. * v);
    b[3] = [yn[4] * z21 + yn[1] * z42 + yn[2] * z14] / (6. * v);
    b[4] = [-yn[1] * z32 - yn[2] * z13 - yn[3] * z21] / (6. * v);

    c = np.zeros(5)  # j'ai gardé les indices donc il n'y a rien à l'indice 0
    c[1] = [xn[2] * z34 + xn[3] * z42 + xn[4] * z23] / (6. * v);
    c[2] = [-xn[3] * z41 - xn[4] * z13 - xn[1] * z34] / (6. * v);
    c[3] = [xn[4] * z12 + xn[1] * z24 + xn[2] * z41] / (6. * v);
    c[4] = [-xn[1] * z23 - xn[2] * z31 - xn[3] * z12] / (6. * v);

    d = np.zeros(5)  # j'ai gardé les indices donc il n'y a rien à l'indice 0
    d[1] = [xn[2] * y43 + xn[3] * y24 + xn[4] * y32] / (6. * v);
    d[2] = [-xn[3] * y14 - xn[4] * y31 - xn[1] * y43] / (6. * v);
    d[3] = [xn[4] * y21 + xn[1] * y42 + xn[2] * y14] / (6. * v);
    d[4] = [-xn[1] * y32 - xn[2] * y13 - xn[3] * y21] / (6. * v);

    return a, b, c, d, v