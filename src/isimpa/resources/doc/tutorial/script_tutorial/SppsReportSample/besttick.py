import math

def frange(limit1, limit2 = None, increment = 1.):
  """
  Range function that accepts floats (and integers).

  Usage:
  frange(-2, 2, 0.1)
  frange(10)
  frange(10, increment = 0.5)

  The returned value is an iterator.  Use list(frange) for a list.
  """

  if limit2 is None:
    limit2, limit1 = limit1, 0.
  else:
    limit1 = float(limit1)

  count = int(math.ceil((limit2 - limit1)/increment))+1
  return [limit1 + n*increment for n in range(count)]

##
# Methode issu de PlPlot (pldtik.c,v 1.15 2005/04/27)
#
def GetTicksFromMinMax( vmin, vmax):
    t1 = math.log10(abs(vmax - vmin))
    np = math.floor(t1)
    t1 = t1 - np
    if t1 > 0.7781512503:
        t2 = 2.0
        ns = 4
    elif t1 > 0.4771212549:
        t2 = 1.0
        ns = 5
    elif t1 > 0.1760912591:
        t2 = 5.0
        ns = 5
        np = np - 1
    else:
        t2 = 2.0
        ns = 4
        np = np - 1
    besttick=t2 * math.pow(10.0, np)
    first_tick=vmin-math.fmod(vmin, besttick);
    return frange(first_tick,vmax,besttick)

class AxesLabelsCreator:
    def __init__(self):
        self.min=None
        self.max=None
    def setmargin(self, percentage=5):
        diff=(self.max-self.min)*(percentage/100.)
        self.min-=diff
        self.max+=diff
    def feed(self, lst):
        if self.min is None:
            self.min=min(lst)
            self.max=max(lst)
        else:
            self.min=min([self.min,min(lst)])
            self.max=max([self.max,max(lst)])
    def GetTicksLabels(self,label_format="%f"):
        return [ label_format % (value) for value in self.GetTicksPosition()]
    def GetTicksPosition(self):
        return GetTicksFromMinMax(self.min,self.max)
            
