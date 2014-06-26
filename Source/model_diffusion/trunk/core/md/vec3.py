#vecteur 3d
from libsimpa import vec3
"""
class vec3(object):
    def __init__(self,vec_or_x=0,_y=0,_z=0):
        if hasattr(vec_or_x,'__len__') and len(vec_or_x)>=3:
            _x=vec_or_x[0]
            _y=vec_or_x[1]
            _z=vec_or_x[2]
        else:
            _x=vec_or_x
        self.x=float(_x)
        self.y=float(_y)
        self.z=float(_z)
    def __abs__(self):
        return vec3(abs(self.x),abs(self.y),abs(self.z))
    def __add__(self,addby):
        if hasattr(addby,'x') and hasattr(addby,'y') and hasattr(addby,'z'):
            return vec3(addby.x+self.x,addby.y+self.z,addby.z+self.z)
        else:
            return vec3(addby+self.x,addby+self.z,addby+self.z)
    def __sub__(self,subby):
        return self.__add__(-subby)
    def __rsub(self,left):
        return left+(-self)
    def __neg__(self):
        return vec3(-self.x,-self.y,-self.z)
    def length(self):
        return sqrt(self.x**2+self.y**2+self.z**2)
    def __len__(self):
        return 3
    def __getitem__(self,item):
        return [self.x,self.y,self.z][item]
    def __repr__(self):
        return "[%s,%s,%s]" % (self.x,self.y,self.z)
"""