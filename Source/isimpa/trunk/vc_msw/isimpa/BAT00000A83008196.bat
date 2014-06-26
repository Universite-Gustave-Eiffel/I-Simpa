@echo off
cd C:\Users\Jimmy\I-Simpa\Source\isimpa\trunk\data_manager\python_interface\py_ui_module\
python generate_enum_def.py

if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: Un outil a retourn‚ un code d'erreurÿ… partir de "Ex‚cution d'un ‚v‚nement avant g‚n‚ration..."
exit 1
:VCEnd