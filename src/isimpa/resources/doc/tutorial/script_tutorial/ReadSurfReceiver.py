import libsimpa

maxtime=0
dat=ls.rsurf_data()
ls.rsurf_io.Load("Niveau_Sonore.csbin",dat)
for rs in range(dat.GetRsCount()):
    nbfaces,rsname,xmlid=dat.GetRsInfo(rs)
    for idface in range(nbfaces):
        vertexA,vertexB,vertexC,recordCount=dat.GetFaceInfo(rs,idface)
        for record in range(recordCount):
            idstep,energy=dat.GetFaceEnergy(rs,idface,record)
            maxtime=max(maxtime,idstep)
print maxtime
