import traceback
import sys
import os

def dummy_notranslation(msg):
    return msg


def InstallUiModule(pathname,language):
    import gettext
    localdir=pathname + os.sep + "locale"

    try:
        locale_dict=gettext.translation("internat",localdir,languages=[language],fallback=True)
    except IOError:
        sys.stderr.write("Language %s is not avaible for this module :\n%s\n" % (language,pathname))
        exceptionType, exceptionValue, exceptionTraceback = sys.exc_info()
        traceback.print_exception(exceptionType, exceptionValue, exceptionTraceback)
        return dummy_notranslation
    return locale_dict.gettext
