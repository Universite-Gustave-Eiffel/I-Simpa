dir /S /B *.py > files.txt
mkdir locale\en\
xgettext --keyword=_ --keyword=wxTRANSLATE  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -olocale\en\internat.pot

pause