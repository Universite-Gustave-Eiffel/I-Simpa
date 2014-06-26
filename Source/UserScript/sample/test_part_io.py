# -*- coding: cp1252 -*-
#####################
# Ce script créé un fichier de particules qui forme une sinusoïde
#
#####################




import libsimpa as ls
from math import *

pwriter=ls.ParticuleIO()
pwriter.OpenForWrite("particules.pbin")
pwriter.SetHeaderData(.02)
nbpart=10000
nbtimestep=12
pospart=[[i/800.,2,1+pi/100.*(i/10.)] for i in range(0,nbpart)]

for part in pospart:
    pwriter.NewParticle(0) #pas de temps de départ n°0
    for i in range(0,nbtimestep):
        pwriter.NewPositionParticle(part[0],2,(sin(part[2]+(i*(pi/6)))+2),1.)
pwriter.Close()
