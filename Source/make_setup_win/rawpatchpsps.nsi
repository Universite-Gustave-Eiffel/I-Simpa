;-------------------------------------------------------------------------------
;Some defines
;-------------------------------------------------------------------------------

; Where are the files to be installed located.
!define PATCH_SOURCE_ROOT "versionReleaseHistory\APPVERSION"

; Where are the "*.pat" files.
!define PATCH_FILES_ROOT "tmppatch"

; The default installation directory

InstallDirRegKey HKCU Software\I-SIMPA ""


; Directory to which the files will be installed.
!define PATCH_INSTALL_ROOT $INSTDIR

!include "patch.nsi"
!include "WordFunc.nsh"
!insertmacro WordReplace

;-------------------------------------------------------------------------------
; Installer fundamentals...
;------------------------------------------------------------------------------- 

; The name of the installer
Name "Patch I-SIMPA OLDVERSION->APPVERSION"

; The file to write
OutFile "..\..\versionReleaseHistory\I-SIMPA_Patch_OLDVERSION_to_APPVERSION.exe"

; Show details
ShowInstDetails show

;-------------------------------------------------------------------------------
;Stuff to install
;------------------------------------------------------------------------------- 


Section "Test Installer Core"

  SectionIn RO
  
  SetOutPath "$INSTDIR"
  
  Var /GLOBAL newstartfolder

  ReadRegStr $0 HKCU Software\I-SIMPA "Start Menu Folder"
  StrCpy $newstartfolder $0
  ${WordReplace} '$0' 'OLDVERSION' 'APPVERSION' '+*' $newstartfolder
  Rename "$SMPROGRAMS\$0" "$SMPROGRAMS\$newstartfolder"
  WriteRegStr HKCU Software\I-SIMPA "Start Menu Folder" "$newstartfolder"

  
  Call patchFilesRemoved
  Call patchDirectoriesRemoved
  Call patchDirectoriesAdded
  Call patchFilesAdded
  Call patchFilesModified

SectionEnd


