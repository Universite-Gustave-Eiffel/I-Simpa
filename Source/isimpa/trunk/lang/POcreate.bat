cd ..
dir /S /B *.c* *.h > files.txt
"C:\Program Files\GnuWin32\bin\xgettext" --keyword=_  --from-code=ISO-8859-15 -s --no-wrap  -no-hash --escape -ffiles.txt -olang\internat.po
pause