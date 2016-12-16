cd ..

dir /S /B *.c* *.h* > files.txt

xgettext --keyword=_ --keyword=wxTRANSLATE  --from-code=ISO-8859-15 -s --no-wrap  -no-hash --escape -ffiles.txt -olang\internat.pot

pause