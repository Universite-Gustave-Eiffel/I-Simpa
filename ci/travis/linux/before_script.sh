# Update translation key files

find src/isimpa -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' > files.txt
find src/spps -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' >> files.txt
find src/theorie_classique -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' >> files.txt
find src/lib_interface -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' >> files.txt

mkdir -p src/isimpa/lang/
xgettext --keyword=_ --keyword=wxTRANSLATE --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -osrc/isimpa/lang/internat.pot
msginit -o src/isimpa/lang/internat.pot -l en.UTF-8

find currentRelease/UserScript/job_tool -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/job_tool/internat.pot
msginit -o currentRelease/UserScript/job_tool/internat.pot -l en.UTF-8

find currentRelease/UserScript/moveto_vertex -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/moveto_vertex/internat.pot
msginit -o currentRelease/UserScript/moveto_vertex/internat.pot -l en.UTF-8

find currentRelease/UserScript/preceiv_sourceTracker -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/preceiv_sourceTracker/internat.pot
msginit -o currentRelease/UserScript/preceiv_sourceTracker/internat.pot -l en.UTF-8

find currentRelease/UserScript/recp_tool -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/recp_tool/internat.pot
msginit -o currentRelease/UserScript/recp_tool/internat.pot -l en.UTF-8

find currentRelease/UserScript/source_tools -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/UserScript/source_tools/internat.pot
msginit -o currentRelease/UserScript/source_tools/internat.pot -l en.UTF-8
