import libsimpa as ls

model=ls.ioModel()
ls.CformatBIN().ImportBIN(model,"modeldebug.cbin")
merger=ls.SurfaceMerging()
merger.LoadGroups(model)
print "Il y a %i groupes de surfaces)" % (merger.GetGroups())
for igroup in range(0,merger.GetGroups()):
    faces=list(merger.GetGroupInformation(igroup))
    print "Groupe %i" % (igroup)
    print faces
    print "\n"
