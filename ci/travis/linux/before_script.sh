#!/usr/bin/env bash
# Update translation key files

find src/isimpa -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' > files.txt
find src/spps -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' >> files.txt
find src/theorie_classique -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' >> files.txt
find src/lib_interface -type f -regex '.*/.*\.\(c\|cpp\|h\|hpp\)$' >> files.txt

mkdir -p src/isimpa/lang/
xgettext --keyword=_ --keyword=wxTRANSLATE --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -osrc/isimpa/lang/internat.pot
msginit --no-translator --input src/isimpa/lang/internat.pot -o src/isimpa/lang/internat.pot -l en.UTF-8

find currentRelease/SystemScript/job_tool -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/SystemScript/job_tool/internat.pot
msginit --no-translator --input currentRelease/SystemScript/job_tool/internat.pot -o currentRelease/SystemScript/job_tool/internat.pot -l en.UTF-8

find currentRelease/SystemScript/moveto_vertex -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/SystemScript/moveto_vertex/internat.pot
msginit --no-translator --input currentRelease/SystemScript/moveto_vertex/internat.pot -o currentRelease/SystemScript/moveto_vertex/internat.pot -l en.UTF-8

find currentRelease/SystemScript/preceiv_sourceTracker -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/SystemScript/preceiv_sourceTracker/internat.pot
msginit --no-translator --input currentRelease/SystemScript/preceiv_sourceTracker/internat.pot -o currentRelease/SystemScript/preceiv_sourceTracker/internat.pot -l en.UTF-8

find currentRelease/SystemScript/recp_tool -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/SystemScript/recp_tool/internat.pot
msginit --no-translator --input currentRelease/SystemScript/recp_tool/internat.pot -o currentRelease/SystemScript/recp_tool/internat.pot -l en.UTF-8

find currentRelease/SystemScript/source_tools -type f -name "*.py" > files.txt
xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/SystemScript/source_tools/internat.pot
msginit --no-translator --input currentRelease/SystemScript/source_tools/internat.pot -o currentRelease/SystemScript/source_tools/internat.pot -l en.UTF-8

# Disabled
#find currentRelease/SystemScript/SppsReportSample -type f -name "*.py" > files.txt
#xgettext --keyword=_  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ocurrentRelease/SystemScript/SppsReportSample/internat.pot
#msginit --no-translator --input currentRelease/SystemScript/SppsReportSample/internat.pot -o currentRelease/SystemScript/SppsReportSample/internat.pot -l en.UTF-8

# Now replace all ASCII charset by UTF-8 in pot files
find . -type f -name "*.pot" -exec sed -i 's/charset=ASCII/charset=UTF-8/g' {} +

# Push translation keys to transifex

if [ -z "$TRANSIFEXPWD" ]; then
    echo "Not in master branch do not push transifex keys"
else
    # Write transifex config file
    printf "[https://www.transifex.com]\nhostname = https://www.transifex.com\npassword = $TRANSIFEXPWD\ntoken =\nusername = travis_lae\n" > ~/.transifexrc
    # Update setup-tools
    pip install setuptools --upgrade
    # install transifex client
    pip install --user transifex-client
    # push transifex keys
    tx push -s
fi
