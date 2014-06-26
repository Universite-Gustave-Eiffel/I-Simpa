import traceback
import sys

def dummy_notranslation(msg):
    return msg


def InstallUiModule(pathname,language):
    import gettext
    localdir=pathname + "\\locale"
    
    try:
        locale_dict=gettext.translation("messages",localdir,languages=[language])
    except IOError:
        sys.stderr.write("Language %s is not avaible for this module :\n%s\n" % (language,pathname))
        exceptionType, exceptionValue, exceptionTraceback = sys.exc_info()
        traceback.print_exception(exceptionType, exceptionValue, exceptionTraceback)
        return dummy_notranslation
    return locale_dict.ugettext
