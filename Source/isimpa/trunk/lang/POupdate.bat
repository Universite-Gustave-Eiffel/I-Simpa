cd ..
dir /B /S *.c* > files.txt
"C:\Program Files\GnuWin32\bin\xgettext" --from-code=ISO-8859-15 -a --no-location -s -j --no-wrap --escape -ffiles.txt -olang/internat.po

pause