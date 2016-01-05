cd ..
dir /S /B *.c* > files.txt
"C:\Program Files\GnuWin32\bin\xgettext" --keyword=_ --debug --from-code=ISO-8859-15 -s --no-wrap --escape -ffiles.txt -olang\internat.po
