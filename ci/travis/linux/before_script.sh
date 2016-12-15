# Update translation key files

find src/isimpa -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' > files.txt
mkdir -p src/isimpa/lang/
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -osrc/isimpa/lang/internat.pot

find currentRelease/UserScript/job_tool -type f -name "*.py" > files.txt
mkdir -p currentRelease/UserScript/job_tool/locale/en/
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/job_tool/locale/en/internat.pot

find currentRelease/UserScript/moveto_vertex -type f -name "*.py" > files.txt
mkdir -p currentRelease/UserScript/moveto_vertex/locale/en/
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/moveto_vertex/locale/en/internat.pot

find currentRelease/UserScript/preceiv_sourceTracker -type f -name "*.py" > files.txt
mkdir -p currentRelease/UserScript/preceiv_sourceTracker/locale/en/
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/preceiv_sourceTracker/locale/en/internat.pot

find currentRelease/UserScript/recp_tool -type f -name "*.py" > files.txt
mkdir -p currentRelease/UserScript/recp_tool/locale/en/
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/recp_tool/locale/en/internat.pot

find currentRelease/UserScript/source_tools -type f -name "*.py" > files.txt
mkdir -p currentRelease/UserScript/source_tools/locale/en/
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/source_tools/locale/en/internat.pot
