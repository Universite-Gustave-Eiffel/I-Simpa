# -*- coding: cp1252 -*-
import time

endlinearr=[",","/*","="]

cppFile="""
//Généré par generate_enum_def.py le datestamp
#include "element_pywrap.hpp"
#include "data_manager/python_interface/instanceManager.hpp"
#include "data_manager/element.h"
#include <boost/python/enum.hpp>


using namespace boost::python;

void export_enum()
{
    EvtEnum

    ElTypeEnum
}

"""


def GetFirstOccurance(line, endlinearr):
    nearest=-1
    for ele in endlinearr:
        addr=line.find(ele)
        if addr!=-1 and (addr<nearest or nearest==-1):
            nearest=addr
    if nearest==-1:
        nearest=len(line)
    return nearest
def GetIMacro(line):
    line_spl=line.split("=")
    return line_spl[1][0:-1]
        
def GetVersion(pspsRoot):
    appconfigelement=open(pspsRoot+"data_manager/appconfig.cpp",'r')
    stateParsing="seek"
    version=[0,0,0]
    buff=""
    for line in appconfigelement:
        if line.find("SPPS_UI_VERSION_MAJOR")!=-1:
            version[0]=GetIMacro(line[:-1])
        if line.find("SPPS_UI_VERSION_MINOR")!=-1:
            version[1]=GetIMacro(line[:-1])
        if line.find("SPPS_UI_VERSION_REVISION")!=-1:
            version[2]=GetIMacro(line[:-1])
    appconfigelement.close()
    chainversion=str(version[0])+"."+str(version[1])+"."+str(version[2])
    return chainversion
