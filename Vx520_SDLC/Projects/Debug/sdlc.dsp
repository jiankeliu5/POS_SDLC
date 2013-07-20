# Microsoft Developer Studio Project File - Name="sdlc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=sdlc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sdlc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdlc.mak" CFG="sdlc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdlc - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /f sdlc.MAK"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "sdlc.EXE"
# PROP BASE Bsc_Name "sdlc.BSC"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "NMAKE /f sdlc.smk"
# PROP Rebuild_Opt "/a"
# PROP Target_File "sdlc.EXE"
# PROP Bsc_Name "sdlc.BSC"
# PROP Target_Dir ""
# Begin Target

# Name "sdlc - Win32 Debug"

!IF  "$(CFG)" == "sdlc - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "Porting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\porting\comio.GprsEth.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\porting\GprsEth.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\porting\modem.GprsEth.c
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Output\Download\d.bat
# End Source File
# Begin Source File

SOURCE=..\..\Output\Download\sdlc.dld
# End Source File
# Begin Source File

SOURCE=.\sdlc.smk
# End Source File
# End Group
# End Target
# End Project
