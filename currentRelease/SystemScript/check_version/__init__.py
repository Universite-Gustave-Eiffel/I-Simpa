# -*- coding: UTF-8 -*-
import uictrl as ui
import uilocale
import urllib2
import os
import re


_=uilocale.InstallUiModule(ui.application.getapplicationpath()["systemscript"]+"check_version"+os.sep,ui.application.getlocale())

def check_version():
    visimpa = map(int, ui.application.getversion().split("."))[:3]

    # https://raw.githubusercontent.com/Ifsttar/I-Simpa/master/CMakeLists.txt
    u = urllib2.urlopen("https://raw.githubusercontent.com/Ifsttar/I-Simpa/master/CMakeLists.txt", timeout=5)
    p = re.compile("^project \(isimpa VERSION (\d+\.)?(\d+\.)?(\*|\d+)")

    for l in u.readlines():
        r = p.match(l)
        if r is not None:
            major = int(r.group(1)[:1])
            minor = int(r.group(2)[:1])
            revision = int(r.group(3))
            if major > visimpa[0] or minor > visimpa[1] or revision > visimpa[2]:
                    ui.application.loginfo(_("A new version of I-Simpa is available, please look at https://github.com/Ifsttar/I-Simpa/releases/latest"))
            return major, minor, revision
    

