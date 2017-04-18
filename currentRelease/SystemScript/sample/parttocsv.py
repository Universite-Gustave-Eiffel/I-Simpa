import libsimpa as ls
filepath="particules.pbin"
csvpath="particules.csv"
preader=ls.ParticuleIO()
preader.OpenForRead(filepath)
timeStep,nbParticules,nbTimeStepMax=preader.GetHeaderData()
fichcsv=file(csvpath,'w')
for idpart in range(0,nbParticules):
    firstTimeStep, NbTimeStep=preader.NextParticle()
    curstep=firstTimeStep*timeStep
    for idstep in range(0,NbTimeStep):
        curstep+=timeStep
        x,y,z,energy=preader.NextTimeStep()
        fichcsv.write("%f;%f;%f;%f;%f\n" % (curstep,x,y,z,energy))
    fichcsv.write("\n")
fichcsv.close()
