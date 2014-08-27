# -*- coding: cp1252 -*-
#vecteur 3d
from math import *

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
        if isinstance(addby,vec3):
            return vec3(addby.x+self.x,addby.y+self.y,addby.z+self.z)
        else:
            return vec3(addby+self.x,addby+self.y,addby+self.z)
    def __sub__(self,subby):
        return self.__add__(-subby)
    def __rsub__(self,left):
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
    def __div__(self,divby):
        if isinstance(divby,vec3):
            return vec3(self.x/divby.x,self.y/divby.y,self.z/divby.z)
        else:
            return vec3(self.x/divby,self.y/divby,self.z/divby)
    def __mul__(self,mulby):
        if isinstance(mulby,vec3):
            return vec3(self.x*mulby.x,self.y*mulby.y,self.z*mulby.z)
        else:
            return vec3(self.x*mulby,self.y*mulby,self.z*mulby)
    def __rmul__(self,mulby):
        return self.__mul__(mulby)
    def __eq__(self,othervec):
        if isinstance(othervec,vec3):
            return self.x==othervec.x and self.y==othervec.y and self.z==othervec.z
        else:
            return False
        
    def rotate(self,n,angle):
        """
        * Fonction pour le calcul du changement de coordonnées du vecteur
        * d'un angle (radians)
        *
        * @param[in] n Vecteur de rotation
        * @param[in] angle Angle en radians
        """
        m1=cos(angle)
        m2=1-m1
        m3=sin(angle)
        m2nx=m2*n.x
        return vec3( (m1 +   m2nx*n.x)*self.x + (m2nx*n.y -  m3*n.z)*self.y + (m2nx*n.z +  m3*n.y)*self.z,
                    (m2nx*n.y +  m3*n.z)*self.x + (m1 +   m2*n.y*n.y)*self.y + (m2*n.y*n.z -  m3*n.x)*self.z,
                    (m2nx*n.z -  m3*n.y)*self.x +(m2*n.y*n.z +  m3*n.x)*self.y + (m1 +   m2*n.z*n.z)*self.z)
    def dot(self,v):
        return self.x*v.x+self.y*v.y+self.z*v.z
    def projectionOnLine(self,vA,vB):
            v=vB-vA
            return v.dot(self-vA)/v.dot(v)
    def closestPointOnLine(self,vA,vB):
        return (((vB-vA) * self.projectionOnLine(vA, vB)) + vA)
    def closestPointOnSegment(self,vA,vB):
        factor = self.projectionOnLine(vA, vB)
        if factor <= 0.0:
            return vA
        if factor >= 1.0:
            return vB
        return (((vB-vA) * factor) + vA);
