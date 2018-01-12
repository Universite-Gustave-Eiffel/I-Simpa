# -*- coding: cp1252 -*-
from __future__ import print_function #compatibilité python 3.0
import xml.dom.minidom  as xmlReader
import sys

##
# @file xmlreader.py
# \~english 
# Xml file io


##
# \~english 
# Sort function for int property
def cmpFuncInt(self,right):
    return int(self.getproperty(self.sortpropertyname))-int(right.getproperty(right.sortpropertyname))
##
# \~english 
# Sort function for string property
def cmpFuncStr(self,right):
    if self.getproperty(self.sortpropertyname)<right.getproperty(right.sortpropertyname):
        return -1
    elif self.getproperty(self.sortpropertyname)==right.getproperty(right.sortpropertyname):
        return 0
    else:
        return 1
##
# \~english 
# @brief Xml node class
#
# This is the node element in a xml tree
class XmlNode(dict):
    def __init__(self,xmlnode):
        #lecture des propriétés
        self._properties={}
        self.sortpropertyname=None
        self.name=xmlnode.tagName
        prop=xmlnode.attributes.items()
        for propitem in prop:
            self._properties[propitem[0]]=propitem[1].encode("iso-8859-1")
        #lecture des éléments fils
        for child in xmlnode.childNodes:
            if hasattr(child,"tagName"):
                if not self.has_key(child.tagName):
                    self[child.tagName]=[]
                dict.__getitem__(self,child.tagName).append(XmlNode(child))

    ##
    # \~english
    # @return Return the string value of the node property. Empty string with error msg if not exist
    # @param propertyname Name of the property
    def getproperty(self, propertyname, default=""):
        return self._properties.get(propertyname, default)


    def addnode(self, name, value):
        if not self.has_key(name):
            self[name] = []
        dict.__getitem__(self, name).append(value)


    def setAttribute(self, name, value):
        self._properties[name] = value


    ##
    # \~english
    # @return Return true if this property exist
    # @param propertyname Name of the property
    def hasproperty(self,propertyname):
        return self._properties.has_key(propertyname)
    ##
    # \~english
    # @return Return the float value of the node property. 0 with error msg if not exist
    # @param propertyname Name of the property
    def getpropertyfloat(self,propertyname, default="0"):
        try:
            return float(self.getproperty(propertyname, str(default)))
        except ValueError:
            raise ValueError("Not parsable " + self.getproperty(propertyname))

    ##
    # \~english
    # @return Return the float value of the node property. 0 with error msg if not exist
    # @param propertyname Name of the property
    def getpropertyint(self, propertyname, default="0"):
        try:
            return int(self.getproperty(propertyname, str(default)))
        except ValueError:
            raise ValueError("Not parsable " + self.getproperty(propertyname))


    def __repr__(self):
        return "Node <%s/> with %i properties and %i childs" % (self.name,len(self._properties),len(self))
    ##
    # \~english
    # @param propertynamesort Name of the property
    # @param childsname Childs to sort
    # @param propertynamesort Sorting property of the child
    # @param ispropertyint Sort by integer value if the parameter is true
    def SortChildsByProperty(self,childsname,propertynamesort,ispropertyint=False):
        lst=self.lstnodesenum(childsname)
        for node in lst:
            node.sortpropertyname=propertynamesort
        if ispropertyint:
            lst.sort(cmpFuncInt)
        else:
            lst.sort(cmpFuncStr)
    
    ##
    # \~english
    # @param ind Child name to iterate over
    # @return iterator with child nodes
    def lstnodesenum(self,ind):
        """
            Retourne la liste des noeuds fils ayant ce nom
        """
        if self.has_key(ind):
            return dict.__getitem__(self,ind)
        else:
            return []
    ##
    # \~english
    # @param ind Child name
    # @return The first child with this name
    def __getitem__(self,ind):
        lst=dict.__getitem__(self,ind)
        return lst[0]
##
# \~english
# @param xmlfilepath The XML file path.
# @return The root node of the XML document.
def readXmlFile(xmlfilepath):
    doc=xmlReader.parse(xmlfilepath)
    return XmlNode(doc.childNodes[0])


def writeXmlFile(rootNode, xmlfilepath):
    impl = xmlReader.getDOMImplementation()
    doc = impl.createDocument(None, "none", None)
    # TODO add nodes
    doc.writexml(open(xmlfilepath, "w"))
    pass
