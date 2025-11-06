# -*- coding: UTF-8 -*-
import uictrl as ui
import uilocale
import os
import re
import requests

_ = uilocale.InstallUiModule(ui.application.getapplicationpath()["systemscript"]+"check_version"+os.sep,ui.application.getlocale())


def versionToCode(major, minor, revision):
    return major * 1000000 + minor * 1000 + revision


def check_version():
    visimpa = list(map(int, ui.application.getversion().split(".")))[:3]
    visimpa = versionToCode(visimpa[0], visimpa[1], visimpa[2])
    response = requests.get("https://api.github.com/repos/Universite-Gustave-Eiffel/I-Simpa/releases/latest")
    data = response.json()
    latest_tag = data['tag_name']
    major, minor, revision = map(int, latest_tag[1:].split("."))
    if visimpa < versionToCode(major, minor, revision):
            ui.application.loginfo(_("A new version of I-Simpa is available, please look at https://github.com/Universite-Gustave-Eiffel/I-Simpa/releases/latest"))
    return major, minor, revision

