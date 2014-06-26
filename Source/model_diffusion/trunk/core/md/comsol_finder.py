# -*- coding: cp1252 -*-

##
# @file comsol_finder.py
# \~english 
# @brief Search comsol path within the windows registry
#
# Explore the windows registry node and give the state of Comsol installation


from __future__ import print_function #compatibilité python 3.0
import _winreg as reg

##
# \~english
# @param root A _winreg.HKEY_ enum
# @param path A string like "SOFTWARE/Comsol"
# Traverse path within the root node
#

def GetRegKey( root, path):
    lpath=path.split("/")
    curkey=root
    while len(lpath)>0:
        curkey=reg.OpenKey(curkey,lpath.pop(0))
    return curkey
##
# \~english 
# @param root A _winreg.HKEY_ enum
# @param path A string like "SOFTWARE/Comsol"
# @param propertyname Name of the key
# @return The registry key value
def GetRegKeyValue( root, path, propertyname ):
    return reg.QueryValueEx(GetRegKey(root,path),propertyname)


##
# \~english 
# @param root A _winreg.HKEY_ enum
# @param path A string like "SOFTWARE/Comsol"
# @return A list containing all the sub-node of the node at path
def GetSubKeys( root, path ):
    res=[]
    key=GetRegKey( root, path)
    infos=reg.QueryInfoKey(key)
    for i in range(0,infos[0]):
        res.append(reg.EnumKey( key, i))
    return res

##
# \~english 
# @param root A _winreg.HKEY_ enum
# @param path A string like "SOFTWARE/Comsol"
# @return A list containing all the sub-node of the node at path
def GetKeyValues( root, path ):
    res={}
    key=GetRegKey( root, path)
    infos=reg.QueryInfoKey(key)
    for i in range(0,infos[1]):
        rec=reg.EnumValue( key, i)
        res[rec[0]]=rec[1:]
    return res


##
# \~english 
# Search comsol path within the windows registry and return all results
# @return A list [ { "version" : version(integer), "name" : nom(string), "path" : exe_path(string) }, .. ]
def GetComSolInfos():
    """
        Retourne les informations sur les installations de comsol
        [ { "version" : version(integer),
            "name" : nom(string),
            "path" : exe_path(string)
          }
          ,
          ...
        ]
    """
    comsolpath="SOFTWARE/Comsol"
    result=[]
    try:
        comsol_versions=GetSubKeys(reg.HKEY_LOCAL_MACHINE,comsolpath)
    except:
        return []
    for version in comsol_versions:
        try:
            curinfos={}
            curinfos["version"]=int(version[version.rfind("L")+1:])
            curinfos["name"]=version
            curinfos["path"]=GetRegKeyValue(reg.HKEY_LOCAL_MACHINE,comsolpath+"/"+version,"COMSOLROOT")[0]+"\\bin\\comsolbatch.exe"
            result.append(curinfos)
        except:
            pass
    return result

##
# \~english 
# Search comsol path within the windows registry and return the more updated version
# @return A dictionnary { "version" : version(integer), "name" : nom(string), "path" : exe_path(string) }
def GetLatestComSolInfos():
    """
        Retourne les informations sur l'installation de comsol la plus récente
        { "version" : version(integer),
            "name" : nom(string),
            "path" : exe_path(string)
          }
    """
    infos=GetComSolInfos()
    if len(infos)==0:
        return None
    if len(infos)==1:
        return infos[0]
    else:
        print("Plusieurs versions de comsol ont été trouvées, utilisation de la dernière version.")
        lastversion=infos[0]
        last=infos[0]["version"]
        for com in infos:
            if com["version"]>last:
                lastversion=com
                last=com["version"]
        return lastversion
        
