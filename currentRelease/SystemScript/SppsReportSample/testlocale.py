import os,sys
import gettext
pathname= os.path.dirname(sys.argv[0])
localdir=os.path.abspath(pathname) + "\\locale"
fr_dict=gettext.translation("messages",localdir,languages=['fr'])
fr_dict.install(unicode=1)

print _("Make report")
input()