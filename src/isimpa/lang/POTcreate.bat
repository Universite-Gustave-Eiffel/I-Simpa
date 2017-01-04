cd ..
cd ..

dir /S /B *.c *.cpp *.h *.hpp > isimpa\lang\files.txt

cd isimpa\lang\
xgettext --keyword=_ --keyword=wxTRANSLATE  --from-code=UTF-8 -s --no-wrap  -no-hash --escape -ffiles.txt -ointernat.pot

pause