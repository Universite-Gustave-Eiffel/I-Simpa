# -*- coding: utf-8 -*-
from libsimpa import *

# Add new functionalities

###########################################################
## Gabe_rw

def ToList(self):
    """
        Return data using python array
    """
    lstret=[]
    coltypes=list(self.GetTabTypes())
    func_binding={ GABE_OBJECTTYPE_SHORTSTRING : self.ReadColStr,
                   GABE_OBJECTTYPE_INT : self.ReadColInt,
                   GABE_OBJECTTYPE_FLOAT : self.ReadColFloat
                 }
    for idcol in range(0,len(self)):
        lstret.append([ self.GetColTitle(idcol)] + list(func_binding[coltypes[idcol]](idcol)))
    return lstret                    

setattr(Gabe_rw,"ToList",ToList)
