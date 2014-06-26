;NSIS Modern User Interface
;Multilingual I-SIMPA 


;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Include Modern UI
  !define MULTIUSER_EXECUTIONLEVEL Highest
  !define MULTIUSER_MUI
  !define MULTIUSER_INSTALLMODE_COMMANDLINE
  !include "MultiUser.nsh"
  !include "MUI2.nsh"
  !include "usermacro.nsi"
  !include "Sections.nsh"
;--------------------------------
;General

  ;Name and file
  Name "I-SIMPA APPVERSION"
  OutFile "..\..\versionReleaseHistory\Setup_I-SIMPA_APPVERSION.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\I-SIMPA"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\I-SIMPA" ""

  !define SelectSection '!insertmacro SelectSection'
  !define UnSelectSection '!insertmacro UnselectSection'

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\I-SIMPA" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "..\currentRelease\Licence.rtf"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MULTIUSER_PAGE_INSTALLMODE
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\I-SIMPA" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "French" ;first language is the default language
  !insertmacro MUI_LANGUAGE "English"
;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
 !insertmacro MUI_RESERVEFILE_LANGDLL
;--------------------------------
;Installer Sections

Section "-I-SIMPA" SecCore

  SetOutPath "$INSTDIR"
  ;ADD YOUR OWN FILES HERE...
  File /r /x python-2.6.4.msi ..\currentRelease\*.*

  ;Store installation folder
  WriteRegStr HKCU "Software\I-SIMPA" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\I-SIMPA.lnk" "$INSTDIR\I-SIMPA.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\UserScript.lnk" "$INSTDIR\UserScript\"
    CreateShortCut "$DESKTOP\I-SIMPA.lnk" "$INSTDIR\I-SIMPA.exe" ""

  
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Python 2.6" SecPython

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...
  ;File currentRelease\Python26.dll
  File ..\currentRelease\python26_LICENSE.txt
  File ..\currentRelease\python-2.6.4.msi
  ExecWait '"msiexec" /i "$INSTDIR\python-2.6.4.msi"'
  Delete "$INSTDIR\python-2.6.4.msi"

SectionEnd
;--------------------------------
;Installer Functions

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY
  
  ; Test if previous version installed
  
  ReadRegStr $0 HKCU Software\I-SIMPA ""
  IfFileExists $0 0 oktoinstall
    MessageBox MB_OK "$(messageuninst)"
    Abort "$(messageuninst)"
  oktoinstall:
  
  ; Test if Python 2.6 is already installed
  ReadRegStr $0 HKLM SOFTWARE\Python\PythonCore\2.6\PythonPath ""
  StrCmp $0 "" pythonisnotinstalled
  ${UnSelectSection} ${SecPython}
  Goto defaultchoiceok
  pythonisnotinstalled:
  ${SelectSection} ${SecPython}
  defaultchoiceok:
  
  !insertmacro MULTIUSER_INIT
FunctionEnd

;--------------------------------
;Descriptions

  ;USE A LANGUAGE STRING IF YOU WANT YOUR DESCRIPTIONS TO BE LANGUAGE SPECIFIC
LangString messageuninst ${LANG_ENGLISH} "A previous version of I-SIMPA is already installed, please uninstall I-SIMPA before run this setup."
LangString messageuninst ${LANG_FRENCH} "I-SIMPA est déjà installé sur cet ordinateur, veuillez désinstaller I-SIMPA avant d'éxecuter cette installation."
LangString message ${LANG_ENGLISH} "If you uncheck this option then you must download and install Python 2.6. I-SIMPA can't work without Python."
LangString message ${LANG_FRENCH} "Si vous ne désirez pas installer ce module alors vous devez obligatoirement télécharger et installer Python version 2.6 pour que I-SIMPA fonctionne."

  ;Assign descriptions to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPython} "$(message)"
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  RMDir /r "$SMPROGRAMS\$StartMenuFolder"
  Delete "$DESKTOP\I-SIMPA.lnk"
  
  DeleteRegKey /ifempty HKCU "Software\I-SIMPA"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  !insertmacro MULTIUSER_UNINIT

FunctionEnd