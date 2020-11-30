# -*- coding: UTF-8 -*-
import uictrl as ui
import uilocale
import urllib.request
import os
import re

_ = uilocale.InstallUiModule(ui.application.getapplicationpath()["systemscript"]+"check_version"+os.sep,ui.application.getlocale())


def versionToCode(major, minor, revision):
    return major * 1000000 + minor * 1000 + revision


def check_version():
    visimpa = list(map(int, ui.application.getversion().split(".")))[:3]
    visimpa = versionToCode(visimpa[0], visimpa[1], visimpa[2])

    with urllib.request.urlopen("https://raw.githubusercontent.com/Ifsttar/I-Simpa/master/CMakeLists.txt") as response:
        html = response.read().decode("utf-8")
        p = re.compile("^project \(isimpa VERSION (\d+\.)?(\d+\.)?(\*|\d+)")

        for line in html.split("\n"):
            r = p.match(line)
            if r is not None:
                major = int(r.group(1)[:1])
                minor = int(r.group(2)[:1])
                revision = int(r.group(3))
                if visimpa < versionToCode(major, minor, revision):
                        ui.application.loginfo(_("A new version of I-Simpa is available, please look at https://github.com/Ifsttar/I-Simpa/releases/latest"))
                return major, minor, revision
    return 0, 0, 0

